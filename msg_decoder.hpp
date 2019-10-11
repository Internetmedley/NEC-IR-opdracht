#ifndef MSG_DECODER_HPP
#def MSG_DECODER_HPP

#include "rots.hpp"
#include "hwlib.hpp"
//this is a class that reads pauses from the pause channel

namespace NEC{
class msg_decoder : public pause_listener{
private:
    pause_listener listener;
	rtos::channel<uint8_t , 4> buffer;
public:
    msg_decoder( msg_listener & l ):
        listener( l )
    {}
    
};
}
#endif //MSG_DECODER_HPP