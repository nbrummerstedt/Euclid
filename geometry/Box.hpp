#ifndef EUCLID_GEOMETRY_BOX
#define EUCLID_GEOMETRY_BOX

#include <array>
#include <vector>
#include <cfloat>

#include "Point.hpp"
#include "Triangle.hpp"
#include "Interval.hpp"
#include "Ray.hpp"

namespace Euclid {

class Box {
	Point _min;
	Point _max;
	public :
	constexpr static double min_side_length { 5.0e-4 };
	Box() {}
	Box( const Point & a, const Point & b ) : _min(a),_max(b) {}
	const Point & 	min() const { return _min; }
	const Point & 	max() const { return _max; }
	bool 				intersect	( const Ray & ) const;
	bool 				intersect	( const Point&, const Vector& ) const;
	Interval<Distance> 	distance	( const Point & ) const;
	void			 	minmax_sq_dist	( const Point & ,double&,double&) const;
	static Box 			box         ( const Triangle & );
	static Box 			box_and_split( const std::vector<Triangle>&, std::vector<Triangle>&, std::vector<Triangle>&) ;				 
};

bool 
Box::intersect( const Ray & r ) const
{
	const double tiny { 3e-7 };
	Point t0 { (_min-r.origin()) / r.direction() }; // Elementwise division
	Point t1 { (_max-r.origin()) / r.direction() }; // Elementwise division
	Point tin  = emin(t0,t1);
	Point tout = emax(t0,t1);
	double tmin = std::max(tin[0], std::max(tin[1], tin[2]));
	double tmax = std::min(tout[0], std::min(tout[1], tout[2]));
	return ( (tmin-tiny) < (tmax+tiny));
};
bool 
Box::intersect( const Point&p, const Vector&v ) const
{
	return intersect(Ray(p,v));
};

Interval<Distance> 
Box::distance( const Point & query ) const
{
	// Distance has properties:
	// If inside box
	// - min distance is 0
	// - max distance is distance to farthest point (negative sign)
	// If outside box
	// - min distance is distance to closest point in box (positive sign)
	// - max distance is distance to farthest point in box (positive sign)
	
	// Point a is positive valued distance from midpoint to side planes
	const Point a = 0.5 * ( _max - _min );
	const Point midpoint = _min + a;
	// Vector v points from midpoint to query point
	Vector v = Vector(midpoint,query);
	Vector d, f;
	for(int i=0;i<3;++i) {
		// If we are outside to the left, take min as distance to leftmost
		// If we are outside to the right, take min as distance to rightmost
		// If we are inside box, take distance component as zero
		if      (v[i] < -a[i])  d[i] = query[i]-_min[i];
		else if (v[i] >  a[i])  d[i] = query[i]-_max[i];
		else                    d[i] = 0;
		// If we are to the right of the middle, take max as distance to leftmost
		// If we are to the left of the middle, take max as distance to rightmost
		if      (v[i] >= 0 )    f[i] = query[i]-_min[i];
		else                    f[i] = query[i]-_max[i];
	}
	double dmin = d.norm();
	double dmax = f.norm();
	assert(dmin<=dmax);
	return Interval<Distance> {Distance(dmin,true),Distance(dmax,true)};
};

void
Box::minmax_sq_dist( const Point & query , double&min, double&max) const
{
	const Point a = 0.5 * ( _max - _min );
	const Point midpoint = _min + a;
	Vector v = Vector(midpoint,query);
	Vector d, f;
	for(int i=0;i<3;++i) {
		if      (v[i] < -a[i])  d[i] = query[i]-_min[i];
		else if (v[i] >  a[i])  d[i] = query[i]-_max[i];
		else                    d[i] = 0;
		if      (v[i] >= 0 )    f[i] = query[i]-_min[i];
		else                    f[i] = query[i]-_max[i];
	}
	min = d.norm();
	max = f.norm();
	return;
};

Box 
Box::box( const Triangle & t ) { return Box( t.pmin() , t.pmax() ); };

Box
Box::box_and_split( const 	std::vector<Triangle> & invec,
							std::vector<Triangle> & lvec,
							std::vector<Triangle> & rvec )
{
	// Find the bounds of the box by considering all triangles
	Point box_pmin(+DBL_MAX,+DBL_MAX,+DBL_MAX);
	Point box_pmax(-DBL_MAX,-DBL_MAX,-DBL_MAX);
	for( Triangle t : invec ) {
		box_pmin = emin( t.pmin(), box_pmin );
		box_pmax = emax( t.pmax(), box_pmax );
	}
	// Construct diagonal vector, S, which spans the length of the box in all three directions
	Vector S = box_pmax - box_pmin;
	
	// Split the box into two on the longest dimension
	// Index of longest dimension coordinate, i
	// Get the threshold coordinate
	int i { S.x()>S.y()?(S.x()>S.z()?0:2):(S.y()>S.z()?1:2) };
	double thr = box_pmin(i) + S(i)/2.;
	
	// For returning a vertex as Box midpoint
	// Initial closest midpoint, mid
	// Box center, c
	Point mid = invec[0].vertex(0);
	Point c = box_pmin + S/2;
	
	for( Triangle t : invec ) {
		// Assign all triangles to left or right based on triangle center coordinate
		if( t.center()[i] > thr ) rvec.push_back(t); else lvec.push_back(t);
		// Find the vertex closest to the box center
		for ( Point p : t.vertices() ) if(Euclid::distance(c,p)<Euclid::distance(c,mid)) mid = p;
	}
	
	// Handle edge case where everything fell to one side
	if( lvec.empty() || rvec.empty() ) {
		size_t N = invec.size();
		lvec.clear();
		rvec.clear();
		lvec.insert(lvec.end(), invec.begin(), invec.begin()+N/2);
		rvec.insert(rvec.end(), invec.begin()+N/2, invec.end());
	}
	
	// Make sure we got it right and return
	assert(!lvec.empty());
	assert(!rvec.empty());
	assert(lvec.size()+rvec.size() == invec.size());
	return Box(box_pmin,box_pmax);
};
		
} // namespace Euclid

#endif
