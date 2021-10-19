#ifndef EUCLID_GEOMETRY_SEGMENT
#define EUCLID_GEOMETRY_SEGMENT

#include "Point.hpp"
#include "Vector.hpp"

namespace Euclid {

	class Segment {
		private : 
			Point p1;
			Point p2;
		 public:
			constexpr Segment( const Point & A, const Point & B ) : p1(A), p2(B) {};
			constexpr Segment operator - ( ) { return Segment {-p1,-p2}; };
			constexpr Point first()  {return p1;};
			constexpr Point second() {return p2;};
			constexpr Vector as_vector() { return Vector { p1 , p2 }; }
			constexpr double length();
	}; 
	
	inline constexpr double Segment::length() 
	{
		double x { p2.x() - p1.x() };
		double y { p2.y() - p1.y() };
		double z { p2.z() - p1.z() };
		return sqrt( x*x + y*y + z*z );
	}
	
}

#endif
