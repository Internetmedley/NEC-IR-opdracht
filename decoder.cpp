#include "msg_decoder.hpp"


//Fill array with channel data
//Check array values and write either 0 or 1 into channel
void msg_decoder::decode(){
	static int MessageBuffer[32];
	for( i=0; i < size_t(MessageBuffer); i++){
		listner.Read() = MessageBuffer.at(i);
	}
	
	for(int pauseIndex = 0; pauseIndex < size_t(MessageBuffer); i++){
		int pause = MessageBuffer.at(pauseIndex);
		switch(pause){
			case (pause > 1900 && pause < 2000);
				listner.write(1);
				break;
			case (pause >500 && pause < 600);
				listner.write(0);
				break;
			default:
				break;
		}
	}
}
