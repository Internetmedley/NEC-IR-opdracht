#ifndef INIT_GAME_HPP
#define INIT_GAME_HPP

#include "ir_sender.hpp"

namespace NEC{
    class init_game : public rtos::task<>, public keypad_listener {
    private:
        rtos::channel<char, 13> init_game_buffer;
        rtos::flag game_leader_flag;
        ir_sender & sender;

        enum class states { WAIT_START_FLAG, WAIT_KEY_C, WAIT_KEY_2_TO_9, WAIT_KEY_SEND_MESSAGE};
        states state = states::WAIT_START_FLAG;
        int game_time  = 0;
        char key;

        void main (){
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
                    {hwlib::cout << "Press C" << hwlib::endl;
                        wait( init_game_buffer );
                        key = init_game_buffer.read();
                        if(key == 'C'){
                            state = states::WAIT_KEY_2_TO_9;
                        }}
                        break;
                    case states::WAIT_KEY_2_TO_9:
                    {hwlib::cout << "Press 2 to 9" << hwlib::endl;
                        wait( init_game_buffer );
                        key = init_game_buffer.read();
                        if(key >= '1' && key <= '9'){
                            game_time = (key - 48);
                            state = states::WAIT_KEY_SEND_MESSAGE;
                        }}
                        break;
                    case states::WAIT_KEY_SEND_MESSAGE:
                    {hwlib::cout << "Press # or *" << hwlib::endl;
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
        init_game(const char * name, const char * buffer, const char * flag, ir_sender & sender):
                task(name),
                init_game_buffer(this, buffer),
                game_leader_flag(this, flag),
                sender(sender)
        {}

        void set_flag(){game_leader_flag.set();}

        void key_detected( const char & key) override {
            init_game_buffer.write( key );
        }
    };
}

#endif