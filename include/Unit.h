#ifndef __UNIT_H__
#define __UNIT_H__

#include <vector>
#include "Function.h"
#include "AsmBuilder.h"

class Unit
{
    typedef std::vector<Function *>::iterator iterator;
    typedef std::vector<Function *>::reverse_iterator reverse_iterator;

private:
    std::vector<SymbolEntry *> decl_list;
    std::vector<SymbolEntry *> glob_list;
    std::vector<Function *> func_list;
public:
    Unit() = default;
    ~Unit();
    void insertFunc(Function *);
    void removeFunc(Function *);
    void insertDecl(SymbolEntry *);
    void removeDecl(SymbolEntry *);
    void insertGlob(SymbolEntry *);
    void removeGlob(SymbolEntry *);
    void output() const;
    void output_glob() const;
    void output_decl() const;
    iterator begin() { return func_list.begin(); };
    iterator end() { return func_list.end(); };
    reverse_iterator rbegin() { return func_list.rbegin(); };
    reverse_iterator rend() { return func_list.rend(); };
    void genMachineCode(MachineUnit* munit);
};

#endif