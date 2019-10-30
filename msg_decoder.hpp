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

    void pause_detected( int & n ) override {
        n = detector.pause_buffer.read();
        return;
    }

    void main() override {
        for( ;; ) {
            int n = 0;
            pause_detected( n );
            hwlib::cout << n << '\n';
        }
    }


public:
    msg_decoder( pause_detector & d, const char * name ):
        task( name ),
        pause_listener( d )
		//msg initialisatie
    {}
	
	
};
}
#endif //MSG_DECODER_HPP