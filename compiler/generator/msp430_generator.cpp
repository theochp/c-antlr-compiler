#include <sstream>

#include "msp430_generator.h"

#define TAB "\t"

MSP430Generator::MSP430Generator(vector<IRFunc*> funcs, map<string, map<string, int>> symbolTables)
    : funcs(funcs), symbolTables(symbolTables) {
}

void MSP430Generator::generate(ostream& os) {
    // main setup
    os << ".global main" << endl;
    
    for (auto func : funcs) {
        int totalSymbolTableSize = 0;
        for(auto block : func->getBlocks()) {
            try {
                totalSymbolTableSize += symbolTables.at(block->getLabel()).size() * 4;
            } catch(std::out_of_range e) {

            }
        }
        os << func->getName() << ":" << endl << TAB;
        os << "PUSHM.W #1, R4" << endl << TAB;
        os << "MOV.W R1, R4" << endl << TAB;
        os << "SUB.W #" << totalSymbolTableSize << ", R1" << endl;
        for (auto block : func->getBlocks()) {
            os << generate_block(*block) << endl;
        }
        os << ".RET" << func->getName() << ":" << endl << TAB;
        os << "MOV.W " << getOffsetRegister(func->getName(), "!funcRet") << ", R12" << endl << TAB;
        os << "ADD.W #" << totalSymbolTableSize << ", R1" << endl << TAB;
        os << "POPM.W #1, r4" << endl << TAB;
        os << "RET" << endl;
    }
}

string MSP430Generator::generate_block(const IRBlock& block) {
    stringstream res;
    
    if (block.getLabel() == block.getFunc()->getName()) {
        int pNum = block.getFunc()->getParams().size();
        const int N_REGISTERS = 6; 
        string registers[N_REGISTERS] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"}; // x86-64 ABI
        auto params = block.getFunc()->getParams();

        for (int i = pNum; i >= 1; --i) {
            res << TAB;
            if (i > N_REGISTERS) {
                res << "popq R12" << endl << TAB;
                res << "MOV.W R12, " << getOffsetRegister(block.getFunc()->getName(), params[i-1]) << endl;
            } else {
                res << "MOV.W " << registers[i - 1] << ", " << getOffsetRegister(block.getFunc()->getName(), params[i-1]) << endl;
            }
        }
        res << endl;
    } else {
        res << block.getLabel() << ":" << endl << TAB;
    }
    for (auto it : block.getInstructions()) {
        Instruction& inst = *it;
        switch (inst.op()) {
            case IROp::ldcst:
                res << TAB << generate_ldcst(inst) << endl;
                break;
            case IROp::store:
                res << TAB << generate_store(inst) << endl;
                break;
            case IROp::ret:
                // TODO: handle return inside other blocks
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
            case IROp::loadT:
                res << TAB << generate_loadT(inst) << endl;
                break;
            case IROp::storeT:
                res << TAB << generate_storeT(inst) << endl;
                break;
            case IROp::call:
                res << TAB << generate_call(inst) << endl;
                break;
            case IROp::equalcomp:
                res << TAB << generate_equal_comp(inst) << endl;
                break;
            case IROp::diffcomp:
                res << TAB << generate_diff_comp(inst) << endl;
                break;
            case IROp::infcomp:
                res << TAB << generate_inf_comp(inst) << endl;
                break;
            case IROp::infeqcomp:
                res << TAB << generate_inf_eq_comp(inst) << endl;
                break;
            case IROp::supcomp:
                res << TAB << generate_sup_comp(inst) << endl;
                break;
            case IROp::supeqcomp:
                res << TAB << generate_sup_eq_comp(inst) << endl;
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
            case IROp::bitwise_not:
                res << TAB << generate_bitwise_not(inst) << endl;
                break;
            case IROp::logicalNot:
                res << TAB << generate_not(inst) << endl;
                break;
            case IROp::je:
                res << TAB << generate_je(inst) << endl;
                break;
            case IROp::jmp:
                res << TAB << generate_jmp(inst) << endl;
                break;
            case IROp::preincre:
                res << TAB << generate_preincre(inst) << endl;
                break;
            case IROp::postincre:
                res << TAB << generate_postincre(inst) << endl;
                break;
            case IROp::predecre:
                res << TAB << generate_predecre(inst) << endl;
                break;
            case IROp::postdecre:
                res << TAB << generate_postdecre(inst) << endl;
                break;
        }
    }

    return res.str();
}

string MSP430Generator::generate_ldcst(Instruction& inst) {
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    return "MOV.W #" + inst.operands()[0] + ", " + dest;
}

// Todo: refactor (same behavior twice)
string MSP430Generator::generate_store(Instruction& inst) {
    string source = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    stringstream res;
    res << "MOV.W " + source + ", " << dest;
    return res.str();
}

string MSP430Generator::generate_ret(Instruction& inst) {
    stringstream res;
    if (inst.operand(0) != "") {
        string source = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    }

    return res.str();
}

string MSP430Generator::generate_add(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "MOV.W " + op1 + ", R12" << endl << TAB;
    res << "ADD.W " << op2 << ", R12" << endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

string MSP430Generator::generate_sub(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "MOV.W " + op1 + ", R12" << endl << TAB;
    res << "SUB.W " << op2 << ", R12" << endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

string MSP430Generator::generate_mul(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "MOV.W " + op1 + ", R12" << endl << TAB;
    res << "MOV.W " + op2 + ", R13" << endl << TAB;
    res << "CALL #__mspabi_mpyi" << endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

string MSP430Generator::generate_div(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "MOV.W " + op1 + ", R12" << endl << TAB;
    res << "MOV.W " + op2 + ", R13" << endl << TAB;
    res << "CALL #__mspabi_divi" << endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

string MSP430Generator::generate_neg(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "MOV.B #0, R12" << endl << TAB;
    res << "SUB.W " + op1 + ", R12" << endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

// TODO
string MSP430Generator::generate_loadT(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffset(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    
    res << "MOV.W " + op2 + ", R12" << endl << TAB;
    res << "cltq" << endl << TAB;
    res << "MOV.W " + op1 + "(%rbp,%rax,4), R12"<< endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

// TODO
string MSP430Generator::generate_call(Instruction& inst) {
    stringstream res;
    string name = inst.operand(0);
    int pNum = inst.operands().size() - 1;
    const int N_REGISTERS = 6; 
    string registers[N_REGISTERS] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"}; // x86-64 ABI
    for (int i = pNum; i >= 1; --i) {
        if (i > N_REGISTERS) {
            res << "pushq " << getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(i)) << endl << TAB;
        } else {
            res << "MOV.W " << getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(i)) << ", " << registers[i - 1] << endl << TAB;
        }
    }
    
    res << "call " << name << endl << TAB;
    res << "MOV.W R12, " << getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest()) << endl;
    
    return res.str();
}

// TODO
string MSP430Generator::generate_equal_comp(Instruction &inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "MOV.W " + op1 + ", R12" << endl << TAB;
    res << "cmpl " + op2 + ", R12" << endl << TAB;
    res << "sete %al" << endl << TAB;
    res << "movzbl %al, R12" << endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

// TODO
string MSP430Generator::generate_diff_comp(Instruction &inst) {

    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "MOV.W " + op1 + ", R12" << endl << TAB;
    res << "cmpl " + op2 + ", R12" << endl << TAB;
    res << "setne %al" << endl << TAB;
    res << "movzbl %al, R12" << endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

// TODO
string MSP430Generator::generate_inf_comp(Instruction &inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "MOV.W " + op1 + ", R12" << endl << TAB;
    res << "cmpl " + op2 + ", R12" << endl << TAB;
    res << "setl %al" << endl << TAB;
    res << "movzbl %al, R12" << endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

// TODO
string MSP430Generator::generate_storeT(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffset(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    
    res << "MOV.W " + op2 + ", R12" << endl << TAB;
    res << "cltq" << endl << TAB;
    res << "MOV.W " + dest + ", %edx" << endl << TAB;
    res << "MOV.W %edx, " + op1 + "(%rbp,%rax,4)"<< endl << TAB;

    return res.str();
}

// TODO
string MSP430Generator::generate_inf_eq_comp(Instruction &inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "MOV.W " + op1 + ", R12" << endl << TAB;
    res << "cmpl " + op2 + ", R12" << endl << TAB;
    res << "setle %al" << endl << TAB;
    res << "movzbl %al, R12" << endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

// TODO
string MSP430Generator::generate_sup_comp(Instruction &inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "MOV.W " + op1 + ", R12" << endl << TAB;
    res << "cmpl " + op2 + ", R12" << endl << TAB;
    res << "setg %al" << endl << TAB;
    res << "movzbl %al, R12" << endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

// TODO
string MSP430Generator::generate_sup_eq_comp(Instruction &inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "MOV.W " + op1 + ", R12" << endl << TAB;
    res << "cmpl " + op2 + ", R12" << endl << TAB;
    res << "setge %al" << endl << TAB;
    res << "movzbl %al, R12" << endl << TAB;
    res << "MOV.W R12, " << dest << endl;
    
    return res.str();
}

// TODO
string MSP430Generator::generate_bitwise_and(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "MOV.W " + op1 + ", R12" << endl << TAB;
    res << "andl " + op2 + ", R12" << endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

// TODO
string MSP430Generator::generate_bitwise_or(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "MOV.W " + op1 + ", R12" << endl << TAB;
    res << "orl " + op2 + ", R12" << endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

// TODO
string MSP430Generator::generate_bitwise_xor(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "MOV.W " + op1 + ", R12" << endl << TAB;
    res << "xorl " + op2 + ", R12" << endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

// TODO
string MSP430Generator::generate_bitwise_not(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "MOV.W " + op1 + ", R12" << endl << TAB;
    res << "notl R12" << endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

// TODO
string MSP430Generator::generate_not(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "cmpl $0," + op1 << endl << TAB;
    res << "sete %al" << endl << TAB;
    res << "movzbl %al, R12" << endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

// TODO
string MSP430Generator::generate_je(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    res << "cmpl $" << inst.operand(1) << ", " << op1 << endl << TAB;
    res << "je " << inst.dest() << endl;

    return res.str();
}

string MSP430Generator::generate_jmp(Instruction& inst) {
    stringstream res;

    res << "BR #" << inst.dest() << endl;

    return res.str();
}

// TODO
string MSP430Generator::generate_preincre(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());

    res << "addl $1," + op1 << endl << TAB;
    res << "MOV.W " + op1 + ", R12"<< endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

// TODO
string MSP430Generator::generate_predecre(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());

    res << "subl $1," + op1 << endl << TAB;
    res << "MOV.W " + op1 + ", R12"<< endl << TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

// TODO
string MSP430Generator::generate_postincre(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
   
    res << "MOV.W " + op1 + ", R12" << endl << TAB;
    res << "leal 1(%rax), %edx" << endl << TAB;
    res << "MOV.W %edx, " + op1 << endl<< TAB;
    res << "MOV.W R12, " << dest << endl;
    
    return res.str();
}

// TODO
string MSP430Generator::generate_postdecre(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());

    res << "MOV.W " + op1 + ", R12" << endl << TAB;
    res << "leal -1(%rax), %edx" << endl << TAB;
    res << "MOV.W %edx, " + op1 << endl<< TAB;
    res << "MOV.W R12, " << dest << endl;

    return res.str();
}

string MSP430Generator::getOffsetRegister(string symbolTable, string symbolName) {
    int offset = symbolTables.at(symbolTable).at(symbolName);
    return to_string(offset) + "(R4)";
}

string MSP430Generator::getOffset(string symbolTable, string symbolName) {
    int offset = symbolTables.at(symbolTable).at(symbolName);
    return to_string(offset);
}