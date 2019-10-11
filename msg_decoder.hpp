#ifndef MSG_DECODER_HPP
#def MSG_DECODER_HPP

<<<<<<< HEAD
#include "rots.hpp"
#include "hwlib.hpp"
=======
#include "pause_listener.hpp"

>>>>>>> 889be729213a1f9e2475457be5f0e0c941f7b4c3
//this is a class that reads pauses from the pause channel

namespace NEC{
class msg_decoder : public pause_listener, public rtos::task<> {
private:
<<<<<<< HEAD
    pause_listener listener;
	rtos::channel<uint8_t , 4> buffer;
=======
    msg_listener & listener;
>>>>>>> 889be729213a1f9e2475457be5f0e0c941f7b4c3
public:
    msg_decoder( msg_listener & l ):
        listener( l )
    {}
    
};
}
#endif //MSG_DECODER_HPP