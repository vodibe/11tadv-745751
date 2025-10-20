#include <stdio.h>
#include <string.h>
#include "funzioni_interfaccia.h"
#include "costanti_variabili_globali.h"
#include "tipo_oggetto_personaggio.h"

/* ----- METODI DI ACCESSO ALLA STRUTTURA tipo_oggetto_personaggio -----*/
void scrivere_id_personaggio(oggetto_personaggio * personaggio, int val_id)
{
    personaggio->id_personaggio = val_id;
}
int leggere_id_personaggio(oggetto_personaggio * personaggio)
{
    return personaggio->id_personaggio;
}

/* scrivere_salute_personaggio
* Questa funzione di accesso alla salute dell' oggetto_personaggio riceve in input il personaggio
* il numero di punti salute e un flag che indica se stiamo modificando i punti salute del protagonista.
* Infatti solo se stiamo modificando i punti vita del protagonista dobbiamo ripristinarli a 20 se raggiungono una quantita' <=0.
*/
void scrivere_salute_personaggio(oggetto_personaggio * personaggio, int val_salute, bool flag_protagonista)
{
    if(val_salute <= 0)
    {
        if(flag_protagonista == true)
        {
            mostrare_sconfitta();
            num_vite_corrente--;
            personaggio->salute_personaggio = SALUTE_PROTAGONISTA_INIZIALE;
        }
        else
            personaggio->salute_personaggio = 0;
    }
    else
        personaggio->salute_personaggio = val_salute;
}
int leggere_salute_personaggio(oggetto_personaggio * personaggio)
{
    return personaggio->salute_personaggio;
}

void scrivere_forza_personaggio(oggetto_personaggio * personaggio, int val_forza)
{
    if(val_forza >= FORZA_PERSONAGGIO_MAX)
        personaggio->forza_personaggio = FORZA_PERSONAGGIO_MAX;
    else
        personaggio->forza_personaggio = val_forza;

}
int leggere_forza_personaggio(oggetto_personaggio * personaggio)
{
    return personaggio->forza_personaggio;
}

void scrivere_id_oggetto_rilasciato(oggetto_personaggio * personaggio, int val_indice_oggetto, int val_id_oggetto_rilasciato)
{
    personaggio->lista_id_oggetti_rilasciati[val_indice_oggetto] = val_id_oggetto_rilasciato;
}
int leggere_id_oggetto_rilasciato(oggetto_personaggio * personaggio, int val_indice_oggetto)
{
    return personaggio->lista_id_oggetti_rilasciati[val_indice_oggetto];
}
/* ----- FINE ---------------------------------------*/

