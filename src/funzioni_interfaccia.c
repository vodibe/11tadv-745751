#include <stdio.h>
#include <stdlib.h>
#include "costanti_variabili_globali.h"
#include "funzioni_interfaccia.h"

#define ID_STANZA_BIBLIOTECA               10

void mostrare_splash()
{
    printf("  ____ ____  __              .___"      
    "\n /_   /_   |/  |______     __| _/__  __"
    "\n  |   ||   \\   __\\__  \\   / __ |\\  \\/ /"
    "\n  |   ||   ||  |  / __ \\_/ /_/ | \\   /" 
    "\n  |___||___||__| (____  /\\____ |  \\_/\n");
}

void mostrare_menu_gioco()
{
    printf ("\n  ___.____________________________________________________\r\n"
				" (__/ \\__________[MENU PRINCIPALE]________________________)\r\n" 
				"    |||                                                |\r\n" 
				"    |||   [1] Nuova partita    [3] Guida del gioco     |\r\n" 
				"    |||   [2] Carica partita   [4] Informazioni        |\r\n" 
				"    |||                        [5] Esci (anche CTRL+C) |\r\n" 
				"  ._|/|_.     _________________________________________|\r\n" 
				"  |_ | _|    /\r\n"      
				"    |:|_____/____.....:::::[%s]:::::.....\n"
				"    \\_/                                               \r\n\n"
                "[i] ",
                nome_giocatore);
}

void mostrare_guida_gioco()
{
    FILE * file_guida_gioco;
    char c;

    file_guida_gioco = fopen("../data/guida_gioco.txt", "r");
    if(file_guida_gioco == NULL)
    {
        printf("[X] Impossibile visualizzare la guida ora.\n");
        return;
    }

    system("clear");
    id_stanza_precedente=-1;
    
    c = fgetc(file_guida_gioco);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(file_guida_gioco);
    }
    fclose(file_guida_gioco);
}

void mostrare_informazioni_gioco()
{
    printf("    ______________________________________________________\r\n" 
				"   )                                                    (\r\n" 
				"  (   - 11TADV  e' un' avventura testuale scritta        )\r\n" 
				"   )   in linguaggio C per l'esame di Laboratorio di    (\r\n"
				"  (    Informatica del CdL 'Informatica' dell' UNIBA.    )\r\n" 
				"   )                                                    (\r\n" 
				"  (   - Realizzato da A. Bellifemine, V. Di Bisceglie,   )\r\n" 
				"   )   A. Fanizzi, A. Lomuscio                          (\r\n" 
				"  (                                                      )\r\n" 
				"  )   - Realizzato a maggio 2021                        (\r\n" 
				"  (________       _       _       _       _       _______)\r\n" 
				"            `-._.-' (___ _) (__ _ ) (_   _) (__  _) \r\n"
				"                    `-._.-' (  ___) ( _  _) ( _ __) \r\n"
				"                            `-._.-' (__  _) (__  _) \r\n" 
				"                                            `-._.-'\n");

}

void mostrare_sconfitta()
{
    printf("      __        __\n"      
        "    /~ .~\\    /~  ~\\ ____...___ ., . . ______ \n" 
        "   '      `\\/'      *\n"  
        "  (                 .*)  Hai perso 1 vita!\n"
        "   \\       > <  . *./   [+] Salute:  %d punti\n"
        "    `\\ .      . .*/'\n"    
        "      `\\ * .*. */'___....._._....,..._.._____\n "
        "       `\\ * */'\n"      
        "          `\\/'  ", SALUTE_PROTAGONISTA_INIZIALE);
}

void mostrare_game_over()
{
    printf("\n\n"
		     " .____.   ___  .__  __.._____.    ___    ._. ._._____.____. \r\n" 
			"  |  __ \\ / _ \\ |  \\/  ||  ___|  ,'   Y`. | | | |  ___| ___ \\\r\n"
			"  | |  \\// /_\\ \\| .  . || |__   /        \\| | | | |__ | |_/ /\r\n" 
			"  | | __ |  _  || |\\/| ||  __|  \\ ()  () /| | | |  __||    / \r\n" 
			"  | |_\\ \\| | | || |  | || |___   '. /\\ .' \\ \\_/ / |___| |\\ \\ \r\n" 
			"   \\____/\\_| |_/\\_|  |_/\\____/  8=\\||||/=8 \\___/\\____/\\_| \\_|");
}

void mostrare_game_win()
{
    printf("\n\n                         .-.\r\n" 
				"  ._. ._.  ___ ._____.   | | / )_____._  ._._____._____._.\r\n"
				"  | | | | / _ \\|_   _|   | |/ /|_   _| \\ | |_   _|  _  | |\r\n" 
				"  | |_| |/ /_\\ \\ | |    _|__ /_  | | |  \\| | | | | | | | |\r\n" 
				"  |  _  ||  _  | | |   / __)-' ) | | | . ` | | | | | | | |\r\n"
				"  | | | || | | |_| |_  \\  `(.-')_| |_| |\\  | | | \\ \\_/ /_|\r\n" 
				"  \\_| |_/\\_| |_/\\___/   > ._>-' \\___/\\_| \\_/ \\_/  \\___/(_)");
}

/* mostrare_mini_mappa
* Questa funzione mostra una mini mappa che indica le direzioni possibili correnti.
* Esempio per capire il funzionamento:
* Stanza 12: stanze collegate: 7 (nord) - 13 (est) - 17 (sud) - 120 (ovest)
* Dalla stanza 12 possiamo andare:
* - a nord (in tal caso spostandoci nella stanza 12-5=7 che e' direttamente collegata)
* - a est (in tal caso spostandoci nella stanza 12+1=13 che e' direttamente collegata)
* - a sud (in tal caso spostandoci nella stanza 12+5=17 che e' direttamente collegata)
* - a ovest (in tal caso spostandoci nella stanza 12-1=11)
*   Ma la stanza 11 non e' direttamente collegata con la stanza 12.
*/
void mostrare_mini_mappa(mappa * mappa_gioco, int * lista_collegamenti_stanza)
{
    int id_stanza_nord, id_stanza_est, id_stanza_sud, id_stanza_ovest;
    id_stanza_nord = *(lista_collegamenti_stanza+0);
    id_stanza_est  = *(lista_collegamenti_stanza+1);
    id_stanza_sud  = *(lista_collegamenti_stanza+2);
    id_stanza_ovest= *(lista_collegamenti_stanza+3);
    printf("________________________\n"
           "____[ MINI MAPPA ]______)\n");

    //Poiche' la biblioteca e' collegata con un passaggio segreto, non lo mostriamo in mappa.
    if( id_stanza_corrente_protagonista != ID_STANZA_BIBLIOTECA &&
       id_stanza_nord != -1 &&
       id_stanza_nord == id_stanza_corrente_protagonista-leggere_col(mappa_gioco))
        printf("                     |\n"
        "         A           |\n"
        "        /_\\          |\n"
        "        _!_          |\n");
    else
        printf("                     |\n"
        "                     |\n"
        "                     |\n"
        "        ___          |\n");

    if(id_stanza_est != -1 && id_stanza_ovest != -1 &&
       id_stanza_est == id_stanza_corrente_protagonista+1 &&
       id_stanza_ovest == id_stanza_corrente_protagonista-1)
        printf("    .  [   ]  .     <\n"
        "   /|__| x |__|\\     |\n"
        "   \\|  [___]  |/     |\n"); 
    else if(id_stanza_est != -1 &&
           (id_stanza_ovest == -1 || id_stanza_ovest != id_stanza_corrente_protagonista-1 ) && 
            id_stanza_est == id_stanza_corrente_protagonista+1)
        printf("       [   ]  .     <\n"
        "       | x |__|\\     |\n"
        "       [___]  |/     |\n");
    else if(  (id_stanza_est == -1 || id_stanza_est != id_stanza_corrente_protagonista+1 ) && id_stanza_ovest != -1 &&
    id_stanza_ovest == id_stanza_corrente_protagonista-1)
        printf("    .  [   ]        <\n"
        "   /|__| x |         |\n"
        "   \\|  [___]         |\n");
    
    else
        printf("    .  [   ]  .     <\n"
        "       | x |         |\n"
        "       [___]         |\n");

    if(id_stanza_sud != -1 &&
    id_stanza_sud == id_stanza_corrente_protagonista+leggere_col(mappa_gioco))
        printf("    '   _!_   '     <\n"
        "		\\ /          |\n"
        "         v           |\n"
        "_____________________|\n");
    else
        printf("    '         '     <\n"
        "		             |\n"
        "                     |\n"
        "_____________________|\n");

}

void mostrare_intro_outro(const int ID_INTRO_OUTRO)
{
    //ID_INTRO_OUTRO = 1 -> mostrare intro
    //ID_INTRO_OUTRO = 2 -> mostrare outro
    FILE * file_intro_outro_gioco;
    char c;
    int temp_id;
    bool fine_ricerca;
    file_intro_outro_gioco = fopen("../data/intro_outro_gioco.txt", "r");
    if(file_intro_outro_gioco == NULL)
    {
        return;
    }
    fine_ricerca = false;
    while(!feof(file_intro_outro_gioco) && fine_ricerca == false)
    {
        fscanf(file_intro_outro_gioco, "%d", &temp_id);
        c = ' ';
        while (c != '&')
        {
            if(temp_id == ID_INTRO_OUTRO)
                printf ("%c", c);
            c = fgetc(file_intro_outro_gioco);
        }
        if(temp_id == ID_INTRO_OUTRO)
            fine_ricerca = true;
    }
    fclose(file_intro_outro_gioco);
}
