#include "pause_detector.hpp"

void NEC::pause_detector::main() {
    state = states::NO_MESSAGE;
    auto sig_start = hwlib::now_us();
    auto pause_start = hwlib::now_us();
    //auto begin = hwlib::now_us();
    int pause_dur = 0;
    

    for( ;; ) {
        receiver.refresh();
        switch( state ) {
            case states::NO_MESSAGE:
                if( !receiver.read() ){                                       //de IR receiver is active-low!
                    sig_start = hwlib::now_us();
                    state = states::SIGNAL;
                    pause_timer.set( 500 * rtos::us );                        //minimale signaal duratie is 560, dus wacht ~500 als signaal gemeten is
                    wait( pause_timer );
                }
                break;
            case states::SIGNAL:
                if( !receiver.read() ){
                    if( hwlib::now_us() - sig_start > 560 ) {
                        pause_timer.set( (hwlib::now_us() + 9000 - sig_start) * rtos::us );         //wacht 9ms als startsignaal aan is
                        wait( pause_timer );
                    }
                }
                else{                                                           //signaal voorbij
                    pause_start = hwlib::now_us();
                    state = states::PAUSE;
                    pause_timer.set( 500 * rtos::us );                          //minimale pauzeduratie is 560, dus wacht ~500
                    wait( pause_timer );
                }
                break;
            case states::PAUSE:
                if( !receiver.read() ){                                         //pauze voorbij
                    pause_dur = hwlib::now_us() - pause_start;         
                    listener.pause_detected( pause_dur );           //deze virtuele functie wordt overridden in decoder en schrijft duratie naar de channel in de listener
                    sig_start = hwlib::now_us();
                    state = states::SIGNAL;
                    pause_timer.set( 500 * rtos::us );
                    wait( pause_timer );
                }
                else{
                    if( hwlib::now_us() - pause_start > 4500 ) {    //message is hoe dan ook afegelopen
                    state = states::NO_MESSAGE;
                    }
                    if( hwlib::now_us() - pause_start > 1690 ) {    //pauze langer dan 1690 seconden indiceert startpauze van 4.5ms of einde van message
                    pause_timer.set( (hwlib::now_us() + 4500 - pause_start) * rtos::us );  //pauze langer dan 1690 seconden indiceert startpauze van 4.5ms of einde van message
                    //hwlib::cout << "wait 5 " << (pause_start - hwlib::now_us() + 4500) * rtos::us << '\n';
                    //hwlib::cout << pause_start << ' ' << hwlib::now_us() << ' ' << rtos::ms << '\n';
                    wait( pause_timer );
                    //hwlib::cout << "lol" << '\n';
                    }
                }
                break;           
        }
    }
}