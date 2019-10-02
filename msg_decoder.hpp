#ifndef MSG_DECODER_HPP
#def MSG_DECODER_HPP

//this is a class that reads pauses from the pause channel

namespace NEC{
class msg_decoder : public pause_listener{
private:
    msg_listener listener;
public:
    msg_decoder( msg_listener & l ):
        listener( l )
    {}
    
};
}
#endif //MSG_DECODER_HPP