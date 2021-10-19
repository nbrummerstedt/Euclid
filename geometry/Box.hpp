#ifndef EUCLID_GEOMETRY_BOX
#define EUCLID_GEOMETRY_BOX

#include <array>
#include <vector>

#include "Point.hpp"
#include "Triangle.hpp"
#include "Interval.hpp"

namespace Euclid {

class Box {
	Point _min;
	Point _max;
	Point _mid;
	public :
	constexpr static double min_side_length { 5.0e-4 };
	Box() {}
	Box( const Point & a, const Point & b, const Point & c ) : _min(a),_max(b),_mid(c)
	{
		double s { min_side_length/2. };
		if ((_max.x()-_min.x()) < min_side_length ) { _max += Point(s,0,0); _min -= Point(s,0,0); }
		if ((_max.y()-_min.y()) < min_side_length ) { _max += Point(0,s,0); _min -= Point(0,s,0); }
		if ((_max.z()-_min.z()) < min_side_length ) { _max += Point(0,0,s); _min -= Point(0,0,s); }
		assert( _min.all_le(_mid) );
		assert( _max.all_ge(_mid) );
	}
	const Point & 	min() const { return _min; }
	const Point & 	max() const { return _max; }
	bool 				intersect			( const Point &, const Vector & ) const;
	Interval<Distance> 	distance	( const Point & ) const;
	static Box 			box         ( const Triangle & t );
	static Box 			box_and_split( const std::vector<Triangle>&, std::vector<Triangle>&, std::vector<Triangle>&) ;				 
};

bool 
Box::intersect( const Point & p, const Vector & dir ) const
{
	const double tiny { 3e-7 };
	Point t0 { (_min-p) / dir }; // Elementwise division
	Point t1 { (_max-p) / dir }; // Elementwise division
	Point tin  = emin(t0,t1);
	Point tout = emax(t0,t1);
	double tmin = std::max(tin[0], std::max(tin[1], tin[2]));
	double tmax = std::min(tout[0], std::min(tout[1], tout[2]));
	return ( (tmin-tiny) < (tmax+tiny));
};

Interval<Distance> 
Box::distance( const Point & p ) const
{
	const Point a = 0.5*_max-0.5*_min;
	const Point p0 = _min + a;
	Vector d = Vector(p-p0);
	Vector f(d);
	for(int i=0;i<3;++i) 
	{
		if(f[i]>=0)  f[i] = p[i]-_min[i];
		else f[i] = p[i]-_max[i];
		if(d[i]<-a[i]) d[i] = p[i]-_min[i];
		else if(d[i]>a[i]) d[i] = p[i]-_max[i];
		else  d[i] = 0;
	}
	double dmin = d.length();
	double dmax = f.length();
	assert(dmin<=dmax);
	return Interval<Distance> {Distance(dmin),Distance(dmax)};
};

Box 
Box::box( const Triangle & t ) { return Box( t.pmin() , t.pmax(), t.center() ); };

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
	return Box(box_pmin,box_pmax,mid);
};
		
} // namespace Euclid

#endif
