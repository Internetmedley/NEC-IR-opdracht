#ifndef RUN_GAME_HPP
#define RUN_GAME_HPP

#include "msg_listener.hpp"
#include "beeper.hpp"

/// @file

namespace NEC{
    class run_game : public rtos::task<>, public keypad_listener, public msg_listener {
    private:
        rtos::channel<char, 13> run_game_buffer_char;
        rtos::channel<int, 13> run_game_buffer_int;
        rtos::flag start_game_flag;
        rtos::clock game_clock;
        rtos::channel< uint16_t, 16 > hits_buffer;
        ir_sender & sender;
        Display & display_obj;
        Beeper & bee;

        enum class states { WAIT_START_FLAG, RUN_GAME};
        states state = states::WAIT_START_FLAG;
        int game_time       = -1;
        int player_number   = -1;
        int fire_power      = -1;
        int health          = 18;
        int cooldown        =  0;
        int hit_recieved;
        int last_player_number;
        int last_fire_power;
        char key;
	
        /// \brief
        ///// function that prints a byte
        ///// \detail
        ///// If this fucntion is given a uint16 value it will loop trought it
        ///// shifting the byte and doing a and operation to get the value
        ///// of individual bits these are than printed to the terminal
        ///// the bits are printed from LSB to MSB
        void bitPrint(uint16_t a) {
            for (int16_t i = 15; i >= 0; --i) {
                hwlib::cout << ((((a >> i) & 1) == 1) ? 1 : 0);
                if(i == 8) { hwlib::cout << ' '; }
            }
            hwlib::cout << hwlib::endl;
        }
	
	/// \brief
        /// main function of the run_game task
        /// \detail
        /// wait_start_flag:
        /// this state reads checks if the start flag is after this the channel run_game_buffer is red
        /// after its red the state switches to the run_game state
        /// run_game:
        /// first this state checks if either game_time, fire_power or player_number are -1
        /// than game time is converted to minutes after a read run_game_buffer.
        /// than the player number and weapon power are also red from the run_game_buffer
        /// after this the display class is called to print the game time, player number and fire power.
        /// Than a event is checked if the event turns out to be game clock the game time is decremented
        /// in the same statement the cooldown is checked if it passes the check it gets decremented
        /// after this the game time is checked if this has reached zero health is reset and the state is switched
        /// to the wait_start_flag state. Otherwise if the channel contains a value while the game time does not equal
        /// zero than there is a check to see if the * is pushed. when this happens firepower and playernumber are written
        /// to the channel cooldown in seconds to the number given as firepower. if niehter of the above have occurd
        /// the player number and hits are recieved these will be processed and are checked if its not the players signal
        /// if its a enemy player that shot and the  health is not 0 than game time and health are decremented
        void main() override{
            for(;;){
                switch( state ){
                    case states::WAIT_START_FLAG:
                    {auto event = wait(run_game_buffer_char + start_game_flag);
                        if(event == run_game_buffer_char){
                            run_game_buffer_char.read();
                        } else {
                            state = states::RUN_GAME;
                        }}
                        break;
                    case states::RUN_GAME:
                        {if(game_time == -1 || player_number == -1 || fire_power == -1){
                            game_time = (run_game_buffer_int.read() * 60);
                            player_number = run_game_buffer_int.read();
                            fire_power = run_game_buffer_int.read();
                            break;
                        }
                        auto event = wait( run_game_buffer_char + game_clock + hits_buffer);
                        if(event == game_clock){
                            game_time--;
                            if(cooldown > 0){
                                cooldown--;
                            }
                            display_obj.write_int_to_channel(8);
                            display_obj.write_int_to_channel(game_time);
                            display_obj.write_int_to_channel(health);
                            if(game_time == 0){
                                health = 18;
                                state = states::WAIT_START_FLAG;
                                break;
                            }
                        } else if (event == run_game_buffer_char){
                            key = run_game_buffer_char.read();
                            if(key == '*' && cooldown == 0){
                                bee.write_int_to_channel(1);
                                sender.write_to_channel(player_number, fire_power);
                                cooldown = fire_power;
                            }
                        } else if (event == hits_buffer){
                            hit_recieved = hits_buffer.read();
                            last_player_number = (hit_recieved & 0x1F);
                            last_fire_power = ((hit_recieved & 0x3E0) >> 5);
                            if(!(last_player_number == player_number)){
                                bee.write_int_to_channel(2);
                                health -= last_fire_power;
                                if(health < 1){
                                    health = 18;
                                    game_time = -1;
                                    display_obj.write_int_to_channel(11);
                                    display_obj.write_int_to_channel(game_time);
                                    display_obj.write_int_to_channel(health);
                                    display_obj.write_int_to_channel(last_player_number);
                                    state = states::WAIT_START_FLAG;
                                    break;
                                }
                            }
                        }}
                        break;
                }
            }
        }
    public:
        run_game(const char * name, const char * buffer1, const char * buffer2,
                 const char * flag, long long int delay, const char * clock, ir_sender & sender, Display & display_obj, Beeper & bee):
                task(name),
                run_game_buffer_char(this,buffer1),
                run_game_buffer_int(this,buffer2),
                start_game_flag(this, flag),
                game_clock( this, delay, clock ),
                hits_buffer(this, "CMD_buffer"),
                sender(sender),
                display_obj(display_obj),
                bee(bee)
        {}

        void set_flag(){start_game_flag.set();}

        void key_detected( const char & key) override {
            run_game_buffer_char.write( key );
        }

        void write_int_to_channel( const int & i ){
            run_game_buffer_int.write( i );
        }

        void msg_received( const uint16_t & msg ) override {
            hits_buffer.write( msg );
        }
    };
}

#endif