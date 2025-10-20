#ifndef FUNZIONI_DATABASE_H_
    #define FUNZIONI_DATABASE_H_

    void init_lista_collegamenti_stanze();
    void init_lista_oggetti_stanze();
    void init_lista_descrizioni_stanze();
    void init_lista_descrizioni_oggetti();
    
    void init_inventario(oggetto * inventario);
    void creare_protagonista(oggetto_personaggio * protagonista, oggetto * inventario);
#endif