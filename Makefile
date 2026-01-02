CXX := g++
CXXFLAGS := -Wall -Wextra -Werror -std=c++20 -g -I./src

all: parser_tests dft_tests

#linking
parser_tests: ./build/parser.o ./build/parser_tests.o
	$(CXX) $(CXXFLAGS) -o parser_tests $^

dft_tests: ./build/dft_algs.o ./build/dft_algs_tests.o
	$(CXX) $(CXXFLAGS) -o dft_tests $^

# compilation
# parser
./build/parser_tests.o: ./unit_tests/parser_tests.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

./build/parser.o: ./src/wav_parser.cpp ./src/wav_parser.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

#dft algs
./build/dft_algs_tests.o: ./unit_tests/dft_algs_tests.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

./build/dft_algs.o: ./src/dft_algs.cpp ./src/dft_algs.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f ./build/*.o parser_tests dft_tests

#important symbols 
#$< : first dependency 
#$@ : target 
#$^ : all dependencies 
#% : is wild card
