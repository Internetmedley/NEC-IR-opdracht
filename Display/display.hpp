#ifndef DISPLAY_HPP
#define DISPLAY_HPP

/// @file

/// \brief
///     displaying oled class 
/// \details
/// this is a class with a switch case, that writes things to the display.
/// the first number written in the channel is the state. the second depending on the case is a variabel or a new state.
/// case 2, case 3 and case 6 set variabels: player_number, fire_power, game_time.
/// case 10 displays the time that counts down, to give a reliabel showing of the time it wil need to be reactivated for the same amount of time.
/// the other cases starting with 1, ending with 11 write something to the display
/// the default constructor initializes the task, channel, oled and the window from the oled. 
/// the write_int_to_channel function writes to the channel.


namespace NEC{
class Display : public rtos::task<>{
private:
    hwlib::glcd_oled & oled;
    hwlib::terminal_from & window;
    int player_number;
    int fire_power;
    int game_time;
    
    rtos::channel< int, 8 > state_decider;
    
    void main(){
        for(;;){
            int state = state_decider.read();
            oled.clear();
            int time;
            switch( state ){
                case 1:
                    time = state_decider.read();
                    window << "\f Press A for PlayerID, \n"
                           << " Press B for firepower \n";
                    oled.flush();
                    break;
                case 2:
                    window << "\f Press 0-9 for PlayerID. \n";
                    player_number = state_decider.read();
                    oled.flush();
                    break;
                case 3:
                    window << "\f Press 1-9 for firepower. \n";
                    fire_power = state_decider.read();
                    oled.flush();
                    break;
                case 4:
                    window << "\f Press * to confirm information, \n"
                           << " or press any other key to change information. \n";
                    oled.flush();
                    break;
                case 5:
                    window << "\f Press C to start configurations \n";
                    oled.flush();
                    break;
                case 6:
                    window << "\f Press 2-9 to set game time. \n";
                    game_time = state_decider.read();
                    oled.flush();
                    break;
                case 7:
                    window << "\f Press # to confirm game time. \n";
                    oled.flush();
                    break;
                case 8:
                    window << "\f Press * to start a game. \n";
                    oled.flush();
                    break;
                case 9:
                    window << "\f Player number: " << player_number << "\n"
                           << " Set game time: " << game_time << "\n"
                           << " Set firepower: " << fire_power << "\n";
                    oled.flush();
                    break;
                case 10:
                    window << "\f Time left: " << game_time;
                    oled.flush();
                    break;
                case 11:
                    window << "\f Waiting for startsignal. \n";
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


//this needs to go eventually;

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
   display.write_int_to_channel(2);
   display.write_int_to_channel(2);
   display.write_int_to_channel(3);
   display.write_int_to_channel(4);
   rtos::run();
};
}

#endif

