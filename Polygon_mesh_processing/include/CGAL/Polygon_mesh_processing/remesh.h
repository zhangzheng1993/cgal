// Copyright (c) 2015 GeometryFactory (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
//
// Author(s)     : Jane Tournois

#ifndef CGAL_POLYGON_MESH_PROCESSING_REMESH_H
#define CGAL_POLYGON_MESH_PROCESSING_REMESH_H

#include <CGAL/Polygon_mesh_processing/internal/remesh_impl.h>

#include <CGAL/Polygon_mesh_processing/internal/named_function_params.h>
#include <CGAL/Polygon_mesh_processing/internal/named_params_helper.h>

namespace CGAL {

namespace Polygon_mesh_processing {

/*!
* \ingroup PMP_meshing_grp
* @brief remeshes a triangulated region of a polygon mesh.
* This operation sequentially performs edge splits, edge collapses,
* edge flips, Laplacian smoothing and projection to the initial surface
* to generate a smooth mesh with a prescribed edge length.
*
* @tparam PolygonMesh model of `MutableFaceGraph` that 
*         has an internal property map for `CGAL::vertex_point_t`.
* @tparam FaceRange range of `boost::graph_traits<PolygonMesh>::%face_descriptor`,
          model of `Range`. Its iterator type is `InputIterator`.
* @tparam NamedParameters a sequence of \ref namedparameters
*
* @param pmesh a polygon mesh with triangulated surface patches to be remeshed
* @param faces the range of triangular faces defining one or several surface patches to be remeshed
* @param target_edge_length the edge length that is targetted in the remeshed patch
* @param np optional sequence of \ref namedparameters among the ones listed below
*
* @pre if constraints protection is activated, the constrained edges should
* not be longer than 4/3*`target_edge_length`
*
* \cgalNamedParamsBegin
*  \cgalParamBegin{vertex_point_map} the property map with the points associated
*    to the vertices of `pmesh`. Instance of a class model of `ReadWritePropertyMap`.
*  \cgalParamEnd
*  \cgalParamBegin{number_of_iterations} the number of iterations for the
*    sequence of atomic operations performed (listed in the above description)
*  \cgalParamEnd
*  \cgalParamBegin{geom_traits} a geometric traits class instance, model of `Kernel`
*  \cgalParamEnd
*  \cgalParamBegin{edge_is_constrained_map} a property map containing the
*    constrained-or-not status of each edge of pmesh. A constrained edge can be splitted
*    or collapsed, but not flipped, nor its endpoints moved by smoothing.
*    Note that patch boundary edges (i.e. incident to only one face in the range)
*    are always considered as constrained edges.
*  \cgalParamEnd
*  \cgalParamBegin{protect_constraints} If `true`, the edges set as constrained
*     in `edge_is_constrained_map` (or by default the boundary edges)
*     are not splitted nor collapsed during remeshing.
*     Note that around constrained edges that have their length higher than
*     twice `target_edge_length`, remeshing will fail to provide
*     good quality results. It can even fail to terminate because of cascading vertex
*     insertions.
*  \cgalParamEnd
* \cgalNamedParamsEnd
*
* @sa `split_long_edges()`
*
*@todo add possibility to provide a functor that projects to a prescribed surface
*/
template<typename PolygonMesh
       , typename FaceRange
       , typename NamedParameters>
void isotropic_remeshing(const FaceRange& faces
                       , const double& target_edge_length
                       , PolygonMesh& pmesh
                       , const NamedParameters& np)
{
  typedef PolygonMesh PM;
  using boost::choose_pmap;
  using boost::get_param;
  using boost::choose_param;

  typedef typename GetGeomTraits<PM, NamedParameters>::type GT;

  typedef typename GetVertexPointMap<PM, NamedParameters>::type VPMap;
  VPMap vpmap = choose_pmap(get_param(np, boost::vertex_point),
                            pmesh,
                            boost::vertex_point);

  typedef typename boost::lookup_named_param_def <
      CGAL::edge_is_constrained_t,
      NamedParameters,
      internal::Border_constraint_pmap<PM, FaceRange>//default
    > ::type ECMap;
  ECMap ecmap
    = choose_param(get_param(np, edge_is_constrained),
                   internal::Border_constraint_pmap<PM, FaceRange>(pmesh, faces));

  double low = 4. / 5. * target_edge_length;
  double high = 4. / 3. * target_edge_length;

  bool protect = choose_param(get_param(np, protect_constraints), false);
  if(protect)
  {
    std::string msg("Isotropic remeshing : protect_constraints cannot be set to");
    msg.append(" true with constraints larger than 4/3 * target_edge_length.");
    msg.append(" Remeshing aborted.");
    CGAL_precondition_msg(
      internal::constraints_are_short_enough(pmesh, ecmap, vpmap, high),
      msg.c_str());
  }

  typename internal::Incremental_remesher<PM, VPMap, GT>
    remesher(pmesh, vpmap, protect);
  remesher.init_remeshing(faces, ecmap);

  unsigned int nb_iterations = choose_param(get_param(np, number_of_iterations), 1);

#ifdef CGAL_PMP_REMESHING_VERBOSE
  std::cout << std::endl;
  std::cout << "Remeshing (size = " << target_edge_length;
  std::cout << ", #iter = " << nb_iterations << ")..." << std::endl;
#endif

  for (unsigned int i = 0; i < nb_iterations; ++i)
  {
#ifdef CGAL_PMP_REMESHING_VERBOSE
    std::cout << " * Iteration " << (i + 1) << " *" << std::endl;
#endif

    remesher.split_long_edges(high);
    remesher.collapse_short_edges(low, high);
    remesher.equalize_valences();
    remesher.tangential_relaxation();
    remesher.project_to_surface();

#ifdef CGAL_PMP_REMESHING_VERBOSE
    std::cout << std::endl;
#endif
  }

#ifdef CGAL_PMP_REMESHING_VERBOSE
  std::cout << "Remeshing done (size = " << target_edge_length;
  std::cout << ", #iter = " << nb_iterations << ")." << std::endl;
#endif
}

template<typename PolygonMesh
       , typename FaceRange>
void isotropic_remeshing(
    const FaceRange& faces
  , const double& target_edge_length
  , PolygonMesh& pmesh)
{
  isotropic_remeshing(
    faces,
    target_edge_length,
    pmesh,
    parameters::all_default());
}

/*!
* \ingroup PMP_meshing_grp
* @brief splits the edges listed in `edges` into sub-edges
* that are not longer than the given threshold `max_length`.
*
* Note this function is useful to split constrained edges before
* calling `isotropic_remeshing()` with protection of constraints
* activated (to match the constrained edge length required by the
* remeshing algorithm to be guaranteed to terminate)
*
* @tparam PolygonMesh model of `MutableFaceGraph` that
*         has an internal property map for `CGAL::vertex_point_t`.
* @tparam EdgeRange range of `boost::graph_traits<PolygonMesh>::%edge_descriptor`,
*   model of `Range`. Its iterator type is `InputIterator`.
* @tparam NamedParameters a sequence of \ref namedparameters
*
* @param pmesh a polygon mesh
* @param edges the range of edges to be split if they are longer than given threshold
* @param max_length the edge length above which an edge from `edges` is split
*        into to sub-edges
* @param np optional \ref namedparameters described below

* \cgalNamedParamsBegin
*  \cgalParamBegin{vertex_point_map} the property map with the points associated
*    to the vertices of `pmesh`. Instance of a class model of `ReadWritePropertyMap`.
*  \cgalParamEnd
* \cgalNamedParamsEnd
*
* @sa `isotropic_remeshing()`
*
*/
template<typename PolygonMesh
       , typename EdgeRange
       , typename NamedParameters>
void split_long_edges(const EdgeRange& edges
                    , const double& max_length
                    , PolygonMesh& pmesh
                    , const NamedParameters& np)
{
  typedef PolygonMesh PM;
  using boost::choose_pmap;
  using boost::get_param;

  typedef typename GetGeomTraits<PM, NamedParameters>::type GT;
  typedef typename GetVertexPointMap<PM, NamedParameters>::type VPMap;
  VPMap vpmap = choose_pmap(get_param(np, boost::vertex_point),
                            pmesh,
                            boost::vertex_point);

  typename internal::Incremental_remesher<PM, VPMap, GT>
    remesher(pmesh, vpmap, false/*protect constraints*/);

  remesher.split_long_edges(edges, max_length, Emptyset_iterator());
}

template<typename PolygonMesh, typename EdgeRange>
void split_long_edges(const EdgeRange& edges
                    , const double& max_length
                    , PolygonMesh& pmesh)
{
  split_long_edges(edges,
    max_length,
    pmesh,
    parameters::all_default());
}

//used in the Polyhedron demo
template<typename PolygonMesh
       , typename EdgeRange
       , typename OutputIterator
       , typename NamedParameters>
void split_long_edges(
          const EdgeRange& edge_range
        , const double& max_length
        , PolygonMesh& pmesh
        , OutputIterator out//edges after splitting, all shorter than target_length
        , const NamedParameters& np)
{
  typedef PolygonMesh PM;
  using boost::choose_pmap;
  using boost::get_param;

  typedef typename GetGeomTraits<PM, NamedParameters>::type GT;
  typedef typename GetVertexPointMap<PM, NamedParameters>::type VPMap;
  VPMap vpmap = choose_pmap(get_param(np, boost::vertex_point),
                            pmesh,
                            boost::vertex_point);

  typename internal::Incremental_remesher<PM, VPMap, GT>
    remesher(pmesh, vpmap, false/*protect constraints*/);

  remesher.split_long_edges(edge_range, max_length, out);
}

} //end namespace Polygon_mesh_processing
} //end namespace CGAL

#endif