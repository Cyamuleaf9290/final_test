#ifndef __TYPE_H__
#define __TYPE_H__
#include <vector>
#include <string>
#include <SymbolTable.h>

class Type
{
private:
    int kind;

public:
    int size;

protected:
    enum
    {
        VOID,
        BOOL,
        FUNC,
        PTR,
        INT,
        CONSTINT,
        INTARRAY,
        CONSTINTARRAY,
        FLOAT,
        CONSTFLOAT,
        FLOATARRAY,
        CONSTFLOATARRAY
    };

public:
    Type(int kind, int size = 0) : kind(kind), size(size){};
    virtual ~Type(){};
    virtual std::string toStr() = 0;
    bool isIntLike() const { return kind >= INT && kind < FLOAT; }
    bool isFloatLike() const { return kind >= FLOAT; };
    bool isInt() const { return kind == INT || kind == CONSTINT; };
    bool isConstInt() const { return kind == CONSTINT; };
    bool isFloat() const { return kind == FLOAT; };
    bool isConstFloat() const { return kind == CONSTFLOAT; };
    bool isVoid() const { return kind == VOID; };
    bool isFunc() const { return kind == FUNC; };
    bool isPtr() const { return kind == PTR; };
    bool isArray() const { return isIntArray() || isFloatArray(); };
    bool isIntArray() const { return kind == INTARRAY; };
    bool isFloatArray() const { return kind == FLOATARRAY; };
    bool isConstIntArray() const { return kind == CONSTINTARRAY; };
    bool isConstFloatArray() const { return kind == CONSTFLOATARRAY; };
    bool isBool() const { return kind == BOOL; };
    int getSize() const { return size; };
};

class BoolType : public Type
{
public:
    BoolType() : Type(Type::BOOL, 1){};
    std::string toStr();
};

class IntType : public Type
{
private:
public:
    IntType(int size) : Type(Type::INT, size){};
    std::string toStr();
};

class ConstIntType : public Type
{
private:
public:
    ConstIntType(int size) : Type(Type::CONSTINT, size){};
    std::string toStr();
};

class FloatType : public Type
{
private:
public:
    FloatType(int size) : Type(Type::FLOAT, size){};
    std::string toStr();
};

class ConstFloatType : public Type
{
private:
public:
    ConstFloatType(int size) : Type(Type::CONSTFLOAT, size){};
    std::string toStr();
};

class VoidType : public Type
{
public:
    VoidType() : Type(Type::VOID){};
    std::string toStr();
};

class FunctionType : public Type
{
private:
    Type *returnType;
    std::vector<Type *> params;
    std::vector<SymbolEntry *> paramsSe;

public:
    FunctionType(Type *retType,
                 std::vector<Type *> params,
                 std::vector<SymbolEntry *> paramsSe) : Type(Type::FUNC),
                                                        returnType(retType),
                                                        params(params),
                                                        paramsSe(paramsSe){};
    Type *getRetType() { return returnType; };
    void setParams(std::vector<Type *> params) { this->params = params; };
    void setParamsSe(std::vector<SymbolEntry *> se_vec) { this->paramsSe = se_vec; };
    std::vector<Type *> getParams() { return params; };
    std::vector<SymbolEntry *> getParamsSe() { return paramsSe; };
    std::string toStr();
};

class PointerType : public Type
{
private:
    Type *valueType;

public:
    PointerType(Type *valueType) : Type(Type::PTR) { this->valueType = valueType; };
    std::string toStr();
    Type *getType() const { return valueType; };
};

class IntArrayType : public Type
{
private:
    std::vector<int> dims; // 记录每一维的长度
public:
    IntArrayType(std::vector<int> dims) : Type(Type::INTARRAY), dims(dims){};
    // IntArrayType() : Type(Type::INTARRAY) {};
    void addDims(int val) { dims.push_back(val); };
    int getTotalLen()
    {
        int sum = 1;
        for (auto it = dims.begin(); it != dims.end(); it++)
        {
            sum *= (*it);
        }
        return sum;
    }
    std::vector<int> getDims() { return dims; };
    std::string toStr();
};

class ArrayType : public Type
{
private:
    Type *elementType;
    Type *arrayType = nullptr;
    int length;
    bool constant;

public:
    ArrayType(Type *elementType, int length, bool constant = false)
        : Type(Type::INTARRAY),
          elementType(elementType),
          length(length),
          constant(constant)
    {
        size = elementType->getSize() * length;
    };
    std::string toStr();
    int getLength() const { return length; };
    Type *getElementType() const { return elementType; };
    void setArrayType(Type *arrayType) { this->arrayType = arrayType; };
    bool isConst() const { return constant; };
    Type *getArrayType() const { return arrayType; };
};

class FloatArrayType : public Type
{
private:
    std::vector<int> dims;

public:
    FloatArrayType(std::vector<int> dims) : Type(Type::FLOATARRAY), dims(dims){};
    void addDims(int val) { dims.push_back(val); };
    std::vector<int> getDims() { return dims; };
    std::string toStr();
};

class ConstIntArrayType : public Type
{
private:
    std::vector<int> dims;

public:
    ConstIntArrayType(std::vector<int> dims) : Type(Type::CONSTINTARRAY), dims(dims){};
    void addDims(int val) { dims.push_back(val); };
    std::vector<int> getDims() { return dims; };
    std::string toStr();
};

class ConstFloatArrayType : public Type
{
private:
    std::vector<int> dims;

public:
    ConstFloatArrayType(std::vector<int> dims) : Type(Type::CONSTFLOATARRAY), dims(dims){};
    void addDims(int val) { dims.push_back(val); };
    std::vector<int> getDims() { return dims; };
    std::string toStr();
};

class TypeSystem
{
private:
    static IntType commonInt;
    static IntType commonBool;
    static VoidType commonVoid;
    static FloatType commonFloat;
    static PointerType commonPointer;
    static ConstIntType commonConstInt;
    static ConstFloatType commonConstFloat;

public:
    static Type *intType;
    static Type *voidType;
    static Type *boolType;
    static Type *floatType;
    static Type *pointerType;
    static Type *constIntType;
    static Type *constFloatType;
};

#endif
