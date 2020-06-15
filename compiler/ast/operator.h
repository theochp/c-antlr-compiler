#pragma once

typedef enum {
    mult,
    div,
    add,
    minus,
} OpType;

class Operator {
    OpType opType;
public:
    Operator(OpType o);
    OpType getType();
};