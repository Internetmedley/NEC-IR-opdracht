#ifndef BEEPER_HPP
#define BEEPER_HPP

///@file

/// \brief
///   class for speaker
/// \details
/// this is a class with a switch case, where the state determines the function that 'playes' the sounds.
/// the first number written in the channel is the state.
/// there are 7 cases that each have a function in them that determines what sound the speaker makes.
/// await keeps track of time
/// the functions that make sound use frequency and time to make a sound
/// the default constructor initializes the task, channel, pin and timer. 
/// the write_int_to_channel function writes to the channel.

namespace NEC{
class Beeper : public rtos::task<> {
private:
    hwlib::pin_out & pin;
    unsigned int timer;
    rtos::channel< int, 8 > stater;
    
    void main(){
        for(;;){
            int state = stater.read();
            switch( state ){
                case 1:
                    peew();
                    break;
                case 2:
                    beep3();
                    break;
            }
        }
    };
    
public:
    Beeper(hwlib::pin_out & pin, int timer):
    task( "Beeper" ),
    pin( pin ),
    timer( timer),
    stater( this, "stater")
    {}
    
    void await( long long unsigned int t ){
        while( t > hwlib::now_us() ){}
    };

    void beep(int f, int d, int fd = 1000 ){
        auto t = hwlib::now_us();    
        auto end = t + d;
        while( end > hwlib::now_us() ){
            auto p = 500'000 / f;
            f = f * fd / 1000;
            pin.write( 1 );
            pin.flush();
            await( t += p );
            pin.write( 0 );
            pin.flush();
            await( t += p );
        }   
    };

    void peew( ){
        beep( 20'000, 200'000, 990 );
    };

    void beep3(){
        for( int i = 0; i < 3; i++ ){
            beep( 1'000, 50'000 );
            hwlib::wait_us( 20'000 );
        }
    };

    void ohhh() {
        for (int i=1000; i<2000; i=i*1.02) { 
            beep(i,10'000); 
        } 
        for (int i=2000; i>1000; i=i*.98) {
            beep(i,10'000);
        }
    };

    void uhoh() {
        for (int i=1000; i<1244; i=i*1.01) { 
            beep(i,10'000); 
        } 
        hwlib::wait_us( 30'000 );
        for (int i=1244; i>1108; i=i*.99) {
            beep(i,10'000);
        }
    };

    void noise() {
        for (int i = 0; i < 200; i++ ){
            pin.write( ( hwlib::rand() & 0x01 ) == 0);
            pin.flush();
            hwlib::wait_us( 1'000 ); 
        }     
    };    

    void noise2(){
        for (int i = 0; i < 200; i++ ){
            pin.write( 1 );
            pin.flush();
            hwlib::wait_us( hwlib::random_in( 500, 2000 )); 
            pin.write( 0 );
            pin.flush();
            hwlib::wait_us( hwlib::random_in( 500, 2000 )); 
        }     
    };    

    void click(){
        for (int i = 0; i < 200; i++ ){
            pin.write( 1 );
            pin.flush();
            hwlib::wait_us( hwlib::random_in( 500, 600 )); 
            pin.write( 0 );
            pin.flush();
            hwlib::wait_us( hwlib::random_in( 500, 600 )); 
        }     
    };
    
    void beep4(){
        for(size_t i = 0; i < timer; i++){
            pin.write(1);
            pin.flush();
            hwlib::wait_us(500);
            pin.write(0);
            pin.flush();
            hwlib::wait_us(500);
        }
    };
    
    
    void write_int_to_channel(int i){
        stater.write(i);
    };
    
};
}

#endif
