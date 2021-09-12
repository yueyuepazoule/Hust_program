#include "define.h"
char c;
int tokeni;

int gettoken()
{
    int i = 0;
    // fp?????????????????¨²???????¨°???
    cnt = 0;
    memset(token_text, 0, sizeof(token_text));
    //???????????????token_text???;
    char c;

    while ((c = fgetc(fp)) == ' ' || c == '\t' || c == '\n') //??????????????????????¦Ë??????????????
    {
        if (c == '\n')
        {
            line++; //????????¦Ë???????????????????????1
        }
    }

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
        int tokeni = 0;
        do
        {
            token_text[tokeni++] = c;
        } while (((c = fgetc(fp)) >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
        //????????
        ungetc(c, fp);
        int nowflag;
        if (!strcmp(token_text, "int"))
        {
            return INT;
        }
        else if (!strcmp(token_text, "char"))
        {
            return CHAR;
        }
        else if (!strcmp(token_text, "float"))
        {
            return FLOAT;
        }
        else if (!strcmp(token_text, "long"))
        {
            return LONG;
        }
        else if (!strcmp(token_text, "if"))
        {
            return IF;
        }
        else if (!strcmp(token_text, "else"))
        {
            return ELSE;
        }
        else if (!strcmp(token_text, "while"))
        {
            return WHILE;
        }
        else if (!strcmp(token_text, "for"))
        {
            return FOR;
        }
        else if (!strcmp(token_text, "return"))
        {
            return RETURN;
        }
        else if (!strcmp(token_text, "continue"))
        {
            return CONTINUE;
        }
        else if (!strcmp(token_text, "break"))
        {
            return BREAK;
        }
        c = fgetc(fp);
        if (c == '[')
        {
            while (c == '[')
            {
                nowflag = tokeni;
                //?§Ø?[]?§Þ??????????//2

                do
                {
                    if (c != ' ')
                        token_text[tokeni++] = c;
                } while ((c = fgetc(fp)) >= '0' && c <= '9' || c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == ' ');

                nowflag = tokeni - nowflag;
                //
                if (c != ']' || nowflag < 2)
                {
                    return ERROR_TOKEN;
                }
                token_text[tokeni++] = c;
                c = getchar();
            }
            ungetc(c, fp);
            return GROUP;
        }
       /* else if (c == '(')
        {
            do
            {
                if (c != ' ')
                    token_text[tokeni++] = c;
            } while ((c = fgetc(fp)) >= '0' && c <= '9' || c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == ',' || c == ' ');
            if (c != ')')
            {
                return ERROR_TOKEN;
            }
            token_text[tokeni++] = c;
            return USEFUN;
        }*/
        ungetc(c, fp); //?????????§Ø??????????????????????
        return IDENT;
    } //???????????????????????
    //??????????¨¿?????????§Ø??????????????????????token_text??
    //????????§Û?????????????????????????????????????????????
    //??????????????????IDENT
    if (c >= '0' && c <= '9')
    {
        token_text[0] = c;
        tokeni = 1;
        int flag = 0;

        if (c == '0')
        {
            flag = 1;

            if ((c = fgetc(fp)) == 'x' || c == 'X')
            {
                flag = 2;
            }
            ungetc(c, fp);
        }

        if (flag == 1)
        {
            //printf(" %c ",c);
            while ((c = fgetc(fp)) >= '0' && c <= '7')
            {
                //printf(" %c ",c);
                token_text[tokeni++] = c;
            }

            if (c == '8' || c == '9')
            {
                return ERROR_TOKEN;
            }
            else
            {
                ungetc(c, fp);
            }
        }
        else if (flag == 0)
        {
            while ((c = fgetc(fp)) >= '0' && c <= '9')
            {
                token_text[tokeni++] = c;
            }
            ungetc(c, fp);
        } //??????
        else
        {
            token_text[0] = '0';
            token_text[1] = fgetc(fp);
            while (((c = fgetc(fp)) >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
            {
                token_text[++tokeni] = c;
            }
            if (c == 'H' || c == 'h')
            {
                token_text[++tokeni] = c;
            }
            else
            {
                ungetc(c, fp);
            }
        }
        //????§Ø?
        c = fgetc(fp);

        if (c == 'u')
        {
            c = fgetc(fp);
            if (c == 'l')
            {
                if ((c = fgetc(fp)) != 'l')
                {
                    ungetc(c, fp);
                    return LONG_CONST;
                }
                return LONG_CONST;
            }
            else
            {
                ungetc(c, fp);
                return INT_CONST;
            }
        }
        else
        {

            ungetc(c, fp);
            return INT_CONST;
            //
            //???????token_text?§µ?????INT_CONST??
        }
    }

    switch (c)
    {
    case ((int)'='):
        c = fgetc(fp);
        if (c == '=') //==
        {
            return EQ;
        }
        ungetc(c, fp);
        return ASSIGN;
    case ((int)'{'):
        return BLP;
    case ((int)'}'):
        return BRP;
    case ((int)'['):
        return SLP;
    case ((int)']'):
        return SRP;
    case ((int)'('):
        cnt = 1;
        return LP;
    case ((int)')'):
        cnt = 1;
        return RP;

    case ((int)'+'):
        c = fgetc(fp);
        cnt = 1;
        if (c == '+') //++
        {
            return PLUSPLUS;
        }
        ungetc(c, fp);
        return PLUS;
    case ((int)'-'):
        cnt = 1;
        return MINUS;
    case ((int)'*'):
        cnt = 1;
        return MULTI;
    case ((int)'/'):
        if ((c = fgetc(fp)) == '/')
        {
            tokeni = 0;
            while ((c = fgetc(fp)) != '\n')
            {
                token_text[tokeni++] = c;
            }
            ungetc(c, fp);
            return ANNO;
        }
        else if (c == '*')
        {
            tokeni = 0;
            while ((c = fgetc(fp)) != EOF)
            {
                if (c == '*')
                {
                    if ((c = fgetc(fp)) == '/')
                    {
                        return ANNO;
                    }
                    else
                    {
                        ungetc(c, fp);
                    }
                }
                token_text[tokeni++] = c;
            }
            ungetc(c, fp);
            return ANNO;
        }
        ungetc(c, fp);
        cnt = 1;
        return DIVISION;
    case ((int)'%'):
        cnt = 1;
        return MOD;

    case ((int)'|'): //?cnt?
        c = fgetc(fp);
        if (c != '|')
        {
            return ERROR_TOKEN;
        }
        return OR;
    case ((int)'&'):
        c = fgetc(fp);
        if (c != '&')
        {
            return ERROR_TOKEN;
        }
        return AND;

    case ((int)'>'):
        if ((c = fgetc(fp)) == '=')
        {
            cnt = 1;
            return BIGGEREQUAL;
        }
        ungetc(c, fp);
        cnt = 1;
        return BIGGER;

    case ((int)'<'):
        cnt = 1;
        if ((c = fgetc(fp)) == '=')
        {
            return SMALLEREQUAL;
        }
        ungetc(c, fp);
        return SMALLER;

    case ((int)'!'):
        c = fgetc(fp);
        if (c == '=')
        {
            cnt = 1;
            return NO_EQ;
        }
        ungetc(c, fp);
        return NOT;

    case ((int)','):
        return COMMA;
    case ((int)';'):
        return SEMI;

    case ((int)'#'):
        i = 0;
        c = fgetc(fp);
        if (c == 'i')
        {
            for (int j = 1; j <= 6; j++)
            {
                c = fgetc(fp);
            }
            while ((c = fgetc(fp)) == ' ')
                ;
            if (c == '<')
            {
                do
                {
                    token_text[i++] = c;
                } while (((c = fgetc(fp)) >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '.');

                if (c != '>')
                {
                    printf(" ero");
                }

                token_text[i++] = c;
                return INCLUDE;
            }
            if (c == '"')
            {
                i = 0;
                do
                {
                    token_text[i++] = c;
                } while (((c = fgetc(fp)) >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || c == '.');
                if (c != '"')
                {
                    printf("ero");
                }
                token_text[i++] = c;
                return INCLUDE;
            }
        }
        else if (c == 'd')
        {
            for (int i = 1; i <= 5; i++)
            {
                c = fgetc(fp);
            }
            return DEFINE;
        }
        else
        {
            return ERROR_TOKEN;
        }
    default:
        if (feof(fp))
            return EOF;
        else
            return ERROR_TOKEN; //?????????????
    }
}

void traverse(Node p)
{
    depth++;
    if (p == NULL)
    {
        printf("\n");
        return;
    }

    switch (p->type)
    {
    case pretree:
        traverse(p->pretreep->preinformation);
        traverse(p->pretreep->extdefinformation);
        break;
    case extdeflist:
        //printf("?????????§µ?");
        traverse(p->extdeflistp->lchild);
        traverse(p->extdeflistp->rchild);
        break;
    case extvardef:
        printf("??????????:\n");
        printf("  ????: ");
        switch (p->extvardefp->kind)
        {
        case INT:
            printf("int\n");
            break;
        case CHAR:
            printf("char\n");
            break;
        case FLOAT:
            printf("float\n");
            break;
        }
        traverse(p->extvardefp->child);
        break;
    case extvarlist:
        if (p->extvarlistp->isar == 1)
        {
            printf("  ???úE??: \n");
            printf("    ID:  %s\n", p->extvarlistp->data);
        }
        else
        {
            printf("  ??????: \n");
            printf("    ID:  %s\n", p->extvarlistp->data);
        }
        traverse(p->extvarlistp->child);
        break;
    case funcdef:
        printf("???????‰Ø\n");
        traverse(p->funcdefp->lchild);
        printf("  ??????:    %s\n", p->funcdefp->name);
        traverse(p->funcdefp->mchild);
        traverse(p->funcdefp->rchild);
        break;
    case funkind:
        printf("  ????: ");
        switch (p->funkindp->type)
        {
        case INT:
            printf("int\n");
            break;
        case CHAR:
            printf("char\n");
            break;
        case FLOAT:
            printf("float\n");
            break;
        }
        break;
    case formal_parameterlist:
        traverse(p->formal_parameterlistp->lchild);
        traverse(p->formal_parameterlistp->rchild);
        break;
    case formal_parameter:
        printf("    ????:  ");
        switch (p->extvardefp->kind)
        {
        case INT:
            printf("int");
            break;
        case CHAR:
            printf("char");
            break;
        case FLOAT:
            printf("float");
            break;
        }
        printf("  ??????:  %s\n", p->formal_parameterp->data);
        return;
    case multisentence:
        printf("    ???????:  \n");
        printf("      ???????????????:  \n");
        traverse(p->multisentencep->lchild);
        printf("      ?????????????:  \n");
        traverse(p->multisentencep->rchild);
        break;
    case locvardeflist:
        traverse(p->locvardeflistp->lchild);
        traverse(p->locvardeflistp->rchild);
        break;
    case locvardef:
        printf("      ????:  ");
        switch (p->locvardefp->kind)
        {
        case INT:
            printf("int\n");
            break;
        case CHAR:
            printf("char\n");
            break;
        case FLOAT:
            printf("float\n");
            break;
        }
        printf("      VAR_NAME:  ");
        traverse(p->locvardefp->child);
        break;
    case locvarnamelist:
        printf("        %s\n", p->locvarnamelistp->data);
        traverse(p->locvarnamelistp->child);
        break;
    case sentencelist:
        traverse(p->sentencelistp->lchild);
        traverse(p->sentencelistp->rchild);
        break;
    case ifelsesentence:
        printf("       IF-ELSE???:\n");
        printf("        ????:\n");
        traverse(p->ifelsesentencep->condition);
        printf("        IF???:\n");
        traverse(p->ifelsesentencep->ifresult);
        printf("        ELSE???:\n");
        traverse(p->ifelsesentencep->elseresult);
        break;
    case ifsentence:
        printf("       IF???:\n");
        printf("        ????:\n");
        traverse(p->ifsentencep->condition);
        printf("        IF???:\n");
        traverse(p->ifsentencep->result);
        break;
    case opnum:
        if (p->opnump->kind == USEFUN)
        {
            printf("????????");
            printf("           ID:   %s\n", p->opnump->data);
            break;
        }
        printf("           ID:   %s\n", p->opnump->data);
        break;
    case opsign:
        switch (p->opsignp->kind)
        {
        case BIGGER:
            printf("         >\n");
            break;
        case SMALLER:
            printf("         <\n");
            break;
        case EQ:
            printf("         ==\n");
            break;
        case ASSIGN:
            printf("         ASSIGNOP\n");
            break;
        case PLUS:
            printf("         +\n");
            break;
        case PLUSPLUS:
            printf("         ++\n");
            break;
        }
        traverse(p->opsignp->lchild);
        traverse(p->opsignp->rchild);
        break;
    case whilesentence:
        printf("       WHILE???:\n");
        printf("        ????:\n");
        traverse(p->whilesentencep->condition);
        printf("        WHILE????§Ö???????:\n");
        traverse(p->whilesentencep->sentence);
        break;
    case dowhilesentence:
        printf("       DO-WHILE???:\n");
        printf("        DO-WHILE????§Ö???????:\n");
        traverse(p->dowhilesentencep->sentence);
        printf("        ????:\n");
        traverse(p->dowhilesentencep->condition);
        break;
    case forsentence:
        break;
    case var:
        printf("        ???????:\n");
        printf("        ID:  %s", p->varp->data);
        break;
    case includesentence:
        printf("??????????\n");
        printf("???????%s\n", p->includesentencep->data);
        traverse(p->includesentencep->child);
        break;
    case definesentence:
        printf("???‰Ø\n");
        printf("  ????%s\n", p->definesentencep->id);
        printf("  ??????%s\n", p->definesentencep->data);
        traverse(p->definesentencep->child);
        break;
    }
    depth--;
    return;
}
