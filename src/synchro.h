#ifndef SYNCHRO_H
#define SYNCHRO_H

#include "ensitheora.h"
#include <stdbool.h>

typedef struct MoniteurTaille {
    pthread_cond_t cond;
    pthread_mutex_t mutex;
} MoniteurTaille;

typedef struct MoniteurTexture {
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    int nb_tex;
} MoniteurTexture;

extern bool fini;

/* Les extern des variables pour la synchro ici */
extern MoniteurTaille* moniteurTailleFenetre;
extern MoniteurTexture* moniteurTextureFenetre;
extern int nb_tex;

/* Fonctions de synchro à implanter */
void init_moniteurs(void);

void envoiTailleFenetre(th_ycbcr_buffer buffer);
void attendreTailleFenetre();

void attendreFenetreTexture();
void signalerFenetreEtTexturePrete();

void debutConsommerTexture();
void finConsommerTexture();

void debutDeposerTexture();
void finDeposerTexture();

#endif
