#pragma once
#include <string>

typedef enum {
    MULT,
    DIV,
    ADD,
    MINUS,
    ASSIGN,

} OpType;

class Operator {
    OpType opType;

public:
    Operator (const OpType& opType);
    const OpType& type() const;
    Operator& operator=(const OpType& opType);
    bool operator ==(const OpType &opType) const;
    virtual std::string print();
};