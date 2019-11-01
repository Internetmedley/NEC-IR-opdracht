#include "sender_class.hpp"

void sender_class::bitPrint(uint16_t a){
    for (int16_t i = 15; i >= 0; --i){
			hwlib::cout << ((((a >> i) & 1) == 1) ? 1 : 0);
			if(i == 8){hwlib::cout << ' ';}
 		}
 		hwlib::cout << hwlib::endl;}
}

void sender_class::main(){
    auto ir_led = hwlib::target::d2_36kHz();
     		for(;;){
			auto c = ir_buffer.read();
 			driver_led(c,ir_led);
 			bitPrint(c);
 		}
}

void sender_class::write_to_channel(int player_num, int weapon_power){
    uint16_t dataByte = 0;
 			dataByte = (dataByte | 1);
			dataByte |= (playerNum <<1);
 			dataByte |= (weaponPower <<6);
 			dataByte |= ((weaponPower ^ playerNum) << 11);
 			ir_buffer.write(dataByte);
			
}