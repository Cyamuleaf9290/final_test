#ifndef __AST_H__
#define __AST_H__

#include <fstream>
#include "Operand.h"
#include "Type.h"
class SymbolEntry;
class Unit;
class Function;
class BasicBlock;
class Instruction;
class IRBuilder;

class Node
{
private:
    static int counter; // 全局Node计数
    int seq;            // 第几个Node
    Node *next;

protected:
    std::vector<Instruction *> true_list;
    std::vector<Instruction *> false_list;
    static IRBuilder *builder;
    void backPatch(std::vector<Instruction *> &list, BasicBlock *bb);
    std::vector<Instruction *> merge(std::vector<Instruction *> &list1, std::vector<Instruction *> &list2);

public:
    Node();
    int getSeq() const { return seq; };
    void setNext(Node *nextNode) { next = nextNode; };
    Node *getNext() { return next; };
    static void setIRBuilder(IRBuilder *ib) { builder = ib; };
    virtual void output(int level) = 0;
    virtual void typeCheck(Type *retType = nullptr) = 0;
    virtual void genCode() = 0;
    std::vector<Instruction *> &trueList() { return true_list; }
    std::vector<Instruction *> &falseList() { return false_list; }
};

class ExprNode : public Node
{

private:
    bool Const = false;
    bool Bool = false;
    bool Unary = false;
    bool Cond = false;
    bool Primary = false;
    bool Funccall = false;
    bool Float = false;
    bool InitValList = false;

protected:
    int intValue = -1;
    float floatValue;
    int *intArrayValue;
    float *floatArrayValue;
    int arrayValueCnt;
    SymbolEntry *symbolEntry;
    ExprNode *next; // for array
    Operand *dst;   // The result of the subtree is stored into dst.
public:
    ExprNode(SymbolEntry *symbolEntry, bool isCond = false) : Cond(isCond), symbolEntry(symbolEntry){};
    Operand *getOperand() { return dst; };
    SymbolEntry *getSymPtr() { return symbolEntry; };
    void setType(Type *type) { this->getSymPtr()->setType(type); };
    void setIntValue(int value) { intValue = value; };
    int getIntValue() { return intValue; };
    void setFloatValue(int value) { floatValue = value; };
    int getFloatValue() { return floatValue; };
    ExprNode *getNext() { return next; };
    void setNext(ExprNode *next) { this->next = next; };
    int *getIntArrayValue() { return intArrayValue; };
    void setIntArrayValue(int *v) { intArrayValue = v; };
    float *getfloatArrayValue() { return floatArrayValue; };
    void setFloatArrayValue(float *v) { floatArrayValue = v; };
    int getArrayValueCnt() { return arrayValueCnt; };

    void setPrimary() { Primary = true; };
    void setCond() { Cond = true; };
    void setFunccall() { Funccall = true; };
    void setBool() { Bool = true; };
    void setConst() { Const = true; };
    void setUnary() { Unary = true; };
    void setInitValExpr() { InitValList = true; };
    void setFuncCall() { Funccall = true; };
    bool isCondition() { return Cond; };
    bool isInitValExpr() { return InitValList; };
    bool isCond() { return Cond; };
    bool isBool() { return Bool; };
    bool isConst() { return Const; };
    bool isUnary() { return Unary; };
    bool isPrimary() { return Primary; };
    bool isFuncCall() { return Funccall; };

    virtual void output(int level){};
    virtual void typeCheck(Type *retType = nullptr){};
    virtual void genCode(){};
};

class BinaryExpr : public ExprNode
{
private:
    int op;
    ExprNode *expr1, *expr2;

public:
    enum
    {
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        AND,
        OR,
        GT,
        GTE,
        LT,
        LTE,
        EQUAL,
        NEQUAL
    };
    BinaryExpr(SymbolEntry *se, int op, ExprNode *expr1, ExprNode *expr2, bool isCond = false) : ExprNode(se, isCond), op(op), expr1(expr1), expr2(expr2)
    {
        dst = new Operand(se);
        //  常量直接运算，存入expr节点，减少指令
        switch (op)
        {
        case ADD:
            intValue = expr1->getIntValue() + expr2->getIntValue();
            break;
        case SUB:
            intValue = expr1->getIntValue() - expr2->getIntValue();
            break;
        case MUL:
            intValue = expr1->getIntValue() * expr2->getIntValue();
            break;
        case DIV:
            if (expr2->getIntValue() != 0)
                intValue = expr1->getIntValue() / expr2->getIntValue();
            break;
        default:
            break;
        };
    };
    void output(int level);
    void typeCheck(Type *retType = nullptr);
    void genCode();
};

class FuncCallExpr : public ExprNode
{
private:
    ExprNode *params;

public:
    FuncCallExpr(SymbolEntry *se, ExprNode *Params = nullptr) : ExprNode(se)
    {
        setFunccall();
        SymbolEntry *temp = new TemporarySymbolEntry(se->getType(), SymbolTable::getLabel());

        if (dynamic_cast<FunctionType *>(se->getType())->getRetType() == TypeSystem::voidType)
        {
            dst = nullptr;
        }
        else
        {
            dst = new Operand(temp);
        }
        if (Params)
        {
            this->params = Params;
        }
    };
    void output(int level);
    void typeCheck(Type *retType = nullptr);
    void genCode();
};

class UnaryExpr : public ExprNode
{
private:
    int op;
    ExprNode *expr;

public:
    enum
    {
        POS,
        NEG,
        NOT
    };
    UnaryExpr(SymbolEntry *se, int op, ExprNode *expr) : ExprNode(se), op(op), expr(expr)
    {
        dst = new Operand(se);
        switch (op)
        {
        case POS:
            intValue = expr->getIntValue();
            break;
        case NEG:
            intValue = -expr->getIntValue();
            break;
        case NOT:
        {
            // 常数值变成bool类型
            this->setBool();
            intValue = expr->getIntValue() == 0 ? 1 : 0;
            break;
        }
        default:
            break;
        }
        //}
    };
    void output(int level);
    int getOP() const { return op; };
    void typeCheck(Type *retType = nullptr);
    void genCode();
};

class Constant : public ExprNode
{
public:
    Constant(SymbolEntry *se) : ExprNode(se)
    {
        se->setConstant();
        setPrimary();
        dst = new Operand(se);
        intValue = se->getIntValue();
    };
    void output(int level);
    void typeCheck(Type *retType = nullptr);
    void genCode();
};

class Id : public ExprNode
{
private:
    ExprNode *indicesExpr;
    Type *type;
    bool left = false;

public:
    Id(SymbolEntry *se, ExprNode *expr = nullptr) : ExprNode(se), indicesExpr(expr)
    {

        setPrimary();
        if (se)
        {
            type = se->getType();
            if (type->isInt())
            {
                SymbolEntry *temp = new TemporarySymbolEntry(
                    TypeSystem::intType, SymbolTable::getLabel());
                dst = new Operand(temp);
            }
            else if (type->isArray())
            {
                SymbolEntry *temp = new TemporarySymbolEntry(
                    new PointerType(((ArrayType *)type)->getElementType()),
                    SymbolTable::getLabel());
                dst = new Operand(temp);
            }
        }
    };
    void output(int level);
    void typeCheck(Type *retType = nullptr);
    void genCode();
    bool isLeft() const { return left; };
    void setLeft() { left = true; }
};

class InitValExpr : public ExprNode
{
private:
    ExprNode *expr;
    Type *type;
    int exprNum;

public:
    InitValExpr(SymbolEntry *se, ExprNode *expr = nullptr)
        : ExprNode(se), expr(expr)
    {
        setInitValExpr();
        exprNum = 0;
        type = se->getType();
    };
    void output(int level);
    ExprNode *getExpr() const { return expr; };
    void addExpr(ExprNode *expr);
    bool isEmpty() { return exprNum == 0; };
    bool isFull();
    void typeCheck(Type *retType = nullptr);
    void genCode();
    void addZero();
    Type *getType() { return type; };
    int getLen() { return exprNum; };
};

class StmtNode : public Node
{
};

// 这样的语句节点使函数调用可以识别。
class ExprStmt : public StmtNode
{
private:
    ExprNode *expr;

public:
    ExprStmt(ExprNode *expr) : expr(expr){};
    void output(int level);
    void typeCheck(Type *retType = nullptr);
    void genCode();
};

class CompoundStmt : public StmtNode
{
private:
    StmtNode *stmt;

public:
    CompoundStmt(StmtNode *stmt) : stmt(stmt){};
    StmtNode *getStmtInBlock() const { return stmt; };
    void output(int level);
    void typeCheck(Type *retType = nullptr);
    void genCode();
};

class SeqNode : public StmtNode
{
private:
    StmtNode *stmt1, *stmt2;

public:
    SeqNode(StmtNode *stmt1, StmtNode *stmt2) : stmt1(stmt1), stmt2(stmt2){};
    StmtNode *getStmt(int chosen)
    {
        if (chosen == 1)
            return stmt1;
        if (chosen == 2)
            return stmt2;
        else
            return nullptr;
    }
    void output(int level);
    void typeCheck(Type *retType = nullptr);
    void genCode();
};

class DeclStmt : public StmtNode
{
private:
    Id *id;
    std::string idName;
    ExprNode *expr;

public:
    DeclStmt(std::string idName, Id *id, ExprNode *expr = nullptr) : id(id), idName(idName)
    {
        if (expr)
        {
            this->expr = expr;
            if (expr->isInitValExpr())
            {
                ((InitValExpr *)(this->expr))->addZero();
            }
        }
    };
    // 这里的expr的作用是记录这个声明是否有初值
    Id *getID() { return id; };
    ExprNode *getExpr() { return expr; };
    std::string getidName() { return idName; };
    void output(int level);
    void typeCheck(Type *retType = nullptr);
    void genCode();
};

class IfStmt : public StmtNode
{
private:
    ExprNode *cond;
    StmtNode *thenStmt;

public:
    IfStmt(ExprNode *cond, StmtNode *thenStmt) : cond(cond), thenStmt(thenStmt)
    {
        cond->setCond();
    };
    void output(int level);
    void typeCheck(Type *retType = nullptr);
    void genCode();
};

class IfElseStmt : public StmtNode
{
private:
    ExprNode *cond;
    StmtNode *thenStmt;
    StmtNode *elseStmt;

public:
    IfElseStmt(ExprNode *cond, StmtNode *thenStmt, StmtNode *elseStmt) : cond(cond), thenStmt(thenStmt), elseStmt(elseStmt)
    {
        cond->setCond();
    };
    void output(int level);
    void typeCheck(Type *retType = nullptr);
    void genCode();
};

class WhileStmt : public StmtNode
{
private:
    ExprNode *cond;
    StmtNode *stmt;
    BasicBlock *cond_bb;
    BasicBlock *end_bb;

public:
    WhileStmt(ExprNode *cond = nullptr, StmtNode *stmt = nullptr) : cond(cond), stmt(stmt)
    {

        cond->setCond();
    };
    void output(int level); // TODO
    void typeCheck(Type *retType = nullptr);
    void genCode();
    void setStmt(StmtNode *stmt) { this->stmt = stmt; };
    BasicBlock *get_cond_bb() { return this->cond_bb; };
    BasicBlock *get_end_bb() { return this->end_bb; };
};

class BlankStmt : public StmtNode
{
private:
public:
    BlankStmt(){};
    void output(int level);
    void typeCheck(Type *retType = nullptr);
    void genCode();
};

class EmptyStmt : public StmtNode
{
public:
    EmptyStmt(){};
    void output(int level);
    void typeCheck(Type *retType = nullptr);
    void genCode();
};

class BreakStmt : public StmtNode
{
private:
    StmtNode *whileStmt;

public:
    BreakStmt(StmtNode *whileStmt = nullptr) : whileStmt(whileStmt){};
    void output(int level);
    void typeCheck(Type *retType = nullptr);
    void genCode();
};

class ContinueStmt : public StmtNode
{
private:
    StmtNode *whileStmt;

public:
    ContinueStmt(StmtNode *whileStmt = nullptr) : whileStmt(whileStmt){};
    void output(int level);
    void typeCheck(Type *retType = nullptr);
    void genCode();
};

class ReturnStmt : public StmtNode
{
private:
    ExprNode *retValue;

public:
    ReturnStmt(ExprNode *RetValue = nullptr) : retValue(RetValue){};
    ExprNode *getRetValue() const { return retValue; };
    void output(int level);
    void typeCheck(Type *retType = nullptr);
    void genCode();
};

class AssignStmt : public StmtNode
{
private:
    ExprNode *lval;
    ExprNode *expr;

public:
    AssignStmt(ExprNode *lval, ExprNode *expr) : lval(lval), expr(expr){};
    void output(int level);
    void typeCheck(Type *retType = nullptr);
    void genCode();
};

class FunctionDef : public StmtNode
{
private:
    SymbolEntry *se;
    DeclStmt *decl;
    StmtNode *stmt;

public:
    FunctionDef(SymbolEntry *se, DeclStmt *decl, StmtNode *stmt)
        : se(se), decl(decl), stmt(stmt){};
    void output(int level);
    void typeCheck(Type *retType = nullptr);
    SymbolEntry *getSymPtr() { return se; };
    void genCode();
};

class Ast
{
private:
    Node *root;

public:
    Ast() { root = nullptr; }
    void setRoot(Node *n) { root = n; }
    void output();
    void typeCheck(Type *retType = nullptr);
    void genCode(Unit *unit);
};

#endif
