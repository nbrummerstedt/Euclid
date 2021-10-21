#ifndef EUCLID_GEOMETRY_TRIANGLE
#define EUCLID_GEOMETRY_TRIANGLE

#include <iostream>
#include <array>
#include "Point.hpp"
#include "Segment.hpp"
#include "Distance.hpp"
#include "Vector.hpp"
#include "Ray.hpp"

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
			//constexpr Distance 	distance		( const Point & p ) const;
			constexpr double 	signedsqrdist	( const Point & p ) const;
			constexpr bool 		distance 		( const Point & p , double&, double&) const;
			constexpr Point 	closest_point 	( const Point & p ) const;
			constexpr bool 		intersect		( const Ray &, double & ) const;
			constexpr bool 		intersect		( const Point &, const Vector &, double & ) const;
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
	Triangle::intersect ( const Ray & r, double & measure ) const
	{
		return intersect(r.origin(),r.direction(),measure);
	};
	
	constexpr bool 
	Triangle::intersect ( const Point & src, const Vector & dir, double & t ) const
	{
		// Directly taken from
		// https://github.com/janba/GEL/blob/master/src/GEL/Geometry/Triangle.cpp
		// Adapted to Euclid syntax
		// "Moellers method"
		// If determinant is near zero, ray lies in plane of triangle
		// which is not considered an intersect
		double eps 	{ 1e-10 };
		Vector tvec { vertex(0),src };
		Vector pvec = cross( dir, edge(2).as_vector() );
		double det { dot( edge(0).as_vector() , pvec ) };
		if ( abs(det) < eps ) return false;
		double inv { 1.0 / det };
		double a = dot( tvec, pvec ) * inv;
		if (a < 0.0 || a > 1.0) return false;
		Vector qvec = cross( tvec, edge(0).as_vector() );
		double b = dot( dir, qvec ) * inv;
		if (b < 0.0 || a + b > 1.0) return false;
		// Ray intersects triangle
		t = dot(-edge(2).as_vector(), qvec) * inv;
		return true;
	};

	/*
	constexpr Distance 
	Triangle::distance ( const Point & p ) const
	{
		Point v { closest_point(p) };
		Vector r { p , v };
		double sq_dist = r.norm();
		bool negative { dot( normal() , r) < 0 };
		bool squared = true;
		return Distance{ sq_dist, squared, negative };
	};
	*/
	
	constexpr double 
	Triangle::signedsqrdist ( const Point & p ) const
	{
		Point 	v 	{ closest_point(p) };
		Vector 	r 	{ p , v };
		double 	d 	{ r.norm() };
		bool 	neg { dot( normal() , r) < 0 };
		return neg?-d:d;
	};
	
	constexpr bool 
	Triangle::distance ( const Point & p , double&sq_dist, double&sign) const
	{
		// C-style version of distance
		Point 	v 	{ closest_point(p) };
		Vector 	r 	{ p , v };
		bool 	neg { dot( normal() , r) < 0 };
		// Assign to input variables (internal screaming)
		sq_dist = r.norm();
		sign    = neg?-1.:1.;
		// Indicate success (internal crying)
		return true;
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
