#ifndef MSG_DECODER_HPP
#define MSG_DECODER_HPP

#include "rtos.hpp"
#include "hwlib.hpp"
#include "pause_listener.hpp"

//this is a class that reads pauses from the pause channel

namespace NEC{
class msg_decoder : public rtos::task<>, public pause_listener {
private:
	//ir_msg msg;			//moet nog geinitialiseerd worden, eerst moet ir_msg constructor gemaakt worden
    rtos::channel< int, 32 > pause_buffer;		        //schrijft pauze duraties

    void main() override {
        for( ;; ) {
            auto c = pause_buffer.read();
            hwlib::cout << c << '\n';
        }
    }

public:
    msg_decoder( const char * name ):
        task( name ),
            pause_buffer( this, "pause_buffer" )
		    //msg initialisatie
    {}

    void pause_detected( const int & dur ) override {
        pause_buffer.write( dur );
    }
	
};
}
#endif //MSG_DECODER_HPP