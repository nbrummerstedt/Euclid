#ifndef EUCLID_GEOMETRY_PLANE
#define EUCLID_GEOMETRY_PLANE

#include "Point.hpp"
#include "Vector.hpp"

// A plane is defined by an intersection point in space and its normal vector.
// A plane spans infinitely unless bounded by a mechanism in a derived class.

namespace Euclid {

	class Plane {
		protected :
			Point 	intersect;
			Vector 	normal;
		public :
			constexpr Plane( const Point & p, const Vector & v ) : intersect(p), normal(v) {};
			constexpr Plane operator - ( ) { return Plane {intersect,-normal}; };
	};
}

#endif
