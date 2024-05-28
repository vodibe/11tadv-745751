#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "costanti_variabili_globali.h"
#include "funzioni_comandi.h"
#include "funzioni_database.h"
#include "funzioni_generali.h"
#include "funzioni_interfaccia.h"
#include "funzioni_regex.h"
#include "mappa.h"

void start_game(bool nuova_partita);
bool exit_game ();
void init_dati (mappa * mappa_gioco, comando * lista_comandi);

int main(void)
{
    int scelta_menu;

    mostrare_splash();
    printf("\n[*] Benvenuto al gioco, inserisci il tuo nome.\n    Caratteri massimi: %d. Sono ammesse solo lettere e/o numeri.\n", LUNG_USERNAME_MAX);

    /* leggo il nome del giocatore. Se non e' un nome valido lo ri-leggo. Se e' valido esco dal ciclo.*/
    while(!leggere_nome_giocatore(nome_giocatore)) ;
    
    scelta_menu = -1;
    while(!(scelta_menu >= 1 && scelta_menu <= 5))
    {
        system("clear");
        mostrare_menu_gioco();
        if(leggere_intero(&scelta_menu) != 1)
            scelta_menu = -1;
            
        if     (scelta_menu == 1)    start_game(true);
        else if(scelta_menu == 2)    start_game(false);
        else if(scelta_menu == 3)    mostrare_guida_gioco();
        else if(scelta_menu == 4)    mostrare_informazioni_gioco();
    }
    return EXIT_SUCCESS;
}

/* start_game
* Questa funzione riceve in input un flag che indica se bisogna avviare una nuova partita 
* o caricarne una esistente dalla cartella slot.
* Inizializza le variabili globali e crea le strutture dati del gioco.
*/
void start_game(bool nuova_partita)
{
    /*dichiarando mappa_gioco e protagonista come variabili static facciamo in modo che
    * questi identificatori esistano in memoria dal momento in cui il programma inizia
    * fino a che termina.
    */
    static mappa               mappa_gioco;
    static oggetto_personaggio protagonista;
    oggetto                    inventario   [NUM_OGGETTI_INVENTARIO_MAX];

    comando lista_comandi[NUM_COMANDI_MAX];
    char comando[LUNG_STRINGA_MAX];

    /* Inizializzazione variabili globali di gioco */
    game_win                      = false;
    esito_controllo               = true;
    id_ultimo_oggetto_ispezionato = -1;
    num_vite_corrente             = NUM_VITE_PROTAGONISTA_INIZIALE;

    /* Caricamento di protagonista, inventario, mappa */
    init_dati(&mappa_gioco, lista_comandi);
    
    if(nuova_partita == true)
    {
        creare_protagonista(&protagonista, inventario);
        aggiornare_id_stanza_corrente(&mappa_gioco, ID_STANZA_PROTAGONISTA_INIZIALE);
        nascondere_oggetti_rilasciati(&mappa_gioco);

        system("clear");
        mostrare_intro_outro(1);
        printf("\n[i] Premi INVIO per iniziare l'avventura! \n");
        fgets(buffer, sizeof(buffer), stdin);
    }
    else
    {
        caricare_partita(&protagonista, inventario, &mappa_gioco);
    }
    // Dopo aver creato/caricato le strutture dati (della mappa, personaggio) controllo che non si sia verificato un errore con i file. 
    if(exit_game() == true)
        return;

    // Se invece non si e' verificato nessun errore inizio col gioco vero e proprio.
    eseguire_guarda(&mappa_gioco, riga_stanza_corrente, col_stanza_corrente);
    id_stanza_precedente = -1;
    while(exit_game() == false)
    {
        printf("\n[>] ");
        fgets(buffer, sizeof(buffer), stdin);
        // Poiche' il buffer ha una dimensione piu' grande della stringa del comando, copio
        // i primi LUNG_STRINGA_MAX byte del buffer in comando.
        strncpy(comando, buffer, sizeof(comando));

        /* Faccio l'analisi lessicale del comando. Per funzionare, lo scanner si serve
        * del comando e degli altri parametri, che verranno passati per indirizzo al parser.
        * Il parser sara' chiamato 'a cascata' dopo lo scanner. Avremmo potuto chiamare direttamente
        * qui il parser (dopo la funzione eseguire_scanner), pero' si e' progettato il parser in modo
        * che ricave in input due dati (il verbo e l'argomento). Per questo, piuttosto che creare
        * un vettore di 2 stringhe e passarlo al parser, si chiama il parser in cascata nella funzione
        * dello scanner.
        */
        eseguire_scanner(lista_comandi,
                         comando,
                         &protagonista,
                         inventario,
                         &mappa_gioco,
                         leggere_stanza_da_mappa(&mappa_gioco, riga_stanza_corrente, col_stanza_corrente));
    }
}

/* init_dati
* Funzione che serve a caricare tutte le strutture dati del gioco.
* Ogni volta che si fa un'operazione di caricamento, dopo averla completata 
* non si passa subito all'altra ma si fa un controllo sulla variabile globale
* esito_controllo. Se esito_controllo = false ovviamente significa che c'e' un errore
* con i file e quindi non le altre operazioni di caricamento delle strutture dati non
* si eseguono, ma si termina il programma. Questo e' fatto mediante un ciclo while con contatore.
*/
void init_dati(mappa * mappa_gioco, comando * lista_comandi)
{
    int i_op, i_op_max;
    i_op     = 0;
    i_op_max = 10;
    while(i_op <= i_op_max && exit_game() == false)
    {
        /* Creazione file binari partendo dai relativi file di testo */
        if     (i_op==0) init_lista_collegamenti_stanze();
        else if(i_op==1) init_lista_oggetti_stanze     (); 
        else if(i_op==2) init_lista_descrizioni_stanze ();
        else if(i_op==3) init_lista_descrizioni_oggetti();
        else if(i_op==4) init_comandi                  (lista_comandi);

        /* Caricamento delle strutture dati */
        else if(i_op==5) creare_mappa                  ("data/dimensione_mappa.txt",         mappa_gioco); 
        else if(i_op==6) aggiungere_stanze_in_mappa    (                                     mappa_gioco);
        else if(i_op==7) aggiungere_porte_in_mappa     ("data/lista_oggetti_porte.txt",      mappa_gioco);
        else if(i_op==8) aggiungere_cibi_in_mappa      ("data/lista_oggetti_cibo.txt" ,      mappa_gioco);
        else if(i_op==9) aggiungere_armi_in_mappa      ("data/lista_oggetti_armi.txt" ,      mappa_gioco);
        else if(i_op==10)aggiungere_personaggi_in_mappa("data/lista_oggetti_personaggi.txt", mappa_gioco);
        i_op++;
    }
}

/* exit_game
* Funzione che controlla le variabili in base alle quali terminare il programma.
*/
bool exit_game()
{
    bool exit_game;

    exit_game = false;
    if     (num_vite_corrente < 0)
    {
        // Vale la vita zero. Se non vale mettere num_vite_corrente <= 0
        system("clear");
        mostrare_game_over();
        exit_game = true;
    }
    else if(esito_controllo == false)
    {
        // Se e' successo un errore con i file, termina il gioco.
        exit_game = true;
    }
    else if(game_win == true)
    {
        system("clear");
        mostrare_intro_outro(2);    //mostriamo la fine della storia
        mostrare_game_win();
        exit_game = true;
    }
    return exit_game;
}