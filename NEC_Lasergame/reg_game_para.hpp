#ifndef REG_GAME_PARA_HPP
#define REG_GAME_PARA_HPP

#include "run_game.hpp"
#include "init_game.hpp"
#include "msg_listener.hpp"
#include "display.hpp"

/// @file

namespace NEC{
    class reg_game_para : public rtos::task<>, public keypad_listener, public msg_listener {
    private:
        init_game init_game_obj;
        run_game & run_game_obj;
        Display & display_obj;
        rtos::channel<char, 13> reg_game_para_buffer;
        rtos::channel<char, 13> reg_game_para_buffer_CMD;
        rtos::channel< uint16_t, 16 > logger_buffer;

        enum class states{ WAIT_KEY_A_OR_B, WAIT_KEY_0_TO_9, WAIT_KEY_CONFIRM, WAIT_GAME_TIME, WAIT_GAME_START, IDLE};
        states state = states::WAIT_KEY_A_OR_B;

        char firstkey;
        char secondkey;
        int game_time       =  1;
        int player_number 	= -1;
        int fire_power		= -1;
        uint16_t CMD;

     	/// \brief
        /// function that prints a byte
        /// \detail
        /// If this fucntion is given a uint16 value it will loop trought it
        /// shifting the byte and doing a and operation to get the value
        /// of individual bits these are than printed to the terminal
        /// the bits are printed from LSB to MSB
        void bitPrint(uint16_t a) {
            for (int16_t i = 15; i >= 0; --i) {
                hwlib::cout << ((((a >> i) & 1) == 1) ? 1 : 0);
                if(i == 8) { hwlib::cout << ' '; }
            }
            hwlib::cout << hwlib::endl;
        }

     	/// \brief
        /// the main fucntion of the  reg_game_para task
        /// \details
        /// has a amount of states with the following names and fucntion:
        /// wait_key_a_or_b:
        /// this state prints to the display after it checks if either A or B is pressed
        /// if this is the case it switches to the wait_key_0_to_9 state
        /// wait_key_0_to_9:
        /// this state prints to the display after it checks what the last button press was
        /// if the button press was A than player number is set and the state swichtes to wait_key_confirm
        /// if the button press was B than weapon power is set and the state switches to wait_key_confirm
        /// wait_key_confirm:
        /// this state waits for the * button after this is pressed either the game leader flag is set
        /// or if this is not the case the state switches to wait_game_time
        /// wait_game_time:
        /// this state checks if the CMD it reads from the channel is bigger than one
        /// if this is the case the CMD becomes the game time and the state switches to wait_game_start state
        /// wait_game_start:
        /// this state writes game time, player number, fire power to the channel
        void main() override {
            for (;;) {
                switch (state) {
                    case states::WAIT_KEY_A_OR_B: {
                        display_obj.write_int_to_channel(1);
                        firstkey = reg_game_para_buffer.read();
                        if (firstkey == 'A' || firstkey == 'B') {
                            state = states::WAIT_KEY_0_TO_9;
                        }
                    }
                        break;
                    case states::WAIT_KEY_0_TO_9: {
                        if (firstkey == 'A') {
                            display_obj.write_int_to_channel(2);
                        } else {
                            display_obj.write_int_to_channel(3);
                        }
                        secondkey = reg_game_para_buffer.read();
                        if (firstkey == 'A') {
                            if (secondkey >= '0' && secondkey <= '9') {
                                player_number = (secondkey - 48);
                                if (player_number != -1 && fire_power != -1) {
                                    state = states::WAIT_KEY_CONFIRM;
                                } else {
                                    state = states::WAIT_KEY_A_OR_B;
                                    hwlib::wait_ms(200);
                                }
                            }
                        } else {
                            if (secondkey >= '1' && secondkey <= '9') {
                                fire_power = (secondkey - 48);
                                if (player_number != -1 && fire_power != -1) {
                                    state = states::WAIT_KEY_CONFIRM;
                                } else {
                                    state = states::WAIT_KEY_A_OR_B;
                                    hwlib::wait_ms(200);
                                }
                            }
                        }
                    }
                        break;
                    case states::WAIT_KEY_CONFIRM: {
                        display_obj.write_int_to_channel(4);
                        firstkey = reg_game_para_buffer.read();
                        if (firstkey == '*') {
                            if (player_number == 0) {
                                init_game_obj.set_flag();
                                state = states::IDLE;
                            } else {
                                state = states::WAIT_GAME_TIME;
                            }
                        } else {
                            state = states::WAIT_KEY_A_OR_B;
                        }
                    }
                        break;
                    case states::WAIT_GAME_TIME:
                        {display_obj.write_int_to_channel(10);
                            CMD = logger_buffer.read();
                            if (CMD > 1) {
                                game_time = CMD;
                                state = states::WAIT_GAME_START;
                            }
                        }
                        break;
                        case states::WAIT_GAME_START:
                            {display_obj.write_int_to_channel(9);
                            CMD = logger_buffer.read();
                            if (CMD == 1) {
                                run_game_obj.set_flag();
                                run_game_obj.write_int_to_channel(game_time);
                                run_game_obj.write_int_to_channel(player_number);
                                run_game_obj.write_int_to_channel(fire_power);
                                state = states::IDLE;
                            }}
                        break;
                    case states::IDLE:
                        {reg_game_para_buffer.read();}
                    }
                }
            }
    public:
        reg_game_para(const char * name, const char * buffer, const char * buffer2, const char * buffer3, init_game & init_game_obj, run_game & run_game_obj, Display & display_obj):
                task(name),
                init_game_obj(init_game_obj),
                run_game_obj(run_game_obj),
                display_obj(display_obj),
                reg_game_para_buffer(this, buffer),
                reg_game_para_buffer_CMD(this, buffer2),
                logger_buffer( this, buffer3 )
        {}

        void key_detected( const char & key) override {
            reg_game_para_buffer.write( key );
        }

        void msg_received( const uint16_t & msg ) override {
            logger_buffer.write( msg );
        }
    };
}

#endif