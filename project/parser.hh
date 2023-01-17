/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_HH_INCLUDED
# define YY_YY_PARSER_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "parser.y"
 
  #include <stdio.h>
  #include "program.h"
  #include "enums.h"

#line 55 "parser.hh"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    PROCEDURE = 258,               /* PROCEDURE  */
    IS = 259,                      /* IS  */
    VAR = 260,                     /* VAR  */
    t_BEGIN = 261,                 /* t_BEGIN  */
    END = 262,                     /* END  */
    PROGRAM = 263,                 /* PROGRAM  */
    IF = 264,                      /* IF  */
    THEN = 265,                    /* THEN  */
    ELSE = 266,                    /* ELSE  */
    ENDIF = 267,                   /* ENDIF  */
    WHILE = 268,                   /* WHILE  */
    DO = 269,                      /* DO  */
    ENDWHILE = 270,                /* ENDWHILE  */
    REPEAT = 271,                  /* REPEAT  */
    UNTIL = 272,                   /* UNTIL  */
    READ = 273,                    /* READ  */
    WRITE = 274,                   /* WRITE  */
    ASSIGN = 275,                  /* ASSIGN  */
    OP = 276,                      /* OP  */
    COMP = 277,                    /* COMP  */
    NUM = 278,                     /* NUM  */
    ID = 279,                      /* ID  */
    ERROR = 280                    /* ERROR  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 24 "parser.y"
 
  long long number; 
  Operators op; 
  Comparisons comp; 
  char* text; 

#line 104 "parser.hh"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (Program& program);

/* "%code provides" blocks.  */
#line 6 "parser.y"
 
  void run_parser( Program & program, FILE * data ); 

#line 123 "parser.hh"

#endif /* !YY_YY_PARSER_HH_INCLUDED  */
