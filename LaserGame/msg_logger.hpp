#ifndef MSG_LOGGER_HPP
#define MSG_LOGGER_HPP

#include "msg_listener.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"

//deze class haalt messages op uit de message channel en stuurt
//ze naar een OLED display als binair of hex.
//het display wordt ook gecleared om de 5 seconden.

namespace NEC{
class msg_logger : public msg_listener, public rtos::task<> {
    private:
    rtos::channel< uint16_t, 16 > logger_buffer;

    void bitPrint(uint16_t a) {
    for (int16_t i = 15; i >= 0; --i) {
            hwlib::cout << ((((a >> i) & 1) == 1) ? 1 : 0);
            if(i == 8) { hwlib::cout << ' '; }
        }
        hwlib::cout << hwlib::endl;
    }

    void main() override {
        for( ;; ){
            //hwlib::cout << "jemoeder" << '\n';
            auto c = logger_buffer.read();
            bitPrint( c );
        }
    }

    public:
    msg_logger( const char * name ):
        task( name ),
            logger_buffer( this, "logger_buffer" )
        {}

    void msg_received( const uint16_t & msg ) override {
        logger_buffer.write( msg );
    }
};
}
#endif //MSG_LOGGER_HPP