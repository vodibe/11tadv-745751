#ifndef TIPO_OGGETTO_H_
    #define TIPO_OGGETTO_H_
    #include "costanti_variabili_globali.h"
    
    /* Descrizione Tipo Oggetto
    * Tale tipo di dato rappresenta un oggetto di gioco con cui il protagonista puo' interagire.
    * Si compone di:
    * - Un ID (identificativo univoco)
    * - L'ID della stanza dove e' presente. (potrebbe variare nel corso del gioco)
    * - L'ID di un eventuale oggetto genitore. Ad es se una chiave e' contenuta in un cassetto, avra' tale ID pari all'ID del cassetto
    * - Nome dell'oggetto
    * - Descrizione dell'oggetto
    * - Attributi dell'oggetto, che gestiscono le operazioni disponibili che si possono fare sull'oggetto
    *   S = Si può salire sull’oggetto
    *   s = Si può scendere dall’oggetto
    *   i = Oggetto ispezionabile   
    *   p = Oggetto prendibile         
    *   A = Oggetto apribile e in tal caso il protagonista si sposta oltre l’oggetto in questione (senza dare il comando vai)
    *   a = Oggetto apribile e in tal caso contiene altri oggetti
    *   u = Oggetto usabile
    *   e = Oggetto equipaggiabile
    *   l = Oggetto leggibile
    *   m = Oggetto mangiabile
    *   w = Oggetto con cui il protagonista può scontrarsi.
    * - Visibilita' dell'oggetto: l'oggetto e' visibile dal giocatore?
    */
    typedef struct
    {
        int  id_oggetto;
        int  id_oggetto_genitore;
        char nome_oggetto        [LUNG_NOME_MAX];
        char descrizione_oggetto [LUNG_DESCRIZIONE_MAX];
        char attributi_oggetto   [NUM_ATTRIBUTI_OGGETTO_MAX];
        bool visibilita_oggetto; 
    } oggetto;

    /* ----- METODI DI ACCESSO ALLA STRUTTURA oggetto -----*/
    void scrivere_id_oggetto(oggetto * oggetto_gioco, int val_id);
    int leggere_id_oggetto(oggetto * oggetto_gioco);

    void scrivere_id_oggetto_genitore(oggetto * oggetto_gioco, int val_id_oggetto_genitore);
    int leggere_id_oggetto_genitore(oggetto * oggetto_gioco);

    void scrivere_nome_oggetto(oggetto * oggetto_gioco, char * val_nome_oggetto);
    char * leggere_nome_oggetto(oggetto * oggetto_gioco);

    void scrivere_descrizione_oggetto(oggetto * oggetto_gioco, char * val_descrizione_oggetto);
    char * leggere_descrizione_oggetto(oggetto * oggetto_gioco);

    void scrivere_attributi_oggetto(oggetto * oggetto_gioco, char * val_attributi_oggetto);
    char * leggere_attributi_oggetto(oggetto * oggetto_gioco);

    void scrivere_visibilita_oggetto(oggetto * oggetto_gioco, bool val_visibilita_oggetto);
    bool leggere_visibilita_oggetto(oggetto * oggetto_gioco);

    

    int leggere_tipo_oggetto(oggetto * oggetto_gioco);
#endif
