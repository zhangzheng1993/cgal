// ============================================================================
//
// Copyright (c) 1998 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------------
//
// release       :
// release_date  :
//
// file          : include/CGAL/Triangulation_vertex_3.h
// revision      : $Revision$
// author(s)     : Monique Teillaud <Monique.Teillaud@sophia.inria.fr>
//
// coordinator   : Mariette Yvinec  <Mariette.Yvinec@sophia.inria.fr>
//
// ============================================================================

#ifndef CGAL_TRIANGULATION_VERTEX_3_H
#define CGAL_TRIANGULATION_VERTEX_3_H

#include <CGAL/Pointer.h>
#include <CGAL/Triangulation_short_names_3.h>
#include <CGAL/Triangulation_data_structure_3.h>
#include <CGAL/Triangulation_circulators_3.h>


template < class Gt, class Tds >
class CGAL_Triangulation_cell_3;

template < class Gt, class Tds >
class CGAL_Triangulation_vertex_handle_3;

template < class Gt, class Tds >
class CGAL_Triangulation_cell_handle_3;

// template<class Gt,  class Tds>
// class CGAL_Triangulation_cell_circulator_3;

// template<class Gt,  class Tds>
// class CGAL_Triangulation_vertex_circulator_3;

// template<class Gt, class Tds>
// class CGAL_Triangulation_edge_circulator_3;

template<class Gt, class Tds >
class CGAL_Triangulation_vertex_3
  : public Tds::Vertex
{
public:
  
  typedef typename Gt::Point Point;

  typedef typename Tds::Vertex Vtds;
  typedef typename Tds::Cell Ctds;

  typedef CGAL_Triangulation_cell_3<Gt,Tds> Cell;
  typedef CGAL_Triangulation_vertex_3<Gt,Tds> Vertex;
  
  typedef CGAL_Triangulation_vertex_handle_3<Gt,Tds> Vertex_handle;
  typedef CGAL_Triangulation_cell_handle_3<Gt,Tds> Cell_handle;
  //  typedef pair<Cell_handle, int, int>     Edge;

  //  typedef CGAL_Triangulation_cell_circulator_3<Gt,Tds>      Cell_circulator;
  //  typedef CGAL_Triangulation_edge_circulator_3<Gt,Tds>      Edge_circulator;
  //  typedef CGAL_Triangulation_vertex_circulator_3<Gt,Tds>    Vertex_circulator;

  
  CGAL_Triangulation_vertex_3()
     : Vtds()
  {}

  CGAL_Triangulation_vertex_3(const Point & p)
    :  Vtds(p)
  {}
    
//   CGAL_Triangulation_vertex_3(const Point & p, Cell_handle c)
//     :  Vtds(p, &(*c))
//   {}

  inline void set_cell(const Cell_handle & c)
  {
    Vtds::set_cell(&(*c));
  }
    
  inline Cell_handle cell() const
  {
    return (Cell *) Vtds::cell();
  }
        
  inline Vertex_handle handle() const
  {
    return Vertex_handle(this);
  }

//   inline Vertex_circulator incident_vertices()
//   {
//     return Vertex_circulator(handle(), cell());
//   }

//   inline Vertex_circulator incident_vertices(const Cell_handle& c)
//   {
//     return Vertex_circulator(handle(), c);
//   } 

//   inline 
//   Cell_circulator incident_cells()
//   {
//     return Cell_circulator(handle(), cell());
//   }
    
//   inline Cell_circulator incident_cells(const Cell_handle& c)
//   {
//     return Cell_circulator(handle(), c);
//   }
    
//   inline Edge_circulator incident_edges()
//   {
//     return Edge_circulator(handle(), cell());
//   }
 
//   inline Edge_circulator incident_edges(const Cell_handle& c)
//   {
//     return Edge_circulator(handle(), c);
//   }
    
};
#endif CGAL_TRIANGULATION_VERTEX_3_H
