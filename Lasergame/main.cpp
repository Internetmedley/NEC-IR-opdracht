#include "hwlib.hpp"
#include "rtos.hpp"
#include "keypad_listener.hpp"
#include "init_game.hpp"
#include "ZRX543.hpp"
#include "reg_game_para.hpp"

int main(void){
	// kill the watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	// wait for the PC console to start
	hwlib::wait_ms( 200 );
	
	namespace target= hwlib::target;
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
	auto init_game_obj		= NEC::init_game("init_game", "game_leader_flag");
	auto reg_game_para_obj	= NEC::reg_game_para("reg_game_para", init_game_obj);
	auto keypad_control 	= NEC::ZRX543("keypad", keypadObj, reg_game_para_obj, init_game_obj);
	rtos::run();
}
