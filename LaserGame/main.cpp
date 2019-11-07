#include "hwlib.hpp"
#include "rtos.hpp"
#include "keypad_listener.hpp"
#include "init_game.hpp"
#include "ZRX543.hpp"
#include "reg_game_para.hpp"
#include "run_game.hpp"
#include "ir_sender.hpp"
#include "pause_detector.hpp"
#include "pause_listener.hpp"
#include "msg_decoder.hpp"
//#include "msg_listener.hpp"
#include "msg_logger.hpp"

int main(void){
	// kill the watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	// wait for the PC console to start
	hwlib::wait_ms( 500 );

    namespace target= hwlib::target;

    auto tsop_signal = target::pin_in( target::pins::d8 );
    auto tsop_gnd    = target::pin_out( target::pins::d9 );
    auto tsop_vdd    = target::pin_out( target::pins::d10 );
    tsop_gnd.write( 0 );
    tsop_vdd.write( 1 );
    tsop_gnd.flush();
    tsop_vdd.flush();

	auto out0 				= hwlib::target::pin_oc( hwlib::target::pins::a0 );
	auto out1 				= hwlib::target::pin_oc( hwlib::target::pins::a1 );
	auto out2 				= hwlib::target::pin_oc( hwlib::target::pins::a2 );
	auto out3 				= hwlib::target::pin_oc( hwlib::target::pins::a3 );
	auto in0  				= hwlib::target::pin_in( hwlib::target::pins::a4 );
	auto in1  				= hwlib::target::pin_in( hwlib::target::pins::a5 );
	auto in2  				= hwlib::target::pin_in( hwlib::target::pins::a6 );
	auto in3  				= hwlib::target::pin_in( hwlib::target::pins::a7 );
	auto out_port 			= hwlib::port_oc_from( out0, out1, out2, out3 );
	auto in_port  			= hwlib::port_in_from( in0,  in1,  in2,  in3  );
	auto matrixObj			= hwlib::matrix_of_switches( out_port, in_port );
	auto keypadObj			= hwlib::keypad< 16 >( matrixObj, "D#0*C987B654A321" );

    auto sender             = ir_sender();
	auto init_game_obj		= NEC::init_game("init_game", "key_buffer", "game_leader_flag", sender);
	auto run_game_obj		= NEC::run_game("run_game", 1000 * rtos::ms);
	auto reg_game_para_obj	= NEC::reg_game_para("reg_game_para", "key_buffer", "hits_buffer", "CMD_buffer", init_game_obj, run_game_obj);
	auto keypad_control 	= NEC::ZRX543("keypad", keypadObj, reg_game_para_obj, init_game_obj, run_game_obj);
    //auto parameter_logger   = NEC::msg_logger( "parameter_logger" );
    auto decoder            = NEC::msg_decoder( reg_game_para_obj, run_game_obj, "message_decoder" );
    auto detector           = NEC::pause_detector( tsop_signal, decoder, "pause_detector" );

    (void) sender;
    (void) init_game_obj;
    (void) run_game_obj;
    (void) reg_game_para_obj;
    (void) keypad_control;
    //(void) parameter_logger;
    (void) decoder;
    (void) detector;

	rtos::run();
}
