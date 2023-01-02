%code requires { 
  #include <stdio.h>
  #include "Program.h"
  #include "enums.h"
}
%code provides { 
  void run_parser( Program & program, FILE * data ); 
}
%{
  #include <iostream>
  #include <stdio.h>
  #include "Program.h"
  #include "enums.h"

  int yylex();
  void yyset_in( FILE * in_str );
  void yyerror(Program & program, char const *errname );
  
  extern int yylineno;
  #ifdef DEBUG
    extern int yydebug;
  #endif
%}
%union { 
  long long number; 
  Operators op; 
  Comparisons comp; 
  char* text; 
}
%parse-param {Program& program }
%left '-' '+'
%left '*' '/' '%'
%token PROCEDURE
%token IS
%token VAR
%token t_BEGIN
%token END
%token PROGRAM
%token IF
%token THEN
%token ELSE
%token ENDIF
%token WHILE
%token DO
%token ENDWHILE
%token REPEAT
%token UNTIL
%token READ
%token WRITE
%token ASSIGN
%token <op> OP
%token <comp> COMP
%token <number> NUM
%token <text> ID
%token ERROR
%%
program_all : procedures main

procedures : procedures PROCEDURE proc_head IS VAR declarations t_BEGIN commands END  {program.def_procedure();}
| procedures PROCEDURE proc_head IS t_BEGIN commands END                              {program.def_procedure();}
| %empty

main : PROGRAM IS VAR declarations t_BEGIN commands END {program.def_main();}
| PROGRAM IS t_BEGIN commands END                       {program.def_main();}

commands : commands command
| command                            {program.new_commands();}

command : ID ASSIGN expression ';'                        {program.set_val($1);}
| IF condition THEN commands ELSE commands ENDIF  {program.if_else(true);}
| IF condition THEN commands ENDIF                {program.if_else(false);}
| WHILE condition DO commands ENDWHILE            {program.loop_while();}
| REPEAT commands UNTIL condition ';'                 {program.loop_until();}
| load_proc_head ';'                                       {program.load_procedure();}
| READ ID ';'                                         {program.read($2);free($2);}
| WRITE value ';'                                     {program.write();}

proc_head : ID '(' parameters ')'     {program.name_procedure($1);free($1);}
load_proc_head : ID '(' load_parameters ')'     {program.call_procedure($1);free($1);}

declarations : declarations ',' ID  {program.var($3);free($3);}
| ID                              {program.var($1);free($1);}

parameters : parameters ',' ID      {program.reference($3);free($3);}
| ID                              {program.reference($1);free($1);}

load_parameters : load_parameters ',' ID      {program.parameter($3);free($3);}
| ID                              {program.parameter($1);free($1);}

expression : value
| value '+' value {program.op(ADD);}
| value '-' value {program.op(SUB);}
| value '*' value {program.op(MUL);}
| value '/' value {program.op(DIV);}
| value '%' value {program.op(REM);}

condition : value COMP value {program.comp($2);}

value : NUM		{program.value($1);}
| ID 				{program.value($1);free($1);}

%%

void yyerror( Program & program, char const* errname)
{
  std::cerr << std::endl << errname << " at line " << yylineno << std::endl;
  exit(-1);
}

void run_parser( Program & program, FILE * data ) 
{
  #ifdef DEBUG
    yydebug = 1;
  #endif
  yyset_in( data );
  yyparse( program );
}
