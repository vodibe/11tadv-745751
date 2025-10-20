#ifndef TIPO_OGGETTO_ARMA_H_
    #define TIPO_OGGETTO_ARMA_H_
    
    /* Descrizione Tipo Oggetto Arma
    * Questo tipo di dato descrive un'arma del gioco che comprende i seguenti attributi:
    * - L'ID univoco dell'arma del gioco
    * - Il bonus Salute dell'arma, cioe' un numero intero che si aggiunge ai punti salute del personaggio
    * - Il bonus Forza dell'arma, cioe' un numero intero che si aggiunge ai punti forza del personaggio
    * - Un flag booleano che indica se l'arma e' correntemente equipaggiata al giocatore.
    */
    typedef struct
    {
        int  id_arma;
        int  bonus_salute_arma;
        int  bonus_forza_arma;
        bool stato_equipaggiato_arma;
    } oggetto_arma;

    /* ----- METODI DI ACCESSO ALLA STRUTTURA tipo_oggetto_arma -----*/
    void scrivere_id_arma(oggetto_arma * arma, int val_id_arma);
    int leggere_id_arma(oggetto_arma * arma);

    void scrivere_bonus_salute_arma(oggetto_arma * arma, int val_bonus_salute_arma);
    int leggere_bonus_salute_arma(oggetto_arma * arma);

    void scrivere_bonus_forza_arma(oggetto_arma * arma, int val_bonus_forza_arma);
    int leggere_bonus_forza_arma(oggetto_arma * arma);

    void scrivere_stato_equipaggiato_arma(oggetto_arma * arma, bool val_stato_equipaggiato_arma);
    bool leggere_stato_equipaggiato_arma(oggetto_arma * arma);
#endif
