#include "pause_detector.hpp"

/// @file

/// \brief
/// Main loop that detects pause durations
/// \brief
/// This main loop member function of pause detector reads from an IR receiver (output active low) and
/// measures the pause durations betweens received IR signals. These measured durations are then written to its pause_listener to
/// be decoded by the pause_listener( msg_decoder ). 
void NEC::pause_detector::main() {
    state = states::NO_MESSAGE;
    auto pause_start = hwlib::now_us();
    int pause_dur = 0;

    for( ;; ) {
        receiver.refresh();
        switch( state ) {

            case states::NO_MESSAGE:
                if( !receiver.read() ){                             //de IR receiver is active-low!
                    state = states::SIGNAL;                         //minimale signaal duratie is 560, dus wacht ~500 als signaal gemeten is
                }
                hwlib::wait_us( 200 );
                break;
            case states::SIGNAL:
                if( receiver.read() ){                                               //signaal voorbij
                    pause_start = hwlib::now_us();
                    state = states::PAUSE;
                }
                hwlib::wait_us( 200 );
                break;
            case states::PAUSE:
                if( !receiver.read() ){                                         //pauze voorbij
                    pause_dur = hwlib::now_us() - pause_start;
                    listener.pause_detected( pause_dur );           //deze virtuele functie wordt overridden in decoder en schrijft duratie naar de channel in de listener
                    state = states::SIGNAL;
                }
                else{
                    if( hwlib::now_us() - pause_start > 6500 ) {    //message is hoe dan ook afegelopen
                        state = states::NO_MESSAGE;
                        break;
                    }
                }
                hwlib::wait_us( 200 );
                break;
        }
    }
}