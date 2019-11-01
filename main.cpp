#include "pause_detector.hpp"
#include "pause_listener.hpp"
#include "msg_decoder.hpp"
#include "msg_listener.hpp"
#include "msg_logger.hpp"

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
    auto parameter_logger       = NEC::msg_logger( "parameter_logger" );
    //auto game_logger            = NEC::msg_logger( "game_logger" );
    auto decoder                = NEC::msg_decoder( parameter_logger/*, game_logger*/, "message_decoder" );
    auto detector               = NEC::pause_detector( tsop_signal, decoder, "pause_detector" );
    (void) parameter_logger;
    //(void) game_logger;
    (void) decoder;
    (void) detector;
    
    rtos::run();

    return 0;
}