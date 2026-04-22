/*
 * Rubrica Telefonica - Versione migliorata
 * Miglioramenti rispetto all'originale:
 *  - Numero di telefono come stringa (non double)
 *  - Campi più lunghi e realistici
 *  - Ricerca per nome (parziale, case-insensitive)
 *  - Ricerca per numero di telefono
 *  - Email aggiunta come campo
 *  - Visualizzazione conteggio contatti
 *  - Validazione input migliorata
 *  - Gestione errori file robusta
 *  - Ordinamento alfabetico nella lista
 *  - File .dat invece di .dll
 *  - Pulizia buffer input sicura
 *  - Menu più chiaro con separatori
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ── Costanti ── */
#define MAX_NOME    50
#define MAX_TEL     20
#define MAX_IND     80
#define MAX_EMAIL   60
#define FILE_DB     "rubrica.dat"
#define FILE_TEMP   "rubrica_temp.dat"

/* ── Struttura contatto ── */
typedef struct {
    char nome[MAX_NOME];
    char telefono[MAX_TEL];
    char indirizzo[MAX_IND];
    char email[MAX_EMAIL];
} Contatto;

/* ── Prototipi ── */
void    pulisci_buffer(void);
void    stampa_separatore(void);
void    stampa_contatto(const Contatto *c, int idx);
int     conta_contatti(void);
void    aggiungi_contatto(void);
void    lista_contatti(void);
void    cerca_contatto(void);
void    modifica_contatto(void);
void    cancella_contatto(void);
int     confronta_contatti(const void *a, const void *b);
void    str_lower(const char *src, char *dst, int len);

/* ════════════════════════════════════════════════════
   Utilità
   ════════════════════════════════════════════════════ */

void pulisci_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void stampa_separatore(void) {
    printf("══════════════════════════════════════\n");
}

void str_lower(const char *src, char *dst, int len) {
    int i;
    for (i = 0; i < len - 1 && src[i]; i++)
        dst[i] = (char)tolower((unsigned char)src[i]);
    dst[i] = '\0';
}

void stampa_contatto(const Contatto *c, int idx) {
    if (idx > 0)
        printf("\n  [%d]\n", idx);
    else
        printf("\n");
    printf("  Nome      : %s\n", c->nome);
    printf("  Telefono  : %s\n", c->telefono);
    if (c->indirizzo[0] != '\0')
        printf("  Indirizzo : %s\n", c->indirizzo);
    if (c->email[0] != '\0')
        printf("  Email     : %s\n", c->email);
}

int conta_contatti(void) {
    FILE *fp = fopen(FILE_DB, "rb");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return (int)(size / sizeof(Contatto));
}

/* Comparatore per qsort (ordinamento alfabetico per nome) */
int confronta_contatti(const void *a, const void *b) {
    const Contatto *ca = (const Contatto *)a;
    const Contatto *cb = (const Contatto *)b;
    char na[MAX_NOME], nb[MAX_NOME];
    str_lower(ca->nome, na, MAX_NOME);
    str_lower(cb->nome, nb, MAX_NOME);
    return strcmp(na, nb);
}

/* ════════════════════════════════════════════════════
   Aggiungi contatto
   ════════════════════════════════════════════════════ */

void aggiungi_contatto(void) {
    Contatto c;
    FILE *fp;

    stampa_separatore();
    printf("  NUOVO CONTATTO\n");
    stampa_separatore();

    /* Nome (obbligatorio) */
    do {
        printf("  Nome e Cognome : ");
        if (!fgets(c.nome, MAX_NOME, stdin)) return;
        c.nome[strcspn(c.nome, "\n")] = '\0';
        if (c.nome[0] == '\0')
            printf("  ⚠ Il nome non può essere vuoto.\n");
    } while (c.nome[0] == '\0');

    /* Telefono (obbligatorio) */
    do {
        printf("  Telefono       : ");
        if (!fgets(c.telefono, MAX_TEL, stdin)) return;
        c.telefono[strcspn(c.telefono, "\n")] = '\0';
        if (c.telefono[0] == '\0')
            printf("  ⚠ Il telefono non può essere vuoto.\n");
    } while (c.telefono[0] == '\0');

    /* Indirizzo (opzionale) */
    printf("  Indirizzo      : ");
    if (!fgets(c.indirizzo, MAX_IND, stdin)) return;
    c.indirizzo[strcspn(c.indirizzo, "\n")] = '\0';

    /* Email (opzionale) */
    printf("  Email          : ");
    if (!fgets(c.email, MAX_EMAIL, stdin)) return;
    c.email[strcspn(c.email, "\n")] = '\0';

    fp = fopen(FILE_DB, "ab");
    if (!fp) {
        printf("  ✗ Errore: impossibile aprire il file.\n");
        return;
    }
    fwrite(&c, sizeof(Contatto), 1, fp);
    fclose(fp);

    printf("\n  ✓ Contatto '%s' aggiunto con successo.\n", c.nome);
}

/* ════════════════════════════════════════════════════
   Lista contatti (ordinata alfabeticamente)
   ════════════════════════════════════════════════════ */

void lista_contatti(void) {
    FILE *fp;
    int n, i;
    Contatto *arr;

    n = conta_contatti();
    stampa_separatore();
    printf("  LISTA CONTATTI  (%d totali)\n", n);
    stampa_separatore();

    if (n == 0) {
        printf("  Rubrica vuota.\n");
        return;
    }

    arr = (Contatto *)malloc(n * sizeof(Contatto));
    if (!arr) { printf("  ✗ Memoria insufficiente.\n"); return; }

    fp = fopen(FILE_DB, "rb");
    if (!fp) { free(arr); printf("  ✗ Impossibile aprire il file.\n"); return; }
    fread(arr, sizeof(Contatto), n, fp);
    fclose(fp);

    qsort(arr, n, sizeof(Contatto), confronta_contatti);

    for (i = 0; i < n; i++)
        stampa_contatto(&arr[i], i + 1);

    free(arr);
}

/* ════════════════════════════════════════════════════
   Cerca contatto (per nome parziale o telefono)
   ════════════════════════════════════════════════════ */

void cerca_contatto(void) {
    FILE *fp;
    char query[MAX_NOME], qlow[MAX_NOME], nlow[MAX_NOME];
    Contatto c;
    int trovati = 0;

    stampa_separatore();
    printf("  CERCA CONTATTO\n");
    stampa_separatore();
    printf("  Inserisci nome (parziale) o numero: ");
    if (!fgets(query, MAX_NOME, stdin)) return;
    query[strcspn(query, "\n")] = '\0';
    if (query[0] == '\0') { printf("  ⚠ Query vuota.\n"); return; }

    str_lower(query, qlow, MAX_NOME);

    fp = fopen(FILE_DB, "rb");
    if (!fp) { printf("  Rubrica vuota.\n"); return; }

    while (fread(&c, sizeof(Contatto), 1, fp) == 1) {
        str_lower(c.nome, nlow, MAX_NOME);
        if (strstr(nlow, qlow) || strstr(c.telefono, query)) {
            stampa_contatto(&c, 0);
            trovati++;
        }
    }
    fclose(fp);

    if (trovati == 0)
        printf("  Nessun contatto trovato per \"%s\".\n", query);
    else
        printf("\n  (%d risultato/i)\n", trovati);
}

/* ════════════════════════════════════════════════════
   Modifica contatto
   ════════════════════════════════════════════════════ */

void modifica_contatto(void) {
    FILE *fp, *tmp;
    char nome_cerca[MAX_NOME], nlow[MAX_NOME], clow[MAX_NOME];
    Contatto c;
    int trovato = 0;

    stampa_separatore();
    printf("  MODIFICA CONTATTO\n");
    stampa_separatore();
    printf("  Nome contatto da modificare: ");
    if (!fgets(nome_cerca, MAX_NOME, stdin)) return;
    nome_cerca[strcspn(nome_cerca, "\n")] = '\0';
    if (nome_cerca[0] == '\0') return;

    str_lower(nome_cerca, nlow, MAX_NOME);

    fp = fopen(FILE_DB, "rb");
    if (!fp) { printf("  Rubrica vuota.\n"); return; }
    tmp = fopen(FILE_TEMP, "wb");
    if (!tmp) { fclose(fp); printf("  ✗ Errore file temporaneo.\n"); return; }

    while (fread(&c, sizeof(Contatto), 1, fp) == 1) {
        str_lower(c.nome, clow, MAX_NOME);
        if (!trovato && strcmp(clow, nlow) == 0) {
            trovato = 1;
            printf("\n  Contatto trovato:\n");
            stampa_contatto(&c, 0);
            printf("\n  Inserisci i nuovi dati (INVIO per mantenere il valore):\n\n");

            /* Nome */
            char buf[MAX_NOME];
            printf("  Nome [%s]: ", c.nome);
            fgets(buf, MAX_NOME, stdin);
            buf[strcspn(buf, "\n")] = '\0';
            if (buf[0] != '\0') strncpy(c.nome, buf, MAX_NOME - 1);

            /* Telefono */
            printf("  Telefono [%s]: ", c.telefono);
            fgets(buf, MAX_TEL, stdin);
            buf[strcspn(buf, "\n")] = '\0';
            if (buf[0] != '\0') strncpy(c.telefono, buf, MAX_TEL - 1);

            /* Indirizzo */
            printf("  Indirizzo [%s]: ", c.indirizzo);
            char buf2[MAX_IND];
            fgets(buf2, MAX_IND, stdin);
            buf2[strcspn(buf2, "\n")] = '\0';
            if (buf2[0] != '\0') strncpy(c.indirizzo, buf2, MAX_IND - 1);

            /* Email */
            printf("  Email [%s]: ", c.email);
            char buf3[MAX_EMAIL];
            fgets(buf3, MAX_EMAIL, stdin);
            buf3[strcspn(buf3, "\n")] = '\0';
            if (buf3[0] != '\0') strncpy(c.email, buf3, MAX_EMAIL - 1);
        }
        fwrite(&c, sizeof(Contatto), 1, tmp);
    }
    fclose(fp);
    fclose(tmp);

    if (!trovato) {
        remove(FILE_TEMP);
        printf("  ✗ Contatto \"%s\" non trovato.\n", nome_cerca);
        return;
    }

    remove(FILE_DB);
    rename(FILE_TEMP, FILE_DB);
    printf("\n  ✓ Contatto aggiornato con successo.\n");
}

/* ════════════════════════════════════════════════════
   Cancella contatto
   ════════════════════════════════════════════════════ */

void cancella_contatto(void) {
    FILE *fp, *tmp;
    char nome_cerca[MAX_NOME], nlow[MAX_NOME], clow[MAX_NOME];
    Contatto c;
    int trovato = 0;
    char conferma;

    stampa_separatore();
    printf("  CANCELLA CONTATTO\n");
    stampa_separatore();
    printf("  Nome contatto da eliminare: ");
    if (!fgets(nome_cerca, MAX_NOME, stdin)) return;
    nome_cerca[strcspn(nome_cerca, "\n")] = '\0';
    if (nome_cerca[0] == '\0') return;

    str_lower(nome_cerca, nlow, MAX_NOME);

    /* Prima verifica che esista */
    fp = fopen(FILE_DB, "rb");
    if (!fp) { printf("  Rubrica vuota.\n"); return; }
    while (fread(&c, sizeof(Contatto), 1, fp) == 1) {
        str_lower(c.nome, clow, MAX_NOME);
        if (strcmp(clow, nlow) == 0) { trovato = 1; break; }
    }
    fclose(fp);

    if (!trovato) {
        printf("  ✗ Contatto \"%s\" non trovato.\n", nome_cerca);
        return;
    }

    printf("  Eliminare '%s'? (s/n): ", c.nome);
    conferma = (char)getchar();
    pulisci_buffer();
    if (tolower((unsigned char)conferma) != 's') {
        printf("  Operazione annullata.\n");
        return;
    }

    fp = fopen(FILE_DB, "rb");
    tmp = fopen(FILE_TEMP, "wb");
    if (!fp || !tmp) {
        if (fp) fclose(fp);
        if (tmp) fclose(tmp);
        printf("  ✗ Errore file.\n");
        return;
    }

    trovato = 0;
    while (fread(&c, sizeof(Contatto), 1, fp) == 1) {
        str_lower(c.nome, clow, MAX_NOME);
        if (!trovato && strcmp(clow, nlow) == 0) {
            trovato = 1; /* salta questo record */
            continue;
        }
        fwrite(&c, sizeof(Contatto), 1, tmp);
    }
    fclose(fp);
    fclose(tmp);

    remove(FILE_DB);
    rename(FILE_TEMP, FILE_DB);
    printf("  ✓ Contatto eliminato con successo.\n");
}

/* ════════════════════════════════════════════════════
   Main
   ════════════════════════════════════════════════════ */

int main(void) {
    int scelta;
    char buf[8];

    printf("\n");
    stampa_separatore();
    printf("      RUBRICA TELEFONICA\n");
    stampa_separatore();

    do {
        int n = conta_contatti();
        printf("\n");
        stampa_separatore();
        printf("  MENU PRINCIPALE  (%d contatti)\n", n);
        stampa_separatore();
        printf("  [1] Nuovo contatto\n");
        printf("  [2] Lista contatti\n");
        printf("  [3] Cerca contatto\n");
        printf("  [4] Modifica contatto\n");
        printf("  [5] Cancella contatto\n");
        printf("  [0] Esci\n");
        stampa_separatore();
        printf("  Scelta: ");

        if (!fgets(buf, sizeof(buf), stdin)) break;
        buf[strcspn(buf, "\n")] = '\0';

        if (sscanf(buf, "%d", &scelta) != 1) {
            printf("  ⚠ Scelta non valida.\n");
            scelta = -1;
            continue;
        }

        printf("\n");
        switch (scelta) {
            case 1: aggiungi_contatto();  break;
            case 2: lista_contatti();     break;
            case 3: cerca_contatto();     break;
            case 4: modifica_contatto();  break;
            case 5: cancella_contatto();  break;
            case 0: printf("  Arrivederci!\n\n"); break;
            default: printf("  ⚠ Scelta non valida.\n");
        }
    } while (scelta != 0);

    return 0;
}
