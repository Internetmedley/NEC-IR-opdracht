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
#include "msg_listener.hpp"
#include "display.hpp"
#include "beeper.hpp"

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

    auto scl                = target::pin_oc( target::pins::scl );
    auto sda                = target::pin_oc( target::pins::sda );
    auto i2c_bus            = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda ); //maakt i2cbus aan

    auto oled               = hwlib::glcd_oled( i2c_bus, 0x3c );  //maakt oled object aan
    auto font               = hwlib::font_default_8x8();  //letter grootte
    auto window             = hwlib::terminal_from( oled, font );   //maakt terminal aan
    oled.clear();
    oled.flush();

    auto pin_o              = hwlib::target::pin_out(hwlib::target::pins::d7);
    auto bee                = NEC::Beeper(pin_o, 1000);

    auto display            = NEC::Display( oled, window );

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
    auto init_game_obj		= NEC::init_game("init_game", "key_buffer", "game_leader_flag", sender, display);
    auto run_game_obj		= NEC::run_game("run_game", "key_buffer", "int_buffer", "start_game_flag", 1000 * rtos::ms, "game_clock", sender, display, bee);
    auto reg_game_para_obj	= NEC::reg_game_para("reg_game_para", "key_buffer", "hits_buffer", "CMD_buffer", init_game_obj, run_game_obj, display);
    auto keypad_control 	= NEC::ZRX543("keypad", keypadObj, reg_game_para_obj, init_game_obj, run_game_obj);
    auto decoder            = NEC::msg_decoder( reg_game_para_obj, run_game_obj, "message_decoder" );
    auto detector           = NEC::pause_detector( tsop_signal, decoder, "pause_detector" );

    rtos::run();
}
