#include <stdio.h>   // Libreria standard di input/output
#include <conio.h>   // Libreria per la gestione della console
#include <string.h>  // Libreria per la manipolazione delle stringhe
#include <stdlib.h>  // Libreria standard per funzioni di utilità generale

// Definizione di una struttura per memorizzare i dettagli di un contatto
struct contact {
    double ph;
    char name[20], add[20];
} list;

char name[20];         // Array per memorizzare temporaneamente il nome del contatto
FILE *fp, *ft;         // Puntatori a file per operazioni di lettura e scrittura
int i, n, ch, l, found;  // Variabili di controllo

// Funzione per pulire l'input rimanente
void flush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    do {
        system("cls");    // pulisco tutto e ristampo dopo
        printf("\n\t\t=========================================================\n\t\t\xB3\t\t  RUBRICA \t TELEFONICA  \t\t\xB3\n\t\t=========================================================");
        printf("\n\n\n\t\t\t\tMenu principale\n\t\t\t\t======================");
        printf("\n\t\t\t\t[1] Nuovo contatto\n\t\t\t\t[2] Lista contatti\n\t\t\t\t[3] Modifica contatto\n\t\t\t\t[4] Cancella contatto\n\t\t\t\t[0] Esci");
        printf("\n\t\t\t\t======================\n\t\t");
        printf("\n\t\t\t\tSeleziona una scelta:");
        scanf("%d", &ch);

        switch (ch) {
            case 0: // ultimo 
                system("cls");
                printf("\n\n\t\tBYE BYE");
                break;

            /* ********************* Nuovo contatto ************ */
            case 1:
                system("cls");
                printf("\n\t\t=========================================================\n\t\t\xB3\t\t  NUOVO \t CONTATTO  \t\t\xB3\n\t\t=========================================================");
                if ((fp = fopen("contact.dll", "a")) == NULL) {
                    perror("Errore apertura file");
                    exit(EXIT_FAILURE);
                }
                while (1) {
                    flush_stdin();
                    printf("\n\t\t[Per uscire premere spazio e invio dopo il nome]");
                    printf("\n\n\t\tNome: ");
                    fgets(list.name, sizeof(list.name), stdin);
                    if (strcmp(list.name, " \n") == 0) // così ritorno al menu 
                        break;
                    list.name[strcspn(list.name, "\n")] = 0; // Rimuovi newline

                    printf("\n\t\tNumero: ");
                    if (scanf("%lf", &list.ph) != 1) {
                        flush_stdin(); // pulisce l'input rimanente
                        printf("\nNumero non valido. Riprova.\n");
                        continue;
                    }

                    flush_stdin();
                    printf("\n\t\tIndirizzo: ");
                    fgets(list.add, sizeof(list.add), stdin);
                    list.add[strcspn(list.add, "\n")] = 0; // Rimuovi newline

                    fwrite(&list, sizeof(list), 1, fp); // annoto quante ce ne sono
                }
                fclose(fp);
                break;

            /* ********************* Lista dei contatti ************************* */
            case 2:
                system("cls");
                printf("\n\t\t=========================================================\n\t\t\xB3\t\t  LISTA \t CONTATTI  \t\t\xB3\n\t\t=========================================================");
                printf("\n\t\t\t\tNome\tNumero\tIndirizzo\n\t\t=========================================================\n");

                for (i = 97; i <= 122; i = i + 1) {
                    if ((fp = fopen("contact.dll", "r")) == NULL) {
                        perror("Errore apertura file");
                        exit(EXIT_FAILURE);
                    }
                    fflush(stdin);
                    found = 0;
                    while (fread(&list, sizeof(list), 1, fp) == 1) // legge in binari
                    {
                        if (list.name[0] == i || list.name[0] == i - 32) {
                            printf("\n\t\tNome: %s\n\t\tNumero: %.0lf\n\t\tIndirizzo: %s \n\t\t=========================================================", list.name, list.ph, list.add);
                            found++;
                        }
                    }
                    if (found != 0) {
                        printf("\n\t\t [%c]-(%d)\n\n", i - 32, found);
                        getch();
                    }
                    fclose(fp);
                }
                break;

            /* ********************* Modifica contatti ************************/
            case 3:
                system("cls");
                if ((fp = fopen("contact.dll", "r")) == NULL) {
                    perror("Errore apertura file");
                    exit(EXIT_FAILURE);
                }
                if ((ft = fopen("temp.dat", "w")) == NULL) {
                    perror("Errore apertura file temporaneo");
                    fclose(fp);
                    exit(EXIT_FAILURE);
                }
                fflush(stdin);
                printf("\n\t\t=========================================================\n\t\t\xB3\t\t  MODIFICA \t CONTATTO  \t\t\xB3\n\t\t=========================================================");
                printf("\n\n\t\t Contatto da modificare: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0; // Rimuovi newline
                while (fread(&list, sizeof(list), 1, fp) == 1) {
                    if (stricmp(name, list.name) != 0) // verifico 
                    {
                        fwrite(&list, sizeof(list), 1, ft);
                    }
                }
                fflush(stdin);
                system("cls");
                printf("\n\n\t\t\t\tModifica di '%s'\n", name);
                printf("\n\t\t\t\tNome: ");
                fgets(list.name, sizeof(list.name), stdin);
                list.name[strcspn(list.name, "\n")] = 0; // Rimuovi newline

                printf("\n\t\t\t\tNumero: ");
                if (scanf("%lf", &list.ph) != 1) {
                    flush_stdin(); // pulisce l'input rimanente
                    printf("\nNumero non valido. Modifica annullata.\n");
                    continue;
                }

                flush_stdin();
                printf("\n\t\t\t\tIndirizzo: ");
                fgets(list.add, sizeof(list.add), stdin);
                list.add[strcspn(list.add, "\n")] = 0; // Rimuovi newline

                printf("\n");
                fwrite(&list, sizeof(list), 1, ft);
                fclose(fp);
                fclose(ft);
                remove("contact.dll");
                rename("temp.dat", "contact.dll");
                break;

            /* ******************** Cancello **********************/
            case 4:
                system("cls");
                fflush(stdin);
                printf("\n\t\t=========================================================\n\t\t\xB3\t\t  ELIMINA \t CONTATTO  \t\t\xB3\n\t\t=========================================================");
                printf("\n\n\t\t\t\tEliminazione dell'account ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0; // Rimuovi newline

                if ((fp = fopen("contact.dll", "r")) == NULL) {
                    perror("Errore apertura file");
                    exit(EXIT_FAILURE);
                }
                if ((ft = fopen("temp.dat", "w")) == NULL) {
                    perror("Errore apertura file temporaneo");
                    fclose(fp);
                    exit(EXIT_FAILURE);
                }
                while (fread(&list, sizeof(list), 1, fp) == 1) {
                    if (stricmp(name, list.name) != 0) {
                        fwrite(&list, sizeof(list), 1, ft);
                    }
                }
                fclose(fp);
                fclose(ft);
                remove("contact.dll");
                rename("temp.dat", "contact.dll");
                break;
        }
    } while (ch != 0);
    return 0;
}
