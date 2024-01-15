#include "Instruction.h"
#include "BasicBlock.h"
#include <iostream>
#include "Function.h"
#include "Type.h"
extern FILE *yyout;

Instruction::Instruction(unsigned instType, BasicBlock *insert_bb)
{
    prev = next = this;
    opcode = -1;
    this->instType = instType;
    if (insert_bb != nullptr)
    {
        insert_bb->insertBack(this);
        parent = insert_bb;
    }
}

Instruction::~Instruction()
{
    parent->remove(this);
}

BasicBlock *Instruction::getParent()
{
    return parent;
}

void Instruction::setParent(BasicBlock *bb)
{
    parent = bb;
}

void Instruction::setNext(Instruction *inst)
{
    next = inst;
}

void Instruction::setPrev(Instruction *inst)
{
    prev = inst;
}

Instruction *Instruction::getNext()
{
    return next;
}

Instruction *Instruction::getPrev()
{
    return prev;
}

BinaryInstruction::BinaryInstruction(unsigned opcode, Operand *dst, Operand *src1, Operand *src2, BasicBlock *insert_bb) : Instruction(BINARY, insert_bb)
{
    this->opcode = opcode;
    operands.push_back(dst);
    operands.push_back(src1);
    operands.push_back(src2);
    dst->setDef(this);
    src1->addUse(this);
    src2->addUse(this);
}

BinaryInstruction::~BinaryInstruction()
{
    operands[0]->setDef(nullptr);
    if (operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
    operands[2]->removeUse(this);
}

void BinaryInstruction::output() const
{
    std::string s1, s2, s3, op, type;
    s1 = operands[0]->toStr();
    s2 = operands[1]->toStr();
    s3 = operands[2]->toStr();
    type = operands[0]->getType()->toStr();
    switch (opcode)
    {
    case ADD:
        op = "add";
        break;
    case SUB:
        op = "sub";
        break;
    case MUL:
        op = "mul";
        break;
    case DIV:
        op = "sdiv";
        break;
    case MOD:
        op = "srem";
        break;
    default:
        break;
    }
    fprintf(yyout, "  %s = %s %s %s, %s\n", s1.c_str(), op.c_str(), type.c_str(), s2.c_str(), s3.c_str());
}

CmpInstruction::CmpInstruction(unsigned opcode, Operand *dst, Operand *src1, Operand *src2, BasicBlock *insert_bb) : Instruction(CMP, insert_bb)
{
    this->opcode = opcode;
    operands.push_back(dst);
    operands.push_back(src1);
    operands.push_back(src2);
    dst->setDef(this);
    src1->addUse(this);
    src2->addUse(this);
}

CmpInstruction::~CmpInstruction()
{
    operands[0]->setDef(nullptr);
    if (operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
    operands[2]->removeUse(this);
}

void CmpInstruction::output() const
{
    std::string s1, s2, s3, op, type;
    s1 = operands[0]->toStr();
    s2 = operands[1]->toStr();
    s3 = operands[2]->toStr();
    if (operands[1]->getType()->isFunc())
        type = dynamic_cast<FunctionType *>(operands[1]->getType())->getRetType()->toStr();
    else
        type = operands[1]->getType()->toStr();
    switch (opcode)
    {
    case E:
        op = "eq";
        break;
    case NE:
        op = "ne";
        break;
    case L:
        op = "slt";
        break;
    case LE:
        op = "sle";
        break;
    case G:
        op = "sgt";
        break;
    case GE:
        op = "sge";
        break;
    default:
        op = "";
        break;
    }

    fprintf(yyout, "  %s = icmp %s %s %s, %s\n", s1.c_str(), op.c_str(), type.c_str(), s2.c_str(), s3.c_str());
}
UncondBrInstruction::UncondBrInstruction(BasicBlock *to, BasicBlock *insert_bb) : Instruction(UNCOND, insert_bb)
{
    branch = to;
}

void UncondBrInstruction::output() const
{
    fprintf(yyout, "  br label %%B%d\n", branch->getNo());
}

void UncondBrInstruction::setBranch(BasicBlock *bb)
{
    branch = bb;
}

BasicBlock *UncondBrInstruction::getBranch()
{
    return branch;
}

CondBrInstruction::CondBrInstruction(BasicBlock *true_branch, BasicBlock *false_branch, Operand *cond, BasicBlock *insert_bb) : Instruction(COND, insert_bb)
{
    this->true_branch = true_branch;
    this->false_branch = false_branch;
    cond->addUse(this);
    operands.push_back(cond);
}

CondBrInstruction::~CondBrInstruction()
{
    operands[0]->removeUse(this);
}

void CondBrInstruction::output() const
{
    std::string cond, type;
    cond = operands[0]->toStr();
    type = operands[0]->getType()->toStr();
    int true_label = true_branch->getNo();
    int false_label = false_branch->getNo();
    fprintf(yyout, "  br %s %s, label %%B%d, label %%B%d\n", type.c_str(), cond.c_str(), true_label, false_label);
}

void CondBrInstruction::setFalseBranch(BasicBlock *bb)
{
    false_branch = bb;
}

BasicBlock *CondBrInstruction::getFalseBranch()
{
    return false_branch;
}

void CondBrInstruction::setTrueBranch(BasicBlock *bb)
{
    true_branch = bb;
}

BasicBlock *CondBrInstruction::getTrueBranch()
{
    return true_branch;
}

RetInstruction::RetInstruction(Operand *src, BasicBlock *insert_bb) : Instruction(RET, insert_bb)
{
    if (src != nullptr)
    {
        operands.push_back(src);
        src->addUse(this);
    }
}

RetInstruction::~RetInstruction()
{
    if (!operands.empty())
        operands[0]->removeUse(this);
}

void RetInstruction::output() const
{
    if (operands.empty())
    {
        fprintf(yyout, "  ret void\n");
    }
    else
    {
        std::string ret_str, type_str;
        ret_str = operands[0]->toStr();
        type_str = operands[0]->getType()->toStr();

        if (type_str.find("(") == std::string::npos)
        {
            fprintf(yyout, "  ret %s %s\n", type_str.c_str(), ret_str.c_str());
        }
        else
        {
            std::string type_str_new = type_str.substr(0, type_str.find('('));
            fprintf(yyout, "  ret %s %s\n", type_str_new.c_str(), ret_str.c_str());
        }
    }
}

AllocaInstruction::AllocaInstruction(Operand *dst, SymbolEntry *se, BasicBlock *insert_bb) : Instruction(ALLOCA, insert_bb)
{
    operands.push_back(dst);
    dst->setDef(this);
    this->se = se;
}

AllocaInstruction::~AllocaInstruction()
{
    operands[0]->setDef(nullptr);
    if (operands[0]->usersNum() == 0)
        delete operands[0];
}

void AllocaInstruction::output() const
{
    std::string dst, type;
    dst = operands[0]->toStr();
    type = se->getType()->toStr();
    fprintf(yyout, "  %s = alloca %s, align 4\n", dst.c_str(), type.c_str());
}

LoadInstruction::LoadInstruction(Operand *dst, Operand *src_addr, BasicBlock *insert_bb) : Instruction(LOAD, insert_bb)
{
    if (!dst)
    {
        // fprintf(stderr, "【WARN LOAD】dst is nullptr!\n");
        dst = new Operand(new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel()));
    }
    if (!src_addr)
    {
        // fprintf(stderr, "【WARN LOAD】src_addr is nullptr!\n");
        //  SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        //  src_addr = dynamic_cast<IdentifierSymbolEntry *>(se)->getAddr();
        src_addr = new Operand(new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel()));
    }
    operands.push_back(dst);
    operands.push_back(src_addr);
    dst->setDef(this);
    src_addr->addUse(this);
}

LoadInstruction::~LoadInstruction()
{
    operands[0]->setDef(nullptr);
    if (operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
}

void LoadInstruction::output() const
{
    std::string dst = operands[0]->toStr();
    std::string src = operands[1]->toStr();
    std::string src_type;
    std::string dst_type;
    dst_type = operands[0]->getType()->toStr();
    src_type = operands[1]->getType()->toStr();
    fprintf(yyout, "  %s = load %s, %s %s, align 4\n", dst.c_str(), dst_type.c_str(), src_type.c_str(), src.c_str());
}

StoreInstruction::StoreInstruction(Operand *dst_addr, Operand *src, BasicBlock *insert_bb) : Instruction(STORE, insert_bb)
{
    if (!dst_addr)
    {
        fprintf(stderr, "【WARN STORE】dst_addr is nullptr!\n");
        // SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        // dst_addr = dynamic_cast<IdentifierSymbolEntry *>(se)->getAddr();
        dst_addr = new Operand(new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel()));
    }
    if (!src)
    {
        fprintf(stderr, "【WARN STORE】src is nullptr!\n");
        src = new Operand(new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel()));
    }
    operands.push_back(dst_addr);
    operands.push_back(src);
    dst_addr->addUse(this);
    src->addUse(this);
}

StoreInstruction::~StoreInstruction()
{
    operands[0]->removeUse(this);
    operands[1]->removeUse(this);
}

void StoreInstruction::output() const
{
    std::string dst = operands[0]->toStr();
    std::string src = operands[1]->toStr();
    std::string dst_type = operands[0]->getType()->toStr();
    std::string src_type = operands[1]->getType()->toStr();

    if (src_type.find("(") == std::string::npos)
    {
        fprintf(yyout, "  store %s %s, %s %s, align 4\n", src_type.c_str(), src.c_str(), dst_type.c_str(), dst.c_str());
    }
    else
    {
        std::string typestr = src_type.substr(0, src_type.find('('));
        fprintf(yyout, "  store %s %s, %s %s, align 4\n", typestr.c_str(), src.c_str(), dst_type.c_str(), dst.c_str());
    }
}

CallInstruction::CallInstruction(Operand *dst,
                                 SymbolEntry *f,
                                 std::vector<Operand *> params,
                                 BasicBlock *insert_bb)
    : Instruction(CALL, insert_bb), f(f), dst(dst)
{
    operands.push_back(dst);
    if (dst)
    {
        dst->setDef(this);
    }
    for (auto param : params)
    {
        operands.push_back(param);
        param->addUse(this);
    }
}

void CallInstruction::output() const
{
    fprintf(yyout, "  ");
    if (operands[0])
        fprintf(yyout, "%s = ", operands[0]->toStr().c_str());
    Type *type = dynamic_cast<FunctionType *>(f->getType())->getRetType();
    fprintf(yyout, "call %s %s(", type->toStr().c_str(),
            f->toStr().c_str());
    int size = operands.size();
    for (int i = 1; i < size; i++)
    {
        if (i - 1)
            fprintf(yyout, ", ");
        std::string ret_str = operands[i]->getType()->toStr();
        std::string ret_str_new = ret_str.substr(0, ret_str.find('('));
        fprintf(yyout, "%s %s", ret_str_new.c_str(),
                operands[i]->toStr().c_str());
    }
    fprintf(yyout, ")\n");
}

NotInstruction::NotInstruction(Operand *dst, Operand *src, BasicBlock *insert_bb) : Instruction(NOT, insert_bb)
{
    operands.push_back(dst);
    operands.push_back(src);
    dst->setDef(this);
    src->addUse(this);
}

void NotInstruction ::output() const
{
    Operand *dst = operands[0];
    Operand *src = operands[1];
    fprintf(yyout, "  %s = xor i1 %s, true\n", dst->toStr().c_str(), src->toStr().c_str());
}

ZextInstruction::ZextInstruction(Operand *dst, Operand *src, BasicBlock *insert_bb) : Instruction(ZEXT, insert_bb)
{
    operands.push_back(dst);
    operands.push_back(src);
    dst->setDef(this);
    src->addUse(this);
}

ZextInstruction2::ZextInstruction2(Operand *dst, Operand *src, BasicBlock *insert_bb) : Instruction(ZEXT, insert_bb)
{
    operands.push_back(dst);
    operands.push_back(src);
    dst->setDef(this);
    src->addUse(this);
}

void ZextInstruction ::output() const
{
    Operand *dst = operands[0];
    Operand *src = operands[1];
    fprintf(yyout, "  %s = zext i1 %s to i32\n", dst->toStr().c_str(), src->toStr().c_str());
}

void ZextInstruction2 ::output() const
{
    Operand *dst = operands[0];
    Operand *src = operands[1];
    fprintf(yyout, "  %s = zext i32 %s to i1\n", dst->toStr().c_str(), src->toStr().c_str());
}

MachineOperand *Instruction::genMachineOperand(Operand *ope)
{
    auto se = ope->getEntry();
    MachineOperand *mope = nullptr;
    if (se->isConstant())
        mope = new MachineOperand(MachineOperand::IMM, dynamic_cast<ConstantSymbolEntry *>(se)->getIntValue());
    else if (se->isTemporary())
        mope = new MachineOperand(MachineOperand::VREG, dynamic_cast<TemporarySymbolEntry *>(se)->getLabel());
    else if (se->isVariable())
    {
        auto var_se = dynamic_cast<IdentifierSymbolEntry *>(se);
        if (var_se->isGlobal())
        {
            mope = new MachineOperand(var_se->toStr().c_str());
        }
        else if (var_se->isParam())
        {

            if (var_se->getParamNo() < 4 && var_se->getParamNo() >= 0)
            {
                mope = new MachineOperand(MachineOperand::REG, var_se->getParamNo());
            }
            else
            {
                // r3作为中介吧
                mope = new MachineOperand(MachineOperand::REG, 3);
            }
        }
        else
        {
            fprintf(stderr, "Can not gen machine operand2.\n");
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "Can not gen machine operand3.\n");
        exit(1);
    }
    return mope;
}

MachineOperand *Instruction::genMachineReg(int reg)
{
    return new MachineOperand(MachineOperand::REG, reg);
}

MachineOperand *Instruction::genMachineVReg()
{
    return new MachineOperand(MachineOperand::VREG, SymbolTable::getLabel());
}

MachineOperand *Instruction::genMachineImm(int val)
{
    return new MachineOperand(MachineOperand::IMM, val);
}

MachineOperand *Instruction::genMachineLabel(int block_no)
{
    std::ostringstream buf;
    buf << ".L" << block_no;
    std::string label = buf.str();
    return new MachineOperand(label);
}

void AllocaInstruction::genMachineCode(AsmBuilder *builder)
{
    /* HINT:
     * Allocate stack space for local variabel
     * Store frame offset in symbol entry */
    auto cur_func = builder->getFunction();
    int bytes = se->getType()->getSize() / 8;
    if (bytes < 0)
        bytes = 4;
    int offset = cur_func->AllocSpace(bytes);
    dynamic_cast<TemporarySymbolEntry *>(operands[0]->getEntry())->setOffset(-offset);
}

void LoadInstruction::genMachineCode(AsmBuilder *builder)
{
    auto mbb = builder->getBlock();
    MachineInstruction *inst = nullptr;
    // global
    if (operands[1]->getEntry()->isVariable() && dynamic_cast<IdentifierSymbolEntry *>(operands[1]->getEntry())->isGlobal())
    {
        auto dst = genMachineOperand(operands[0]);
        auto internal_reg1 = genMachineVReg();
        auto internal_reg2 = new MachineOperand(*internal_reg1);
        auto src = genMachineOperand(operands[1]);
        // eg. load r0, addr_a
        inst = new LoadMInstruction(mbb, internal_reg1, src);
        mbb->InsertInst(inst);
        // eg. load r1, [r0]
        inst = new LoadMInstruction(mbb, dst, internal_reg2);
        mbb->InsertInst(inst);
    }
    // local
    else if (operands[1]->getEntry()->isTemporary() && operands[1]->getDef() && operands[1]->getDef()->isAlloc())
    {
        // eg. load r1, [r0, #4]
        auto dst = genMachineOperand(operands[0]);
        auto src1 = genMachineReg(11); // fp
        int offset = dynamic_cast<TemporarySymbolEntry *>(operands[1]->getEntry())->getOffset();
        auto src2 = genMachineImm(offset);
        if (offset > 255 || offset < -255)
        {
            auto temp_vreg = genMachineVReg();
            inst = new LoadMInstruction(mbb, temp_vreg, src2);
            mbb->InsertInst(inst);
            src2 = temp_vreg;
        }
        inst = new LoadMInstruction(mbb, dst, src1, src2);
        mbb->InsertInst(inst);
    }
    // from temp var
    else
    {
        // eg. load r1, [r0]
        auto dst = genMachineOperand(operands[0]);
        auto src = genMachineOperand(operands[1]);
        inst = new LoadMInstruction(mbb, dst, src);
        mbb->InsertInst(inst);
    }
}

void StoreInstruction::genMachineCode(AsmBuilder *builder)
{
    // TODO
    auto mbb = builder->getBlock();
    MachineInstruction *inst = nullptr;
    auto src = genMachineOperand(operands[1]);
    auto dst = genMachineOperand(operands[0]);
    if (src->isImm())
    {
        // 形如 store i32 1 i32* t1...,src是一个imm
        auto internal_reg = genMachineVReg();
        inst = new LoadMInstruction(mbb, internal_reg, src);
        mbb->InsertInst(inst);
        src = new MachineOperand(*internal_reg);
    }
    // store global
    if (operands[0]->getEntry()->isVariable() && dynamic_cast<IdentifierSymbolEntry *>(operands[0]->getEntry())->isGlobal())
    {
        // I wanwa: str 10, a
        // ldr 10, vreg(in isImm())
        // ldr reg, addr_x
        // str vreg, reg
        auto internal_reg1 = genMachineVReg();
        inst = new LoadMInstruction(mbb, internal_reg1, dst);
        mbb->InsertInst(inst);
        inst = new StoreMInstruction(mbb, src, internal_reg1);
        mbb->InsertInst(inst);
    }
    else if (operands[0]->getEntry()->isTemporary() && operands[0]->getDef() && operands[0]->getDef()->isAlloc()) // store local eg: str v8, [fp, #-8]
    {
        auto dst_1 = genMachineReg(11); // fp
        int offset = dynamic_cast<TemporarySymbolEntry *>(operands[0]->getEntry())
                         ->getOffset(); // get the offset of the
        auto dst_2 = genMachineImm(offset);
        if (offset > 255 || offset < -255)
        {
            auto temp_vreg = genMachineVReg();
            inst = new LoadMInstruction(mbb, temp_vreg, dst_2);
            mbb->InsertInst(inst);
            dst_2 = temp_vreg;
        }
        inst = new StoreMInstruction(mbb, src, dst_1, dst_2);
        mbb->InsertInst(inst);
    }
    else
    {
        inst = new StoreMInstruction(mbb, src, dst);
        mbb->InsertInst(inst);
    }
}

void BinaryInstruction::genMachineCode(AsmBuilder *builder)
{
    // TODO:
    // complete other instructions
    auto mbb = builder->getBlock();
    auto dst = genMachineOperand(operands[0]);
    auto src1 = genMachineOperand(operands[1]);
    auto src2 = genMachineOperand(operands[2]);
    /* HINT:
     * The source operands of ADD instruction in ir code both can be immediate num.
     * However, it's not allowed in assembly code.
     * So you need to insert LOAD/MOV instrucrion to load immediate num into register.
     * As to other instructions, such as MUL, CMP, you need to deal with this situation, too.*/
    MachineInstruction *inst = nullptr;
    if (src1->isImm())
    {
        auto internal_reg = genMachineVReg();
        inst = new LoadMInstruction(mbb, internal_reg, src1);
        mbb->InsertInst(inst);
        src1 = new MachineOperand(*internal_reg);
    }
    if (src2->isImm())
    {
        if ((opcode <= BinaryInstruction::OR &&
             ((ConstantSymbolEntry *)(operands[2]->getEntry()))->getIntValue() >
                 255) ||
            opcode >= BinaryInstruction::MUL)
        {
            auto internal_reg = genMachineVReg();
            inst = new LoadMInstruction(mbb, internal_reg, src2);
            mbb->InsertInst(inst);
            src2 = new MachineOperand(*internal_reg);
        }
    }
    switch (opcode)
    {
    case ADD:
        inst = new BinaryMInstruction(mbb, BinaryMInstruction::ADD, dst, src1, src2);
        break;
    case SUB:
        inst = new BinaryMInstruction(mbb, BinaryMInstruction::SUB, dst, src1, src2);
        break;
    case AND:
        inst = new BinaryMInstruction(mbb, BinaryMInstruction::AND, dst, src1, src2);
        break;
    case OR:
        inst = new BinaryMInstruction(mbb, BinaryMInstruction::OR, dst, src1, src2);
        break;
    case MUL:
        inst = new BinaryMInstruction(mbb, BinaryMInstruction::MUL, dst, src1, src2);
        break;
    case DIV:
        inst = new BinaryMInstruction(mbb, BinaryMInstruction::DIV, dst, src1, src2);
        break;
    case MOD:
    { // a mod b = a - (a / b) * b
        inst = new BinaryMInstruction(mbb, BinaryMInstruction::DIV, dst, src1, src2);
        auto temp_dst = new MachineOperand(*dst);
        src1 = new MachineOperand(*src1);
        src2 = new MachineOperand(*src2);
        mbb->InsertInst(inst);
        inst = new BinaryMInstruction(
            mbb, BinaryMInstruction::MUL, temp_dst, dst, src2);
        mbb->InsertInst(inst);
        dst = new MachineOperand(*temp_dst);
        inst = new BinaryMInstruction(
            mbb, BinaryMInstruction::SUB, dst, src1, temp_dst);
        break;
    }
    default:
        break;
    }
    mbb->InsertInst(inst);
}

void CmpInstruction::genMachineCode(AsmBuilder *builder)
{
    auto mbb = builder->getBlock();
    auto dst = genMachineOperand(operands[0]);
    auto src_1 = genMachineOperand(operands[1]);
    auto src_2 = genMachineOperand(operands[2]);
    MachineInstruction *inst = nullptr;
    if (src_1->isImm())
    { // like before, load first
        auto temp_vreg = genMachineVReg();
        inst = new LoadMInstruction(mbb, temp_vreg, src_1);
        mbb->InsertInst(inst);
        src_1 = new MachineOperand(*temp_vreg);
    }
    if (src_2->isImm() && (operands[2]->getEntry()->getIntValue() > 255 || operands[2]->getEntry()->getIntValue() < -255))
    {
        auto temp_vreg = genMachineVReg();
        inst = new LoadMInstruction(mbb, temp_vreg, src_2);
        mbb->InsertInst(inst);
        src_2 = new MachineOperand(*temp_vreg);
    }
    inst = new CmpMInstruction(mbb, src_1, src_2, opcode);
    builder->setCmpOpcode(opcode);
    mbb->InsertInst(inst);
    // E、NE
    if (opcode >= CmpInstruction::L && opcode <= CmpInstruction::GE)
    {
        auto trueope = genMachineImm(1);
        auto falseope = genMachineImm(0);
        inst = new MovMInstruction(mbb, MovMInstruction::MOV, dst,
                                   trueope, opcode);
        mbb->InsertInst(inst);
        inst = new MovMInstruction(mbb, MovMInstruction::MOV, dst,
                                   falseope, 7 - opcode);
        mbb->InsertInst(inst);
    }
}

void UncondBrInstruction::genMachineCode(AsmBuilder *builder)
{
    auto mbb = builder->getBlock();
    std::stringstream s;
    s << ".L" << branch->getNo();
    MachineOperand *dst = new MachineOperand(s.str());
    auto inst =
        new BranchMInstruction(mbb, BranchMInstruction::B, dst);
    mbb->InsertInst(inst);
}

void CondBrInstruction::genMachineCode(AsmBuilder *builder)
{
    auto mbb = builder->getBlock();
    std::stringstream true_branch_label, false_branch_label;
    true_branch_label << ".L" << true_branch->getNo();
    auto dst = new MachineOperand(true_branch_label.str());
    auto inst = new BranchMInstruction(mbb, BranchMInstruction::B,
                                       dst, builder->getCmpOpcode());
    mbb->InsertInst(inst);
    false_branch_label << ".L" << false_branch->getNo();
    dst = new MachineOperand(false_branch_label.str());
    inst = new BranchMInstruction(mbb, BranchMInstruction::B, dst);
    mbb->InsertInst(inst);
}

void RetInstruction::genMachineCode(AsmBuilder *builder)
{
    // TODO
    /* HINT:
     * 1. Generate mov instruction to save return value in r0
     * 2. Restore callee saved registers and sp, fp
     * 3. Generate bx instruction */
    auto mbb = builder->getBlock();
    MachineInstruction *inst = nullptr;
    if (operands.size())
    {
        // store retval in r0
        auto dst = new MachineOperand(MachineOperand::REG, 0);
        auto src = genMachineOperand(operands[0]);
        inst = new MovMInstruction(mbb, MovMInstruction::MOV, dst, src);
        mbb->InsertInst(inst);
    }
    auto cur_func = builder->getFunction();
    auto reg_sp = new MachineOperand(MachineOperand::REG, 13);
    // add sp, sp, #20
    auto func_stack_size = new MachineOperand(MachineOperand::IMM, cur_func->AllocSpace(0));
    inst = new BinaryMInstruction(mbb, BinaryMInstruction::ADD, reg_sp, reg_sp, func_stack_size);
    mbb->InsertInst(inst);
    auto lr = new MachineOperand(MachineOperand::REG, 14);
    // bx lr
    inst = new BranchMInstruction(mbb, BranchMInstruction::BX, lr);
    mbb->InsertInst(inst);
}

void CallInstruction::genMachineCode(AsmBuilder *builder) // todo::xiugai
{
    auto mbb = builder->getBlock();
    MachineOperand *operand;
    MachineInstruction *inst;
    long unsigned int i = 0;
    for (i = 1; i < operands.size() && i < 5; i++)
    {
        operand = genMachineReg(i - 1); // r0~r3
        auto src = genMachineOperand(operands[i]);
        if (src->isImm() && src->getVal() > 255)
        {
            inst = new LoadMInstruction(mbb, operand, src);
        }
        else
            inst = new MovMInstruction(mbb, MovMInstruction::MOV,
                                       operand, src);
        mbb->InsertInst(inst);
    }
    i = operands.size() - 1;
    for (; i > 4; i--)
    {
        operand = genMachineOperand(operands[i]);
        if (operand->isImm())
        {
            auto dst = genMachineVReg();
            if (operand->getVal() < 255 && operand->getVal() > -255)
                inst = new MovMInstruction(mbb, MovMInstruction::MOV,
                                           dst, operand);
            else
                inst = new LoadMInstruction(mbb, dst, operand);
            mbb->InsertInst(inst);
            operand = dst;
        }
        std::vector<MachineOperand *> vec;
        inst = new StackMInstruction(mbb, StackMInstruction::PUSH, vec, operand);
        mbb->InsertInst(inst);
    }
    auto label = new MachineOperand(f->toStr().c_str());
    inst = new BranchMInstruction(mbb, BranchMInstruction::BL, label);
    mbb->InsertInst(inst);
    if (operands.size() > 5)
    {
        auto off = genMachineImm((operands.size() - 5) * 4);
        auto sp = new MachineOperand(MachineOperand::REG, 13);

        inst = new BinaryMInstruction(mbb, BinaryMInstruction::ADD,
                                      sp, sp, off);
        mbb->InsertInst(inst);
    }
    // return value, mov r0, r4
    if (dst)
    {
        operand = genMachineOperand(dst);
        auto r0 = new MachineOperand(MachineOperand::REG, 0);
        inst =
            new MovMInstruction(mbb, MovMInstruction::MOV, operand, r0);
        mbb->InsertInst(inst);
    }
    else
    {
    }
}

void NotInstruction::genMachineCode(AsmBuilder *builder)
{
    // ! xor
    auto mbb = builder->getBlock();
    auto dst = genMachineOperand(operands[0]);
    auto trueOperand = genMachineImm(1);
    auto falseOperand = genMachineImm(0);
    auto inst = new MovMInstruction(mbb, MovMInstruction::MOV, dst,
                                    trueOperand, MachineInstruction::EQ);
    mbb->InsertInst(inst);
    inst = new MovMInstruction(mbb, MovMInstruction::MOV, dst,
                               falseOperand, MachineInstruction::NE);
    mbb->InsertInst(inst);
}

void ZextInstruction::genMachineCode(AsmBuilder *builder)
{
    auto mbb = builder->getBlock();
    auto dst = genMachineOperand(operands[0]);
    auto src = genMachineOperand(operands[1]);
    auto inst =
        new MovMInstruction(mbb, MovMInstruction::MOV, dst, src);
    mbb->InsertInst(inst);
}

void ZextInstruction2::genMachineCode(AsmBuilder *builder)
{
    // do nothing bool->int
}

GepInstruction::GepInstruction(Operand *dst,
                               Operand *arr,
                               Operand *idx,
                               BasicBlock *insert_bb,
                               bool paramFirst)
    : Instruction(GEP, insert_bb), paramFirst(paramFirst)
{
    operands.push_back(dst);
    operands.push_back(arr);
    operands.push_back(idx);
    dst->setDef(this);
    arr->addUse(this);
    idx->addUse(this);
    first = false;
    init = nullptr;
    final = false;
}

void GepInstruction::output() const
{
    Operand *dst = operands[0];
    Operand *arr = operands[1];
    Operand *idx = operands[2];
    std::string arrType = arr->getType()->toStr();

    if (paramFirst)
        fprintf(yyout, "  %s = getelementptr inbounds %s, %s %s, i32 %s\n",
                dst->toStr().c_str(),
                arrType.substr(0, arrType.size() - 1).c_str(), arrType.c_str(),
                arr->toStr().c_str(), idx->toStr().c_str());
    else
        fprintf(
            yyout, "  %s = getelementptr inbounds %s, %s %s, i32 0, i32 %s\n",
            dst->toStr().c_str(), arrType.substr(0, arrType.size() - 1).c_str(),
            arrType.c_str(), arr->toStr().c_str(), idx->toStr().c_str());
}

GepInstruction::~GepInstruction()
{
    operands[0]->setDef(nullptr);
    if (operands[0]->usersNum() == 0)
        delete operands[0];
    operands[1]->removeUse(this);
    operands[2]->removeUse(this);
}

void GepInstruction::genMachineCode(AsmBuilder *builder)
{
    // dst, arr, idx      first paramFirst final
    auto mbb = builder->getBlock();
    MachineInstruction *inst;
    auto dst = genMachineOperand(operands[0]);
    // auto arr = genMachineOperand(operands[1]);
    PointerType *ptype = (PointerType *)(operands[1]->getType());
    auto idx = genMachineOperand(operands[2]);
    int size;
    auto tmpidx = genMachineVReg();
    auto tmpsize = genMachineVReg();

    // 1.初始化列表，地址依次加4，野指针 => final
    if (init && final)
    {
        auto base = genMachineOperand(init);
        inst = new BinaryMInstruction(
            mbb, BinaryMInstruction::ADD, dst, base, genMachineImm(4));
        mbb->InsertInst(inst);
        return;
    }
    MachineOperand *base = nullptr;
    // 2.idx替换=>tmpidx, a[10000]
    if (idx->isImm())
    {
        if (idx->getVal() < 255 && idx->getVal() > -255)
        {
            inst =
                new MovMInstruction(mbb, MovMInstruction::MOV, tmpidx, idx);
        }
        else
        {
            inst = new LoadMInstruction(mbb, tmpidx, idx);
        }
        idx = new MachineOperand(*tmpidx);
        mbb->InsertInst(inst);
    }
    // 3.first, load地址
    if (paramFirst)
    {
        // pointer type,        void f(int arr[])
        size = ptype->getType()->getSize() / 8;
    }
    else
    {
        // 需要重新计算地址
        if (first)
        {
            base = genMachineVReg();
            if (operands[1]->getEntry()->isVariable() &&
                ((IdentifierSymbolEntry *)(operands[1]->getEntry()))->isGlobal())
            {
                auto src = genMachineOperand(operands[1]);
                inst = new LoadMInstruction(mbb, base, src);
            }
            else
            {
                int offset = ((TemporarySymbolEntry *)(operands[1]->getEntry()))
                                 ->getOffset();
                if (offset > -255 && offset < 255)
                {
                    inst =
                        new MovMInstruction(mbb, MovMInstruction::MOV,
                                            base, genMachineImm(offset));
                }
                else
                {
                    inst = new LoadMInstruction(mbb, base,
                                                genMachineImm(offset));
                }
            }
            mbb->InsertInst(inst);
        }
        ArrayType *atype = (ArrayType *)(ptype->getType());
        size = atype->getElementType()->getSize() / 8;
    }
    //  4.size替换=>tmpsize
    if (size > -255 && size < 255)
    {
        inst = new MovMInstruction(mbb, MovMInstruction::MOV, tmpsize,
                                   genMachineImm(size));
    }
    else
    {
        inst = new LoadMInstruction(mbb, tmpsize, genMachineImm(size));
    }
    // 5. MUL ADD => addr
    mbb->InsertInst(inst);
    auto off = genMachineVReg();
    inst = new BinaryMInstruction(mbb, BinaryMInstruction::MUL, off,
                                  idx, tmpsize);
    off = new MachineOperand(*off);
    mbb->InsertInst(inst);

    // 加载新id的地址
    if (!paramFirst && first)
    {
        auto addr = genMachineVReg();
        auto basemop = new MachineOperand(*base);
        inst = new BinaryMInstruction(mbb, BinaryMInstruction::ADD,
                                      addr, basemop, off);
        mbb->InsertInst(inst);
        addr = new MachineOperand(*addr);
        if (operands[1]->getEntry()->isVariable() &&
            ((IdentifierSymbolEntry *)(operands[1]->getEntry()))->isGlobal())
        {
            inst =
                new MovMInstruction(mbb, MovMInstruction::MOV, dst, addr);
        }
        else
        {
            auto fp = genMachineReg(11);
            inst = new BinaryMInstruction(
                mbb, BinaryMInstruction::ADD, dst, fp, addr);
        }
        mbb->InsertInst(inst);
    }
    else
    {
        auto arr = genMachineOperand(operands[1]);
        inst = new BinaryMInstruction(mbb, BinaryMInstruction::ADD,
                                      dst, arr, off);
        mbb->InsertInst(inst);
    }
}
