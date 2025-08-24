FROM frolvlad/alpine-gcc:latest

# Set working directory
WORKDIR /app

# Copy the app directory contents into the container
COPY ./ ./

CMD ["/bin/sh"]

# Compile main.c
#RUN gcc main.c funzioni_comandi.c funzioni_database.c funzioni_generali.c funzioni_interfaccia.c funzioni_regex.c mappa.c tipo_oggetto.c tipo_oggetto_arma.c tipo_oggetto_cibo.c tipo_oggetto_personaggio.c tipo_oggetto_porta.c tipo_stanza.c -o main

# Set the default command to run the compiled binary
#CMD ["./main"]

#CMD ["tail", "-f", "/dev/null"]