#ifndef EUCLID_GEOMETRY_POINT
#define EUCLID_GEOMETRY_POINT

#include <vector>
#include <array>
#include <numeric>
#include <cassert>
#include <iostream>
#include <math.h> // isnan

#include "Cartesian.hpp"

namespace Euclid {

class Point : public Cartesian
{
	public :

	// Constructors
	constexpr Point	( const double & a = 0.) : Cartesian(a) {};
	constexpr Point	( const double & x, const double & y, const double & z ) : Cartesian(x,y,z) {};
	constexpr Point	( const std::array<double,3> & A ) : Cartesian(A) {};
	constexpr Point	( const std::array<float,3> & A ) : Cartesian(A) {};
	constexpr Point	( const Point & P ) : Cartesian(P.x(),P.y(),P.z()) {};

	// A point is empty if any coordinate is unknown
	constexpr bool empty() { return ( isnan(data[0])|| isnan(data[1]) || isnan(data[2]) ); };

	constexpr Point & 		operator = 	( const Point & );

	constexpr Point 		operator - 	() { return Point( -x(),-y(),-z() ); };

	constexpr Point & 		operator += ( const Point & );
	constexpr Point & 		operator -= ( const Point & );
	constexpr Point & 		operator += ( const double & );
	constexpr Point & 		operator -= ( const double & );
	constexpr Point & 		operator *= ( const double & );
	constexpr Point & 		operator /= ( const double & );

	constexpr bool all_l  (const Point &) const;
	constexpr bool all_le (const Point &) const;
	constexpr bool all_g  (const Point &) const;
	constexpr bool all_ge (const Point &) const;

	friend std::ostream & operator << ( std::ostream & out, const Point A ) { return out << A.x() << "," << A.y() << "," << A.z() << ";"; };
	friend std::ostream & operator << ( std::ostream & out, const std::vector<Point> v ) { for ( Point c : v ) out << "\n\t" << c; out << std::endl; return out; };


}; // class Point

// Short scalar arithmetic
constexpr Point & 	Point::operator += ( const double & a ) { data[0] += a; data[1] += a; data[2] += a; return *this; }
constexpr Point & 	Point::operator -= ( const double & a ) { data[0] -= a; data[1] -= a; data[2] -= a; return *this; }
constexpr Point & 	Point::operator *= ( const double & a ) { data[0] *= a; data[1] *= a; data[2] *= a; return *this; }
constexpr Point & 	Point::operator /= ( const double & a ) { data[0] /= a; data[1] /= a; data[2] /= a; return *this; }

// Short Point arithmetic
constexpr Point & 	Point::operator += ( const Point & P ) { data[0] += P.x(); data[1] += P.y(); data[2] += P.z(); return *this; }
constexpr Point & 	Point::operator -= ( const Point & P ) { data[0] -= P.x(); data[1] -= P.y(); data[2] -= P.z(); return *this; }
constexpr Point & 	Point::operator =  ( const Point & P ) { data[0] =  P.x(); data[1] =  P.y(); data[2] =  P.z(); return *this; }

// Comparison from class
constexpr bool Point::all_l  (const Point & p) const { return (x() <  p.x() && y() <  p.y() && z() <  p.z() ); }
constexpr bool Point::all_le (const Point & p) const { return (x() <= p.x() && y() <= p.y() && z() <= p.z() ); }
constexpr bool Point::all_g  (const Point & p) const { return (x() >  p.x() && y() >  p.y() && z() >  p.z() ); }
constexpr bool Point::all_ge (const Point & p) const { return (x() >= p.x() && y() >= p.y() && z() >= p.z() ); }

// Comparison
inline constexpr bool 	operator ==	( const Point & A, const Point & B ) { return A.x() == B.x() && A.y() == B.y() && A.z() == B.z(); }
inline constexpr bool 	operator != ( const Point & A, const Point & B ) { return A.x() != B.x() || A.y() != B.y() || A.z() != B.z(); }

// Unary arithmetic
inline constexpr Point 	operator + 	( const Point & P ) { return P; }
inline constexpr Point 	operator - 	( const Point & P ) { return Point( -P.x(),-P.y(),-P.z() ); }

// Arithmetic
inline constexpr Point operator + ( const Point  & A, const Point  & B) { Point P =  A; P += B; return P; }
inline constexpr Point operator + ( const Point  & A, const double & a) { Point P =  A; P += a; return P; }
inline constexpr Point operator + ( const double & a, const Point  & B) { Point P =  B; P += a; return P; }

inline constexpr Point operator - ( const Point  & A, const Point  & B) { Point P =  A; P -= B; return P; }
inline constexpr Point operator - ( const Point  & A, const double & b) { Point P =  A; P -= b; return P; }
inline constexpr Point operator - ( const double & a, const Point  & B) { Point P = -B; P += a; return P; }

inline constexpr Point operator * ( const Point  & A, const double & b) { Point P =  A; P *= b; return P; }
inline constexpr Point operator * ( const double & a, const Point  & B) { Point P =  B; P *= a; return P; }

inline constexpr Point operator / ( const Point  & A, const double & b) { Point P =  A; P /= b; return P; }
inline constexpr Point operator / ( const double & a, const Point  & B) { Point P =  B; P /= a; return P; }

// Argument, angle, with respect to axes in cartesian coordinate system
std::array<double,3> 	arg ( const Point & ); // angle to axes
double 					arg ( const Point & , int plane ); // angle above specified plane

// Returns the elementwise smallest value
inline constexpr Point
emin ( const Point & v1 , const Point & v2 )
{
	using std::min;
	return Point {min(v1.x(),v2.x()),min(v1.y(),v2.y()),min(v1.z(),v2.z())};
}
// Returns the elementwise largest value
inline constexpr Point
emax ( const Point & v1 , const Point & v2 )
{
	using std::max;
	return Point {max(v1.x(),v2.x()),max(v1.y(),v2.y()),max(v1.z(),v2.z())};
}

} // namespace Euclid

#endif
