#ifndef MSG_DECODER_HPP
#define MSG_DECODER_HPP

#include "rtos.hpp"
#include "hwlib.hpp"
#include "pause_listener.hpp"

//this is a class that reads pauses from the pause channel

namespace NEC{
class msg_decoder : public pause_listener, public rtos::task<> {
private:
	ir_msg msg;			//moet nog geinitialiseerd worden, eerst moet ir_msg constructor gemaakt worden
    msg_listener & listener;

public:
    msg_decoder( msg_listener & l ):
        listener( l )
		//msg initialisatie
    {}
	
	void decode();		//verander hier member var msg met static pause channel uit pause_listener superklasse
};
}
#endif //MSG_DECODER_HPP