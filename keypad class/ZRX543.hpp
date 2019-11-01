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
	keypad_listener & keypad_listener_obj;
	hwlib::keypad<16> & keypadObj;
	
public:
	ZRX543(const char * name, auto & keypadObj, keypad_listener & l): 
		task(name),keypad_listener_obj(l), keypadObj(keypadObj){}
	
	void main(){
		for(;;){
			if(keypadObj.pressed()){keypad_listener_obj.key_detected(keypadObj.getc());}
		}
	}
};
}

#endif