#indef MSG_LOGGER_HPP
#def MSG_LOGGER_HPP

//deze class haalt messages op uit de message channel en stuurt
//ze naar een OLED display als binair of hex.
//het display wordt ook gecleared om de 5 seconden.

namespace NEC{
class msg_logger : public msg_listener, public rtos::task<> {
    public:
};
}

#endif //MSG_LOGGER_HPP