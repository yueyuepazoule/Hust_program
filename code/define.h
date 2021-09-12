#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum token_class
{
    ERROR_TOKEN,IDENT,INT_CONST,FLOAT_CONST,LONG_CONST,CHAR_CONST,
    INT,FLOAT,LONG,CHAR,
    IF,ELSE,ASSIGN,LP,RP,SLP,SRP,
    SEMI,COMMA,
    PLUS,MINUS,MULTI,DIVISION,PLUSPLUS,MOD,
    NOT,BLP,BRP,WHILE,BIGGER,BIGGEREQUAL,SMALLER,SMALLEREQUAL,
    EQ,NO_EQ,BEGIN_END,
    DEFINE,INCLUDE,
    FOR,SWITCH,DEFAULT,CASE,
    OR,AND,
    USEFUN,
    DO,
    GROUP,
    RETURN,
    CONTINUE,
    BREAK,
	ANNO
};

enum node_type
{
    //ext
    extdeflist,
    extvardef,
    extvarlist,
    extvarkind,
    extvardata,
    funcdef,
    funkind,
    formal_parameterlist,
    formal_parameter,
    funname,funbody,
    multisentence,
    //local
    locvardeflist,
    locvardef,
    locvarnamelist,
    sentencelist,
    ifelsesentence,
    ifsentence,
    forsentence,
    switchsentence,
    whilesentence,
    opnum,
    opsign,
    array,
    definesentence,
    includesentence,
    pretree,
    var,
    dowhilesentence,
    continuesentence,
    breaksentence
};

char precede[14][14]=
{
    '>','>','<','<','<','>',' ','>','>','>','>','>','>','>',
    '>','>','<','<','<','>',' ','>','>','>','>','>','>','>',
    '>','>','>','>','<','>',' ','>','>','>','>','>','>','>',
    '>','>','>','>','<','>',' ','>','>','>','>','>','>','>',
    '<','<','<','<','<','=',' ','>','>','>','>','>','>','>',
    '>','>','>','>','<',' ',' ','>','>','>','>','>','>','>',
    '<','<','<','<','<',' ','<','<','<','<','<','<','>','>',
    '<','<','<','<','<','>',' ','>','>','>','>','>','>','>',
    '<','<','<','<','<','>',' ','>','>','>','>','>','>','>',
    '<','<','<','<','<','>',' ','>','>','>','>','>','>','>',
    '<','<','<','<','<','>',' ','>','>','>','>','>','>','>',
    '<','<','<','<','<',' ','<','<','<','<','<','=','>','>',
    '<','<','<','<','<','<','<','<','=','<','<','<','>','>',
    '<','<','<','<','<','<','<','<','=','<','<','<','>','>',
};

typedef struct node
{
    int type;

    union//? if + name then how to use
    {
        /* data */
        struct extdeflist* extdeflistp;
        struct extvardef* extvardefp;
        struct extvarlist* extvarlistp;
        struct funcdef* funcdefp;
        struct funkind* funkindp;
        struct funname* funnamep;
        struct formal_parameterlist* formal_parameterlistp;
        struct formal_parameter* formal_parameterp;
        struct multisentence* multisentencep;
        struct sentencelist* sentencelistp;
        struct locvardeflist* locvardeflistp;
        struct locvardef* locvardefp;
        struct locvarnamelist* locvarnamelistp;
        struct ifelsesentence* ifelsesentencep;
        struct ifsentence* ifsentencep;
        struct opnum* opnump;
        struct opsign* opsignp;
        struct array *arrayp;
        struct forsentence *forsentencep;
        struct whilesentence *whilesentencep;
        struct definesentence* definesentencep;
        struct includesentence* includesentencep;
        struct pretree *pretreep;
        struct var* varp;
        struct dowhilesentence* dowhilesentencep;
        struct continuesentence* continuesentencep;
        struct breaksentence* breaksentencep;
    };
} *Node,Tnode;

struct pretree
{
    Node preinformation;
    Node extdefinformation;
};

struct extdeflist
{
    Node lchild;
    Node rchild;
};//typedef

struct extvardef{

    int kind;
    Node child;
};//var

struct extvarlist
{
    /* data */
    Node child;
    char data[100];
    int isar;
};

struct funcdef
{
    /* data */
    char name[100];
    Node lchild;
    Node rchild;
    Node mchild;

};

struct array
{
    char name[100];
    int type;
    int scale[10];
};

struct funkind
{
    /* data */
    int type;
};

/*
struct funnname
{
    Node child;
    char data[100];
};
*/

struct funbody
{
    /* data */
    Node child;
    char data[100];
};

struct formal_parameterlist
{
    /* data */
    Node lchild;
    Node rchild;

};

struct formal_parameter
{
    /* data */
    int kind;
    char data[100];
};

struct multisentence
{
    /* data */
    Node lchild;
    Node rchild;
};

//

struct locvardeflist
{
    /* data */
    Node lchild;
    Node rchild;
};

struct locvardef
{
    /* data */
    int kind;
    Node child;
};

struct locvarnamelist
{
    /* data */
    Node child;
    char data[100];
    int isar;
};


struct sentencelist
{
    /* data */
    Node lchild;
    Node rchild;
};

struct exp
{
    /* data */
        struct node* child;

};

struct var//返回语句下挂内容
{
    char data[100];
};

struct ifelsesentence
{
    /* data */
    Node condition;
    Node ifresult;
    Node elseresult;
};


struct ifsentence
{
    /* data */
    Node condition;//if(?)
    Node result;//ifif so
};

struct forsentence
{
    Node start;
    Node condition;
    Node change;
    Node sentence;
};

struct whilesentence
{
    /* data */
    Node condition;
    Node sentence;
};
struct dowhilesentence
{
    Node condition;
    Node sentence;
};
struct definesentence
{
	Node child;
	char id[100];
	char data[100];
};//?

struct includesentence
{
    char data[100];
    Node child;
};


/*
struct switchsentence
{
    char data[100];
    Node ccase[100];
    Node ddefault;
};*/

struct opnum
{
    int kind;
    char data[100];
};

struct opsign
{
    /* data */
    int kind;
    Node lchild;
    Node rchild;
};

struct continuesentence
{

};

struct breaksentence
{

};

//var
FILE *fp;//?全局变量
int line = 1;
int errors = 0;
char token_text[1000];
char token_text0[1000];
char text_data[100];
int w;
int cnt = 0; //运算符计数
int cls;
int depth = 0;
Node funlist[100];//记录已出现函数
int funnum = 0;
int ar;

