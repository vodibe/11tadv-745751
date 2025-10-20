#ifndef FUNZIONI_COMANDI_H_
    #define FUNZIONI_COMANDI_H_
    #include "costanti_variabili_globali.h"
    #include "mappa.h"

    typedef struct
    {
        int id_comando;
        char regex_verbo    [LUNG_DESCRIZIONE_MAX];
        bool argomento_oggetto;
        char regex_argomento[LUNG_DESCRIZIONE_MAX];
        bool argomento_oggetto_in_stanza;
        bool argomento_oggetto_in_inventario;
    }
    comando;

    void eseguire_parser(comando             * lista_comandi,
                     char                * verbo_comando,
                     char                * argomento_comando,
                     oggetto_personaggio * protagonista,
                     oggetto             * inventario,
                     mappa               * mappa_gioco,
                     stanza              * stanza_corrente);
    void init_comandi(comando * lista_comandi);
    void eseguire_guarda(mappa * mappa_gioco, int val_riga, int val_col);

    oggetto_arma * ricercare_arma_equipaggiata(mappa * mappa_gioco, oggetto * inventario);
    

#endif