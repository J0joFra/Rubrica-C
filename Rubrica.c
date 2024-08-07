#include <stdio.h>   // Libreria standard di input/output
#include <conio.h>   // Libreria per la gestione della console
#include <string.h>  // Libreria per la manipolazione delle stringhe
#include <stdlib.h>  // Libreria standard per funzioni di utilità generale

// Definizione di una struttura per memorizzare i dettagli di un contatto
struct contact {
    double ph;          // Numero di telefono
    char name[20];      // Nome del contatto
    char add[20];       // Indirizzo del contatto
} list;                // Variabile globale di tipo struct contact

char name[20];         // Array per memorizzare temporaneamente il nome del contatto
FILE *fp, *ft;         // Puntatori a file per operazioni di lettura e scrittura
int i, n, ch, l, found; // Variabili di controllo

// Funzione per pulire l'input rimanente
void flush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Funzione principale del programma
int main() {
    do {
        system("cls");    // Pulisce lo schermo della console
        // Stampa il menu principale
        printf("\n\t\t=========================================================\n\t\t\xB3\t\t  RUBRICA \t TELEFONICA  \t\t\xB3\n\t\t=========================================================");
        printf("\n\n\n\t\t\t\tMenu principale\n\t\t\t\t======================");
        printf("\n\t\t\t\t[1] Nuovo contatto\n\t\t\t\t[2] Lista contatti\n\t\t\t\t[3] Modifica contatto\n\t\t\t\t[4] Cancella contatto\n\t\t\t\t[0] Esci");
        printf("\n\t\t\t\t======================\n\t\t");
        printf("\n\t\t\t\tSeleziona una scelta:");
        scanf("%d", &ch);  // Legge la scelta dell'utente

        switch (ch) {
            case 0: // Uscita dal programma
                system("cls");
                printf("\n\n\t\tBYE BYE");
                break;

            /* ********************* Nuovo contatto ************ */
            case 1:
                system("cls");
                printf("\n\t\t=========================================================\n\t\t\xB3\t\t  NUOVO \t CONTATTO  \t\t\xB3\n\t\t=========================================================");
                // Apre il file contact.dll in modalità aggiunta
                if ((fp = fopen("contact.dll", "a")) == NULL) {
                    perror("Errore apertura file");
                    exit(EXIT_FAILURE);
                }
                while (1) {
                    flush_stdin();  // Pulisce l'input rimanente
                    printf("\n\t\t[Per uscire premere spazio e invio dopo il nome]");
                    printf("\n\n\t\tNome: ");
                    fgets(list.name, sizeof(list.name), stdin);  // Legge il nome del contatto
                    if (strcmp(list.name, " \n") == 0) // Se l'utente inserisce solo uno spazio, esce
                        break;
                    list.name[strcspn(list.name, "\n")] = 0; // Rimuove il newline

                    printf("\n\t\tNumero: ");
                    if (scanf("%lf", &list.ph) != 1) {  // Legge il numero di telefono
                        flush_stdin(); // Pulisce l'input rimanente
                        printf("\nNumero non valido. Riprova.\n");
                        continue;
                    }

                    flush_stdin();  // Pulisce l'input rimanente
                    printf("\n\t\tIndirizzo: ");
                    fgets(list.add, sizeof(list.add), stdin);  // Legge l'indirizzo del contatto
                    list.add[strcspn(list.add, "\n")] = 0; // Rimuove il newline

                    fwrite(&list, sizeof(list), 1, fp); // Scrive il contatto nel file
                }
                fclose(fp);  // Chiude il file
                break;

            /* ********************* Lista dei contatti ************************* */
            case 2:
                system("cls");
                printf("\n\t\t=========================================================\n\t\t\xB3\t\t  LISTA \t CONTATTI  \t\t\xB3\n\t\t=========================================================");
                printf("\n\t\t\t\tNome\tNumero\tIndirizzo\n\t\t=========================================================\n");

                // Ciclo attraverso le lettere dell'alfabeto
                for (i = 97; i <= 122; i = i + 1) {
                    if ((fp = fopen("contact.dll", "r")) == NULL) {
                        perror("Errore apertura file");
                        exit(EXIT_FAILURE);
                    }
                    fflush(stdin);
                    found = 0;  // Inizializza il contatore dei contatti trovati
                    while (fread(&list, sizeof(list), 1, fp) == 1) { // Legge i contatti dal file
                        // Stampa il contatto se il nome inizia con la lettera corrente (i) o la sua maiuscola (i - 32)
                        if (list.name[0] == i || list.name[0] == i - 32) {
                            printf("\n\t\tNome: %s\n\t\tNumero: %.0lf\n\t\tIndirizzo: %s \n\t\t=========================================================", list.name, list.ph, list.add);
                            found++;
                        }
                    }
                    // Se sono stati trovati contatti, stampa il numero di contatti trovati
                    if (found != 0) {
                        printf("\n\t\t [%c]-(%d)\n\n", i - 32, found);
                        getch();  // Attende un input dall'utente
                    }
                    fclose(fp);  // Chiude il file
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
                fgets(name, sizeof(name), stdin);  // Legge il nome del contatto da modificare
                name[strcspn(name, "\n")] = 0; // Rimuove il newline
                while (fread(&list, sizeof(list), 1, fp) == 1) {
                    if (stricmp(name, list.name) != 0) { // Se il nome non corrisponde, scrive il contatto nel file temporaneo
                        fwrite(&list, sizeof(list), 1, ft);
                    }
                }
                fflush(stdin);
                system("cls");
                printf("\n\n\t\t\t\tModifica di '%s'\n", name);
                printf("\n\t\t\t\tNome: ");
                fgets(list.name, sizeof(list.name), stdin);  // Legge il nuovo nome del contatto
                list.name[strcspn(list.name, "\n")] = 0; // Rimuove il newline

                printf("\n\t\t\t\tNumero: ");
                if (scanf("%lf", &list.ph) != 1) {  // Legge il nuovo numero di telefono
                    flush_stdin(); // Pulisce l'input rimanente
                    printf("\nNumero non valido. Modifica annullata.\n");
                    continue;
                }

                flush_stdin();  // Pulisce l'input rimanente
                printf("\n\t\t\t\tIndirizzo: ");
                fgets(list.add, sizeof(list.add), stdin);  // Legge il nuovo indirizzo
                list.add[strcspn(list.add, "\n")] = 0; // Rimuove il newline

                printf("\n");
                fwrite(&list, sizeof(list), 1, ft); // Scrive il nuovo contatto nel file temporaneo
                fclose(fp);  // Chiude il file originale
                fclose(ft);  // Chiude il file temporaneo
                remove("contact.dll");  // Elimina il file originale
                rename("temp.dat", "contact.dll");  // Rinomina il file temporaneo come file originale
                break;

            /* ******************** Cancello **********************/
            case 4:
                system("cls");
                fflush(stdin);
                printf("\n\t\t=========================================================\n\t\t\xB3\t\t  ELIMINA \t CONTATTO  \t\t\xB3\n\t\t=========================================================");
                printf("\n\n\t\t\t\tEliminazione dell'account ");
                fgets(name, sizeof(name), stdin);  // Legge il nome del contatto da eliminare
                name[strcspn(name, "\n")] = 0; // Rimuove il newline

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
                    if (stricmp(name, list.name) != 0) { // Se il nome non corrisponde, scrive il contatto nel file temporaneo
                        fwrite(&list, sizeof(list), 1, ft);
                    }
                }
                fclose(fp);  // Chiude il file originale
                fclose(ft);  // Chiude il file temporaneo
                remove("contact.dll");  // Elimina il file originale
                rename("temp.dat", "contact.dll");  // Rinomina il file temporaneo come file originale
                break;
        }
    } while (ch != 0);  // Ripete il ciclo finché l'utente non sceglie di uscire (ch == 0)
    return 0;
}
