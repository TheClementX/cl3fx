#include "wav_parser.h"

int main() {
	//currently test for compilation and correct build system
	WavParser parser{}; 

	channel_v sine = parser.read_file("./unit_tests/test_files/sine.wav"); 
	parser.print_wav(); 
	channel_v saw = parser.read_file("unit_tests/test_files/saw.wav"); 
	parser.print_wav(); 
	channel_v tri = parser.read_file("./unit_tests/test_files/tri.wav"); 
	parser.print_wav(); 
	channel_v square = parser.read_file("./unit_tests/test_files/square.wav"); 
	parser.print_wav(); 
	channel_v table1 = parser.read_file("./unit_tests/test_files/table1.wav"); 
	parser.print_wav(); 
	channel_v table2 = parser.read_file("./unit_tests/test_files/table2.wav"); 
	parser.print_wav(); 
	channel_v tamb = parser.read_file("./unit_tests/test_files/tamb.wav"); 
	parser.print_wav(); 
	channel_v bell = parser.read_file("./unit_tests/test_files/bell.wav"); 
	parser.print_wav(); 
	channel_v bell16 = parser.read_file("./unit_tests/test_files/bell16bit.wav"); 
	parser.print_wav(); 
	channel_v bell24 = parser.read_file("./unit_tests/test_files/bell24bit.wav"); 
	parser.print_wav(); 
	
	std::cout << "parser unit tests passed - Exiting Successfully" << std::endl; 

	std::string out = "bell_test_write"; 
	parser.write_file(out, bell, 48000); 

	return 0; 
}
