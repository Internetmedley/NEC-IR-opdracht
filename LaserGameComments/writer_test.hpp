#ifndef WRITER_TEST_HPP
#define WRITER_TEST_HPP

#include "rtos.hpp"
#include "hwlib.hpp"
#include "ir_sender.hpp"

class writerTest : public rtos::task<>{
private:
 	ir_sender & sender;
 	int player_number;
 	int weapon_power;
public:
 	writerTest(ir_sender & sender, const int & player_number, const int & weapon_power);
    void main();

};
#endif