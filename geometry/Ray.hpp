#ifndef EUCLID_GEOMETRY_RAY
#define EUCLID_GEOMETRY_RAY

#include "Point.hpp"
#include "Vector.hpp"

namespace Euclid {

	class Ray {
		private :
			Point 	origin;
			Vector 	direction;
		public :
			constexpr Ray( const Point & p, const Point & v ) : orinin(p), direction(v) {};
			constexpr Ray operator - ( ) { return Ray {origin,-direction}; };
	};
}

#endif
