#include "hwlib.hpp"
#include "rtos.hpp"
#include "pause_detector.hpp"

namespace target = hwlib::target;

int main( void ){
    hwlib::wait_ms( 2000 );
    auto ir_pin = target::pin_in( target::d2_36kHz(); );
    for( ;; ) {
        hwlib::cout << p.read();
    }
	
	auto pause_l = NEC::pause_listener();
	auto pause_d = NEC::pause_detector( pause_l, ir_pin );

    return 0;
}