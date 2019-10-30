#ifndef PAUSE_DETECTOR_HPP
#define PAUSE_DETECTOR_HPP

#include "hwlib.hpp"
#include "rtos.hpp"


namespace target = hwlib::target;

namespace NEC{
class pause_detector : public rtos::task<> {
private:
    target::pin_in receiver;
	enum class states { NO_MESSAGE, SIGNAL, PAUSE };
	states state;

	void main() override;

public:
    rtos::channel< int, 32 > pause_buffer;		        //schrijft pauze duraties
    
    pause_detector( target::pin_in & p, const char * name ):
        task( name ),
            receiver( p ),
            pause_buffer( this, "pause_buffer" )
    {}
	
};
}
#endif //PAUSE_DETECTOR_HPP