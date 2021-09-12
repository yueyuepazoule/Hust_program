#include "token.h"

typedef struct STACK{
    int top;
    int data[10000];
} S;//intջ

typedef struct NodeStack{
    int top;
    Node data[1000];
} NS;//Nodeջ

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
//ջ����

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
        //�����﷨��ȷ�����ص��﷨�������ָ�룬�ɱ�����ʾ
        printf("success\n");
        traverse(prep);
        return prep;
    }
    else
    {
        printf("error\n");
        return NULL;
    } //���﷨����
}

Node PreInfo()//ͷ�ļ��ͺ�
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
    //�����ⲿ�������У���ȷʱ���������������ָ�룬���������
    if (w == EOF)
    {
        return NULL;
    }
    Node root = (Node)malloc(sizeof(struct node));
    root->type = extdeflist;
    root->extdeflistp = (struct extdeflist *)malloc(sizeof(struct extdeflist));
    //����һ���ⲿ�������н��root
    root->extdeflistp->lchild = ExtDef();
    //����һ���ⲿ���壬�õ�һ����������Ϊroot�ĵ�һ������
    root->extdeflistp->rchild = ExtDefList();
    //�õ�����������Ϊroot�ĵڶ�������
    return root;
}

Node ExtDef()
{
    ar = 0;
    //�����ⲿ�������У���ȷʱ���������������ָ�룬���򷵻�NULL

    if (w != INT && w != CHAR && w != FLOAT) //?���͹ؼ���
    {
        printf("ERROR IN LINE %d : UNKOWN VAR TYPE %d \n", line,w);//?
        exit(0);
        return NULL;
    }

    //��������NULL
    cls = w; //��������˵����

    w = gettoken();

    if (w != IDENT )
    {
        printf("ERROR IN LINE %d :�������\n", line);
        printf("ERROR");
        return NULL;
    }//��������NULL

    strcpy(token_text0,token_text); // �����һ����������������tokenText0
    ar = w;
    w = gettoken();
    Node p;
    if(w == LP)
    {
        p = FuncDef();//���ú��������ӳ���
    }
    else
    {
        p = ExtVarDef(); //�����ⲿ���������ӳ���
    }
    if(p)
    {
        return p;
    }
    else
        return NULL;
    //�������ֵp�ǿգ���ʾ�ɹ����һ���ⲿ����Ĵ�������p
}

Node ExtVarList()
{
    // ��ʼʱ��tokenText0�����˵�һ��������
    Node root = (Node)malloc(sizeof(Tnode));
    root->type = extvarlist;
    // root=�����ⲿ�������н��
    root->extvarlistp=(struct extvarlist*)malloc(sizeof(struct extvarlist));
    strcpy(root->extvarlistp->data, token_text0);
    //�ɱ�����tokenText0�ĵ�һ������������һ����������㣬��Ϊroot�ĵ�һ�����ӣ�
    if(ar==GROUP)
        root->extvarlistp->isar=1;
    else
        root->extvarlistp->isar=0;
    //��ʼʶ������ı�����
    if (w != COMMA && w != SEMI)
    {
        //?free root?
        printf("ERROR IN LINE %d : 1ȱ��';'��','\n", line);
        return NULL;
    }
    //�����ͷ�rootΪ����ȫ����㣬���ؿ�ָ��
    if (w == SEMI)
    {
        w = gettoken();
        root->extvarlistp-> child= NULL;
        return root;
    }

    w = gettoken();

    if (w != IDENT  )
    {
        //�����ͷ�rootΪ����ȫ����㣬���ؿ�ָ��
        //*?��������
        printf("ERROR IN LINE %d : ����֮��ӦΪ��ʶ��\n", line);        return NULL;
    }
	strcpy(token_text0,token_text);
    //��������w������tokenText0��
    w = gettoken(); //*why gettoken
    root->extvarlistp->child = ExtVarList();
    return root;
    //����ExtVarList���õ���������Ϊroot�ĵڶ�������������root��
}


Node ExtVarDef()
{
	// root=����һ���ⲿ���������㣻
    Node root = (Node)malloc(sizeof(Tnode));
    root->type = extvardef;
    root->extvardefp = (struct extvardef*)malloc(sizeof(struct extvardef));
    //.*��λ��
    root->extvardefp->kind = cls;
    //�����Ѷ�����ⲿ���������ͣ������ⲿ�������ͽ�㣬��Ϊroot�ĵ�һ������
    root->extvardefp->child = ExtVarList();
    //����ExtVarList���õ�����������Ϊroot�ĵڶ�������
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
        printf("ERROR IN LINE %d : �����βθ�ʽ����ȷ\n", line);
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
        printf("ERROR IN LINE %d : �����βθ�ʽ����ȷ\n", line);
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
    //���ɺ���������root��
    strcpy(root->funcdefp->name,token_text0);//name
	Node p = (Node)malloc(sizeof(Tnode));
    p->type = funkind;
    p->funkindp = (struct funkind *)malloc(sizeof(struct funkind));
    //���ɷ���ֵ���ͽ�㣬��Ϊroot�ĵ�һ������
    p->funkindp->type = cls;
    root->funcdefp->lchild = p; //���ɷ���ֵ���ͽ�㣬��Ϊroot�ĵ�һ������
    root->funcdefp->mchild = Formal_parameterlist();
    //����������ֵ���С���Ž����������β��ӳ��򣬵õ��������ֵ�������ָ�룬�޲κ����õ�NULL����������Ϊroot�ĵڶ���������
    //?printf()
    w = gettoken();//�������
    if (w == SEMI)
    {
        root->funcdefp->rchild = NULL;//����������Ϊ��
    }//���w�Ƿֺţ����Ǻ���ԭ��������
    else if (w == BLP)
    {
        root->funcdefp->rchild = MultiSentence();
    }//�������ţ�����ú����壨������䣩�ӳ��򣬵õ�������������ָ��
    else//�������ű�ʾ�д�
    {
        printf("ERROR IN LINE %d : ���������ʽ����ȷ\n", line);
        return NULL;
    }
    funlist[funnum++] = root;//��¼�ѳ��ֺ���
	return root;//����root
}

Node MultiSentence()
{
    Node root = (Node)malloc(sizeof(Tnode));
    root->type = multisentence;
    root->multisentencep = (struct multisentence *)malloc(sizeof(struct multisentence));
    //root=���ɸ��������ע�����оֲ�����˵����������ж�����Ϊ��
    w=gettoken();
    if (w == INT || w == CHAR || w == FLOAT)//?
    {
        //���ô���ֲ�����˵�������ӳ���,���ص������������Ϊroot�ĵ�һ������;
        root->multisentencep->lchild = LocDefList();
    }
    else
    {
        root->multisentencep->lchild = NULL;
        //root�޾ֲ�����˵����root�ĵ�һ����������Ϊ��ָ��
    }

    if(w != BRP)
    {
        printf("ERROR IN LINE %d : ȱ�ٷ�������\n", line);
        return NULL;
    }
    w = gettoken();
    return root;//���ظ�������������ָ��


	/**
    if ��w�����͹ؼ��֣�{ ���ô���ֲ�����˵�������ӳ���
	�õ����ص������������Ϊroot�ĵ�һ������}
	else { �޾ֲ�����˵����root�ĵ�һ����������Ϊ��ָ��}
    ���ô�����������ӳ��򣬷������������ָ�룬��Ϊroot��s2������
    if ��w���Ƿ�������} �����ؿ�ָ�룬�����ͷŽ��
    w=gettoken();
	���ظ�������������ָ�롣
    **/
}

Node SentenceList()
{
    Node root = NULL;
    //��ʼ����������ָ��root=NULL��
    Node r1 = Statement();
    //���ô���һ�������ӳ��򣻷�����������ָ��r1
    if (r1 == NULL)
    {
        if(errors>0)//û�з�����1����䣬
        {
            printf("ERROR IN LINE %d : ����г��ִ���\n", line);
            return NULL;
        }//errors>0ʱ�������
        else
        {
            //?
            return NULL;
        }//�����ʾ��������ѽ���,����NULL
    }
    else
    {
        Node root = (Node)malloc(sizeof(struct node));
        root->type = sentencelist;
        root->sentencelistp = (struct sentencelist *)malloc(sizeof(struct sentencelist));
        root->sentencelistp->lchild = r1;             //root->��1����=r1;
        root->sentencelistp->rchild = SentenceList(); //root->��2����=�ݹ���ô�����������ӳ����ķ���ֵ��
        return root;
    }
}



Node Statement()//����һ�������ӳ���
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
                printf("ERROR IN LINE %d : if��������ʽ����\n", line);
                return NULL;
            }//���w������С���� �������ؿ�
            w=gettoken();
            exp = Exp(RP);//���ô�����ʽ���ӳ��򣨽�������Ϊ��С���ţ�����ȷʱ�õ��������ʽ�������ָ��
            w=gettoken();
            ifn = Statement();//���ô���һ�������ӳ��򣬵õ�IF�Ӿ��������ָ�롣
            if( w == ELSE)
            {
                w=gettoken();
                eln = Statement();//���ô���һ�������ӳ��򣬵õ�IF�Ӿ��������ָ�롣
                root =(Node)malloc(sizeof(Tnode));
                root->type = ifelsesentence;
                root->ifelsesentencep = (struct ifelsesentence *)malloc(sizeof(struct ifelsesentence));
                root->ifelsesentencep->condition = exp;
                root->ifelsesentencep->ifresult = ifn;
                root->ifelsesentencep->elseresult = eln;
                return root;
                //����IF-ELSE��㣬�¹�������IF�Ӿ䡢ELSE�Ӿ�3������
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
            //else ����IF��㣬�¹�������IF�Ӿ�2������
        case FOR:
            w = gettoken();
            if(w != LP)
            {
                printf("ERROR IN LINE %d : forѭ����ʽ����\n", line);
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
                    printf("ERROR IN LINE %d : ȱ��while\n", line);
                    return NULL;
                }
                w = gettoken();
                w = gettoken();
                exp = Exp(RP);
                root->dowhilesentencep->condition = exp;
                w = gettoken();//������
                if(w!=SEMI)
                {
                    printf("ERROR IN LINE %d : DO-WHILE��������Ӧ�üӷֺ�\n", line);
                    return NULL;
                }
                w = gettoken();//�ֺ�
                return root;
			}
        case BLP :
            return MultiSentence();
        case WHILE:
            w=gettoken();
			if (w!=LP)
            {
                printf("ERROR IN LINE %d : while��ȱ��ѭ����������\n", line);
                return NULL;
            }
            w=gettoken();
            exp=Exp(RP);//���ô�����ʽ���ӳ��򣨽�������Ϊ��С���ţ�����ȷʱ�õ��������ʽ�������ָ��
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
            p = Exp(SEMI);//���ñ��ʽ�����ӳ��򣨽�������Ϊ�ֺţ�
            if(errors == 0)
            {
                w = gettoken();
            }
            return p;
            //��ȷʱ��w=gettoken();  ���ر��ʽ��������ĸ����ָ��
        case FLOAT_CONST:
            p = Exp(SEMI);//���ñ��ʽ�����ӳ��򣨽�������Ϊ�ֺţ�
            if(errors == 0)
            {
                w = gettoken();
            }
            return p;
        case LONG_CONST:
            p = Exp(SEMI);//���ñ��ʽ�����ӳ��򣨽�������Ϊ�ֺţ�
            if(errors == 0)
            {
                w = gettoken();
            }
            return p;
        case CHAR_CONST:
            p = Exp(SEMI);//���ñ��ʽ�����ӳ��򣨽�������Ϊ�ֺţ�
            if(errors == 0)
            {
                w = gettoken();
            }
            return p;
        case GROUP:
            p = Exp(SEMI);//���ñ��ʽ�����ӳ��򣨽�������Ϊ�ֺţ�
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

Node Exp(int endsym)//���ʽ��������endsym�����Ƿֺţ�����ʽ��䣬					          //�����Ƿ�С���ţ���Ϊ����ʱʹ��
{
    //���ø��㷨ʱ���ڵ������Ѿ������˵�һ��������w��
    S op;
    initStack(op);
    pushStack(op,BEGIN_END);
    //���������ջ op;����ʼ��������ֹ��#��ջ
    NS opn;
    initNodeStack(opn);
    //���������ջopn��Ԫ���ǽ���ָ��
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
            strcpy(p->opnump->data,token_text);//����w����һ�����
            pushNodeStack(opn, p);//���ָ���ջopn
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
                    w=gettoken();//ȥ����
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
                    //���������ջ��ջ�õ��������t�Ͳ������Ľ��ָ��t1��t2

                    p = CreateNode(opsign);
                    p->opsignp=(struct opsign*)malloc(sizeof(struct opsign));
                    p->opsignp->kind = t;
                    p->opsignp->lchild = t1;
                    p->opsignp->rchild = t2;
                    pushNodeStack(opn, p);
                    break;
                    //��ɽ�������һ��������Ľ�㣬���ָ���ջopn
                default:
                    if (w == endsym)
                        w = BEGIN_END; //����������ǣ���w���滻��#
                    else
                        errors = 1;
            }

        }
        else if (w == endsym)
        {
            popStackToEnd(op,opn,BEGIN_END);
            w = BEGIN_END; //����������ǣ���w���滻��#
        }
        else
            errors = 1;
    }

    if (opn.top == 0 && errors == 0 && getTop(op) == BEGIN_END)
    {
        return opn.data[0];//�ɹ����ر��ʽ�﷨���ĸ����ָ��
    }
    else
    {
        return NULL;//���ʽ�����д�
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
        printf("ERROR IN LINE %d : �ֲ����������ʽ����\n", line);
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
        printf("ERROR IN LINE %d : �ֲ����������ʽ����\n", line);
        return NULL;
    }
    //strcpy(token_text0, token_text);
    root->locvarnamelistp->child = LocVarList();
    return root;
}

//�ֲ���������ڵ�
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

//�ֲ������������нڵ�
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


