#include "hwlib.hpp"


int main( void ){
    namespace target = hwlib::target;
    hwlib::wait_ms( 2000 );
    auto p = target::pin_in( target::pins::d10 );
    for( ;; ){
        hwlib::cout << p.read();
        
    }
}