#include <cstdio>
#include <iostream>
#include <iterator>

#include "program.h" 
#ifdef DEBUG
	#include "parser-dbg.hh" 
#else
	#include "parser.hh" 
#endif

int main(int argc, char* argv[]){
	FILE* infile;
	string outfilename;
	
	if(argc<2){
		std::cerr<<"Nie podano pliku wejściowego, używanie stdin."<<std::endl;
		infile = stdin;
	}else{
		infile = fopen(argv[1],"r");
		if(infile == NULL){
			std::cerr<<"Nie można otworzyć pliku wejściowego."<<std::endl;
		}
	}

	if(argc<3){
		std::cerr<<"Nie podano pliku wyjściowego, używanie stdout."<<std::endl;
		outfilename = "";
	}else{
		outfilename = argv[2];
	}

	Program program;
	 
	run_parser(program,infile);
	cout<<"Parsowanie zakończone."<<endl;
	program.finish();
	cout<<"Optymalizacja zakończona."<<endl;
	program.save(outfilename);
	cout<<"Zapisywanie zakończone."<<endl;

	return 0;
}