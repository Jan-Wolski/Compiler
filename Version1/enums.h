#pragma once
enum Operators : int {NOP,ADD,SUB,MUL,DIV,REM};
enum Comparisons : int {NO,EQ,GT,LT,GTE,LTE,NEQ};
enum class Ctrl : int {IF,IFELSE,WHILE,UNTIL};
enum class Inst : int { GET, PUT, LOAD, STORE, LOADI, STOREI, ADD, SUB, ADDI, SUBI, SET, HALF, JUMP, JPOS, JZERO, JUMPI, HALT, CALL, LABEL };
