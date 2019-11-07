#ifndef INIT_GAME_HPP
#define INIT_GAME_HPP

#include "ir_sender.hpp"
#include "display.hpp"

/// @file

namespace NEC{
    class init_game : public rtos::task<>, public keypad_listener {
    private:
        rtos::channel<char, 13> init_game_buffer;
        rtos::flag game_leader_flag;
        ir_sender & sender;
        Display & display_obj;

        enum class states { WAIT_START_FLAG, WAIT_KEY_C, WAIT_KEY_2_TO_9, WAIT_KEY_SEND_MESSAGE};
        states state = states::WAIT_START_FLAG;
        int game_time  = 0;
        char key;

    /// \brief
    /// main function of the task init game
    /// \detail
    /// has a amount of states with the following names and fucntion:
    /// wait_start_flag:
    /// this state waits for a flag after this flag is set it will either
    /// just read from its channel  "init_game_buffer" or it will read from
    /// its channel and after will switch to the wait_key_c state
    /// wait_key_c:
    /// this state asks the user to press the c key. this states than reads
    /// its channel until the key is pressed. after this key is recived
    /// the state switches to wait_key_2_to_9
    /// wait_key_2_to_9:
    /// this state asks the user to press a key between ranging  1 and 9. If this
    /// is pressed the state than converts the char to a int and switches to the
    /// wait_key_send_game_time state
    /// wait_key_send_game_time:
    /// this state asks the user to press the # key. If this key is pressed the state
    /// than writes the game time to the channel and switches the state to the
    /// wait_key_start_game state
    /// wait_key_start_game:
    /// this state ask the user to press the * key. If this key is pressed the state
    /// than sends the start command to the channel after this the function ends
        void main () override {
            for(;;){
                switch( state ){
                    case states::WAIT_START_FLAG:
                    {auto event = wait(init_game_buffer + game_leader_flag);
                        if(event == init_game_buffer){
                            init_game_buffer.read();
                        } else {
                            init_game_buffer.read();
                            state = states::WAIT_KEY_C;
                        }}
                        break;
                    case states::WAIT_KEY_C:
                    {display_obj.write_int_to_channel(5);
                        wait( init_game_buffer );
                        key = init_game_buffer.read();
                        if(key == 'C'){
                            state = states::WAIT_KEY_2_TO_9;
                        }}
                        break;
                    case states::WAIT_KEY_2_TO_9:
                    {display_obj.write_int_to_channel(6);
                        wait( init_game_buffer );
                        key = init_game_buffer.read();
                        if(key >= '2' && key <= '9'){
                            game_time = (key - 48);
                            state = states::WAIT_KEY_SEND_MESSAGE;
                        }}
                        break;
                    case states::WAIT_KEY_SEND_MESSAGE:
                    {display_obj.write_int_to_channel(7);
                        wait( init_game_buffer );
                        key = init_game_buffer.read();
                        if(key == '#'){
                            sender.write_to_channel(0, game_time);
                        } else if (key == '*') {
                            sender.write_to_channel(0, 1);
                        }}
                        break;
                }
            }
        }
    public:
    ///\brief
    /// construtor that sets the name for task channel and flag
        init_game(const char * name, const char * buffer, const char * flag, ir_sender & sender, Display & display_obj):
                task(name),
                init_game_buffer(this, buffer),
                game_leader_flag(this, flag),
                sender(sender),
                display_obj(display_obj)
        {}

	///\brief
	/// Functionthat sets rtos flag
	/// \detail
	/// sets the game_leader flag that is declared in init_game
        void set_flag(){game_leader_flag.set();}

	/// \brief
	/// Function that writes to rtos channel
	/// \detail
	/// this fucntion allows the writing of keys to the channel init_game_buffer
	/// that is declared in init_game
        void key_detected( const char & key) override {
            init_game_buffer.write( key );
        }
    };
}

#endif