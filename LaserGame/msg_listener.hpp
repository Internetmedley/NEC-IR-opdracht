#ifndef MSG_LISTENER_HPP
#define MSG_LISTENER_HPP

namespace NEC{
class msg_listener{
public:
    virtual void msg_received( const uint16_t & msg ) = 0;

    msg_listener() {}
};
}
#endif //MSG_LISTENER_HPP