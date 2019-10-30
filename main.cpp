#include "pause_detector.hpp"
#include "pause_listener.hpp"
#include "msg_decoder.hpp"
#include "msg_listener.hpp"

namespace target = hwlib::target;

int main( void ){
    hwlib::wait_ms( 2000 );

    auto tsop_signal = target::pin_in( target::pins::d8 );
	auto tsop_gnd    = target::pin_out( target::pins::d9 );
    auto tsop_vdd    = target::pin_out( target::pins::d10 );
    tsop_gnd.write( 0 );
    tsop_vdd.write( 1 );
    tsop_gnd.flush();
    tsop_vdd.flush();

    //auto led = target::pin_out( target::pins::d4 );
    auto pause_d = NEC::pause_detector( tsop_signal, "pause_detector" );
    auto decoder = NEC::msg_decoder( pause_d, "message_decoder" );
    (void) pause_d;
    (void) decoder;

    rtos::run();

    return 0;
}