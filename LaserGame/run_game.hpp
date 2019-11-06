#ifndef RUN_GAME_HPP
#define RUN_GAME_HPP

#include "msg_listener.hpp"

namespace NEC{
class run_game : public rtos::task<>, public keypad_listener, public msg_listener {
private:
	rtos::channel<char, 13> run_game_buffer_char;
	rtos::channel<int, 13> run_game_buffer_int;
	rtos::channel<uint16_t, 16> hit_buffer;
	rtos::flag start_game_flag;
	rtos::clock game_clock;

	enum class states { WAIT_START_FLAG, RUN_GAME};
	states state = states::WAIT_START_FLAG;
	int game_time  = -1;
	int player_number = -1;
	int fire_power = -1;
	char key;
	
	void main (){
		for(;;){
			switch( state ){
				case states::WAIT_START_FLAG:
					{auto event = wait(run_game_buffer_char + start_game_flag);
					if(event == run_game_buffer_char){
						key = run_game_buffer_char.read();
						if(key == 'D'){
							hwlib::cout << key << hwlib::endl;
						}
					} else {
						run_game_buffer_char.read();
						state = states::RUN_GAME;
					}}
					break;
				case states::RUN_GAME:
					{if(game_time == -1 || player_number == -1 || fire_power == -1){
						wait(run_game_buffer_int);
						game_time = (run_game_buffer_int.read() * 60);
						player_number = run_game_buffer_int.read();
						fire_power = run_game_buffer_int.read();
						hwlib::cout << "game time: " << game_time << "player number: " << player_number << "fire power: " << fire_power << hwlib::endl;
						break;
					}
					auto event = wait( run_game_buffer_char + game_clock );
					if(event == game_clock){
					    game_time--;
					    hwlib::cout << "TIme left: " << game_time << hwlib::endl;
					    if(game_time == 0){
					        state = states::WAIT_START_FLAG;
					        break;
					    }
					} else if (event == run_game_buffer_char){
                        key = run_game_buffer_char.read();
                        if(key == '*'){
                            hwlib::cout << key << hwlib::endl;
                        }
					}}
					break;
			}
		}
	}
public:
	run_game( const char * name, long long int delay ):
		task(name),
		run_game_buffer_char( this, "buffer_char" ),
		run_game_buffer_int(this, "buffer_int" ),
		cmd_buffer( this, "cmd_buffer" ),
		hit_buffer( this, "hit_buffer" ),
		start_game_flag(this, "start_game_flag"),
		game_clock( this, delay, "game_clock" )
	{}

	void set_flag(){start_game_flag.set();}

	void key_detected( const char & key) override {
        run_game_buffer_char.write( key );
	}

	void write_int_to_channel( const int & i ){
		run_game_buffer_int.write( i );
	}

	void cmd_received( const uint8_t & msg ) override {
		cmd_buffer.write( msg );
	}

	void hit_received( const uint8_t & msg ) override {
		hit_buffer.write( msg );
	}
};
}

#endif