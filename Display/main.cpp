#include "hwlib.hpp"
#include "rtos.hpp"

class Display : public rtos::task<>{
private:
    hwlib::glcd_oled & oled;
    hwlib::terminal_from & window;
    
    rtos::channel< int, 8 > state_decider;
    
    void main(){
        for(;;){
            int state = state_decider.read();
            oled.clear();
            int time;
            switch( state ){
                case 1:
                    time = state_decider.read();
                    window << "\f Gegeven tijd: \n"
                             << time << " \n";
                    oled.flush();
                    break;
                case 2:
                    window << "\f case 2";
                    oled.flush();
                    break;
                case 3:
                    window << "\f simpel text bericht. \n";
                    oled.flush();
                    break;
                case 4:
                    window << "\f case 4";
                    oled.flush();
                    break;
            }
        }
    }
    
public:
    Display( auto & oled, auto & window): 
    task( "Display" ),
    oled( oled ),
    window( window ),
    state_decider( this, "state decider" )
   {}
   
   void write_int_to_channel(int i){
       state_decider.write( i );
   }
};

int main(){	
   hwlib::wait_ms( 500 );   // wait for the PC console to start
   
   namespace target = hwlib::target;

   auto scl = target::pin_oc( target::pins::scl );
   auto sda = target::pin_oc( target::pins::sda );   
   auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda ); //maakt i2cbus aan
   
   auto oled = hwlib::glcd_oled( i2c_bus, 0x3c );  //maakt oled object aan
      
   auto font = hwlib::font_default_8x8();  //letter grootte
   
   auto window = hwlib::terminal_from( oled, font );   //maakt terminal aan
   
   auto display = Display( oled, window );
   display.write_int_to_channel(1);
   display.write_int_to_channel(60);
   display.write_int_to_channel(2);
   display.write_int_to_channel(3);
   display.write_int_to_channel(4);
   rtos::run();
}

