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
    int game_time;
    int health;
    
    rtos::channel< int, 8 > state_decider;
    
    void main(){
        for(;;){
            int state = state_decider.read();
            oled.clear();
            switch( state ){
                case 1:
                    {window << "\fPress A for \nPlayerID \n \n"
                           << "Press B for \nfirepower \n";}
                    break;
                case 2:
                    {window << "\fPress 0-9 for \nPlayerID. \n";}
                    break;
                case 3:
                    {window << "\fPress 1-9 for \nfirepower. \n";}
                    break;
                case 4:
                    {window << "\fPress * to \nconfirm \ninformation \n \n"
                           << "or press any \nother key to \nchange \ninformation. \n";}
                    break;
                case 5:
                    {window << "\fPress C to start\nconfigurations \n";}
                    break;
                case 6:
                    {window << "\fPress 2-9 to set\ngame time. \n";}
                    break;
                case 7:
                    {window << "\fPress # to send\ngame time. \n \n"
                            << "Press * to send\nstart command.";}
                    break;
                case 8:
                    {game_time = state_decider.read();
                    health = state_decider.read();
                    window << "\fTime left: " << game_time << "\n \n"
                            << "Health left: " << health;}
                    break;
                case 9:
                    {window << "\fWaiting for\nstartsignal... \n";}
                    break;
                case 10:
                    {window << "\fWaiting for\ngame time... \n";}
                    break;
                case 11:
                    {game_time = state_decider.read();
                    health = state_decider.read();
                    player_number = state_decider.read();
                    window << "\fTime left: " << game_time << "\n \n"
                           << "Health left: " << health << "\n \n"
                           << "Hit by: " << player_number;}
                    break;
            }
            oled.flush();
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



}

#endif

