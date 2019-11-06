#include "msg_decoder.hpp"

/// @file

/// \brief
///	Checks the decoded message by our protocol.
/// \details
///	This member function takes a decoded message and checks whether the last 5 bits
/// equals an XOR of bits 2 through 11 and if this is true it will send the message to
/// the correct listener.
/// If the player number in the received message is player 0, the message is a command instead of a hit and
/// the message will be sent to the object that listens to the commands.

bool NEC::msg_decoder::is_valid( uint16_t msg ) {
    if( ((msg & 0x3E) ^ (msg & 0x7C0) >> 5) == (msg & 0xF800) >> 10) {         //0x3E is player section, 0x7C0 is data section, 0xF800 is checksum section
        return true;
    }
    else{
        return false;
    }
}



void NEC::msg_decoder::send_message( uint16_t msg ) {
    if( (msg & 0x3E) == 0 ) {                               //als speler nr een 0 is, is het een commando, anders een hit
        cmd_l.msg_received( (msg & 0x7C0) >> 6 );            //stuurt alleen data section
    }
    else {
        hit_l.msg_received( (msg & 0x7FE) >> 1 );            //dit stuurt alleen data en speler nummer
//        cmd_l.msg_received( (msg & 0x7C0) >> 1 );
    }

}
/// \brief
/// Message decoder main loop.
/// \details
/// This main loop is a member function of the message decoder that will read its pause duration channel to
/// decode a message from the pause_durations.
/// A measured pause of 560 micro-seconds between IR signal pulses
/// indicates a 0-bit, and a 1690 micro-seconds pause indicates a 1-bit.
/// A decoded message consists of 16 bits.

void NEC::msg_decoder::main() {
    state = states::WAITING_FOR_START_PAUSE;
    uint16_t decoded_msg;
    int pause_dur;

    for( ;; ) {
        switch( state ) {
            case states::WAITING_FOR_START_PAUSE:
                pause_dur = pause_buffer.read();
                if( pause_dur > 4000 && pause_dur < 5000 ) {			//4500us pauze is een startsignaal, dus reset de message
                    state = states::DECODING;
                }
                else if( pause_dur > 2200 && pause_dur < 2800 ) {		//indiceert herhalingssignaal, dus stuur laatste decoded message
                    send_message( decoded_msg );
                }
                break;
            case states::DECODING:
                decoded_msg = 0x00;
                for( uint32_t n = 0x01; n < __UINT16_MAX__; n <<= 1 ) {		//channel begint bij de LSB
                    pause_dur = pause_buffer.read();
                    if( pause_dur > 1390 && pause_dur < 1990 ) {		//1690us pauze is een 1
                        decoded_msg |= n;										//set de bit op de plaats van n
                    }
                    else if( !((pause_dur > 260) && (pause_dur < 860)) ) {
                        state = states::WAITING_FOR_START_PAUSE;
                        break;
                    }
                }

                send_message( decoded_msg );
                state = states::WAITING_FOR_START_PAUSE;
                break;
        }
    }
}
