#include "hwlib.hpp"
#include "rtos.hpp"
#include "pause_detector.hpp"


int main( void ){
    namespace target = hwlib::target;
    hwlib::wait_ms( 2000 );
    auto ir_pin = target::pin_in( target::pins::d10 );
    for( ;; ){
        hwlib::cout << p.read();
    }
	
	auto pause_l = NEC::msg_listener();
	auto pause_d = NEC::pause_detector( pause_l, ir_pin );
}