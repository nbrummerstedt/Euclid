#ifndef EUCLID_GEOMETRY_VECTOR
#define EUCLID_GEOMETRY_VECTOR

#include <array>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cmath>

#include "Cartesian.hpp"
#include "Point.hpp" // For the sake of a direct constructor

namespace Euclid {

class Vector : public Cartesian {

	public:
	
	// Constructors
	constexpr Vector( const double & a = 0.) : Cartesian(a) {};
	constexpr Vector( const double & x, const double & y, const double & z ) : Cartesian(x,y,z) {};
	constexpr Vector( const Point & P ) : Cartesian(P.x(),P.y(),P.z()) {};
	constexpr Vector( const Point & p1 , const Point & p2 ) : Cartesian(p2.x()-p1.x(),p2.y()-p1.y(),p2.z()-p1.z()) {};
	
	// Properties
	constexpr double norm	( ) const { return data[0]*data[0] + data[1]*data[1] + data[2]*data[2]; };
	constexpr double length	( ) const { return sqrt( norm() ); };
	
	// Binary arithmetic explicitly synthesised from Cartesian
	constexpr Vector operator * ( const double & a ) const { Cartesian::operator * ( a ); return *this; };
	constexpr Vector operator / ( const double & a ) const { Cartesian::operator / ( a ); return *this; }; 
	constexpr Vector operator += ( const double & a ) { Cartesian::operator += ( a ); return *this; };
	constexpr Vector operator -= ( const double & a ) { Cartesian::operator -= ( a ); return *this; }; 
	constexpr Vector operator *= ( const double & a ) { Cartesian::operator *= ( a ); return *this; };
	constexpr Vector operator /= ( const double & a ) { Cartesian::operator /= ( a ); return *this; }; 
	
	// Linear algebra operators
	constexpr Vector cross 	( const Vector & ) 	const;
	constexpr double dot   	( const Vector & ) 	const;
	constexpr Vector normalised( ) 				const;
	constexpr double angle (const Vector & ) 	const;
	
	// Comparison returning zero-one (double-valued) vectors
	constexpr Vector operator <	 ( const Vector & );
	constexpr Vector operator >  ( const Vector & );
	constexpr Vector operator <= ( const Vector & );
	constexpr Vector operator >= ( const Vector & );
	constexpr Vector operator <	 ( const double & );
	constexpr Vector operator >  ( const double & );
	constexpr Vector operator <= ( const double & );
	constexpr Vector operator >= ( const double & );
	
	friend std::ostream & operator << ( std::ostream & out, const Vector A ) { return out << A.x() << "," << A.y() << "," << A.z() << ";"; };
	friend std::ostream & operator << ( std::ostream & out, const std::vector<Vector> v ) { for ( Vector c : v ) out << "\n\t" << c; out << std::endl; return out; };
	
};

// Comparison operators
inline constexpr Vector Vector::operator <	( const Vector & v ) { return Vector(x()<v.x()?1.:0.,y()<v.y()?1.:0.,z()<v.z()?1.:0. );};
inline constexpr Vector Vector::operator >  ( const Vector & v ) { return Vector(x()>v.x()?1.:0.,y()>v.y()?1.:0.,z()>v.z()?1.:0. );};
inline constexpr Vector Vector::operator <= ( const Vector & v ) { return Vector(x()<=v.x()?1.:0.,y()<=v.y()?1.:0.,z()<=v.z()?1.:0. );};
inline constexpr Vector Vector::operator >= ( const Vector & v ) { return Vector(x()>=v.x()?1.:0.,y()>=v.y()?1.:0.,z()>=v.z()?1.:0. );};
inline constexpr Vector Vector::operator <	( const double & a ) { return Vector(x()<a?1.:0.,y()<a?1.:0.,z()<a?1.:0. );};
inline constexpr Vector Vector::operator >  ( const double & a ) { return Vector(x()>a?1.:0.,y()>a?1.:0.,z()>a?1.:0. );};
inline constexpr Vector Vector::operator <= ( const double & a ) { return Vector(x()<=a?1.:0.,y()<=a?1.:0.,z()<=a?1.:0. );};
inline constexpr Vector Vector::operator >= ( const double & a ) { return Vector(x()>=a?1.:0.,y()>=a?1.:0.,z()>=a?1.:0. );};

// Unary arithmetic 
inline constexpr Vector	operator + 	( const Vector & v ) { return v; } 
inline constexpr Vector	operator - 	( const Vector & v ) { return Vector( -v.x(),-v.y(),-v.z() ); } 

// Vector operations
inline constexpr Vector Vector::cross ( const Vector & a ) const
{
	return Vector { y()*a.z() - z()*a.y() , z()*a.x() - x()*a.z() , x()*a.y() - y()*a.x() };
};
inline constexpr double Vector::dot ( const Vector & a )  const
{
	return double { x() * a.x() + y() * a.y() + z() * a.z() };
};
inline constexpr Vector Vector::normalised( ) const
{
	Vector v {data[0],data[1],data[2]};
	double l { length() };
	return { v / l };
};
inline constexpr double Vector::angle ( const Vector & b )  const
{
	return double { acos( dot(b) / ( length() * b.length() ) ) };
};

// Vector operations
constexpr Vector cross ( const Vector & v1 , const Vector & v2 ) { return v1.cross(v2); };
constexpr double dot   ( const Vector & v1 , const Vector & v2 ) { return v1.dot(v2); };

// Vector point friendships
inline constexpr Point operator + ( const Point & p , const Vector & v ) { return Point { p + Point( v.x(),v.y(),v.z() ) }; };
inline constexpr Point operator - ( const Point & p , const Vector & v ) { return Point { p - Point( v.x(),v.y(),v.z() ) }; };
inline constexpr Point operator * ( const Point & p , const Vector & v ) { return Point { p.x()*v.x() , p.y()*v.y() , p.z()*v.z() }; };
inline constexpr Point operator / ( const Point & p , const Vector & v ) { return Point { p.x()/v.x() , p.y()/v.y() , p.z()/v.z() }; };

// Returns the elementwise smallest value
inline constexpr Vector emin ( const Vector & v1 , const Vector & v2 ) 
{
	using std::min;
	return Vector {min(v1.x(),v2.x()),min(v1.y(),v2.y()),min(v1.z(),v2.z())};
}
// Returns the elementwise largest value
inline constexpr Vector emax ( const Vector & v1 , const Vector & v2 ) 
{
	using std::max;
	return Vector {max(v1.x(),v2.x()),max(v1.y(),v2.y()),max(v1.z(),v2.z())};
}

} // namespace Euclid

#endif
