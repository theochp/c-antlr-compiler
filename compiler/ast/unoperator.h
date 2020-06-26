#pragma once
#include <string>

typedef enum {
    UN_PLUS,
    UN_MINUS,
    BITWISE_NOT
} UnOpType;

class UnOperator {
    UnOpType opType;

public:
    UnOperator (const UnOpType& opType);
    const UnOpType& type() const;
    UnOperator& operator=(const UnOpType& opType);
    bool operator ==(const UnOpType &opType) const;
    virtual std::string print();
};