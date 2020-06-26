#ifndef C_ANTLR_COMPILER_CHAR_H
#define C_ANTLR_COMPILER_CHAR_H
#pragma once
#import "statement.h"

class Char : public Statement {
    int value;
public:
    Char(int value);
    int getValue () const;
    virtual std::string print();
};

#endif //C_ANTLR_COMPILER_CHAR_H
