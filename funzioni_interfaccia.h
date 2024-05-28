#ifndef FUNZIONI_INTERFACCIA_H_
    #define FUNZIONI_INTERFACCIA_H_
    #include "mappa.h"

    /* FUNZIONI INTERFACCIA */
    void mostrare_splash();
    void mostrare_menu_gioco();
    void mostrare_guida_gioco();
    void mostrare_informazioni_gioco();

    void mostrare_sconfitta();
    void mostrare_game_over();
    void mostrare_game_win();
    void mostrare_mini_mappa(mappa * mappa_gioco, int * lista_collegamenti_stanza);
    void mostrare_intro_outro(const int ID_INTRO_OUTRO);

#endif