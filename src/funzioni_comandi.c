#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "costanti_variabili_globali.h"
#include "funzioni_comandi.h"
#include "funzioni_generali.h"
#include "funzioni_interfaccia.h"
#include "funzioni_regex.h"
#include "tipo_oggetto_porta.h"

/* ID dei comandi */
#define ID_COMANDO_VAI        0
#define ID_COMANDO_SALI       1
#define ID_COMANDO_SCENDI     2
#define ID_COMANDO_GUARDA     3
#define ID_COMANDO_ISPEZIONA  4
#define ID_COMANDO_PRENDI     5
#define ID_COMANDO_APRI       6
#define ID_COMANDO_USA        7
#define ID_COMANDO_MANGIA     8
#define ID_COMANDO_EQUIPAGGIA 9
#define ID_COMANDO_LEGGI      10
#define ID_COMANDO_UCCIDI     11
#define ID_COMANDO_SALVA      12
#define ID_COMANDO_CARICA     13
#define ID_COMANDO_AIUTO      14
#define ID_COMANDO_SALTA      15

/* Costanti usate per la gestione dei vincoli del gioco */
// (vincolo 8) - costante referenziata nella funzione eseguire_scendi
#define ID_OGGETTO_CAMINO              82  
// (vincolo 4) - costante referenziata nella funzione eseguire_prendi
#define ID_OGGETTO_GUFO                42
// (vincolo 13) - costanti referenziate nella funzione eseguire_usa
#define ID_OGGETTO_POZZO              132
#define ID_OGGETTO_FUNE              1203
// (vincolo 2) - costanti referenziate nella funzione eseguire_usa
#define ID_OGGETTO_FUOCHERELLO         62
#define ID_OGGETTO_CANDELABRO_SPENTO   21
#define ID_OGGETTO_CANDELABRO_ACCESO  921
// (vincolo 10a/10b) - costanti referenziate nella funzione eseguire_usa
#define ID_OGGETTO_LIBRERIA           106
#define ID_OGGETTO_PIUMA_GUFO         840
// (vincolo 170) - costanti referenziate nella funzione eseguire_salta
#define ID_STANZA_ALBERO              170
#define ID_STANZA_SCUDERIA            120
// (vincolo 11) - costante referenziata nella funzione eseguire_apri
#define ID_STANZA_PIAZZA               13  
// (vincolo vittoria del gioco) - costanti referenziate nella funzione eseguire_uccidi e eseguire_prendi
#define ID_OGGETTO_RE                 150
#define ID_OGGETTO_CORONA             151   

/* ----- PROTOTIPI FUNZIONI (PRIVATE) MODULO funzioni_comandi -----*/
oggetto * ricercare_nome_oggetto_in_stanza(stanza * stanza_corrente, char * nome_oggetto);
oggetto * ricercare_nome_oggetto_in_inventario(oggetto * inventario, char * nome_oggetto);

void eseguire_vai            (mappa * mappa_gioco, const char DIREZIONE[6],            oggetto * inventario, int id_stanza_arrivo_porta);
void eseguire_sali           (mappa * mappa_gioco, oggetto * oggetto_corrente);
void eseguire_scendi         (mappa * mappa_gioco, oggetto * oggetto_corrente, oggetto_personaggio * protagonista);
void eseguire_salta          (mappa * mappa_gioco);
void eseguire_ispeziona      (                     oggetto * oggetto_corrente);
void eseguire_prendi         (                     oggetto * oggetto_corrente, oggetto * inventario, oggetto_personaggio * protagonista);
void eseguire_apri           (mappa * mappa_gioco, oggetto * oggetto_corrente, oggetto * inventario);
void eseguire_apri_inventario(mappa * mappa_gioco,                             oggetto_personaggio * protagonista, oggetto * inventario);
void eseguire_mangia         (mappa * mappa_gioco, oggetto * oggetto_corrente, oggetto_personaggio * protagonista);
void eseguire_equipaggia     (mappa * mappa_gioco, oggetto * oggetto_corrente, oggetto * inventario, oggetto_personaggio * protagonista);
void eseguire_leggi          (                     oggetto * oggetto_corrente);
void eseguire_uccidi         (mappa * mappa_gioco, oggetto * oggetto_corrente, oggetto_personaggio * protagonista);
void eseguire_usa            (mappa * mappa_gioco, oggetto * oggetto_corrente, oggetto_personaggio * protagonista);

int ricercare_collegamento_stanza(mappa * mappa_gioco, int id_stanza_arrivo_protagonista, const char DIREZIONE[6]);
int ricercare_porta_in_stanza    (mappa * mappa_gioco, int id_stanza_corrente_protagonista, int id_stanza_arrivo_protagonista, oggetto * inventario);
int avviare_sistema_combattimento(mappa * mappa_gioco, oggetto_personaggio * protagonista, oggetto * oggetto_nemico);
int generare_numero_casuale      (int numero_minimo, int numero_massimo);
/* ----- FINE ---------------------------------------*/

/* ----- METODI DI ACCESSO ALLA STRUTTURA comando -----*/
void scrivere_id_comando(comando * comando_gioco, int val_id_comando)
{
    comando_gioco->id_comando = val_id_comando;
}
int leggere_id_comando(comando * comando_gioco)
{
    return comando_gioco->id_comando;
}

void scrivere_regex_verbo(comando * comando_gioco, char val_regex_verbo[LUNG_DESCRIZIONE_MAX])
{
    strncpy(comando_gioco->regex_verbo, val_regex_verbo, strlen(val_regex_verbo) );
}
char * leggere_regex_verbo(comando * comando_gioco)
{
    return comando_gioco->regex_verbo;
}

void scrivere_argomento_oggetto(comando * comando_gioco, bool val_flag)
{
    comando_gioco->argomento_oggetto = val_flag;
}
bool leggere_argomento_oggetto(comando * comando_gioco)
{
    return comando_gioco->argomento_oggetto;
}

void scrivere_regex_argomento(comando * comando_gioco, char val_regex_argomento[LUNG_DESCRIZIONE_MAX])
{
    strncpy(comando_gioco->regex_argomento, val_regex_argomento, strlen(val_regex_argomento) );
}
char * leggere_regex_argomento(comando * comando_gioco)
{
    return comando_gioco->regex_argomento;
}

void scrivere_argomento_oggetto_in_stanza(comando * comando_gioco, bool val_flag)
{
    comando_gioco->argomento_oggetto_in_stanza = val_flag;
}
bool leggere_argomento_oggetto_in_stanza(comando * comando_gioco)
{
    return comando_gioco->argomento_oggetto_in_stanza;
}

void scrivere_argomento_oggetto_in_inventario(comando * comando_gioco, bool val_flag)
{
    comando_gioco->argomento_oggetto_in_inventario = val_flag;
}
bool leggere_argomento_oggetto_in_inventario(comando * comando_gioco)
{
    return comando_gioco->argomento_oggetto_in_inventario;
}
/* ----- FINE ---------------------------------------*/

/* init_comandi
* Questa funzione carica il vettore lista comandi con le strutture dati di tipo comando che sono descritte
* nel file comandi_informazioni.txt
*/
void init_comandi(comando * lista_comandi)
{
    FILE * file_lista_comandi;
    int i;
    int temp_val;
    char temp_regex[LUNG_DESCRIZIONE_MAX];

    file_lista_comandi = fopen("../data/comandi_informazioni.txt", "r");
    if(file_lista_comandi == NULL)
    {
        printf("\n[X] Si e' verificato un errore con il file \"comandi_informazioni.txt\".");
        esito_controllo = false;
        return;
    }

    i=0;
    while(!feof(file_lista_comandi) && i<NUM_COMANDI_MAX )
    {        
        scrivere_id_comando(lista_comandi+i, i);

        fscanf(file_lista_comandi, "%s", temp_regex);
        scrivere_regex_verbo(lista_comandi+i, temp_regex);

        fscanf(file_lista_comandi, "%d", &temp_val);
        scrivere_argomento_oggetto(lista_comandi+i, (temp_val==1) ? true : false);

        fscanf(file_lista_comandi, "%s", temp_regex);
        scrivere_regex_argomento(lista_comandi+i, temp_regex);

        fscanf(file_lista_comandi, "%d", &temp_val);
        scrivere_argomento_oggetto_in_stanza(lista_comandi+i, (temp_val==1) ? true : false);

        fscanf(file_lista_comandi, "%d", &temp_val);
        scrivere_argomento_oggetto_in_inventario(lista_comandi+i, (temp_val==1) ? true : false);
        i++;
    }
    fclose(file_lista_comandi);
}

/*========= ANALISI SINTATTICA =========
* Il linguaggio utilizzato nel nostro gioco e' generato da una grammatica semplice
* descritta di seguito
* G = (X,V,S,P) ove X={'A','B',...,'y','z',' '}
*                   V={S, <verbo>, <argomento>, <nomeoggetto>}
* P={
*      S             ::= <verbo> <argomento>
*      <verbo>       ::= 'guarda'|'prendi'|'mangia'...
*      <argomento>   ::= lambda  |  <nomeoggetto>
*      <nomeoggetto> ::= 'nome di un oggetto nella stanza' | 
*                        'nome di un oggetto nell'inventario'
*   }
*/
void eseguire_parser(comando             * lista_comandi,
                     char                * verbo_comando,
                     char                * argomento_comando,
                     oggetto_personaggio * protagonista,
                     oggetto             * inventario,
                     mappa               * mappa_gioco,
                     stanza              * stanza_corrente)
{
    oggetto * oggetto_interazione;
    char      attributo_da_ricercare;
    int       pos_attributo_da_ricercare, i, temp_id;
    bool      regex_match,          comando_trovato;
    bool      ricerca_in_stanza,    ricerca_in_inventario;
    bool      argomento_inventario, argomento_corretto;

    /* 1 - Verifica dell'esistenza del comando
    * Scansiono l'array dei comandi e se trovo un comando che ha una regex_verbo che compare
    * nel verbo del comando, interrompo la ricerca e segno che il comando e' valido
    */
    regex_match = false;
    i=0;
    while(i<NUM_COMANDI_MAX && regex_match == false)
    {
        temp_id = leggere_id_comando(lista_comandi+i);
        regex_match = applicare_regex(verbo_comando, leggere_regex_verbo(lista_comandi+i));
        comando_trovato = regex_match;
        i++;
    }
    if(comando_trovato == false)
    {
        printf("[?] Non capisco cosa vuoi fare\n");
        return;
    }

    /* 2 - Controllo se l'argomento del comando e' "inventario" 
    * Impostando la variabile argomento_inventario si controllera' il comportamento del parser
    */
    if(argomento_comando != NULL)
    {
        if(strcmp(argomento_comando, "inventario") == 0)
            argomento_inventario = true;
        else
            argomento_inventario = false;
    }
    else
        argomento_inventario = false;

    /* 3 - Verifico se l'argomento del comando deve essere un oggetto o meno
    */
    if(leggere_argomento_oggetto(lista_comandi+temp_id) == false)
    {
        /* 3a - L'argomento del comando NON e' un oggetto ma una LOCUZIONE.
        * Controllo che la locuzione sia valida, cioe' che l'espressione regolare che descrive l'argomento
        * di tale comando sia rispettata e che quindi ci sia un match.
        * Se il match non avviene, il comando non e' valido
        */
        if(argomento_comando != NULL)
        {
            regex_match = applicare_regex(argomento_comando, leggere_regex_argomento(lista_comandi+temp_id));
            argomento_corretto = regex_match;
        }
        else 
        {
            /* In questo caso il comando e' del tipo "guarda"/"salva"/"carica",
            * dunque e' comunque corretto anche se non ha argomento
            */
            if( temp_id == ID_COMANDO_GUARDA ||
                temp_id == ID_COMANDO_SALVA  ||
                temp_id == ID_COMANDO_CARICA ||
                temp_id == ID_COMANDO_AIUTO  ||
                temp_id == ID_COMANDO_SALTA   )
                argomento_corretto = true;
            else
                argomento_corretto = false;
        }
        if(argomento_corretto == false)
        {
            if(argomento_comando == NULL)
                printf("[?] Su quale oggetto devo fare l'azione?\n");
            else
            {
                if(temp_id == ID_COMANDO_SALVA)
                    printf("[?] Per salvare i tuoi progressi digita solo '%s'.\n", verbo_comando);
                else if(temp_id == ID_COMANDO_CARICA)
                    printf("[?] Per caricare i tuoi progressi digita '%s'.\n", verbo_comando);
                else if(temp_id == ID_COMANDO_AIUTO)
                    printf("[?] Digita solo '%s'.\n", verbo_comando);
                else if(temp_id == ID_COMANDO_SALTA)
                    printf("[?] Devo solo saltare. Digita '%s'.\n", verbo_comando);
                else
                    printf("[?] %s cosa? Non so a cosa ti riferisci.\n", verbo_comando);
            }   
        }
        else
        {
            id_ultimo_oggetto_ispezionato = -1;
            if     (temp_id == ID_COMANDO_VAI && strcmp(argomento_comando, "porta") != 0)
            {
                if     (strcmp(argomento_comando, "nord") == 0)
                    eseguire_vai    (mappa_gioco, "nord", inventario, id_stanza_corrente_protagonista - leggere_col(mappa_gioco));
                else if(strcmp(argomento_comando, "est")  == 0)
                    eseguire_vai    (mappa_gioco, "est", inventario, id_stanza_corrente_protagonista + 1);
                else if(strcmp(argomento_comando, "sud")  == 0)
                    eseguire_vai    (mappa_gioco, "sud", inventario, id_stanza_corrente_protagonista + leggere_col(mappa_gioco));
                else if(strcmp(argomento_comando, "ovest")== 0)
                    eseguire_vai    (mappa_gioco, "ovest", inventario, id_stanza_corrente_protagonista - 1);
                
            }
            else if(temp_id == ID_COMANDO_GUARDA)
                eseguire_guarda (mappa_gioco, riga_stanza_corrente, col_stanza_corrente);
            else if(temp_id == ID_COMANDO_SALVA)
                salvare_partita (protagonista, inventario, mappa_gioco);
            else if(temp_id == ID_COMANDO_CARICA)
                caricare_partita(protagonista, inventario, mappa_gioco);
            else if(temp_id == ID_COMANDO_AIUTO)
                mostrare_guida_gioco();
            else if(temp_id == ID_COMANDO_SALTA)
                eseguire_salta(mappa_gioco);
            else
                printf("[?] Non ho capito cosa vorresti fare.\n");
        }
    }
    else
    {
        /* 3a - L'argomento del comando e' un OGGETTO.
        * Ricerco (nella stanza corrente o nell'inventario a seconda dei flag ricerca_in_inventario e
        * ricerca_in_stanza) l'oggetto su cui deve essere applicato il comando
        */
        if(argomento_comando == NULL)
        {
            argomento_corretto = false;
            printf("[?] Su quale oggetto devo compiere l'azione?\n");
            return;
        }
        argomento_corretto    = true;
        oggetto_interazione   = NULL;
        ricerca_in_stanza     = leggere_argomento_oggetto_in_stanza(lista_comandi+temp_id);
        ricerca_in_inventario = leggere_argomento_oggetto_in_inventario(lista_comandi+temp_id);
        
        if(ricerca_in_stanza == true)
            oggetto_interazione = ricercare_nome_oggetto_in_stanza(stanza_corrente, argomento_comando);
        if(oggetto_interazione == NULL && ricerca_in_inventario == true)
            oggetto_interazione = ricercare_nome_oggetto_in_inventario(inventario, argomento_comando);

        if(oggetto_interazione == NULL && argomento_inventario == false)
        {
            if     (ricerca_in_stanza == true && ricerca_in_inventario == false)
                printf("[?] Non vedo nessun %s nella stanza.\n", argomento_comando);
            else if(ricerca_in_stanza == false && ricerca_in_inventario == true)
                printf("[?] Non vedo nessun %s nell'inventario.\n", argomento_comando);
            else if(ricerca_in_stanza == true && ricerca_in_inventario == true)
                printf("[?] Non vedo nessun %s nella stanza o nell'inventario.\n", argomento_comando);
            return;
        }
        if(temp_id != ID_COMANDO_APRI && argomento_inventario == false)
        {
            attributo_da_ricercare     = leggere_regex_argomento(lista_comandi+temp_id)[0];
            pos_attributo_da_ricercare = ricercare_attributo(leggere_attributi_oggetto(oggetto_interazione), attributo_da_ricercare);
            if(pos_attributo_da_ricercare == -1 || argomento_inventario == true)
            {
                if(argomento_comando != NULL)
                    printf("[?] Non posso fare questa azione su %s.\n", argomento_comando );
                else
                    printf("[?] Non posso fare questa azione.\n");
            }
            else
            {
                if(temp_id != ID_COMANDO_ISPEZIONA && temp_id != ID_COMANDO_USA)
                {
                    id_ultimo_oggetto_ispezionato = -1;
                }
                    
                if     (temp_id == ID_COMANDO_SALI)      eseguire_sali      (mappa_gioco, oggetto_interazione);
                else if(temp_id == ID_COMANDO_SCENDI)    eseguire_scendi    (mappa_gioco, oggetto_interazione,             protagonista);
                else if(temp_id == ID_COMANDO_ISPEZIONA) eseguire_ispeziona (             oggetto_interazione);
                else if(temp_id == ID_COMANDO_PRENDI)    eseguire_prendi    (             oggetto_interazione, inventario, protagonista);
                else if(temp_id == ID_COMANDO_USA)       eseguire_usa       (mappa_gioco, oggetto_interazione,             protagonista);
                else if(temp_id == ID_COMANDO_MANGIA)    eseguire_mangia    (mappa_gioco, oggetto_interazione,             protagonista);
                else if(temp_id == ID_COMANDO_EQUIPAGGIA)eseguire_equipaggia(mappa_gioco, oggetto_interazione, inventario, protagonista);
                else if(temp_id == ID_COMANDO_LEGGI)     eseguire_leggi     (             oggetto_interazione);
                else if(temp_id == ID_COMANDO_UCCIDI)    eseguire_uccidi    (mappa_gioco, oggetto_interazione,             protagonista);
            }
        }
        else if(temp_id == ID_COMANDO_APRI && argomento_inventario == false)
        {
            if(oggetto_interazione == NULL)
                printf("[?] Non so cosa dovrei aprire.\n");
            else
                eseguire_apri(mappa_gioco, oggetto_interazione, inventario);
        }
        else if(temp_id != ID_COMANDO_APRI && argomento_inventario == true)
            printf("[?] L'inventario puo' essere solo aperto.\n    Digita 'apri inventario'.");
        else
            eseguire_apri_inventario(mappa_gioco, protagonista, inventario);
    }  
}

/* ricercare_nome_oggetto_in_stanza
* Questa funzione prende in input il nome di un oggetto che si vuole ricercare nella stanza, lo ricerca
* nell'array degli oggetti della stanza corrente e se lo trova restituisce l'oggetto.
* Se non lo trova restituisce un puntatore a NULL.
*/
oggetto * ricercare_nome_oggetto_in_stanza(stanza * stanza_corrente, char * nome_oggetto)
{
    oggetto * oggetto_trovato;
    oggetto * oggetto_corrente;
    char temp_nome_oggetto[LUNG_NOME_MAX];
    bool fine_ricerca;
    int i;
    
    oggetto_trovato = NULL;
    fine_ricerca = false;
    i=0;
    while(i<NUM_OGGETTI_STANZA_MAX && fine_ricerca == false)
    {
        oggetto_corrente = leggere_oggetto_stanza(stanza_corrente, i);
        if(leggere_id_oggetto(oggetto_corrente)!= -1 && leggere_visibilita_oggetto(oggetto_corrente) == true)
        {
            strncpy(temp_nome_oggetto, leggere_nome_oggetto(oggetto_corrente), LUNG_NOME_MAX);
            togliere_maiuscole_da_stringa(temp_nome_oggetto);
            if(strcmp(temp_nome_oggetto, nome_oggetto) == 0)
            {
                oggetto_trovato = oggetto_corrente;
                fine_ricerca = true;
            }
        }
        i++;
    }
    if(oggetto_trovato != NULL)
        return oggetto_trovato;

    // Se non trovo nulla nella stanza faccio la ricerca negli oggetti contenuti
    fine_ricerca = false;
    i=0;
    while(i<NUM_OGGETTI_CONTENUTI_PER_STANZA_MAX && fine_ricerca == false)
    {
        oggetto_corrente = leggere_oggetto_contenuto(stanza_corrente, i);
        if(leggere_id_oggetto(oggetto_corrente)!= -1 && leggere_visibilita_oggetto(oggetto_corrente) == true)
        {
            strncpy(temp_nome_oggetto, leggere_nome_oggetto(oggetto_corrente), LUNG_NOME_MAX);
            togliere_maiuscole_da_stringa(temp_nome_oggetto);
            if(strcmp(temp_nome_oggetto, nome_oggetto) == 0)
            {
                oggetto_trovato = oggetto_corrente;
                fine_ricerca = true;
            }
        }
        i++;
    }
    return oggetto_trovato;
}

/* ricercare_nome_oggetto_in_inventario
* Questa funzione prende in input il nome di un oggetto che si vuole ricercare nell'inventario, lo ricerca
* nell'array degli oggetti dell'inventario e se lo trova restituisce l'oggetto.
* Se non lo trova restituisce un puntatore a NULL.
*/
oggetto * ricercare_nome_oggetto_in_inventario(oggetto * inventario, char * nome_oggetto)
{
    oggetto * oggetto_trovato;
    oggetto * oggetto_corrente;
    char temp_nome_oggetto[LUNG_NOME_MAX];
    bool fine_ricerca;
    int i;
    
    oggetto_trovato = NULL;
    fine_ricerca = false;
    i=0;
    while(i<NUM_OGGETTI_INVENTARIO_MAX && fine_ricerca == false)
    {
        oggetto_corrente = inventario+i;
        if(leggere_id_oggetto(oggetto_corrente)!= -1)
        {
            strncpy(temp_nome_oggetto, leggere_nome_oggetto(oggetto_corrente), LUNG_NOME_MAX);
            togliere_maiuscole_da_stringa(temp_nome_oggetto);
            if(strcmp(temp_nome_oggetto, nome_oggetto) == 0)
            {
                oggetto_trovato = oggetto_corrente;
                fine_ricerca = true;
            }
        }
        i++;
    }
    return oggetto_trovato;
}

/*=========================COMANDO VAI=========================
* La funzione vai permette lo spostamento del giocatore da una stanza all'altra.
* Il giocatore puo' spostarsi a nord, a est, a sud, a ovest oppure puo' spostarsi perche'
* ha varcato una porta. Ad esempio nella stanza 12 il giocatore puo' scrivere 'apri porta saloon'
* e spostarsi nella stanza 120.
* Questa funzione riceve in input la direzione che puo' avere uno dei seguenti valori:
* "nord"/"est"/"sud"/"ovest"/"porta". Riceve in input anche l'inventario (perche' ad es.
* per poter andare in una direzione serve oltrepassare una porta che richiede una chiave)
* Riceve in input anche l'id della stanza dove il protagonista si trovera' dopo essersi spostato.
* id_stanza_protagonista viene calcolato prima di eseguire la funzione eseguire_vai e
* puo' avere i possibili valori:
* - id_stanza_corrente-leggere_col(mappa) se il protagonista vuole andare a nord
* - id_stanza_corrente+1 se il protagonista vuole andare a est
* - id_stanza_corrente+leggere_col(mappa) se il protagonista vuole andare a sud
* - id_stanza_corrente-1 se il protagonista vuole andare a ovest
* - id_stanza_arrivo_porta della porta se il protagonista vuole varcare una porta
*
* Innanzitutto controlliamo che l'id_stanza_arrivo_protagonista sia l'id di una stanza
* direttamente collegata. Se si ricerchiamo una porta nella stanza che ci consente di spostarci nella direzione voluta. Infine controlliamo che nell'inventario ci siano tutte
* le chiavi necessarie per varcare tale porta.
*/
void eseguire_vai(mappa      * mappa_gioco,
                  const char   DIREZIONE[6],
                  oggetto    * inventario,
                  int          id_stanza_arrivo_protagonista)
{
    //int id_stanza_arrivo_protagonista;
    int esito_ricerca_collegamento_stanza, esito_ricerca_porta_in_stanza;

    //id_stanza_arrivo_protagonista = id_stanza_arrivo_porta;
    
    if(strcmp(DIREZIONE, "porta") == 0)
    {
        stanza * stanza_corrente;
        int temp_id, temp_id_nord, temp_id_est, temp_id_sud, temp_id_ovest;

        stanza_corrente = leggere_stanza_da_mappa(mappa_gioco, riga_stanza_corrente, col_stanza_corrente);
        temp_id_nord  = leggere_collegamento_id_stanza(stanza_corrente, 0);
        temp_id_est   = leggere_collegamento_id_stanza(stanza_corrente, 1);
        temp_id_sud   = leggere_collegamento_id_stanza(stanza_corrente, 2);
        temp_id_ovest = leggere_collegamento_id_stanza(stanza_corrente, 3);

        if     (id_stanza_arrivo_protagonista == temp_id_nord)
            eseguire_vai(mappa_gioco, "nord", inventario, id_stanza_arrivo_protagonista);
        else if(id_stanza_arrivo_protagonista == temp_id_est)
            eseguire_vai(mappa_gioco, "est", inventario, id_stanza_arrivo_protagonista);
        else if(id_stanza_arrivo_protagonista == temp_id_sud)
            eseguire_vai(mappa_gioco, "sud", inventario, id_stanza_arrivo_protagonista);
        else if(id_stanza_arrivo_protagonista == temp_id_ovest)
            eseguire_vai(mappa_gioco, "ovest", inventario, id_stanza_arrivo_protagonista);
        return;
    }
    
    esito_ricerca_collegamento_stanza = ricercare_collegamento_stanza(mappa_gioco, id_stanza_arrivo_protagonista, DIREZIONE);
    if(esito_ricerca_collegamento_stanza >= 0 && id_stanza_arrivo_protagonista >= 0)
    {
        esito_ricerca_porta_in_stanza = ricercare_porta_in_stanza(mappa_gioco, id_stanza_corrente_protagonista, id_stanza_arrivo_protagonista, inventario);
        if( esito_ricerca_porta_in_stanza >= 0)
        {
            /*In questo caso, vuol dire che la stanza e' collegata con la stanza dove il protagonista
            * vuole andare. Inoltre c'e' una porta. Infine il protagonista ha tutte le chiavi per varcare la porta.
            * OK, il protagonista si reca con successo nella stanza
            */
            nascondere_oggetti_contenuti_stanza(mappa_gioco, riga_stanza_corrente, col_stanza_corrente);

            id_stanza_precedente = id_stanza_corrente_protagonista;
            aggiornare_id_stanza_corrente(mappa_gioco, id_stanza_arrivo_protagonista);

            eseguire_guarda(mappa_gioco, riga_stanza_corrente, col_stanza_corrente);
        }
        else if(esito_ricerca_porta_in_stanza == -1)
        {
            /*In questo caso, vuol dire che la stanza e' collegata con la stanza dove il protagonista
            * vuole andare, ma non c'e' una porta (perche' ad esempio tra la stanza "spiazzale del castello" e "piazza principale" non c'e' una porta).
            OK, il protagonista si reca con successo nella stanza
            */
            nascondere_oggetti_contenuti_stanza(mappa_gioco, riga_stanza_corrente, col_stanza_corrente);

            id_stanza_precedente = id_stanza_corrente_protagonista;
            aggiornare_id_stanza_corrente(mappa_gioco, id_stanza_arrivo_protagonista);

            eseguire_guarda(mappa_gioco, riga_stanza_corrente, col_stanza_corrente);
        }
        else if(esito_ricerca_porta_in_stanza == -2)
        {
            // (vincolo 11a)
            if(id_stanza_corrente_protagonista == ID_STANZA_PIAZZA)
            {
                printf("[#] Stai per entrare nel palazzo, cerchi di aprire la porta facendo meno rumore possibile.\n");
                eseguire_ispeziona(leggere_oggetto_stanza(leggere_stanza_da_mappa(mappa_gioco, riga_stanza_corrente, col_stanza_corrente), 4));
            }
            else
                printf("[?] Non sembra aprirsi, hai bisogno di qualcosa per varcarlo.\n");
        }
    }
    else
    {
        printf("[?] A %s non c'e' nessuna via d'uscita, solo un muro.\n    Vai in qualche direzione o interagisci con un oggetto nella stanza.\n", DIREZIONE);
    }
}

/* ricercare_collegamento_stanza
* Questa funzione prende in input la direzione in cui il protagonista si vuole muovere e 
* la relativa stanza di arrivo. Controlla che la stanza di arrivo sia una delle stanze direttamente
* collegate alla stanza corrente. Se si', restituisce un valore >=0, altrimenti -1.
*/
int ricercare_collegamento_stanza(mappa * mappa_gioco, int id_stanza_arrivo_protagonista, const char DIREZIONE[6])
{
    stanza * stanza_corrente;
    int pos_stanza, pos_collegamento_ricercato;
    //se scrivo stanza_corrente = malloc(sizeof(stanza)); e poi al termine del metodo chiamo free(stanza_corrente) il programma da' errore free(): invalid pointer perche' l'indirizzo che malloc assegna cambia e quindi non e' piu' quello che la funzione malloc aveva riservato

    stanza_corrente = leggere_stanza_da_mappa(mappa_gioco, riga_stanza_corrente, col_stanza_corrente);
    pos_collegamento_ricercato = -1;

    if     (strcmp(DIREZIONE, "nord")==0)  pos_stanza = 0;
    else if(strcmp(DIREZIONE, "est") ==0)  pos_stanza = 1;
    else if(strcmp(DIREZIONE, "sud") ==0)  pos_stanza = 2;
    else                                   pos_stanza = 3;
    
    if(id_stanza_arrivo_protagonista == leggere_collegamento_id_stanza(stanza_corrente, pos_stanza))
    {
        pos_collegamento_ricercato = pos_stanza;
    }
    else
    {
        pos_collegamento_ricercato = -1;
    }
    return pos_collegamento_ricercato;
}

int ricercare_porta_in_stanza(mappa * mappa_gioco,
                             int id_stanza_corrente_protagonista,
                             int id_stanza_arrivo_protagonista,
                             oggetto * inventario)
{
    stanza        * stanza_corrente;
    oggetto       * oggetto_corrente;
    oggetto_porta * porta_corrente;
    int i,j;
    int pos_porta_ricercata;
    int chiavi_trovate;

    stanza_corrente = leggere_stanza_da_mappa(mappa_gioco, riga_stanza_corrente, col_stanza_corrente);
    pos_porta_ricercata = -1;
    i=0;
    while(i<NUM_OGGETTI_STANZA_MAX)
    {
        oggetto_corrente = leggere_oggetto_stanza(stanza_corrente, i);
        if(leggere_visibilita_oggetto(oggetto_corrente) == true)
        {
            if(leggere_tipo_oggetto(oggetto_corrente) == ID_TIPO_OGGETTO_PORTA &&
               leggere_id_oggetto(oggetto_corrente)   != ID_OGGETTO_CAMINO )
            {
                porta_corrente = (oggetto_porta *) ottenere_oggetto_specifico_da_oggetto(mappa_gioco, oggetto_corrente, ID_TIPO_OGGETTO_PORTA);

                if(leggere_id_stanza_partenza_porta(porta_corrente) == id_stanza_corrente_protagonista && leggere_id_stanza_arrivo_porta(porta_corrente) == id_stanza_arrivo_protagonista)
                {
                    //Esiste una porta per la direzione del protagonista
                    pos_porta_ricercata = i;
                    //Controllo che il protagonista abbia tutte gli oggetti richiesti per varcare la porta
                    chiavi_trovate = ricercare_id_in_inventario(inventario, leggere_id_chiavi_porta(porta_corrente), NUM_CHIAVI_PORTA_MAX);
                    if(chiavi_trovate == -1)
                    {
                        pos_porta_ricercata = -2;
                    }
                }
            }
        }
        i++;
    }
    return pos_porta_ricercata;
}

/*=========================COMANDO SALI=========================
* Questa funzione cambia la posizione del protagonista con la posizione in cui si giunge
* dopo aver salito oggetto_corrente
*/
void eseguire_sali(mappa * mappa_gioco, oggetto * oggetto_corrente)
{
    oggetto_porta * porta_stanza;

    porta_stanza = (oggetto_porta *) ottenere_oggetto_specifico_da_oggetto(mappa_gioco, oggetto_corrente, ID_TIPO_OGGETTO_PORTA);

    id_stanza_precedente = id_stanza_corrente_protagonista;
    aggiornare_id_stanza_corrente(mappa_gioco, leggere_id_stanza_arrivo_porta(porta_stanza));

    eseguire_guarda(mappa_gioco, riga_stanza_corrente, col_stanza_corrente);
}

/*=========================COMANDO SCENDI=========================
* Questa funzione cambia la posizione del protagonista con la posizione in cui si giunge
* dopo aver sceso oggetto_corrente
*/
void eseguire_scendi(mappa * mappa_gioco, oggetto * oggetto_corrente, oggetto_personaggio * protagonista)
{
    oggetto_porta * porta_stanza;

    //(vincolo 8a)
    if(leggere_id_oggetto(oggetto_corrente) == ID_OGGETTO_CAMINO)
    {
        printf("[#] <<< AARGH!! >>>\n"
               "    ...............\n"
               "    ...............\n"
               "    Hai provato ad infilarti nel camino ma ad un tratto finisci rovinosamente in una trappola!\n");
        scrivere_salute_personaggio(protagonista, leggere_salute_personaggio(protagonista)*0, true); // Il protagonista perde una vita
        return;
    }

    porta_stanza = (oggetto_porta *) ottenere_oggetto_specifico_da_oggetto(mappa_gioco, oggetto_corrente, ID_TIPO_OGGETTO_PORTA);

    id_stanza_precedente = id_stanza_corrente_protagonista;
    aggiornare_id_stanza_corrente(mappa_gioco, leggere_id_stanza_arrivo_porta(porta_stanza));

    eseguire_guarda(mappa_gioco, riga_stanza_corrente, col_stanza_corrente);
}

/*=========================COMANDO SALTA=========================
* Questa funzione cambia la posizione del protagonista con la posizione. 
* Come descritto dal vincolo, il protagonista nel gioco puo' saltare dopo esser giunto in
* stanza 170
*/
void eseguire_salta(mappa * mappa_gioco)
{
    // (vincolo 170)
    if(id_stanza_corrente_protagonista == ID_STANZA_ALBERO)
    {
        system("clear");
        aggiornare_id_stanza_corrente(mappa_gioco, ID_STANZA_SCUDERIA);
        printf("[#] Hai fatto un salto incredibile e sei arrivato sul tetto della scuderia!\n"
        "    In meno di un secondo riesci ad accovacciarti e ad entrare nella fessura. Tiri un sospiro di sollievo.\n    Dove ti trovi? Guardati intorno.\n");
    }
    else
    {
        //Il protagonista puo' saltare solo nella stanza 170
        printf("[#] Hai saltato per cercare di aggrapparti a qualcosa ma fallisci miseramente nell'intento.\n");
    }
}

/*=========================COMANDO GUARDA=========================
* Questa funzione ottiene in input la mappa e le coordinate di una cella (cioe' una stanza).
* Elenca, mediante i metodi di accesso alla struttura mappa, le stanze direttamente collegate
* e gli oggetti presenti in tale stanza.
*/
void eseguire_guarda(mappa * mappa_gioco, int val_riga, int val_col)
{
    stanza * stanza_corrente;
    oggetto * oggetto_corrente;
    int i, c;

    stanza_corrente = leggere_stanza_da_mappa(mappa_gioco, val_riga, val_col);

    if(id_stanza_precedente != id_stanza_corrente_protagonista)
    {
        system("clear");
        mostrare_mini_mappa(mappa_gioco, leggere_collegamenti_id_stanza(stanza_corrente));
        printf("\n     , , ,\n%s\n     , , ,\n\n", leggere_descrizione_stanza(stanza_corrente));
    }

    printf("[i] Qui trovi:");
    int temp_id;
    i=0;
    while(i<NUM_OGGETTI_STANZA_MAX)
    {
        oggetto_corrente = leggere_oggetto_stanza(stanza_corrente, i);
        if(leggere_visibilita_oggetto(oggetto_corrente) == true && leggere_id_oggetto(oggetto_corrente) != -1)
        {
            printf("\n( ) %s",leggere_nome_oggetto(oggetto_corrente));
            
            /* Mostro dei simboli per far capire al giocatore che la scala porta giu' (si puo' scendere) o su (si puo' salire)*/
            c=0;
            while(c<20-strlen(leggere_nome_oggetto(oggetto_corrente)))
            {
                printf(" ");
                c++;
            }
            if(leggere_tipo_oggetto(oggetto_corrente) == ID_TIPO_OGGETTO_PORTA)
            {
                if     (ricercare_attributo(leggere_attributi_oggetto(oggetto_corrente), 's') >=0)
                    printf(" [v]");
                else if(ricercare_attributo(leggere_attributi_oggetto(oggetto_corrente), 'S') >=0)
                    printf(" [^]");
            }
        }
        i++;
    }
    c=0;
    i=0;
    while(i<NUM_OGGETTI_CONTENUTI_PER_STANZA_MAX)
    {
        oggetto_corrente = leggere_oggetto_contenuto(stanza_corrente, i);
        if(leggere_visibilita_oggetto(oggetto_corrente) == true)
        {
            c++;
            if(c==1)
                printf("\n[i] Dentro l'oggetto aperto trovi:");
            printf("\n(-) %s", leggere_nome_oggetto(oggetto_corrente));
        }
        i++;
    }
    id_stanza_precedente = id_stanza_corrente_protagonista;
}

/*=========================COMANDO ISPEZIONA=========================
* Questa funzione mostra la descrizione narrata dell'oggetto e pone l'id dell'oggetto
* come id dell'ultimo oggetto ispezionato. Questo serve per la funzione usa, che dipende
* fortemente dall'id dell'ultimo oggetto ispezionato.
*/
void eseguire_ispeziona(oggetto * oggetto_corrente)
{
    id_ultimo_oggetto_ispezionato = leggere_id_oggetto(oggetto_corrente);
    printf("[i] %s\n",leggere_descrizione_oggetto(oggetto_corrente));
}

/*=========================COMANDO PRENDI=========================
* Questa funzione sposta l'oggetto dalla lista degli oggetti della stanza nell'inventario.
*/
void eseguire_prendi(oggetto * oggetto_corrente, oggetto * inventario, oggetto_personaggio * protagonista)
{
    oggetto * oggetto_corrente_inventario;
    bool fine_ricerca;
    int i;
    // (vincolo 4)
    if(leggere_id_oggetto(oggetto_corrente) == ID_OGGETTO_GUFO)
    {
        printf("[#] Hai provato a prendere il Gufo Reale, ma questo e' riuscito ad uscire dalla gabbia e a volare in alto. Mentre lo guardi, fa alcuni versi che attirano le guardie.\n    Cerchi di uscire dalla stanza ma vieni scoperto da un gruppo di guardie.\n\n");
        scrivere_salute_personaggio(protagonista, leggere_salute_personaggio(protagonista)*0, true);
        return;
    }

    // (vincolo vittoria gioco)
    if(leggere_id_oggetto(oggetto_corrente) == ID_OGGETTO_CORONA)
    {
        game_win = true;
    }
    
    fine_ricerca = false;
    i=0;
    while(i<NUM_OGGETTI_INVENTARIO_MAX && fine_ricerca == false)
    {
        oggetto_corrente_inventario = inventario+i;
        // Appena trovo un oggetto non valido nell'inventario, lo sostituisco con l'oggetto preso
        if(leggere_id_oggetto(oggetto_corrente_inventario) == -1 || leggere_visibilita_oggetto(oggetto_corrente_inventario) == false )
        {
            *oggetto_corrente_inventario = *oggetto_corrente;
            scrivere_visibilita_oggetto(oggetto_corrente_inventario, true);
            fine_ricerca = true;
        }
        i++;
    }
    if(fine_ricerca == false)
    {
        printf("[i] L'inventario e' pieno, non puoi prendere piu' niente\n");
    }
    else
    {
        if(leggere_tipo_oggetto(oggetto_corrente) == ID_TIPO_OGGETTO_ARMA)
            printf("[i] Preso. Ricordati di equipaggiarlo.\n");
        else
            printf("[i] Preso.\n");
        //L'oggetto nella stanza non esiste piu', e' stato spostato nell'inventario
        rendere_oggetto_non_valido(oggetto_corrente);
    }
}

/*=========================COMANDO APRI=========================
* Questa funzione verifica se l'oggetto da aprire sia un oggetto che ti conduce da qualche
* parte (perche' e' una porta) oppure un oggetto che contiene altri oggetti. In quest'ultimo caso
* mostra gli oggetti contenuti nella stanza, mentre nel primo caso esegue il comando vai.
*/
void eseguire_apri(mappa * mappa_gioco, oggetto * oggetto_corrente, oggetto * inventario)
{
    //A -> porta    a -> oggetto genitore
    stanza  * stanza_corrente;
    oggetto * oggetto_contenuto_corrente;
    char attributo_oggetto_apribile = 'a';
    int pos_attributo_ricercato;
    int i;

    stanza_corrente = leggere_stanza_da_mappa(mappa_gioco, riga_stanza_corrente, col_stanza_corrente);

    pos_attributo_ricercato = ricercare_attributo(leggere_attributi_oggetto(oggetto_corrente), attributo_oggetto_apribile);
    if(pos_attributo_ricercato == -1)
    {
        attributo_oggetto_apribile = 'A';
        pos_attributo_ricercato = ricercare_attributo(leggere_attributi_oggetto(oggetto_corrente), attributo_oggetto_apribile);

        if(pos_attributo_ricercato == -1)
        {
            printf("[?] %s non si puo' aprire.\n",leggere_nome_oggetto(oggetto_corrente));
            return;
        }

        oggetto_porta * porta_corrente;
        int id_stanza_arrivo_porta;

        porta_corrente = (oggetto_porta *) ottenere_oggetto_specifico_da_oggetto(mappa_gioco, oggetto_corrente, ID_TIPO_OGGETTO_PORTA);
        id_stanza_arrivo_porta = leggere_id_stanza_arrivo_porta(porta_corrente);

        /* Per poter eseguire il comando vai, e' necessario specificare una direzione o in
        * alternativa la stringa 'porta'. Per questo convertiamo la destinazione della porta in una direzione
        */
        if(id_stanza_arrivo_porta == id_stanza_corrente_protagonista - leggere_col(mappa_gioco))
            eseguire_vai(mappa_gioco, "nord", inventario, id_stanza_arrivo_porta);
        else if(id_stanza_arrivo_porta == id_stanza_corrente_protagonista + 1)
            eseguire_vai(mappa_gioco, "est", inventario, id_stanza_arrivo_porta);
        else if(id_stanza_arrivo_porta == id_stanza_corrente_protagonista + leggere_col(mappa_gioco))
            eseguire_vai(mappa_gioco, "sud", inventario, id_stanza_arrivo_porta);
        else if(id_stanza_arrivo_porta == id_stanza_corrente_protagonista - 1)
            eseguire_vai(mappa_gioco, "ovest", inventario, id_stanza_arrivo_porta);
        else
        {
            eseguire_vai(mappa_gioco, "porta", inventario, id_stanza_arrivo_porta);
        }
    }
    else
    {
        i=0;
        while(i<NUM_OGGETTI_CONTENUTI_PER_STANZA_MAX)
        {
            oggetto_contenuto_corrente = leggere_oggetto_contenuto(stanza_corrente, i);
            if((int)(leggere_id_oggetto(oggetto_contenuto_corrente)/10) == leggere_id_oggetto(oggetto_corrente))
            {
                scrivere_visibilita_oggetto(oggetto_contenuto_corrente, true);
            }
            i++;
        }
        id_stanza_precedente = -1;
        eseguire_guarda(mappa_gioco, riga_stanza_corrente, col_stanza_corrente); 
    }
}

/* eseguire_apri_inventario
* Questa funzione mostra lo stato del giocatore e gli oggetti presi.
*/
void eseguire_apri_inventario(mappa               * mappa_gioco,
                              oggetto_personaggio * protagonista,
                              oggetto             * inventario)
{
    oggetto      * oggetto_corrente;
    oggetto_arma * arma_corrente;
    oggetto_cibo * cibo_corrente;
    int i, c;
    int id_tipo_corrente;

    printf("\n   _____________________________________________. _. _ ..\r\n"
				"  (__________[GESTIONE STATO GIOCATORE]______._._. . . \r\n"
				"   |                                                     \r\n" 
				"   |  	     [==] Giocatore: %s\r\n" 
				"   |         [<3] Vite rimaste: %d\r\n   |\n"
				"   >    [+] Salute      : %d punti\r\n" 
				"   |    [w] Forza       : %d punti\r\n   |",
                nome_giocatore,
                num_vite_corrente,
                leggere_salute_personaggio(protagonista),
                leggere_forza_personaggio(protagonista));

    printf("\n   |          .:: [Oggetti generici] ::.");
    id_tipo_corrente = -1;
    i=0;
    while(i<NUM_OGGETTI_INVENTARIO_MAX)
    {
        oggetto_corrente = inventario+i;
        if(leggere_id_oggetto(oggetto_corrente) != -1   &&
           leggere_visibilita_oggetto(oggetto_corrente) &&
           leggere_tipo_oggetto(oggetto_corrente) == id_tipo_corrente)
        {
            if(leggere_tipo_oggetto(oggetto_corrente) == ID_TIPO_OGGETTO_ARMA)
            {
                arma_corrente = (oggetto_arma *) ottenere_oggetto_specifico_da_oggetto(mappa_gioco, oggetto_corrente, ID_TIPO_OGGETTO_ARMA);
                if(leggere_stato_equipaggiato_arma(arma_corrente) == true)
                    printf("\n   |  (E) %s [+%d/+%d]", leggere_nome_oggetto(oggetto_corrente), leggere_bonus_salute_arma(arma_corrente), leggere_bonus_forza_arma(arma_corrente));
                else
                    printf("\n   |  ( ) %s [+%d/+%d]", leggere_nome_oggetto(oggetto_corrente), leggere_bonus_salute_arma(arma_corrente), leggere_bonus_forza_arma(arma_corrente));
            }
            else if(leggere_tipo_oggetto(oggetto_corrente) == ID_TIPO_OGGETTO_CIBO)
            {
                cibo_corrente = (oggetto_cibo *) ottenere_oggetto_specifico_da_oggetto(mappa_gioco, oggetto_corrente, ID_TIPO_OGGETTO_CIBO);
                printf("\n   |  ( ) %s [+%d]", leggere_nome_oggetto(oggetto_corrente), leggere_bonus_salute_cibo(cibo_corrente));
            }
            else
                printf("\n   |   °  %s", leggere_nome_oggetto(oggetto_corrente));
            c++;
        }
        i++;
        if(i==NUM_OGGETTI_INVENTARIO_MAX)
        {
            i=0;
            if(id_tipo_corrente == -1)
            {
                if(c==0)
                    printf("\n   |  [i] Nessun oggetto raccolto fino ad ora.");
                c=0;
                id_tipo_corrente = 1;                  
                printf("\n   |\n   |          .:: [Oggetti   cibo  ] ::.");
            }
            else if(id_tipo_corrente == 1)
            {
                if(c==0)
                    printf("\n   |  [i] Nessun cibo raccolto fino ad ora.");
                c=0;
                id_tipo_corrente = 2;
                printf("\n   |\n   |          .:: [Oggetti   armi  ] ::.");
            }
            else if(id_tipo_corrente == 2)
            {
                if(c==0)
                    printf("\n   |  [i] Nessun arma raccolta fino ad ora.");
                c=0;
                i=NUM_OGGETTI_INVENTARIO_MAX;
            } 
        }
    }
    printf("\n   |__/\\____________________/\\/\\__________/\\____._. . .\r\n");
}

/*=========================COMANDO USA=========================
* Questa funzione esegue uno dei seguenti vincoli in base all'ultimo oggetto ispezionato
* e all'oggetto che si vuole utilizzare.
*/
void eseguire_usa(mappa * mappa_gioco, oggetto * oggetto_corrente, oggetto_personaggio * protagonista)
{
    int id_oggetto_utilizzato;
    id_oggetto_utilizzato = leggere_id_oggetto(oggetto_corrente);

    /* Il comando usa e' l'unica funzione del gioco la cui logica di funzionamento e' molto
    * legata ai vincoli e alla storia del gioco.
    */
    //printf("----%d-----%d----",id_ultimo_oggetto_ispezionato, id_oggetto_utilizzato);

    if     (id_ultimo_oggetto_ispezionato == ID_OGGETTO_POZZO && id_oggetto_utilizzato == ID_OGGETTO_FUNE)
    {
        // (vincolo 13)
        scrivere_visibilita_oggetto(leggere_oggetto_stanza(leggere_stanza_da_mappa(mappa_gioco,riga_stanza_corrente, col_stanza_corrente),3), true);
        rendere_oggetto_non_valido(oggetto_corrente);
        printf("[#] Hai attaccato la fune alla carrucola del pozzo e strinto piu' che puoi.\n"
               "    Ora c'e' un passaggio per andare giu' al pozzo. NEAT!\n    Guarda intorno.\n");
    }
    else if(id_ultimo_oggetto_ispezionato == ID_OGGETTO_FUOCHERELLO && id_oggetto_utilizzato == ID_OGGETTO_CANDELABRO_SPENTO )
    {
        // (vincolo 2)
        scrivere_id_oggetto(oggetto_corrente, ID_OGGETTO_CANDELABRO_ACCESO);
        printf("[#] Hai avvicinato il candelabro al fuoco per accenderlo e creare una torcia.\n"
               "    Ora puoi esplorare il palazzo reale. Ottimo lavoro.\n");
    }
    else if(id_ultimo_oggetto_ispezionato == ID_OGGETTO_LIBRERIA)
    {
        // (vincolo 10a/10b)
        if(id_oggetto_utilizzato != ID_OGGETTO_PIUMA_GUFO)
        {
            printf("[#] ...... <<< CLANG!! >>>\n"
               "    Dopo un attimo di silenzio, senti un forte rumore dalla libreria.\n"
               "    Capisci instantaneamente: usare %s aziona una TRAPPOLA!!!\n"
               "    Senti udire dei passi dalla porta...\n"
               "    Spalanchi gli occhi e ti giri.\n    Un gruppo di guardie entra nella stanza e ti uccide.\n", leggere_nome_oggetto(oggetto_corrente));
               scrivere_salute_personaggio(protagonista, leggere_salute_personaggio(protagonista)*0, true);
        }
        else
        {
            scrivere_visibilita_oggetto(leggere_oggetto_stanza(leggere_stanza_da_mappa(mappa_gioco,riga_stanza_corrente, col_stanza_corrente),7), true);
            rendere_oggetto_non_valido(oggetto_corrente);
            printf("[#] Hai posizionato la piuma del Gufo Reale in un involucro nascosto nella libreria.\n"
               "    Non credi ai tuoi occhi, la piuma e' sospesa e sta girando su se' stessa!\n"
               "    Scorgi la testa per terra e noti che c'e' un portale chiuso.\n");
        }
    }
    else if(id_ultimo_oggetto_ispezionato == -1)
    {
        printf("[?] Non puoi utilizzare %s senza prima aver ispezionato qualcosa.\n", leggere_nome_oggetto(oggetto_corrente));
    }
    else
    {
        printf("[?] Non succede niente, probabilmente non puoi usare %s dopo aver ispezionato quell'oggetto.\n", leggere_nome_oggetto(oggetto_corrente));
    }
}


/*=========================COMANDO MANGIA=========================
* Questa funzione incrementa i punti salute del protagonista ed elimina l'oggetto mangiato.
*/
void eseguire_mangia(mappa * mappa_gioco, oggetto * oggetto_corrente, oggetto_personaggio * protagonista)
{
    oggetto_cibo * cibo_generico;
    cibo_generico = (oggetto_cibo *) ottenere_oggetto_specifico_da_oggetto(mappa_gioco, oggetto_corrente, ID_TIPO_OGGETTO_CIBO);

    printf("[i] Mangiando %s i tuoi punti salute sono aumentati di %d.\n", leggere_nome_oggetto(oggetto_corrente), leggere_bonus_salute_cibo(cibo_generico) );
    scrivere_salute_personaggio(protagonista, leggere_salute_personaggio(protagonista) + leggere_bonus_salute_cibo(cibo_generico), true);

    rendere_oggetto_non_valido(oggetto_corrente);
}

/*=========================COMANDO EQUIPAGGIA=========================
* Questa funzione incrementa i punti salute e forza del protagonista aggiornando il flag che
* segna se un'arma e' gia' stata equipaggiata.
*/
void eseguire_equipaggia(mappa * mappa_gioco, oggetto * oggetto_corrente, oggetto * inventario, oggetto_personaggio * protagonista)
{
    oggetto_arma * arma_da_equipaggiare;
    oggetto_arma * arma_gia_equipaggiata;
    int differenza_bonus_salute;
    
    arma_da_equipaggiare = (oggetto_arma *) ottenere_oggetto_specifico_da_oggetto(mappa_gioco, oggetto_corrente, ID_TIPO_OGGETTO_ARMA);

    if(leggere_stato_equipaggiato_arma(arma_da_equipaggiare) == true)
    {
        printf("[i] %s e' un'arma gia' equipaggiata.\n",leggere_nome_oggetto(oggetto_corrente));
    }
    else
    {
        arma_gia_equipaggiata = ricercare_arma_equipaggiata(mappa_gioco, inventario);
        if(arma_gia_equipaggiata != NULL)
        {
            // In questo caso andiamo a disequipaggiare l'arma gia' equipaggiata.

            differenza_bonus_salute = - leggere_bonus_salute_arma(arma_gia_equipaggiata) + leggere_bonus_salute_arma(arma_da_equipaggiare);

            scrivere_salute_personaggio(protagonista, leggere_salute_personaggio(protagonista) + differenza_bonus_salute, true);
            
            scrivere_forza_personaggio(protagonista, leggere_forza_personaggio(protagonista) - leggere_bonus_forza_arma(arma_gia_equipaggiata));
            scrivere_forza_personaggio(protagonista, leggere_forza_personaggio(protagonista) + leggere_bonus_forza_arma(arma_da_equipaggiare));

            scrivere_stato_equipaggiato_arma(arma_gia_equipaggiata, false);
            scrivere_stato_equipaggiato_arma(arma_da_equipaggiare, true);
        }
        else
        {
            scrivere_salute_personaggio(protagonista, leggere_salute_personaggio(protagonista) + leggere_bonus_salute_arma(arma_da_equipaggiare), true);

            scrivere_forza_personaggio(protagonista, leggere_forza_personaggio(protagonista) + leggere_bonus_forza_arma(arma_da_equipaggiare));

            scrivere_stato_equipaggiato_arma(arma_da_equipaggiare, true);
        }
        printf("[i] Hai equipaggiato %s.\n",leggere_nome_oggetto(oggetto_corrente));
    }
}

/* ricercare_arma_equipaggiata
* Questa funzione restituisce il puntatore all'oggetto arma gia' equipaggiato.
* Restituisce null se non esiste un'arma gia' equipaggiata.
*/
oggetto_arma * ricercare_arma_equipaggiata(mappa * mappa_gioco, oggetto * inventario)
{
    oggetto      * oggetto_corrente_inventario;
    oggetto_arma * arma_corrente_inventario;
    int i;
    bool fine_ricerca;

    arma_corrente_inventario = NULL;
    fine_ricerca = false;
    i=0;
    while(i<NUM_OGGETTI_INVENTARIO_MAX && fine_ricerca == false)
    {
        oggetto_corrente_inventario = inventario+i;
        if(leggere_tipo_oggetto(oggetto_corrente_inventario) == ID_TIPO_OGGETTO_ARMA &&
           leggere_visibilita_oggetto(oggetto_corrente_inventario) == true )
        {
            arma_corrente_inventario = (oggetto_arma *) ottenere_oggetto_specifico_da_oggetto(mappa_gioco, oggetto_corrente_inventario, ID_TIPO_OGGETTO_ARMA);

            if(leggere_stato_equipaggiato_arma(arma_corrente_inventario) == true)
            {
                fine_ricerca = true;
            }
        }
        i++;
    }
    if(fine_ricerca == true)
        return arma_corrente_inventario;
    else
        return NULL;
}

/*=========================COMANDO LEGGI=========================
* Questa funzione accede al file dove sono memorizzati tutti i messaggi leggibili.
*/
void eseguire_leggi(oggetto * oggetto_corrente)
{
    int temp_id, i;
    char temp_messaggio[LUNG_DESCRIZIONE_MAX];
    char c;
    bool fine_ricerca;
        
    FILE * txt_file_lista_messaggi;
    txt_file_lista_messaggi = fopen("../data/lista_messaggi_oggetti_leggibili.txt", "r");
    if(txt_file_lista_messaggi == NULL)
    {
        printf("\n[X] Errore nell'apertura del file dei messaggi");
        esito_controllo = false;
        return;
    }

    fine_ricerca = false;
    while(!feof(txt_file_lista_messaggi) && fine_ricerca == false)
    {
        //Preleva l'id dell'oggetto su cui c'e' scritto qualcosa
        fscanf(txt_file_lista_messaggi, "%d", &temp_id);
        //Preleva il messaggio dell'oggetto fino al carattere di fine campo (&)
        c = fgetc(txt_file_lista_messaggi);
        i=0;
        while(c != '&' && c!= EOF)
        {
            c = fgetc(txt_file_lista_messaggi);
            if(c != '&' && c!=EOF)
            {
                temp_messaggio[i]=c;
                i++;
            }
        }
        temp_messaggio[i]='\0';
        //Se l'id letto dal file corrisponde all'id dell'oggetto che l'utente vuole leggere,
        //stampa il messaggio.
        if(temp_id == leggere_id_oggetto(oggetto_corrente))
        {
            printf("[i] Trovi scritto:\n    \"%s\"",temp_messaggio);
            fine_ricerca = true;
        }
    }
    fclose(txt_file_lista_messaggi);
}

/*=========================COMANDO UCCIDI=========================
* Questa funzione avvia il sistema di combattimento e decrementa la salute del protagonista
* in quantita' pari al danno ricevuto.
*/
void eseguire_uccidi(mappa * mappa_gioco, oggetto * oggetto_corrente,oggetto_personaggio * protagonista)
{
    oggetto_personaggio * nemico;
    oggetto             * oggetto_stanza_corrente;
    int danno_subito;
    int i,j;

    nemico = (oggetto_personaggio *) ottenere_oggetto_specifico_da_oggetto(mappa_gioco, oggetto_corrente, ID_TIPO_OGGETTO_PERSONAGGIO);

    system("clear");
    danno_subito = avviare_sistema_combattimento(mappa_gioco, protagonista, oggetto_corrente);
    scrivere_salute_personaggio(protagonista, leggere_salute_personaggio(protagonista) - danno_subito, true);

    if(leggere_salute_personaggio(protagonista) > 0 && leggere_salute_personaggio(nemico) <= 0)
    {
        if(leggere_id_oggetto(oggetto_corrente) == ID_OGGETTO_RE)
        {
            printf("\n[i] E' tutto vero, hai ucciso il Re. Prendi la corona, presto!\n");
        }
        else
            printf("\n[i] Hai ucciso %s.\n    Dopo lo scontro ti sei pulito gli indumenti dal sangue.\n    Guardati intorno per vedere cosa puoi recuperare da %s.\n", leggere_nome_oggetto(oggetto_corrente),
            leggere_nome_oggetto(oggetto_corrente));
        
        /* Poiche' il sistema da combattimento termina quando uno dei due personaggi
        * muore, in questo caso se la salute del protagonista e' >0 significa che il nemico
        * e' morto.
        */
        i=0;
        while(i<NUM_OGGETTI_RILASCIATI_MAX)
        {
            //rendi visibile ciascuno degli oggetti rilasciati dal nemico
            j=0;
            while(j<NUM_OGGETTI_STANZA_MAX)
            {
                oggetto_stanza_corrente = leggere_oggetto_stanza(leggere_stanza_da_mappa(mappa_gioco, riga_stanza_corrente, col_stanza_corrente), j);
                if(leggere_id_oggetto(oggetto_stanza_corrente) == leggere_id_oggetto_rilasciato(nemico, i))
                {
                    scrivere_visibilita_oggetto(oggetto_stanza_corrente, true);
                    j=NUM_OGGETTI_STANZA_MAX;
                }
                j++;
            }
            i++;
        }
        //Il nemico e' morto, quindi non e' piu' visibile nella stanza.
        rendere_oggetto_non_valido(oggetto_corrente);
    }
    else
    {
        printf("\n[i] %s ti ha sconfitto.\n    Per fortuna i tuoi punti salute sono ripristinati, mentre %s e' ferito.\n", leggere_nome_oggetto(oggetto_corrente),
        leggere_nome_oggetto(oggetto_corrente));

    }
}

/* avviare_sistema_combattimento
* Questa funzione restistuisce il danno che il protagonista riceve battendosi con il nemico.
*/
int avviare_sistema_combattimento(mappa * mappa_gioco, oggetto_personaggio * protagonista, oggetto * oggetto_nemico)
{
    oggetto_personaggio * nemico;
    int salute_protagonista, forza_protagonista;
    int salute_nemico,       forza_nemico;
    int dado;
    int danno, danno_differenza_salute, danno_nemico;
    nemico = (oggetto_personaggio *) ottenere_oggetto_specifico_da_oggetto(mappa_gioco, oggetto_nemico, ID_TIPO_OGGETTO_PERSONAGGIO);

    danno=0;
    salute_protagonista = leggere_salute_personaggio(protagonista);
    forza_protagonista  = leggere_forza_personaggio (protagonista);
    salute_nemico       = leggere_salute_personaggio(nemico);
    forza_nemico        = leggere_forza_personaggio (nemico);

    printf("\n -._.-=-._.-=-._.-=-._.-[SCONTRO]-._.-=-._.-=-._.-=-._.-\n"
           "\n  [i] Nome giocatore    : %s"
           "\n  [i] Nemico            : %s"
           "\n  [>] Vite rimaste      : %d"
           "\n  [+] Salute  io/nemico : %d / %d"
           "\n  [w] Forza   io/nemico : %d / %d\n\n",
           nome_giocatore,
           leggere_nome_oggetto(oggetto_nemico),
           num_vite_corrente,
           leggere_salute_personaggio(protagonista), leggere_salute_personaggio(nemico),
           leggere_forza_personaggio (protagonista), leggere_forza_personaggio(nemico));

    //In base al timestamp corrente ottenuto con time(0), srand() crea un seed per la generazione di numeri casuali.
    srand(time(0)); 
    while(salute_protagonista > 0 && salute_nemico > 0)
    {
        dado = generare_numero_casuale(1, 15);
        if(dado <= forza_protagonista)
        {
            if(dado%2 == 0)
            {
                salute_nemico-=forza_protagonista+2;
                scrivere_salute_personaggio(nemico, leggere_salute_personaggio(nemico) - (forza_protagonista+2), false);
                printf("[%d/%d] Bel colpo, hai buttato %s a terra!\n",salute_protagonista, salute_nemico, leggere_nome_oggetto(oggetto_nemico));
            }
            else
            {
                salute_nemico-=forza_protagonista;
                scrivere_salute_personaggio(nemico, leggere_salute_personaggio(nemico) - (forza_protagonista), false);
                printf("[%d/%d] Stai soffocando %s!\n",salute_protagonista, salute_nemico, leggere_nome_oggetto(oggetto_nemico));
            }

            danno_differenza_salute = generare_numero_casuale(0, (int)(forza_protagonista/3));
            salute_protagonista-=danno_differenza_salute;
            danno+=danno_differenza_salute;
            printf("[%d/%d] %s non sembra arrendersi e ti ha colpito!\n",salute_protagonista, salute_nemico, leggere_nome_oggetto(oggetto_nemico));
        }
        else if(dado > forza_protagonista && dado <= forza_nemico)
        {
            if(forza_protagonista < forza_nemico)
            {
                salute_protagonista-=dado;
                danno+=dado;
                printf("[%d/%d] %s ti ha colpito con la sua spada!\n",salute_protagonista, salute_nemico, leggere_nome_oggetto(oggetto_nemico));
            }
            else
            {
                danno_nemico = (int)generare_numero_casuale(forza_nemico+1, forza_protagonista-1)/3;
                salute_nemico-=danno_nemico;
                scrivere_salute_personaggio(nemico, leggere_salute_personaggio(nemico) - danno_nemico, false);
            }
        }
        else
        {
            if(forza_nemico > forza_protagonista)
            {
                if(dado<=forza_nemico+2)
                {
                    salute_protagonista-=(int)(forza_nemico/2);
                    danno+=(int)(forza_nemico/2);
                }
                else
                {
                    salute_protagonista-=(int)(dado/2);
                    danno+=(int)(dado/2);
                    salute_nemico-=(int)(dado/2);
                    scrivere_salute_personaggio(nemico, leggere_salute_personaggio(nemico) - (int)(dado/2), false);
                }
                printf("[%d/%d] %s ti ha scaraventato a terra!\n",salute_protagonista, salute_nemico, leggere_nome_oggetto(oggetto_nemico));
            }
            else
            {
                if(generare_numero_casuale(1, 2) == 1)
                {
                    salute_protagonista--;
                    danno++;
                }
                else
                {
                    salute_nemico--;
                    scrivere_salute_personaggio(nemico, leggere_salute_personaggio(nemico) - 1, false);
                } 
            }
            /*
            if(dado <= forza_nemico+2)
            {
                salute_protagonista-=(int)(forza_nemico/2);
                danno+=(int)(forza_nemico/2);
            }
            else
            {
                salute_protagonista-=(int)(dado/2);
                danno+=(int)(dado/2);

                salute_nemico-=(int)(dado/2);
                scrivere_salute_personaggio(nemico, leggere_salute_personaggio(nemico) - ((int)(dado/2)), false);
            }
            printf("[%d/%d] %s ti ha ferito ma sei riuscito a controbattere!\n",salute_protagonista, salute_nemico, leggere_nome_oggetto(oggetto_nemico));
            */
        }
    }
    printf("\n-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-._.-=-\n");
    return danno;
}

/* generare_numero_casuale
* Questa funzione genera un numero casuale i cui valori minimo e massimo sono specificati come parametri.
*/
int generare_numero_casuale(int numero_minimo, int numero_massimo)
{
    return (rand() % (numero_massimo-numero_minimo+1)) + numero_minimo;
}
