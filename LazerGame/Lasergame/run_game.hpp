#ifndef RUN_GAME_HPP
#define RUN_GAME_HPP


namespace NEC{
class run_game : public rtos::task<>, public keypad_listener {
private:
	rtos::channel<char, 13> run_game_buffer_char;
	rtos::channel<int, 13> run_game_buffer_int;
	rtos::flag start_game_flag;
	rtos::clock game_clock;

	enum class states { WAIT_START_FLAG, RUN_GAME};
	states state = states::WAIT_START_FLAG;
	int game_time  = 0;
	int player_number = 0;
	int fire_power = 0;
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
					{if(game_time == 0 || player_number == 0 || fire_power == 0){
						wait(run_game_buffer_int);
						game_time = run_game_buffer_int.read();
						player_number = run_game_buffer_int.read();
						fire_power = run_game_buffer_int.read();
						hwlib::cout << "game time: " << game_time << "player number: " << player_number << "fire power: " << fire_power << hwlib::endl;
						break;
					} else {
						hwlib::cout << "starting game" << hwlib::endl;
					}
					wait( run_game_buffer_char );
					key = run_game_buffer_char.read();
					if(key == '*'){
						hwlib::cout << key << hwlib::endl;
					}}
					break;
			}
		}
	}
public: 
	run_game(const char * name, const char * buffer1, const char * buffer2,
			 const char * flag, long long int delay, const char * clock): 
		task(name),
		run_game_buffer_char(this,buffer1),
		run_game_buffer_int(this,buffer2), 
		start_game_flag(this, flag),
		game_clock( this, delay, clock )
	{}
		
	void set_flag(){start_game_flag.set();}

	void key_detected( const char & key) override {
        run_game_buffer_char.write( key );
	}
	
	void write_int_to_channel( const int & i ){
		run_game_buffer_int.write( i );
	}
};
}

#endif