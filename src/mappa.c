#include <stdio.h>
#include <stdlib.h>
#include "costanti_variabili_globali.h"
#include "tipo_stanza.h"
#include "tipo_oggetto.h"
#include "mappa.h"

void scrivere_righe(mappa * mappa_gioco, int val_righe);
void scrivere_col(mappa * mappa_gioco, int val_col);
void scrivere_elenco_stanze_ptr(mappa * mappa_gioco, stanza ** val_elenco_stanze_ptr);
stanza ** leggere_elenco_stanze_ptr(mappa * mappa_gioco);

void scrivere_porta_in_mappa(mappa * mappa_gioco, int val_pos, oggetto_porta * porta_da_aggiungere);
oggetto_porta * leggere_porta_da_mappa(mappa * mappa_gioco, int val_pos);

void scrivere_cibo_in_mappa(mappa * mappa_gioco, int val_pos, oggetto_cibo * cibo_da_aggiungere);
oggetto_cibo * leggere_cibo_da_mappa(mappa * mappa_gioco, int val_pos);

void scrivere_arma_in_mappa(mappa * mappa_gioco, int val_pos, oggetto_arma * arma_da_aggiungere);

void scrivere_personaggio_in_mappa(mappa * mappa_gioco, int val_pos, oggetto_personaggio * personaggio_da_aggiungere);

/* ----- METODI DI ACCESSO ALLA STRUTTURA mappa -----*/
void scrivere_righe(mappa * mappa_gioco, int val_righe)
{
    mappa_gioco->righe = val_righe;
}
int leggere_righe(mappa * mappa_gioco)
{
	return mappa_gioco->righe;
}

void scrivere_col(mappa * mappa_gioco, int val_col)
{
    mappa_gioco->col = val_col;
}
int leggere_col(mappa * mappa_gioco)
{
	return mappa_gioco->col;
}

void scrivere_elenco_stanze_ptr(mappa * mappa_gioco, stanza ** val_elenco_stanze_ptr)
{
    mappa_gioco->elenco_stanze = val_elenco_stanze_ptr;
}
stanza ** leggere_elenco_stanze_ptr(mappa * mappa_gioco)
{
	return mappa_gioco->elenco_stanze;
}

void scrivere_stanza_in_mappa(mappa * mappa_gioco, int val_riga, int val_col, stanza * stanza_da_aggiungere)
{
    stanza ** indirizzo_righe;
	stanza * indirizzo_col;

	indirizzo_righe = leggere_elenco_stanze_ptr(mappa_gioco);
	indirizzo_col = *(indirizzo_righe + val_riga);

	*(indirizzo_col + val_col) = *stanza_da_aggiungere;
}
stanza * leggere_stanza_da_mappa(mappa * mappa_gioco, int val_riga, int val_col)
{
    stanza ** indirizzo_righe;
	stanza * indirizzo_col;

	indirizzo_righe = leggere_elenco_stanze_ptr(mappa_gioco);
	indirizzo_col = *(indirizzo_righe + val_riga);

	return indirizzo_col + val_col;
}

void scrivere_porta_in_mappa(mappa * mappa_gioco, int val_pos, oggetto_porta * porta_da_aggiungere)
{
    mappa_gioco->elenco_porte[val_pos] = *porta_da_aggiungere;
}
oggetto_porta * leggere_porta_da_mappa(mappa * mappa_gioco, int val_pos)
{
    return &(mappa_gioco->elenco_porte[val_pos]);
}

void scrivere_cibo_in_mappa(mappa * mappa_gioco, int val_pos, oggetto_cibo * cibo_da_aggiungere)
{
    mappa_gioco->elenco_cibi[val_pos] = *cibo_da_aggiungere;
}
oggetto_cibo * leggere_cibo_da_mappa(mappa * mappa_gioco, int val_pos)
{
    return &(mappa_gioco->elenco_cibi[val_pos]);
}

void scrivere_arma_in_mappa(mappa * mappa_gioco, int val_pos, oggetto_arma * arma_da_aggiungere)
{
    mappa_gioco->elenco_armi[val_pos] = *arma_da_aggiungere;
}
oggetto_arma * leggere_arma_da_mappa(mappa * mappa_gioco, int val_pos)
{
    return &(mappa_gioco->elenco_armi[val_pos]);
}

void scrivere_personaggio_in_mappa(mappa * mappa_gioco, int val_pos, oggetto_personaggio * personaggio_da_aggiungere)
{
    mappa_gioco->elenco_personaggi[val_pos] = *personaggio_da_aggiungere;
}
oggetto_personaggio * leggere_personaggio_da_mappa(mappa * mappa_gioco, int val_pos)
{
    return &(mappa_gioco->elenco_personaggi[val_pos]);
}

/* ----- FINE ---------------------------------------*/

/* ----- METODI AGGIUNTIVI DELLA STRUTTURA mappa -----*/
/*
* creare_mappa
* Questa funzione controlla che il file dove e' memorizzata la dimensione della mappa esista
* e successivamente ne estrapola il numero di riga e colonna.
* Infine provvede ad allocare dinamicamente la memoria che deve contenere le righe e le colonne
* della mappa. Ogni cella della mappa e' di tipo stanza.
*/
void creare_mappa(const char * PATH_DIMENSIONE_MAPPA, mappa * mappa_gioco)
{
    FILE * file_dimensione_mappa;
    stanza ** indirizzo_righe;
    int in_righe, in_col;
    int i;

    file_dimensione_mappa = fopen(PATH_DIMENSIONE_MAPPA, "r");
    if(file_dimensione_mappa == NULL)
    {
        printf("\n[X] Si e' verificato un errore durante la creazione della mappa.");
        esito_controllo = false;
        return;
    }
    fscanf(file_dimensione_mappa, "%d%d", &in_righe, &in_col);
    fclose(file_dimensione_mappa);


    scrivere_righe(mappa_gioco, in_righe);
    scrivere_col(mappa_gioco, in_col);
    if(in_righe > 0 && in_col > 0)
    {
        indirizzo_righe = calloc(in_righe, sizeof(stanza *));
        scrivere_elenco_stanze_ptr(mappa_gioco, indirizzo_righe);

	    i=0;
	    while(i<in_righe)
	    {
		    *(indirizzo_righe+i) = calloc(in_col, sizeof(stanza));
		    i++;
	    }

        //printf("[OK] Mappa %dx%d creata con successo!\n\n", in_righe, in_col);
    }
}

/*
* aggiungere_stanze_in_mappa
* Questa funzione scandisce tutte le celle della mappa, dunque per ciascuna cella di questa mappa
* si utilizza un'unica variabile stanza che poi sara' scritta sulla mappa e successivamente verrà
* aggiornata. L'aggiornamento di tale variabile (stanza_da_aggiungere) viene fatto mediante la funzione
* creare_stanza_da_file.
*/
void aggiungere_stanze_in_mappa(mappa * mappa_gioco)
{
    stanza stanza_da_aggiungere;
    int in_righe, in_col;
    int i,j;
    
    in_righe = leggere_righe(mappa_gioco);
    in_col = leggere_col(mappa_gioco);
    
    i=0;
    while(i<in_righe)
    {
        j=0;
        while(j<in_col)
        {
            creare_stanza_da_file(&stanza_da_aggiungere, i, j, in_righe, in_col);
            if(leggere_id_stanza(&stanza_da_aggiungere) == -1)
            {
                printf("\n[X] Si e' verificato un errore durante la creazione di una stanza.");
                j=in_col;
                i=in_righe-1;
                esito_controllo = false;
            }
            else
            {
                scrivere_stanza_in_mappa(mappa_gioco, i, j, &stanza_da_aggiungere);
            }
            j++;
        }
        i++;
    }
}

/*
* aggiungere_porte_in_mappa
* Questa funzione accede al file dove sono memorizzate tutti gli oggetti porta del gioco (+ altre informazioni 
* come l'ID della stanza di partenza/arrivo della porta e degli eventuali oggetti che servono ad aprirla
* e carica tutto in un array di oggetti porta relativi alla mappa
*/
void aggiungere_porte_in_mappa(const char * PATH_LISTA_PORTE, mappa * mappa_gioco)
{
    FILE * file_lista_porte;
    int temp_id;
    oggetto_porta * porta_da_aggiungere;   porta_da_aggiungere = malloc(sizeof(oggetto_porta));
    int i, j;

    file_lista_porte = fopen(PATH_LISTA_PORTE, "r");
    if(file_lista_porte == NULL)
    {
        printf("\n[X] Si e' verificato un errore durante l'aggiunta di una porta in mappa.");
        esito_controllo = false;
        return;
    }

    i=0;
    while(!feof(file_lista_porte))
    {
        /* =============== CREAZIONE DA FILE DELLA PORTA DA AGGIUNGERE ===============*/
        fscanf(file_lista_porte, "%d", &temp_id);
        scrivere_id_porta                (porta_da_aggiungere, temp_id);
        fscanf(file_lista_porte, "%d", &temp_id);
        scrivere_id_stanza_partenza_porta(porta_da_aggiungere, temp_id);
        fscanf(file_lista_porte, "%d", &temp_id);
        scrivere_id_stanza_arrivo_porta  (porta_da_aggiungere, temp_id);

        j=0;
        fscanf(file_lista_porte, "%d", &temp_id);
        if(temp_id != -1)
        {
            //La porta necessita di almeno una chiave
            while(temp_id != -1)
            {
                scrivere_id_chiave_porta(porta_da_aggiungere, j, temp_id);
                fscanf(file_lista_porte, "%d", &temp_id);
                j++;
            }  
        }
        while(j<NUM_CHIAVI_PORTA_MAX)
        {
            scrivere_id_chiave_porta    (porta_da_aggiungere, j, temp_id);
            j++;
        }

        /* =============== SCRITTURA DELLA PORTA DA AGGIUNGERE IN MAPPA ===============*/
        scrivere_porta_in_mappa(mappa_gioco, i, porta_da_aggiungere);
        i++;
    }
    fclose(file_lista_porte);
    free(porta_da_aggiungere);
}

/*
* aggiungere_cibi_in_mappa
* Questa funzione accede al file dove sono memorizzati tutti gli oggetti cibo del gioco
* e carica tutto in un array di oggetti cibo relativi alla mappa
*/
void aggiungere_cibi_in_mappa(const char * PATH_LISTA_CIBI, mappa * mappa_gioco)
{
    FILE * file_lista_cibi;
    oggetto_cibo * cibo_da_aggiungere;  cibo_da_aggiungere = malloc(sizeof(oggetto_cibo));
    int temp_id;
    int i;

    file_lista_cibi = fopen(PATH_LISTA_CIBI, "r");
    if(file_lista_cibi == NULL)
    {
        printf("\n[X] Si e' verificato un errore durante l'aggiunta di un oggetto cibo in mappa.");
        esito_controllo = false;
        return;
    }

    i=0;
    while(!feof(file_lista_cibi))
    {
        fscanf(file_lista_cibi, "%d", &temp_id);
        scrivere_id_cibo          (cibo_da_aggiungere, temp_id);
        fscanf(file_lista_cibi, "%d", &temp_id);
        scrivere_bonus_salute_cibo(cibo_da_aggiungere, temp_id);

        scrivere_cibo_in_mappa(mappa_gioco, i, cibo_da_aggiungere);
        i++;
    }
    fclose(file_lista_cibi);
    free(cibo_da_aggiungere);
}

/*
* aggiungere_armi_in_mappa
* Questa funzione accede al file dove sono memorizzati tutti gli oggetti arma del gioco
* e carica tutto in un array di oggetti arma relativi alla mappa
*/
void aggiungere_armi_in_mappa(const char * PATH_LISTA_ARMI, mappa * mappa_gioco)
{
    FILE * file_lista_armi;
    oggetto_arma * arma_da_aggiungere;  arma_da_aggiungere = malloc(sizeof(oggetto_arma));
    int temp_id;    
    int i;

    file_lista_armi = fopen(PATH_LISTA_ARMI, "r");
    if(file_lista_armi == NULL)
    {
        printf("\n[X] Si e' verificato un errore durante l'aggiunta di un'arma in mappa.");
        esito_controllo = false;
        return;
    }

    i=0;
    while(!feof(file_lista_armi))
    {
        fscanf(file_lista_armi, "%d", &temp_id);
        scrivere_id_arma                (arma_da_aggiungere, temp_id);
        fscanf(file_lista_armi, "%d", &temp_id);
        scrivere_bonus_salute_arma      (arma_da_aggiungere, temp_id);
        fscanf(file_lista_armi, "%d", &temp_id);
        scrivere_bonus_forza_arma       (arma_da_aggiungere, temp_id);
        scrivere_stato_equipaggiato_arma(arma_da_aggiungere, false  );

        scrivere_arma_in_mappa(mappa_gioco, i, arma_da_aggiungere);
        i++;
    }
    fclose(file_lista_armi);
    free(arma_da_aggiungere);
}

/*
* aggiungere_personaggi_in_mappa
* Questa funzione accede al file dove sono memorizzati tutti gli oggetti personaggio del gioco (+ altre informazioni)
* e carica tutto in un array di oggetti personaggio relativi alla mappa
*/
void aggiungere_personaggi_in_mappa(const char * PATH_LISTA_PERSONAGGI, mappa * mappa_gioco)
{
    FILE * file_lista_personaggi;
    oggetto_personaggio * personaggio_da_aggiungere;  personaggio_da_aggiungere = malloc(sizeof(oggetto_personaggio));
    int temp_id;
    int i,j;

    file_lista_personaggi = fopen(PATH_LISTA_PERSONAGGI, "r");
    if(file_lista_personaggi == NULL)
    {
        printf("\n[X] Si e' verificato un errore durante l'aggiunta di un personaggio in mappa.");
        esito_controllo = false;
        return;
    }

    i=0;
    while(!feof(file_lista_personaggi))
    {
        fscanf(file_lista_personaggi, "%d", &temp_id);
        scrivere_id_personaggio(personaggio_da_aggiungere, temp_id);
        fscanf(file_lista_personaggi, "%d", &temp_id);
        scrivere_salute_personaggio(personaggio_da_aggiungere, temp_id, false);
        fscanf(file_lista_personaggi, "%d", &temp_id);
        scrivere_forza_personaggio(personaggio_da_aggiungere, temp_id);

        fscanf(file_lista_personaggi, "%d", &temp_id);

        j=0;
        if(temp_id != -1)
        {
            //Il personaggio rilascia almeno un oggetto
            while(temp_id != -1)
            {
                scrivere_id_oggetto_rilasciato(personaggio_da_aggiungere, j, temp_id);
                fscanf(file_lista_personaggi, "%d", &temp_id);
                j++;
            }
        }
        while(j<NUM_OGGETTI_RILASCIATI_MAX)
        {
            scrivere_id_oggetto_rilasciato(personaggio_da_aggiungere, j, temp_id);
            j++;
        } 

        scrivere_personaggio_in_mappa(mappa_gioco, i, personaggio_da_aggiungere);
        i++;
    }
    fclose(file_lista_personaggi);
    free(personaggio_da_aggiungere);
}

/*
* ottenere_oggetto_specifico_da_oggetto
* Questa funzione effettua la "conversione" di un oggetto generico in un oggetto specifico, nel senso che
* ricerca l'id dell'oggetto generico da convertire in uno dei 4 array di mappa_gioco che contengono
* tutti gli oggetti specifici.
* Restituisce un puntatore a void (perche' non si sa di che tipo sara' l'oggetto trovato, potrebbe essere 
* di tipo_oggetto_porta, tipo_oggetto_cibo, tipo_oggetto_arma o tipo_oggetto_personaggio).
* L'importante e' che la funzione che chiama questa funzione successivamente faccia un cast del puntatore.
*/
void * ottenere_oggetto_specifico_da_oggetto(mappa * mappa_gioco, oggetto * oggetto_gioco, const int tipo_oggetto_da_restituire)
{
    int id_oggetto_gioco;
    int id_oggetto_specifico;
    bool oggetto_specifico_trovato;
    int i,j;

    id_oggetto_gioco = leggere_id_oggetto(oggetto_gioco);
    oggetto_specifico_trovato = false;
    i=0;
    while(i<NUM_PORTE_MAPPA_MAX && oggetto_specifico_trovato == false)
    {
        if     (tipo_oggetto_da_restituire == ID_TIPO_OGGETTO_PORTA)
            id_oggetto_specifico = leggere_id_porta(leggere_porta_da_mappa(mappa_gioco, i));
        else if(tipo_oggetto_da_restituire == ID_TIPO_OGGETTO_CIBO)
            id_oggetto_specifico = leggere_id_cibo(leggere_cibo_da_mappa(mappa_gioco, i));
        else if(tipo_oggetto_da_restituire == ID_TIPO_OGGETTO_ARMA)
            id_oggetto_specifico = leggere_id_arma(leggere_arma_da_mappa(mappa_gioco, i));
        else if(tipo_oggetto_da_restituire == ID_TIPO_OGGETTO_PERSONAGGIO)
            id_oggetto_specifico = leggere_id_personaggio(leggere_personaggio_da_mappa(mappa_gioco, i));

        if(id_oggetto_specifico == id_oggetto_gioco)
        {
            j=i;
            oggetto_specifico_trovato = true;
        }
        i++;
    }
    
    if(oggetto_specifico_trovato == true)
    {
        if     (tipo_oggetto_da_restituire == ID_TIPO_OGGETTO_PORTA)
            return (void *) leggere_porta_da_mappa(mappa_gioco, j);
        else if(tipo_oggetto_da_restituire == ID_TIPO_OGGETTO_CIBO)
            return (void *) leggere_cibo_da_mappa(mappa_gioco, j);
        else if(tipo_oggetto_da_restituire == ID_TIPO_OGGETTO_ARMA)
            return (void *) leggere_arma_da_mappa(mappa_gioco, j);
        //else if(tipo_oggetto_da_restituire == ID_TIPO_OGGETTO_PERSONAGGIO)
        else
            return (void *) leggere_personaggio_da_mappa(mappa_gioco, j);
    }
    else
        return NULL;
}
