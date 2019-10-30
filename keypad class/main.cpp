#include "hwlib.hpp"
#include "rtos.hpp"


class logger : public rtos::task<>{
private:
	rtos::channel<char, 1024> buffer; 
	void main (){
		for(;;){
			auto c = buffer.read();
			hwlib::cout<<c;
			}
		
	}
public: 
	logger(): 
		task("logger"), buffer(this,"char buffer"){}
	
	//writes the given char into the rtos channel
	void add(char c){buffer.write(c);}

	//writes the given string as single chars into the rtos channel
	void add(const char * s){
		while(*s != '\0'){buffer.write(*s++);}
	}

	char read(){return buffer.read();}
};

class ZRX543: public rtos::task<>{
private:
	logger & log;
	hwlib::keypad<16> & keypadObj;
	
public:
	ZRX543(logger & log, auto & keypadObj): log(log), keypadObj(keypadObj){}
	
	void main(){
		
		for(;;){
			if(keypadObj.pressed()){
			log.add(keypadObj.getc());
			log.add("\n");
			}
		}
	}
};




int main(void){
	namespace target= hwlib::target;
	auto out0 				= hwlib::target::pin_oc( hwlib::target::pins::a0 );
	auto out1 				= hwlib::target::pin_oc( hwlib::target::pins::a1 );
	auto out2 				= hwlib::target::pin_oc( hwlib::target::pins::a2 );
	auto out3 				= hwlib::target::pin_oc( hwlib::target::pins::a3 );
	auto in0  				= hwlib::target::pin_in( hwlib::target::pins::a4 );
	auto in1  				= hwlib::target::pin_in( hwlib::target::pins::a5 );
	auto in2  				= hwlib::target::pin_in( hwlib::target::pins::a6 );
	auto in3  				= hwlib::target::pin_in( hwlib::target::pins::a7 );
	auto out_port 			= hwlib::port_oc_from( out0, out1, out2, out3 );
	auto in_port  			= hwlib::port_in_from( in0,  in1,  in2,  in3  );
	auto matrixObj			= hwlib::matrix_of_switches( out_port, in_port );
	auto keypadObj			= hwlib::keypad< 16 >( matrixObj, "D#0*C987B654A321" );
	auto log = logger();
	auto writer = ZRX543(log, keypadObj);
	rtos::run();
}




//class logger : public rtos::task<> {
//private:
//
//   rtos::channel< char, 1024 > buffer;
//   
//   void main(){
//      for(;;){
//         auto c = buffer.read();
//         hwlib::cout << c;
//      }
//   }
//   
//public:
//   logger( ):
//      task( "logger" ),
//      buffer( this, "character buffer" )
//   { }   
//   
//   void add( char c ){
//      buffer.write( c );     
//   }
//   void add( const char * s ){
//      while( *s != '\0' ){
//         buffer.write( *s++ );     
//      }
//   }
//};
//
//class writer : public rtos::task<> {
//private:
//
//   hwlib::pin_out & pin;
//   logger & log;
//   rtos::clock blink_clock;
//   rtos::clock log_clock;
//   
//   void main(){
//      bool level = 0;
//      for(;;){
//         auto event = wait( blink_clock + log_clock );
//         if( event == blink_clock ){
//            level = ! level;
//            pin.write( level ); 
//            pin.flush();
//         } else {
//            log.add( "Hello\n" );
//         }
//      }
//   } 
//   
//public:
//   writer( 
//      hwlib::pin_out & pin,
//      logger & log
//   ):
//      task( "writer" ),
//      pin( pin ),
//      log( log ),
//      blink_clock( this, 100 * rtos::ms, "blink_clock"),
//      log_clock( this, 1'000 * rtos::ms, "log_clock")
//   {}   
//};
//
//int main( void ){	
//   
//   // wait for the PC console to start
//   hwlib::wait_ms( 500 );
//   
//   hwlib::cout << "channel demo\n";
//   
//   namespace target = hwlib::target;
//   
//   auto led = target::pin_out( target::pins::d42 );      
//   
//   auto log = logger();
//   auto write = writer( led, log );
//   rtos::run();
//}
