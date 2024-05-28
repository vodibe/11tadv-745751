#ifndef TIPO_STANZA_H_
    #define TIPO_STANZA_H_
    #include "costanti_variabili_globali.h"
    #include "tipo_oggetto.h"
    
    /* Descrizione Tipo Stanza
    * Questo tipo di dato rappresenta una stanza della mappa. Questa si compone di:
    * - ID stanza (identificatore univoco)
    * - Un array di ID stanze collegate alla stanza corrente
    *   collegamenti_id_stanza[0] contiene l'ID della stanza collegata a nord
    *   collegamenti_id_stanza[1] contiene l'ID della stanza collegata a est
    *   collegamenti_id_stanza[2] contiene l'ID della stanza collegata a sud
    *   collegamenti_id_stanza[3] contiene l'ID della stanza collegata a ovest
    * - Un array di oggetti (che comprende sia i visibili che i non visibili) presenti nella stanza
    * - La descrizione narrata della stanza
    */
    typedef struct
    {
        int     id_stanza;
        int     collegamenti_id_stanza [NUM_STANZE_COLLEGATE_MAX];
        oggetto lista_oggetti_stanza   [NUM_OGGETTI_STANZA_MAX];
        oggetto lista_oggetti_contenuti[NUM_OGGETTI_CONTENUTI_PER_STANZA_MAX];
        char    descrizione_stanza     [LUNG_DESCRIZIONE_MAX];
    } stanza;

    /* ----- METODI DI ACCESSO ALLA STRUTTURA oggetto -----*/
    int leggere_id_stanza(stanza * stanza_mappa);

    int leggere_collegamento_id_stanza(stanza * stanza_mappa, int pos_stanza);
    int * leggere_collegamenti_id_stanza(stanza * stanza_mappa);

    oggetto * leggere_oggetto_stanza   (stanza * stanza_mappa, int val_indice_oggetto);
    oggetto * leggere_oggetto_contenuto(stanza * stanza_mappa, int val_indice_oggetto);

    char * leggere_descrizione_stanza(stanza * stanza_mappa);
    
    /* ----- METODI AGGIUNTIVI DELLA STRUTTURA stanza -----*/
    void creare_stanza_da_file(stanza * stanza_da_aggiungere, int i_riga, int j_col, const int VAL_RIGHE_MAPPA, const int VAL_COL_MAPPA);

#endif