#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024

int getLastId(FILE *file, int poemNum)
{
    int lastId;
    char buff[BUFFER_SIZE];

    file = fopen("test.txt", "r");
    int currindex;
    for (int i = 0; i < poemNum; i++)
    {

        // Read first '|'
        fscanf(file, "%s", buff);

        // Read poem id
        fscanf(file, "%d", &currindex);
        if (i == poemNum - 1)
        {
            lastId = currindex;
        }

        // Read second '|'
        fscanf(file, "%s", buff);

        do
        {
            fscanf(file, "%s", buff);

        } while (strcmp(buff, "|"));
    }

    fclose(file);
    return lastId;
}

// CREATE - Vers hozzáadaása
void addNewPoem(FILE *file, int poemNum)
{
    file = fopen("test.txt", "a");
    char newPoem[BUFFER_SIZE];

    // Give ID to poem
    int id = getLastId(file, poemNum);
    fprintf(file, "\n| %d | ", ++id);

    // Get poem from console
    printf("Ird le/vagy masold be a verset!\n");
    fgets(newPoem, BUFFER_SIZE - 2, stdin);

    // Change end character
    char *newline = strchr(newPoem, '\n');
    *newline = ' ';
    *(newline + 1) = '|';
    newPoem[BUFFER_SIZE] = '\0';

    // Write to file
    fprintf(file, "%s", newPoem);
    fclose(file);
}

// READ - Versek listázása
void printPoems(char **poems, int poemNum)
{
    printf("\nVersek: \n");
    for (int i = 0; i < poemNum; i++)
    {
        printf("%d -%s\n", (i + 1), poems[i]);
    }
}

// UPDATE - Vers módosítása
void updatePoem()
{
}
// DELETE - Vers törlése
void deletePoem()
{
}

void savePomes(FILE *file, int poemNum, char **poems)
{

    char buff[BUFFER_SIZE];

    file = fopen("test.txt", "r");
    int currindex;
    for (int i = 0; i < poemNum; i++)
    {

        // Beolvas elso |
        fscanf(file, "%s", buff);

        // Beolvas poem id
        fscanf(file, "%d", &currindex);

        // Beolvas masodik |
        fscanf(file, "%s", buff);

        fgets(buff, sizeof(buff), file);

        poems[i] = (char *)malloc((strlen(buff) + 1) * sizeof(char));
        strcpy(poems[i], buff);

        char *newline = strchr(poems[i], '|');
        *(newline - 1) = '\0';
    }
    fclose(file);
}

void writeFile(FILE *file, int poemNum, char **poems)
{
    file = fopen("test.txt", "w");

    for (int i = 0; i < poemNum; i++)
    {
        fprintf(file, "| %d | ", i + 1);
        fprintf(file, "%s", poems[i]);
        fprintf(file, " |\n");
    }

    fclose(file);
}

int main()
{
    FILE *fpi;
    fpi = fopen("test.txt", "r");

    int db = 0;
    char buff[BUFFER_SIZE];
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

    char **poems = (char **)malloc((poemNum + 20) * sizeof(char *));
    if (poems == NULL)
    {
        perror("Memory allocation failed");
        return 1;
    }

    savePomes(fpi, poemNum, poems);

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
            addNewPoem(fpi, poemNum);
            poemNum++;
            savePomes(fpi, poemNum, poems);
            printf("Vers hozzaadva \n");
            break;
        case 2:
            printPoems(poems, poemNum);
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

    // Free allocated memory
    for (int i = 0; i < poemNum; ++i)
    {
        free(poems[i]);
    }
    free(poems);
    return 0;
}
