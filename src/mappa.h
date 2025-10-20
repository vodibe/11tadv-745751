#ifndef MAPPA_H_
    #define MAPPA_H_
    #include "tipo_stanza.h"
    #include "tipo_oggetto_porta.h"
    #include "tipo_oggetto_cibo.h"
    #include "tipo_oggetto_arma.h"
    #include "tipo_oggetto_personaggio.h"

    /* Descrizione Struttura mappa
    * Analogalmente a una semplice tabella, la mappa e' un array bidimensionale in cui tutte
    * le celle hanno tipo 'stanza' e sono identificate da un numero di riga e colonna.
    * - Array bidimensionale di tipo stanza
    * - Numero di righe della mappa
    * - Numero di colonne della mappa
    * - Elenco delle porte, cibi, armi e personaggi del gioco.
    */
    typedef struct
    {
        stanza ** elenco_stanze;
        int righe;
        int col;
        oggetto_porta       elenco_porte     [NUM_PORTE_MAPPA_MAX];
        oggetto_cibo        elenco_cibi      [NUM_CIBI_MAPPA_MAX];
        oggetto_arma        elenco_armi      [NUM_ARMI_MAPPA_MAX];
        oggetto_personaggio elenco_personaggi[NUM_PERSONAGGI_MAPPA_MAX];
    } mappa;

    /* ----- METODI DI ACCESSO ALLA STRUTTURA mappa -----*/
    
    int leggere_righe(mappa * mappa_gioco);
    int leggere_col(mappa * mappa_gioco);

    void scrivere_stanza_in_mappa(mappa * mappa_gioco, int val_riga, int val_col, stanza * stanza_da_aggiungere);
    stanza * leggere_stanza_da_mappa(mappa * mappa_gioco, int val_riga, int val_col);
    
    oggetto_arma * leggere_arma_da_mappa(mappa * mappa_gioco, int val_pos);
    void scrivere_personaggio_in_mappa(mappa * mappa_gioco, int val_pos, oggetto_personaggio * personaggio_da_aggiungere);
    oggetto_personaggio * leggere_personaggio_da_mappa(mappa * mappa_gioco, int val_pos);

    /* ----- METODI AGGIUNTIVI DELLA STRUTTURA mappa -----*/
    void creare_mappa(const char * PATH_DIMENSIONE_MAPPA, mappa * mappa_gioco);
    void aggiungere_porte_in_mappa     (const char * PATH_LISTA_PORTE,      mappa * mappa_gioco);
    void aggiungere_stanze_in_mappa                                        (mappa * mappa_gioco);
    void aggiungere_cibi_in_mappa      (const char * PATH_LISTA_CIBO,       mappa * mappa_gioco);
    void aggiungere_armi_in_mappa      (const char * PATH_LISTA_ARMI,       mappa * mappa_gioco);
    void aggiungere_personaggi_in_mappa(const char * PATH_LISTA_PERSONAGGI, mappa * mappa_gioco);
    void * ottenere_oggetto_specifico_da_oggetto(mappa * mappa_gioco, oggetto * oggetto_gioco, const int tipo_oggetto_da_restituire);
    
#endif