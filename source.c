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

        printf("Valsztott menu: ");
        printf("\n");

        int menuitem;
        scanf("%d", &menuitem);
        switch (menuitem)
        {
        case 1:
            printf("Vers hozzaadva \n");
            break;
        case 2:
            printf("Versek: \n");
            break;
        case 3:
            printf("Vers modositva \n");
            break;
        case 4:
            printf("Vers torlve \n");
            break;
        case 5:
            printf("Kilepes \n");
            return;

        default:
            break;
        }
    }

    return 0;
}
