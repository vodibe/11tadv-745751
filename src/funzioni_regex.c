#include <stdio.h>
#include <string.h>
#include "funzioni_regex.h"
/* ====== ALCUNI TEST DI MATCH PATTERN REGEX =====
* Test pattern username: https://regexr.com/5tk0v
* Test pattern comando : https://regexr.com/5tk18
*/

/* applicare_regex
* Funzione che restituisce vero se il linguaggio descritto da regex_testo compare in testo.
* Falso altrimenti
*/
bool applicare_regex(char testo[LUNG_STRINGA_MAX], char regex_testo[LUNG_STRINGA_MAX])
{
    regex_t regex_pattern;
    bool    regex_match;
    int     regex_errore;

    //1 - Controllo che l'espressione regolare sia compilata correttamente
    //     Se c'e' un errore in questa fase, allora interrompi il programma perche' e' un errore di libreria
    //
    regex_errore = regcomp(&regex_pattern, regex_testo, REG_EXTENDED);
    if(regex_errore)
    {
        printf("[X] Impossibile compilare l'espressione regolare.\n");
        esito_controllo = false;
        return false;
    }    
    //2 - Applicazione espressione regolare. 
    regex_errore = regexec(&regex_pattern, testo, 0, NULL, 0);
    if (!regex_errore)
        regex_match = true;
    else if (regex_errore == REG_NOMATCH)
        regex_match = false;
    else
    {
        printf("[X] Impossibile eseguire l'espressione regolare.\n");
        esito_controllo = false;
        return false;
    }
    //3 - De alloca la memoria assegnata all'espressione regolare dalla funzione regcomp() 
    regfree(&regex_pattern);
    return regex_match;
}
