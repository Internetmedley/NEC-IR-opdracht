#ifndef REG_GAME_PARA_HPP
#define REG_GAME_PARA_HPP

#include "run_game.hpp"
#include "init_game.hpp"
#include "msg_listener.hpp"

namespace NEC{
class reg_game_para : public rtos::task<>, public keypad_listener, public msg_listener {
private:
	init_game init_game_obj;
	run_game & run_game_obj;
	rtos::channel<char, 13> reg_game_para_buffer;
	rtos::channel<char, 13> reg_game_para_buffer_CMD;
    rtos::channel< uint16_t, 16 > logger_buffer;

	enum class states{ WAIT_KEY_A_OR_B, WAIT_KEY_0_TO_9, WAIT_KEY_CONFIRM, WAIT_GAME_TIME };
	states state = states::WAIT_KEY_A_OR_B;

	char firstkey;
	char secondkey;
	int game_time       =  1;
	int player_number 	= -1;
	int fire_power		= -1;

    void bitPrint(uint16_t a) {
        for (int16_t i = 15; i >= 0; --i) {
            hwlib::cout << ((((a >> i) & 1) == 1) ? 1 : 0);
            if(i == 8) { hwlib::cout << ' '; }
        }
        hwlib::cout << hwlib::endl;
    }

	void main() override {
		for(;;){
			switch( state ){
				case states::WAIT_KEY_A_OR_B:
					{hwlib::cout << "Press A or B" << hwlib::endl;
					firstkey = reg_game_para_buffer.read();
					if(firstkey == 'A' || firstkey == 'B'){
						state = states::WAIT_KEY_0_TO_9;
					}}
					break;
				case states::WAIT_KEY_0_TO_9:
					{if(firstkey == 'A'){
						hwlib::cout << "Press 0 to 9 for player number" << hwlib::endl;
					} else {
						hwlib::cout << "Press 1 to 9 for fire power" << hwlib::endl;
					}
					secondkey = reg_game_para_buffer.read();
					if(firstkey == 'A'){
						if(secondkey >= '0' && secondkey <= '9'){
							player_number = (secondkey - 48);
							if(player_number != -1 && fire_power != -1){
								state = states::WAIT_KEY_CONFIRM;		
							} else {
								state = states::WAIT_KEY_A_OR_B;
							}
						}
					} else {
						if(secondkey >= '1' && secondkey <= '9'){
							fire_power = (secondkey - 48);
							if(player_number != -1 && fire_power != -1){
								state = states::WAIT_KEY_CONFIRM;			
							} else {
								state = states::WAIT_KEY_A_OR_B;
							}
						}
					}}
					break;
				case states::WAIT_KEY_CONFIRM:
					{hwlib::cout << "Press * to confirm" << hwlib::endl;
					hwlib::cout << "Or press A or B to edit" << hwlib::endl;
					firstkey = reg_game_para_buffer.read();
					if(firstkey == '*'){
                        if(player_number == 0){
                            init_game_obj.set_flag();
                        }
						state = states::WAIT_GAME_TIME;
					} else {
						hwlib::cout << "Changing the values" << hwlib::endl;
						state = states::WAIT_KEY_A_OR_B;
					}}
					break;
				case states::WAIT_GAME_TIME:
					{firstkey = reg_game_para_buffer.read();
					if(firstkey == 'D' ){
						run_game_obj.set_flag();
						run_game_obj.write_int_to_channel(game_time);
						run_game_obj.write_int_to_channel(player_number);
						run_game_obj.write_int_to_channel(fire_power);
					}}
					break;
			}
		}
	}
public: 
	reg_game_para(const char * name, const char * buffer, const char * buffer2, const char * buffer3, init_game & init_game_obj, run_game & run_game_obj):
		task(name),
		init_game_obj(init_game_obj),
		run_game_obj(run_game_obj),
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