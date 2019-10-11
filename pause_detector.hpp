#ifndef PAUSE_DETECTOR_HPP
#def PAUSE_DETECTOR_HPP

namespace NEC{
class pause_detector : public rtos::task<> {
private:
    pause_listener & listener;
    hwlib::pin_in receiver;
public:
    pause_detector( pause_listener & l, hwlib::pin_in & p ):
        listener( l ),
        receiver( p )
    {}
};
}
#endif //PAUSE_DETECTOR_HPP