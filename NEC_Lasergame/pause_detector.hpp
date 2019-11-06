#ifndef PAUSE_DETECTOR_HPP
#define PAUSE_DETECTOR_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "pause_listener.hpp"


namespace target = hwlib::target;

namespace NEC{
    class pause_detector : public rtos::task<> {
    private:
        target::pin_in receiver;
        pause_listener & listener;
        rtos::timer pause_timer;
        enum class states { NO_MESSAGE, SIGNAL, PAUSE };
        states state;

        void main() override;

    public:
        pause_detector( target::pin_in & p, pause_listener & l, const char * name ):
                task( name ),
                receiver( p ),
                listener( l ),
                pause_timer( this, "pause_timer" )
        {}

    };
}
#endif //PAUSE_DETECTOR_HPP