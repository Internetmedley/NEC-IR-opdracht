#include "pause_detector.hpp"

void NEC::pause_detector::main() {
    state = states::NO_MESSAGE;
    auto sig_start = hwlib::now_us();
    auto pause_start = hwlib::now_us();
    //auto begin = hwlib::now_us();
    int pause_dur = 0;
    

    for( ;; ) {
        receiver.refresh();
        if( state == states::NO_MESSAGE && !receiver.read() ) {              //de IR receiver is active-low!
            sig_start = hwlib::now_us();
            state = states::SIGNAL;
            pause_timer.set( 500 * rtos::us );                        //minimale signaal duratie is 560, dus wacht ~500 als signaal gemeten is
            //hwlib::cout << "wait 1" << '\n';
            wait( pause_timer );
        }
        if( state == states::SIGNAL && !receiver.read()  ) {
            if( hwlib::now_us() - sig_start > 560 ) {    
                //hwlib::wait_us( (sig_start + 9000) - hwlib::now_us() );    //wacht 9ms als startsignaal aan is
                pause_timer.set( (hwlib::now_us() + 9000 - sig_start) * rtos::us );
                //hwlib::cout << "wait 2" << '\n';
                wait( pause_timer );
            }
        }
        if( state == states::SIGNAL && receiver.read() ) {         //signaal voorbij
            pause_start = hwlib::now_us();
            state = states::PAUSE;
            pause_timer.set( 500 * rtos::us );                          //minimale pauzeduratie is 560, dus wacht ~500
            //hwlib::cout << "wait 3" << '\n';
            wait( pause_timer );
        }
        if( state == states::PAUSE && !receiver.read() ) {           //pauze voorbij
            pause_dur = hwlib::now_us() - pause_start;         
            listener.pause_detected( pause_dur );           //deze virtuele functie wordt overridden in decoder en schrijft duratie naar de channel in de listener
            sig_start = hwlib::now_us();
            state = states::SIGNAL;
            pause_timer.set( 500 * rtos::us );
            //hwlib::cout << "wait 4" << '\n';
            wait( pause_timer );
        }

        if( state == states::PAUSE && receiver.read() ) {
            if( hwlib::now_us() - pause_start > 4500 ) {
                state = states::NO_MESSAGE;
            }
            if( hwlib::now_us() - pause_start > 1690 ) {
                //hwlib::wait_us( (pause_start + 4500) - hwlib::now_us() );   //pauze langer dan 1690 seconden indiceert startpauze van 4.5ms of einde van message
                pause_timer.set( (hwlib::now_us() + 4500 - pause_start) * rtos::us );  //pauze langer dan 1690 seconden indiceert startpauze van 4.5ms of einde van message
                //hwlib::cout << "wait 5 " << (pause_start - hwlib::now_us() + 4500) * rtos::us << '\n';
                //hwlib::cout << pause_start << ' ' << hwlib::now_us() << ' ' << rtos::ms << '\n';
                wait( pause_timer );
                //hwlib::cout << "lol" << '\n';
            }
        }
    }
}