#include "Unit.h"
#include <string>
#include "Ast.h"
#include "SymbolTable.h"
#include "Type.h"

void Unit::insertFunc(Function *f)
{
    func_list.push_back(f);
}

void Unit::removeFunc(Function *func)
{
    func_list.erase(std::find(func_list.begin(), func_list.end(), func));
}

void Unit::insertGlob(SymbolEntry *se)
{
    glob_list.push_back(se);
}

void Unit::removeGlob(SymbolEntry *se)
{
    glob_list.erase(std::find(glob_list.begin(), glob_list.end(), se));
}

void Unit::insertDecl(SymbolEntry *se)
{
    for (auto decl : decl_list)
    {
        if (se->toStr() == decl->toStr())
        {
            return;
        }
    }

    decl_list.push_back(se);
    fprintf(stderr, "%s decl installed\n", se->toStr().c_str());
}

void Unit::removeDecl(SymbolEntry *se)
{
    decl_list.erase(std::find(decl_list.begin(), decl_list.end(), se));
}

void Unit::output() const
{
    output_glob();
    output_decl();
    for (auto &func : func_list)
        func->output();
}

void Unit::output_glob() const
{
    extern FILE *yyout;
    //    std::vector<SymbolEntry *> glob_list;
    for (auto &se : glob_list)
    {
        fprintf(yyout, "%s = global %s %d, align 4\n", se->toStr().c_str(), se->getType()->toStr().c_str(), se->getIntValue());
        fprintf(stderr, "%s = global %s %d, align 4\n", se->toStr().c_str(), se->getType()->toStr().c_str(), se->getIntValue());
    }
}

void Unit::output_decl() const
{
    extern FILE *yyout;
    // fprintf(stderr, "size = %d\n", decl_list.size());
    for (auto se : decl_list)
    {
        FunctionType *type = (FunctionType *)(se->getType());
        std::string str = type->toStr();
        std::string name = str.substr(0, str.find('('));
        std::string param = str.substr(str.find('('));
        fprintf(yyout, "declare %s %s%s\n", type->getRetType()->toStr().c_str(),
                se->toStr().c_str(), param.c_str());
    }
}


void Unit::genMachineCode(MachineUnit* munit) 
{
    AsmBuilder *builder = new AsmBuilder();
    builder->setUnit(munit);
    for (auto &func : func_list)
        func->genMachineCode(builder);
}

Unit::~Unit()
{
    for(auto &func:func_list)
        delete func;
}


