#ifndef KEYPAD_LISTENER_HPP
#define KEYPAD_LISTENER_HPP

//contains a key_detected function that requires a char as variable
namespace NEC{
class keypad_listener {
public:
	virtual void key_detected( const char & key) = 0;
	keypad_listener() {}
};
}

#endif