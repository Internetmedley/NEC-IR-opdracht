#ifndef REG_GAME_PARA_HPP
#define REG_GAME_PARA_HPP

namespace NEC{
class reg_game_para : public rtos::task<>, public keypad_listener {
private:
	init_game init_game_obj;
	rtos::channel<char, 13> reg_game_para_buffer;
	
	enum class states{ WAIT_KEY_A_OR_B, WAIT_KEY_0_TO_9, WAIT_GAME_TIME };
	states state = states::WAIT_KEY_A_OR_B;
	char firstkey;
	char secondkey;
	int player_number 	= -1;
	int fire_power		= -1;
	
	void main (){
		for(;;){
			switch( state ){
				case states::WAIT_KEY_A_OR_B:
					{hwlib::cout << "Press A or B" << hwlib::endl;
					wait(reg_game_para_buffer);
					firstkey = reg_game_para_buffer.read();
					hwlib::cout << "firstkey: " << firstkey << hwlib::endl;
					if(firstkey == 'A' || firstkey == 'B'){
						state = states::WAIT_KEY_0_TO_9;
					}}
					break;
				case states::WAIT_KEY_0_TO_9:
					{hwlib::cout << "Press key 0 to 9" << hwlib::endl;
					wait(reg_game_para_buffer);
					secondkey = reg_game_para_buffer.read();
					hwlib::cout << "secondkey: " << secondkey << hwlib::endl;
					if(secondkey >= '0' && secondkey <= '9'){
						if(firstkey == 'A'){
							player_number = (secondkey - 48);
							hwlib::cout << "playernumber: " << player_number << hwlib::endl;
							if(player_number != -1 && fire_power != -1){
								state = states::WAIT_GAME_TIME;
							} else {
								state = states::WAIT_KEY_A_OR_B;
							}
						} else {
							fire_power = (secondkey - 48);
							hwlib::cout << "firepower: " << fire_power << hwlib::endl;
							if(player_number != -1 && fire_power != -1){
								state = states::WAIT_GAME_TIME;
							} else {
								state = states::WAIT_KEY_A_OR_B;
							}
						}
					}}
					break;
				case states::WAIT_GAME_TIME:
					{hwlib::cout << "succes" << hwlib::endl;
					if(player_number == 0){
						init_game_obj.set_flag();
					}
					wait(reg_game_para_buffer);}
					break;
			}
		}
	}
public: 
	reg_game_para(const char * name, init_game & init_game_obj): 
		task(name), init_game_obj(init_game_obj), reg_game_para_buffer(this,"key buffer"){}

	void key_detected( const char & key) override {
        reg_game_para_buffer.write( key );
	}
};
}

#endif