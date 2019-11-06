#ifndef ZXR543_HPP
#define ZXR543_HPP

namespace NEC{
class ZRX543: public rtos::task<>{
private:
	keypad_listener & keypad_listener_obj;
	keypad_listener & keypad_listener_obj2;
	keypad_listener & keypad_listener_obj3;
	hwlib::keypad<16> & keypadObj;
	
public:
    /// \brief
    /// constructor for the keyboard class
    /// instances the name, keypad, and the listeners
    /// this makes three objects of the keypad listener
	ZRX543(const char * name, hwlib::keypad<16> & keypadObj,
			keypad_listener & l, keypad_listener & l2, keypad_listener & l3): 
		task(name),
		keypad_listener_obj(l),
		keypad_listener_obj2(l2),
		keypad_listener_obj3(l3),
		keypadObj(keypadObj)
	{}

	/// \brief
	/// main function of the keyboard task
	/// \details
	/// sends the recieved key to all instances of the keypad linstener
	void main(){
		for(;;){
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