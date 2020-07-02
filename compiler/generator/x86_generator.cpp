#include <sstream>

#include "x86_generator.h"

#define TAB "\t"

X86Generator::X86Generator(vector<IRFunc*> funcs, map<string, map<string, int>> symbolTables)
    : funcs(funcs), symbolTables(symbolTables) {
}

void X86Generator::generate(ostream& os) {
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
        os << "pushq	%rbp" << endl << TAB;
        os << "movq	%rsp, %rbp" << endl << TAB;
        os << "subq $" << totalSymbolTableSize << ", %rsp" << endl;
        for (auto block : func->getBlocks()) {
            os << generate_block(*block) << endl;
        }
        os << ".RET" << func->getName() << ":" << endl << TAB;
        os << "movl " << getOffsetRegister(func->getName(), "!funcRet") << ", %eax" << endl << TAB;
        os << "addq $" << totalSymbolTableSize << ", %rsp" << endl << TAB;
        os << "popq %rbp" << endl << TAB;
        os << "ret" << endl;
    }
}

string X86Generator::generate_block(const IRBlock& block) {
    stringstream res;
    
    if (block.getLabel() == block.getFunc()->getName()) {
        int pNum = block.getFunc()->getParams().size();
        const int N_REGISTERS = 6; 
        string registers[N_REGISTERS] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"}; // x86-64 ABI
        auto params = block.getFunc()->getParams();

        for (int i = pNum; i >= 1; --i) {
            res << TAB;
            if (i > N_REGISTERS) {
                res << "popq %eax" << endl << TAB;
                res << "movl %eax, " << getOffsetRegister(block.getFunc()->getName(), params[i-1]) << endl;
            } else {
                res << "movl " << registers[i - 1] << ", " << getOffsetRegister(block.getFunc()->getName(), params[i-1]) << endl;
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

string X86Generator::generate_ldcst(Instruction& inst) {
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    return "movl $" + inst.operands()[0] + ", " + dest;
}

// Todo: refactor (same behavior twice)
string X86Generator::generate_store(Instruction& inst) {
    string source = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    stringstream res;
    res << "movl " + source + ", %eax" << endl;
    res << TAB << "movl %eax," + dest;
    return res.str();
}

string X86Generator::generate_ret(Instruction& inst) {
    stringstream res;
    if (inst.operand(0) != "") {
        string source = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
        inst.getBlock()->getFunc()->setReturnValueLoc(source);
    }

    return res.str();
}

string X86Generator::generate_add(Instruction& inst) {
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

string X86Generator::generate_sub(Instruction& inst) {
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

string X86Generator::generate_mul(Instruction& inst) {
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

string X86Generator::generate_div(Instruction& inst) {
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

string X86Generator::generate_neg(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "negl %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string X86Generator::generate_loadT(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffset(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    
    res << "movl " + op2 + ", %eax" << endl << TAB;
    res << "cltq" << endl << TAB;
    res << "movl " + op1 + "(%rbp,%rax,4), %eax"<< endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string X86Generator::generate_call(Instruction& inst) {
    stringstream res;
    string name = inst.operand(0);
    int pNum = inst.operands().size() - 1;
    const int N_REGISTERS = 6; 
    string registers[N_REGISTERS] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"}; // x86-64 ABI
    for (int i = pNum; i >= 1; --i) {
        if (i > N_REGISTERS) {
            res << "pushq " << getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(i)) << endl << TAB;
        } else {
            res << "movl " << getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(i)) << ", " << registers[i - 1] << endl << TAB;
        }
    }
    
    res << "call " << name << endl << TAB;
    res << "movl %eax, " << getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest()) << endl;
    
    return res.str();
}

string X86Generator::generate_equal_comp(Instruction &inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "cmpl " + op2 + ", %eax" << endl << TAB;
    res << "sete %al" << endl << TAB;
    res << "movzbl %al, %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string X86Generator::generate_diff_comp(Instruction &inst) {

    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "cmpl " + op2 + ", %eax" << endl << TAB;
    res << "setne %al" << endl << TAB;
    res << "movzbl %al, %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string X86Generator::generate_inf_comp(Instruction &inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "cmpl " + op2 + ", %eax" << endl << TAB;
    res << "setl %al" << endl << TAB;
    res << "movzbl %al, %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string X86Generator::generate_storeT(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffset(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    
    res << "movl " + op2 + ", %eax" << endl << TAB;
    res << "cltq" << endl << TAB;
    res << "movl " + dest + ", %edx" << endl << TAB;
    res << "movl %edx, " + op1 + "(%rbp,%rax,4)"<< endl << TAB;

    return res.str();
}

string X86Generator::generate_inf_eq_comp(Instruction &inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "cmpl " + op2 + ", %eax" << endl << TAB;
    res << "setle %al" << endl << TAB;
    res << "movzbl %al, %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string X86Generator::generate_sup_comp(Instruction &inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "cmpl " + op2 + ", %eax" << endl << TAB;
    res << "setg %al" << endl << TAB;
    res << "movzbl %al, %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}
  
string X86Generator::generate_sup_eq_comp(Instruction &inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "cmpl " + op2 + ", %eax" << endl << TAB;
    res << "setge %al" << endl << TAB;
    res << "movzbl %al, %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;
    
    return res.str();
}

string X86Generator::generate_bitwise_and(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "andl " + op2 + ", %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string X86Generator::generate_bitwise_or(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "orl " + op2 + ", %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string X86Generator::generate_bitwise_xor(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string op2 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(1));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "xorl " + op2 + ", %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string X86Generator::generate_bitwise_not(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "notl %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string X86Generator::generate_not(Instruction& inst) {
    stringstream res;
    
    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
    res << "cmpl $0," + op1 << endl << TAB;
    res << "sete %al" << endl << TAB;
    res << "movzbl %al, %eax" << endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string X86Generator::generate_je(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    res << "cmpl $" << inst.operand(1) << ", " << op1 << endl << TAB;
    res << "je " << inst.dest() << endl;

    return res.str();
}

string X86Generator::generate_jmp(Instruction& inst) {
    stringstream res;

    res << "jmp " << inst.dest() << endl;

    return res.str();
}

string X86Generator::generate_preincre(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());

    res << "addl $1," + op1 << endl << TAB;
    res << "movl " + op1 + ", %eax"<< endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string X86Generator::generate_predecre(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());

    res << "subl $1," + op1 << endl << TAB;
    res << "movl " + op1 + ", %eax"<< endl << TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string X86Generator::generate_postincre(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());
   
    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "leal 1(%rax), %edx" << endl << TAB;
    res << "movl %edx, " + op1 << endl<< TAB;
    res << "movl %eax, " << dest << endl;
    
    return res.str();
}

string X86Generator::generate_postdecre(Instruction& inst) {
    stringstream res;

    string op1 = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.operand(0));
    string dest = getOffsetRegister(inst.getBlock()->getFunc()->getName(), inst.dest());

    res << "movl " + op1 + ", %eax" << endl << TAB;
    res << "leal -1(%rax), %edx" << endl << TAB;
    res << "movl %edx, " + op1 << endl<< TAB;
    res << "movl %eax, " << dest << endl;

    return res.str();
}

string X86Generator::getOffsetRegister(string symbolTable, string symbolName) {
    int offset = symbolTables.at(symbolTable).at(symbolName);
    return to_string(offset) + "(%rbp)";
}

string X86Generator::getOffset(string symbolTable, string symbolName) {
    int offset = symbolTables.at(symbolTable).at(symbolName);
    return to_string(offset);
}