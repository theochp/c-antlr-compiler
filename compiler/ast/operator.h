#pragma once

typedef enum {
    mult,
    div,
    add,
    minus,
} OpType;

class Operator {
    OpType opType;
};