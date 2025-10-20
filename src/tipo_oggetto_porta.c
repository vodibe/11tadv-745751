#include "costanti_variabili_globali.h"
#include "tipo_oggetto_porta.h"

/* ----- METODI DI ACCESSO ALLA STRUTTURA tipo_oggetto_porta -----*/
void scrivere_id_porta(oggetto_porta * porta, int val_id)
{
    porta->id_porta = val_id;
}
int leggere_id_porta(oggetto_porta * porta)
{
    return porta->id_porta;
}

void scrivere_id_stanza_partenza_porta(oggetto_porta * porta, int val_id_stanza_partenza_porta)
{
    porta->id_stanza_partenza_porta = val_id_stanza_partenza_porta;
}
int leggere_id_stanza_partenza_porta(oggetto_porta * porta)
{
    return porta->id_stanza_partenza_porta;
}

void scrivere_id_stanza_arrivo_porta(oggetto_porta * porta, int val_id_stanza_arrivo_porta)
{
    porta->id_stanza_arrivo_porta = val_id_stanza_arrivo_porta;
}
int leggere_id_stanza_arrivo_porta(oggetto_porta * porta)
{
    return porta->id_stanza_arrivo_porta;
}

void scrivere_id_chiave_porta(oggetto_porta * porta, int val_indice_chiave, int val_id_chiave_porta)
{
    porta->lista_id_chiavi_porta[val_indice_chiave] = val_id_chiave_porta;
}
int leggere_id_chiave_porta(oggetto_porta * porta, int val_indice_chiave)
{
    return porta->lista_id_chiavi_porta[val_indice_chiave];
}
int * leggere_id_chiavi_porta(oggetto_porta * porta)
{
    return porta->lista_id_chiavi_porta;
}
/* ----- FINE ---------------------------------------*/
