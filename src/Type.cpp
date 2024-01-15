#include "Type.h"
#include <sstream>

IntType TypeSystem::commonInt = IntType(32);
IntType TypeSystem::commonBool = IntType(1);
FloatType TypeSystem::commonFloat = FloatType(32);
ConstIntType TypeSystem::commonConstInt = ConstIntType(32);
ConstFloatType TypeSystem::commonConstFloat = ConstFloatType(32);
VoidType TypeSystem::commonVoid = VoidType();

Type *TypeSystem::intType = &commonInt;
Type *TypeSystem::voidType = &commonVoid;
Type *TypeSystem::boolType = &commonBool;
Type *TypeSystem::floatType = &commonFloat;
Type *TypeSystem::constIntType = &commonConstInt;
Type *TypeSystem::constFloatType = &commonConstFloat;

std::string IntType::toStr()
{
    std::ostringstream buffer;
    buffer << "i" << size;
    return buffer.str();
}

std::string ConstIntType::toStr()
{
    std::ostringstream buffer;
    buffer << "i" << size;
    return buffer.str();
}

std::string FloatType::toStr()
{
    std::ostringstream buffer;
    buffer << "f" << size;
    return buffer.str();
}

std::string ConstFloatType::toStr()
{
    std::ostringstream buffer;
    buffer << "f" << size;
    return buffer.str();
}

std::string VoidType::toStr()
{
    return "void";
}

std::string FunctionType::toStr()
{
    std::ostringstream buffer;
    buffer << returnType->toStr() << "(";
    int cnt = params.size();
    fprintf(stderr, "size= %d\n", cnt);
    for (int i = 0; i < cnt - 1; ++i)
    {
        buffer << params[i]->toStr() << ",";
    }
    if (cnt > 0)
        buffer << params[cnt - 1]->toStr();
    buffer << ")";
    return buffer.str();
}

std::string PointerType::toStr()
{
    std::ostringstream buffer;
    buffer << valueType->toStr() << "*";
    return buffer.str();
}

std::string ArrayType::toStr()
{
    std::vector<std::string> vec;
    Type *temp = this;
    int count = 0;
    bool flag = false;
    while (temp && temp->isArray())
    {
        std::ostringstream buffer;
        if (((ArrayType *)temp)->getLength() == -1)
        {
            flag = true;
        }
        else
        {
            buffer << "[" << ((ArrayType *)temp)->getLength() << " x ";
            count++;
            vec.push_back(buffer.str());
        }
        temp = ((ArrayType *)temp)->getElementType();
    }
    assert(temp->isInt());
    std::ostringstream buffer;
    for (auto it = vec.begin(); it != vec.end(); it++)
        buffer << *it;
    buffer << "i32";
    while (count--)
        buffer << ']';
    if (flag)
        buffer << '*';
    return buffer.str();
}

std::string IntArrayType::toStr()
{
    std::ostringstream buffer;
    int i = 0;
    buffer << "int array ";
    for (std::vector<int>::iterator iter = dims.begin(); iter != dims.end(); iter++)
    {
        buffer << "  dim" << i << ":" << *iter;
        i++;
    }
    return buffer.str();
}

std::string FloatArrayType::toStr()
{
    std::ostringstream buffer;
    int i = 0;
    buffer << "float array ";
    for (std::vector<int>::iterator iter = dims.begin(); iter != dims.end(); iter++)
    {
        buffer << "  dim" << i << ":" << *iter;
        i++;
    }
    return buffer.str();
}

std::string ConstIntArrayType::toStr()
{
    std::ostringstream buffer;
    int i = 0;
    buffer << "const int array ";
    for (std::vector<int>::iterator iter = dims.begin(); iter != dims.end(); iter++)
    {
        buffer << "  dim" << i << ":" << *iter;
        i++;
    }
    return buffer.str();
}

std::string ConstFloatArrayType::toStr()
{
    std::ostringstream buffer;
    int i = 0;
    buffer << "const float array ";
    for (std::vector<int>::iterator iter = dims.begin(); iter != dims.end(); iter++)
    {
        buffer << "  dim" << i << ":" << *iter;
        i++;
    }
    return buffer.str();
}
