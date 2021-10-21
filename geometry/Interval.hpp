#ifndef EUCLID_INTERVAL
#define EUCLID_INTERVAL

namespace Euclid {
	
	template<class ContentType>
	class Interval {
			ContentType _v1;
			ContentType _v2;
		public:
			Interval ( ContentType & a, ContentType & b ) : _v1(a), _v2(b) {};
			Interval ( ContentType a  = ContentType(), ContentType b  = ContentType()) : _v1(a), _v2(b) {};
			constexpr ContentType smallest () const { return _v1; }
			constexpr ContentType largest  () const { return _v2; }
	};
	
}

#endif
