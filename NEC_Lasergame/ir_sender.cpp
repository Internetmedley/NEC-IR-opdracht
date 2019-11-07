#include "ir_sender.hpp"

/// @file

/// \brief
/// Constructor for ir_sender: inits a rtos task with name sender
/// and makes both a ir_buffer (rtos channel) and sendTimer(rtos::timer)
ir_sender::ir_sender():task("sender"), ir_buffer(this,"ir buffer"), sendTimer(this,"sendTimer"){}


/// \brief
///// function that prints a byte
///// \detail
///// If this fucntion is given a uint16 value it will loop trought it
///// shifting the byte and doing a and operation to get the value
///// of individual bits these are than printed to the terminal
///// the bits are printed from LSB to MSB
void ir_sender::bitPrint(uint16_t a){
    for (int16_t i = 15; i >= 0; --i){
        hwlib::cout << ((((a >> i) & 1) == 1) ? 1 : 0);
        if(i == 8){hwlib::cout << ' ';}
    }
    hwlib::cout << hwlib::endl;
}

/// \brief
/// the main function of the task sender
/// \detail
/// has a amount of states with the following names and fucntion:
/// Wait_for_channel: waits on the channel (for now reads own channel and than calls encode_to_array() )
/// Send_start_bit: writes the led high for 9000 ms before writing it low for 4500 ms
/// turn_led_on: wirtes the led high
/// turn_led_off: writes the led low for a given time depeingin on the value in "arr" this can either be 1960 ms for a one or 560 ms for a zero
void ir_sender::main(){
    auto ir_led = hwlib::target::d2_36kHz();
    int bit_counter = 0;
    //int msg_counter = 0;
    int  arr[16] = {};
    enum class states {WAIT_FOR_CHANNEL, SEND_START_BIT, TURN_LED_ON, TURN_LED_OFF};
    states state = states::WAIT_FOR_CHANNEL;


    for(;;){
        switch(state){
            case states::WAIT_FOR_CHANNEL:{
                auto c = ir_buffer.read();
                encode_to_array(arr,c);
                state = states::SEND_START_BIT;
                break;
            }
            case states::SEND_START_BIT:{
                ir_led.write(1);
                ir_led.flush();
                hwlib::wait_us( 9000 );
                ir_led.write(0);
                ir_led.flush();
                hwlib::wait_us(4500);
                state = states::TURN_LED_ON;
                break;
            }
            case states::TURN_LED_ON:{
                ir_led.write(1);
                ir_led.flush();
                hwlib::wait_us(560);
                ir_led.write( 0 );
                ir_led.flush();

                if(bit_counter == 16){ bit_counter = 0; ir_led.write(0 ); ir_led.flush(); hwlib::wait_us( 9000 );  state = states::WAIT_FOR_CHANNEL;}
                else{state = states::TURN_LED_OFF;}
                break;
            }
            case states::TURN_LED_OFF:{
                ir_led.write(0);
                ir_led.flush();
                if(arr[bit_counter] == 1) {
                    hwlib::wait_us(1690);
                }
                else {
                    hwlib::wait_us(560);
                }
                ir_led.write( 1 );
                ir_led.flush();
                bit_counter++;
                state = states::TURN_LED_ON;
                break;
            }
        }
    }
}


/// \brief
/// Function that allows writing into the channel of the class "ir_sender"
/// \detail
/// when the player number and weapon power are given. After this bitwise conversions
/// take place to set all bytes of the message from LSB to MSB in order of:
/// bit 0 start bit
/// bits 1-5 player nummber (max 31)
/// bits 6-10 weapon power/command (max 31)
/// bits 11-15 checksum xor of bit 1&6,2&7,3&8,4&9,5&10
/// after this it is written to the channel of ir sender
void ir_sender::write_to_channel(int player_num, int weapon_power){
    uint16_t dataByte = 0;
    dataByte = (dataByte | 1);
    dataByte |= (player_num <<1);
    dataByte |= (weapon_power <<6);
    dataByte |= ((weapon_power ^ player_num) << 11);
    ir_buffer.write(dataByte);

}


/// \brief
/// This function encodes uint16 to array
/// \detail
/// This function encodes all the bits in a uint16 data type
/// to be translated to an array. This is done by shifting
/// the uint16_t with a i that decrements form fifhteen to zero
/// while ANDing with 1 thus getting its values wich is passed
/// to the array "arr"
int * ir_sender::encode_to_array(int arr[16],uint16_t data){
    for (int16_t i = 15; i >= 0; --i){
        arr[i] = ((data >> i) & 1);
    }
    return arr;
}