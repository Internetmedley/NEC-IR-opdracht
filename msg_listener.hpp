#ifndef MSG_LISTENER_HPP
#define MSG_LISTENER_HPP

namespace NEC{
class msg_listener{
public:
    void msg_received( const uint16_t & msg );      //moet nog virtueel gemaakt worden, maar is nu om te testen

    msg_listener() {}

};
}
#endif //MSG_LISTENER_HPP