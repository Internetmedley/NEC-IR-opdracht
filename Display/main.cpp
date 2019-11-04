#include "hwlib.hpp"
#include "pool.hpp"


int main( void ){	
   hwlib::wait_ms( 500 );   // wait for the PC console to start
   
   namespace target = hwlib::target;

   auto scl = target::pin_oc( target::pins::scl );
   auto sda = target::pin_oc( target::pins::sda );   
   auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );
   
   auto oled = hwlib::glcd_oled( i2c_bus, 0x3c );  
      
   auto f1 = hwlib::font_default_16x16();
   auto f2 = hwlib::font_default_8x8();
   
   auto d = hwlib::terminal_from( oled, f2 );   
      
//   int n = 0;   
//   for(;;){
//      hwlib::wait_ms( 1000 );
//      d << "\f" << n++;
//      d << "\t0002" << "score : " << 5 - n;
//      oled.flush();
//   }      
   
      auto log = logger( oled, d );
     auto displa = display( log );
     rtos::run();
      
}

