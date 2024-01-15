#include "MachineCode.h"
#include "Type.h"
extern FILE *yyout;
int MachineBlock::label = 0;

MachineOperand::MachineOperand(int tp, int val)
{
    this->type = tp;
    if (tp == MachineOperand::IMM)
        this->val = val;
    else
        this->reg_no = val;
}

MachineOperand::MachineOperand(std::string label)
{
    this->type = MachineOperand::LABEL;
    this->label = label;
}

bool MachineOperand::operator==(const MachineOperand &a) const
{
    if (this->type != a.type)
        return false;
    if (this->type == IMM)
        return this->val == a.val;
    return this->reg_no == a.reg_no;
}

bool MachineOperand::operator<(const MachineOperand &a) const
{
    if (this->type == a.type)
    {
        if (this->type == IMM)
            return this->val < a.val;
        return this->reg_no < a.reg_no;
    }
    return this->type < a.type;

    if (this->type != a.type)
        return false;
    if (this->type == IMM)
        return this->val == a.val;
    return this->reg_no == a.reg_no;
}

void MachineOperand::PrintReg()
{
    switch (reg_no)
    {
    case 11:
        fprintf(yyout, "fp");
        break;
    case 13:
        fprintf(yyout, "sp");
        break;
    case 14:
        fprintf(yyout, "lr");
        break;
    case 15:
        fprintf(yyout, "pc");
        break;
    default:
        fprintf(yyout, "r%d", reg_no);
        break;
    }
}

void MachineOperand::output()
{
    /* HINT：print operand
     * Example:
     * immediate num 1 -> print #1;
     * register 1 -> print r1;
     * lable addr_a -> print addr_a; */
    switch (this->type)
    {
    case IMM:
        fprintf(yyout, "#%d", this->val);
        break;
    case VREG:
        fprintf(yyout, "v%d", this->reg_no);
        break;
    case REG:
        PrintReg();
        break;
    case LABEL:
        if (this->label.substr(0, 2) == ".L")
            fprintf(yyout, "%s", this->label.c_str());
        else if (this->label[0] == '@')
            fprintf(yyout, "%s", this->label.c_str() + 1); // global
        else
            fprintf(yyout, "addr_%s%d", this->label.c_str(), parent->getParent()->getParent()->getParent()->getZone());
    default:
        break;
    }
}

void MachineInstruction::PrintCond()
{
    // TODO
    switch (cond)
    {
    case LT:
        fprintf(yyout, "lt");
        break;
    case LE:
        fprintf(yyout, "le");
        break;
    case GT:
        fprintf(yyout, "gt");
        break;
    case GE:
        fprintf(yyout, "ge");
        break;
    case EQ:
        fprintf(yyout, "eq");
        break;
    case NE:
        fprintf(yyout, "ne");
        break;
    default:
        break;
    }
}

void MachineInstruction::insertBefore(MachineInstruction *inst)
{
    auto &instructions = parent->getInsts();
    auto it = std::find(instructions.begin(), instructions.end(), this);
    instructions.insert(it, inst);
}

void MachineInstruction::insertAfter(MachineInstruction *inst)
{
    auto &instructions = parent->getInsts();
    auto it = std::find(instructions.begin(), instructions.end(), this);
    instructions.insert(++it, inst);
}

BinaryMInstruction::BinaryMInstruction(
    MachineBlock *p, int op,
    MachineOperand *dst, MachineOperand *src1, MachineOperand *src2,
    int cond)
{
    this->parent = p;
    this->type = MachineInstruction::BINARY;
    this->op = op;
    this->cond = cond;
    this->def_list.push_back(dst);
    this->use_list.push_back(src1);
    this->use_list.push_back(src2);
    dst->setParent(this);
    src1->setParent(this);
    src2->setParent(this);
}

void BinaryMInstruction::output()
{
    // TODO:
    // Complete other instructions
    switch (this->op)
    {
    case BinaryMInstruction::ADD:
        fprintf(yyout, "\tadd ");
        break;
    case BinaryMInstruction::SUB:
        fprintf(yyout, "\tsub ");
        break;
    case BinaryMInstruction::AND:
        fprintf(yyout, "\tand ");
        break;
    case BinaryMInstruction::OR:
        fprintf(yyout, "\tor ");
        break;
    case BinaryMInstruction::MUL:
        fprintf(yyout, "\tmul ");
        break;
    case BinaryMInstruction::DIV:
        fprintf(yyout, "\tsdiv ");
        break;
    default:
        break;
    }
    this->PrintCond();
    this->def_list[0]->output();
    fprintf(yyout, ", ");
    this->use_list[0]->output();
    fprintf(yyout, ", ");
    this->use_list[1]->output();
    fprintf(yyout, "\n");
}

LoadMInstruction::LoadMInstruction(MachineBlock *p,
                                   MachineOperand *dst, MachineOperand *src1, MachineOperand *src2,
                                   int cond)
{
    this->parent = p;
    this->type = MachineInstruction::LOAD;
    this->op = -1;
    this->cond = cond;
    this->def_list.push_back(dst);
    this->use_list.push_back(src1);
    if (src2)
        this->use_list.push_back(src2);
    dst->setParent(this);
    src1->setParent(this);
    if (src2)
        src2->setParent(this);
}

void LoadMInstruction::output()
{
    fprintf(yyout, "\tldr ");
    this->def_list[0]->output();
    fprintf(yyout, ", ");

    // Load immediate num, eg: ldr r1, =8
    if (this->use_list[0]->isImm())
    {
        fprintf(yyout, "=%d\n", this->use_list[0]->getVal());
        return;
    }

    // Load address
    if (this->use_list[0]->isReg() || this->use_list[0]->isVReg())
        fprintf(yyout, "[");

    this->use_list[0]->output();
    if (this->use_list.size() > 1)
    {
        fprintf(yyout, ", ");
        this->use_list[1]->output();
    }

    if (this->use_list[0]->isReg() || this->use_list[0]->isVReg())
        fprintf(yyout, "]");
    fprintf(yyout, "\n");
}

StoreMInstruction::StoreMInstruction(MachineBlock *p,
                                     MachineOperand *src1, MachineOperand *src2, MachineOperand *src3,
                                     int cond)
{
    this->parent = p;
    this->type = MachineInstruction::STORE;
    this->op = -1;
    this->cond = cond;
    this->use_list.push_back(src1);
    this->use_list.push_back(src2);
    if (src3)
        this->use_list.push_back(src3);
    src1->setParent(this);
    src2->setParent(this);
    if (src3)
        src3->setParent(this);
}

void StoreMInstruction::output()
{
    fprintf(yyout, "\tstr ");
    this->use_list[0]->output();
    fprintf(yyout, ", ");
    if (this->use_list[1]->isReg() || this->use_list[1]->isVReg())
        fprintf(yyout, "[");
    this->use_list[1]->output();
    if (this->use_list.size() > 2)
    {
        fprintf(yyout, ", ");
        this->use_list[2]->output();
    }
    if (this->use_list[1]->isReg() || this->use_list[1]->isVReg())
        fprintf(yyout, "]");
    fprintf(yyout, "\n");
}

MovMInstruction::MovMInstruction(MachineBlock *p, int op,
                                 MachineOperand *dst, MachineOperand *src,
                                 int cond)
{
    this->type = MachineInstruction::MOV;
    this->parent = p;
    this->op = op;
    this->def_list.push_back(dst);
    this->use_list.push_back(src);
    dst->setParent(this);
    src->setParent(this);
    this->cond = cond;
}

void MovMInstruction::output()
{
    switch (this->op)
    {
    case MovMInstruction::MOV:
        fprintf(yyout, "\tmov");
        break;
    case MovMInstruction::MVN:
        fprintf(yyout, "\tmvn");
        break;
    default:
        break;
    }
    PrintCond();
    fprintf(yyout, " ");
    this->def_list[0]->output();
    fprintf(yyout, ", ");
    this->use_list[0]->output();
    fprintf(yyout, "\n");
}

BranchMInstruction::BranchMInstruction(MachineBlock *p, int op,
                                       MachineOperand *dst,
                                       int cond)
{
    this->parent = p;
    this->op = op;
    this->cond = cond;
    this->type = MachineInstruction::BRANCH;
    this->use_list.push_back(dst);
    dst->setParent(this);
}

void BranchMInstruction::output()
{
    switch (op)
    {
    case B:
        fprintf(yyout, "\tb");
        break;
    case BX:
        fprintf(yyout, "\tbx");
        break;
    case BL:
        fprintf(yyout, "\tbl");
        break;
    }
    PrintCond();
    fprintf(yyout, " ");
    this->use_list[0]->output();
    fprintf(yyout, "\n");
}

CmpMInstruction::CmpMInstruction(MachineBlock *p,
                                 MachineOperand *src1, MachineOperand *src2,
                                 int cond)
{
    // TODO
    this->parent = p;
    p->setCond(cond);
    this->use_list.push_back(src1);
    this->use_list.push_back(src2);
    src1->setParent(this);
    src2->setParent(this);
    this->type = MachineInstruction::CMP;
    this->op = -1;
    this->cond = cond;
}

void CmpMInstruction::output()
{
    // TODO
    // Just for reg alloca test
    // delete it after test
    fprintf(yyout, "\tcmp ");
    this->use_list[0]->output();
    fprintf(yyout, ", ");
    this->use_list[1]->output();
    fprintf(yyout, "\n");
}

StackMInstruction::StackMInstruction(MachineBlock *p, int op,
                                     std::vector<MachineOperand *> extra_src,
                                     MachineOperand *src,
                                     MachineOperand *lr,
                                     int cond)
{
    this->type = MachineInstruction::STACK;
    this->parent = p;
    this->op = op;
    this->cond = cond;
    if (!extra_src.empty())
    {
        for (auto it = extra_src.begin(); it != extra_src.end(); it++)
        {
            this->use_list.push_back(*it);
        }
    }
    use_list.push_back(src);
    src->setParent(this);
    if (lr)
    {
        this->use_list.push_back(lr);
        lr->setParent(this);
    }
}

void StackMInstruction::output()
{
    switch (op)
    {
    case PUSH:
        fprintf(yyout, "\tpush ");
        break;
    case POP:
        fprintf(yyout, "\tpop ");
        break;
    }
    fprintf(yyout, "{");
    this->use_list[0]->output();
    for (long unsigned int i = 1; i < use_list.size(); i++)
    {
        fprintf(yyout, ", ");
        this->use_list[i]->output();
    }
    fprintf(yyout, "}\n");
}

MachineFunction::MachineFunction(MachineUnit *p, SymbolEntry *sym_ptr)
{
    this->parent = p;
    this->sym_ptr = sym_ptr;
    this->stack_size = 0;
    this->paramsNum = ((FunctionType *)(sym_ptr->getType()))->getParamsSe().size();
};

void MachineBlock::output()
{
    int offset = (parent->getSavedRegs().size() + 2) * 4;
    MachineFunction *cur_func = this->parent;
    int num = cur_func->getParamsNum();
    bool flag = true;
    if (inst_list.empty())
    {
        return;
    }
    fprintf(yyout, ".L%d:\n", this->no);
    for (auto it = inst_list.begin(); it != inst_list.end(); it++)
    {
        if ((*it)->isBX())
        {
            auto fp = new MachineOperand(MachineOperand::REG, 11);
            auto lr = new MachineOperand(MachineOperand::REG, 14);
            auto cur_inst =
                new StackMInstruction(this, StackMInstruction::POP,
                                      parent->getSavedRegs(), fp, lr);
            cur_inst->output();
        }
        if (num > 4 && (*it)->isStore())
        {
            MachineOperand *operand = (*it)->getUse()[0];
            if (operand->isReg() && operand->getReg() == 3)
            {
                if (flag)
                {
                    flag = false;
                }
                else
                {
                    auto fp = new MachineOperand(MachineOperand::REG, 11);
                    auto r3 = new MachineOperand(MachineOperand::REG, 3);
                    auto off =
                        new MachineOperand(MachineOperand::IMM, offset);
                    offset += 4;
                    auto cur_inst = new LoadMInstruction(this, r3, fp, off);
                    cur_inst->output();
                }
            }
        }
        // inst is 'add inst'
        // add sp, sp, [x], x needed => AllocSpace
        if ((*it)->isBinary() && (*it)->getOp() == 0)
        {
            auto dst = (*it)->getDef()[0];
            auto src1 = (*it)->getUse()[0];
            if (dst->isReg() && dst->getReg() == 13 && src1->isReg() &&
                src1->getReg() == 13 && (*(it + 1))->isBX())
            {
                int size = parent->AllocSpace(0);
                if (size < -255 || size > 255)
                {
                    auto r1 = new MachineOperand(MachineOperand::REG, 1);
                    auto off =
                        new MachineOperand(MachineOperand::IMM, size);
                    (new LoadMInstruction(nullptr, r1, off))->output();
                    (*it)->getUse()[1]->setReg(1);
                }
                else
                    (*it)->getUse()[1]->setVal(size);
            }
        }
        (*it)->output();
    }
}

std::vector<MachineOperand *> MachineFunction::getSavedRegs()
{
    std::vector<MachineOperand *> regs;
    for (std::set<int>::iterator it = saved_regs.begin(); it != saved_regs.end(); it++)
    {
        MachineOperand *reg = new MachineOperand(MachineOperand::REG, *it);
        regs.push_back(reg);
    }
    return regs;
}

void MachineFunction::output()
{
    const char *func_name = this->sym_ptr->toStr().c_str() + 1;
    fprintf(yyout, "\t.global %s\n", func_name);
    fprintf(yyout, "\t.type %s , %%function\n", func_name);
    fprintf(yyout, "%s:\n", func_name);
    // TODO
    /* Hint:
     *  1. Save fp
     *  2. fp = sp
     *  3. Save callee saved register
     *  4. Allocate stack space for local variable */
    MachineOperand *fp = new MachineOperand(MachineOperand::REG, 11);
    MachineOperand *sp = new MachineOperand(MachineOperand::REG, 13);
    MachineOperand *lr = new MachineOperand(MachineOperand::REG, 14);
    (new StackMInstruction(nullptr, StackMInstruction::PUSH, getSavedRegs(), fp, lr))->output();
    (new MovMInstruction(nullptr, MovMInstruction::MOV, fp, sp))->output();
    int offset = AllocSpace(0);
    auto size = new MachineOperand(MachineOperand::IMM, offset);
    if (offset < -255 || offset > 255)
    {
        auto r4 = new MachineOperand(MachineOperand::REG, 4);
        (new LoadMInstruction(nullptr, r4, size))->output();
        (new BinaryMInstruction(nullptr, BinaryMInstruction::SUB, sp, sp, r4))
            ->output();
    }
    else
    {
        (new BinaryMInstruction(nullptr, BinaryMInstruction::SUB, sp, sp, size))
            ->output();
    }

    // Traverse all the block in block_list to print assembly code.
    for (auto iter : block_list)
    {
        iter->output();
    }
}

void MachineUnit::insertGlob(SymbolEntry *se)
{
    global_list.push_back(se);
}

void MachineUnit::PrintGlobalDecl()
{
    // TODO:
    // You need to print global variable/const declarition code;
    if (!global_list.empty())
        fprintf(yyout, "\t.data\n");
    for (long unsigned int i = 0; i < global_list.size(); i++)
    {
        IdentifierSymbolEntry *se = (IdentifierSymbolEntry *)global_list[i];
        if (se->isZero())
        {
            IdentifierSymbolEntry *se1 = (IdentifierSymbolEntry *)global_list[i];
            if (se1->getType()->isArray())
            {
                fprintf(yyout, "\t.comm %s, %d, 4\n", se1->toStr().c_str(),
                        se1->getType()->getSize() / 8);
            }
        }
        else
        {
            fprintf(yyout, "\t.global %s\n", se->toStr().c_str());
            fprintf(yyout, "\t.align 4\n");
            fprintf(yyout, "\t.size %s, %d\n", se->toStr().c_str(),
                    se->getType()->getSize() / 8);
            fprintf(yyout, "%s:\n", se->toStr().c_str());
            if (!se->getType()->isIntArray())
            {
                fprintf(yyout, "\t.word %d\n", se->getIntValue());
            }
            else
            {
                int n = se->getType()->getSize() / 32;
                int *p = se->getIntArrValue();
                for (int i = 0; i < n; i++)
                {
                    fprintf(yyout, "\t.word %d\n", p[i]);
                }
            }
        }
    }
}

void MachineUnit::output()
{
    // TODO
    /* Hint:
     * 1. You need to print global variable/const declarition code;
     * 2. Traverse all the function in func_list to print assembly code;
     * 3. Don't forget print bridge label at the end of assembly code!! */
    fprintf(yyout, "\t.arch armv8-a\n");
    fprintf(yyout, "\t.arch_extension crc\n");
    fprintf(yyout, "\t.arm\n");

    // 1.Glob/Decl
    PrintGlobalDecl();
    fprintf(yyout, "\t.text\n");

    // 2.Traverse
    for (auto iter : func_list)
        iter->output();

    // 3.bridge label
    fprintf(yyout, "\n");
    for (auto s : global_list)
    {
        IdentifierSymbolEntry *se = (IdentifierSymbolEntry *)s;
        fprintf(yyout, "addr_%s%d:\n", se->toStr().c_str(), zone);
        fprintf(yyout, "\t.word %s\n", se->toStr().c_str());
    }
}
