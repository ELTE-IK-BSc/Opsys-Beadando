#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// READ - Versek listázása
void printPoems(FILE *file, int poemNum)
{
    char buff[1024];

    file = fopen("test.txt", "r+");
    int currindex;
    for (int i = 0; i < poemNum; i++)
    {
        printf("\n");

        // Beolvas elso |
        fscanf(file, "%s", buff);

        // Beolvas poem id
        fscanf(file, "%d", &currindex);
        printf("ID: %d\n", currindex);

        // Beolvas masodik |
        fscanf(file, "%s", buff);

        do
        {
            fscanf(file, "%s", buff);
            if (strcmp(buff, "|"))
            {
                printf("%s ", buff);
            }
        } while (strcmp(buff, "|"));

        printf("\n");
    }

    fclose(file);
}

int main()
{
    FILE *fpi;
    fpi = fopen("test.txt", "r+");

    int db = 0;
    char buff[1024];
    while (fscanf(fpi, "%s", buff) == 1)
    {
        if (!strcmp(buff, "|"))
        {
            db++;
        }
    }
    fclose(fpi);
    int poemNum = db / 3;
    printf("Versek szama: %d", poemNum);

    // CREATE - Vers hozzáadaása

    // UPDATE - Vers módosítása
    // DELETE - Vers törlése

    while (1)
    {
        printf("\n");
        printf("---------Menu---------\n");
        printf("[1] - Vers hozzaadasa \n");
        printf("[2] - Versek listazasa \n");
        printf("[3] - Vers modositasa \n");
        printf("[4] - Vers torlese \n");
        printf("[5] - Kilepes \n");
        printf("----------------------\n");

        printf("Valsztott menupont: ");

        int menuitem;
        scanf("%d", &menuitem);
        getchar();
        switch (menuitem)
        {
        case 1:
            printf("\n");

            printf("Vers hozzaadva \n");
            break;
        case 2:
            printf("\n");
            printf("Versek: \n");
            printPoems(fpi, poemNum);
            break;
        case 3:
            printf("\n");

            printf("Vers modositva \n");
            break;
        case 4:
            printf("\n");
            printf("Vers torlve \n");
            break;
        case 5:
            printf("\n");
            printf("Kilepes \n");
            return 0;

        default:
            printf("\n");
            printf("Isemretlen menupont probald ujra! \n");
            break;
        }
    }

    return 0;
}
