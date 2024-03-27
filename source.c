#include <stdio.h>

int main()
{
    // CREATE - Vers hozzáadaása
    // READ - Versek listázása
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
        switch (menuitem)
        {
        case 1:
            printf("\n");

            printf("Vers hozzaadva \n");
            break;
        case 2:
            printf("\n");
            printf("Versek: \n");
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
