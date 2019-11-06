#ifndef ZXR543_HPP
#define ZXR543_HPP

/*
Contains a class used to read keypad keys
It has a keypad object and if it detects a key being pressed
it wil write that key into the keypad_listener channel
*/
namespace NEC{
    class ZRX543: public rtos::task<>{
    private:
        hwlib::keypad<16> & keypadObj;
        keypad_listener & keypad_listener_obj;
        keypad_listener & keypad_listener_obj2;
        keypad_listener & keypad_listener_obj3;

    public:
        ZRX543(const char * name, hwlib::keypad<16> & keypadObj, keypad_listener & l, keypad_listener & l2, keypad_listener & l3):
                task(name),
                keypadObj(keypadObj),
                keypad_listener_obj(l),
                keypad_listener_obj2(l2),
                keypad_listener_obj3(l3)
        {}

        void main(){
            for(;;){
                hwlib::wait_us(50000);
                if(keypadObj.pressed()){
                    char key = keypadObj.getc();
                    keypad_listener_obj.key_detected(key);
                    keypad_listener_obj2.key_detected(key);
                    keypad_listener_obj3.key_detected(key);
                }
            }
        }
    };
}

#endif