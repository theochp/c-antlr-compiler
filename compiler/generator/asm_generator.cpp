#include <sstream>

#include "asm_generator.h"

#define TAB "\t"

AsmGenerator::AsmGenerator(vector<instruction*> instructions, map<string, int> symbolTable)
    : instructions(instructions), symbolTable(symbolTable) {
}

void AsmGenerator::generate(ostream& os) {
    // main setup
    os << ".global main" << endl;
    os << "main:" << endl;
    os << TAB << "pushq	%rbp" << endl;
    os << TAB << "movq	%rsp, %rbp" << endl;

    for (auto it = instructions.begin(); it != instructions.end(); ++it) {
        instruction& inst = **it;

        switch (inst.op()) {
            case cst:
                os << TAB << generate_cst(inst) << endl;
                break;
            case load:
                os << TAB << generate_load(inst) << endl;
                break;
            case store:
                os << TAB << generate_store(inst) << endl;
                break;
            case ret:
                os << TAB << generate_ret(inst) << endl;
                break;
        }
    }
}

string AsmGenerator::generate_cst(instruction& inst) {
    string dest = getOffsetRegister(inst.dest());
    return "movl $" + inst.source() + ", " + dest;
}

string AsmGenerator::generate_load(instruction& inst) {
    string source = getOffsetRegister(inst.source());
    string dest = getOffsetRegister(inst.dest());
    stringstream res;
    res << "movl " + source + ", %eax" << endl;
    res << TAB << "movl %eax," + dest;
    return res.str();
}

// Todo: refactor (same behavior twice)
string AsmGenerator::generate_store(instruction& inst) {
    string source = getOffsetRegister(inst.source());
    string dest = getOffsetRegister(inst.dest());
    stringstream res;
    res << "movl " + source + ", %eax" << endl;
    res << TAB << "movl %eax," + dest;
    return res.str();
}

string AsmGenerator::generate_ret(instruction& inst) {
    stringstream res;
    if (inst.source() != "") {
        string source = getOffsetRegister(inst.source());
        res << "movl " + source + ", %eax" << endl << TAB;
    }

    res << "popq %rbp" << endl;
    res << TAB << "ret" << endl;

    return res.str();
}


string AsmGenerator::getOffsetRegister(string symbolName) {
    int offset = symbolTable.at(symbolName);
    return to_string(offset) + "(%rbp)";
}