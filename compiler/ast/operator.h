#pragma once

typedef enum {
    MULT,
    DIV,
    ADD,
    MINUS,
} OpType;

class Operator {
    OpType opType;

public:
    Operator (const OpType& opType);
    Operator& operator=(const OpType& opType);
    bool operator ==(const OpType &opType) const;
};