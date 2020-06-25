//
// Created by tania on 25/06/2020.
//

#ifndef C_ANTLR_COMPILER_CHAR_H
#define C_ANTLR_COMPILER_CHAR_H


class Char : public Statement {
    char value;
public:
    Char(char value);
    char getValue const;
    virtual std::string print();
};

#endif //C_ANTLR_COMPILER_CHAR_H
