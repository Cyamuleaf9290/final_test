#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include "Operand.h"
#include "AsmBuilder.h"
#include <vector>
#include <map>
#include <sstream>

class BasicBlock;

class Instruction
{
public:
    Instruction(unsigned instType, BasicBlock *insert_bb = nullptr);
    virtual ~Instruction();
    BasicBlock *getParent();
    bool isUncond() const { return instType == UNCOND; };
    bool isCond() const { return instType == COND; };
    bool isAlloc() const { return instType == ALLOCA; };
    bool isRet() const { return instType == RET; };
    void setParent(BasicBlock *);
    void setNext(Instruction *);
    void setPrev(Instruction *);
    Instruction *getNext();
    Instruction *getPrev();
    virtual void output() const = 0;
    MachineOperand *genMachineOperand(Operand *); // new
    MachineOperand *genMachineReg(int reg);
    MachineOperand *genMachineVReg();
    MachineOperand *genMachineImm(int val);
    MachineOperand *genMachineLabel(int block_no);
    virtual void genMachineCode(AsmBuilder *) = 0;
    void display() const
    {
        std::string type = "";
        switch (instType)
        {
        case BINARY:
            type = "Binary";
            break;
        case COND:
            type = "Cond";
            break;
        case UNCOND:
            type = "UnCond";
            break;
        case RET:
            type = "Ret";
            break;
        case LOAD:
            type = "Load";
            break;
        case STORE:
            type = "Store";
            break;
        case CMP:
            type = "Cmp";
            break;
        case ALLOCA:
            type = "Alloca";
            break;
        case CALL:
            type = "Call";
            break;
        case NOT:
            type = "Not";
            break;
        case ZEXT:
            type = "Zext";
            break;
        case GEP:
            type = "Gep";
            break;
        default:
            break;
        }
        fprintf(stderr, "【INST TYPE】: %s\n", type.c_str());
    }

protected:
    unsigned instType;
    unsigned opcode;
    Instruction *prev;
    Instruction *next;
    BasicBlock *parent;
    std::vector<Operand *> operands;
    enum
    {
        BINARY,
        COND,
        UNCOND,
        RET,
        LOAD,
        STORE,
        CMP,
        ALLOCA,
        CALL,
        NOT,
        ZEXT,
        GEP
    };
};

// meaningless instruction, used as the head node of the instruction list.
class DummyInstruction : public Instruction
{
public:
    DummyInstruction() : Instruction(-1, nullptr){};
    void output() const {};
    void genMachineCode(AsmBuilder *){};
};

class AllocaInstruction : public Instruction
{
public:
    AllocaInstruction(Operand *dst, SymbolEntry *se, BasicBlock *insert_bb = nullptr);
    ~AllocaInstruction();
    void output() const;
    void genMachineCode(AsmBuilder *);

private:
    SymbolEntry *se;
};

class LoadInstruction : public Instruction
{
public:
    LoadInstruction(Operand *dst, Operand *src_addr, BasicBlock *insert_bb = nullptr);
    ~LoadInstruction();
    void output() const;
    void genMachineCode(AsmBuilder *);
};

class StoreInstruction : public Instruction
{
public:
    StoreInstruction(Operand *dst_addr, Operand *src, BasicBlock *insert_bb = nullptr);
    ~StoreInstruction();
    void output() const;
    void genMachineCode(AsmBuilder *);
};

class BinaryInstruction : public Instruction
{
public:
    BinaryInstruction(unsigned opcode, Operand *dst, Operand *src1, Operand *src2, BasicBlock *insert_bb = nullptr);
    ~BinaryInstruction();
    void output() const;
    void genMachineCode(AsmBuilder *);
    enum
    {
        SUB,
        ADD,
        AND,
        OR,
        MUL,
        DIV,
        MOD
    };
};

class CmpInstruction : public Instruction
{
public:
    CmpInstruction(unsigned opcode, Operand *dst, Operand *src1, Operand *src2, BasicBlock *insert_bb = nullptr);
    ~CmpInstruction();
    void output() const;
    void genMachineCode(AsmBuilder *);
    enum
    {
        E,
        NE,
        L,
        LE,
        G,
        GE
    };
};

// unconditional branch
class UncondBrInstruction : public Instruction
{
public:
    UncondBrInstruction(BasicBlock *, BasicBlock *insert_bb = nullptr);
    void output() const;
    void setBranch(BasicBlock *);
    BasicBlock *getBranch();
    void genMachineCode(AsmBuilder *);

protected:
    BasicBlock *branch;
};

// conditional branch
class CondBrInstruction : public Instruction
{
public:
    CondBrInstruction(BasicBlock *, BasicBlock *, Operand *, BasicBlock *insert_bb = nullptr);
    ~CondBrInstruction();
    void output() const;
    void setTrueBranch(BasicBlock *);
    BasicBlock *getTrueBranch();
    void setFalseBranch(BasicBlock *);
    BasicBlock *getFalseBranch();
    void genMachineCode(AsmBuilder *);

protected:
    BasicBlock *true_branch;
    BasicBlock *false_branch;
};

class RetInstruction : public Instruction
{
public:
    RetInstruction(Operand *src, BasicBlock *insert_bb = nullptr);
    ~RetInstruction();
    void output() const;
    void genMachineCode(AsmBuilder *);
};

class CallInstruction : public Instruction
{
private:
    SymbolEntry *f;
    Operand *dst;

public:
    CallInstruction(Operand *dst,
                    SymbolEntry *f,
                    std::vector<Operand *> params,
                    BasicBlock *insert_bb = nullptr);
    void output() const;
    void genMachineCode(AsmBuilder *);
};

class NotInstruction : public Instruction
{
public:
    NotInstruction(Operand *dst, Operand *src, BasicBlock *insert_bb = nullptr);
    void output() const;
    void genMachineCode(AsmBuilder *);
};

class ZextInstruction : public Instruction
{
public:
    ZextInstruction(Operand *dst, Operand *src, BasicBlock *insert_bb = nullptr);
    void output() const;
    void genMachineCode(AsmBuilder *);
};

class ZextInstruction2 : public Instruction
{
public:
    ZextInstruction2(Operand *dst, Operand *src, BasicBlock *insert_bb = nullptr);
    void output() const;
    void genMachineCode(AsmBuilder *);
};

class GepInstruction : public Instruction
{
private:
    bool paramFirst;
    bool first;
    bool final;
    Operand *init;

public:
    GepInstruction(Operand *dst,
                   Operand *arr,
                   Operand *idx,
                   BasicBlock *insert_bb = nullptr,
                   bool paramFirst = false);
    ~GepInstruction();
    void output() const;
    void genMachineCode(AsmBuilder *);
    void setFirst() { first = true; };
    void setFinal() { final = true; };
    Operand *getInit() const { return init; };
    void setInit(Operand *init) { this->init = init; };
};

#endif