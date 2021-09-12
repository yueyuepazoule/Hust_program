#include "token.h"

typedef struct STACK{
    int top;
    int data[10000];
} S;//int栈

typedef struct NodeStack{
    int top;
    Node data[1000];
} NS;//Node栈

Node CreateNode(int type)
{
	Node p = (Node)malloc(sizeof(Tnode));
	p->type = type;
	return p;
}

void initStack(S &s)
{
    s.top = -1;
    memset(s.data, 0, sizeof(s.data));
}

int isEmpty(S s)
{
    if (s.top == -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int popStack(S &s,int &e)
{
    if(!isEmpty(s))
    {
        e = s.data[s.top];
        s.top--;
        return 1;
    }
    else
    {
        return 0;
    }
}

void pushStack(S &s,int e)
{
    s.data[++s.top] = e;
}

int isLp(S s)
{
    for(int i=s.top;i>=0;i--)
    {
        if(s.data[i]==LP)
        {
            return 1;
        }
    }
    return 0;
}

int getTop(S s)
{
    if(!isEmpty(s))
    {
        return s.data[s.top];
    }
    else
    {
        return -1;//?
    }
}

void initNodeStack(NS &s)
{
    s.top = -1;
    memset(s.data, NULL, sizeof(s.data));//?
}
int isNodeStackEmpty(NS s)
{
    if (s.top == -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int popNodeStack(NS &s,Node e)
{
    if(!isNodeStackEmpty(s))
    {
        e = s.data[s.top];
        s.top--;
        return 1;
    }
    else
    {
        return 0;
    }
}

void pushNodeStack(NS &s,Node e)
{
    s.data[++s.top] = e;
}

Node getNodeStackTop(NS s)
{
    if(!isNodeStackEmpty(s))
    {
        return s.data[s.top];
    }
    else
    {
        return NULL;
    }
}

void popStackToEnd(S &op,NS &opn,int kind)
{
    if(getTop(op)==kind)
    {
        return;
    }
    Node t1,t2;
    int t;
    if (!popNodeStack(opn,t2))
        errors++;
    if (!popNodeStack(opn,t1))
        errors++;
    if (!popStack(op,t))
        errors++;
    Node p = (Node)malloc(sizeof(Tnode));
    p->type = opsign;
    p->opsignp = (struct opsign *)malloc(sizeof(struct opsign));
    p->opsignp->kind = t;
    p->opsignp->lchild = t1;
    p->opsignp->rchild = t2;
    pushNodeStack(opn, p);
    return;
}
//栈函数

Node program();
Node getArray();
Node ExtDefList();
Node ExtDef();

Node FuncDef();

Node ExtVarList();
Node ExtVarDef();
Node Exp(int endsym);
Node Statement();
Node SentenceList();
Node MultiSentence();
Node LocVarList();
Node LocVarDef();
Node LocDefList();
Node Formal_parameter(int kind);
Node Formal_parameterlist();
Node PreInfo();
Node ReturnSentence();
Node ContinueSentence();
Node BreakSentence();

Node program()
{
    w = gettoken();
    Node prep;
    prep = CreateNode(pretree);
    prep->pretreep = (struct pretree*)malloc(sizeof(struct pretree));
    prep->pretreep->preinformation = PreInfo();
    prep->pretreep->extdefinformation = ExtDefList();
    printf(" %d ", w);
    if (prep)
    {
        //程序语法正确，返回的语法树根结点指针，可遍历显示
        printf("success\n");
        traverse(prep);
        return prep;
    }
    else
    {
        printf("error\n");
        return NULL;
    } //有语法错误
}

Node PreInfo()//头文件和宏
{
	int i;

	if(w != INCLUDE && w!= DEFINE)
	{
		return NULL;
	}

    Node root = (Node)malloc(sizeof(struct node));
    switch (w)
    {
        case INCLUDE:
        {
            root->type = includesentence;
            root->includesentencep = (struct includesentence*)malloc(sizeof(struct includesentence));
            strcpy(root->includesentencep->data, token_text);

            w = gettoken();

            if(w == INCLUDE||w == DEFINE)
            {
                root->includesentencep->child = PreInfo();
                break;
            }
            else
            {
                root->includesentencep->child = NULL;
                break;
            }
        }
        case DEFINE:
        {
            root->type = definesentence;
            root->definesentencep = (struct definesentence*)malloc(sizeof(struct definesentence));
            w = gettoken();
            if(w != IDENT)
            {
                return NULL;
            }
            strcpy(root->definesentencep->id,token_text);
            w = gettoken();
            if(w != INT_CONST && w != FLOAT_CONST && w != LONG_CONST && w != CHAR_CONST)
            {
                return NULL;
            }
            strcpy(root->definesentencep->data,token_text);
            w = gettoken();
            if(w == INCLUDE||w == DEFINE)
            {
                root->definesentencep->child = PreInfo();
                break;
            }
            else
            {
                root->definesentencep->child = NULL;
                break;
            }
        }
    }

    return root;
}

Node ExtDefList()
{
    //处理外部定义序列，正确时，返回子树根结点指针，否则错误处理
    if (w == EOF)
    {
        return NULL;
    }
    Node root = (Node)malloc(sizeof(struct node));
    root->type = extdeflist;
    root->extdeflistp = (struct extdeflist *)malloc(sizeof(struct extdeflist));
    //生成一个外部定义序列结点root
    root->extdeflistp->lchild = ExtDef();
    //处理一个外部定义，得到一棵子树，作为root的第一棵子树
    root->extdeflistp->rchild = ExtDefList();
    //得到的子树，作为root的第二棵子树
    return root;
}

Node ExtDef()
{
    ar = 0;
    //处理外部定义序列，正确时，返回子树根结点指针，否则返回NULL

    if (w != INT && w != CHAR && w != FLOAT) //?类型关键字
    {
        printf("ERROR IN LINE %d : UNKOWN VAR TYPE %d \n", line,w);//?
        exit(0);
        return NULL;
    }

    //报错并返回NULL
    cls = w; //保存类型说明符

    w = gettoken();

    if (w != IDENT )
    {
        printf("ERROR IN LINE %d :错误变量\n", line);
        printf("ERROR");
        return NULL;
    }//报错并返回NULL

    strcpy(token_text0,token_text); // 保存第一个变量名或函数名到tokenText0
    ar = w;
    w = gettoken();
    Node p;
    if(w == LP)
    {
        p = FuncDef();//调用函数定义子程序
    }
    else
    {
        p = ExtVarDef(); //调用外部变量定义子程序
    }
    if(p)
    {
        return p;
    }
    else
        return NULL;
    //如果返回值p非空，表示成功完成一个外部定义的处理，返回p
}

Node ExtVarList()
{
    // 初始时，tokenText0保存了第一个变量名
    Node root = (Node)malloc(sizeof(Tnode));
    root->type = extvarlist;
    // root=生成外部变量序列结点
    root->extvarlistp=(struct extvarlist*)malloc(sizeof(struct extvarlist));
    strcpy(root->extvarlistp->data, token_text0);
    //由保存在tokenText0的第一个变量名生成一个变量名结点，作为root的第一个孩子；
    if(ar==GROUP)
        root->extvarlistp->isar=1;
    else
        root->extvarlistp->isar=0;
    //开始识别后续的变量名
    if (w != COMMA && w != SEMI)
    {
        //?free root?
        printf("ERROR IN LINE %d : 1缺少';'或','\n", line);
        return NULL;
    }
    //报错，释放root为根的全部结点，返回空指针
    if (w == SEMI)
    {
        w = gettoken();
        root->extvarlistp-> child= NULL;
        return root;
    }

    w = gettoken();

    if (w != IDENT  )
    {
        //报错，释放root为根的全部结点，返回空指针
        //*?错误描述
        printf("ERROR IN LINE %d : 逗号之后应为标识符\n", line);        return NULL;
    }
	strcpy(token_text0,token_text);
    //将变量名w保存在tokenText0中
    w = gettoken(); //*why gettoken
    root->extvarlistp->child = ExtVarList();
    return root;
    //调用ExtVarList，得到的子树作为root的第二棵字数，返回root；
}


Node ExtVarDef()
{
	// root=生成一个外部变量定义结点；
    Node root = (Node)malloc(sizeof(Tnode));
    root->type = extvardef;
    root->extvardefp = (struct extvardef*)malloc(sizeof(struct extvardef));
    //.*的位置
    root->extvardefp->kind = cls;
    //根据已读入的外部变量的类型，生成外部变量类型结点，作为root的第一个孩子
    root->extvardefp->child = ExtVarList();
    //调用ExtVarList，得到的子树根作为root的第二个子树
    return root;
}


Node Formal_parameterlist()
{
    w = gettoken();
    //*
//    if (w == RP)
  //  {
    //    return NULL;
    //}

    if (w != INT && w != CHAR && w != FLOAT)
    {
        printf("ERROR IN LINE %d : 函数形参格式不正确\n", line);
        return NULL;
    }

    Node root = (Node)malloc(sizeof(struct node));
    root->type = formal_parameterlist;//create root
    root->formal_parameterlistp = (struct formal_parameterlist *)malloc(sizeof(struct formal_parameterlist));
    root->formal_parameterlistp->lchild = Formal_parameter(w);
    w = gettoken();

    if (w == COMMA)
    {
        root->formal_parameterlistp->rchild = Formal_parameterlist();
    }
    else if (w == RP)
    {
        root->formal_parameterlistp->rchild = NULL;
    }
    else
    {
        printf("ERROR IN LINE %d : 函数形参格式不正确\n", line);
    }
    return root;
}

Node Formal_parameter(int kind)
{
    w = gettoken();
    if(w!=IDENT)//?
    {
        return NULL;
    }
    Node root = (Node)malloc(sizeof(struct node));
    root->type=formal_parameter;
    root->formal_parameterp = (struct formal_parameter *)malloc(sizeof(struct formal_parameter));
    root->formal_parameterp->kind = kind;
    strcpy(root->formal_parameterp->data, token_text);
    return root;
}

Node FuncDef()
{
    Node root = (Node)malloc(sizeof(Tnode));
    root->type = funcdef;
    root->funcdefp = (struct funcdef *)malloc(sizeof(struct funcdef));
    //生成函数定义结点root；
    strcpy(root->funcdefp->name,token_text0);//name
	Node p = (Node)malloc(sizeof(Tnode));
    p->type = funkind;
    p->funkindp = (struct funkind *)malloc(sizeof(struct funkind));
    //生成返回值类型结点，作为root的第一个孩子
    p->funkindp->type = cls;
    root->funcdefp->lchild = p; //生成返回值类型结点，作为root的第一个孩子
    root->funcdefp->mchild = Formal_parameterlist();
    //处理参数部分到反小括号结束，调用形参子程序，得到参数部分的子树根指针，无参函数得到NULL，该子树作为root的第二棵子树；
    //?printf()
    w = gettoken();//读入符号
    if (w == SEMI)
    {
        root->funcdefp->rchild = NULL;//函数体子树为空
    }//如果w是分号，就是函数原型声明；
    else if (w == BLP)
    {
        root->funcdefp->rchild = MultiSentence();
    }//正大括号，则调用函数体（复合语句）子程序，得到函数体子树根指针
    else//其它符号表示有错
    {
        printf("ERROR IN LINE %d : 函数定义格式不正确\n", line);
        return NULL;
    }
    funlist[funnum++] = root;//记录已出现函数
	return root;//返回root
}

Node MultiSentence()
{
    Node root = (Node)malloc(sizeof(Tnode));
    root->type = multisentence;
    root->multisentencep = (struct multisentence *)malloc(sizeof(struct multisentence));
    //root=生成复合语句结点注意其中局部变量说明和语句序列都可以为空
    w=gettoken();
    if (w == INT || w == CHAR || w == FLOAT)//?
    {
        //调用处理局部变量说明序列子程序到,返回的子树根结点作为root的第一个孩子;
        root->multisentencep->lchild = LocDefList();
    }
    else
    {
        root->multisentencep->lchild = NULL;
        //root无局部变量说明，root的第一个孩子设置为空指针
    }

    if(w != BRP)
    {
        printf("ERROR IN LINE %d : 缺少反大括号\n", line);
        return NULL;
    }
    w = gettoken();
    return root;//返回复合语句的子树根指针


	/**
    if （w是类型关键字）{ 调用处理局部变量说明序列子程序，
	得到返回的子树根结点作为root的第一个孩子}
	else { 无局部变量说明，root的第一个孩子设置为空指针}
    调用处理语句序列子程序，返回子树根结点指针，作为root的s2个孩子
    if （w不是反大括号} ）返回空指针，报错并释放结点
    w=gettoken();
	返回复合语句的子树根指针。
    **/
}

Node SentenceList()
{
    Node root = NULL;
    //初始化子树，根指针root=NULL；
    Node r1 = Statement();
    //调用处理一条语句的子程序；返回其子树根指针r1
    if (r1 == NULL)
    {
        if(errors>0)//没有分析到1条语句，
        {
            printf("ERROR IN LINE %d : 语句中出现错误\n", line);
            return NULL;
        }//errors>0时处理错误，
        else
        {
            //?
            return NULL;
        }//否则表示语句序列已结束,返回NULL
    }
    else
    {
        Node root = (Node)malloc(sizeof(struct node));
        root->type = sentencelist;
        root->sentencelistp = (struct sentencelist *)malloc(sizeof(struct sentencelist));
        root->sentencelistp->lchild = r1;             //root->第1孩子=r1;
        root->sentencelistp->rchild = SentenceList(); //root->第2孩子=递归调用处理语句序列子程序后的返回值；
        return root;
    }
}



Node Statement()//处理一条语句的子程序
{
    Node exp;
    Node sentences;
    Node ifn, eln;
    Node root;
    int i;
    Node p,wh;
    switch(w)
    {
        case IF:// analysis if-else sentences
            w=gettoken();

            if (w != LP)
            {
                printf("ERROR IN LINE %d : if条件语句格式错误\n", line);
                return NULL;
            }//如果w不是左小括号 报错并返回空
            w=gettoken();
            exp = Exp(RP);//调用处理表达式的子程序（结束符号为反小括号），正确时得到条件表达式子树结点指针
            w=gettoken();
            ifn = Statement();//调用处理一条语句的子程序，得到IF子句的子树根指针。
            if( w == ELSE)
            {
                w=gettoken();
                eln = Statement();//调用处理一条语句的子程序，得到IF子句的子树根指针。
                root =(Node)malloc(sizeof(Tnode));
                root->type = ifelsesentence;
                root->ifelsesentencep = (struct ifelsesentence *)malloc(sizeof(struct ifelsesentence));
                root->ifelsesentencep->condition = exp;
                root->ifelsesentencep->ifresult = ifn;
                root->ifelsesentencep->elseresult = eln;
                return root;
                //生成IF-ELSE结点，下挂条件、IF子句、ELSE子句3棵子树
            }
            else
            {
                root =(Node)malloc(sizeof(Tnode));
                root->type = ifsentence;
                root->ifsentencep = (struct ifsentence *)malloc(sizeof(struct ifsentence));
                root->ifsentencep->condition = exp;
                root->ifsentencep->result = ifn;
                return root;
            }
            //else 生成IF结点，下挂条件、IF子句2棵子树
        case FOR:
            w = gettoken();
            if(w != LP)
            {
                printf("ERROR IN LINE %d : for循环格式错误\n", line);
                return NULL;
            }
            root = CreateNode(forsentence);
            root->forsentencep = (struct forsentence *)malloc(sizeof(struct forsentence));
            exp = Exp(SEMI);
            root->forsentencep->start = exp;

            exp = Exp(SEMI);
            root->forsentencep->condition = exp;

            exp = Exp(RP);
            root->forsentencep->change = exp;
            w=gettoken();
            if( w== SEMI)
                root->forsentencep->sentence=NULL;
            //ungetc(c, fp);
            else
                root->forsentencep->sentence = Statement();
            break;
        case DO:
            {
                w = gettoken();
                root = CreateNode(dowhilesentence);
                root->dowhilesentencep = (struct dowhilesentence *)malloc(sizeof(struct dowhilesentence));
                root->dowhilesentencep->sentence = Statement();//?
                //w = gettoken();
                //w = gettoken();

                if(w != WHILE)
                {
                    printf("ERROR IN LINE %d : 缺少while\n", line);
                    return NULL;
                }
                w = gettoken();
                w = gettoken();
                exp = Exp(RP);
                root->dowhilesentencep->condition = exp;
                w = gettoken();//反括号
                if(w!=SEMI)
                {
                    printf("ERROR IN LINE %d : DO-WHILE语句结束后应该加分号\n", line);
                    return NULL;
                }
                w = gettoken();//分号
                return root;
			}
        case BLP :
            return MultiSentence();
        case WHILE:
            w=gettoken();
			if (w!=LP)
            {
                printf("ERROR IN LINE %d : while后缺少循环控制条件\n", line);
                return NULL;
            }
            w=gettoken();
            exp=Exp(RP);//调用处理表达式的子程序（结束符号为反小括号），正确时得到条件表达式子树结点指针
            w=gettoken();
            wh=Statement();
            root =CreateNode(whilesentence);
            root->whilesentencep=(struct whilesentence*)malloc(sizeof(struct whilesentence));
            root->whilesentencep->condition=exp;
            root->whilesentencep->sentence=wh;
            return root;
            break;
        case LP:
            Exp(RP);
            break;
        case IDENT://?
            p=Exp(SEMI);
            if(!errors)
            {
                w=gettoken();
            }
            return p;
        case USEFUN:
            p=Exp(SEMI);
            if(!errors)
            {
                w=gettoken();
            }
            return p;
        case INT_CONST:
            p = Exp(SEMI);//调用表达式处理子程序（结束符号为分号）
            if(errors == 0)
            {
                w = gettoken();
            }
            return p;
            //正确时，w=gettoken();  返回表达式语句子树的根结点指针
        case FLOAT_CONST:
            p = Exp(SEMI);//调用表达式处理子程序（结束符号为分号）
            if(errors == 0)
            {
                w = gettoken();
            }
            return p;
        case LONG_CONST:
            p = Exp(SEMI);//调用表达式处理子程序（结束符号为分号）
            if(errors == 0)
            {
                w = gettoken();
            }
            return p;
        case CHAR_CONST:
            p = Exp(SEMI);//调用表达式处理子程序（结束符号为分号）
            if(errors == 0)
            {
                w = gettoken();
            }
            return p;
        case GROUP:
            p = Exp(SEMI);//调用表达式处理子程序（结束符号为分号）
            if(errors == 0)
            {
                w = gettoken();
            }
            return p;
        case BRP:
            //w=gettoken();
			return NULL;
        case INT:
            return LocDefList();
        case FLOAT:
            return LocDefList();
        case CHAR :
            return LocDefList();
        case CONTINUE:
            return ContinueSentence();
        case BREAK:
            return BreakSentence();
        case RETURN:
            w=gettoken();
            p = ReturnSentence();
            return p;
        case ANNO:
            return p;
        default:
            errors++;
            return NULL;
    }
}

Node Exp(int endsym)//表达式结束符号endsym可以是分号，如表达式语句，					          //可以是反小括号，作为条件时使用
{
    //调用该算法时，在调用者已经读入了第一个单词在w中
    S op;
    initStack(op);
    pushStack(op,BEGIN_END);
    //定义运算符栈 op;并初始化，将起止符#入栈
    NS opn;
    initNodeStack(opn);
    //定义操作数栈opn，元素是结点的指针
    errors = 0;
    Node t1,t2;
    int t;
    int ac;
    while ((w != BEGIN_END || getTop(op) != BEGIN_END) && !errors)//?
    {
        if (w == IDENT || w == INT_CONST || w == FLOAT_CONST ||w==LONG_CONST ||w == GROUP)
        {
            Node p = (Node)malloc(sizeof(Tnode));
            p->type = opnum;
            p->opnump=(struct opnum*)malloc(sizeof(struct opnum));
            p->opnump->kind=w;
            strcpy(p->opnump->data,token_text);//根据w生成一个结点
            pushNodeStack(opn, p);//结点指针进栈opn
            w = gettoken();
        }
        else if (cnt == 1)
        {
            Node p;
            if(w == SMALLEREQUAL)
            {
                ac = SMALLER;
            }
            else if( w == BIGGEREQUAL)
            {
                ac == BIGGER;
            }
            else
            {
                ac = w;
            }

            switch (precede[getTop(op)][ac])
            {
                case '<':
                    pushStack(op,w);
                    w=gettoken();
                    break;
                case '=':
                    if (!popStack(op,t))
                        errors++;
                    w=gettoken();//去括号
                    break;
                case '>':
                    if (w == RP && isLp(op))
                    {
                        popStackToEnd(op,opn,LP);
                        break;
                    }
                    if (w == RP && !isLp(op) && w == endsym)
                    {
                        popStackToEnd(op,opn,BEGIN_END);
                        w = BEGIN_END;
                        break;
                    }
                    if (!popNodeStack(opn,t2))
                        errors++;
                    if (!popNodeStack(opn,t1))
                        errors++;
                    if (!popStack(op,t))
                        errors++;
                    //根据运算符栈退栈得到的运算符t和操作数的结点指针t1和t2

                    p = CreateNode(opsign);
                    p->opsignp=(struct opsign*)malloc(sizeof(struct opsign));
                    p->opsignp->kind = t;
                    p->opsignp->lchild = t1;
                    p->opsignp->rchild = t2;
                    pushNodeStack(opn, p);
                    break;
                    //完成建立生成一个运算符的结点，结点指针进栈opn
                default:
                    if (w == endsym)
                        w = BEGIN_END; //遇到结束标记），w被替换成#
                    else
                        errors = 1;
            }

        }
        else if (w == endsym)
        {
            popStackToEnd(op,opn,BEGIN_END);
            w = BEGIN_END; //遇到结束标记），w被替换成#
        }
        else
            errors = 1;
    }

    if (opn.top == 0 && errors == 0 && getTop(op) == BEGIN_END)
    {
        return opn.data[0];//成功返回表达式语法树的根结点指针
    }
    else
    {
        return NULL;//表达式分析有错
    }
}

Node ContinueSentence()
{
    Node root = (Node)malloc(sizeof(struct node));
    root -> type = continuesentence;
    root ->continuesentencep = (struct continuesentence *)malloc(sizeof(struct continuesentence));
    return root;
}
Node BreakSentence()
{
    Node root = (Node)malloc(sizeof(struct node));
    root -> type = breaksentence;
    root -> breaksentencep = (struct breaksentence *)malloc(sizeof(struct breaksentence));
    return root;
}

Node LocVarList()
{
    Node root = (Node)malloc(sizeof(struct node));
    root->type = locvarnamelist;
    root->locvarnamelistp = (struct locvarnamelist *)malloc(sizeof(struct locvarnamelist));
    //create root
    //w = gettoken();

    strcpy(root->locvarnamelistp->data,token_text);
    if(ar==GROUP)
        root->locvarnamelistp->isar=1;
    else
        root->locvarnamelistp->isar=0;
    w = gettoken();

    if (w != COMMA && w != SEMI)
    {
        //free(root)
        printf("ERROR IN LINE %d : 局部变量定义格式错误\n", line);
        return NULL;
    }
    if (w == SEMI)
    {
        w = gettoken();
        root->locvarnamelistp->child=NULL;
        return root;
	}

    w = gettoken();
    if (w != IDENT)
    {
        //free(root);
        printf("ERROR IN LINE %d : 局部变量定义格式错误\n", line);
        return NULL;
    }
    //strcpy(token_text0, token_text);
    root->locvarnamelistp->child = LocVarList();
    return root;
}

//局部变量定义节点
Node LocVarDef()
{
    Node root = (Node)malloc(sizeof(Tnode));
    root->type = locvardef;
    root->locvardefp = (struct locvardef *)malloc(sizeof(struct locvardef));
    root->locvardefp->kind = w;
    w=gettoken();
    root->locvardefp->child = LocVarList();
    return root;
}

//局部变量定义序列节点
Node LocDefList()
{
    if (w == EOF)
    {
        return NULL;
    }
    Node root = (Node)malloc(sizeof(Tnode));
    root->type = locvardeflist;
    root->locvardeflistp = (struct locvardeflist *)malloc(sizeof(struct locvardeflist));
    root->locvardeflistp->lchild = LocVarDef();
    if (w == INT || w == FLOAT || w == CHAR)
    {
        root->locvardeflistp->rchild = LocDefList();
    }
    else
    {
        root->locvardeflistp->rchild = NULL;
    }
    return root;
}

Node ReturnSentence()
{
    if(w == IDENT || w == INT_CONST || w == USEFUN|| w == CHAR_CONST || w == FLOAT_CONST|| w == LONG_CONST)
    {
        Node p = (struct node*)malloc(sizeof(struct node));
        p->type = var;
        p->varp = (struct var*)malloc(sizeof(struct var));
        strcpy(p->varp->data,token_text);
        w = gettoken();
        if(w == SEMI)
        {
            w  = gettoken();
            return p;
        }
    }
}


