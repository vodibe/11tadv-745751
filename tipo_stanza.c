#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "costanti_variabili_globali.h"
#include "mappa.h"

void scrivere_descrizione_oggetto_da_file(oggetto * oggetto_corrente);
void scrivere_oggetto_contenuto(stanza * stanza_mappa, int pos_oggetto, oggetto * val_oggetto);

/* ----- METODI DI ACCESSO ALLA STRUTTURA oggetto -----*/
void scrivere_id_stanza(stanza * stanza_mappa, int val_id)
{
    stanza_mappa->id_stanza = val_id;
}
int leggere_id_stanza(stanza * stanza_mappa)
{
    return stanza_mappa->id_stanza;
}

void scrivere_collegamento_id_stanza(stanza * stanza_mappa, int pos_stanza, int id_stanza_collegata)
{
    stanza_mappa->collegamenti_id_stanza[pos_stanza] = id_stanza_collegata;
}

/* leggere_collegamento_id_stanza
* Quale stanza (ID) e' collegata a stanza_mappa in base a pos_stanza?
*/
int leggere_collegamento_id_stanza(stanza * stanza_mappa, int pos_stanza)
{
    return stanza_mappa->collegamenti_id_stanza[pos_stanza];
}
int * leggere_collegamenti_id_stanza(stanza * stanza_mappa)
{
    return stanza_mappa->collegamenti_id_stanza;
}

void scrivere_oggetto_stanza(stanza * stanza_mappa, int pos_oggetto, oggetto * val_oggetto)
{
    stanza_mappa->lista_oggetti_stanza[pos_oggetto] = *val_oggetto;
}
oggetto * leggere_oggetto_stanza(stanza * stanza_mappa, int val_indice_oggetto)
{
    return &(stanza_mappa->lista_oggetti_stanza[val_indice_oggetto]);
}

void scrivere_oggetto_contenuto(stanza * stanza_mappa, int pos_oggetto, oggetto * val_oggetto)
{
    stanza_mappa->lista_oggetti_contenuti[pos_oggetto] = *val_oggetto;
}
oggetto * leggere_oggetto_contenuto(stanza * stanza_mappa, int val_indice_oggetto)
{
    return &(stanza_mappa->lista_oggetti_contenuti[val_indice_oggetto]);
}

void scrivere_descrizione_stanza(stanza * stanza_mappa, char * val_descrizione_stanza)
{
    strncpy(stanza_mappa->descrizione_stanza, val_descrizione_stanza, LUNG_DESCRIZIONE_MAX);
}
char * leggere_descrizione_stanza(stanza * stanza_mappa)
{
    return stanza_mappa->descrizione_stanza;
}
/* ----- FINE ---------------------------------------*/

/* ----- METODI AGGIUNTIVI DELLA STRUTTURA stanza -----*/
/*
* creare_stanza_da_file
* Questa funzione carica tutti gli attributi di stanza_da_aggiungere recuperando le informazioni da file binari.
* Questa funzione prende in input le coordinate della stanza da creare e la dimensione della mappa.
* - ID della stanza            -> calcolato oppure letto da lista_collegamenti_stanze
* - Collegamenti della stanza  -> letto da lista_collegamenti_stanze
* - Lista oggetti della stanza -> letto da lista_oggetti_stanza
* - Descrizione stanza         -> letta da lista_descrizioni_oggetti
* Se si incontra un errore di I/O sul file, la funzione assegna a stanza_da_aggiungere un ID non valido (-1)
* così che la funzione chiamante possa controllare l'output
*/
void creare_stanza_da_file(stanza * stanza_da_aggiungere, int i_riga, int j_col, const int VAL_RIGHE_MAPPA, const int VAL_COL_MAPPA)
{
    FILE * file_collegamenti_stanze, * file_lista_oggetti_stanze, * file_lista_descrizioni_stanze, * file_lista_descrizioni_oggetti;
    int id_stanza_assegnato;
    int i,j;
    bool fine;

    //variabili che memorizzano il contenuto recuperato dai file
    int temp_id_stanza_collegata, temp_id_stanza_oggetto, temp_id_ogg_genitore, temp_id_oggetto;
    char temp_nome_oggetto      [LUNG_NOME_MAX];
    char temp_attributi_oggetto [LUNG_ATTRIBUTI_MAX];
    char temp_descrizione_stanza[LUNG_DESCRIZIONE_MAX];
    oggetto * temp_oggetto_contenuto;   temp_oggetto_contenuto = malloc(sizeof(oggetto));

    file_collegamenti_stanze = fopen("data/lista_collegamenti_stanze.bin", "rb");
    if(file_collegamenti_stanze == NULL)
    {
        scrivere_id_stanza(stanza_da_aggiungere, -1);
        return;
    }

    /* ====================== CALCOLO ID STANZA ======================*/
    if(i_riga != VAL_RIGHE_MAPPA-1 )
    {
        //Se si sta creando una stanza NON isolata della mappa (cioe che NON compare all'ultima
        //riga come le stanze 50, 120, 130 ecc..) si calcola l'id facilmente data la riga e la colonna)
        id_stanza_assegnato = VAL_COL_MAPPA*(i_riga)+(j_col+1);
        scrivere_id_stanza(stanza_da_aggiungere, id_stanza_assegnato);
        fseek(file_collegamenti_stanze, sizeof(int) *
                                        (NUM_STANZE_COLLEGATE_MAX+1) *
                                        (leggere_id_stanza(stanza_da_aggiungere)-1), SEEK_SET);
    }
    else
    {
        //Altrimenti il calcolo dell'id si rimanda a dopo e si mette temporaneamente id = -1
        scrivere_id_stanza(stanza_da_aggiungere, -1);
        fseek(file_collegamenti_stanze, sizeof(int) *
                                        (NUM_STANZE_COLLEGATE_MAX+1) *
                                        ( VAL_COL_MAPPA * (VAL_RIGHE_MAPPA -1) ) +
                                        (   sizeof(int) *
                                            (NUM_STANZE_COLLEGATE_MAX+1) *
                                            j_col
                                        ) , SEEK_SET);
    }
    
    /* ====================== LETTURA ID STANZE COLLEGATE ======================*/
    fine=false;
    while(!feof(file_collegamenti_stanze) && fine==false)
    {
        // I campi di ogni record del file lista_collegamenti_stanze sono 5
        //(l'id della stanza corrente + 4 id stanze collegate)
        i=0;
        while(i<NUM_STANZE_COLLEGATE_MAX+1)
        {
            if(i==0)
            {
                // Il primo campo di ogni record del file lista_collegamenti_stanze e' l'id della stanza corrente.
                // Questo viene assegnato alla stanza_da_aggiungere se questa e' una stanza isolata
                fread(&id_stanza_assegnato,sizeof(int),1,file_collegamenti_stanze);
                if(i_riga == VAL_RIGHE_MAPPA-1 )
                    scrivere_id_stanza(stanza_da_aggiungere, id_stanza_assegnato);
            }
            else
            {
                //Gli altri campi del record vengono assegnati al vettore collegamenti_id_stanza mediante metodo d'accesso
                fread(&temp_id_stanza_collegata, sizeof(int), 1, file_collegamenti_stanze);
                scrivere_collegamento_id_stanza(stanza_da_aggiungere, i-1, temp_id_stanza_collegata);
            }
            i++;
        }
        fine=true;
    }
    fclose(file_collegamenti_stanze);

    /* ====================== LETTURA OGGETTI STANZA ======================*/
    file_lista_oggetti_stanze = fopen("data/lista_oggetti_stanze.bin","rb");

    if(file_lista_oggetti_stanze == NULL)
    {
        scrivere_id_stanza(stanza_da_aggiungere, -1);
        return;
    }

    /* Se si devono recuperare gli oggetti di una stanza NON isolata utilizza l'id della stanza (1-20)
    * per posizionarti nel file binario. Altrimenti posizionati nel file binario a fine oggetti della stanza
    * 20 e utilizza l'indice di colonna per posizionarti
    */
    if(i_riga != VAL_RIGHE_MAPPA-1 )
        fseek(file_lista_oggetti_stanze,( sizeof(int) * 3 +
                                        LUNG_NOME_MAX +
                                        LUNG_ATTRIBUTI_MAX
                                        ) * NUM_OGGETTI_STANZA_MAX *
                                        (leggere_id_stanza(stanza_da_aggiungere)-1), SEEK_SET);
    else
        fseek(file_lista_oggetti_stanze,( sizeof(int) * 3 +
                                        LUNG_NOME_MAX +
                                        LUNG_ATTRIBUTI_MAX
                                        ) * NUM_OGGETTI_STANZA_MAX *
                                        ( VAL_COL_MAPPA * (VAL_RIGHE_MAPPA -1) ) + 
                                        ( sizeof(int) * 3 +
                                        LUNG_NOME_MAX +
                                        LUNG_ATTRIBUTI_MAX
                                        ) * NUM_OGGETTI_STANZA_MAX *
                                        j_col, SEEK_SET);    
    i=0;
    while(!feof(file_lista_oggetti_stanze) && i<NUM_OGGETTI_STANZA_MAX)
    {
        fread(&temp_id_stanza_oggetto, sizeof(int),                    1, file_lista_oggetti_stanze);
        fread(&temp_id_ogg_genitore,   sizeof(int),                    1, file_lista_oggetti_stanze);
        fread(&temp_id_oggetto,        sizeof(int),                    1, file_lista_oggetti_stanze);
        fread(temp_nome_oggetto,       sizeof(temp_nome_oggetto),      1, file_lista_oggetti_stanze);
        fread(temp_attributi_oggetto,  sizeof(temp_attributi_oggetto), 1, file_lista_oggetti_stanze);

        /* Sostituisco '_' con ' ' nel nome nell'oggetto */
        j=0;
        while(j<sizeof(temp_nome_oggetto))
        {
            if(temp_nome_oggetto[j] == '_')
                temp_nome_oggetto[j] = ' ';
            j++;
        }
        scrivere_id_oggetto         (leggere_oggetto_stanza(stanza_da_aggiungere, i), temp_id_oggetto);
        scrivere_id_oggetto_genitore(leggere_oggetto_stanza(stanza_da_aggiungere, i), temp_id_ogg_genitore);
        scrivere_nome_oggetto       (leggere_oggetto_stanza(stanza_da_aggiungere, i), temp_nome_oggetto);
        scrivere_attributi_oggetto  (leggere_oggetto_stanza(stanza_da_aggiungere, i), temp_attributi_oggetto );

        //Se l'oggetto letto e' un oggetto vuoto o non valido, questo non e' visibile
        if(temp_id_oggetto == -1)
            scrivere_visibilita_oggetto (leggere_oggetto_stanza(stanza_da_aggiungere, i), false);
        else
        {
            scrivere_descrizione_oggetto_da_file(leggere_oggetto_stanza(stanza_da_aggiungere, i));

            /* (vincolo generico)
            * I passaggi segreti vengono nascosti quando si crea una nuova mappa.
            */
            if(temp_id_oggetto == 107 ||
               temp_id_oggetto == 133 ||
               temp_id_oggetto == 501  )
                scrivere_visibilita_oggetto (leggere_oggetto_stanza(stanza_da_aggiungere, i), false);
            else
                scrivere_visibilita_oggetto (leggere_oggetto_stanza(stanza_da_aggiungere, i), true);
        }
        i++;
    }
    /* ================== LETTURA OGGETTI CONTENUTI (OGGETTI FIGLI) ==================*/
    /*Scansiona tutti gli oggetti visibili della stanza corrente aventi attributo 'a'
    * Ricordiamo che l'array di oggetti lista_oggetti_contenuti contiene tutti gli oggetti che sono contenuti
    * in qualsiasi oggetto apribile della stanza. Es. Nella stanza x ci sono due oggetti apribili:
    * - teca:     contiene oggetto1
    * - cassetto: contiene oggetto2, oggetto 5
    * Allora l'array lista_oggetti_contenuti (relativo a stanza x) contiene oggetto1, oggetto2, oggetto5
    */
    int c;
    j=0;
    i=0;
    while(i<NUM_OGGETTI_STANZA_MAX)
    {
        if(leggere_tipo_oggetto(leggere_oggetto_stanza(stanza_da_aggiungere, i)) == ID_TIPO_OGGETTO_GENITORE && leggere_visibilita_oggetto(leggere_oggetto_stanza(stanza_da_aggiungere,i)) == true)
        {
            //Posizionati nel file lista_oggetti_stanze.bin a partire da dove iniziano gli oggetti contenuti, cioe' dopo gli oggetti di tutte le stanze
            fseek(file_lista_oggetti_stanze,( sizeof(int) * 3 +
                                        LUNG_NOME_MAX +
                                        LUNG_ATTRIBUTI_MAX
                                        ) * NUM_OGGETTI_STANZA_MAX *
                                        ( VAL_COL_MAPPA * VAL_RIGHE_MAPPA ), SEEK_SET);

            while(!feof(file_lista_oggetti_stanze) && j<NUM_OGGETTI_CONTENUTI_PER_STANZA_MAX)
            {
                fread(&temp_id_stanza_oggetto, sizeof(int),                  1, file_lista_oggetti_stanze);
                fread(&temp_id_ogg_genitore,   sizeof(int),                  1, file_lista_oggetti_stanze);
                fread(&temp_id_oggetto,        sizeof(int),                  1, file_lista_oggetti_stanze);
                fread(temp_nome_oggetto,       sizeof(temp_nome_oggetto),      1, file_lista_oggetti_stanze);
                fread(temp_attributi_oggetto,  sizeof(temp_attributi_oggetto), 1, file_lista_oggetti_stanze);

                /* Sostituisco '_' con ' ' nel nome nell'oggetto */
                c=0;
                while(c<sizeof(temp_nome_oggetto))
                {
                    if(temp_nome_oggetto[c] == '_')
                        temp_nome_oggetto[c] = ' ';
                    c++;
                }

                scrivere_id_oggetto                 (temp_oggetto_contenuto, temp_id_oggetto);
                //scrivere_id_stanza_oggetto  (temp_oggetto_contenuto, temp_id_stanza_oggetto);
                scrivere_id_oggetto_genitore        (temp_oggetto_contenuto, temp_id_ogg_genitore);
                scrivere_nome_oggetto               (temp_oggetto_contenuto, temp_nome_oggetto);
                scrivere_descrizione_oggetto_da_file(temp_oggetto_contenuto);
                scrivere_attributi_oggetto          (temp_oggetto_contenuto, temp_attributi_oggetto);
                scrivere_visibilita_oggetto         (temp_oggetto_contenuto, false);

                if(leggere_id_oggetto(leggere_oggetto_stanza(stanza_da_aggiungere,i)) == temp_id_ogg_genitore)
                {
                    scrivere_oggetto_contenuto(stanza_da_aggiungere, j, temp_oggetto_contenuto);
                    j++;
                }
            }
        }
        i++; 
    }
    //Riempi gli elementi restanti dell'array con oggetti vuoti
    while(j<NUM_OGGETTI_CONTENUTI_PER_STANZA_MAX)
    {
        strcpy(temp_nome_oggetto, "n");
        strcpy(temp_attributi_oggetto, "n");
        scrivere_id_oggetto         (temp_oggetto_contenuto, -1);
        scrivere_id_oggetto_genitore(temp_oggetto_contenuto, -1);
        scrivere_nome_oggetto       (temp_oggetto_contenuto, temp_nome_oggetto);
        //scrivere_attributi_oggetto  (temp_oggetto_contenuto, temp_attributi_oggetto);
        scrivere_visibilita_oggetto (temp_oggetto_contenuto, false);

        scrivere_oggetto_contenuto(stanza_da_aggiungere, j, temp_oggetto_contenuto);
        j++;
    }
    fclose(file_lista_oggetti_stanze);
    free(temp_oggetto_contenuto);

    /* ================== LETTURA DESCRIZIONE STANZA ==================*/
    int temp_id;

    file_lista_descrizioni_stanze = fopen("data/lista_descrizioni_stanze.bin", "rb");
    if(file_lista_oggetti_stanze == NULL)
    {
        scrivere_id_stanza(stanza_da_aggiungere, -1);
        return;
    }

    if(i_riga != VAL_RIGHE_MAPPA-1 )
        fseek(file_lista_descrizioni_stanze,( sizeof(int) +
                                        LUNG_DESCRIZIONE_MAX
                                        ) *
                                        (leggere_id_stanza(stanza_da_aggiungere)-1), SEEK_SET);
    else
        fseek(file_lista_descrizioni_stanze,( sizeof(int) +
                                        LUNG_DESCRIZIONE_MAX
                                        ) *
                                        ( VAL_COL_MAPPA * (VAL_RIGHE_MAPPA -1) ) + 
                                        ( sizeof(int) +
                                        LUNG_DESCRIZIONE_MAX 
                                        ) * j_col, SEEK_SET); 
                                         
    fread(&temp_id, sizeof(int), 1, file_lista_descrizioni_stanze);
    fread(temp_descrizione_stanza, sizeof(temp_descrizione_stanza), 1, file_lista_descrizioni_stanze);

    scrivere_descrizione_stanza(stanza_da_aggiungere, temp_descrizione_stanza);

    fclose(file_lista_descrizioni_stanze);
}


void scrivere_descrizione_oggetto_da_file(oggetto * oggetto_corrente)
{
    FILE * file_lista_descrizioni_oggetti;
    int id_da_ricercare, pos_id_da_ricercare, offset;
    int temp_id;
    char temp_descrizione[LUNG_DESCRIZIONE_MAX];
    bool fine_ricerca;

    id_da_ricercare     = leggere_id_oggetto(oggetto_corrente);

    file_lista_descrizioni_oggetti = fopen("data/lista_descrizioni_oggetti.bin", "rb");
    if(file_lista_descrizioni_oggetti == NULL || id_da_ricercare <= -1)
    {
        return;
    }

    fine_ricerca = false;
    while(!feof(file_lista_descrizioni_oggetti) && fine_ricerca == false)
    {
        fread(&temp_id, sizeof(int), 1, file_lista_descrizioni_oggetti);
        fread(temp_descrizione, sizeof(temp_descrizione), 1, file_lista_descrizioni_oggetti);
        if(temp_id == id_da_ricercare)
            scrivere_descrizione_oggetto(oggetto_corrente, temp_descrizione);
    }
    fclose(file_lista_descrizioni_oggetti); 
}
