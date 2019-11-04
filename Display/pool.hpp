#include "hwlib.hpp"
#include "rtos.hpp"

class logger : public rtos::task<>{
private:
   hwlib::glcd_oled & oled;
   hwlib::terminal_from & d; 
   rtos::mutex display_mutex;
   rtos::pool< int > display;
   
   void main(){
       
      for(;;){
         hwlib::wait_ms( 1'000 );
         display_mutex.wait();
         oled.clear();
         d << "\f" <<  display.read();
         oled.flush();
         display_mutex.signal();         
      }
   }
   
public:
   logger( auto & oled, auto & d ):
      
      task( "logger" ),
      oled( oled ),
      d( d ),
      display_mutex( "display_mutex" ),
      display( "display_pool" )
   {
         display.write( 0 );
   }   
   
   
   void increment(){
      display_mutex.wait();
      display.write( display.read() + 1 );
      display_mutex.signal();      
   }
};

class display : public rtos::task<>{
private:
    logger & log;
    
    void main(){
      for(;;){
         hwlib::wait_ms( 800 );
         log.increment();
      }
   }
public:
   display( 
      logger & log
   ):
      task( "display" ),
      log( log )
   {}   
};

