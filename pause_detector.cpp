#include "pause_detector.hpp"

void NEC::pause_detector::main() {
    state = states::NO_MESSAGE;
    auto sig_start = hwlib::now_us();
    auto pause_start = hwlib::now_us();
    //auto begin = hwlib::now_us();
    int pause_dur = 0;

    for( ;; ) {
        /*if( (begin + 10000000) > hwlib::now_us() ){
            begin = hwlib::now_us();
            for( int i = 0; i < 16; i++ ){
                hwlib::cout << pause_buffer.read() << '\n';
            }
        }*/
        if( state == states::NO_MESSAGE && receiver.read() ) {
            sig_start = hwlib::now_us();
            state = states::SIGNAL;
            hwlib::wait_us( 500 );                          //minimale signaal duratie is 560, dus wacht ~500 als signaal gemeten is
        }
        if( state == states::SIGNAL && receiver.read()  ) {
            if( sig_start - hwlib::now_us() > 560 ) {    
                hwlib::wait_us( (sig_start + 9000) - hwlib::now_us() );    //wacht 9ms als startsignaal aan is
            }
        }
        if( state == states::SIGNAL && !receiver.read() ) {         //signaal voorbij
            pause_start = hwlib::now_us();
            state = states::PAUSE;
            hwlib::wait_us( 500 );                          //minimale pauzeduratie is 560, dus wacht ~500
        }
        if( state == states::PAUSE && receiver.read() ) {           //pauze voorbij
            pause_dur = pause_start - hwlib::now_us();          
            pause_buffer.write( pause_dur );     //schrijf duratie naar de channe

            sig_start = hwlib::now_us();
            state = states::SIGNAL;
            hwlib::wait_us( 500 );
        }

        if( state == states::PAUSE && !receiver.read() ) {
            if( pause_start - hwlib::now_us() > 4500 ) {
                state = states::NO_MESSAGE;
            }
            if( pause_start - hwlib::now_us() > 1690 ) {
                hwlib::wait_us( (pause_start + 4500) - hwlib::now_us() );   //pauze langer dan 1690 seconden indiceert startpauze van 4.5ms of einde van message
            }
        }
    }
}