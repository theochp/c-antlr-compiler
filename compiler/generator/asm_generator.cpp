#include <sstream>

#include "asm_generator.h"

#define TAB "\t"

AsmGenerator::AsmGenerator(vector<IRFunc*> funcs, map<string, map<string, int>> symbolTables)
    : funcs(funcs), symbolTables(symbolTables) {
}

void AsmGenerator::generate(ostream& os) {
    // main setup
    os << ".global main" << endl;
    
    for (auto func : funcs) {
        for (auto block : func->getBlocks()) {
            os << generate_block(*block) << endl;
        }   
    }
}

string AsmGenerator::generate_block(IRBlock& block) {
    stringstream res;
    res << block.getLabel() << ":" << endl << TAB;
    res  << "pushq	%rbp" << endl << TAB;
    res  << "movq	%rsp, %rbp" << endl;

    if (block.getLabel() == block.getFunc()->getName()) {
        int pNum = block.getFunc()->getParams().size();
        const int nRegisters = 6; 
        string registers[nRegisters] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"}; // x86-64 ABI
        auto params = block.getFunc()->getParams();

        for (int i = pNum; i >= 1; --i) {
            res << TAB;
            if (i > nRegisters) {
                res << "popq %eax" << endl << TAB;
                res << "movl %eax, " << getOffsetRegister(block.getFunc()->getName(), params[i-1]) << endl;
            } else {
                res << "movl " << registers[i - 1] << ", " << getOffsetRegister(block.getFunc()->getName(), params[i-1]) << endl;
            }
        }    
    }

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
            case IROp::call:
                res << TAB << generate_call(inst) << endl;
                break;
        }
    }

    return res.str();
}

string AsmGenerator::generate_ldcst(Instruction& inst) {
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    return "movl $" + inst.operands()[0] + ", " + dest;
}

// Todo: refactor (same behavior twice)
string AsmGenerator::generate_store(Instruction& inst) {
    string source = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    stringstream res;
    res << "movl " + source + ", %eax" << endl;
    res << TAB << "movl %eax," + dest;
    return res.str();
}

string AsmGenerator::generate_ret(Instruction& inst) {
    stringstream res;
    if (inst.operand(0) != "") {
        string source = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
        res << "movl " + source + ", %eax" << endl << TAB;
    }

    res << "popq %rbp" << endl;
    res << TAB << "ret" << endl;

    return res.str();
}

string AsmGenerator::generate_add(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "movl " + op2 + ", %ebx" << endl << TAB;
    res << "addl %ebx, %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string AsmGenerator::generate_sub(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "movl " + op2 + ", %ebx" << endl << TAB;
    res << "subl %ebx, %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string AsmGenerator::generate_mul(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "movl " + op2 + ", %ebx" << endl << TAB;
    res << "imull %ebx, %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string AsmGenerator::generate_div(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "cltd" << endl << TAB; // convert %eax to dword
    res << "idivl " << op2 << endl << TAB; // result to eax, remainder to edx
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string AsmGenerator::generate_neg(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "negl %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

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
            res << "pushq " << getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(i)) << endl << TAB;
        } else {
            res << "movl " << getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(i)) << ", " << registers[i - 1] << endl << TAB;
        }
    }
    
    res << "call " << name << endl << TAB;
    res << "movl %eax, " << getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest()) << endl;

    return res.str();
}

string AsmGenerator::getOffsetRegister(string symbolTable, string symbolName) {
    int offset = symbolTables.at(symbolTable).at(symbolName);
    return to_string(offset) + "(%rbp)";
}