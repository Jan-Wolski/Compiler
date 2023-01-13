#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <fstream>

#include "enums.h"


/*Memory layout
0 - accumulator
1 - constant place
1.. - main
x.. - procedures
y.. - loop registers
*/


using namespace std;
typedef pair<enum Inst,long long> Instruction;
typedef map<string, long long> Var_table;

struct Value{
	long long val; 
	bool is_ref=false;
};
// typedef struct value Value;

struct Procedure{
	string name = "";
	vector<Instruction> insts;
	int bindings=0;
	Var_table table;
	int label;
	bool always_embed = false;
};
// typedef struct procedure Procedure;

struct Proc_call{
	string proc;
	vector<Value> args;
};
// typedef struct proc_call Proc_call;
typedef vector<Instruction> Inst_list;

class Program{
	public:

		const long long accu = 0;
		const long long expr_reg = 2;
		const long long comp_reg1 = 3;
		const long long comp_reg2 = 4;
		const long long first_var = 5;

		Var_table main_table;
		
		Procedure current_proc;

		Inst_list out_list;
		
		map<string,Procedure> procs;
		vector<Proc_call> calls;
		int labels_id = 0;

		Inst_list current_insts;
		Inst_list last_insts;
		stack<Inst_list> inst_stack;
		long long last_var_field=first_var;
		stack<Value> current_values; //value/reference , is reference

		Operators last_op = NOP;
		Comparisons last_comp = NO;



		Program(){
			name_procedure("*");
			reference("x");
			reference("y");
			// current_insts={};
			def_procedure();
		}
		
		int new_label(){
			labels_id++;
			return labels_id-1;
		}

		void new_seq(){
			copy(current_insts.begin(),current_insts.end(),last_insts.end());
			current_insts.clear();
		}

		void new_commands(){
			inst_stack.push(last_insts);
			last_insts = current_insts;
		}


		void def_procedure(){
			current_proc.label = new_label();
			current_proc.insts = last_insts;
			procs[current_proc.name] = current_proc;
			reset_procedure();
		}

		void reset_procedure(){
			current_proc = Procedure();
			current_insts.clear();
			last_var_field=first_var;
		}

		void def_main(){
			name_procedure("main");
			def_procedure();
		}


		void set_val(string id){
			cout<<"YOLOOOOOO"<<endl;
			long long posret;
			try{
				posret = current_proc.table.at(id);
			}catch(out_of_range &e){
				cerr << "Zmienna o nazwie " << id << "nie istnieje." << endl;
				exit(-3);
			}
			if(last_op == NOP){
				Value val = current_values.top();
				current_values.pop();
				if(val.is_ref){
					if(posret!=val.val){
						current_insts.push_back(make_pair(Inst::LOAD,val.val));
						current_insts.push_back(make_pair(Inst::STORE,posret));
					}
				}else{
					current_insts.push_back(make_pair(Inst::SET,val.val));
					current_insts.push_back(make_pair(Inst::STORE,posret));
				}
			}else{
				Operators oper = last_op;
				last_op=NOP;

				Value first = current_values.top();
				current_values.pop();
				Value second = current_values.top();
				current_values.pop();

				long long pos;

				if(first.is_ref){
					if(second.is_ref){
						pos = second.val;
					}else{
						if(oper == SUB || oper == DIV || oper == REM){
							current_insts.push_back(make_pair(Inst::SET,second.val));
							current_insts.push_back(make_pair(Inst::STORE,expr_reg));
							current_insts.push_back(make_pair(Inst::LOAD,first.val));
							pos = expr_reg;
						}else{
							pos = first.val;
							current_insts.push_back(make_pair(Inst::SET,second.val));
						}
					}
				}else{
					if(second.is_ref){
						pos = second.val;
						current_insts.push_back(make_pair(Inst::SET,first.val));
					}else{
						// long long val = const_eval(oper);
						// current_insts.push_back(make_pair(Inst::SET,val));
						// oper = 0;
						current_insts.push_back(make_pair(Inst::SET,second.val));
						current_insts.push_back(make_pair(Inst::STORE,expr_reg));
						current_insts.push_back(make_pair(Inst::SET,first.val));
					}
				}
				switch (oper){
					case ADD:
						current_insts.push_back(make_pair(Inst::ADD,pos));
					break;
					case SUB:
						current_insts.push_back(make_pair(Inst::SUB,pos));
					break;
					case MUL:
						current_insts.push_back(make_pair(Inst::CALL,create_call("*")));
					break;
					case DIV:
						current_insts.push_back(make_pair(Inst::CALL,create_call("/")));
					break;
					case REM:
						current_insts.push_back(make_pair(Inst::CALL,create_call("%")));
					break;
					case 0:
					break;
				}
				current_insts.push_back(make_pair(Inst::STORE,posret));
			}
		}

		void read(string id){

			long long pos;
			try{
				pos = current_proc.table.at(id);
			}catch(out_of_range &e){
				cerr << "Zmienna o nazwie " << id << "nie istnieje." << endl;
				exit(-3);
			}
			current_insts.push_back(Instruction(Inst::GET,pos));
		}

		void write(){
			auto value = current_values.top();
			current_values.pop();
			if (value.is_ref){
				current_insts.push_back(make_pair(Inst::PUT,value.val));
			}else{
				current_insts.push_back(make_pair(Inst::SET,value.val));
				current_insts.push_back(make_pair(Inst::PUT,accu));
			}
		}

		void name_procedure(string name){
			current_proc.name = string(name);
		}

		void var(string id ){
			if(current_proc.table.find(string(id)) != current_proc.table.end()){
				cerr << "Zmienna o nazwie " << id << " już istnieje." << endl;
				exit(-3);
				
			}
			current_proc.table[id] = last_var_field;
			last_var_field++;
		}

		void reference(string id ){
			current_proc.bindings++;
			var(id);
		}

		void op(Operators oper){
			last_op = oper;
		}

		long long create_call(const string &name){ //fix what if function is in uncalled proc
			Proc_call call;
			call.proc = name;
			while(!current_values.empty()){
				call.args.push_back(current_values.top());
				current_values.pop();
			}
			calls.push_back(call);
			return calls.size()-1;
		}

		void call_procedure(string name){
			try{
				Procedure pn = procs.at(name);
			}catch(out_of_range &e){
				cerr << "Procedura " << name << " nie istnieje." << endl;
				exit(-3);
			}
			current_insts.push_back(make_pair(Inst::CALL,create_call(name)));
		}

		void comp(Comparisons compr){
			last_comp = compr;
		}

		void control(Ctrl ctrl){
			auto first = current_values.top();
			current_values.pop();
			auto second = current_values.top();
			current_values.pop();

			long long fail_pos = new_label();
			long long success_pos = new_label();
			long long posA, posB;

			Comparisons compr = last_comp;
			last_comp = NO;


			if(first.is_ref){
				posA = first.val;
			}else{
				current_insts.push_back(make_pair(Inst::SET,first.val));
				current_insts.push_back(make_pair(Inst::STORE,comp_reg1));
			}

			if(second.is_ref){
				posB = second.val;
			}else{
				current_insts.push_back(make_pair(Inst::SET,second.val));
				current_insts.push_back(make_pair(Inst::STORE,comp_reg2));
			}

			if(ctrl == Ctrl::WHILE){
				switch (compr){
					case EQ:
						compr = NEQ;
					break;
					case GT:
						compr = LTE;
					break;
					case LT:
						compr = GTE;
					break;
					case GTE:
						compr = LT;
					break;
					case LTE:	
						compr = GT;
					break;
					case NEQ:
						compr = EQ;
					break;
				}
			}
			
			switch (compr){
				case EQ:
					current_insts = {
						{Inst::LOAD, posA},
						{Inst::SUB, posB},
						{Inst::JPOS, fail_pos},
						{Inst::LOAD, posB},
						{Inst::SUB, posA},
						{Inst::JPOS, fail_pos}
					};
				break;
				case GT:
					current_insts = {
						{Inst::LOAD,posA},
						{Inst::SUB,posB},
						{Inst::JZERO,fail_pos}
						};
				break;
				case LT:
					current_insts = {
						{Inst::LOAD,posB},
						{Inst::SUB,posA},
						{Inst::JZERO,fail_pos}
						};
				break;
				case GTE:
					current_insts = {
						{Inst::LOAD,posB},
						{Inst::SUB,posA},
						{Inst::JPOS, fail_pos}
						};
				break;
				case LTE:	
					current_insts = {
						{Inst::LOAD,posA},
						{Inst::SUB,posB},
						{Inst::JPOS, fail_pos}
						};
				break;
				case NEQ:
				int if_label = new_label();
				current_insts = {
					{Inst::LOAD, posA},
					{Inst::SUB, posB},
					{Inst::JPOS, if_label},
					{Inst::LOAD, posB},
					{Inst::SUB, posA},
					{Inst::JPOS, if_label},
					{Inst::JUMP, fail_pos},
					{Inst::LABEL, if_label}
				};
				break;
			}

			switch(ctrl){
				case Ctrl::IF:{
					copy(last_insts.begin(),last_insts.end(),current_insts.end());
					current_insts.push_back(make_pair(Inst::LABEL,fail_pos));
				break;}
				case Ctrl::IFELSE:{
					Inst_list good = inst_stack.top();
					int end = new_label();
					copy(good.begin(),good.end(),current_insts.end());
					current_insts.push_back(make_pair(Inst::JUMP,end));
					current_insts.push_back(make_pair(Inst::LABEL,fail_pos));
					copy(last_insts.begin(),last_insts.end(),current_insts.end());
					current_insts.push_back(make_pair(Inst::LABEL,end));
				break;}
				case Ctrl::WHILE:{
					int loop = new_label();
					current_insts.insert(current_insts.begin(),make_pair(Inst::LABEL,loop));
					copy(last_insts.begin(),last_insts.end(),current_insts.end());
					current_insts.push_back(make_pair(Inst::JUMP,loop));
					current_insts.push_back(make_pair(Inst::LABEL,fail_pos));
				break;}
				case Ctrl::UNTIL:{
					int loop = new_label();
					copy(current_insts.begin(),current_insts.end(),last_insts.end());
					current_insts = last_insts;
					current_insts.insert(current_insts.begin(),make_pair(Inst::LABEL,fail_pos));
				break;}
			}
		}

		void finish(){

		}

		void save(string &filename){
			ostream *out;
			if(filename.size() == 0){
				out = &cout;
			}else{
				ofstream file(filename);
				if(!file.good()){
					cerr<<"Can't open file: "<<filename<<endl;
					exit(-20);
				}
				out = &file;
			}

			for(Instruction instruction : out_list){
				Inst inst = instruction.first;
				long long arg = instruction.second;
				string text;
				//([A-Z]*),
				/*
								case $1:
				text = "$1";
				break;
				*/
				switch(inst){
					case Inst::GET:
					text = "GET";
					break; case Inst::PUT:
					text = "PUT";
					break; case Inst::LOAD:
					text = "LOAD";
					break; case Inst::STORE:
					text = "STORE";
					break; case Inst::LOADI:
					text = "LOADI";
					break; case Inst::STOREI:
					text = "STOREI";
					break; case Inst::ADD:
					text = "ADD";
					break; case Inst::SUB:
					text = "SUB";
					break; case Inst::ADDI:
					text = "ADDI";
					break; case Inst::SUBI:
					text = "SUBI";
					break; case Inst::SET:
					text = "SET";
					break; case Inst::HALF:
					text = "HALF";
					break; case Inst::JUMP:
					text = "JUMP";
					break; case Inst::JPOS:
					text = "JPOS";
					break; case Inst::JZERO:
					text = "JZERO";
					break; case Inst::JUMPI:
					text = "JUMPI";
					break; case Inst::HALT:
					text = "HALT";
					break; 
				}
				if(inst == Inst::HALT){
					*out<<text<<endl;
				}else{
					*out<<text<<" "<<arg<<endl;
				}
				
			}
		}


		void value(string id){
			long long pos;
			try{
				pos = current_proc.table.at(id);
			}catch(out_of_range &e){
				cerr << "Zmienna " << id << " nie istnieje." << endl;
				exit(-3);
			}
			current_values.push(Value{pos,true});
		}
		void value(long long num){
			current_values.push(Value{num,false});
		}

		void parameter(string name){
			value(name);
		}
};
