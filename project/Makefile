FLAGS = -W -pedantic -std=c++17

.PHONY = all debug clean 

all: kompilator
debug: kompilator-dbg

kompilator: FLAGS += -O3
kompilator: lexer.o parser.o main.o
	$(CXX) $^ -o $@

kompilator-dbg: FLAGS += -g -DDEBUG
kompilator-dbg: lexer-dbg.o parser-dbg.o main.o
	$(CXX) $^ -o $@

%.o: %.cc
	$(CXX) $(FLAGS) -c $^

lexer.cc: lexer.l parser.hh
	flex -o $@ $<

parser.cc parser.hh: parser.y
	bison -Wall -d -o parser.cc $^

lexer-dbg.cc: lexer.l parser-dbg.hh
	flex -d -o $@ $<

parser-dbg.cc parser-dbg.hh: parser.y
	bison -t -Wall -d -o parser-dbg.cc $^


clean:
	rm -f *.o parser.cc parser.hh lexer.cc kompilator