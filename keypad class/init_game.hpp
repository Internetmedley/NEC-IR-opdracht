#ifndef INIT_GAME_HPP
#define INIT_GAME_HPP

namespace NEC{
class init_game : public rtos::task<>, public keypad_listener {
private:
	enum class init_game_states { WAIT_KEY_C, WAIT_KEY_1_TO_9, WAIT_KEY_SEND_GAME_TIME, WAIT_KEY_START_GAME};
	init_game_states state = init_game_states::WAIT_KEY_C;
	rtos::channel<char, 13> key_buffer;
	char key;
	int game_time  = 0;
	void main (){
		for(;;){
			switch( state ){
				case init_game_states::WAIT_KEY_C:
					hwlib::cout << "Press C" << hwlib::endl;
					wait( key_buffer );
					key = key_buffer.read();
					if(key == 'C'){
						state = init_game_states::WAIT_KEY_1_TO_9;
					}
					break;
				case init_game_states::WAIT_KEY_1_TO_9:
					hwlib::cout << "Press 1 to 9" << hwlib::endl;
					wait( key_buffer );
					key = key_buffer.read();
					if(key >= '1' && key <= '9'){
						game_time = (key - 48);
						state = init_game_states::WAIT_KEY_SEND_GAME_TIME;
					}
					break;
				case init_game_states::WAIT_KEY_SEND_GAME_TIME:
					hwlib::cout << "Press #" << hwlib::endl;
					wait( key_buffer );
					key = key_buffer.read();
					if(key == '#'){
						//Todo ir_send_channel.add(message);
						state = init_game_states::WAIT_KEY_START_GAME;
						hwlib::cout << game_time << hwlib::endl;
					}
					break;
				case init_game_states::WAIT_KEY_START_GAME:
					hwlib::cout << "Press *" << hwlib::endl;
					wait( key_buffer );
					key = key_buffer.read();
					if(key == '*'){
						//Todo ir_send_channel.add(message);
						hwlib::cout << "start game " << (game_time * 5) << hwlib::endl;
					}
					break;
			}
		}
	}
public: 
	init_game(const char * name ): 
		task(name), key_buffer(this,"key buffer"){}

	char read(){return key_buffer.read();}

	void key_detected( const char & key) override {
        key_buffer.write( key );
	}
};
}

#endif