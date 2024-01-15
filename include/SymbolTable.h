#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__

#include <string>
#include <map>
#include <assert.h>
#include <vector>

class Type;
class Operand;
class SymbolTable;

class SymbolEntry
{
public:
    bool isBoolExpr;

private:
    int kind;

protected:
    enum
    {
        CONSTANT,
        VARIABLE,
        TEMPORARY
    };
    Type *type;
    SymbolEntry *next;
    int intValue;     // 只在constantsymbolentry和identifierentry用到
    float floatValue; // 只在constantsymbolentry和identifierentry用到
public:
    SymbolEntry(Type *type, int kind);
    virtual ~SymbolEntry(){};
    bool isConstant() const { return kind == CONSTANT; };
    void setConstant() { kind = CONSTANT; };
    bool isTemporary() const { return kind == TEMPORARY; };
    bool isVariable() const { return kind == VARIABLE; };
    Type *getType(bool getRet = false);
    void setType(Type *type) { this->type = type; };
    virtual std::string toStr() = 0;
    bool setNext(SymbolEntry *se);
    SymbolEntry *getNext() const { return next; };
    void setIntValue(int value)
    {
        intValue = value;
    };
    int getIntValue() const;
    float getFloatValue() const;
    // You can add any function you need here.
};

// symbol table managing identifier symbol entries
class SymbolTable
{
private:
    std::map<std::string, SymbolEntry *> symbolTable;
    SymbolTable *prev;
    int level;
    static int counter;

public:
    SymbolTable();
    SymbolTable(SymbolTable *prev);
    void install(std::string name, SymbolEntry *entry);
    SymbolEntry *lookup(std::string name);
    SymbolEntry *localLookup(std::string name);
    SymbolTable *getPrev() { return prev; };
    int getLevel() { return level; };
    static int getLabel() { return counter++; };
};

/*
    Symbol entry for literal constant. Example:

    int a = 1;

    Compiler should create constant symbol entry for literal constant '1'.
*/
class ConstantSymbolEntry : public SymbolEntry // 数值符号表项
{
public:
    ConstantSymbolEntry(Type *type, int value);
    ConstantSymbolEntry(Type *type, float value);
    virtual ~ConstantSymbolEntry(){};
    std::string toStr();
    // You can add any function you need here.
};

/*
    Symbol entry for identifier. Example:

    int a;
    int b;
    void f(int c)
    {
        int d;
        {
            int e;
        }
    }

    Compiler should create identifier symbol entries for variables a, b, c, d and e:

    | variable | scope    |
    | a        | GLOBAL   |
    | b        | GLOBAL   |
    | c        | PARAM    |
    | d        | LOCAL    |
    | e        | LOCAL +1 |
*/
class IdentifierSymbolEntry : public SymbolEntry
{
private:
    enum
    {
        GLOBAL,
        PARAM,
        LOCAL
    };
    std::string name;
    int scope;
    int paramNo;
    int *intArrValue = nullptr;
    float *floatArrValue = nullptr;
    bool isarr = false;
    bool zero = false;
    int label = -1;
    bool isconst = false;
    bool isfunc = false;
    Operand *addr; // The address of the identifier.
    // You can add any field you need here.

public:
    IdentifierSymbolEntry(Type *type, std::string name, int scope, int paramNo = -1);
    virtual ~IdentifierSymbolEntry(){};
    std::string toStr();
    bool isGlobal() const { return scope == GLOBAL; };
    bool isParam() const { return scope == PARAM; };
    bool isLocal() const { return scope >= LOCAL; };
    bool isConst() const { return isconst == true; }; // 判断这个id是否为常量
    bool isArr() const { return isarr == true; };
    bool isFunc() const { return isfunc == true; };
    int getScope() const { return scope; };
    void setAddr(Operand *addr) { this->addr = addr; };
    void setIntValue(int value) { intValue = value; };
    void setFloatValue(float value) { floatValue = value; };
    void setIntArrValue(int *intarr) { intArrValue = intarr; };
    int *getIntArrValue() { return intArrValue; };
    void setConst() { isconst = true; };
    void setArr() { isarr = true; };
    void setFunc() { isfunc = true; };
    void setZero() { zero = true; };
    bool isZero() { return zero; };
    Operand *getAddr() { return addr; };
    int getLabel() const { return label; };
    void setLabel() { label = SymbolTable::getLabel(); };
    int getParamNo() { return paramNo; };
    // You can add any function you need here.
};

/*
    Symbol entry for temporary variable created by compiler. Example:

    int a;
    a = 1 + 2 + 3;

    The compiler would generate intermediate code like:

    t1 = 1 + 2
    t2 = t1 + 3
    a = t2

    So compiler should create temporary symbol entries for t1 and t2:

    | temporary variable | label |
    | t1                 | 1     |
    | t2                 | 2     |
*/
class TemporarySymbolEntry : public SymbolEntry
{
private:
    int label;
    int stack_offset;

public:
    TemporarySymbolEntry(Type *type, int label);
    virtual ~TemporarySymbolEntry(){};
    std::string toStr();
    int getLabel() const { return label; };
    void setLabel() { label = SymbolTable::getLabel(); }
    void setOffset(int offset) { this->stack_offset = offset; };
    int getOffset() { return this->stack_offset; };
    // You can add any function you need here.
};

extern SymbolTable *identifiers;
extern SymbolTable *globals;

#endif
