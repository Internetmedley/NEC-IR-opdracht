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
        rtos::channel< int, 16 > pause_buffer;		        //schrijft pauze duraties (16 is 15 pauzes voor de bits plus 1 voor de start pauze)
        msg_listener & cmd_l;
        msg_listener & hit_l;
        enum class states { WAITING_FOR_START_PAUSE, DECODING };
        states state;

        void send_message( uint16_t msg );                  //schrijft een message vanuit de main naar de juiste listener

        bool is_valid( uint16_t msg );

        void main() override;

    public:
        msg_decoder( msg_listener & c_l, msg_listener & h_l, const char * name ):
                task( name ),
                pause_buffer( this, "pause_buffer" ),
                cmd_l( c_l ),
                hit_l( h_l )
        {}

        void pause_detected( const int & dur ) override {
            pause_buffer.write( dur );
        }
    };
}
#endif //MSG_DECODER_HPP