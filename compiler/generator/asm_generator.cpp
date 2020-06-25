#include <sstream>

#include "asm_generator.h"

#define TAB "\t"

AsmGenerator::AsmGenerator(vector<IRBlock*> blocks, map<string, int> symbolTable)
    : blocks(blocks), symbolTable(symbolTable) {
}

void AsmGenerator::generate(ostream& os) {
    // main setup
    os << ".global main" << endl;
    
    for (auto it = blocks.begin(); it != blocks.end(); ++it) {
        os << generate_block(**it) << endl;
    }
}

string AsmGenerator::generate_block(IRBlock& block) {
    stringstream res;
    res << block.getLabel() << ":" << endl << TAB;
    res  << "pushq	%rbp" << endl << TAB;
    res  << "movq	%rsp, %rbp" << endl;

    for (auto it = block.getInstructions().begin(); it != block.getInstructions().end(); ++it) {
        Instruction& inst = **it;

        switch (inst.op()) {
            case IROp::ldcst:
                res << TAB << generate_ldcst(inst) << endl;
                break;
            case IROp::store:
                res << TAB << generate_store(inst) << endl;
                break;
            case IROp::ret:
                res << TAB << generate_ret(inst) << endl;
                break;
            case IROp::add:
                res << TAB << generate_add(inst) << endl;
                break;
            case IROp::sub:
                res << TAB << generate_sub(inst) << endl;
                break;
            case IROp::mul:
                res << TAB << generate_mul(inst) << endl;
                break;
            case IROp::div:
                res << TAB << generate_div(inst) << endl;
                break;
            case IROp::neg:
                res << TAB << generate_neg(inst) << endl;
                break;
            case IROp::bitwise_and:
                res << TAB << generate_bitwise_and(inst) << endl;
                break;
            case IROp::bitwise_or:
                res << TAB << generate_bitwise_or(inst) << endl;
                break;
            case IROp::bitwise_xor:
                res << TAB << generate_bitwise_xor(inst) << endl;
                break;
            case IROp::logicalNot:
                res << TAB << generate_not(inst) << endl;
                break;
            case IROp::call:
                res << TAB << generate_call(inst) << endl;
                break;
        }
    }

    return res.str();
}

string AsmGenerator::generate_ldcst(Instruction& inst) {
    string dest = getOffsetRegister(inst.dest());
    return "movl $" + inst.operands()[0] + ", " + dest;
}

// Todo: refactor (same behavior twice)
string AsmGenerator::generate_store(Instruction& inst) {
    string source = getOffsetRegister(inst.operand(0));
    string dest = getOffsetRegister(inst.dest());
    stringstream res;
    res << "movl " + source + ", %eax" << endl;
    res << TAB << "movl %eax," + dest;
    return res.str();
}

string AsmGenerator::generate_ret(Instruction& inst) {
    stringstream res;
    if (inst.operand(0) != "") {
        string source = getOffsetRegister(inst.operand(0));
        res << "movl " + source + ", %eax" << endl << TAB;
    }

    res << "popq %rbp" << endl;
    res << TAB << "ret" << endl;

    return res.str();
}

string AsmGenerator::generate_add(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.operand(0));
    string op2 = getOffsetRegister(inst.operand(1));
    string dest = getOffsetRegister(inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "movl " + op2 + ", %ebx" << endl << TAB;
    res << "addl %ebx, %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string AsmGenerator::generate_sub(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.operand(0));
    string op2 = getOffsetRegister(inst.operand(1));
    string dest = getOffsetRegister(inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "movl " + op2 + ", %ebx" << endl << TAB;
    res << "subl %ebx, %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string AsmGenerator::generate_mul(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.operand(0));
    string op2 = getOffsetRegister(inst.operand(1));
    string dest = getOffsetRegister(inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "movl " + op2 + ", %ebx" << endl << TAB;
    res << "imull %ebx, %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string AsmGenerator::generate_div(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.operand(0));
    string op2 = getOffsetRegister(inst.operand(1));
    string dest = getOffsetRegister(inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "cltd" << endl << TAB; // convert %eax to dword
    res << "idivl " << op2 << endl << TAB; // result to eax, remainder to edx
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string AsmGenerator::generate_neg(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.operand(0));
    string dest = getOffsetRegister(inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "negl %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string AsmGenerator::generate_bitwise_and(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.operand(0));
    string op2 = getOffsetRegister(inst.operand(1));
    string dest = getOffsetRegister(inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "andl " + op2 + ", %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string AsmGenerator::generate_bitwise_or(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.operand(0));
    string op2 = getOffsetRegister(inst.operand(1));
    string dest = getOffsetRegister(inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "orl " + op2 + ", %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string AsmGenerator::generate_bitwise_xor(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.operand(0));
    string op2 = getOffsetRegister(inst.operand(1));
    string dest = getOffsetRegister(inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "xorl " + op2 + ", %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string AsmGenerator::generate_not(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.operand(0));
    string dest = getOffsetRegister(inst.dest());
    res << "cmpl $0," + op1 << endl << TAB;
    res << "setne %al" << endl << TAB;
    res << "xorb $-1, %al" << endl << TAB;
    res << "andb $1, %al" << endl << TAB;
    res << "movzbl %al, %ecx" << endl << TAB;
    res << "movl %ecx, " << dest << endl;

    return res.str();
}

string AsmGenerator::generate_call(Instruction& inst) {
    stringstream res;
    string name = inst.operand(0);
    int pNum = inst.operands().size() - 1;
    const int nRegisters = 6; 
    string registers[nRegisters] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"}; // x86-64 ABI
    for (int i = pNum; i >= 1; --i) {
        if (i > nRegisters) {
            res << "pushq " << getOffsetRegister(inst.operand(i)) << endl << TAB;
        } else {
            res << "movl " << getOffsetRegister(inst.operand(i)) << ", " << registers[i - 1] << endl << TAB;
        }
    }
    
    res << "call " << name << endl << TAB;
    res << "movl %eax, " << getOffsetRegister(inst.dest()) << endl;

    return res.str();
}

string AsmGenerator::getOffsetRegister(string symbolName) {
    int offset = symbolTable.at(symbolName);
    return to_string(offset) + "(%rbp)";
}

