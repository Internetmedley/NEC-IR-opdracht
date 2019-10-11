#ifndef PAUSE_LISTENER_HPP
#def PAUSE_LISTENER_HPP

#include "ir_msg.hpp"

namespace NEC{
class pause_listener{
protected:
	static rtos::channel<ir_msg, 4> pause_buffer;
public:
    pause_listener():
	{}
};
}
#endif //PAUSE_LISTENER_HPP