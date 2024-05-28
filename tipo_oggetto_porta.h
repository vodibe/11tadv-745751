#ifndef TIPO_OGGETTO_PORTA_H_
    #define TIPO_OGGETTO_PORTA_H_
    #include "costanti_variabili_globali.h"
    /* Descrizione Tipo Oggetto Porta
    *
    *
    */
    typedef struct
    {
        int id_porta;
        int id_stanza_partenza_porta;
        int id_stanza_arrivo_porta;
        int lista_id_chiavi_porta    [NUM_CHIAVI_PORTA_MAX];
    } oggetto_porta;

    /* ----- METODI DI ACCESSO ALLA STRUTTURA tipo_oggetto_porta -----*/
    void scrivere_id_porta(oggetto_porta * porta, int val_id);
    int leggere_id_porta(oggetto_porta * porta);

    void scrivere_id_stanza_partenza_porta(oggetto_porta * porta, int val_id_stanza_partenza_porta);
    int leggere_id_stanza_partenza_porta(oggetto_porta * porta);

    void scrivere_id_stanza_arrivo_porta(oggetto_porta * porta, int val_id_stanza_arrivo_porta);
    int leggere_id_stanza_arrivo_porta(oggetto_porta * porta);

    void scrivere_id_chiave_porta(oggetto_porta * porta, int val_indice_chiave, int val_id_chiave_porta);
    int leggere_id_chiave_porta(oggetto_porta * porta, int val_indice_chiave);
    int * leggere_id_chiavi_porta(oggetto_porta * porta);
#endif
