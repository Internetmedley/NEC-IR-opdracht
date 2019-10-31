#ifndef MSG_DECODER_HPP
#define MSG_DECODER_HPP

#include "rtos.hpp"
#include "hwlib.hpp"
#include "pause_listener.hpp"
#include "msg_listener.hpp"

//this is a class that reads pauses from the pause channel

namespace NEC{
class msg_decoder : public rtos::task<>, public pause_listener {
private:
    rtos::channel< int, 33 > pause_buffer;		        //schrijft pauze duraties (33 is 32 pauzes plus 1 voor de start pauze)
    msg_listener & cmd_listener;
    msg_listener & hit_listener;


    void main() override;

public:
    msg_decoder( msg_listener & c_l, msg_listener & h_l, const char * name ):
        task( name ),
            pause_buffer( this, "pause_buffer" ),
            cmd_listener( c_l ),
            hit_listener( h_l )
    {}

    void pause_detected( const int & dur ) override {
        pause_buffer.write( dur );
    }
	
};
}
#endif //MSG_DECODER_HPP