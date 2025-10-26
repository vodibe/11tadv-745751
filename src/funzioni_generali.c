#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include "funzioni_comandi.h"
#include "funzioni_generali.h"
#include "funzioni_regex.h"

/* ----- FUNZIONI DI ANALISI LESSICALE E OTTIMIZZAZIONE DEL COMANDO -----*/
void eseguire_scanner(comando             * lista_comandi,
                      char                  comando_utente[LUNG_STRINGA_MAX],
                      oggetto_personaggio * protagonista,
                      oggetto             * inventario,
                      mappa               * mappa_gioco,
                      stanza              * stanza_corrente)
{
    char * verbo_comando;      verbo_comando     = malloc(LUNG_STRINGA_MAX);
    char * argomento_comando;  argomento_comando = malloc(LUNG_STRINGA_MAX);
    bool   regex_match;

    /*1 - Ottimizzazione stringa input */
    togliere_maiuscole_da_stringa(comando_utente);
    comando_utente = togliere_spazi_da_bordi_stringa(comando_utente);
    
    /*2 - Controllo che il comando non sia vuoto */
    if(comando_utente[0] == '\0' || comando_utente[0] == '\n')
    {
        printf("[?] Non posso perdere tempo, cosa devo fare?\n");
        return;
    }

    /*3 - Applicazione espressione regolare*/
    regex_match = applicare_regex(comando_utente, "[^a-zA-Z ][^a-zA-Z ]*");
    if(regex_match == true)
    {
        printf("[?] Dimmi qualcosa di sensato..\n");
        return;
    }

    /*4 - Estrazione token
    * Il C mette a disposizione la funzione strtok che, come uno scanner, suddivide la stringa in token a patto
    * che il programmatore specifichi il separatore di token.
    * Nel gioco, un comando deve avere la forma "verbo argomento" dove:
    * - verbo e' una singola parola, dunque verbo e argomento sono separati dal carattere spazio ' '
    * - argomento e' una o piu' parole separate da uno spazio, pertanto leggo il token fino a quando non incontro '\0'
    */
    verbo_comando     = strtok(comando_utente, " ");
    argomento_comando = strtok(NULL, "\0");
    if(argomento_comando != NULL)
        argomento_comando = togliere_spazi_da_bordi_stringa(argomento_comando);

    //printf("%s----%s", verbo_comando, argomento_comando);
    
    /* 5 - Chiamata del parser */
    eseguire_parser(lista_comandi,
                    verbo_comando,
                    argomento_comando,
                    protagonista,
                    inventario,
                    mappa_gioco,
                    stanza_corrente);
}

void togliere_maiuscole_da_stringa(char * stringa_da_elaborare)
{
    int i;
    i=0;
    while(i<strlen(stringa_da_elaborare))
    {
        stringa_da_elaborare[i] = tolower(stringa_da_elaborare[i]);
        i++;
    }
}

char * togliere_spazi_da_bordi_stringa(char * stringa_ptr)
{
    char * carattere_finale;

    while(isspace((unsigned char) *stringa_ptr))
    {
        //Incremento il puntatore al primo carattere della stringa finche' il primo carattere
        // e' un carattere NON spazio.
        stringa_ptr++;
    }

    if(*stringa_ptr == 0)  
        return stringa_ptr;

    carattere_finale = stringa_ptr + strlen(stringa_ptr) - 1;
    while(carattere_finale > stringa_ptr && isspace((unsigned char)*carattere_finale))
    {
        //Stesso algoritmo precedente, con l'unica differenza che partiamo dalla fine della
        //stringa per togliere gli spazi finali
        carattere_finale--;
    }
    carattere_finale[1] = '\0';
    return stringa_ptr;
}

void togliere_spazi_multipli(char * stringa_da_elaborare)
{
    int i,j;
    j=0;
    i=0;
    while(i<strlen(stringa_da_elaborare))
    {
        if(!isspace(stringa_da_elaborare[i]) || (i > 0 && !isspace(stringa_da_elaborare[i-1])))
            stringa_da_elaborare[j++] = stringa_da_elaborare[i];
        i++;
    } 
    stringa_da_elaborare[j] = '\0';
}

/* ----- FUNZIONI DI INPUT -----*/
/* leggere_nome_giocatore
* Funzione che legge il nome utente del giocatore
*/
bool leggere_nome_giocatore(char nome_giocatore[LUNG_USERNAME_MAX+1])
{
    //char buffer[1024];  //l'utente puo' scrivere fino a 1024 caratteri su una linea
    bool regex_match;

    printf("[i] ");
    fgets(buffer, sizeof(buffer), stdin);
    //Se la lunghezza del nome supera 10 caratteri+1 (\0), scrivo un errore
    if(strlen(buffer) > LUNG_USERNAME_MAX+1)
    {
        printf("[X] L'username deve avere lunghezza massima di %d.\n", LUNG_USERNAME_MAX);
        return false;
    }
    //Controllo che il nome utente non sia vuoto
    buffer[strlen(buffer)-1] = '\0';
    if(buffer[0] == '\0')
    {
        printf("[X] Il tuo username non puo' essere vuoto.\n");
        return false;
    }
    //Applicazione dell'espressione regolare
    regex_match = applicare_regex(buffer, "[^a-zA-Z0-9][^a-zA-Z0-9]*");
    if(regex_match == true)
    {
        printf("[X] L'username immesso contiene simboli non ammessi.\n");
        return false;
    }
    else
    {
        //Copio il contenuto del buffer nell'array nome_giocatore
        strncpy(nome_giocatore, buffer, (LUNG_USERNAME_MAX+1));
        return true;
    }
}

/* leggere_intero
* FONTE: http://sekrit.de/webdocs/c/beginners-guide-away-from-scanf.html
* Funzione ri-adattata dal sito sovra citato
* che NON utilizza scanf per l'input di interi ma fgets (piu' sicuro)
*/
int leggere_intero(int * intero)
{
    long a;  // Numero letto dal buffer.

    if (!fgets(buffer, 1024, stdin))  // se la funzione fgets fallisce l'acquisizione
        return 0;

    char *endptr;

    errno = 0;                      // reset del codice d'errore
    a = strtol(buffer, &endptr, 10);   
    if (errno == ERANGE)            // se supera il range per il formato long
        return 0;
    if (endptr == buffer)           // se nessun carattere e' stato letto
        return 0;

    // se *endptr non e' ne' \0 ne' \n, significa che non abbiamo convertito l'intero input
    if (*endptr && *endptr != '\n')
        return 0;

    if (a > INT_MAX || a < INT_MIN) // se il risultato non entra in un int
        return 0;

    // scrivi il risultato
    *intero = (int) a;
    return 1;       
}

/* ----- FUNZIONI CHE OPERANO SULLA POSIZIONE DEL PROTAGONISTA -----*/
/* aggiornare_id_stanza_corrente
* Questa funzione cambia la stanza in cui si trova il protagonista, aggiornando anche le variabili globali
* della riga e colonna della mappa relativi alla nuova stanza.
*/
void aggiornare_id_stanza_corrente(mappa * mappa_gioco, int id_stanza_corrente)
{
    FILE * file_lista_id_stanze_isolate;
    int temp_id, j;

    id_stanza_corrente_protagonista = id_stanza_corrente;
    if(id_stanza_corrente <= leggere_col(mappa_gioco)*(leggere_righe(mappa_gioco)-1))
    {
        riga_stanza_corrente = (int)id_stanza_corrente/leggere_col(mappa_gioco);
        col_stanza_corrente = id_stanza_corrente-(riga_stanza_corrente*leggere_col(mappa_gioco))-1;
        /* Se la mappa e' di dimensioni 5x5, le stanze con ID 5/10/15/20 avranno
        * riga_stanza_corrente errato e col_stanza_corrente errato.
        * Esempio: id_stanza_corrente   = 10
        *          riga_stanza_corrente = 10/5 = 2        (errato perche'dovrebbe essere 1)
        *          col_stanza_corrente  = 10-(2*5)-1 = -1 (errato)
        */
        if(col_stanza_corrente == -1)
        {
            riga_stanza_corrente--;
            col_stanza_corrente = leggere_col(mappa_gioco)-1;
        }
    }
    else
    {
        file_lista_id_stanze_isolate = fopen("data/lista_id_stanze_isolate.txt", "r");
        if(file_lista_id_stanze_isolate == NULL)
        {
            esito_controllo = false;
            return;
        }
        j=0;
        while(j<leggere_col(mappa_gioco))
        {
            fscanf(file_lista_id_stanze_isolate, "%d", &temp_id);
            if(temp_id == id_stanza_corrente)
            {
                riga_stanza_corrente = leggere_righe(mappa_gioco)-1;
                col_stanza_corrente = j;
                j=leggere_col(mappa_gioco);
            }
            j++;
        }
        fclose(file_lista_id_stanze_isolate);
    }  
}

/* leggere_stanza_oggetto
* Questa funzione restituisce la stanza dove e' presente un oggetto della mappa.
*/
stanza * leggere_stanza_oggetto(mappa * mappa_gioco, int id_oggetto)
{
    FILE * file_lista_id_stanze_isolate;
    int riga_stanza_oggetto, col_stanza_oggetto;
    int temp_id, j;

    if(id_oggetto <= leggere_col(mappa_gioco)*(leggere_righe(mappa_gioco)-1)*NUM_OGGETTI_STANZA_MAX)
    {
        riga_stanza_oggetto = (int) ( ((int)(id_oggetto/NUM_OGGETTI_STANZA_MAX))  /   leggere_col(mappa_gioco));
        col_stanza_oggetto  = (int)(id_oggetto/NUM_OGGETTI_STANZA_MAX)-(riga_stanza_oggetto*leggere_col(mappa_gioco))-1;
        if(col_stanza_oggetto == -1)
        {
            riga_stanza_oggetto--;
            col_stanza_oggetto = leggere_col(mappa_gioco)-1;
        }
    }
    else
    {
        file_lista_id_stanze_isolate = fopen("data/lista_id_stanze_isolate.txt", "r");
        if(file_lista_id_stanze_isolate == NULL)
        {
            esito_controllo = false;
            riga_stanza_oggetto = 0;
            col_stanza_oggetto = 0;
        }
        j=0;
        while(j<leggere_col(mappa_gioco))
        {
            fscanf(file_lista_id_stanze_isolate, "%d", &temp_id);
            if(temp_id == (int)(id_oggetto/NUM_OGGETTI_STANZA_MAX))
            {
                riga_stanza_oggetto = leggere_righe(mappa_gioco)-1;
                col_stanza_oggetto = j;
                j=leggere_col(mappa_gioco);
            }
            j++;
        }
        fclose(file_lista_id_stanze_isolate);
    }
    return leggere_stanza_da_mappa(mappa_gioco, riga_stanza_oggetto, col_stanza_oggetto); 
}

/* ----- FUNZIONI CHE OPERANO SUGLI OGGETTI DI UNA STANZA -----*/
/* rendere_oggetto_non_valido
* Questa funzione prende in input un puntatore ad un oggetto ed utilizza le funzioni di accesso agli attributi
* di oggetto per rendere non valido l'oggetto 
*/
void rendere_oggetto_non_valido(oggetto * oggetto_da_annullare)
{
    scrivere_id_oggetto         (oggetto_da_annullare, -1);
    scrivere_id_oggetto_genitore(oggetto_da_annullare, -1);
    scrivere_nome_oggetto       (oggetto_da_annullare, "n");
    scrivere_descrizione_oggetto(oggetto_da_annullare, "n");
    scrivere_attributi_oggetto  (oggetto_da_annullare, "n");
    scrivere_visibilita_oggetto (oggetto_da_annullare, false);
}

/* nascondere_oggetti_contenuti_stanza
* Se il protagonista apre un oggetto nella stanza, puo' vedere quali altri oggetti sono contenuti.
* Se pero' esce dalla stanza i cassetti ed altri oggetti apribili saranno di nuovo chiusi.
* Con questa funzione nascondiamo gli oggetti contenuti in una stanza 
*/
void nascondere_oggetti_contenuti_stanza(mappa * mappa_gioco, int val_riga, int val_col)
{
    stanza * stanza_corrente;
    oggetto * oggetto_contenuto_corrente;
    int i;

    stanza_corrente = leggere_stanza_da_mappa(mappa_gioco, val_riga, val_col);
    i=0;
    while(i<NUM_OGGETTI_CONTENUTI_PER_STANZA_MAX)
    {
        oggetto_contenuto_corrente = leggere_oggetto_contenuto(stanza_corrente, i);
        scrivere_visibilita_oggetto(oggetto_contenuto_corrente, false);
        i++;
    }
}

/* nascondere_oggetti_rilasciati
* Questa funzione nasconde gli oggetti che diventano visibili solo dopo aver ucciso aventuali
* personaggi del gioco.
*/
void nascondere_oggetti_rilasciati(mappa * mappa_gioco)
{
    oggetto_personaggio * personaggio_corrente;
    stanza              * stanza_personaggio_corrente;
    oggetto             * oggetto_corrente;
    int id_oggetto_rilasciato_corrente, riga_stanza_personaggio, col_stanza_personaggio;
    int i,j,c;
    i=0;
    while(i<NUM_PERSONAGGI_MAPPA_MAX)
    {
        personaggio_corrente = leggere_personaggio_da_mappa(mappa_gioco, i);
        if(leggere_id_personaggio(personaggio_corrente) > 0)
        {
            /*Siccome gli oggetti personaggio non si spostano durante il gioco, calcoliamo
            * in quale stanza si trovano mediante l'id */
            stanza_personaggio_corrente = leggere_stanza_oggetto(mappa_gioco, leggere_id_personaggio(personaggio_corrente));
            j=0;
            while(j<NUM_OGGETTI_RILASCIATI_MAX)
            {
                id_oggetto_rilasciato_corrente = leggere_id_oggetto_rilasciato(personaggio_corrente, j);
                if(id_oggetto_rilasciato_corrente > 0)
                {
                    c=0;
                    while(c<NUM_OGGETTI_STANZA_MAX)
                    {
                        oggetto_corrente = leggere_oggetto_stanza(stanza_personaggio_corrente, c);
                        if(leggere_id_oggetto(oggetto_corrente) == id_oggetto_rilasciato_corrente)
                        {
                            scrivere_visibilita_oggetto(oggetto_corrente, false);
                            
                            c = NUM_OGGETTI_STANZA_MAX;
                        }
                        c++;
                    }
                }
                j++;
            }
        }
        i++;
    }
}

/* ricercare_attributo
* Questa funzione analizza gli attributi di un oggetto alla ricerca di un attributo
* specificato come secondo parametro. Restituisce -1 se non lo trova, oppure la sua posizione
* nell'array degli attirbuti se lo trova. 
*/
int ricercare_attributo(char * attributi_oggetto, char attributo_da_ricercare)
{
    int pos_attributo_trovato;
    bool attributo_trovato;
    int i;

    pos_attributo_trovato = -1;
    attributo_trovato = false;
    i=0;
    while(i<NUM_ATTRIBUTI_OGGETTO_MAX && attributo_trovato == false)
    {
        if(attributi_oggetto[i] == attributo_da_ricercare)
        {
            attributo_trovato = true;
            pos_attributo_trovato = i;
        }
        i++;
    }
    return pos_attributo_trovato;
}

/* ----- FUNZIONI CHE OPERANO SULL'INVENTARIO -----*/
/* ricercare_id_in_inventario
* Questa funzione ricerca UNO O PIU' ID nell'inventario. E' necessario che il numero di ID da ricercare
* sia specificato come terzo parametro.
* Restituisce la posizione del primo elemento della lista degli ID. -1 se non lo/li trova
*/
int ricercare_id_in_inventario(oggetto * inventario, int * lista_id_da_ricercare, const int DIMENSIONE_LISTA)
{
    int pos_id_trovato;
    bool id_trovato;
    int i,j;

    if(DIMENSIONE_LISTA <= 1)       //basterebbe dimensione_lista == 1
    {
        pos_id_trovato = -1;
        id_trovato = false;
        j=0;
        while(j<NUM_OGGETTI_INVENTARIO_MAX && id_trovato == false)
        {
            if(leggere_id_oggetto(inventario+j) != -1 && leggere_id_oggetto(inventario+j) == *lista_id_da_ricercare)
            {
                pos_id_trovato = j;
                id_trovato = true;
            }
            j++;
        }
    }
    else
    {
        id_trovato = true;
        i=0;
        while(i<DIMENSIONE_LISTA)
        {
            //l'i-esimo oggetto (non vuoto) di lista da ricercare e' presente nell'inventario?
            if(*(lista_id_da_ricercare+i) != -1)
            {
                id_trovato = false;
                j=0;
                while(j<NUM_OGGETTI_INVENTARIO_MAX && id_trovato == false)
                {
                    if(leggere_id_oggetto(inventario+j) != -1 && leggere_id_oggetto(inventario+j) == *(lista_id_da_ricercare+i))
                    {
                        id_trovato = true;
                    }
                    j++;
                }
                if(id_trovato == false)
                {
                    i=DIMENSIONE_LISTA;
                }
            }
            i++;
        }
        if(id_trovato == true)
            pos_id_trovato = 0;
        else
            pos_id_trovato = -1;
    }
    return pos_id_trovato;
}


/* ----- FUNZIONI CHE OPERANO SULLA PARTITA -----*/
/* salvare_partita
* Questa funzione prende in input il nome del giocatore, crea il nome del file del salvataggio (file binario).
* Successivamente scrive nel file binario i seguenti elementi del gioco:
* - Numero vite corrente del protagonista
* - ID stanza corrente del protagonista
* - Struttura dati del protagonista
* - Inventario del protagonista
* - Mappa del gioco
* - Tutte le strutture dati delle stanze della mappa
*/
void salvare_partita(oggetto_personaggio * protagonista, oggetto * inventario, mappa * mappa_gioco)
{
    FILE                * file_salvataggio; 
    char                * path_file_salvataggio;
    stanza              * stanza_da_scrivere;
    oggetto_arma        * arma_equipaggiata;
    oggetto_personaggio * personaggio_corrente;
    int id_arma_equipaggiata;
    int i,j;
    
    /*Elaborazione del percorso del file da cui caricare i progressi */
    path_file_salvataggio = malloc(strlen("../slot/") + strlen(nome_giocatore) + strlen("_save.bin") + 1);
    //Se la malloc fallisce nel recuperare un indirizzo di un blocco contiguo di memoria dai errore
    if(path_file_salvataggio == NULL)
    {
        printf("\n[X] Errore nella scrittura del salvataggio.");
        esito_controllo = false;
        return;
    }
    strcpy(path_file_salvataggio, "../slot/");
    strcat(path_file_salvataggio, nome_giocatore);
    strcat(path_file_salvataggio, "_save.bin");

    /* Apertura del file */
    file_salvataggio = fopen(path_file_salvataggio, "wb");
    if(file_salvataggio == NULL)
    {
        printf("\n[X] Errore nella scrittura del salvataggio");
        esito_controllo = false;
        return;
    }

    arma_equipaggiata = ricercare_arma_equipaggiata(mappa_gioco, inventario);
    if(arma_equipaggiata == NULL)
        id_arma_equipaggiata = -1;
    else
        id_arma_equipaggiata = leggere_id_arma(arma_equipaggiata);

    /* Scrittura dei progressi su file */
    fwrite(&num_vite_corrente,               sizeof(int),                 1, file_salvataggio);
    fwrite(&id_stanza_corrente_protagonista, sizeof(int),                 1, file_salvataggio);
    fwrite(protagonista,                     sizeof(oggetto_personaggio), 1, file_salvataggio);
    fwrite(inventario,                       sizeof(oggetto), NUM_OGGETTI_INVENTARIO_MAX, file_salvataggio);
    fwrite(&id_arma_equipaggiata,            sizeof(int),                 1, file_salvataggio);
    fwrite(mappa_gioco,                      sizeof(mappa),               1, file_salvataggio);

    i=0;
    while(i<leggere_righe(mappa_gioco))
    {
        j=0;
        while(j<leggere_col(mappa_gioco))
        {
            stanza_da_scrivere = leggere_stanza_da_mappa(mappa_gioco,i,j);
            fwrite(stanza_da_scrivere,       sizeof(stanza),              1, file_salvataggio );
            j++;
        }
        i++;
    }
    i=0;
    while(i<NUM_PERSONAGGI_MAPPA_MAX)
    {
        personaggio_corrente = leggere_personaggio_da_mappa(mappa_gioco, i);
        fwrite(personaggio_corrente,         sizeof(oggetto_personaggio), 1, file_salvataggio );
        i++;
    }
    fclose(file_salvataggio);
    printf("[i] Scrittura dei progressi su %s avvenuta con successo!\n    Puoi continuare l'avventura.\n", path_file_salvataggio);
}

/* caricare_partita
* Questa funzione prende in input il nome del giocatore, crea il nome del file del caricamento (file binario).
* Successivamente legge dal file i seguenti elementi di gioco:
* - Numero vite corrente del protagonista
* - ID stanza corrente del protagonista
* - Struttura dati del protagonista
* - Inventario del protagonista
* - Mappa del gioco
* - Tutte le strutture dati delle stanze della mappa
*/
void caricare_partita(oggetto_personaggio * protagonista, oggetto * inventario, mappa * mappa_gioco)
{
    FILE                * file_caricamento; 
    char                * path_file_caricamento;
    mappa               * mappa_gioco2;          mappa_gioco2         = malloc(sizeof(mappa));
    stanza              * stanza_da_leggere;     stanza_da_leggere    = malloc(sizeof(stanza));
    oggetto_personaggio * personaggio_corrente;  personaggio_corrente = malloc(sizeof(oggetto_personaggio));
    int id_arma_equipaggiata;
    int i,j;
    
    /*Elaborazione del percorso del file da cui caricare i progressi */
    path_file_caricamento = malloc(strlen("../slot/") + strlen(nome_giocatore) + strlen("_save.bin") + 1);
    if(path_file_caricamento == NULL)
    {
        printf("\n[X] Errore durante il caricamento del salvataggio.");
        esito_controllo = false;
        return;
    }
    strcpy(path_file_caricamento, "../slot/");
    strcat(path_file_caricamento, nome_giocatore);
    strcat(path_file_caricamento, "_save.bin");

    /* Apertura del file */
    file_caricamento = fopen(path_file_caricamento, "rb");
    if(file_caricamento == NULL)
    {
        printf("[X] Il giocatore %s non ha mai salvato prima d'ora i propri progressi.", nome_giocatore);
        esito_controllo = false;
        return;
    }

    /* Lettura dei progressi dal file */
    fread(&num_vite_corrente,               sizeof(int),                 1, file_caricamento);
    fread(&id_stanza_corrente_protagonista, sizeof(int),                 1, file_caricamento);
    aggiornare_id_stanza_corrente(mappa_gioco, id_stanza_corrente_protagonista);
    fread(protagonista,                     sizeof(oggetto_personaggio), 1, file_caricamento);
    i=0;
    while(i<NUM_OGGETTI_INVENTARIO_MAX)
    {
        fread(inventario+i,                 sizeof(oggetto),             1, file_caricamento);
        i++;
    }
    fread(&id_arma_equipaggiata,            sizeof(int),                 1, file_caricamento);
    fread(mappa_gioco2,                     sizeof(mappa),               1, file_caricamento);
    i=0;
    while(i<leggere_righe(mappa_gioco))
    {
        j=0;
        while(j<leggere_col(mappa_gioco))
        {
            fread(stanza_da_leggere,        sizeof(stanza),              1, file_caricamento);
            scrivere_stanza_in_mappa(mappa_gioco, i,j, stanza_da_leggere);
            j++;
        }
        i++;
    }
    i=0;
    while(i<NUM_PERSONAGGI_MAPPA_MAX)
    {
        fread(personaggio_corrente,        sizeof(oggetto_personaggio), 1, file_caricamento );
        scrivere_personaggio_in_mappa(mappa_gioco, i, personaggio_corrente);
        i++;
    }
    fclose(file_caricamento);
    free(stanza_da_leggere);
    free(mappa_gioco2);

    /* Se al momento del salvataggio il protagonista aveva un'arma equipaggiata,
    * allora in fase di caricamento, devo impostare il relativo flag stato_equipaggiato_arma a true.
    * Per questo entro nel ciclo solo se id_arma_equipaggiata != -1
    */
    i=0;
    while(i<NUM_ARMI_MAPPA_MAX && id_arma_equipaggiata != -1)
    {
        if(leggere_id_arma(leggere_arma_da_mappa(mappa_gioco,i)) == id_arma_equipaggiata)
        {
            scrivere_stato_equipaggiato_arma(leggere_arma_da_mappa(mappa_gioco,i), true);
            i=NUM_ARMI_MAPPA_MAX;
        }
        i++;   
    }
    
    printf("[i] Lettura dei progressi da %s avvenuta con successo!\n    Guarda la stanza per sapere dove ti trovi.\n", path_file_caricamento);
}

