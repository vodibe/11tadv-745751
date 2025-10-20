#ifndef FUNZIONI_GENERALI_H_
    #define FUNZIONI_GENERALI_H_
    #include "costanti_variabili_globali.h"
    #include "funzioni_comandi.h"
    #include "mappa.h"
    #include "tipo_stanza.h"
    
    /* ----- FUNZIONI DI ANALISI LESSICALE E OTTIMIZZAZIONE DEL COMANDO -----*/
    void eseguire_scanner(comando             * lista_comandi,
                      char                  comando_utente[LUNG_STRINGA_MAX],
                      oggetto_personaggio * protagonista,
                      oggetto             * inventario,
                      mappa               * mappa_gioco,
                      stanza              * stanza_corrente);
    void   togliere_maiuscole_da_stringa(char * stringa_da_elaborare);
    char * togliere_spazi_da_bordi_stringa(char * stringa_da_elaborare);
    void   togliere_spazi_multipli(char * stringa_da_elaborare);

    /* ----- FUNZIONI DI INPUT -----*/
    bool leggere_nome_giocatore(char nome_giocatore[LUNG_USERNAME_MAX]);
    int  leggere_intero(int * intero);
    
    /* ----- FUNZIONI CHE OPERANO SULLA POSIZIONE DEL PROTAGONISTA -----*/
    void aggiornare_id_stanza_corrente(mappa * mappa_gioco, int id_stanza_corrente);

    /* ----- FUNZIONI CHE OPERANO SUGLI OGGETTI DI UNA STANZA -----*/
    void rendere_oggetto_non_valido(oggetto * oggetto_da_annullare);
    void nascondere_oggetti_contenuti_stanza(mappa * mappa_gioco, int val_riga, int val_col);
    void nascondere_oggetti_rilasciati(mappa * mappa_gioco);
    int  ricercare_attributo(char * attributi_oggetto, char attributo_da_ricercare);
    stanza * leggere_stanza_oggetto(mappa * mappa_gioco, int id_oggetto);

    /* ----- FUNZIONI CHE OPERANO SULL'INVENTARIO -----*/
    int ricercare_id_in_inventario(oggetto * inventario, int * lista_id_da_ricercare, const int DIMENSIONE_LISTA);

    /* ----- FUNZIONI CHE OPERANO SULLA PARTITA -----*/
    void salvare_partita (oggetto_personaggio * protagonista, oggetto * inventario, mappa * mappa_gioco);
    void caricare_partita(oggetto_personaggio * protagonista, oggetto * inventario, mappa * mappa_gioco);

    

#endif