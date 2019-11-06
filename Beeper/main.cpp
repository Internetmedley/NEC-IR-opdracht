#include "hwlib.hpp"
#include "rtos.hpp"

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
                    hwlib::wait_ms(1000);
                    ear_rape();
                    break;
                case 2:
                    hwlib::wait_ms(1000);
                    beep4();
                    break;
                case 3:
                    hwlib::wait_ms(1000);
                    ohhh();
                    break;
                case 4:
                    hwlib::wait_ms(1000);
                    noise();
                    break;
                case 5:
                    hwlib::wait_ms(1000);
                    click();
                    break;
                case 6:
                    hwlib::wait_ms(1000);
                    uhoh();
                    break;
                case 7:
                    hwlib::wait_ms(1000);
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
 
    void ear_rape(){
        for(size_t i = 0; i < timer; i++){
            pin.write( 1 );
            pin.flush( );
            hwlib::wait_us( 100 );
            pin.write( 0 );
            pin.flush( );
            hwlib::wait_us( 100 );
            pin.write( 1 );
            pin.flush( );
            hwlib::wait_us( 100 );
            pin.write( 0 );
            pin.flush( );
            hwlib::wait_us( 100 );
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

int main(){
    hwlib::wait_ms(500);
    
    namespace target = hwlib::target;
    
    auto pin_o = hwlib::target::pin_out(hwlib::target::pins::d7);
    
    auto bee = Beeper(pin_o, 1000);
    bee.write_int_to_channel(3);
    bee.write_int_to_channel(4);
    bee.write_int_to_channel(5);
    bee.write_int_to_channel(6);
    bee.write_int_to_channel(7);
    rtos::run();
}