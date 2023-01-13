#include <cstdio>
#include <iostream>
#include <iterator>

#include "Program.h" 
#include "parser.hh" 

int main(int argc, char* argv[]){
	FILE* infile;
	string outfilename;
	if(argc<2){
		std::cerr<<"No input file given defaulting to stdin."<<std::endl;
		infile = stdin;
	}else{
		infile = fopen(argv[1],"r");
	}
	if(argc<3){
		std::cerr<<"No output file given defaulting to stdout."<<std::endl;
		outfilename = "";
	}else{
		outfilename = argv[2];
	}

	Program program;
	 
	run_parser(program,infile);

	program.finish();
	program.save(outfilename);


	return 0;
}