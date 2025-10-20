#ifndef FUNZIONI_REGEX_H_
    #define FUNZIONI_REGEX_H_
    #include "costanti_variabili_globali.h"
    #include <regex.h>
    
    bool applicare_regex(char testo[LUNG_STRINGA_MAX], char regex_testo[LUNG_STRINGA_MAX]);
#endif