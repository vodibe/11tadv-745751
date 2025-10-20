#ifndef COSTANTI_VARIABILI_GLOBALI_H_
    #define COSTANTI_VARIABILI_GLOBALI_H_
    #include <stdbool.h>
    /* costanti_variabili_globali
    * Questo modulo del programma deve essere incluso in tutti gli altri moduli.
    * Qui sono memorizzati i parametri del gioco e le variabili globali.
    * In questo modulo avremmo potuto memorizzare variabili di tipo mappa, stanza eccetera
    * pero' abbiamo ritenuto 'snellire' questo modulo facendo in modo che esso includa solo la libreria
    * stdbool (per la gestione dei dati booleani) e non altre librerie con altri tipi di dato.
    */
    
    /*=====COSTANTI CLASSIFICAZIONE OGGETTI=====*/
    #define NUM_TIPI_OGGETTO_MAX                   5 
    #define ID_TIPO_OGGETTO_PORTA                  0 
    #define ID_TIPO_OGGETTO_CIBO                   1 
    #define ID_TIPO_OGGETTO_ARMA                   2 
    #define ID_TIPO_OGGETTO_PERSONAGGIO            3 
    #define ID_TIPO_OGGETTO_GENITORE               4 

    /*=====COSTANTI LUNGHEZZA ATTRIBUTI OGGETTI=====*/
    #define LUNG_NOME_MAX                          80 
    #define LUNG_DESCRIZIONE_MAX                   350 
    #define LUNG_ATTRIBUTI_MAX                     20 
    #define NUM_ATTRIBUTI_OGGETTO_MAX              11 

    /*=====COSTANTI RELATIVE ALLE STANZE=====    
    * Ogni stanza e' collegata al massimo con 4 stanze (nord, est, sud, ovest)
    * Ci sono al massimo 10 oggetti in ogni stanza
    * Ci sono al massimo 5 oggetti che sono contenuti in altri oggetti della stanza
    */
    #define NUM_STANZE_COLLEGATE_MAX               4 
    #define NUM_OGGETTI_STANZA_MAX                 10 
    #define NUM_OGGETTI_CONTENUTI_PER_STANZA_MAX   5  

    /*=====COSTANTI RELATIVE ALLA MAPPA=====    
    * Nella mappa ci possono essere al massimo 32 porte, 32 cibi, 32 armi, 32 personaggi.
    */
    #define NUM_PORTE_MAPPA_MAX                    32 
    #define NUM_CIBI_MAPPA_MAX                     32 
    #define NUM_ARMI_MAPPA_MAX                     32 
    #define NUM_PERSONAGGI_MAPPA_MAX               32 

    /*=====ALTRE COSTANTI=====   
    * Ogni porta della mappa puo' essere aperto con al massimo 4 oggetti.
    * Nell'inventario ci possono essere al massimo 80 oggetti.
    * Ogni personaggio del gioco (e quindi i nemici) possono rilasciare al massimo 5 oggetti.
    */
    #define NUM_CHIAVI_PORTA_MAX                   4 
    #define NUM_OGGETTI_INVENTARIO_MAX             80 
    #define NUM_OGGETTI_RILASCIATI_MAX             5 

    /*=====COSTANTI PROTAGONISTA=====    
    * La stanza iniziale del protagonista e' la stanza 18.
    * Il protagonista parte con 20 punti salute.
    * Il protagonista parte con 5 punti forza e puo' ottenerne masismo 15
    * Il protagonista parte con 3 vite. Il fatto che valga o meno la vita zero
    * lo stabiliamo nella funzione exit_game nel modulo main.
    */
    #define ID_STANZA_PROTAGONISTA_INIZIALE        18  // default: 18
    #define SALUTE_PROTAGONISTA_INIZIALE           20 
    #define FORZA_PROTAGONISTA_INIZIALE            5      
    #define FORZA_PERSONAGGIO_MAX                  15 
    #define NUM_VITE_PROTAGONISTA_INIZIALE         3 

    /*=====COSTANTI COMANDI=====    
    * Nel gioco ci sono 16 comandi
    */
    #define NUM_COMANDI_MAX                        16 

    #define LUNG_USERNAME_MAX                      10 
    #define LUNG_STRINGA_MAX                       64

    /* Descrizione variabili globali
    * I nomi delle variabili globali sono autoesplicativi.
    * - buffer ha una dimensione di 1KiB, il che vuol dire che ogni qualvolta l'utente deve digitare
    *   qualcosa da tastiera, puo' inserire al massimo 1024 caratteri.
    * - esito_controllo = flag che se posto a falso indica che c'e' stato un errore con i file.
    */
    char buffer[1024];
    
    char nome_giocatore[LUNG_USERNAME_MAX+1];
    bool esito_controllo;
    bool game_win;

    int riga_stanza_corrente;
    int col_stanza_corrente;
    int num_vite_corrente;
    
    int id_stanza_corrente_protagonista;
    int id_ultimo_oggetto_ispezionato;
    int id_stanza_precedente;    

#endif