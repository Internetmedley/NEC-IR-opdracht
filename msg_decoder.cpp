#include "msg_decoder.hpp"


//Fill array with channel data
//Check array values and write either 0 or 1 into channel

void NEC::msg_decoder::send_message( uint16_t msg ) {
	if( ((msg & 0x3E) ^ (msg & 0x7C)) == (msg & 0xF8) ) {			//checksum
		if( (msg & 0x3E) == 0 ) {                                //als speler nr een 0 is, is het een commando, anders een hit
			cmd_listener.msg_received( msg );
		}
		else { 
			//hit_listener.msg_received( msg );      
		}
    }
}

void NEC::msg_decoder::main() {
	state = states::WAITING_FOR_START_PAUSE;
	uint16_t decoded_msg;
	int pause_dur;

	for( ;; ) {
		switch( state ) {
			case states::WAITING_FOR_START_PAUSE:
				pause_dur = pause_buffer.read();
				if( pause_dur > 4000 ) {								//4500us pauze is een startsignaal, dus reset de message
					decoded_msg = 0x01;									//eerste bit is altijd 1
					//hwlib::cout << "waiting for pause start" << '\n';
					state = states::DECODING;
				}
				else if( pause_dur > 2200 && pause_dur < 2800 ) {		//indiceert herhalingssignaal, dus stuur laatste decoded message
					//send_message( decoded_msg );
					hwlib::cout << decoded_msg << '\n';
					
				}
				break;
			case states::DECODING:
				for( int n = 0x02; n < __UINT16_MAX__; n <<= 0x01 ) {		//channel begint bij de LSB
					hwlib::cout << "decoding" << '\n';
					pause_dur = pause_buffer.read();
					if( pause_dur > 400 && pause_dur < 700 ) {				//560us pauze is een 0
						decoded_msg &= ~n;										//unset de bit op de plaats van n
					}
					else if( pause_dur > 1400 && pause_dur < 2000 ) {		//1690us pauze is een 1
						decoded_msg |= n;										//set de bit op de plaats van n
					}
				}
				//hwlib::cout << "done" << '\n';
				//send_message( decoded_msg );
				//hwlib::cout << decoded_msg << '\n';
				cmd_listener.msg_received( decoded_msg );
				state = states::WAITING_FOR_START_PAUSE;
				break;
		}
	}
}
