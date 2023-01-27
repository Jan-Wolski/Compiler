#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <iostream>
#include <iomanip>
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
struct Cell{
	Inst inst; 
	long long val;
};

// typedef pair<enum Inst,long long> Cell;
typedef map<string, long long> Var_table;
typedef vector<Cell> Inst_list;

struct Value{
	long long val; 
	bool is_ref=false;
};

struct Procedure{
	string name = "";
	Inst_list insts;
	
	Var_table table;
	unsigned int bindings=0;
	int padding=0;
	int retadr=0;

	int label = -1;
	bool always_embed = false;
};

struct Proc_call{
	string proc;
	string from_proc;
	vector<Value> args;
};



class Program{
	public:
		const long long accu = 0;
		const long long expr_reg1 = 1;
		const long long expr_reg2 = 2;
		const long long comp_reg1 = 3;
		const long long comp_reg2 = 4;
		const long long one_reg = 5;
		const long long first_var = 6;

		bool one_reg_used = false;

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
		queue<Value> current_values; //value/reference , is reference
		stack<Value> comp_values; //value/reference , is reference

		int param_num=0;
		Operators last_op = NOP;
		stack<Comparisons> comp_stack;
		// Comparisons last_comp = NO;

	void mul(){
		int load = new_label();
		int end = new_label();
		int justa = new_label();
		int set_zero = new_label();

		long long inA = first_var;
		long long inB = first_var+1;
		long long posC = first_var+2;
		long long posA = first_var+3;
		long long posB = first_var+4;
		long long posD = first_var+5;
		long long posE = first_var+6;

		Inst_list instrs = {
			{Inst::LOAD,inA},
			{Inst::JZERO,set_zero},
			{Inst::STORE,posA},
			{Inst::LOAD,inB},
			{Inst::JZERO,set_zero},
			{Inst::STORE,posB},
			{Inst::SET,0},
			{Inst::STORE,posC},
			{Inst::LABEL,load},

			{Inst::LOAD,posB},
			{Inst::JZERO,end},
			{Inst::STORE,posD},
			{Inst::HALF,1},
			{Inst::STORE,posB},
			{Inst::ADD,0},
			{Inst::STORE,posE},
			{Inst::LOAD,posD},
			{Inst::SUB,posE},
			{Inst::JZERO,justa},

			{Inst::LOAD,posA},
			{Inst::ADD,posC},
			{Inst::STORE,posC},
			{Inst::LABEL,justa},
			{Inst::LOAD,posA},
			{Inst::ADD,posA}, 
			{Inst::STORE,posA},
			{Inst::JUMP,load},
			{Inst::LABEL,set_zero},
			{Inst::STORE,posC},
			{Inst::LABEL,end}
		};
		last_insts = instrs;
		name_procedure("*");
		reference("x");
		reference("y");
		reference("z");
		var("a");
		var("b");
		var("d");
		var("e");
		def_procedure();

	}

	void div(){
		int div0a = new_label();
		int div0b = new_label();
		int shloop = new_label();
		int shldone = new_label();
		int ezcase = new_label();
		int loop = new_label();
		int endif = new_label();
		int end = new_label();

		long long inA = first_var;
		long long inB = first_var+1;
		long long posA = first_var+2;//rest
		long long posD = first_var+3;//quotient
		long long posB = first_var+4;
		long long posC = first_var+5;
		


		Inst_list instrs={
			{Inst::LOAD, inA},
			{Inst::STORE, posA},
			{Inst::JZERO, div0a},
			{Inst::LOAD, inB},
			{Inst::JZERO, div0b},
			{Inst::STORE, posB},
			{Inst::SET, 0},
			{Inst::STORE, posC},
			{Inst::LABEL, shloop},
			{Inst::LOAD, posB},
			{Inst::SUB, posA},
			{Inst::JPOS, shldone},
			{Inst::LOAD, posB},
			{Inst::ADD, 0},
			{Inst::STORE, posB},
			{Inst::LOAD, posC},
			{Inst::ADD, one_reg},
			{Inst::STORE, posC},
			{Inst::JUMP, shloop},
			{Inst::LABEL, shldone},
			{Inst::LOAD, posC},
			{Inst::JZERO, ezcase},
			{Inst::SET, 0},
			{Inst::STORE, posD},
			{Inst::LABEL, loop},
			{Inst::LOAD, posB},
			{Inst::HALF, posB},
			{Inst::STORE, posB},
			{Inst::LOAD, posC},
			{Inst::SUB, one_reg},
			{Inst::STORE, posC},
			{Inst::LOAD, posD},
			{Inst::ADD, 0},
			{Inst::STORE, posD},
			{Inst::LOAD, posB},
			{Inst::SUB, posA},
			{Inst::JPOS, endif},
			{Inst::LOAD, posA},
			{Inst::SUB, posB},
			{Inst::STORE, posA},
			{Inst::LOAD, posD},
			{Inst::ADD, one_reg},
			{Inst::STORE, posD},
			{Inst::LABEL, endif},
			{Inst::LOAD, posC},
			{Inst::JZERO, end},
			{Inst::JUMP, loop},
			{Inst::LABEL, div0b},

			{Inst::STORE, posA},
			{Inst::LABEL, div0a},
			{Inst::LABEL, ezcase},

			{Inst::STORE, posD},
			{Inst::LABEL, end}
			// {Inst::LOAD, posD},
			// {Inst::STORE, outQuot},
			// {Inst::LOAD, posA},
			// {Inst::STORE, outRest}
		};
		last_insts = instrs;
		name_procedure("/");
		reference("x");
		reference("y");
		reference("r");
		reference("q");	
		var("b");
		var("c");
		def_procedure();

	}

		Program(){
			mul();
			div();
		}
		
		int new_label(){
			labels_id++;
			return labels_id-1;
		}

		void new_seq(){
			// cout<<"add"<<endl;
			// out_list = current_insts;
			// save("");
			last_insts.insert(last_insts.end(),current_insts.begin(),current_insts.end());
			current_insts.clear();
		}

		void new_commands(){
			// cout<<"new"<<endl;		
			// out_list = current_insts;
			// save("");
			if(!last_insts.empty()){
				inst_stack.push(last_insts);
			}
			last_insts = current_insts;
			current_insts.clear();
		}


		void def_procedure(){
			// current_proc.label = new_label();
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
			long long posret;
			try{
				posret = current_proc.table.at(id);
			}catch(out_of_range &e){
				cerr << "Zmienna o nazwie " << id << "nie istnieje." << endl;
				exit(-3);
			}
			if(last_op == NOP){
				Value val = current_values.front();
				current_values.pop();
				if(val.is_ref){
					if(posret!=val.val){
						current_insts.push_back(Cell{Inst::LOAD,val.val});
						current_insts.push_back(Cell{Inst::STORE,posret});
					}
				}else{
					current_insts.push_back(Cell{Inst::SET,val.val});
					current_insts.push_back(Cell{Inst::STORE,posret});
				}
			}else{
				Operators oper = last_op;
				last_op=NOP;

				Value first = current_values.front();
				current_values.pop();
				Value second = current_values.front();
				current_values.pop();

				// cout<<first.val<<" "<<oper<<" "<<second.val<<endl;

				long long pos;

				

				switch (oper){
					case ADD:
					case SUB:
						if(first.is_ref){
							if(second.is_ref){
								current_insts.push_back(Cell{Inst::LOAD,first.val});
								pos = second.val;
							}else{
								if(oper == SUB){
									current_insts.push_back(Cell{Inst::SET,second.val});
									current_insts.push_back(Cell{Inst::STORE,expr_reg1});
									current_insts.push_back(Cell{Inst::LOAD,first.val});
									pos = expr_reg1;
								}else{
									pos = first.val;
									current_insts.push_back(Cell{Inst::SET,second.val});
								}
							}
						}else{
							if(second.is_ref){
								pos = second.val;
								current_insts.push_back(Cell{Inst::SET,first.val});
							}else{
								// long long val = const_eval(oper);
								// current_insts.push_back(Cell{Inst::SET,val});
								// oper = 0;
								pos=expr_reg1;
								current_insts.push_back(Cell{Inst::SET,second.val});
								current_insts.push_back(Cell{Inst::STORE,expr_reg1});
								current_insts.push_back(Cell{Inst::SET,first.val});
							}
						}
						if(oper == ADD){
							current_insts.push_back(Cell{Inst::ADD,pos});
						}else{			
							current_insts.push_back(Cell{Inst::SUB,pos});
						}
						current_insts.push_back(Cell{Inst::STORE,posret});
					break;
					case MUL:
						if(first.is_ref){
							if(second.is_ref){
								current_values.push(first);
								current_values.push(second);
								current_values.push(Value{posret,true});
								param_num=3;
								current_insts.push_back(Cell{Inst::CALL,create_call("*")});
							}else{
								if(second.val<10){
									current_insts.push_back(Cell{Inst::LOAD,first.val});
									for(int i = 0; i<second.val-1;i++){
										current_insts.push_back(Cell{Inst::ADD,first.val});
									}
									current_insts.push_back(Cell{Inst::STORE,posret});
								}else{
									current_values.push(first);
									current_insts.push_back(Cell{Inst::SET,second.val});
									current_insts.push_back(Cell{Inst::STORE,expr_reg2});
									current_values.push(Value{expr_reg2,true});
									current_values.push(Value{posret,true});
									param_num=3;
									current_insts.push_back(Cell{Inst::CALL,create_call("*")});
								}
							}
						}else{
							if(second.is_ref){
								if(second.val<10){
									current_insts.push_back(Cell{Inst::LOAD,second.val});
									for(int i = 0; i<first.val-1;i++){
										current_insts.push_back(Cell{Inst::ADD,second.val});
									}
									current_insts.push_back(Cell{Inst::STORE,posret});
								}else{
									current_insts.push_back(Cell{Inst::SET,second.val});
									current_insts.push_back(Cell{Inst::STORE,expr_reg1});
									current_values.push(Value{expr_reg2,true});
									current_values.push(second);
									current_values.push(Value{posret,true});
									param_num=3;
									current_insts.push_back(Cell{Inst::CALL,create_call("*")});
								}
							}else{
								current_insts.push_back(Cell{Inst::SET,first.val});
								current_insts.push_back(Cell{Inst::STORE,expr_reg1});
								current_values.push(Value{expr_reg1,true});
								current_insts.push_back(Cell{Inst::SET,second.val});
								current_insts.push_back(Cell{Inst::STORE,expr_reg2});
								current_values.push(Value{expr_reg2,true});
								current_values.push(Value{posret,true});
								param_num=3;
								current_insts.push_back(Cell{Inst::CALL,create_call("*")});
							}
						}
					break;
					case DIV:
					case REM:
						if(first.is_ref){
							if(second.is_ref){
								current_values.push(first);
								current_values.push(second);
								if(oper == DIV){
									current_values.push(Value{comp_reg1,true});
									current_values.push(Value{posret,true});
								}else{
									current_values.push(Value{posret,true});
									current_values.push(Value{comp_reg1,true});
								}
								param_num=4;
								current_insts.push_back(Cell{Inst::CALL,create_call("/")});
							}else{
								if(oper == DIV && second.val == 2){
									current_insts.push_back(Cell{Inst::LOAD,first.val});
									current_insts.push_back(Cell{Inst::HALF,1});
									current_insts.push_back(Cell{Inst::STORE,posret});
								}else{
									current_values.push(first);
									current_insts.push_back(Cell{Inst::SET,second.val});
									current_insts.push_back(Cell{Inst::STORE,expr_reg1});
									current_values.push(Value{expr_reg1,true});
									if(oper == DIV){
										current_values.push(Value{comp_reg1,true});
										current_values.push(Value{posret,true});
									}else{
										current_values.push(Value{posret,true});
										current_values.push(Value{comp_reg1,true});
									}
									param_num=4;
									current_insts.push_back(Cell{Inst::CALL,create_call("/")});
								}
							}
						}else{
							if(second.is_ref){
								current_insts.push_back(Cell{Inst::SET,first.val});
								current_insts.push_back(Cell{Inst::STORE,expr_reg1});
								current_values.push(Value{expr_reg1,true});
								current_values.push(second);
								if(oper == DIV){
									current_values.push(Value{comp_reg1,true});
									current_values.push(Value{posret,true});
								}else{
									current_values.push(Value{posret,true});
									current_values.push(Value{comp_reg1,true});
								}
								param_num=4;
								current_insts.push_back(Cell{Inst::CALL,create_call("/")});
							}else{
								current_insts.push_back(Cell{Inst::SET,first.val});
								current_insts.push_back(Cell{Inst::STORE,expr_reg1});
								current_values.push(Value{expr_reg1,true});
								current_insts.push_back(Cell{Inst::SET,second.val});
								current_insts.push_back(Cell{Inst::STORE,expr_reg2});
								current_values.push(Value{expr_reg2,true});
								if(oper == DIV){
									current_values.push(Value{comp_reg1,true});
									current_values.push(Value{posret,true});
								}else{
									current_values.push(Value{posret,true});
									current_values.push(Value{comp_reg1,true});
								}
								param_num=4;
								current_insts.push_back(Cell{Inst::CALL,create_call("/")});
							}
						}
					break;
					case 0:
					break;
				}
				
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
			current_insts.push_back(Cell{Inst::GET,pos});
		}

		void write(){
			auto value = current_values.front();
			current_values.pop();
			if (value.is_ref){
				current_insts.push_back(Cell{Inst::PUT,value.val});
			}else{
				current_insts.push_back(Cell{Inst::SET,value.val});
				current_insts.push_back(Cell{Inst::PUT,accu});
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

		long long create_call(const string name){
			Procedure pn;
			try{
				pn = procs.at(name);
			}catch(out_of_range &e){
				cerr << "Procedura " << name << " nie istnieje." << endl;
				exit(-3);
			}



			Proc_call call;
			call.proc = name;

			if(param_num<pn.bindings){
				cerr << "Podano za mało argumentów do procedury "<<name<<"."<<endl;
				exit(-4);
			}else if(param_num>pn.bindings){
				cerr << "Podano za dużo argumentów do procedury "<<name<<"."<<endl;
				exit(-4);
			}


			while(param_num>0){
				call.args.push_back(current_values.front());
				current_values.pop();
				param_num--;
			}


			calls.push_back(call);
			return calls.size()-1;
		}

		void call_procedure(string name){
			current_insts.push_back(Cell{Inst::CALL,create_call(name)});
		}

		void comp(Comparisons compr){
			comp_stack.push(compr);
			comp_values.push(current_values.front());
			current_values.pop();
			comp_values.push(current_values.front());
			current_values.pop();
		}

		void control(Ctrl ctrl){
			auto second = comp_values.top();
			comp_values.pop();
			auto first = comp_values.top();
			comp_values.pop();

			long long fail_pos = new_label();
			long long success_pos = new_label();
			long long posA, posB;

			Comparisons compr = comp_stack.top();
			comp_stack.pop();

			// cout<<first.val<<" "<<compr<<" "<<second.val<<endl;
			Inst_list comp_insts;


			// if(ctrl == Ctrl::WHILE){
			// 	switch (compr){
			// 		case EQ:
			// 			compr = NEQ;
			// 		break;
			// 		case GT:
			// 			compr = LTE;
			// 		break;
			// 		case LT:
			// 			compr = GTE;
			// 		break;
			// 		case GTE:
			// 			compr = LT;
			// 		break;
			// 		case LTE:	
			// 			compr = GT;
			// 		break;
			// 		case NEQ:
			// 			compr = EQ;
			// 		break;
			// 	}
			// }

			enum Type { consts, zero, normal };
			Type t;
			if(first.val != second.val || first.is_ref != second.is_ref){
				if(first.is_ref){
					posA = first.val;
				}else{
					if(first.val == 0){
						posA = 0;
					}else{
						posA=comp_reg1;
						current_insts.push_back(Cell{Inst::SET,first.val});
						current_insts.push_back(Cell{Inst::STORE,comp_reg1});	
					}

				}

				if(second.is_ref){
					posB = second.val;
				}else{
					if(second.val == 0){
						posB = 0;
					}else{
						posB=comp_reg2;
						current_insts.push_back(Cell{Inst::SET,second.val});
						current_insts.push_back(Cell{Inst::STORE,comp_reg2});
					}

				}
			}else{
				posA = first.val;
				posB = second.val;
			}

			if(posA == posB){
				switch (compr){
					case EQ:
						comp_insts = {

						};
					break;
					case GT:
						comp_insts = {
							{Inst::JZERO,fail_pos}
							};
					break;
					case LT:
						comp_insts = {
							{Inst::JZERO,fail_pos}
							};
					break;
					case GTE:
						comp_insts = {

							};
					break;
					case LTE:	
						comp_insts = {

							};
					break;
					case NEQ:
						comp_insts = {
							{Inst::JZERO, fail_pos}
						};
					break;
				}
			}else if(posA == 0 || posB == 0){
				if(posA == 0){
					switch (compr){
						case GT:
							compr = LT;
						break;
						case LT:
							compr = GT;
						break;
						case GTE:
							compr = LTE;
						break;
						case LTE:	
							compr = GTE;
						break;
					}
					posA = posB;
					posB = 0;

				}

				switch (compr){
					case EQ:
						comp_insts = {
							{Inst::LOAD, posA},
							{Inst::JPOS, fail_pos}
						};
					break;
					case GT:
						comp_insts = {
							{Inst::LOAD,posA},
							{Inst::JZERO,fail_pos}
							};
					break;
					case LT:
						comp_insts = {
							{Inst::JZERO,fail_pos}
							};
					break;
					case GTE:
						comp_insts = {

							};
					break;
					case LTE:	
						comp_insts = {
							{Inst::LOAD,posA},
							{Inst::JPOS, fail_pos}
							};
					break;
					case NEQ:
						comp_insts = {
							{Inst::LOAD, posA},
							{Inst::JZERO, fail_pos}
						};
					break;
				}
			}else{

				switch (compr){
					case EQ:
						comp_insts = {
							{Inst::LOAD, posA},
							{Inst::SUB, posB},
							{Inst::JPOS, fail_pos},
							{Inst::LOAD, posB},
							{Inst::SUB, posA},
							{Inst::JPOS, fail_pos}
						};
					break;
					case GT:
						comp_insts = {
							{Inst::LOAD,posA},
							{Inst::SUB,posB},
							{Inst::JZERO,fail_pos}
							};
					break;
					case LT:
						comp_insts = {
							{Inst::LOAD,posB},
							{Inst::SUB,posA},
							{Inst::JZERO,fail_pos}
							};
					break;
					case GTE:
						comp_insts = {
							{Inst::LOAD,posB},
							{Inst::SUB,posA},
							{Inst::JPOS, fail_pos}
							};
					break;
					case LTE:	
						comp_insts = {
							{Inst::LOAD,posA},
							{Inst::SUB,posB},
							{Inst::JPOS, fail_pos}
							};
					break;
					case NEQ:
						int if_label = new_label();
						comp_insts = {
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
			}

			current_insts.insert(current_insts.end(),comp_insts.begin(),comp_insts.end());

			switch(ctrl){
				case Ctrl::IF:{
					current_insts.insert(current_insts.end(),last_insts.begin(),last_insts.end());
					current_insts.push_back(Cell{Inst::LABEL,fail_pos});
				break;}
				case Ctrl::IFELSE:{
					Inst_list good = inst_stack.top();
					inst_stack.pop();
					int end = new_label();
					current_insts.insert(current_insts.end(),good.begin(),good.end());
					current_insts.push_back(Cell{Inst::JUMP,end});
					current_insts.push_back(Cell{Inst::LABEL,fail_pos});
					current_insts.insert(current_insts.end(),last_insts.begin(),last_insts.end());
					current_insts.push_back(Cell{Inst::LABEL,end});
				break;}
				case Ctrl::WHILE:{
					int loop = new_label();
					current_insts.insert(current_insts.begin(),Cell{Inst::LABEL,loop});
					current_insts.insert(current_insts.end(),last_insts.begin(),last_insts.end());
					current_insts.push_back(Cell{Inst::JUMP,loop});
					current_insts.push_back(Cell{Inst::LABEL,fail_pos});
				break;}
				case Ctrl::UNTIL:{
					int loop = new_label();
					last_insts.insert(last_insts.end(),current_insts.begin(),current_insts.end());
					current_insts = last_insts;
					current_insts.insert(current_insts.begin(),Cell{Inst::LABEL,fail_pos});
				break;}
			}
			last_insts = inst_stack.top();
			inst_stack.pop();
		}

		void value(string id){
			long long pos;
			try{
				pos = current_proc.table.at(id);
			}catch(out_of_range &e){
				cerr << "Zmienna " << id << " nie istnieje." << endl;
				exit(-3);
			}
			// cout<<"Var: "<<pos<<endl;
			current_values.push(Value{pos,true});
		}
		void value(long long num){
			// cout<<"Val: "<<num<<endl;
			current_values.push(Value{num,false});
		}

		void parameter(string name){
			param_num++;
			value(name);
		}

		void finish(){
			out_list = procs["main"].insts;
			out_list.push_back(Cell{Inst::HALT,1});
			int padding = procs["main"].table.size();
			int procedures = false;
			// save("beforeFinish.mr");


			//call functions
			for(unsigned int i=0;i<out_list.size();i++){
				if(out_list[i].inst == Inst::CALL){
					Proc_call call = calls[out_list[i].val];
					Procedure &proc = procs[call.proc];
					if(proc.label == -1){
						proc.label = new_label();
						for(unsigned int j=0;j<proc.insts.size();j++){
							if(proc.insts[j].val<first_var+proc.bindings && proc.insts[j].val>=first_var){
								switch(proc.insts[j].inst){
									case Inst::GET:
										proc.insts.insert(proc.insts.begin()+j+1,Cell{Inst::STORE,proc.insts[j].val});
										proc.insts[j].val = 0;
										break;
									case Inst::PUT:
										proc.insts.insert(proc.insts.begin()+j,Cell{Inst::LOADI,proc.insts[j].val});
										j++;
										proc.insts[j].val = 0;
										break;
									case Inst::LOAD:
										proc.insts[j].inst = Inst::LOADI;
										break;
									case Inst::STORE:
										proc.insts[j].inst = Inst::STOREI;
										break;
									case Inst::ADD:
										proc.insts[j].inst = Inst::ADDI;
										break;
									case Inst::SUB:
										proc.insts[j].inst = Inst::SUBI;
										break;
								}
							}
						}
						for(unsigned int j=0;j<proc.insts.size();j++){
							switch(proc.insts[j].inst){
								case Inst::GET:
								case Inst::PUT:
								case Inst::LOAD:
								case Inst::STORE:
								case Inst::LOADI:
								case Inst::STOREI:
								case Inst::ADD:
								case Inst::SUB:
								case Inst::ADDI:
								case Inst::SUBI:
								case Inst::SET:
								// case Inst::HALF:
								// case Inst::JUMP:
								// case Inst::JPOS:
								// case Inst::JZERO:
								case Inst::JUMPI:
								// case Inst::HALT:
								if(proc.insts[j].val>=first_var){
									proc.insts[j].val = proc.insts[j].val + padding;
								}
								break;
								case Inst::CALL:
									Proc_call &icall = calls[proc.insts[j].val];
									icall.from_proc = proc.name;
									for(int k = 0;k<icall.args.size();k++){
										if(icall.args[k].val>=first_var){
											icall.args[k].val = icall.args[k].val + padding;
										}
									}
							}
						}
						proc.padding = padding;
						padding += proc.table.size() + 1;
						proc.retadr = padding - 1+ first_var;
						proc.insts.insert(proc.insts.begin(),Cell{Inst::LABEL,proc.label});
						proc.insts.push_back(Cell{Inst::JUMPI,proc.retadr});
						out_list.insert(out_list.end(),proc.insts.begin(),proc.insts.end());
					}
					Inst_list call_list;
					for(unsigned int j=0;j<call.args.size();j++){
						if(!call.args[j].is_ref){
							cerr<<"Nie można przekazać stałej do funkcji."<<endl;
							// call_list.push_back(Cell{Inst::SET,call.args[j].val});
						}
						Procedure &from_proc = procs[call.from_proc];
						if(call.args[j].val>=from_proc.padding+from_proc.bindings+first_var || call.args[j].val<first_var){
							call_list.push_back(Cell{Inst::SET,call.args[j].val});
						}else{
							call_list.push_back(Cell{Inst::LOAD,call.args[j].val});
						}
						call_list.push_back(Cell{Inst::STORE,proc.padding+j+first_var});
					}

					call_list.push_back(Cell{Inst::SET,-1});
					call_list.push_back(Cell{Inst::STORE,proc.retadr});
					call_list.push_back(Cell{Inst::JUMP,proc.label});
					// for(unsigned int j=0;j<call.args.size();j++){
					// 	if(call.args[j].is_ref){
					// 		call_list.push_back(Cell{Inst::LOAD,proc.padding+j+first_var});
					// 	}
					// 	call_list.push_back(Cell{Inst::STORE,call.args[j].val});
					// }
					out_list.erase(out_list.begin()+i);
					out_list.insert(out_list.begin()+i,call_list.begin(),call_list.end());

					i--;
					
				}else if((out_list[i].inst == Inst::LOAD || out_list[i].inst == Inst::SUB || out_list[i].inst == Inst::ADD) && out_list[i].val == one_reg && one_reg_used == false){
					out_list.insert(out_list.begin(),Cell{Inst::SET,1});
					out_list.insert(out_list.begin()+1,Cell{Inst::STORE,one_reg});
					one_reg_used = true;
				}else if(out_list[i].inst==Inst::HALT){
					procedures = true;
				}
			}

			//remove labels
			vector<int> label_pos(labels_id);
			for(unsigned int i=0;i<out_list.size();i++){
				if(out_list[i].inst == Inst::LABEL){
					label_pos[out_list[i].val] = i;
					out_list.erase(out_list.begin()+i);
					i--;
				}
			}

			for(unsigned int i=0;i<out_list.size();i++){
				if(out_list[i].inst == Inst::JUMP || out_list[i].inst == Inst::JZERO || out_list[i].inst == Inst::JPOS){
					out_list[i].val = label_pos[out_list[i].val];
				}
				if(out_list[i].inst == Inst::SET && out_list[i].val == -1){
					out_list[i].val = i+3;
				}
			}

		}

		void save(string filename){
			ostream *out;
			ofstream file;
			if(filename.size() == 0){
				out = &cout;
			}else{
				file.open(filename);
				if(!file.good()){
					cerr<<"Can't open file: "<<filename<<endl;
					exit(-20);
				}
				out = &file;
			}

			for(Cell instruction : out_list){
				Inst inst = instruction.inst;
				long long arg = instruction.val;
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
					break; case Inst::CALL:
					text = "CALL";
					cerr << "Niestandardowa instrukcja" << endl;
					break; case Inst::LABEL:
					text = "LABEL";
					cerr << "Niestandardowa instrukcja" << endl;
					break; default:
					cerr << "Nieznana instrukcja" << endl;
					break;  
				}
				
				(*out)<<left<<setw(6)<<text;
				if(!(inst == Inst::HALT || inst == Inst::HALF)){
					(*out)<<" "<<arg<<endl;
				}else{
					(*out)<<endl;
				}
				
			}
		}

};
