#ifndef EUCLID_GEOMETRY_RAY
#define EUCLID_GEOMETRY_RAY

#include "Point.hpp"
#include "Vector.hpp"

namespace Euclid {

	class Ray {
		private :
			Point 	_p;
			Vector 	_v;
		public :
			constexpr Ray( const Point & p, const Vector & v ) : _p(p), _v(v) {};
			constexpr Ray operator - ( ) { return Ray(_p,-_v); };
			constexpr Point origin() const { return _p; };
			constexpr Vector direction() const { return _v; };
	};
}

#endif
