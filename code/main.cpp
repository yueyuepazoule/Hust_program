#include "LA.cpp"
FILE* fpx= fopen("output.txt", "w");
typedef struct token_link
{
    char data[1000];
    struct token_link *next;
} Tlink;

Tlink *readline()
{
    w = gettoken();
    if (w == -1)
    {
        return NULL;
    }

    Tlink *root = NULL, *tail = NULL;
    root = (Tlink *)malloc(sizeof(Tlink));
    char *token_texty = (char *)malloc(25 * sizeof(char));
    strcpy(token_texty, token_text);
    strcpy(root->data, token_texty);
    root->next = NULL;
    tail = root;
    char c;
    while ((c = fgetc(fp)) != '\n')
    {
        ungetc(c, fp);
        gettoken();
        tail->next = (Tlink *)malloc(sizeof(Tlink));
        tail = tail->next;
        token_texty = (char *)malloc(25 * sizeof(char));
        strcpy(token_texty, token_text);
        strcpy(root->data, token_texty);
        tail->next = NULL;
    }
    return root;
}

void Format()
{
    int tabs = 0;
    int flag = 0;
    Tlink *root, *tail, *p;

    while (1)
    {
        root = readline();

        if (root == NULL)
        {
            break;
        }

        tail = root;

        while (tail->next != NULL)
        {
            tail = tail->next;
        }
        if ((root == tail) && (strcmp(tail->data, "}") == 0))
        {
            tabs--;
        }
        if ((root != tail) && (strcmp(root->data, "}") == 0))
        {
            tabs--;
        }
        for (int i = 0; i < tabs; i++)
        {
            fprintf(fpx,"    ");
        }
        if (flag == 1)
        {
            tabs--;
            flag = 0;
        }
        p = root;
        while (p != NULL)
        {
            fprintf(fpx,"%s ", p->data);
            p = p->next;
        }
        fprintf(fpx,"\n");
        if (strcmp(tail->data, "{") == 0)
        {
            tabs++;
        }
        p = root;
        while (p != NULL)
        {
            if ((strcmp(p->data, "if") == 0) || (strcmp(p->data, "for") == 0) ||
                (strcmp(p->data, "while") == 0))
            {
                if (strcmp(tail->data, "{") != 0 && strcmp(tail->data, ";") != 0)
                {
                    tabs++;
                    flag = 1;
                }
            }
            p = p->next;
        }
    }
}

Node R;

int main()
{
    int op;
    fprintf(fpx," +----------------------------+\n");
    fprintf(fpx," |       Function List        |\n");
    fprintf(fpx," |    1. Lexical Analysis     |\n");
    fprintf(fpx," |    2. Syntax Analysis      |\n");
    fprintf(fpx," |    3. Show Formated File   |\n");
    fprintf(fpx," |    0. Exit                 |\n");
    fprintf(fpx," +----------------------------+\n");
    scanf("%d", &op);

    while (op != 0)
    {
        switch (op)
        {
        case 1:
            line = 1;
            fp = fopen("file.txt", "r");
            //TokenTraverse();
            fclose(fp);
            break;
        case 2:
            line = 1;
            fp = fopen("file.txt", "r");
            R = program();
            fclose(fp);
            break;
        case 3:
            line = 1;
            fp = fopen("file.txt", "r");
            Format();
            printf("EENNDD!!!!!!");
            fclose(fp);
            break;
        }
        fprintf(fpx," +----------------------------+\n");
        fprintf(fpx," |       Function List        |\n");
        fprintf(fpx," |    1. Lexical Analysis     |\n");
        fprintf(fpx," |    2. Syntax Analysis      |\n");
        fprintf(fpx," |    3. Show Formated File   |\n");
        fprintf(fpx," |    0. Exit                 |\n");
        fprintf(fpx," +----------------------------+\n");
        scanf("%d", &op);
    }
}
