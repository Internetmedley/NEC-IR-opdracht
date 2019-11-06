#ifndef PAUSE_LISTENER_HPP
#define PAUSE_LISTENER_HPP

namespace NEC{
class pause_listener {
public:
	virtual void pause_detected( const int & dur ) = 0;

	pause_listener() {}

};
}
#endif //PAUSE_LISTENER_HPP