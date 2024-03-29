#include <algorithm>
#include "LinearScan.h"
#include "MachineCode.h"
#include "LiveVariableAnalysis.h"

LinearScan::LinearScan(MachineUnit *unit)
{
    this->unit = unit;
    for (int i = 4; i < 11; i++)
        regs.push_back(i);
}

void LinearScan::allocateRegisters()
{
    for (auto &f : unit->getFuncs())
    {
        func = f;
        bool success;
        success = false;
        while (!success) // repeat until all vregs can be mapped
        {
            computeLiveIntervals();
            success = linearScanRegisterAllocation();
            if (success) // all vregs can be mapped to real regs
                modifyCode();
            else // spill vregs that can't be mapped to real regs
                genSpillCode();
            // fprintf(stderr, "end\n");
        }
    }
}

void LinearScan::makeDuChains()
{
    LiveVariableAnalysis lva;
    lva.pass(func);
    du_chains.clear();
    int i = 0;
    std::map<MachineOperand, std::set<MachineOperand *>> liveVar;
    for (auto &bb : func->getBlocks())
    {
        liveVar.clear();
        for (auto &t : bb->getLiveOut())
            liveVar[*t].insert(t);
        int no;
        no = i = bb->getInsts().size() + i;
        for (auto inst = bb->getInsts().rbegin(); inst != bb->getInsts().rend(); inst++)
        {
            (*inst)->setNo(no--);
            for (auto &def : (*inst)->getDef())
            {
                if (def->isVReg())
                {
                    auto &uses = liveVar[*def];
                    du_chains[def].insert(uses.begin(), uses.end());
                    auto &kill = lva.getAllUses()[*def];
                    std::set<MachineOperand *> res;
                    set_difference(uses.begin(), uses.end(), kill.begin(), kill.end(), inserter(res, res.end()));
                    liveVar[*def] = res;
                }
            }
            for (auto &use : (*inst)->getUse())
            {
                if (use->isVReg())
                    liveVar[*use].insert(use);
            }
        }
    }
}

void LinearScan::computeLiveIntervals()
{
    makeDuChains();
    intervals.clear();
    for (auto &du_chain : du_chains)
    {
        int t = -1;
        for (auto &use : du_chain.second)
            t = std::max(t, use->getParent()->getNo());
        Interval *interval = new Interval({du_chain.first->getParent()->getNo(), t, false, 0, 0, {du_chain.first}, du_chain.second});
        intervals.push_back(interval);
    }
    for (auto& interval : intervals) {
        auto uses = interval->uses;
        auto begin = interval->start;
        auto end = interval->end;
        for (auto block : func->getBlocks()) {
            auto liveIn = block->getLiveIn();
            auto liveOut = block->getLiveOut();
            bool in = false;
            bool out = false;
            for (auto use : uses)
                if (liveIn.count(use)) {
                    in = true;
                    break;
                }
            for (auto use : uses)
                if (liveOut.count(use)) {
                    out = true;
                    break;
                }
            if (in && out) {
                begin = std::min(begin, (*(block->begin()))->getNo());
                end = std::max(end, (*(block->getInsts().rbegin()))->getNo());
            } else if (!in && out) {
                for (auto i : block->getInsts())
                    if (i->getDef().size() > 0 &&
                        i->getDef()[0] == *(uses.begin())) {
                        begin = std::min(begin, i->getNo());
                        break;
                    }
                end = std::max(end, (*(block->getInsts().rbegin()))->getNo());
            } else if (in && !out) {
                begin = std::min(begin, (*(block->begin()))->getNo());
                int temp = 0;
                for (auto use : uses)
                    if (use->getParent()->getParent() == block)//machine inst 类没有getParent方法
                        temp = std::max(temp, use->getParent()->getNo());
                end = std::max(temp, end);
            }
        }
        interval->start = begin;
        interval->end = end;
    }
    bool change;
    change = true;
    while (change)
    {
        change = false;
        std::vector<Interval *> t(intervals.begin(), intervals.end());
        for (size_t i = 0; i < t.size(); i++)
            for (size_t j = i + 1; j < t.size(); j++)
            {
                Interval *w1 = t[i];
                Interval *w2 = t[j];
                if (**w1->defs.begin() == **w2->defs.begin())
                {
                    std::set<MachineOperand *> temp;
                    set_intersection(w1->uses.begin(), w1->uses.end(), w2->uses.begin(), w2->uses.end(), inserter(temp, temp.end()));
                    if (!temp.empty())
                    {
                        change = true;
                        w1->defs.insert(w2->defs.begin(), w2->defs.end());
                        w1->uses.insert(w2->uses.begin(), w2->uses.end());
                        // w1->start = std::min(w1->start, w2->start);
                        // w1->end = std::max(w1->end, w2->end);
                        auto w1Min = std::min(w1->start, w1->end);
                        auto w1Max = std::max(w1->start, w1->end);
                        auto w2Min = std::min(w2->start, w2->end);
                        auto w2Max = std::max(w2->start, w2->end);
                        w1->start = std::min(w1Min, w2Min);
                        w1->end = std::max(w1Max, w2Max);
                        auto it = std::find(intervals.begin(), intervals.end(), w2);
                        if (it != intervals.end())
                            intervals.erase(it);
                    }
                }
            }
    }
    sort(intervals.begin(), intervals.end(), compareStart);
}

bool LinearScan::linearScanRegisterAllocation()
{
    bool success = true;
    // Todo
    regs.clear();
    for (int i = 4; i < 11; i++)
    regs.push_back(i);
    active.clear();
    for(auto &itv:intervals) {
        expireOldIntervals(itv);//回收结束时间早于itv开始时间的区间的物理寄存器
        if(regs.empty()) {
            //物理寄存器分配完了，需要spill
            spillAtInterval(itv);
            success = false;//一旦有溢出，就要反复遍历直到全部可分配物理寄存器
        } else{
            //还有没分配的物理寄存器可以分配给区间
            itv->rreg = regs.front();
            regs.erase(regs.begin());
            active.push_back(itv);
            //将active列表按结束时间排序
            sort(active.begin(), active.end(), compareEnd);
        }
    }
    return success;
}

void LinearScan::modifyCode()
{
    // fprintf(stderr, "LinearScan: modifyCode modify\n");
    for (auto &interval : intervals)
    {
        func->addSavedRegs(interval->rreg);
        for (auto def : interval->defs)
        {
            // fprintf(stderr, "modify 1 def reg %d", def->getReg());
            def->setReg(interval->rreg);
            // fprintf(stderr, ", new reg %d\n", def->getReg());
        }
        for (auto use : interval->uses)
            use->setReg(interval->rreg);
    }
}


void LinearScan::genSpillCode()
{
    for(auto &interval:intervals)
    {
        if(!interval->spill)
            continue;
        // TODO
        /* HINT:
         * The vreg should be spilled to memory.
         * 1. insert ldr inst before the use of vreg
         * 2. insert str inst after the def of vreg
         */ 
        interval->disp = - func->AllocSpace(4);
        auto offset = new MachineOperand(MachineOperand::IMM, interval->disp);
        auto fp = new MachineOperand(MachineOperand::REG, 11);
        for(auto use:interval->uses) {
            auto load_operand = new MachineOperand(*use);
            auto u_block = use->getParent()->getParent();
            MachineOperand* operand = nullptr;
            if (interval->disp > 255 || interval->disp < -255) {
                operand = new MachineOperand(MachineOperand::VREG,
                                             SymbolTable::getLabel());
                auto inst = new LoadMInstruction(u_block,
                                                  operand, offset);
                use->getParent()->insertBefore(inst);
                inst = new LoadMInstruction(u_block, load_operand,
                                         fp, new MachineOperand(*operand));
                use->getParent()->insertBefore(inst);
            } else {
                auto inst = new LoadMInstruction(u_block,
                                    load_operand, fp, offset);
                use->getParent()->insertBefore(inst);
            }
        }
        for (auto def : interval->defs) {
            auto store_operand = new MachineOperand(*def);
            MachineBlock* d_block = def->getParent()->getParent();
            MachineOperand* operand = nullptr;
            MachineInstruction *inst1 = nullptr, *inst = nullptr;
            if (interval->disp > 255 || interval->disp < -255) {
                operand = new MachineOperand(MachineOperand::VREG,
                                             SymbolTable::getLabel());
                inst1 = new LoadMInstruction(d_block,
                                             operand, offset);
                def->getParent()->insertAfter(inst1);
                inst =
                    new StoreMInstruction(d_block, store_operand,
                                          fp, new MachineOperand(*operand));
            }
            else
                inst = new StoreMInstruction(d_block,
                                             store_operand, fp, offset);
            if (inst1)
                inst1->insertAfter(inst);
            else
                def->getParent()->insertAfter(inst);
        }
    }
}

void LinearScan::expireOldIntervals(Interval *interval)
{
    auto it = active.begin();
    while(true) {
        if(it == active.end() || (*it)->end >= interval->start) return ;
        regs.push_back((*it)->rreg);// 回收
        auto victim = it;
        it = active.erase(victim);
        sort(regs.begin(), regs.end());
    }
}

void LinearScan::spillAtInterval(Interval *interval)
{
    //1. 为其在栈内分配空间，获取当前在栈内相对 FP 的偏移；
    //2. 遍历其 USE 指令的列表，在 USE 指令前插入 LoadMInstruction，将其从栈内加载到目前的虚拟寄存器中;
    //3. 遍历其 DEF 指令的列表，在 DEF 指令后插入 StoreMInstruction，将其从目前的虚拟寄存器中存到栈内;
    //此函数需要判断是否需要溢出
    auto victim = active.back();//已按结束时间排序
    if(compareEnd(victim,interval)) {
        interval->spill = true;
    } else {
        victim->spill = true;
        interval->rreg = victim->rreg;
        active.push_back(interval);
        sort(active.begin(),active.end(),compareEnd);
    }
}


bool LinearScan::compareStart(Interval *a, Interval *b)
{
    return a->start < b->start;
}

bool LinearScan::compareEnd(Interval *a, Interval *b)
{
    return a->end < b->end;
}