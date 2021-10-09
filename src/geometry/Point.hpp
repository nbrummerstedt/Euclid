#ifndef EUCLID_GEOMETRY_POINT
#define EUCLID_GEOMETRY_POINT

#include <vector>
#include <array>
#include <numeric>
#include <cassert>
#include <iostream>
#include <math.h> // isnan

namespace Euclid {
	
	class Point
	{	
		private:
			std::array<double,3> data;
			
		public:
		
			constexpr Point	( const Point & P ) : data({{P.x(),P.y(),P.z()}}) {};
			constexpr Point	( const double & x = double(), const double & y = double(), const double & z = double() ) : data({{x,y,z}}) {};
			constexpr Point	( const std::array<double,3> & P ) : data({{P[0],P[1],P[2]}}) {};
			constexpr Point	( const std::array<float,3> & P ) : data({{P[0],P[1],P[2]}}) {};
			
			// Data access by indexing
			// Const index operator []		-- access : return reference
			// Non-const index operator []	-- access : return reference
			// Const index operator ()		-- extraction : should not return a reference
			// Non-const index operator ()	-- extraction : should not return a reference
			const 	double& operator [] ( size_t i ) const 	{ assert(i<3); return data[i]; };
					double& operator [] ( size_t i ) 		{ assert(i<3); return data[i]; };
			const 	double& operator () ( size_t i ) const 	{ assert(i<3); return data[i]; };
					double& operator () ( size_t i ) 		{ assert(i<3); return data[i]; };
			
			// Data extract (const) using identifiers
			constexpr double x	() 				const { return data[0]; };
			constexpr double y	() 				const { return data[1]; };
			constexpr double z	() 				const { return data[2]; };
			
			// Iterators
			array<double,3>::iterator 		begin() 		{ return data.begin(); }
			array<double,3>::iterator 		end() 			{ return data.end(); }
			double* 						get() 			{ return data.data(); }
			array<double,3>::const_iterator begin() const 	{ return data.begin(); }
			array<double,3>::const_iterator end() 	const 	{ return data.end(); }
			const double* 					get() 	const 	{ return data.data(); }
	
			// A point is empty if any coordinate is unknown
			constexpr bool empty() { return ( isnan(data[0])|| isnan(data[1]) || isnan(data[2]) ); };
			
			// Data assignment by index
			constexpr Point &   set	( size_t i , double a ) { assert(i<3); data[i] = a; return *this; };
			
			// Data assignment by istream
			// Warning: This is by no means robust yet.
			friend std::istream & operator >> ( std::istream & in, Point & p)
			{
				in >> std::ws;
				if (in.peek() == '[') in.ignore();
				in >> std::ws;
				for (int c=0; c<3; ++c) in >> p(c) >> std::ws;
				if (in.peek() == ']') in.ignore();
				return in;
			}
			
			constexpr Point & 		operator = 	( const Point & );
			
			constexpr Point 		operator - 	() { return Point( -x(),-y(),-z() ); };
			
			constexpr Point 		operator + 	( const Point & );
			constexpr Point 		operator - 	( const Point & );
			constexpr Point & 		operator += ( const Point & );
			constexpr Point & 		operator -= ( const Point & );
			constexpr Point 		operator + 	( const double & );
			constexpr Point 		operator - 	( const double & );
			constexpr Point 		operator * 	( const double & );
			constexpr Point 		operator / 	( const double & );
			constexpr Point & 		operator += ( const double & );
			constexpr Point & 		operator -= ( const double & );
			constexpr Point & 		operator *= ( const double & );
			constexpr Point & 		operator /= ( const double & );
			
			constexpr double 		norm		( ); // magnitude squared
			constexpr double 		abs			( ); // magnitude
			constexpr double 		major		( ); // largest absolute-value coordinate
			constexpr double 		minor		( ); // smallest absolute-value coordinate
			
			constexpr bool all_l  (const Point &) const;
			constexpr bool all_le (const Point &) const;
			constexpr bool all_g  (const Point &) const;
			constexpr bool all_ge (const Point &) const;
			
			// Print
			friend std::ostream & 	operator << ( std::ostream & out, const Point & p ) 
			{
				return out << p.x() << "," << p.y() << "," << p.z() << ";";
			};
			friend std::ostream & 	operator << ( std::ostream & out, std::vector<Point> v )
			{
				for ( Point p : v ) out << "\n\t" << p;
				out << std::endl;
				return out;
			};
			
	}; // class Point
	
	// Major, minor: largest and smallest absolute value of individual coordinate
	constexpr double Point::major() { return std::max(std::max(std::abs(x()),std::abs(y())),std::abs(z())); }
	constexpr double Point::minor() { return std::min(std::min(std::abs(x()),std::abs(y())),std::abs(z())); }
	
	// Magnitude, distance to origin, square root of norm
	constexpr double Point::abs() 	{ return (major()==double())?(double()):(sqrt(norm())); }
	
	// Norm, sum of coordinates squared
	constexpr double Point::norm() 	{ return data[0]*data[0] + data[1]*data[1] + data[2]*data[2]; }
	
	// Scalar arithmetic 
	constexpr Point 	Point::operator + ( const double & a ) { return Point( x() + a, y() + a, z() + a ); }
	constexpr Point 	Point::operator - ( const double & a ) { return Point( x() - a, y() - a, z() - a ); }
	constexpr Point 	Point::operator * ( const double & a ) { return Point( x() * a, y() * a, z() * a ); }
	constexpr Point 	Point::operator / ( const double & a ) { return Point( x() / a, y() / a, z() / a ); }
	
	// Point arithmetic 
	constexpr Point 	Point::operator + ( const Point & P )  { return Point( x() + P.x(), y() + P.y(), z() + P.z() ); }
	constexpr Point 	Point::operator - ( const Point & P )  { return Point( x() - P.x(), y() - P.y(), z() - P.z() ); }
	
	// Short scalar arithmetic
	constexpr Point & 	Point::operator += ( const double & a ) { data[0] += a; data[1] += a; data[2] += a; return *this; }
	constexpr Point & 	Point::operator -= ( const double & a ) { data[0] -= a; data[1] -= a; data[2] -= a; return *this; }
	constexpr Point & 	Point::operator *= ( const double & a ) { data[0] *= a; data[1] *= a; data[2] *= a; return *this; }
	constexpr Point & 	Point::operator /= ( const double & a ) { data[0] /= a; data[1] /= a; data[2] /= a; return *this; }
	
	// Short Point arithmetic
	constexpr Point & 	Point::operator += ( const Point & P ) { data[0] += P.x(); data[1] += P.y(); data[2] += P.z(); return *this; }
	constexpr Point & 	Point::operator -= ( const Point & P ) { data[0] -= P.x(); data[1] -= P.y(); data[2] -= P.z(); return *this; }
	constexpr Point & 	Point::operator =  ( const Point & P ) { data[0] =  P.x(); data[1] =  P.y(); data[2] =  P.z(); return *this; }
	
	// Comparison from class
	constexpr bool Point::all_l  (const Point & p) const { return (x() <  p.x() && y() <  p.y() && z() <  p.z() ); }
	constexpr bool Point::all_le (const Point & p) const { return (x() <= p.x() && y() <= p.y() && z() <= p.z() ); }
	constexpr bool Point::all_g  (const Point & p) const { return (x() >  p.x() && y() >  p.y() && z() >  p.z() ); }
	constexpr bool Point::all_ge (const Point & p) const { return (x() >= p.x() && y() >= p.y() && z() >= p.z() ); }
	
	// Comparison
	inline constexpr bool 	operator ==	( const Point & A, const Point & B ) { return A.x() == B.x() && A.y() == B.y() && A.z() == B.z(); }
	inline constexpr bool 	operator != ( const Point & A, const Point & B ) { return A.x() != B.x() || A.y() != B.y() || A.z() != B.z(); } 
	
	// Unary arithmetic 
	inline constexpr Point 	operator + 	( const Point & P ) { return P; } 
	inline constexpr Point 	operator - 	( const Point & P ) { return Point( -P.x(),-P.y(),-P.z() ); } 
	
	// Arithmetic
	inline constexpr Point operator + ( const Point  & A, const Point  & B) { Point P =  A; P += B; return P; } 
	inline constexpr Point operator + ( const Point  & A, const double & a) { Point P =  A; P += a; return P; } 
	inline constexpr Point operator + ( const double & a, const Point  & B) { Point P =  B; P += a; return P; } 
	
	inline constexpr Point operator - ( const Point  & A, const Point  & B) { Point P =  A; P -= B; return P; } 
	inline constexpr Point operator - ( const Point  & A, const double & b) { Point P =  A; P -= b; return P; } 
	inline constexpr Point operator - ( const double & a, const Point  & B) { Point P = -B; P += a; return P; } 
	
	inline constexpr Point operator * ( const Point  & A, const double & b) { Point P =  A; P *= b; return P; } 
	inline constexpr Point operator * ( const double & a, const Point  & B) { Point P =  B; P *= a; return P; }
	
	inline constexpr Point operator / ( const Point  & A, const double & b) { Point P =  A; P /= b; return P; } 
	inline constexpr Point operator / ( const double & a, const Point  & B) { Point P =  B; P /= a; return P; } 
	
	// Value return
	constexpr double x ( const Point & P ) { return P.x(); } 
	constexpr double y ( const Point & P ) { return P.y(); }
	constexpr double z ( const Point & P ) { return P.z(); }
	
	// Argument, angle, with respect to axes in cartesian coordinate system
	std::array<double,3> 	arg ( const Point & ); // angle to axes
	double 					arg ( const Point & , int plane ); // angle above specified plane
	
	// Returns the elementwise smallest value
	inline constexpr Point 
	emin ( const Point & v1 , const Point & v2 ) 
	{
		using std::min;
		return Point {min(v1.x(),v2.x()),min(v1.y(),v2.y()),min(v1.z(),v2.z())};
	}
	// Returns the elementwise largest value
	inline constexpr Point 
	emax ( const Point & v1 , const Point & v2 ) 
	{
		using std::max;
		return Point {max(v1.x(),v2.x()),max(v1.y(),v2.y()),max(v1.z(),v2.z())};
	}
	
} // namespace Euclid

#endif
