#include "costanti_variabili_globali.h"
#include "tipo_oggetto_arma.h"

/* ----- METODI DI ACCESSO ALLA STRUTTURA tipo_oggetto_arma -----*/
void scrivere_id_arma(oggetto_arma * arma, int val_id_arma)
{
    arma->id_arma = val_id_arma;
}
int leggere_id_arma(oggetto_arma * arma)
{
    return arma->id_arma;
}

void scrivere_bonus_salute_arma(oggetto_arma * arma, int val_bonus_salute_arma)
{
    arma->bonus_salute_arma = val_bonus_salute_arma;
}
int leggere_bonus_salute_arma(oggetto_arma * arma)
{
    return arma->bonus_salute_arma;
}

void scrivere_bonus_forza_arma(oggetto_arma * arma, int val_bonus_forza_arma)
{
    arma->bonus_forza_arma = val_bonus_forza_arma;
}
int leggere_bonus_forza_arma(oggetto_arma * arma)
{
    return arma->bonus_forza_arma;
}

void scrivere_stato_equipaggiato_arma(oggetto_arma * arma, bool val_stato_equipaggiato_arma)
{
    arma->stato_equipaggiato_arma = val_stato_equipaggiato_arma;
}
bool leggere_stato_equipaggiato_arma(oggetto_arma * arma)
{
    return arma->stato_equipaggiato_arma;
}
/* ----- FINE ---------------------------------------*/
