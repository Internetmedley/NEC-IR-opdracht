#ifndef SENDER_CLASS_HPP
#define SENDER_CLASS_HPP

#include "rtos.hpp"
#include "hwlib.hpp"

class sender_class{
private:
    rtos::channel<uint16_t ,1> ir_buffer;
 	rtos::timer sendTimer;
 	void bitPrint(uint16_t a){}
    void main override;
public:
    Ir_sender():
 	    task("sender"), ir_buffer(this,"ir buffer"), sendTimer(this,"sendTimer"){}

    void write_to_channel(int playernum, int weapon_power){}
};
#endef