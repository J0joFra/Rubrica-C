# Rubrica Telefonica
Questo progetto è una semplice rubrica telefonica sviluppata in linguaggio C. Permette di aggiungere, visualizzare, modificare e cancellare contatti.

## Funzionalità

- **Aggiungi nuovo contatto**: Permette di aggiungere un nuovo contatto alla rubrica.
- **Visualizza lista contatti**: Mostra tutti i contatti salvati nella rubrica in ordine alfabetico.
- **Modifica contatto**: Consente di modificare le informazioni di un contatto esistente.
- **Cancella contatto**: Elimina un contatto dalla rubrica.

## Struttura del Codice

Il codice principale si trova in `main.c` e utilizza una struttura per memorizzare le informazioni del contatto:

```c
struct contact {
    double ph;
    char name[20], add[20];
} list;
```

## Utilizzo

1. **Compilazione del Codice**

   Per compilare il codice, è necessario un compilatore C come `gcc`. Utilizza il seguente comando per compilare il codice:

   ```bash
   gcc main.c -o rubrica
   ```

2. **Esecuzione del Programma**

   Dopo aver compilato il codice, esegui il programma con il seguente comando:

   ```bash
   ./rubrica
   ```

3. **Menu Principale**

   Il menu principale offre diverse opzioni:

   ```
   Menu principale
   ======================
   [1] Nuovo contatto
   [2] Lista contatti
   [3] Modifica contatto
   [4] Cancella contatto
   [0] Esci
   ======================
   Seleziona una scelta:
   ```

   - **[1] Nuovo contatto**: Aggiungi un nuovo contatto fornendo nome, numero di telefono e indirizzo.
   - **[2] Lista contatti**: Visualizza tutti i contatti in ordine alfabetico.
   - **[3] Modifica contatto**: Inserisci il nome del contatto da modificare e aggiorna le informazioni.
   - **[4] Cancella contatto**: Inserisci il nome del contatto da eliminare.
   - **[0] Esci**: Termina il programma.

## Note

- I contatti sono salvati in un file binario chiamato `contact.dll`.
- Un file temporaneo `temp.dat` viene utilizzato per le operazioni di modifica e cancellazione.
- Assicurati di avere i permessi necessari per leggere e scrivere file nella directory di esecuzione del programma.

## Requisiti

- Compilatore C (ad esempio `gcc`).
- Ambiente di sviluppo che supporta la programmazione in C.

