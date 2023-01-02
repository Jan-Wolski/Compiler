#include <cstdio>
#include <iostream>
#include <iterator>

#include "Program.h" 
#include "parser.hh" 

int main(int argc, char* argv[]){
	FILE* infile;
	if(argc<2){
		std::cerr<<"No input file given defaulting to stdin."<<std::endl;
		infile = stdin;
	}else{
		infile = fopen(argv[1],"r");
	}
	if(argc<3){
		std::cerr<<"No output file given defaulting to stdout."<<std::endl;
	}

	Program program;
	 
	run_parser(program,infile);


	return 0;
}