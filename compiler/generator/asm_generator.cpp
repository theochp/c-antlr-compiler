#include <sstream>

#include "asm_generator.h"

#define TAB "\t"

AsmGenerator::AsmGenerator(vector<Instruction*> instructions, map<string, int> symbolTable)
    : instructions(instructions), symbolTable(symbolTable) {
}

void AsmGenerator::generate(ostream& os) {
    // main setup
    os << ".global main" << endl;
    os << "main:" << endl;
    os << TAB << "pushq	%rbp" << endl;
    os << TAB << "movq	%rsp, %rbp" << endl;

    for (auto it = instructions.begin(); it != instructions.end(); ++it) {
        Instruction& inst = **it;

        switch (inst.op()) {
            case inst_type::cst:
                os << TAB << generate_cst(inst) << endl;
                break;
            case inst_type::load:
                os << TAB << generate_load(inst) << endl;
                break;
            case inst_type::store:
                os << TAB << generate_store(inst) << endl;
                break;
            case inst_type::ret:
                os << TAB << generate_ret(inst) << endl;
                break;
            case inst_type::add:
                os << TAB << generate_add(inst) << endl;
                break;
            case inst_type::sub:
                os << TAB << generate_sub(inst) << endl;
                break;
            case inst_type::mul:
                os << TAB << generate_mul(inst) << endl;
                break;
            case inst_type::div:
                os << TAB << generate_div(inst) << endl;
            break;
            case inst_type::neg:
                os << TAB << generate_neg(inst) << endl;
                break;
        }
    }
}

string AsmGenerator::generate_cst(Instruction& inst) {
    string dest = getOffsetRegister(inst.dest());
    return "movl $" + inst.source() + ", " + dest;
}

string AsmGenerator::generate_load(Instruction& inst) {
    string source = getOffsetRegister(inst.source());
    string dest = getOffsetRegister(inst.dest());
    stringstream res;
    res << "movl " + source + ", %eax" << endl;
    res << TAB << "movl %eax," + dest;
    return res.str();
}

// Todo: refactor (same behavior twice)
string AsmGenerator::generate_store(Instruction& inst) {
    string source = getOffsetRegister(inst.source());
    string dest = getOffsetRegister(inst.dest());
    stringstream res;
    res << "movl " + source + ", %eax" << endl;
    res << TAB << "movl %eax," + dest;
    return res.str();
}

string AsmGenerator::generate_ret(Instruction& inst) {
    stringstream res;
    if (inst.source() != "") {
        string source = getOffsetRegister(inst.source());
        res << "movl " + source + ", %eax" << endl << TAB;
    }

    res << "popq %rbp" << endl;
    res << TAB << "ret" << endl;

    return res.str();
}

string AsmGenerator::generate_add(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.source());
    string op2 = getOffsetRegister(inst.operand());
    string dest = getOffsetRegister(inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "movl " + op2 + ", %ebx" << endl << TAB;
    res << "addl %ebx, %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string AsmGenerator::generate_sub(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.source());
    string op2 = getOffsetRegister(inst.operand());
    string dest = getOffsetRegister(inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "movl " + op2 + ", %ebx" << endl << TAB;
    res << "subl %ebx, %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string AsmGenerator::generate_mul(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.source());
    string op2 = getOffsetRegister(inst.operand());
    string dest = getOffsetRegister(inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "movl " + op2 + ", %ebx" << endl << TAB;
    res << "imull %ebx, %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string AsmGenerator::generate_div(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.source());
    string op2 = getOffsetRegister(inst.operand());
    string dest = getOffsetRegister(inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "cltd" << endl << TAB; // convert %eax to dword
    res << "idivl " << op2 << endl << TAB; // result to eax, remainder to edx
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string AsmGenerator::generate_neg(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.source());
    string dest = getOffsetRegister(inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "negl %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string AsmGenerator::getOffsetRegister(string symbolName) {
    int offset = symbolTable.at(symbolName);
    return to_string(offset) + "(%rbp)";
}