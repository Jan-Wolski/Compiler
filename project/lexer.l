%option noyywrap
%option yylineno
%{
  #include <string.h>
  #include "parser.hh"
  #include "enums.h"
%}
%%
PROCEDURE {return PROCEDURE;}
IS {return IS;}
VAR {return VAR;}
END {return END;}
PROGRAM {return PROGRAM;}
IF {return IF;}
THEN {return THEN;}
ELSE {return ELSE;}
ENDIF {return ENDIF;}
WHILE {return WHILE;}
DO {return DO;}
ENDWHILE {return ENDWHILE;}
REPEAT {return REPEAT;}
UNTIL {return UNTIL;}
READ {return READ;}
WRITE {return WRITE;}
ASSIGN {return ASSIGN;}
BEGIN {return t_BEGIN;}
":=" {return ASSIGN; }
"("   {return '('; }
")"   {return ')'; }
","   {return ','; }
";"   {return ';'; }

"+"  { return '+'; }
"-"  { return '-'; }
"*"  { return '*'; }
"/"  { return '/'; }
"%"  { return '%'; }

"=" { yylval.comp = EQ;    return COMP; }
"!=" { yylval.comp = NEQ;    return COMP; }
">" { yylval.comp = GT;    return COMP; }
"<" { yylval.comp = LT;    return COMP; }
">=" { yylval.comp = GTE;    return COMP; }
"<=" { yylval.comp = LTE;    return COMP; }
[0-9]+ {yylval.number = atoll(yytext); return NUM; }
[_a-z]+ {yylval.text = strdup(yytext); return ID; }


\[[^\[]*\] ;
[[:blank:]]+
\n ;
. {return ERROR;}
