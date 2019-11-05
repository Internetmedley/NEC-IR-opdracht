#include "writer_test.hpp"
#include "ir_sender.hpp"

//constructor of the writerTest class 
writerTest::writerTest(ir_sender & sender, const int & player_number, const int & weapon_power):
    sender(sender), 
 	player_number(player_number),
 	weapon_power(weapon_power){}

/*
	main function of wrtiterTest class
	waits for a button input on arduino pin d8
	after it will write the player number and weapon power
	into the channel of the ir sender class 
*/
void writerTest::main(){
    
 		for(;;){ 
 			sender.write_to_channel(player_number, weapon_power); 
			hwlib::wait_ms(200);
			
 		}
}
