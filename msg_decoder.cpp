#include "msg_decoder.hpp"


//Fill array with channel data
//Check array values and write either 0 or 1 into channel

void NEC::msg_decoder::main() {
	int pause_dur;
	uint16_t message;

	for( ;; ) {
		for( uint16_t n = 0x01; n < __UINT16_MAX__; n <<= 0x01 ) {		//channel begint bij de LSB
			pause_dur = pause_buffer.read();
			if( pause_dur > 500 && pause_dur < 600 ) {				//560us pauze is een 0
				message &= ~n;										//unset de bit op de plaats van n
			}
			else if( pause_dur > 1500 && pause_dur < 1800 ){		//1690us pauze is een 1
				message |= n;										//set de bit op de plaats van n
			}
			else if( pause_dur > 4400 ){							//4500us pauze is een startsignaal, dus reset de message
				message = 0;
			}
		}
		if( (message & 0x3E) ^ (message & 0x7C) == (message & 0xF8) {			//checksum
			(message & 0x3E) == 0 ? cmd_listener.msg_received( message ) : hit_listener.msg_received( message );
		}
	}
}
