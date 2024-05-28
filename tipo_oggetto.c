#include <stdio.h>
#include <string.h>
#include "costanti_variabili_globali.h"
#include "tipo_oggetto.h"

/* ----- METODI DI ACCESSO ALLA STRUTTURA oggetto -----*/
void scrivere_id_oggetto(oggetto * oggetto_gioco, int val_id)
{
    oggetto_gioco->id_oggetto = val_id;
}
int leggere_id_oggetto(oggetto * oggetto_gioco)
{
    return  oggetto_gioco->id_oggetto;
}

void scrivere_id_oggetto_genitore(oggetto * oggetto_gioco, int val_id_oggetto_genitore)
{
    oggetto_gioco->id_oggetto_genitore = val_id_oggetto_genitore;
}
int leggere_id_oggetto_genitore(oggetto * oggetto_gioco)
{
    return  oggetto_gioco->id_oggetto_genitore;
}

void scrivere_nome_oggetto(oggetto * oggetto_gioco, char * val_nome_oggetto)
{
    strncpy(oggetto_gioco->nome_oggetto, val_nome_oggetto, LUNG_NOME_MAX);
}
char * leggere_nome_oggetto(oggetto * oggetto_gioco)
{
    return oggetto_gioco->nome_oggetto;
}

void scrivere_descrizione_oggetto(oggetto * oggetto_gioco, char * val_descrizione_oggetto)
{
    strncpy(oggetto_gioco->descrizione_oggetto, val_descrizione_oggetto, LUNG_DESCRIZIONE_MAX);
}
char * leggere_descrizione_oggetto(oggetto * oggetto_gioco)
{
    return oggetto_gioco->descrizione_oggetto;
}

/*
* scrivere_attributi_oggetto
* Questa funzione trasforma la stringa degli attributi (separati da ',') in un vettore di caratteri
* senza il carattere ',' ed inoltre viene aggiunto alla prima posizione utile il carattere 'n' che indica
* la fine del vettore. Se l'oggetto non ha attributi (cioe' ha solo il simbolo 'n') non viene fatta alcuna
* operazione sul vettore di caratteri finali
* i,p,l   ->  [i][p][l][n]             n   ->  [n]
*/
void scrivere_attributi_oggetto(oggetto * oggetto_gioco, char * val_attributi_oggetto)
{
    int i,c;
    i=0;
    while(i<NUM_ATTRIBUTI_OGGETTO_MAX)
    {
        oggetto_gioco->attributi_oggetto[i] = 'n';
        i++;
    }

    c=0;
    i=0;
    while(i<LUNG_ATTRIBUTI_MAX)
    {
        if(val_attributi_oggetto[i] != ',' && val_attributi_oggetto[i] != '\0' && val_attributi_oggetto[i] != 'n')
        {
            oggetto_gioco->attributi_oggetto[c] = val_attributi_oggetto[i];
            c++;
        }
        else
        {
            if(val_attributi_oggetto[i] == '\0' || val_attributi_oggetto[i] == 'n')
            {
                i=LUNG_ATTRIBUTI_MAX;
            }
        }
        i++;
    }
}
char * leggere_attributi_oggetto(oggetto * oggetto_gioco)
{
    return oggetto_gioco->attributi_oggetto;
}

void scrivere_visibilita_oggetto(oggetto * oggetto_gioco, bool val_visibilita_oggetto)
{
    oggetto_gioco->visibilita_oggetto = val_visibilita_oggetto;
}
bool leggere_visibilita_oggetto(oggetto * oggetto_gioco)
{
    return oggetto_gioco->visibilita_oggetto;
}

/* ----- METODI AGGIUNTIVI DELLA STRUTTURA tipo_oggetto -----*/

/*
* leggere_tipo_oggetto
* Questa funzione prende in input un oggetto ed, esaminando gli attributi, restituisce un valore intero che indica la sua 
* classificazione, cosi' da poter usare i metodi di tipo_oggetto_porta/tipo_oggetto_cibo/tipo_oggetto_arma/tipo_oggetto_personaggio
* classificazione_oggetto:
* -1 -> oggetto generico
*  0 -> oggetto apribile (che porta in un altra stanza) Es. porta, portone, scala, passaggio ...
*  1 -> oggetto mangiabile                              Es. mela
*  2 -> oggetto equipaggiabile                          Es. pugnale
*  3 -> personaggio                                     Es. guardia della torre
*/
int leggere_tipo_oggetto(oggetto * oggetto_gioco)
{
    int classificazione_oggetto;
    char   attributo_da_trovare;
    char * attributo_trovato;
    int i;
    
    classificazione_oggetto = -1;
    i=0;
    while(i<NUM_TIPI_OGGETTO_MAX+2 && classificazione_oggetto == -1)
    {
        if     (i==ID_TIPO_OGGETTO_PORTA)       attributo_da_trovare = 'A';
        else if(i==ID_TIPO_OGGETTO_CIBO)        attributo_da_trovare = 'm';
        else if(i==ID_TIPO_OGGETTO_ARMA)        attributo_da_trovare = 'e';
        else if(i==ID_TIPO_OGGETTO_PERSONAGGIO) attributo_da_trovare = 'w';
        else if(i==ID_TIPO_OGGETTO_GENITORE)    attributo_da_trovare = 'a';
        else if(i==NUM_TIPI_OGGETTO_MAX)        attributo_da_trovare = 's';
        else                                    attributo_da_trovare = 'S';
        
        attributo_trovato = strchr(leggere_attributi_oggetto(oggetto_gioco), attributo_da_trovare);
        
        //if(attributo_trovato != NULL && leggere_id_oggetto(oggetto_gioco) != 82)
        if(attributo_trovato != NULL)
        {
            classificazione_oggetto = i;
            if(classificazione_oggetto == NUM_TIPI_OGGETTO_MAX || classificazione_oggetto == NUM_TIPI_OGGETTO_MAX+1)
                classificazione_oggetto = ID_TIPO_OGGETTO_PORTA;
        }
        i++;
    }
    return classificazione_oggetto;
}

/* ----- FINE ---------------------------------------*/





