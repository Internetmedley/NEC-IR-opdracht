#ifndef IR_SENDER_HPP
#define IR_SENDER_HPP

#include "rtos.hpp"
#include "hwlib.hpp"
class ir_sender : public rtos::task<>{
private:
    rtos::channel<uint16_t ,1> ir_buffer;
 	rtos::timer sendTimer;
 	void bitPrint(uint16_t a);
    void main();
public:
    ir_sender();

    void write_to_channel(int playernum, int weapon_power);
    int * encode_to_array(int arr[], uint16_t data);
};
#endif