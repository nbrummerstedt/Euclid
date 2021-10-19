#ifndef EUCLID_DISTANCE_H
#define EUCLID_DISTANCE_H

#include <iostream>
#include <cmath>
#include "Point.hpp"

namespace Euclid {

	class Distance {
		private : 
			// There really is no need to save the sign with individual bits:
			// IEEE double has a dedicated bit for sign, and since the value stored 
			// is the squared value (always positive), the bit can be used to encode the sign
			// of the original value.
			// Care should be taken with comparison operators not to waste 
			// resources on taking absolute value. A simple bitshift will suffice. 
			// ( However, that's probably how std::abs is implemented anyway )
			double _data;
		public :
			constexpr Distance( const double & a = double() ) : _data(a) {};
			constexpr double Sign() const { return std::signbit(_data) ? 1. : -1. ; };
			constexpr double Value() const { return sqrt(Squared()); };
			constexpr double Squared() const { return abs(_data); };
			constexpr double SignedSquare() const;
			constexpr operator bool() const { return abs(_data) > 1e-20; };
			
			// Print
			friend std::ostream & 	operator << ( std::ostream & out, const Distance & d ) 
			{
				return out << d.Sign() << " * " << d.Value();
			};
	};
	
	
    // Properties
    
	inline constexpr Distance 
	distance	( const Point & p1, const Point & p2 ) 
	{
		double a { p2.x()-p1.x() };
		double b { p2.y()-p1.y() };
		double c { p2.z()-p1.z() };
		return Distance { a*a+b*b+c*c };
	};
	
}

#endif
