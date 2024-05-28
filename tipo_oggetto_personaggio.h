#ifndef TIPO_OGGETTO_PERSONAGGIO_H_
    #define TIPO_OGGETTO_PERSONAGGIO_H_
    #include "costanti_variabili_globali.h"

    /* Descrizione Tipo Oggetto Personaggio
    * Questo tipo di dato descrive un generico personaggio del gioco. I personaggi sono caratterizati da:
    * - Un ID univoco per quel personaggio
    * - La salute del personaggio che e' illimitata, cioe' non esiste un limite ai punti vita
    * - La forza del personaggio il cui valore massimo e' descritto nel modulo costanti_variabili_globali.h
    * - Un elenco di ID degli oggetti che il personaggio rilascia (cioe' rende visibili nella stanza) alla morte. Questo array e' pieno solo quando il personaggio in questione e' un nemico del protagonista
    *
    */
    typedef struct
    {
        int id_personaggio;
        int salute_personaggio;
        int forza_personaggio;
        int lista_id_oggetti_rilasciati[NUM_OGGETTI_RILASCIATI_MAX];
    } oggetto_personaggio;

    /* ----- METODI DI ACCESSO ALLA STRUTTURA tipo_oggetto_personaggio -----*/
    void scrivere_id_personaggio(oggetto_personaggio * personaggio, int val_id);
    int leggere_id_personaggio(oggetto_personaggio * personaggio);
    
    void scrivere_salute_personaggio(oggetto_personaggio * personaggio, int val_salute, bool flag_protagonista);
    int leggere_salute_personaggio(oggetto_personaggio * personaggio);

    void scrivere_forza_personaggio(oggetto_personaggio * personaggio, int val_forza);
    int leggere_forza_personaggio(oggetto_personaggio * personaggio);

    void scrivere_id_oggetto_rilasciato(oggetto_personaggio * personaggio, int val_indice_oggetto, int val_id_oggetto_rilasciato);
    int leggere_id_oggetto_rilasciato(oggetto_personaggio * personaggio, int val_indice_oggetto);

#endif
