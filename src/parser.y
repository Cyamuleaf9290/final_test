%code top{
    #include <iostream>
    #include <assert.h>
    #include "parser.h"
    #include <cstring>
    #include <stack>
    extern Ast ast;

    // for typecheck
    Type *latestType;   //由于vardef看不到type，我们保留一下当前最后一次词法分析出的type
    std::string lastid;
    // for array
    ArrayType* arrayType;
    int arrayIdx;
    int* arrayValue;
    int leftbrace = 0;
    std::stack<InitValExpr*> stk;
    InitValExpr* top;
    // for while stmt
    int is_in_while = 0;
    std::stack<StmtNode*> whileStack;
    // for function
    int glob_param_cnt = 0;

    int yylex();
    int yyerror( char const * );
    Type* getMaxType(ExprNode *expr1, ExprNode *expr2)
    {
        Type* t1;
        Type* t2;
        t1 = expr1->getSymPtr()->getType(true);
        t2 = expr2->getSymPtr()->getType(true);
        if(t1->isVoid() || t2->isVoid()) {
            fprintf(stderr,"operator can not be void type.\n");
            exit(EXIT_FAILURE);
        }
        if(t1->isFloatLike()|| t2->isFloatLike()) return TypeSystem::floatType;
        return TypeSystem::intType;
    }
}

%code requires {
    #include "Ast.h"
    #include "SymbolTable.h"
    #include "Type.h"
}

%union {
    int itype;
    float ftype;
    char* strtype;
    StmtNode* stmttype;
    ExprNode* exprtype;
    Type* type;
    SymbolEntry* se;
}

%start Program
%token <strtype> ID
%token <itype> INTEGER
%token <ftype> FLOAT FLOATS
%token IF ELSE WHILE
%token INT VOID
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON LBRACKET RBRACKET COMMA
%token ADD SUB MUL DIV MOD OR AND LT LTE GT GTE ASSIGN EQUAL NEQUAL NOT UNKNOWN
%token CONST
%token RETURN BREAK CONTINUE

%nterm <stmttype> Stmts Stmt AssignStmt BlockStmt IfStmt WhileStmt BreakStmt ContinueStmt ReturnStmt DeclStmt FuncDef VarDef VarDefList FuncFParams FuncFParam ExprStmt BlankStmt
%nterm <exprtype> Exp AddExp Cond LOrExp PrimaryExp LVal RelExp EqExp LAndExp InitVal InitValList MulExp UnaryExp FuncRParams ArrayIndices FuncArrayIndices
%nterm <type> Type

%precedence THEN
%precedence ELSE
%%
Program
    : Stmts {
        ast.setRoot($1);
    }
    ;
Stmts
    : Stmt {$$=$1;}
    | Stmts Stmt{
        $$ = new SeqNode($1, $2);
    }
    ;
Stmt
    : AssignStmt {$$=$1;}
    | BlockStmt {$$=$1;}
    | IfStmt {$$=$1;}
    | WhileStmt {$$=$1;}
    | BreakStmt {$$=$1;}
    | ContinueStmt {$$=$1;}
    | ReturnStmt {$$=$1;}
    | DeclStmt {$$=$1;}
    | ExprStmt {$$=$1;}
    | BlankStmt {$$=$1;}
    | FuncDef {$$=$1;}
    ;
LVal
    : ID {
        // fprintf(stderr,"LVal => ID\n");
        SymbolEntry *se;
        se = identifiers->lookup($1);
        if(se == nullptr)
        {
            fprintf(stderr, "identifier \"%s\" is undefined\n", (char*)$1);
            delete [](char*)$1;
            exit(EXIT_FAILURE);
        }
        $$ = new Id(se);
        // const int N = 3, a[N+1][2]={}，需要修改lval表达式的值
        if(se->getType()->isConstInt())
            $$->setIntValue(se->getIntValue());
        delete []$1;
    }
    | ID ArrayIndices { //数组左值
        SymbolEntry *se;
        se = identifiers->lookup($1);
        if(se == nullptr)
        {
            fprintf(stderr, "identifier \"%s\" is undefined\n", (char*)$1);
            delete [](char*)$1;
            exit(EXIT_FAILURE);
        }
        $$ = new Id(se, $2);
        $$->setIntValue(se->getIntValue());
        delete []$1;
    }
    ;
AssignStmt
    :
    LVal ASSIGN Exp SEMICOLON {
        // fprintf(stderr,"AssignStmt : LVal ASSIGN Exp SEMICOLON\n");
        if($1->getSymPtr()->getType()->isConstInt() || $1->getSymPtr()->getType()->isConstFloat()) {
            fprintf(stderr,"You can't assign a constant.\n");
            exit(EXIT_FAILURE);
        }
        // 判断exp是否为void函数
        if($3->getSymPtr()->isVariable() && (dynamic_cast<IdentifierSymbolEntry*>($3->getSymPtr()))->isFunc()){
            Type *retType = dynamic_cast<FunctionType*>(dynamic_cast<IdentifierSymbolEntry *>($3->getSymPtr())->getType(true));
            if(retType->isVoid()) {
                fprintf(stderr,"right val can not be a void func call\n");
                exit(EXIT_FAILURE);
            } else  {
                // 给左值赋予函数调用的返回值，这是在运行时才能决定的。TODO
            }
        } /*else {    
            // exp为标识符或数值或数组，数组暂不考虑
            if ($1->getSymPtr()->getType()->isInt()) {
                if($3->getSymPtr()->getType()->isConstFloat() || $3->getSymPtr()->getType()->isFloat()) {
                    int val = $3->getSymPtr()->getFloatValue();
                    (dynamic_cast<IdentifierSymbolEntry*>($1->getSymPtr()))->setIntValue(val);
                } else (dynamic_cast<IdentifierSymbolEntry*>($1->getSymPtr()))->setIntValue($3->getSymPtr()->getIntValue());
            } else {
                if($3->getSymPtr()->getType()->isConstFloat() || $3->getSymPtr()->getType()->isFloat()) {
                    //(dynamic_cast<IdentifierSymbolEntry*>($1->getSymPtr()))->setFloatValue($3->getSymPtr()->getFloatValue());
                } else {
                    //float val = (float)($3->getSymPtr()->getIntValue());
                    //(dynamic_cast<IdentifierSymbolEntry*>($1->getSymPtr()))->setFloatValue(val);
                }
            }  
        }*/
        $1->setIntValue($3->getIntValue());
        $$ = new AssignStmt($1, $3);
    }
    ;
BlockStmt
    :   LBRACE 
        {identifiers = new SymbolTable(identifiers);} 
        Stmts RBRACE 
        {
            $$ = new CompoundStmt($3);
            SymbolTable *top = identifiers;
            identifiers = identifiers->getPrev();
            delete top;
        }
    | LBRACE RBRACE {
        $$ = new EmptyStmt();
    }
    ;
IfStmt
    : IF LPAREN Cond RPAREN Stmt %prec THEN {
        $$ = new IfStmt($3, $5);
    }
    | IF LPAREN Cond RPAREN Stmt ELSE Stmt {
        $$ = new IfElseStmt($3, $5, $7);
    }
    ;
WhileStmt
    : WHILE LPAREN Cond RPAREN {
        // fprintf(stderr,"WhileStmt : WHILE LPAREN Cond RPAREN\n");
        is_in_while ++;
        // pass through
        $<stmttype>$ = new WhileStmt($3);
        whileStack.push($<stmttype>$);
    }Stmt {
        $$ = $<stmttype>5;
        dynamic_cast<WhileStmt*>($$)->setStmt($6);
        whileStack.pop();
        is_in_while --;
    }
    ;
BreakStmt
    : BREAK SEMICOLON {
        if(is_in_while)
            $$ = new BreakStmt(whileStack.top());
        else {
            fprintf(stderr,"break statement not in while statemrnt.\n");
            exit(EXIT_FAILURE);
        }
    }
    ;
ContinueStmt
    : CONTINUE SEMICOLON {
        if(is_in_while)
            $$ = new ContinueStmt(whileStack.top());
        else {
            fprintf(stderr,"continue statement not in while statemrnt.\n");
            exit(EXIT_FAILURE);
        }
    }
    ;
ReturnStmt
    :
    RETURN Exp SEMICOLON{
        // fprintf(stderr,"ReturnStmt : RETURN Exp SEMICOLON.\n");
        $$ = new ReturnStmt($2);
    }
    | RETURN SEMICOLON {
        $$ = new ReturnStmt(); 
    }
    ;

ExprStmt
    : Exp SEMICOLON {
        $$ = new ExprStmt($1);
    }
BlankStmt
    : SEMICOLON {
        $$ = new BlankStmt();
    }
    ;
Exp
    :
    AddExp {
        // fprintf(stderr,"Exp : AddExp\n");
        $$ = $1;
    }
    ;
Cond
    :
    LOrExp {
        // fprintf(stderr,"Cond : LOrExp\n");
        $$ = $1;
        $$->setCond();
    }
    ;
PrimaryExp
    :
    LVal {
        // fprintf(stderr,"PrimaryExp => LVal!\n");
        $$ = $1;
    }
    | INTEGER {
        // fprintf(stderr,"PrimaryExp => INTEGER!:%d\n",$1);
        SymbolEntry *se = new ConstantSymbolEntry(TypeSystem::constIntType, $1);
        se->setIntValue($1);
        $$ = new Constant(se);
        $$->setConst();
        $$->setIntValue($1);
    }
    | FLOATS {
        // fprintf(stderr,"PrimaryExp => FLOATS!:%f\n",$1);
        SymbolEntry *se = new ConstantSymbolEntry(TypeSystem::constFloatType, $1);
        $$ = new Constant(se);
        $$->setConst();
        $$->setFloatValue($1);
    }
    | LPAREN Exp RPAREN {
        $$ = $2;
    }
    ;
AddExp
    :
    MulExp {
        // fprintf(stderr,"AddExp : MulExp \n");
        $$ = $1;
    }
    |
    AddExp ADD MulExp
    {
        // fprintf(stderr,"AddExp : AddExp ADD MulExp \n");
        SymbolEntry *se;
        Type* resType = getMaxType($1,$3);
        se = new TemporarySymbolEntry(resType, SymbolTable::getLabel());
        if($1->isConst() && $3->isConst())
            $$->setConst();
        $$ = new BinaryExpr(se, BinaryExpr::ADD, $1, $3);
    }
    |
    AddExp SUB MulExp
    {
        SymbolEntry *se;
        Type* resType = getMaxType($1,$3);
        se = new TemporarySymbolEntry(resType, SymbolTable::getLabel());
        if($1->isConst() && $3->isConst())
            $$->setConst();
        $$ = new BinaryExpr(se, BinaryExpr::SUB, $1, $3);
    }
    ;

MulExp
    : UnaryExp {
        // fprintf(stderr,"MulExp : UnaryExp.\n");
        $$ = $1;
    }
    | MulExp MUL UnaryExp {
        SymbolEntry *se;
        //判断UnaryExp：标识符，数值，函数调用
        Type* resType = getMaxType($1,$3);
        se = new TemporarySymbolEntry(resType, SymbolTable::getLabel());
        if($1->isConst() && $3->isConst())
            $$->setConst();
        $$ = new BinaryExpr(se, BinaryExpr::MUL, $1, $3);
        
    }
    | MulExp DIV UnaryExp {
        SymbolEntry *se;
        Type* resType = getMaxType($1,$3);
        se = new TemporarySymbolEntry(resType, SymbolTable::getLabel());
        if($1->isConst() && $3->isConst())
            $$->setConst();
        $$ = new BinaryExpr(se, BinaryExpr::DIV, $1, $3); 
    }
    | MulExp MOD UnaryExp {
        SymbolEntry *se;
        Type* resType = getMaxType($1,$3);
        se = new TemporarySymbolEntry(resType, SymbolTable::getLabel());
        if($1->isConst() && $3->isConst())
            $$->setConst();
        $$ = new BinaryExpr(se, BinaryExpr::MOD, $1, $3); 
    }
    ;

UnaryExp
    : PrimaryExp {  //id   , 1,  1.23 constant
        // fprintf(stderr,"UnaryExp => PrimaryExp.\n");
        /*SymbolEntry *se;
        if($1->getSymPtr()->getType()->isInt() || $1->getSymPtr()->getType()->isConstInt())
            se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        else if($1->getSymPtr()->getType()->isFloat() || $1->getSymPtr()->getType()->isConstFloat())
            se = new TemporarySymbolEntry(TypeSystem::floatType, SymbolTable::getLabel());
        $$ = new UnaryExpr(se, UnaryExpr::POS, $1);*/
        $$ = $1;
    }
    | ID LPAREN FuncRParams RPAREN {
        // fprintf(stderr,"UnaryExp => ID LPAREN FuncRPamras RPAREN.\n");
        // 检查函数id是否被注册过
        SymbolEntry* se;
        se = identifiers->lookup($1);
        if(se == nullptr) {
            fprintf(stderr,"function did not defined.\n");
            exit(EXIT_FAILURE);
        }
        //检查函数的参数类型是否正确     func(32, max(a,b));
        ExprNode *node = $3;
        if(se->getType()->isFunc()) {
            std::vector<Type*> params = dynamic_cast<FunctionType*>(se->getType())->getParams();
            for(size_t i = 0; i < params.size(); i ++) {
                if(node->getSymPtr()->getType()->isFunc()){
                    if(!dynamic_cast<FunctionType* >(node->getSymPtr()->getType())->getRetType()->isIntLike()){
                        fprintf(stderr,"function params type not match.\n");
                        exit(EXIT_FAILURE);
                    }
                }
                else if(node == nullptr || !node->getSymPtr()->getType()->isIntLike()) {
                    fprintf(stderr,"function params type not match.\n");
                    exit(EXIT_FAILURE);
                }
                node = dynamic_cast<ExprNode*>(node->getNext());
            }
        }
        if(dynamic_cast<FunctionType* >(se->getType())->getRetType() == TypeSystem::voidType){
            fprintf(stderr,"parser: void ret type \n");
        }
        else{
            fprintf(stderr,"parser: NOT void ret type %s\n", se->getType(true)->toStr().c_str());
        }
        if($3) $$ = new FuncCallExpr(se, $3);
        else $$ = new FuncCallExpr(se);
        fprintf(stderr,"func call\n");
    }
    | ADD UnaryExp {
        // fprintf(stderr,"UnaryExp : ADD UnaryExp.\n");
        SymbolEntry *se;
        if(!$2->isConst()){
        /*if((dynamic_cast<UnaryExpr*>($2))->getOP() == UnaryExpr::NOT) {
            $2->setType(TypeSystem::boolType);
            se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        } else {
            if($2->getSymPtr()->getType()->isInt() || $2->getSymPtr()->getType()->isConstInt())
                se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
            else if($2->getSymPtr()->getType()->isFloat() || $2->getSymPtr()->getType()->isConstFloat())
                se = new TemporarySymbolEntry(TypeSystem::floatType, SymbolTable::getLabel());
        }*/
            se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
            $$ = new UnaryExpr(se, UnaryExpr::POS, $2);
        }
        else{
            se = $2->getSymPtr();
            $$ = new UnaryExpr(se, UnaryExpr::POS, $2);
            $$->setConst();
        }
        $$->setUnary();
    }
    | SUB UnaryExp { // -2 -a -(a+b-c)
        // fprintf(stderr,"UnaryExp : SUB UnaryExp.\n");
        SymbolEntry *se;
        if(!$2->isConst()){
        /*if(((dynamic_cast<UnaryExpr*>($2)))->getOP() == UnaryExpr::NOT) {
            $2->setType(TypeSystem::boolType);
            se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        } else {
            if($2->getSymPtr()->getType()->isInt() || $2->getSymPtr()->getType()->isConstInt())
                se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
            else if($2->getSymPtr()->getType()->isFloat() || $2->getSymPtr()->getType()->isConstFloat())
                se = new TemporarySymbolEntry(TypeSystem::floatType, SymbolTable::getLabel());
        }*/
            se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
            $$ = new UnaryExpr(se, UnaryExpr::NEG, $2);
        }
        else{
            se = $2->getSymPtr();
            se->setIntValue(-se->getIntValue());
            $$ = new UnaryExpr(se, UnaryExpr::NEG, $2);
            $$->setIntValue(-se->getIntValue());
            $$->setConst();
        }
        $$->setUnary();
    }
    | NOT UnaryExp {
        // fprintf(stderr,"UnaryExp : NOT UnaryExp.\n");
        /*
        if((dynamic_cast<UnaryExpr*>($2))->getOP() == UnaryExpr::NOT) {
            $2->setType(TypeSystem::boolType);
        }
        SymbolEntry *se;
        if($2->getSymPtr()->getType()->isInt())
            se = new TemporarySymbolEntry(TypeSystem::intType, SymbolTable::getLabel());
        else if($2->getSymPtr()->getType()->isFloat())
            se = new TemporarySymbolEntry(TypeSystem::floatType, SymbolTable::getLabel());*/
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new UnaryExpr(se, UnaryExpr::NOT, $2);
        $$->setBool();
        $$->setUnary();
    }
    ;
FuncRParams
    : Exp {$$ =$1;/*函数实参TODO*/} 
    | Exp COMMA FuncRParams {
        $$ = $1;
        $1->setNext($3);
    }
    | %empty {$$ = nullptr;}
    ;
RelExp
    : AddExp {
        $$ = $1;
    }
    | RelExp LT AddExp {    //<
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::LT, $1, $3);
    }
    | RelExp LTE AddExp {   //<=
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::LTE, $1, $3);
    }
    | RelExp GT AddExp {    //>
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::GT, $1, $3);
    }
    | RelExp GTE AddExp {   //>=
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::GTE, $1, $3);
    }
    ;
EqExp
    : RelExp {$$ = $1;}
    | EqExp EQUAL RelExp {   //==
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::EQUAL, $1, $3);
    }
    | EqExp NEQUAL RelExp {    //!=
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::NEQUAL, $1, $3);
    }
    ;
LAndExp
    :
    EqExp {
        // fprintf(stderr,"LAndExp : EqExp\n");
        $$ = $1;}
    |
    LAndExp AND EqExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::AND, $1, $3);
    }
    ;
LOrExp
    :
    LAndExp {
        // fprintf(stderr,"LOrExp : LAndExp\n");
        $$ = $1;}
    |
    LOrExp OR LAndExp
    {
        SymbolEntry *se = new TemporarySymbolEntry(TypeSystem::boolType, SymbolTable::getLabel());
        $$ = new BinaryExpr(se, BinaryExpr::OR, $1, $3);
    }
    ;
Type
    : INT {
        $$ = TypeSystem::intType;
        latestType = TypeSystem::intType;
    }
    | VOID {
        $$ = TypeSystem::voidType;
        latestType = TypeSystem::voidType;
    }
    | FLOAT {
        $$ = TypeSystem::floatType;
        latestType = TypeSystem::floatType;
    }
    | CONST INT {
        $$ = TypeSystem::constIntType;
        latestType = TypeSystem::constIntType;
    }
    | CONST FLOAT {
        $$ = TypeSystem::constFloatType;
        latestType = TypeSystem::constFloatType;
    }
    ;
DeclStmt
    : Type VarDefList SEMICOLON {
        $$ = $2;
    }
    ;

VarDefList
    : VarDef COMMA VarDefList {
        //fprintf(stderr,"Vardeflist set next:%s\n",((DeclStmt*)$3->getidName()).c_str());
        $$ = $1;
        $1->setNext($3);
    }
    | VarDef {
        $$ = $1;
    }
    ;

//Todo::const int a; 这样的声明，由于a是常量，但初始化时也没赋值，后续也无法赋值。 
VarDef
    : ID {
        // fprintf(stderr, "VarDef => ID\n");
        if(identifiers->localLookup($1)) {
            fprintf(stderr, "identifier \"%s\" is already defined\n", (char*)$1);
            exit(EXIT_FAILURE);
        }
        IdentifierSymbolEntry *se = new IdentifierSymbolEntry(latestType, $1, identifiers->getLevel());
        identifiers->install($1, se);   //防止重复声明
        $$ = new DeclStmt($1,new Id(se));
        delete []$1;
    }
    | ID ASSIGN InitVal {   //int a = 1; b func()
        if(identifiers->localLookup($1)) {
            fprintf(stderr, "identifier \"%s\" is already defined\n", (char*)$1);
            exit(EXIT_FAILURE);
        }
        fprintf(stderr, "ID ASSIGN\n");
        IdentifierSymbolEntry *se = new IdentifierSymbolEntry(latestType, $1, identifiers->getLevel());
        if($3->getSymPtr()->isVariable() && (dynamic_cast<IdentifierSymbolEntry*>($3->getSymPtr()))->isFunc()){
            Type *retType = dynamic_cast<FunctionType*>(dynamic_cast<IdentifierSymbolEntry *>($3->getSymPtr())->getType(true));
            if(retType->isVoid()) {
                fprintf(stderr,"right val can not be a void func call\n");
                exit(EXIT_FAILURE);
            } else  {
                //给左值赋予函数调用的返回值，这是在运行时才能决定的。TODO
            }
        }
        /*else {    //exp为标识符或数值或数组，数组暂不考虑
            if(latestType == TypeSystem::intType || latestType == TypeSystem::constintType) {
                if($3->getSymPtr()->getType()->isConstFloat() || $3->getSymPtr()->getType()->isFloat()) {
                    int val = $3->getSymPtr()->getFloatValue();
                    se->setIntValue(val);
                } else se->setIntValue($3->getSymPtr()->getIntValue());
            }else {
                if($3->getSymPtr()->getType()->isConstFloat() || $3->getSymPtr()->getType()->isFloat()) {
                    se->setFloatValue($3->getSymPtr()->getFloatValue());
                } else {
                    float val = (float)($3->getSymPtr()->getIntValue());
                    se->setFloatValue(val);
                }
            }
            if (latestType == TypeSystem::constintType || latestType == TypeSystem::constfloatType) {
                se->setConst();//标识这个标识符为常量
            }  
        }*/
        se->setIntValue($3->getSymPtr()->getIntValue());
        identifiers->install($1, se);
        $$ = new DeclStmt($1,new Id(se),$3);
        dynamic_cast<DeclStmt*>($$)->getID()->getSymPtr()->setIntValue(se->getIntValue());
        dynamic_cast<DeclStmt*>($$)->getID()->setIntValue(se->getIntValue());
        delete []$1;  
    }
    | ID ArrayIndices {
        // int a[10];
        if(identifiers->localLookup($1)) {
            fprintf(stderr, "identifier \"%s\" is already defined\n", (char*)$1);
            exit(EXIT_FAILURE);
        }
        ExprNode *node = $2;
        std::vector<int> dimsize;
        while(node) {
            dimsize.push_back(node->getSymPtr()->getIntValue());
            node = node->getNext();
        }
        Type *type = TypeSystem::intType;
        if(dimsize.size()>1){
            while(!dimsize.empty()){
                type = new ArrayType(type, dimsize.back());
                dimsize.pop_back();
            }
        }else{
            type = new ArrayType(type, dimsize.back());
        }
        /*        
            Type* temp;
            while(!dimsize.empty()){
            temp = new ArrayType(type, dimsize.back());
            if(type->isArray())
                ((ArrayType*)type)->setArrayType(temp);
            type = temp;
            dimsize.pop_back();
        }*/
        arrayType = (ArrayType*)type;
        SymbolEntry* se = new IdentifierSymbolEntry(type, $1, identifiers->getLevel());
        ((IdentifierSymbolEntry*)se)->setZero();
        int *p = new int[type->getSize()];
        ((IdentifierSymbolEntry*)se)->setIntArrValue(p);
        identifiers->install($1, se);
        $$ = new DeclStmt($1, new Id(se));
        delete []$1;
    }
    | ID ArrayIndices ASSIGN {
        if(identifiers->localLookup($1)) {
            fprintf(stderr, "identifier \"%s\" is already defined\n", (char*)$1);
            exit(EXIT_FAILURE);
        }
        ExprNode *node = $2;
        std::vector<int> dimsize;
        while(node) {
            //fprintf(stderr, "array indices: val = %d\n", node->getIntValue());
            dimsize.push_back(node->getIntValue());
            node = node->getNext();
        }
        Type* type = TypeSystem::intType;
        if(dimsize.size()>1){
            while(!dimsize.empty()){
                type = new ArrayType(type, dimsize.back());
                dimsize.pop_back();
            }
        }else{
            type = new ArrayType(type, dimsize.back());
        }

        arrayType = (ArrayType*)type;
        SymbolEntry *se = new IdentifierSymbolEntry(type, $1, identifiers->getLevel());
        (dynamic_cast<IdentifierSymbolEntry*>(se))->setArr();
        if(latestType == TypeSystem::constIntType || latestType == TypeSystem::constFloatType)
            (dynamic_cast<IdentifierSymbolEntry*>(se))->setConst();

        arrayIdx = 0;
        std::stack<InitValExpr*>().swap(stk);
        se = new IdentifierSymbolEntry(type, $1, identifiers->getLevel());
        $<se>$ = se;
        // 初始化列表不为空，用全局变量存
        arrayValue = new int[arrayType->getSize()];
    } InitVal {
        ((IdentifierSymbolEntry*)$<se>4)->setIntArrValue(arrayValue);
        if(((InitValExpr*)$5)->isEmpty())
            ((IdentifierSymbolEntry*)$<se>4)->setZero();
        if(identifiers->localLookup($1)) {
            fprintf(stderr, "identifier \"%s\" is already defined\n", (char*)$1);
            exit(EXIT_FAILURE);
        }
        identifiers->install($1, $<se>4);  
        $$ = new DeclStmt($1, new Id($<se>4), $5);
        delete []$1;
    }
    ;

ArrayIndices
    : LBRACKET Exp RBRACKET {
        // fprintf(stderr, "ArrayIndices : LBRACKET Exp RBRACKET\n");
        if($2->getSymPtr()->getType()->isConstInt() || $2->getSymPtr()->getType()->isInt()) {
            $$ = $2;
        }else if($2->getSymPtr()->getType()->isArray()){
            $$ = $2;
        }
        else{
            fprintf(stderr,"array index must be int. %s\n", $2->getSymPtr()->getType()->toStr().c_str());
            exit(EXIT_FAILURE);
        }
        //fprintf(stderr, "array Indices exp val = %d\n", $2->getIntValue());
    }
    | LBRACKET Exp RBRACKET ArrayIndices{
        // fprintf(stderr, "ArrayIndices : LBRACKET Exp RBRACKET ArrayIndices\n");
        if($2->getSymPtr()->getType()->isConstInt() || $2->getSymPtr()->getType()->isInt()) {
            $$ = $2;
            $2->setNext($4);
        }else {
            fprintf(stderr,"array index must be int.\n");
            exit(EXIT_FAILURE);
        }
        //fprintf(stderr, "array Indices exp val = %d\n", $2->getIntValue());
    }
    ;
InitVal
    : Exp {
        // fprintf(stderr,"InitVal : Exp \n");
        $$ = $1;
        if(!stk.empty()){
            // todo:float
            arrayValue[arrayIdx++] = $1->getIntValue();
            Type* arrType = stk.top()->getSymPtr()->getType();
            if(arrType == TypeSystem::intType){
                stk.top()->addExpr($1);
            }
            else
                while(arrType){
                    // 栈顶是个数组类型，并且element不是int，可能是{2,3,4...，👉exp}加到一半了，push
                    if(((ArrayType*)arrType)->getElementType() != TypeSystem::intType){
                        arrType = ((ArrayType*)arrType)->getElementType();
                        // 数组的se都是constant吗，确实
                        SymbolEntry* se = new ConstantSymbolEntry(arrType, 0);
                        InitValExpr* list = new InitValExpr(se);
                        stk.top()->addExpr(list);
                        stk.push(list);
                    // element是int，不push
                    }else{
                        // 单独的数作为exp
                        stk.top()->addExpr($1);
                        // 是否满，更换arrTy，pop
                        while(stk.top()->isFull() && stk.size() != (long unsigned int)leftbrace){
                            arrType = ((ArrayType*)arrType)->getArrayType();
                            stk.pop();
                        }
                        break;
                    }
                }
        }    
    }
    | LBRACE RBRACE {
        // fprintf(stderr,"InitVal : {}  \n");
        SymbolEntry* se;
        ExprNode* list;
        if(stk.empty()){
            // a={}，所以se是全零，initvalexpr包含符号表项
            memset(arrayValue, 0, arrayType->getSize());
            arrayIdx += arrayType->getSize() / TypeSystem::intType->getSize();
            se = new ConstantSymbolEntry(arrayType, 0);
            list = new InitValExpr(se);
        }else{
            Type* type = ((ArrayType*)(stk.top()->getSymPtr()->getType()))->getElementType();
            int len = type->getSize() / TypeSystem::intType->getSize();
            memset(arrayValue + arrayIdx, 0, type->getSize());
            arrayIdx += len;
            se = new ConstantSymbolEntry(type, 0);
            list = new InitValExpr(se);
            // = {0}
            stk.top()->addExpr(list);
            while(stk.top()->isFull() && stk.size() != (long unsigned int)leftbrace){
                stk.pop();
            }
        }
        $$ = list;
    }
    | LBRACE {
        // fprintf(stderr,"InitVal : { InitValList }  \n");
        SymbolEntry* se;
        if(!stk.empty()){
            // 当前的类型
            arrayType = (ArrayType*)(((ArrayType*)(stk.top()->getSymPtr()->getType()))->getElementType());
        }
        se = new ConstantSymbolEntry(arrayType, 0);
        if(arrayType->getElementType() != TypeSystem::intType){
            arrayType = (ArrayType*)(arrayType->getElementType());
        }
        InitValExpr* expr = new InitValExpr(se);
        // {...{}}
        if(!stk.empty())
            stk.top()->addExpr(expr);
        stk.push(expr);
        $<exprtype>$ = expr;
        leftbrace++;
    } InitValList RBRACE {
        leftbrace--;        
        // pop至当前维
        while(stk.top() != $<exprtype>2 && stk.size() > (long unsigned int)(leftbrace + 1))
            stk.pop();
        if(stk.top() == $<exprtype>2)
            stk.pop();
        $$ = $<exprtype>2;
        // pop至非满
        if(!stk.empty())
            while(stk.top()->isFull() && stk.size() != (long unsigned int)leftbrace){
                stk.pop();
            }
        int size = ((ArrayType*)($$->getSymPtr()->getType()))->getSize()/ TypeSystem::intType->getSize();
        // 填0
        while(arrayIdx % size != 0)
            arrayValue[arrayIdx++] = 0;
        // 全局变量更新
        if(!stk.empty())
            arrayType = (ArrayType*)(((ArrayType*)(stk.top()->getSymPtr()->getType()))->getElementType());
    }
    ;

InitValList
    : InitVal {
        $$ = $1;
    }
    | InitValList COMMA InitVal {
        $$ = $1;
        // 加不加都行
        //$1->setNext($3);
    }
    ;

FuncDef
    :
    Type ID LPAREN {
        // fprintf(stderr, "FuncDef => Type ID LPAREN FuncFParams RPAREN\n");
        // 无需实现函数重载，只用检查id是否被注册过
        SymbolEntry *se1 = identifiers->lookup($2);
        if(se1 != nullptr) {
            fprintf(stderr,"funtion redefined.\n");
            exit(EXIT_FAILURE);
        }
        glob_param_cnt=0;
        Type *funcType = new FunctionType($1,{},{});
        SymbolEntry *se = new IdentifierSymbolEntry(funcType, $2, identifiers->getLevel());
        identifiers->install($2, se);
        dynamic_cast<IdentifierSymbolEntry*>(se)->setFunc();
        identifiers = new SymbolTable(identifiers);
    }FuncFParams {
        // 参数的scope应在函数作用域内?
        SymbolEntry* se = identifiers->lookup($2);
        assert(se != nullptr);
        DeclStmt* node = dynamic_cast<DeclStmt*>($5);
        std::vector<Type*> vec;
        std::vector<SymbolEntry*> se_vec;
        while(node) {
            vec.push_back(node->getID()->getSymPtr()->getType());
            se_vec.push_back(node->getID()->getSymPtr());
            node = dynamic_cast<DeclStmt*>(node->getNext());
        }
        (dynamic_cast<FunctionType*>(se->getType()))->setParams(vec);
        (dynamic_cast<FunctionType*>(se->getType()))->setParamsSe(se_vec);
    }RPAREN
    BlockStmt
    {
        /*StmtNode *stmt;
        if($8)
            stmt = dynamic_cast<CompoundStmt*>($8)->getStmtInBlock(); //TODO::这句可能会错，因为blockstmt可能为emptystmt
        StmtNode *laststmt;
        if(dynamic_cast<SeqNode*>(stmt) != NULL) laststmt = dynamic_cast<SeqNode*>(stmt)->getStmt(2);
        else laststmt = stmt;*/
        //laststmt此时为函数定义block中的最后一句
        //ReturnStmt *retstmt = dynamic_cast<ReturnStmt*>(laststmt);
        /*if($1->isVoid()) {
            if(retstmt != NULL && retstmt->getRetValue() != nullptr) {
                fprintf(stderr,"void function should not have returnstmt.\n");
                exit(EXIT_FAILURE);
            } else fprintf(stderr,"void function check ok\n");
        } else {
            if(retstmt == NULL) {
                fprintf(stderr,"non-void function should have returnstmt.\n");
                exit(EXIT_FAILURE);
            }
            //getType(true)是因为如果返回语句是类似： return func(); 即函数递归，则要获取的是函数的返回类型
            Type *retType = retstmt->getRetValue()->getSymPtr()->getType(true);
            if(($1->isIntLike() && retType->isFloatLike()) ||($1->isFloatLike()&&retType->isIntLike())){
                fprintf(stderr,"In funcdef : function return type not match.\n");
                exit(EXIT_FAILURE);
            }
            if($1 != retType) {
                fprintf(stderr,"In funcdef : function return type not match.\n");
                exit(EXIT_FAILURE);
            }
        }*/
        SymbolEntry *se;
        se = identifiers->getPrev()->lookup($2);
        assert(se != nullptr);
        $$ = new FunctionDef(se, (DeclStmt*)$5, $8);
        SymbolTable *top = identifiers;
        identifiers = identifiers->getPrev();
        delete top;
        delete []$2;
    }
    ;
//函数形参列表
FuncFParams
    : FuncFParam {
        $$ = $1;
        //fprintf(stderr,"3\n");
    }
    | FuncFParam COMMA FuncFParams {
        $$ = $1;
        $1->setNext($3);
        //fprintf(stderr,"2\n");
    }
    | %empty {$$ = nullptr;}
    ;
//函数形参
FuncFParam
    : Type ID {
        // 对应 int func(int a)的情况
        SymbolEntry *se;
        se = new IdentifierSymbolEntry($1, $2, identifiers->getLevel(), glob_param_cnt++);
        identifiers->install($2, se);
        ((IdentifierSymbolEntry*)se)->setLabel();
        ((IdentifierSymbolEntry*)se)->setAddr(new Operand(se));
        $$ = new DeclStmt($2,new Id(se));
        delete []$2;
        //fprintf(stderr,"1\n");
    }
    | Type ID FuncArrayIndices {
        SymbolEntry* se;
        ExprNode* temp = $3;
        Type* arr = TypeSystem::intType;
        Type* arr1;
        std::stack<ExprNode*> stk;
        while(temp){
            stk.push(temp);
            temp = (ExprNode*)(temp->getNext());
        }
        while(!stk.empty()){
            arr1 = new ArrayType(arr, stk.top()->getIntValue());
            if(arr->isArray())
                ((ArrayType*)arr)->setArrayType(arr1);
            arr = arr1;
            stk.pop();
        }
    
        se = new IdentifierSymbolEntry(arr, $2, identifiers->getLevel(), glob_param_cnt++);
        identifiers->install($2, se);
        ((IdentifierSymbolEntry*)se)->setLabel();
        ((IdentifierSymbolEntry*)se)->setAddr(new Operand(se));
        $$ = new DeclStmt($2, new Id(se));
        delete []$2;
    }
    ;
FuncArrayIndices 
    : LBRACKET RBRACKET {
        // []
        $$ = new ExprNode(nullptr);
    }
    | FuncArrayIndices LBRACKET Exp RBRACKET {
        // [][exp]
        $$ = $1;
        $$->setNext($3);
    }
%%

int yyerror(char const* message)
{
    std::cerr<<message<<std::endl;
    return -1;
}
