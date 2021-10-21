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
			constexpr Distance( const double & a = double() , bool squared = false , bool negative = false ) 
			: _data(negative?(squared?-abs(a):-(a*a)):(squared?a:a*a)) {};
			constexpr double sign() const { return std::signbit(_data) ? 1. : -1. ; };
			constexpr double value() const { return sign()*sqrt(squared()); };
			constexpr double squared() const { return abs(_data); };
			constexpr double signedSquare() const { return sign()*abs(_data); };
			constexpr operator bool() const { return abs(_data) > 1e-20; };
			
			constexpr bool operator < (const Distance & ) const;
			constexpr bool operator < (const double & ) const;
			
			// Print
			friend std::ostream & 	operator << ( std::ostream & out, const Distance & d ) 
			{
				return out << d.sign() << " * " << d.value();
			};
	};
	
	constexpr bool Distance::operator < (const Distance & other ) const
	{
		return squared() < other.squared();
	}
	
	constexpr bool Distance::operator < (const double & other ) const
	{
		return squared() < other*other;
	}
	
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
