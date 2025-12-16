CXX := g++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17 -g -I./src

all: parser_tests

#linking
parser_tests: ./build/parser.o ./build/parser_tests.o
	$(CXX) $(CXXFLAGS) -o parser_tests $^

# compilation
./build/parser_tests.o: ./unit_tests/parser_tests.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

./build/parser.o: ./src/wav_parser.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f ./build/*.o parser_tests

#important symbols 
#$< : first dependency 
#$@ : target 
#$^ : all dependencies 
#% : is wild card
