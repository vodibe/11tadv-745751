#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "costanti_variabili_globali.h"
#include "tipo_oggetto.h"
#include "tipo_oggetto_personaggio.h"

/* I seguenti metodi creano dei file binari a partire dai file di testo.
* I file binari sono piu' efficienti in quanto possono essere letti mediante un accesso casuale
* (e non sequenziale) visto che sappiamo la loro struttura.
*/

/* ----- METODI DEL MODULO inizializzazione_database -----*/
/*
* init_lista_collegamenti_stanze
* Questo metodo accede al file di testo (facilmente comprensibile dall'utente) che contiene per ciascuna
* stanza della mappa gli id delle stanze collegate direttamente e lo trasforma in un file binario
* che e' gestibile in maniera ottimale il programma, compiendo ad esempio salti da un record all'altro del file
* senza procedere in maniera sequenziale.
*
* Struttura record file lista_collegamenti_stanze.txt
* ...
* 1 -1 -1 6 -1
* ...
* - ID stanza corrente: 1
* - ID stanza collegata a nord:  -1 (nessuna)
* - ID stanza collegata a est:   -1 (nessuna)
* - ID stanza collegata a sud:    6
* - ID stanza collegata a ovest: -1 (nessuna) 
*/
void init_lista_collegamenti_stanze()
{
    FILE * txt_lista_collegamenti_stanze;
    FILE * bin_lista_collegamenti_stanze;
    int temp_id;

    txt_lista_collegamenti_stanze = fopen("../data/lista_collegamenti_stanze.txt", "r");
    bin_lista_collegamenti_stanze = fopen("../data/lista_collegamenti_stanze.bin","wb");
    if(txt_lista_collegamenti_stanze == NULL || bin_lista_collegamenti_stanze == NULL)
    {
        printf("\n[X] Si e' verificato un errore con i file \"lista_collegamenti_stanze\".");
        esito_controllo = false;
        return;
    }

    while(!feof(txt_lista_collegamenti_stanze))
    {
        fscanf(txt_lista_collegamenti_stanze, "%d", &temp_id);
        fwrite(&temp_id, sizeof(int), 1, bin_lista_collegamenti_stanze);
        fscanf(txt_lista_collegamenti_stanze, "%d", &temp_id);
        fwrite(&temp_id, sizeof(int), 1, bin_lista_collegamenti_stanze);
        fscanf(txt_lista_collegamenti_stanze, "%d", &temp_id);
        fwrite(&temp_id, sizeof(int), 1, bin_lista_collegamenti_stanze);
        fscanf(txt_lista_collegamenti_stanze, "%d", &temp_id);
        fwrite(&temp_id, sizeof(int), 1, bin_lista_collegamenti_stanze);
        fscanf(txt_lista_collegamenti_stanze, "%d", &temp_id);
        fwrite(&temp_id, sizeof(int), 1, bin_lista_collegamenti_stanze);
    }

    fclose(txt_lista_collegamenti_stanze);
    fclose(bin_lista_collegamenti_stanze);
}

/*
* init_lista_oggetti_stanze
* Questo metodo accede al file di testo (facilmente comprensibile dall'utente) che contiene per ciascuna
* stanza della mappa gli id (+ altre informazioni) degli oggetti della stanza corrente e lo trasforma in un file binario
* che e' gestibile in maniera ottimale il programma, compiendo ad esempio salti da un record all'altro del file
* senza procedere in maniera sequenziale.
*
* Struttura record file lista_oggetti_stanze.txt
* ...
* 1 -1 10 Scala_che_collega_1-70 i,S
* ...
* - ID stanza dove e' presente l'oggetto: 1
* - ID oggetto genitore:                  -1 (nessuno)
* - ID oggetto:                           10
* - Nome oggetto:                         Scala_che_collega_1-70
* - Attributi oggetto:                    i,S
*/
void init_lista_oggetti_stanze()
{
    FILE * bin_lista_oggetti_stanze;
    FILE * txt_lista_oggetti_stanze;
    int  temp_id;
    char temp_nome     [LUNG_NOME_MAX];
    char temp_attributi[LUNG_ATTRIBUTI_MAX];

    txt_lista_oggetti_stanze = fopen("../data/lista_oggetti_stanze.txt", "r");
    bin_lista_oggetti_stanze = fopen("../data/lista_oggetti_stanze.bin","wb");
    if(txt_lista_oggetti_stanze == NULL || bin_lista_oggetti_stanze == NULL)
    {
        printf("\n[X] Si e' verificato un errore con i file \"lista_oggetti_stanze\".");
        esito_controllo = false;
        return;
    }

    while(!feof(txt_lista_oggetti_stanze))
    {
        fscanf(txt_lista_oggetti_stanze, "%d", &temp_id);
        fwrite(&temp_id, sizeof(int), 1, bin_lista_oggetti_stanze);
        fscanf(txt_lista_oggetti_stanze, "%d", &temp_id);
        fwrite(&temp_id, sizeof(int), 1, bin_lista_oggetti_stanze);
        fscanf(txt_lista_oggetti_stanze, "%d", &temp_id);
        fwrite(&temp_id, sizeof(int), 1, bin_lista_oggetti_stanze);
        fscanf(txt_lista_oggetti_stanze, "%s", temp_nome);
        fwrite(temp_nome, sizeof(temp_nome), 1, bin_lista_oggetti_stanze);
        fscanf(txt_lista_oggetti_stanze, "%s", temp_attributi);
        fwrite(temp_attributi, sizeof(temp_attributi), 1, bin_lista_oggetti_stanze);
    }
    fclose(txt_lista_oggetti_stanze);
    fclose(bin_lista_oggetti_stanze);
}

/* init_lista_descrizioni_stanze
* Questa funzione crea un file binario delle descrizioni delle stanze in base al file di testo.
*/
void init_lista_descrizioni_stanze()
{
    FILE * txt_lista_descrizioni_stanze;
    FILE * bin_lista_descrizioni_stanze;
    int temp_id, i;
    char temp_descrizione[LUNG_DESCRIZIONE_MAX];
    char c;

    txt_lista_descrizioni_stanze = fopen("../data/lista_descrizioni_stanze.txt", "r");
    bin_lista_descrizioni_stanze = fopen("../data/lista_descrizioni_stanze.bin", "wb");
    if(txt_lista_descrizioni_stanze == NULL || bin_lista_descrizioni_stanze == NULL)
    {
        printf("\n[X] Si e' verificato un errore con i file \"lista_descrizioni_stanze\".");
        esito_controllo = false;
        return;
    }

    while(!feof(txt_lista_descrizioni_stanze))
    {
        //Preleva il numero della stanza
        fscanf(txt_lista_descrizioni_stanze, "%d", &temp_id);
        //Preleva la descrizione della stanza fino al carattere di fine campo (&)
        c = fgetc(txt_lista_descrizioni_stanze);
        i=0;
        while(c != '&' && c!= EOF)
        {
            c = fgetc(txt_lista_descrizioni_stanze);
            if(c != '&' && c!=EOF)
            {
                temp_descrizione[i]=c;
                i++;
            }
        }
        temp_descrizione[i]='\0';

        fwrite(&temp_id,         sizeof(int),              1, bin_lista_descrizioni_stanze);
        fwrite(temp_descrizione, sizeof(temp_descrizione), 1, bin_lista_descrizioni_stanze);
    }
    fclose(txt_lista_descrizioni_stanze);
    fclose(bin_lista_descrizioni_stanze);
}

/* init_lista_descrizioni_oggetti
* Questa funzione crea un file binario delle descrizioni degli oggetti in base al file di testo.
*/
void init_lista_descrizioni_oggetti()
{
    FILE * txt_lista_descrizioni_oggetti;
    FILE * bin_lista_descrizioni_oggetti;
    int temp_id, i;
    char temp_descrizione[LUNG_DESCRIZIONE_MAX];
    char c;

    txt_lista_descrizioni_oggetti = fopen("../data/lista_descrizioni_oggetti.txt", "r");
    bin_lista_descrizioni_oggetti = fopen("../data/lista_descrizioni_oggetti.bin", "wb");
    if(txt_lista_descrizioni_oggetti == NULL || bin_lista_descrizioni_oggetti == NULL)
    {
        printf("\n[X] Si e' verificato un errore con i file \"lista_oggetti_oggetti\".");
        esito_controllo = false;
        return;
    }

    while(!feof(txt_lista_descrizioni_oggetti))
    {
        fscanf(txt_lista_descrizioni_oggetti, "%d", &temp_id);
        c = fgetc(txt_lista_descrizioni_oggetti);
        i=0;
        while(c != '&' && c!= EOF)
        {
            c = fgetc(txt_lista_descrizioni_oggetti);
            if(c != '&' && c!=EOF)
            {
                temp_descrizione[i]=c;
                i++;
            }
        }
        temp_descrizione[i]='\0';

        fwrite(&temp_id,         sizeof(int),              1, bin_lista_descrizioni_oggetti);
        fwrite(temp_descrizione, sizeof(temp_descrizione), 1, bin_lista_descrizioni_oggetti);
    }
    fclose(txt_lista_descrizioni_oggetti);
    fclose(bin_lista_descrizioni_oggetti);
}

/* init_inventario
* Questa funzione inizializza l'inventario
*/
void init_inventario(oggetto * inventario)
{
    int i;
    char temp_nome_oggetto[LUNG_NOME_MAX];
    strcpy(temp_nome_oggetto, "n");
    
    i=0;
    while(i<NUM_OGGETTI_INVENTARIO_MAX)
    {
        scrivere_id_oggetto         (inventario+i, -1);
        scrivere_id_oggetto_genitore(inventario+i, 1); //ID_INVENTARIO
        scrivere_nome_oggetto       (inventario+i, temp_nome_oggetto);
        scrivere_visibilita_oggetto (inventario+i, false);
        i++;
    }
}

/* init_inventario
* Questa funzione inizializza il protagonista.
*/
void creare_protagonista(oggetto_personaggio * protagonista, oggetto * inventario)
{
    scrivere_salute_personaggio(protagonista, SALUTE_PROTAGONISTA_INIZIALE, true);
    scrivere_forza_personaggio (protagonista, FORZA_PROTAGONISTA_INIZIALE  );
    init_inventario(inventario);
}








