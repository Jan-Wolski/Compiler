#pragma once
enum Operators : int {ADD,SUB,MUL,DIV,REM};
enum Comparisons : int {EQ,GT,LT,GTE,LTE,NEQ};
enum class Inst : int { GET, PUT, LOAD, STORE, LOADI, STOREI, ADD, SUB, ADDI, SUBI, SET, HALF, JUMP, JPOS, JZERO, JUMPI, HALT };
