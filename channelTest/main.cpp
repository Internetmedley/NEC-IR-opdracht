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
};

class writer : public rtos::task<>{
private:
	logger & log;
public:
	writer: logger & log{}
	
	void main(){
		
		for(;;){
			log.add("hello /n");
			log.add("world /n");
		}
	}
};


int main(void){
	hwlib::wait_ms(500);
	namespace target= hwlib::target;
	auto log = logger();
	auto write = writer(log);
	rtos::run();
}