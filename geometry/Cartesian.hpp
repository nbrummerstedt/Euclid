#ifndef EUCLID_GEOMETRY_CARTESIAN
#define EUCLID_GEOMETRY_CARTESIAN

#include <vector>
#include <array>
#include <numeric>
#include <cassert>
#include <iostream>
#include <math.h>

namespace Euclid {

class Cartesian
{
	protected : std::array<double,3> data;
	
	public :
	
	constexpr Cartesian ( const double & a) : data({a,a,a}) {};
	constexpr Cartesian	( const double & x, const double & y, const double & z ) : data({{x,y,z}}) {};
	constexpr Cartesian	( const std::array<double,3> & A ) : data({{A[0],A[1],A[2]}}) {};
	constexpr Cartesian	( const std::array<float,3> & A ) : data({{A[0],A[1],A[2]}}) {};
	
	// Data access by indexing
	// Const index operator []		-- access : return reference
	// Non-const index operator []	-- access : return reference
	// Const index operator ()		-- extraction : should not return a reference
	// Non-const index operator ()	-- extraction : should not return a reference
	const 		double &	operator [] ( size_t i ) const 	{ assert(i<3); return data[i]; };
				double &	operator [] ( size_t i ) 		{ assert(i<3); return data[i]; };
	constexpr 	double  	operator () ( size_t i ) const 	{ assert(i<3); return data[i]; };
	
	// Data extract (const) using identifiers
	constexpr double x	() 	const { return data[0]; };
	constexpr double y	() 	const { return data[1]; };
	constexpr double z	() 	const { return data[2]; };
	
	// Data assignment by index
	constexpr Cartesian &  set	( size_t i , double a ) { assert(i<3); data[i] = a; return *this; };
	
	// Data assignment by istream
	// Inspired by GEL
	// Warning: This is by no means robust yet.
	friend std::istream & operator >> ( std::istream & in, Cartesian & p)
	{
		in >> std::ws; if (in.peek() == '[') in.ignore();
		in >> std::ws; for (int c=0; c<3; ++c) in >> p[c] >> std::ws;
		if (in.peek() == ']') in.ignore();
		return in;
	}
	
	// Iterators
	typedef std::array<double,3>::iterator iterator;
	typedef std::array<double,3>::const_iterator citerator;
	iterator		begin() 		{ return data.begin(); }
	iterator 		end() 			{ return data.end(); }
	double* 		get() 			{ return data.data(); }
	citerator begin() const 	{ return data.begin(); }
	citerator end() 	const 	{ return data.end(); }
	const double* 	get() 	const 	{ return data.data(); }

	// Unary elementwise arithmetic
	constexpr Cartesian operator - 	() { return Cartesian( -x(),-y(),-z() ); };
	
	// Binary elementwise arithmetic
	constexpr Cartesian operator + ( const Cartesian & ) const;
	constexpr Cartesian operator - ( const Cartesian & ) const;
	constexpr Cartesian operator * ( const Cartesian & ) const;
	constexpr Cartesian operator / ( const Cartesian & ) const;
	constexpr Cartesian operator + ( const double & ) const;
	constexpr Cartesian operator - ( const double & ) const; 
	constexpr Cartesian operator * ( const double & ) const;
	constexpr Cartesian operator / ( const double & ) const; 

	// Assignment operators
	constexpr Cartesian & 	operator +=	( const Cartesian & );
	constexpr Cartesian & 	operator -=	( const Cartesian & );
	constexpr Cartesian & 	operator *=	( const Cartesian & );
	constexpr Cartesian & 	operator /=	( const Cartesian & );
	constexpr Cartesian & 	operator +=	( const double & );
	constexpr Cartesian & 	operator -=	( const double & );
	constexpr Cartesian & 	operator *=	( const double & );
	constexpr Cartesian & 	operator /=	( const double & );
	
	constexpr double 		major		( ); // largest absolute-value coordinate
	constexpr double 		minor		( ); // smallest absolute-value coordinate
	
}; // class Cartesian

// Binary elementwise arithmetic
constexpr Cartesian 	Cartesian::operator + ( const Cartesian & other ) const { return Cartesian(x()+other.x(),y()+other.y(),z()+other.z()); };
constexpr Cartesian 	Cartesian::operator - ( const Cartesian & other ) const { return Cartesian(x()-other.x(),y()-other.y(),z()-other.z()); };
constexpr Cartesian 	Cartesian::operator * ( const Cartesian & other ) const { return Cartesian(x()*other.x(),y()*other.y(),z()*other.z()); };
constexpr Cartesian 	Cartesian::operator / ( const Cartesian & other ) const { return Cartesian(x()/other.x(),y()/other.y(),z()/other.z()); };
constexpr Cartesian 	Cartesian::operator + ( const double & a ) const { return Cartesian( x() + a, y() + a, z() + a ); };
constexpr Cartesian 	Cartesian::operator - ( const double & a ) const { return Cartesian( x() - a, y() - a, z() - a ); }; 
constexpr Cartesian 	Cartesian::operator * ( const double & a ) const { return Cartesian( x() * a, y() * a, z() * a ); };
constexpr Cartesian 	Cartesian::operator / ( const double & a ) const { return Cartesian( x() / a, y() / a, z() / a ); }; 

// Assigning operators
constexpr Cartesian & 	Cartesian::operator += ( const Cartesian & v ) { data[0] += v.x(); data[1] += v.y(); data[2] += v.z(); return *this; };
constexpr Cartesian & 	Cartesian::operator -= ( const Cartesian & v ) { data[0] -= v.x(); data[1] -= v.y(); data[2] -= v.z(); return *this; };
constexpr Cartesian & 	Cartesian::operator *= ( const Cartesian & v ) { data[0] *= v.x(); data[1] *= v.y(); data[2] *= v.z(); return *this; };
constexpr Cartesian & 	Cartesian::operator /= ( const Cartesian & v ) { data[0] /= v.x(); data[1] /= v.y(); data[2] /= v.z(); return *this; };
constexpr Cartesian & 	Cartesian::operator += ( const double & a ) { data[0] += a; data[1] += a; data[2] += a; return *this; }
constexpr Cartesian & 	Cartesian::operator -= ( const double & a ) { data[0] -= a; data[1] -= a; data[2] -= a; return *this; }
constexpr Cartesian & 	Cartesian::operator *= ( const double & a ) { data[0] *= a; data[1] *= a; data[2] *= a; return *this; }
constexpr Cartesian & 	Cartesian::operator /= ( const double & a ) { data[0] /= a; data[1] /= a; data[2] /= a; return *this; }

// Major, minor: largest and smallest absolute value of individual coordinate
constexpr double Cartesian::major() { return std::max(std::max(std::abs(x()),std::abs(y())),std::abs(z())); }
constexpr double Cartesian::minor() { return std::min(std::min(std::abs(x()),std::abs(y())),std::abs(z())); }

// Value return
constexpr double x ( const Cartesian & P ) { return P.x(); } 
constexpr double y ( const Cartesian & P ) { return P.y(); }
constexpr double z ( const Cartesian & P ) { return P.z(); }

} // namespace Euclid

#endif
