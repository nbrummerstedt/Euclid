#ifndef EUCLID_GEOMETRY_TRIANGLE
#define EUCLID_GEOMETRY_TRIANGLE

#include <array>
#include "Point.hpp"
#include "Segment.hpp"
#include "Distance.hpp"
#include "Vector.hpp"

namespace Euclid {

	class Triangle {
		protected :
			std::array<Point,3> data;
		public :
			constexpr Triangle ( const Point & A, const Point & B, const Point & C ) : data({{A,B,C}}) {};
			// Data access
			constexpr Point 	vertex		( size_t i ) const;
			constexpr Segment 	edge		( size_t i ) const;
			constexpr std::array<Point,3> vertices() const;
			// Computations
			constexpr double 	area			( ) const;
			constexpr Point 	center			( ) const;
			constexpr Vector 	normal			( ) const;
			constexpr Distance 	distance		( const Point & p ) const;
			constexpr Point 	closest_point 	( const Point & p ) const;
			constexpr bool 		intersect		( const Point & origin, const Vector & direction, double & ) const;
			constexpr double 	angle 			( size_t );
			constexpr Point 	pmin() const  { return emin(data[0],emin(data[1],data[2])); }
			constexpr Point 	pmax() const  { return emax(data[0],emax(data[1],data[2])); }
	};
	
	inline constexpr double 
	Triangle::area() const
	{ 
		Vector v1 { data[0] , data[1] };
		Vector v2 { data[0] , data[2] };
		return 0.5 * ( cross(v1,v2) ).length(); 
	};
	
	inline constexpr double
	Triangle::angle ( size_t i ) 
	{
		Vector v1 {vertex(i),vertex((i+1)%3)};
		Vector v2 {vertex(i),vertex((i+2)%3)};
		return v1.angle(v2);
	};
	
	inline constexpr Point 	
	Triangle::center() const
	{ 
		return (data[0]+data[1]+data[2])/3.; 
	};
	
	inline constexpr Vector 
	Triangle::normal() const
	{ 
		Vector v1 {data[0],data[1]};
		Vector v2 {data[0],data[2]};
		return cross(v1,v2).normalised(); 
	};
	
	constexpr Point
	Triangle::vertex ( size_t i ) const {
		assert(i<3);
		return data[i];
	};
	
	constexpr std::array<Point,3>
	Triangle::vertices() const {
		return data;
	};
	
	constexpr Segment
	Triangle::edge ( size_t i ) const
	{
		return Segment { vertex(i), vertex( (i+1) % 3 ) };
	};
	

	constexpr bool 
	Triangle::intersect ( const Point & origin, const Vector & direction, double & measure ) const
	{
		// Directly taken from
		// https://github.com/janba/GEL/blob/master/src/GEL/Geometry/Triangle.cpp
		// Adapted to Euclid syntax
		// "Moellers method"
		// If determinant is near zero, ray lies in plane of triangle
		// which is not considered an intersect
		double eps { 1e-10 };
		Vector p = cross( direction, edge(2).as_vector() );
		double det { dot( edge(0).as_vector() , p ) };
		if ( abs(det) < eps ) return false;
		double inv_det { 1.0 / det };
		Point p0 { origin - vertex(0) };
		Vector t {  p0  };
		double u = dot( t, p ) * inv_det;
		if (u < 0.0 || u > 1.0) return false;
		Vector q = cross( t, edge(0).as_vector() );
		double v = dot( direction, q ) * inv_det;
		if (v < 0.0 || u + v > 1.0) return false;
		// Ray intersects triangle
		measure = dot(-edge(2).as_vector(), q) * inv_det;
		return true;
	};

	constexpr Distance 
	Triangle::distance ( const Point & p ) const
	{
		// Using triangle face normal, not a manifold normal
		// A manifold normal should not be required to compute sign even in manifold
		Point v { closest_point(p) };
		Vector r { p , v };
		double sq_dist = r.norm();
		double sign { (dot( normal() , r) >=0) ? 1. : -1. };
		return Distance{ sign*sq_dist };
	};
	
	constexpr Point
	Triangle::closest_point ( const Point & p ) const
	{
		int scores[3] {0,0,0};
		// Check all cases where point is not directly above or below
		for ( int i = 0; i < 3; i++ ) {
			int j = (i+1) % 3;
			Vector s { edge(i).as_vector().normalised() }; // Direction from vertex i to j
			Vector t { vertex(i) , p }; // Vector pointing to input
			double l { edge(i).length() }; // Edge length
			double a = dot(t,s) / l; // Coordinate along edge
			if ( a <= 0 ) { scores[i]++; if ( scores[i] == 2 ) return vertex(i); } 
			if ( a >= 1 ) { scores[j]++; if ( scores[j] == 2 ) return vertex(j); }
			if ( a < 1 && a > 0 && dot(t,cross(s,normal())) >= 0 ) return { Point( vertex(i) + s ) * ( a * l ) };
		}
		// If we get here, we are directly above or below
		return { Point( p - normal() ) * dot(Vector(vertex(0),p),normal()) };
	};

}
#endif
