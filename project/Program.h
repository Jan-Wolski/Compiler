#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <vector>
#include <stack>

#include "enums.h"

using namespace std;
typedef pair<enum Inst,long long> Instruction;
typedef map<string, long long> Var_table;

struct procedure{
	string name = "";
	vector<Instruction> insts;
	int bindings=0;
};

typedef struct procedure Procedure;



class Program{
	public:
		Var_table main_table;
		vector<Var_table> sub_tables;
		const long long accu = 0;
		Procedure current_proc;

		//name_procedure context
			string procedure_name;



		vector<Instruction> current_inst;
		Var_table current_table;
		long long last_field=2;
		stack<pair<long long,bool>> current_values; //value/reference , is reference


		Program(){}

		void def_procedure(){

		}
		void def_main(){
			name_procedure("main");
			last_field = 2;
			def_procedure();
		}
		void new_commands(){}
		void set_val(char* id){}
		void if_else(bool ifelse = false){}
		void loop_while(){}
		void loop_until(){}
		void load_procedure(){}
		void read(char* id){
			long long pos;
			try{
				pos = current_table.at(id);
			}catch(out_of_range &e){
				cerr << "Variable of name " << id << "doesn't exist." << endl;
				exit(-3);
			}
			current_inst.push_back(Instruction(Inst::GET,pos));
		}
		void write(){
			auto value = current_values.top;
			current_values.pop();
			if (value.second){
				current_inst.push_back(make_pair(Inst::PUT,value.first));
			}else{
				current_inst.push_back(make_pair(Inst::SET,value.first));
				current_inst.push_back(make_pair(Inst::PUT,accu));
			}
		}
		void name_procedure(char* name){
			procedure_name = string(name);
		}

		void reset_var(){
			current_table = map<string, long long>();
			last_field = 1;
		}

		void var(char* id ){
			if(current_table.find(string(id)) != current_table.end()){
				cerr << "Variable of name " << id << "already exist." << endl;
				exit(-3);
				
			}
			current_table[id] = last_field;
			last_field++;
		}

		void reference(char* id ){
			current_proc.bindings++;
			var(id);
		}

		void op(Operators oper){
			auto first = current_values.top;
			current_values.pop();
			auto second = current_values.top;
			current_values.pop();

			long long pos;
			if(first.second){
				pos = first.first;
				if(second.second){
					current_inst.push_back(make_pair(Inst::LOAD,second.first));
				}else{
					current_inst.push_back(make_pair(Inst::SET,second.first));
				}
			}else{
				current_inst.push_back(make_pair(Inst::SET,first.first));
				if(second.second){
					pos = second.first;
				}else{
					pos = 1;
					current_inst.push_back(make_pair(Inst::STORE,1));
					current_inst.push_back(make_pair(Inst::SET,second.first));
				}
			}

			switch (oper){
				case ADD:
					current_inst.push_back(make_pair(Inst::ADD,pos));
				break;
				case SUB:
					current_inst.push_back(make_pair(Inst::SUB,pos));
				break;
				case MUL:
					current_inst.push_back(make_pair(Inst::CALL,call_proc("multiply")));
				break;
				case DIV:
					current_inst.push_back(make_pair(Inst::CALL,call_proc("divide")));
				break;
				case REM:
					current_inst.push_back(make_pair(Inst::CALL,call_proc("remainder")));
				break;

			}
		}
		void comp(Comparisons compr){

		}
		void value(char* id){
			long long pos;
			try{
				pos = current_table.at(id);
			}catch(out_of_range &e){
				cerr << "Variable of name " << id << "doesn't exist." << endl;
				exit(-3);
			}
			current_values.push(pair::make_pair(pos,true));
		}
		void value(long long num){
			current_values.push(pair::make_pair(num,false));
		}
};
