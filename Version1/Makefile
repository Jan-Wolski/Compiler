FLAGS = -pedantic -std=c++17

.PHONY = all release debug clean 

release: kompilator
debug: kompilator-dbg
all: release debug

kompilator: FLAGS += -g
kompilator: parser.cc lexer.cc main.cpp program.h enums.h
	$(CXX) $(FLAGS) $^ -o $@

kompilator-dbg: FLAGS += -g -DDEBUG
kompilator-dbg: parser-dbg.cc lexer-dbg.cc main.cpp program.h enums.h
	$(CXX) $(FLAGS) $^ -o $@


%.o: %.cpp
	$(CXX) $(FLAGS) -c $^

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
	rm -f *.o *.cc *.hh kompilator kompilator-dbg