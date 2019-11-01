#ifndef INIT_GAME_HPP
#define INIT_GAME_HPP

#include <string>

namespace NEC{
class init_game : public rtos::task<>, public keypad_listener {
private:
	rtos::channel<char, 13> init_game_buffer;
	rtos::flag game_leader_flag;

	enum class states { WAIT_START_FLAG, WAIT_KEY_C, WAIT_KEY_1_TO_9, WAIT_KEY_SEND_GAME_TIME, WAIT_KEY_START_GAME};
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
						state = states::WAIT_KEY_1_TO_9;
					}}
					break;
				case states::WAIT_KEY_1_TO_9:
					{hwlib::cout << "Press 1 to 9" << hwlib::endl;
					wait( init_game_buffer );
					key = init_game_buffer.read();
					if(key >= '1' && key <= '9'){
						game_time = (key - 48);
						state = states::WAIT_KEY_SEND_GAME_TIME;
					}}
					break;
				case states::WAIT_KEY_SEND_GAME_TIME:
					{hwlib::cout << "Press #" << hwlib::endl;
					wait( init_game_buffer );
					key = init_game_buffer.read();
					if(key == '#'){
						//Todo ir_send_channel.add(message);
						state = states::WAIT_KEY_START_GAME;
						hwlib::cout << game_time << hwlib::endl;
					}}
					break;
				case states::WAIT_KEY_START_GAME:
					{hwlib::cout << "Press *" << hwlib::endl;
					wait( init_game_buffer );
					key = init_game_buffer.read();
					if(key == '*'){
						//Todo ir_send_channel.add(message);
						hwlib::cout << "start game " << (game_time * 5) << hwlib::endl;
						state = states::WAIT_START_FLAG;
					}}
					break;
			}
		}
	}
public: 
	init_game(const char * name , const char * flag): 
		task(name), init_game_buffer(this,"key buffer"), game_leader_flag(this, flag){}
		
	void set_flag(){game_leader_flag.set();}

	void key_detected( const char & key) override {
        init_game_buffer.write( key );
	}
};
}

#endif