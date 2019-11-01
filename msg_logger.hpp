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

    void main() override {
        for( ;; ){
            auto c = logger_buffer.read();
            hwlib::cout << c << '\n';
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