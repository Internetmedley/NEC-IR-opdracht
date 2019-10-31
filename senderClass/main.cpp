#include "hwlib.hpp"
#include "rtos.hpp"
class for;
class Ir_sender : public rtos::task<>{
private:
	rtos::channel<uint16_t ,1> ir_buffer;
	rtos::timer sendTimer;
	void bitPrint(uint16_t a){
		for (int16_t i = 15; i >= 0; --i){
			hwlib::cout << ((((a >> i) & 1) == 1) ? 1 : 0);
			if(i == 8){hwlib::cout << ' ';}
		}
		hwlib::cout << hwlib::endl;}
	void driver_led(uint16_t msg, auto ir_led){
		ir_led.write(1);
		ir_led.flush();
		sendTimer.set(9000);
		wait(sendTimer);
		ir_led.write(0);
		ir_led.flush();
		sendTimer.set(4500);
		wait(sendTimer);
		for(int16_t i = 0; i < 16; i++){
			ir_led.write(1);
			ir_led.flush();
			sendTimer.set(560);
			wait(sendTimer);
			ir_led.write(0);
			ir_led.flush();
			if(((msg >> i) & 1) == 1){ sendTimer.set(1690); wait(sendTimer); }
			else{ sendTimer.set(560); wait(sendTimer);}
		}
		ir_led.write(1);
		ir_led.flush();
		sendTimer.set(560);
		wait(sendTimer);
		ir_led.write(0);
		ir_led.flush();
	}
	void main(){
		auto ir_led = hwlib::target::d2_36kHz();
		for(;;){
			auto c = ir_buffer.read();
			bitPrint(c);
			driver_led(c,ir_led);
		}
	}

public: 
	Ir_sender():
		task("sender"), ir_buffer(this,"ir buffer"){}

		/*
			
			write lsb to channel we choose lsb becaus Nec protocol starts with lsb as well
		*/
		void write_to_channel(int playerNum, int weaponPower){
			uint16_t dataByte = 0;
			dataByte = (dataByte | 1);
			dataByte |= (playerNum <<1);
			dataByte |= (weaponPower <<6);
			dataByte |= ((weaponPower ^ playerNum) << 11);
			ir_buffer.write(dataByte);
			
		}
};

class writerTest : public rtos::task<>{
private:
	Ir_sender & sender;
	int player_number;
	int weapon_power;
public:
	writerTest(Ir_sender & sender, const int & player_number, const int & weapon_power):
	sender(sender), 
	player_number(player_number),
	weapon_power(weapon_power){}

	void main(){
		for(;;){ 
			sender.write_to_channel(player_number, weapon_power);
		}
	}


};


int main(void){
	hwlib::wait_ms(500);
	int player = 2;
	int weapon = 5;
	auto sender = Ir_sender();
	auto writer = writerTest(sender,player,weapon);
	rtos::run();
}

//		
//	}
//public: 
//	logger(): 
//		task("logger"), buffer(this,"char buffer"){}
//	
//	//writes the given char into the rtos channel
//	void add(char c){buffer.write(c);}
//
//	//writes the given string as single chars into the rtos channel
//	void add(const char * s){
//		while(*s != '\0'){buffer.write(*s++);}
//	}
//
//	char read(){return buffer.read();}
//};
//
//class ZRX543: public rtos::task<>{
//private:
//	logger & log;
//	hwlib::keypad<16> & keypadObj;
//	
//public:
//	ZRX543(logger & log, auto & keypadObj): log(log), keypadObj(keypadObj){}
//	
//	void main(){
//		
//		for(;;){
//			if(keypadObj.pressed()){
//			log.add(keypadObj.getc());
//			log.add("\n");
//			}
//		}
//	}
//};
//
//
//
//
//int main(void){
//	namespace target= hwlib::target;
//	auto out0 				= hwlib::target::pin_oc( hwlib::target::pins::a0 );
//	auto out1 				= hwlib::target::pin_oc( hwlib::target::pins::a1 );
//	auto out2 				= hwlib::target::pin_oc( hwlib::target::pins::a2 );
//	auto out3 				= hwlib::target::pin_oc( hwlib::target::pins::a3 );
//	auto in0  				= hwlib::target::pin_in( hwlib::target::pins::a4 );
//	auto in1  				= hwlib::target::pin_in( hwlib::target::pins::a5 );
//	auto in2  				= hwlib::target::pin_in( hwlib::target::pins::a6 );
//	auto in3  				= hwlib::target::pin_in( hwlib::target::pins::a7 );
//	auto out_port 			= hwlib::port_oc_from( out0, out1, out2, out3 );
//	auto in_port  			= hwlib::port_in_from( in0,  in1,  in2,  in3  );
//	auto matrixObj			= hwlib::matrix_of_switches( out_port, in_port );
//	auto keypadObj			= hwlib::keypad< 16 >( matrixObj, "D#0*C987B654A321" );
//	auto log = logger();
//	auto writer = ZRX543(log, keypadObj);
//	rtos::run();
//}




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
