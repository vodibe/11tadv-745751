#include "costanti_variabili_globali.h"
#include "tipo_oggetto_cibo.h"

/* ----- METODI DI ACCESSO ALLA STRUTTURA tipo_oggetto_cibo -----*/
void scrivere_id_cibo(oggetto_cibo * cibo, int val_id_cibo)
{
    cibo->id_cibo = val_id_cibo;
}
int leggere_id_cibo(oggetto_cibo * cibo)
{
    return cibo->id_cibo;
}

void scrivere_bonus_salute_cibo(oggetto_cibo * cibo, int val_bonus_salute_cibo)
{
    cibo->bonus_salute_cibo = val_bonus_salute_cibo;
}
int leggere_bonus_salute_cibo(oggetto_cibo * cibo)
{
    return cibo->bonus_salute_cibo;
}
/* ----- FINE ---------------------------------------*/
