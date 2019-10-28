#ifndef PAUSE_DETECTOR_HPP
#define PAUSE_DETECTOR_HPP

namespace NEC{
class pause_detector : public rtos::task<> {
private:
    pause_listener & listener;
    hwlib::pin_in receiver;
	enum class state_t { NO_MESSAGE, SIGNAL, PAUSE };
	state_t state = NO_MESSAGE;

	void main();

public:
    pause_detector( pause_listener & l, hwlib::pin_in & p ):
        listener( l ),
        receiver( p )
    {}
	
	
};
}
#endif //PAUSE_DETECTOR_HPP