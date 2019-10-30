#ifndef PAUSE_LISTENER_HPP
#define PAUSE_LISTENER_HPP

#include "pause_detector.hpp"


namespace NEC{
class pause_listener {
protected:
	pause_detector & detector;

	virtual void pause_detected( int & n ) = 0;

public:
	pause_listener( pause_detector & d ):
		detector( d )
	{}
	
};
}
#endif //PAUSE_LISTENER_HPP