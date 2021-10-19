#ifndef EUCLID_GEOMETRY_LINE
#define EUCLID_GEOMETRY_LINE

// Defined by intersecting two points (not with point and vector)
// A line has no direction, ie. point A and B on a line has no logical test A<B
// That would be either an "Infinite Ray" or an "Oriented Line"
// A line has infinite extent.

#include "Point.hpp"

namespace Euclid {

	class Line {
		private :
			Point p1, p2;
		 public:
			constexpr Line( const Point & A, const Point & B ) : p1(A), p2(B) {};
	};

}

#endif
