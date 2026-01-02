#include "wav_parser.h"

int main() {
	//currently test for compilation and correct build system
	WavParser parser{}; 

	channel_v sine = parser.read_file("./test_files/sine.wav"); 
	parser.print_wav(); 
	channel_v saw = parser.read_file("./test_files/saw.wav"); 
	channel_v tri = parser.read_file("./test_files/tri.wav"); 
	channel_v square = parser.read_file("./test_files/square.wav"); 
	channel_v table1 = parser.read_file("./test_files/table1.wav"); 
	channel_v table2 = parser.read_file("./test_files/table2.wav"); 
	channel_v tamb = parser.read_file("./test_files/tamb.wav"); 
	channel_v bell = parser.read_file("./test_files/bell.wav"); 
	channel_v bell16 = parser.read_file("./test_files/bell16bit.wav"); 
	channel_v bell24 = parser.read_file("./test_files/bell24bit.wav"); 
	
	std::cout << "parser unit tests passed - Exiting Successfully" << std::endl; 

	return 0; 
}
