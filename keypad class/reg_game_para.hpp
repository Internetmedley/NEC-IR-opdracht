#ifndef REG_GAME_PARA_HPP
#define REG_GAME_PARA_HPP

namespace NEC{
class reg_game_para : public rtos::task<>, public keypad_listener {
private:
	enum class reg_game_para_states { WAIT_KEY_C, WAIT_KEY_1_TO_9};
	reg_game_para_states state = reg_game_para_states::WAIT_KEY_A_OR_B;
	rtos::channel<char, 13> key_buffer;
	char firstkey;
	char secondkey;
	int player_number 	= 0;
	int fire_power		= 0;
	void main (){
		for(;;){
			switch( state ){
				case reg_game_para_states::WAIT_KEY_C:
					hwlib::cout << "Press A or B" << hwlib::endl;
					wait( key_buffer );
					firstkey = key_buffer.read();
					if(firstkey == 'A' || firstkey == 'B'){
						state = reg_game_para_states::WAIT_KEY_1_TO_9;
					}
					break;
				case reg_game_para_states::WAIT_KEY_1_TO_9:
					hwlib::cout << "Press 1 to 9" << hwlib::endl;
					wait( key_buffer );
					firstkey = key_buffer.read();
					if(key >= '1' && key <= '9'){
						state = reg_game_para_states::WAIT_KEY_1_TO_9;
					}
					break;
			}
		}
	}
public: 
	reg_game_para(const char * name ): 
		task(name), key_buffer(this,"key buffer"){}

	char read(){return key_buffer.read();}

	void key_detected( const char & key) override {
        key_buffer.write( key );
	}
};
}

#endif