#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   //fork
#include <sys/wait.h> //waitpid
#include <wait.h>
#include <sys/msg.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024

int getLastId(FILE *file, int poemNum)
{
    int lastId;
    char buff[BUFFER_SIZE];

    file = fopen("data.txt", "r");
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
    file = fopen("data.txt", "a");
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
void updatePoem(char **poems)
{

    int poemID;
    printf("\nAdd meg az ID-t! ");
    scanf("%d", &poemID);
    getchar();
    poemID -= 1;
    printf("Eredeti: %s\n", poems[poemID]);
    printf("Ird le/vagy masold be a modositott verset!\n");
    char buff[BUFFER_SIZE];

    fgets(buff, sizeof(buff), stdin);

    poems[poemID] = (char *)malloc((strlen(buff) + 1) * sizeof(char));
    char *newline = strchr(buff, '\n');
    *newline = '\0';
    strcpy(poems[poemID], buff);

    printf("Vers modositva \n");
}
// DELETE - Vers törlése
void deletePoem(char **poems, int poemNum)
{
    int poemID;
    printf("\nAdd meg az ID-t! ");
    scanf("%d", &poemID);
    getchar();
    poemID -= 1;

    for (int i = poemID; i < poemNum - 1; i++)
    {
        poems[i] = (char *)malloc((strlen(poems[i + 1]) + 1) * sizeof(char));
        strcpy(poems[i], poems[i + 1]);
    }

    printf("Vers torolve \n");
}

void savePomes(FILE *file, int poemNum, char **poems, int *poemsLen)
{
    if (poemNum == *poemsLen)
    {
        char **temp = realloc(poems, (poemNum + 5) * sizeof(char *));
        if (temp == NULL)
        {
            perror("Memory reallocation failed");
            return;
        }
        poems = temp;
        *poemsLen += 5;
    }

    char buff[BUFFER_SIZE];

    file = fopen("data.txt", "r");
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
    file = fopen("data.txt", "w");

    for (int i = 0; i < poemNum; i++)
    {
        fprintf(file, "| %d | ", i + 1);
        fprintf(file, "%s", poems[i]);
        fprintf(file, " |\n");
    }

    fclose(file);
}

// LOCSOLAS
struct message
{
    long mtype;
    char mtext[BUFFER_SIZE];
};

// sendig a message
int send(int msgQueue, char *poem, int size)
{
    const struct message msg = {5, poem};
    int status;

    status = msgsnd(msgQueue, &msg, size, 0);
    if (status < 0)
        perror("msgsnd");

    return 0;
}

int receive(int msgQueue)
{
    struct message msg;
    int status;
    status = msgrcv(msgQueue, &msg, BUFFER_SIZE, 5, 0);

    if (status < 0)
        perror("msgsnd");
    return 0;
}

void handler(int signumber)
{
    printf("Megérkeztem Mama!\n");
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    char childs[4][10] = {"Pufi", "Roli", "Hapsi", "Tapsi"};

    struct sigaction sigact;
    sigact.sa_handler = handler;
    sigaction(SIGUSR1, &sigact, NULL);

    FILE *fpi;
    fpi = fopen("data.txt", "r");

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
    int poemsLen = poemNum + 5;

    char **poems = (char **)malloc((poemsLen) * sizeof(char *));
    if (poems == NULL)
    {
        perror("Memory allocation failed");
        return 1;
    }

    savePomes(fpi, poemNum, poems, &poemsLen);

    pid_t pid;

    int pipefd[2];
    char vers1[1024]; // char array for reading from pipe
    char vers2[1024]; // char array for reading from pipe
    if (pipe(pipefd) == -1)
    {
        perror("Hiba a pipe nyitaskor!");
        exit(EXIT_FAILURE);
    }

    int msgQueue;
    int status;
    key_t key;

    key = ftok(argv[0], 1);
    msgQueue = msgget(key, 0600 | IPC_CREAT);
    if (msgQueue < 0)
    {
        perror("msgget");
        return 1;
    }

    while (1)
    {
        printf("\n-------------Menu-------------\n");
        printf("    [1] - Vers hozzaadasa \n");
        printf("    [2] - Versek listazasa \n");
        printf("    [3] - Vers modositasa \n");
        printf("    [4] - Vers torlese \n");
        printf("    [5] - Locsolas \n");
        printf("    [6] - Kilepes \n");
        printf("------------------------------\n");
        printf("    Versek aktualis szama: %d\n", poemNum);
        printf("------------------------------\n");
        printf("    Valsztott menupont: ");

        int menuitem;
        scanf("%d", &menuitem);
        getchar();
        printf("------------------------------\n");

        switch (menuitem)
        {
        case 1:
            printf("\n");
            addNewPoem(fpi, poemNum);
            poemNum++;
            savePomes(fpi, poemNum, poems, &poemsLen);
            printf("Vers hozzaadva \n");
            break;
        case 2:
            printPoems(poems, poemNum);
            break;
        case 3:
            printPoems(poems, poemNum);
            updatePoem(poems);
            writeFile(fpi, poemNum, poems);
            break;
        case 4:
            printPoems(poems, poemNum);
            deletePoem(poems, poemNum);
            poemNum--;
            writeFile(fpi, poemNum, poems);
            break;
        case 5:
            printf("\n");
            printf("Locsolas \n");
            int r = rand() % 4; // number between 0-3
            pid = fork();
            if (pid == -1)
            {
                perror("Fork hiba");
                exit(EXIT_FAILURE);
            }
            if (pid == 0) // child process
            {
                printf("Megyek Mama!\n");
                sleep(3);
                kill(getppid(), SIGUSR1);
                sleep(2);

                close(pipefd[1]);
                read(pipefd[0], vers1, BUFFER_SIZE);
                printf("Elso:\n %s \n", vers1);
                read(pipefd[0], vers2, BUFFER_SIZE);
                printf("Masodik:\n %s", vers2);
                close(pipefd[0]);
                printf("\n");
                char *poem;
                r = rand() % 10;
                if (r <= 4)
                {
                    poem = vers1;
                }
                else
                {
                    poem = vers2;
                }
                int size = strlen(poem) + 1;
                send(msgQueue, poem, size);
                printf("%s\nSzabad-e locsolni!", poem);
                return 0;
            }
            else // parent
            {
                printf("%s indulj locsolni!\n", childs[r]);
                pause();
                close(pipefd[0]);
                write(pipefd[1], poems[0], BUFFER_SIZE);
                write(pipefd[1], poems[1], BUFFER_SIZE);
                close(pipefd[1]);
                printf("Tessek itt van ket vers!\n");
                wait(NULL);
                receive(msgQueue);
            }

            break;

        case 6:
            printf("\n");
            // Free allocated memory
            for (int i = 0; i < poemNum; ++i)
            {
                free(poems[i]);
            }
            free(poems);
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
