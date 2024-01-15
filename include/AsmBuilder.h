/*
在中间代码向目标代码进行自顶向下
的转换时，记录当前正在进行转换操作的对象，以进行函数、基本块及指令的插入
*/
#ifndef __ASMBUILDER_H__
#define __ASMBUILDER_H__

#include "MachineCode.h"

class AsmBuilder
{
private:
    MachineUnit *mUnit;         // mahicne unit代表一个机器代码单元
    MachineFunction *mFunction; // current machine code function;代表函数
    MachineBlock *mBlock;       // current machine code block;代表程序块
    int cmpOpcode;              // CmpInstruction opcode, for CondInstruction;比较指令的操作码，用于条件指令
public:
    void setUnit(MachineUnit *unit) { this->mUnit = unit; };
    void setFunction(MachineFunction *func) { this->mFunction = func; };
    void setBlock(MachineBlock *block) { this->mBlock = block; };
    void setCmpOpcode(int opcode) { this->cmpOpcode = opcode; };
    MachineUnit *getUnit() { return this->mUnit; };
    MachineFunction *getFunction() { return this->mFunction; };
    MachineBlock *getBlock() { return this->mBlock; };
    int getCmpOpcode() { return this->cmpOpcode; };
};

#endif