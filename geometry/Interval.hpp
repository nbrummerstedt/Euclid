#ifndef EUCLID_INTERVAL
#define EUCLID_INTERVAL

namespace Euclid {
	
	template<class ContentType>
	class Interval {
		public :
			ContentType smallest;
			ContentType largest;
			//array<bool,2> openclosed;
	};
	
}

#endif
