%option noyywrap
%option yylineno
%{
    #define YY_NO_UNPUT
    #define YY_NO_INPUT

    #include "parser.h"
    #include <ostream>
    #include <fstream>
    #include <Unit.h>
    #include <SymbolTable.h>
    using namespace std;

    extern FILE *yyin; 
    extern FILE *yyout;
    extern bool dump_tokens;
    extern Unit unit;

    int offset = 0;
    // 二进制转十进制
    int btod(const char *pbin)
    {
    int ii=0;
    int result=0;
        while (pbin[ii] != 0)
        {
            result = result * 2 + (pbin[ii] - '0');
            ii++;
        }
        return result;
    } 
    // 整数转字符串
    string itoa(int num)
    {
        char buf[100]={0};
        sprintf(buf,"%d",num);
        return string(buf);
    } 
    // lab4.词法分析 debug
    void DEBUG_FOR_LAB4(std::string s){
        std::string DEBUG_INFO = s + "\t"+ itoa(yylineno) +"\t"+ itoa(offset) +"\n";
        fputs(DEBUG_INFO.c_str(), yyout);
    }
%}

INT_BINARY (0[b|B][0|1]+)
INT_DEX [1-9][0-9]*|[0]
INT_HEX [0][Xx](([1-9a-fA-F][0-9a-fA-F]*)|[0])
INT_OCT [0][0-7]+
FLOAT ([0-9]|[1-9][0-9]+)[.][0-9][0-9]*[fF]{0,1}
FLOATE ([0-9]|[1-9][0-9]+)[.][0-9][0-9]*[eE][+-]([0-9]|[1-9][0-9]+)
ID [[:alpha:]_][[:alpha:][:digit:]_]*
TAB [\t]
WHITE [ ]
EOL (\r\n|\n|\r)

BLOCKCOMMENTBEGIN "/*"
BLOCKCOMMENTELEMENT .|\n
BLOCKCOMMENTEND "*/"
%x BLOCKCOMMENT
LINECOMMENT \/\/[^\n]* 
%%

"int" {
    offset += 3;
    /*
    * Questions: 
    *   Q1: Why we need to return INT in further labs?
    *   Q2: What is "INT" actually?
    */
    if(dump_tokens)
        DEBUG_FOR_LAB4("INT\tint");
    return INT;
}
"void" {
    offset += 4;
    if(dump_tokens)
        DEBUG_FOR_LAB4("VOID\tvoid");
    return VOID;
}

"float" {
    offset += 5;
    if(dump_tokens)
        DEBUG_FOR_LAB4("FLOAT\tfloat");
    return FLOAT;
}
"const" {
    offset += 5;
    if(dump_tokens)
        DEBUG_FOR_LAB4("CONST\tconst");
    return CONST;
}
"if" {
    offset += 2;
    if(dump_tokens)
        DEBUG_FOR_LAB4("IF\tif");
    return IF;
}
"else" {
    offset += 4;
    if(dump_tokens)
        DEBUG_FOR_LAB4("ELSE\telse");
    return ELSE;
}
"while" {
    offset += 5;
    if(dump_tokens)
        DEBUG_FOR_LAB4("WHILE\twhile");
    return WHILE;
}
"break" {
    offset += 5;
    if(dump_tokens)
        DEBUG_FOR_LAB4("BREAK\tbreak");
    return BREAK;
}
"continue" {
    offset += 8;
    if(dump_tokens)
        DEBUG_FOR_LAB4("CONTINUE\tcontinue");
    return CONTINUE;
}
"return" {
    offset += 6;
    if(dump_tokens)
        DEBUG_FOR_LAB4("RETURN\treturn");
    return RETURN;
}
"!" {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("NOT\t!");
    return NOT;
}
"=" {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("ASSIGN\t=");
    return ASSIGN;
}
"<" {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("LESS\t<");
    return LT;
}
"<=" {
    offset += 2;
    if(dump_tokens)
        DEBUG_FOR_LAB4("LESSEQUAL\t<=");
    return LTE;
}
">" {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("BIGGER\t>");
    return GT;
}
">=" {
    offset += 2;
    if(dump_tokens)
        DEBUG_FOR_LAB4("BIGGEREQUAL\t>=");
    return GTE;
}
"==" {
    offset += 2;
    if(dump_tokens)
        DEBUG_FOR_LAB4("BIGGEREQUAL\t==");
    return EQUAL;
}
"!=" {
    offset += 2;
    if(dump_tokens)
        DEBUG_FOR_LAB4("NOTEQUAL\t!=");
    return NEQUAL;
}
"+" {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("ADD\t+");
    return ADD;
}
"-" {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("SUB\t-");
    return SUB;
}
"*" {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("MUL\t*");
    return MUL;
}
"/" {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("DIV\t/");
    return DIV;
}
"%" {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("MOD\t%%");
    return MOD;
}
";" {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("SEMICOLON\t;");
    return SEMICOLON;
}
"(" {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("LPAREN\t(");
    return LPAREN;
}
")" {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("RPAREN\t)");
    return RPAREN;
}
"{" {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("LBRACE\t{");
    return LBRACE;
}
"}" {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("RBRACE\t}");
    return RBRACE;
}
"[" {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("LBRACKET\t[");
    return LBRACKET;
}
"]" {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("RBRACKET\t]");
    return RBRACKET;
}
"," {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("COMMA\t,");
    return COMMA;
}
"?" {
    offset += 1;
    if(dump_tokens)
        DEBUG_FOR_LAB4("UNKNOWN\t?");
    return UNKNOWN;
}
"&&" {
    offset += 2;
    if(dump_tokens)
        DEBUG_FOR_LAB4("AND\t&&");
    return AND;
}
"||" {
    offset += 2;
    if(dump_tokens)
        DEBUG_FOR_LAB4("OR\t||");
    return OR;
}

{INT_BINARY} {
    // 二进制
    offset+=strlen(yytext);
    memcpy(yytext,yytext+2,strlen(yytext)-1);
    int n = btod(yytext);
    if(dump_tokens){
        char c[1000] = {0};
        sprintf(c,"INT_BINARY\tb%d\t",n);
        DEBUG_FOR_LAB4((string)c);
    }
    yylval.itype = n;
    return INTEGER;
}

{INT_DEX} {
    // 十进制
    offset+=strlen(yytext);
    int n;
    sscanf(yytext,"%d",&n);
    if(dump_tokens){
        char c[1000] = {0};
        sprintf(c,"NUM\t%d\t",n);
        DEBUG_FOR_LAB4((string)c);
    }
    yylval.itype = n;
    return INTEGER;
}

{INT_HEX} {
    // 十六进制
    offset+=strlen(yytext);
    int n;
    sscanf(yytext,"%x",&n);
    if(dump_tokens){
        char c[1000] = {0};
        sprintf(c,"INT_HEX\t0x%x\t",n);
        DEBUG_FOR_LAB4((string)c);
    }
    yylval.itype = n;
    return INTEGER;
}

{INT_OCT} {
    // 八进制
    offset+=strlen(yytext);
    int n;
    sscanf(yytext,"%o",&n);
    if(dump_tokens){
        char c[1000] = {0};
        sprintf(c,"INT_OCT\tO%o\t",n);
        DEBUG_FOR_LAB4((string)c);
    }
    yylval.itype = n;
    return INTEGER;
}

{FLOAT} {
    // 浮点数
    offset+=strlen(yytext);
    float f;
    sscanf(yytext,"%f",&f);
    if(dump_tokens){
        char c[1000] = {0};
        sprintf(c,"FLOATS\t0x%f\t",f);
        DEBUG_FOR_LAB4((string)c);
    }
    yylval.ftype = f;
    return FLOATS;
}

{FLOATE} {
    // 浮点数，科学计数法
    offset+=strlen(yytext);
    float f;
    sscanf(yytext,"%f",&f);
    if(dump_tokens){
        char c[1000] = {0};
        sprintf(c,"FLOATS\t0x%f\t",f);
        DEBUG_FOR_LAB4((string)c);
    }
    yylval.ftype = f;
    return FLOATS;
}

"getint" {
    offset+=strlen(yytext);
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    char *lexeme;
    lexeme = new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    Type* funcType = new FunctionType(TypeSystem::intType,{},{});
    SymbolEntry* se = new IdentifierSymbolEntry(funcType, yytext, globals->getLevel());
    globals->install(yytext, se);
    unit.insertDecl(se);
    return ID;
}

"putint" {
    offset+=strlen(yytext);
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    char *lexeme;
    lexeme = new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    std::vector<Type*> vec;
    vec.push_back(TypeSystem::intType);
    Type* funcType = new FunctionType(TypeSystem::voidType, vec,{});
    SymbolEntry* se = new IdentifierSymbolEntry(funcType, yytext, globals->getLevel());
    globals->install(yytext, se);
    fprintf(stderr, "putint installed\n");
    unit.insertDecl(se);
    return ID;
}

"getch" {
    offset+=strlen(yytext);
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    char *lexeme;
    lexeme = new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    Type* funcType = new FunctionType(TypeSystem::intType,{ },{});
    SymbolEntry* se = new IdentifierSymbolEntry(funcType, yytext, globals->getLevel());
    globals->install(yytext, se);
    unit.insertDecl(se);
    return ID;
}
"putch" {
    offset+=strlen(yytext);
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    char *lexeme;
    lexeme = new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    std::vector<Type*> vec;
    vec.push_back(TypeSystem::intType);
    Type* funcType = new FunctionType(TypeSystem::voidType,vec,{});
    SymbolEntry* se = new IdentifierSymbolEntry(funcType, yytext, globals->getLevel());
    globals->install(yytext, se);
    unit.insertDecl(se);
    return ID;
}


"getarray" {
    offset+=strlen(yytext);
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    char *lexeme;
    lexeme = new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    vector<Type* > params;
    ArrayType* arr = new ArrayType(TypeSystem::intType, -1);
    params.push_back(arr);
    Type* funcType = new FunctionType(TypeSystem::intType,params,{});
    SymbolEntry* se = new IdentifierSymbolEntry(funcType, yytext, globals->getLevel());
    globals->install(yytext, se);
    unit.insertDecl(se);
    return ID;
}

"putarray" {
    offset+=strlen(yytext);
    if(dump_tokens)
        DEBUG_FOR_LAB4(yytext);
    char *lexeme;
    lexeme = new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    std::vector<Type*> params;
    ArrayType* arr = new ArrayType(TypeSystem::intType, -1);  
    params.push_back(arr);
    Type* funcType = new FunctionType(TypeSystem::voidType,params, {});
    SymbolEntry* se = new IdentifierSymbolEntry(funcType, yytext, globals->getLevel());
    globals->install(yytext, se);
    unit.insertDecl(se);
    return ID;
}




{ID} {
    offset += strlen(yytext);
    if(dump_tokens){
        char c[1000] = {0};
        sprintf(c,"ID\t%s\t",yytext);
        DEBUG_FOR_LAB4((string)c);
    }
    char *lexeme;
    lexeme = new char[strlen(yytext) + 1];
    strcpy(lexeme, yytext);
    yylval.strtype = lexeme;
    return ID;
}

{BLOCKCOMMENTBEGIN} {BEGIN BLOCKCOMMENT;}
<BLOCKCOMMENT>{BLOCKCOMMENTELEMENT} {}
<BLOCKCOMMENT>{BLOCKCOMMENTEND} {BEGIN INITIAL;}
{LINECOMMENT}
{EOL} offset = 0;
{TAB} offset += 4;
{WHITE} offset += 1;
%%
