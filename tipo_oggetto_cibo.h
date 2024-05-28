#ifndef TIPO_OGGETTO_CIBO_H_
    #define TIPO_OGGETTO_CIBO_H_
    
    /* Descrizione Tipo Oggetto Arma
    * Questo tipo di dato descrive un generico oggetto di tipo cibo del gioco.
    * - ID univoco del cibo
    * - Un bonus salute che e' un numero intero che si va ad aggiungere ai punti salute del personaggio
    */
    typedef struct
    {
        int id_cibo;
        int bonus_salute_cibo;
    } oggetto_cibo;

    /* ----- METODI DI ACCESSO ALLA STRUTTURA tipo_oggetto_cibo -----*/
    void scrivere_id_cibo(oggetto_cibo * cibo, int val_id_cibo);
    int leggere_id_cibo(oggetto_cibo * cibo);

    void scrivere_bonus_salute_cibo(oggetto_cibo * cibo, int val_bonus_salute_cibo);
    int leggere_bonus_salute_cibo(oggetto_cibo * cibo);
#endif
