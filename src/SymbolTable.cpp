#include "SymbolTable.h"
#include <iostream>
#include <sstream>
#include "Type.h"

SymbolEntry::SymbolEntry(Type *type, int kind)
{
    this->type = type;
    this->kind = kind;
}

Type *SymbolEntry::getType(bool getRet)
{
    /*if (getRet && this->isVariable() && (dynamic_cast<IdentifierSymbolEntry *>(this))->isFunc())
    {
        return dynamic_cast<FunctionType *>(dynamic_cast<IdentifierSymbolEntry *>(this)->getType())->getRetType();
    }*/
    // functiontype
    return type;
}

int SymbolEntry::getIntValue() const
{
    return intValue;
}

float SymbolEntry::getFloatValue() const
{
    return floatValue;
}

ConstantSymbolEntry::ConstantSymbolEntry(Type *type, int value) : SymbolEntry(type, SymbolEntry::CONSTANT)
{
    this->intValue = value;
}

ConstantSymbolEntry::ConstantSymbolEntry(Type *type, float value) : SymbolEntry(type, SymbolEntry::CONSTANT)
{
    this->floatValue = value;
}

std::string ConstantSymbolEntry::toStr()
{
    std::ostringstream buffer;
    if (type->isIntLike())
        buffer << intValue;
    else if (type->isFloatLike())
        buffer << floatValue;
    else
        fprintf(stderr, "ERROR\n");
    return buffer.str();
}

IdentifierSymbolEntry::IdentifierSymbolEntry(Type *type, std::string name, int scope, int paramNo) : SymbolEntry(type, SymbolEntry::VARIABLE), name(name), paramNo(paramNo)
{
    this->scope = scope;
    this->isconst = false;
    this->isarr = false;
    this->isfunc = false;

    addr = nullptr;
}

std::string IdentifierSymbolEntry::toStr()
{
    std::ostringstream buffer;
    if (label < 0)
    {
        if (type->isFunc())
            buffer << '@';
        buffer << name;
    }
    else
        buffer << "%t" << label;
    return buffer.str();
}

TemporarySymbolEntry::TemporarySymbolEntry(Type *type, int label) : SymbolEntry(type, SymbolEntry::TEMPORARY)
{
    this->label = label;
}

std::string TemporarySymbolEntry::toStr()
{
    std::ostringstream buffer;
    buffer << "%t" << label;
    return buffer.str();
}

SymbolTable::SymbolTable()
{
    prev = nullptr;
    level = 0;
}

SymbolTable::SymbolTable(SymbolTable *prev)
{
    this->prev = prev;
    this->level = prev->level + 1;
}

/*
    Description: lookup the symbol entry of an identifier in the symbol table
    Parameters:
        name: identifier name
    Return: pointer to the symbol entry of the identifier

    hint:
    1. The symbol table is a stack. The top of the stack contains symbol entries in the current scope.
    2. Search the entry in the current symbol table at first.
    3. If it's not in the current table, search it in previous ones(along the 'prev' link).
    4. If you find the entry, return it.
    5. If you can't find it in all symbol tables, return nullptr.
*/
SymbolEntry *SymbolTable::lookup(std::string name)
{
    // map<string, SymbolEntry*>, 使用find找表项, 若没找到就prev, 直到level=0
    SymbolTable *t = this;
    for (int level = this->level; level >= 0; level--)
    {
        // 从t中找symbolTable
        auto iter = t->symbolTable.find(name);
        if (iter != t->symbolTable.end())
        {
            // find it!
            return iter->second;
        }
        else
        {
            t = t->prev;
        }
    }
    return nullptr;
}

SymbolEntry *SymbolTable::localLookup(std::string name)
{
    // 只在当前域内查找
    SymbolTable *t = this;
    auto iter = t->symbolTable.find(name);
    if (iter != t->symbolTable.end())
    {
        // find it!
        return iter->second;
    }
    return nullptr;
}

// install the entry into current symbol table.
void SymbolTable::install(std::string name, SymbolEntry *entry)
{
    symbolTable[name] = entry;
}

int SymbolTable::counter = 0;
static SymbolTable t;
SymbolTable *identifiers = &t;
SymbolTable *globals = &t;
