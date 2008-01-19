// TODO: Add licence
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL:$
// $Id: $
// 
//
// Author(s)     : Eric Berberich <eric@mpi-inf.mpg.de>
//                 Pavel Emeliyanenko <asm@mpi-sb.mpg.de>
//
// ============================================================================

#ifndef CGAL_CURVED_KERNEL_POINT_2_H
#define CGAL_CURVED_KERNEL_POINT_2_H

/*! \file Curved_kernel_via_analysis_2/Point_2.h
 *  \brief defines class \c Point_2
 *  
 *  Point on a generic curve
 */

#include <CGAL/basic.h>
#include <CGAL/Handle_with_policy.h>
#include <boost/optional.hpp>

#include <CGAL/Arr_enums.h>

#include <CGAL/Curved_kernel_via_analysis_2/Curved_kernel_via_analysis_2_functors.h>

CGAL_BEGIN_NAMESPACE

namespace CGALi {

//! forward class declaration
template < class CurvedKernelViaAnalysis_2, class Rep_ > 
class Point_2;

template < class CurvedKernelViaAnalysis_2 >
class Arc_2_rep;

//! forward class declaration for befriending
template < class CurvedKernelViaAnalysis_2, class Rep_ =
    Arc_2_rep< CurvedKernelViaAnalysis_2 > > 
class Arc_2;

template < class CurvedKernelViaAnalysis_2, class Rep_ > 
std::ostream& operator<< (std::ostream&,
    const Point_2< CurvedKernelViaAnalysis_2, Rep_ >&);

template <class CurvedKernelViaAnalysis_2>
class Point_2_rep 
{
public:
    //! this instance's template parameter
    typedef CurvedKernelViaAnalysis_2 Curved_kernel_via_analysis_2;
    
    //! myself
    typedef Point_2_rep< Curved_kernel_via_analysis_2 > Self;

    //! type of curve kernel
    typedef typename Curved_kernel_via_analysis_2::Curve_kernel_2 
    Curve_kernel_2;

    //! type of x-coordinate
    typedef typename Curve_kernel_2::X_coordinate_1 X_coordinate_1;
    
    //! type of a finite point on curve
    typedef typename Curve_kernel_2::Xy_coordinate_2 Xy_coordinate_2;

    //! type of supporting curve
    typedef typename Curve_kernel_2::Curve_2 Curve_2;
    
    //! type of arc-rep
    typedef CGALi::Arc_2_rep< Curved_kernel_via_analysis_2 >  Arc_2_rep;
        
    //! default constructor
    Point_2_rep() :
        _m_arc_rep(NULL),
        _m_ckva(NULL) {
    }
    
    //! constructs a "finite" point on curve,
    //! implies CGAL::NO_BOUNDARY in x/y
    Point_2_rep(const Xy_coordinate_2& xy) : 
        _m_xy(xy), _m_arc_rep(NULL), _m_location(CGAL::ARR_INTERIOR),
        _m_ckva(NULL) {
    }

    //! constructs a point on curve with y-coordinate at infinity
    Point_2_rep(const X_coordinate_1& x, CGAL::Arr_curve_end inf_end) :
        _m_arc_rep(NULL),
        _m_ckva(NULL) {
        _m_location = (inf_end == CGAL::ARR_MIN_END ?
             CGAL::ARR_BOTTOM_BOUNDARY : CGAL::ARR_TOP_BOUNDARY);
        _m_x = x;
    }

    //! constructs a point at +/-oo in x
    Point_2_rep(CGAL::Arr_curve_end inf_end) :
        _m_arc_rep(NULL),
        _m_ckva(NULL) {

        _m_location = (inf_end == CGAL::ARR_MIN_END ?
                CGAL::ARR_LEFT_BOUNDARY : CGAL::ARR_RIGHT_BOUNDARY);
    }
    
    // curve point finite coordinates. They are valid only if boundary in y 
    // is not set (CGAL::NO_BOUNDARY), otherwise only x-coordinate is
    // accessible (point lies at +/-oo)
    boost::optional<Xy_coordinate_2> _m_xy;
        
    // x-coordinate of a curve point
    boost::optional<X_coordinate_1> _m_x;

    // rep of incident arc
    mutable const Arc_2_rep *_m_arc_rep;

    // surface boundary type
    //mutable CGAL::Arr_boundary_type _m_boundary;
    // location of a point in parameter space
    mutable CGAL::Arr_parameter_space _m_location;

    // pointer to underlying ckva
    mutable Curved_kernel_via_analysis_2 *_m_ckva;

    // friends
    friend std::ostream& operator << <>(
            std::ostream&, 
            const Point_2< Curved_kernel_via_analysis_2, Self >&);

};

//! \brief class defines a point on a generic curve
//!
//! only points with finite x/y-coordinates can be constructed explicitly 
//! (by the user). Points at infinity use special private constructors and
//! required to represent infinite ends of curve arcs. In this case neither
//! supporting curve nor point's arcno is stored in \c Point_2 type - this
//! information is taken from \c Arc_2 this point belongs to.
template <class CurvedKernelViaAnalysis_2, 
          class Rep_ = CGALi::Point_2_rep<CurvedKernelViaAnalysis_2> >
class Point_2 : 
        public CGAL::Handle_with_policy< Rep_ > {
public:
    //!@{
    //!\name typedefs

    //! this instance's first template parameter
    typedef CurvedKernelViaAnalysis_2 Curved_kernel_via_analysis_2;

     //! this instance's second template parameter
    typedef Rep_ Rep;

    //! this instance itself
    typedef Point_2< Curved_kernel_via_analysis_2, Rep > Self;
    
    //! type of underlying curve analysis
    typedef typename Curved_kernel_via_analysis_2::Curve_kernel_2
        Curve_kernel_2;
    
    //! type of x-coordinate
    typedef typename Curve_kernel_2::X_coordinate_1 X_coordinate_1;
    
    //! type of a finite point on curve
    typedef typename Curve_kernel_2::Xy_coordinate_2 Xy_coordinate_2;
    
    //! type of generic curve
    typedef typename Curve_kernel_2::Curve_2 Curve_2;
    
    //! the handle superclass
    typedef ::CGAL::Handle_with_policy< Rep > Base;
    
    //! type of rep for arcs
    typedef CGALi::Arc_2_rep< Curved_kernel_via_analysis_2 > 
    Arc_rep;

    //! type of kernel point
    typedef typename Curved_kernel_via_analysis_2::Point_2 Kernel_point_2;
    
    //!@}
    
public:
    // Rebind
    /*!\brief
     * An auxiliary structure for rebinding the point with a new rep
     */
    template < typename NewCKvA_2, typename NewRep >
    class rebind
    {
    public:
        //! this instance's first template parameter
        typedef NewCKvA_2 New_curved_kernel_via_analysis_2;

        //! this instance's second template parameter
        typedef NewRep New_rep;

        //! the rebound type
        typedef Point_2< New_curved_kernel_via_analysis_2, NewRep > Other;
        
        //! the rebound point
        typedef typename New_curved_kernel_via_analysis_2::Point_2 
        Rebound_point_2;
        
        /*!\brief
         * constructs a point of type \c Rebound_point_2 from the point \c pt 
         * of type \c Self.
         *
         * All known items of the base class rep will be copied.
         */
        Rebound_point_2 operator()(const Self& pt) {
            New_rep newrep;
            newrep._m_xy = pt.ptr()->_m_xy;
            newrep._m_x = pt.ptr()->_m_x;
            // TODO set arc_rep in rebind of arc! (eriC)
            //newrep._m_arc_rep = pt.ptr()->_m_arc_rep;
            newrep._m_location = pt.ptr()->_m_location;
            //newrep._m_ckva will be set in calling constructor
            return Rebound_point_2(newrep);
        }
    };
    
public:
    //!\name public constructors
    //!@{

    /*!\brief
     * Default constructor
     */
    Point_2() : 
        Base(Rep()) {   
    }

    /*!\brief
     * copy constructor
     */
    Point_2(const Self& p) : 
            Base(static_cast<const Base&>(p)) {  
    }

protected: 

    //!\brief standard constructor: constructs a finite point with x-coordinate
    //! \c x on curve \c c with arc number \c arcno
    //!
    //! implies no boundary conditions in x/y
    Point_2(Curved_kernel_via_analysis_2 *kernel,
            const X_coordinate_1& x, const Curve_2& c, int arcno) :
        Base(Rep(Xy_coordinate_2(x, c, arcno))) {
        
        _set_ckva(kernel);
    }
    
    //!@}
private:
    // FUTURE TODO allow to construct without curve, 
    // i.e, isolated points on toric identifications -> do it also for arcs
    // FUTURE TODO parameter space in x/y (full set of tasks)
    
    //!@{
    //!\name private constructors for special cases (points at infinity)   

    //!\brief constructs a point with x-coordinate at infinity
    //! 
    //! \c inf_end defines whether the point lies at +/- infinity
    // TODO add arc_rep* (eriC)
    Point_2(CGAL::Arr_curve_end inf_end) :
         Base(Rep(inf_end)) {
    }
    
    //!\brief constructs a point with y-coordinate at infinity having
    //! x-coordinate \c x
    //!
    //! \c inf_end defines whether the point lies at +/- infinity
    // TODO add arc_rep* (eriC)
    Point_2(const X_coordinate_1& x, CGAL::Arr_curve_end inf_end) :
         Base(Rep(x, inf_end)) {
    }

    //!@}

protected:
    //!\name Constructors for rebind
    //!@{
    
    /*!\brief
     * constructs from a given represenation
     */
    // needed for rebind
    Point_2(Rep rep) : 
        Base(rep) {  
    }
    
    //!@}
    
protected:    
    //!\name Pointers
    //!@{

    //! sets pointer to ckva instance
    void _set_ckva(Curved_kernel_via_analysis_2 *ckva) const {
        this->ptr()->_m_ckva = ckva;
    }
    
    //! return pointer to ckva instance
    inline
    Curved_kernel_via_analysis_2* _ckva() const {
        return this->ptr()->_m_ckva;
    }

    // TODO  remove (eriC)
    //! sets pointer to incident arc
    void _add_ref(const Arc_rep *arc_rep) const {
        this->ptr()->_m_arc_rep = arc_rep;
    }

    //! returns pointer to incident arc
    inline
    const Arc_rep* _arc_rep() const {
        return this->ptr()->_m_arc_rep;
    }
    
public:

    //! returns whether the point is valid
    inline 
    bool is_finite() const {
        // FUTURE TODO on torus all points are finite
        return this->ptr()->_m_arc_rep == NULL;
    }

    //!@}
    
public:
    //!\name Destructors
    //!@{

    // virtual destructor
    virtual ~Point_2() {
    }

    //!@}

public:
    //!\name access functions and predicates
    //!@{

    //! access to underlying \c Xy_coordinate_2 object
    //!
    //! \pre finite x/y coordinates must be set by construction
    inline 
    const Xy_coordinate_2& xy() const {
        CGAL_precondition_msg(this->ptr()->_m_xy,
            "Denied access to the curve end lying at x/y-infinity");
        return *(this->ptr()->_m_xy);
    }

    //! access to the point's x-coordinate (y-coordinate can be undefined)
    //!
    //! \pre the point's x must be finite (set by construction)
    inline 
    const X_coordinate_1& x() const {
    
        CGAL_precondition_msg(this->ptr()->_m_xy || this->ptr()->_m_x,
          "Denied access to x-coordinate of the curve end \
            lying at x-infinity");
        return (location() == CGAL::ARR_INTERIOR ?
                (*(this->ptr()->_m_xy)).x() : *(this->ptr()->_m_x));
    }
    
    //! returns a supporting curve of underlying \c Xy_coordinate_2 object
    //!
    //! \pre this object must represent a finite point on curve
    inline 
    Curve_2 curve() const {
        CGAL_precondition_msg(
                this->ptr()->_m_xy || this->ptr()->_m_arc_rep != NULL,
                "Denied access to the curve end lying at y-infinity");
        return (location() == CGAL::ARR_INTERIOR ?
                (*(this->ptr()->_m_xy)).curve() :
                this->ptr()->_m_arc_rep->_m_support);
    }
    
    //! returns an arc number of underlying \c Xy_coordinate_2 object
    //!
    //! \pre this object must represent a finite point on curve
    inline int arcno() const {
        CGAL_precondition_msg(this->ptr()->_m_xy ||
            this->ptr()->_m_arc_rep != NULL,
            "Denied access to the curve end lying at y-infinity");
        return (location() == CGAL::ARR_INTERIOR ?
            (*(this->ptr()->_m_xy)).arcno() :
            this->ptr()->_m_arc_rep->_m_arcno);
    }
    
    public: 
    //!\name methods for location
    //!@{
    
    /*! \brief
     *  sets boundary type and location of a point in parameter space
     */
    void set_location(CGAL::Arr_parameter_space loc) const {
        this->ptr()->_m_location = loc;
    }
    
    //! returns location of a point in parameter space
    inline CGAL::Arr_parameter_space location() const { 
        return this->ptr()->_m_location; 
    } 
    
    //! checks if the point lies at x-infinity (x/y-coordinates are 
    //! inaccessible)
    inline bool is_on_left_right() const {
        return (location() == CGAL::ARR_LEFT_BOUNDARY ||
                location() == CGAL::ARR_RIGHT_BOUNDARY);
    }
    
    //! checks if the point lies at y-infinity (y-coordinate is inaccessible)
    inline bool is_on_bottom_top() const {
        return (location() == CGAL::ARR_BOTTOM_BOUNDARY ||
                location() == CGAL::ARR_TOP_BOUNDARY);
    }

    //!@}      

    
#define CGAL_CKvA_2_GRAB_CK_FUNCTOR_FOR_POINT(X, Y, Z) \
    CGAL_precondition(_ckva() != NULL); \
    typename Curved_kernel_via_analysis_2::X Y = \
         _ckva()->Z(); \
        
    //!\brief compares x-coordinates of two points 
    //!
    //!\pre compared points have finite x-coordinates
    inline
    CGAL::Comparison_result compare_x(const Kernel_point_2& q) const {
        CGAL_precondition(this->ptr()->_m_xy);
        CGAL_precondition(q.ptr()->_m_xy);

        CGAL_CKvA_2_GRAB_CK_FUNCTOR_FOR_POINT(Compare_x_2, 
                                              compare_x_2, 
                                              compare_x_2_object);
        CGAL_precondition(dynamic_cast< const Kernel_point_2* >(this));
        return compare_x_2(*dynamic_cast< const Kernel_point_2* >(this), q);
    }

    //!\brief compares two points lexicographical
    //!
    //!\pre compared points have finite x/y-coordinates
    inline
    CGAL::Comparison_result compare_xy(const Kernel_point_2& q, 
                                       bool equal_x = false) const {
        CGAL_precondition(this->ptr()->_m_xy);
        CGAL_precondition(q.ptr()->_m_xy);

        CGAL_CKvA_2_GRAB_CK_FUNCTOR_FOR_POINT(Compare_xy_2, 
                                              compare_xy_2, 
                                              compare_xy_2_object);
        CGAL_precondition(dynamic_cast< const Kernel_point_2* >(this));
        return compare_xy_2(
                *dynamic_cast< const Kernel_point_2* >(this), q, equal_x
        );
    }

    //! checks if the point lies on a curve
    inline 
    bool is_on(
            const typename Curved_kernel_via_analysis_2::Curve_2& curve
    ) const {
        CGAL_precondition(this->ptr()->_m_xy);

        CGAL_CKvA_2_GRAB_CK_FUNCTOR_FOR_POINT(Is_on_2, 
                                              is_on_2, 
                                              is_on_2_object);
        CGAL_precondition(dynamic_cast< const Kernel_point_2* >(this));
        return is_on_2(*dynamic_cast< const Kernel_point_2* >(this), curve);
    }

#undef CGAL_CKvA_2_GRAB_CK_FUNCTOR_FOR_POINT


    //! comparison operators (only for finite points):
    //! equality
    inline
    bool operator == (const Kernel_point_2& q) const { 
        return this->compare_xy(q) == CGAL::EQUAL;
    }
    
    //! inequality
    inline
    bool operator != (const Kernel_point_2& q) const {
        return this->compare_xy(q) != CGAL::EQUAL;
    }
    
    //! less than in (x,y) lexicographic order
    inline
    bool operator <  (const Kernel_point_2& q) const {
        return this->compare_xy(q) == CGAL::SMALLER;
    }
    
    //! less-equal in (x,y) lexicographic order
    inline
    bool operator <= (const Kernel_point_2& q) const {
        return this->compare_xy(q) != CGAL::LARGER;
    }

    //! greater than in (x,y) lexicographic order
    inline
    bool operator >  (const Kernel_point_2& q) const {
        return this->compare_xy(q) == CGAL::LARGER;
    }

    //! greater-equal in (x,y) lexicographic order
    inline
    bool operator >= (const Kernel_point_2& q) const {
        return this->compare_xy(q) != CGAL::SMALLER;
    }
    
    //!@}
    
public:
    
    //!\name IO
    //!@{
    
    /*!\brief 
     * writes point to \c os
     */
    void write(std::ostream& os) const {
        
        switch(::CGAL::get_mode(os)) {
        case ::CGAL::IO::PRETTY:
            os << "point@" << this->id() << "(";
            os << "sup@" << this->curve().id();
            os << " " << this->location() << "; ";
            if (this->location() != CGAL::ARR_LEFT_BOUNDARY &&
                this->location() != CGAL::ARR_RIGHT_BOUNDARY) {
                os << "x=" << NiX::to_double(this->x());
            } else {
                if (this->location() == CGAL::ARR_LEFT_BOUNDARY) {
                    os << "x=-oo";
                } else {
                    os << "x=+oo";
                }
            }
            os << ", ";
            if (this->location() != CGAL::ARR_BOTTOM_BOUNDARY &&
                this->location() != CGAL::ARR_TOP_BOUNDARY) {
                os << "y=n/a"; // TODO give y-coordinate (eriC)
            } else {
                if (this->location() == CGAL::ARR_BOTTOM_BOUNDARY) {
                    os << "y=-oo";
                } else {
                    os << "y=+oo";
                }
            }
            os << ", ";
            if (this->ptr()->_m_xy || this->ptr()->_m_arc_rep != NULL) {
                os << "ARCNO=" << this->arcno();
            } else {
                os << "VERT";
            }
            os << ")";
            break;
        case ::CGAL::IO::BINARY:
            std::cerr << "BINARY format not yet implemented" << std::endl;
            break;
        default:
            // ASCII 
            std::cerr << "ASCII format not yet implemented" << std::endl;
        }
    }
    
    //!@}
  
    // friends ////////////////////////////////////////////////////////////////

    //! befriending \c Arc_2_rep class
    friend class Arc_2_rep< Curved_kernel_via_analysis_2 >;

    //! befriending the arc base
    friend class Arc_2< Curved_kernel_via_analysis_2 >;

    // befriending the functors
    
#define CGAL_BEFRIEND_CKvA_2_FUNCTOR(Z) \
    friend class Curved_kernel_via_analysis_2::Z; \
    friend class Curved_kernel_via_analysis_2_Functors:: \
    Z< Curved_kernel_via_analysis_2 >; \
    
    CGAL_BEFRIEND_CKvA_2_FUNCTOR(Construct_point_2);
    CGAL_BEFRIEND_CKvA_2_FUNCTOR(Compare_x_2);
    CGAL_BEFRIEND_CKvA_2_FUNCTOR(Compare_xy_2);

#undef CGAL_BEFRIEND_CKvA_2_FUNCTOR

}; // class Point_2


/*!\relates Point_2
 * \brief 
 * output operator
 */
template < class CurvedKernelViaAnalysis_2, class Rep_ >
std::ostream& operator <<(std::ostream& os,
    const Point_2< CurvedKernelViaAnalysis_2, Rep_ >& pt) {

    pt.write(os);
    return os;
}

} // namespace CGALi

CGAL_END_NAMESPACE

#endif // CGAL_CURVED_KERNEL_POINT_2_H
