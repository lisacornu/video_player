#include "ensitheora.h"
#include "synchro.h"
#include <pthread.h>

MoniteurTaille* moniteurTailleFenetre;
MoniteurTexture* moniteurTextureFenetre;


/* les variables pour la synchro, ici */


/* l'implantation des fonctions de synchro ici */
void init_moniteurs(void) {
    moniteurTailleFenetre = malloc(sizeof(MoniteurTaille));
    pthread_mutex_init(&moniteurTailleFenetre->mutex, NULL);
    pthread_cond_init(&moniteurTailleFenetre->cond, NULL);

    moniteurTextureFenetre = malloc(sizeof(MoniteurTexture));
    pthread_mutex_init(&moniteurTextureFenetre->mutex, NULL);
    pthread_cond_init(&moniteurTextureFenetre->cond, NULL);
    moniteurTextureFenetre->nb_tex = 0;
}


void envoiTailleFenetre(th_ycbcr_buffer buffer) {
    pthread_mutex_lock(&(moniteurTailleFenetre->mutex));

    windowsx = buffer[0].width;
    windowsy = buffer[0].height;

    pthread_cond_signal(&(moniteurTailleFenetre->cond));
    pthread_mutex_unlock(&(moniteurTailleFenetre->mutex));
}


void attendreTailleFenetre() {
    pthread_mutex_lock(&(moniteurTailleFenetre->mutex));
    pthread_cond_wait(&(moniteurTailleFenetre->cond), &(moniteurTailleFenetre->mutex));
    pthread_mutex_unlock(&(moniteurTailleFenetre->mutex));
}


void signalerFenetreEtTexturePrete() {
    pthread_mutex_lock(&(moniteurTextureFenetre->mutex));
    pthread_cond_signal(&(moniteurTextureFenetre->cond));
    pthread_mutex_unlock(&(moniteurTextureFenetre->mutex));
}


void attendreFenetreTexture() {
    pthread_mutex_lock(&(moniteurTextureFenetre->mutex));
    pthread_cond_wait(&(moniteurTextureFenetre->cond), &(moniteurTextureFenetre->mutex));
    pthread_mutex_unlock(&(moniteurTextureFenetre->mutex));
}


void debutConsommerTexture() {
    pthread_mutex_lock(&(moniteurTextureFenetre->mutex));
    (moniteurTextureFenetre->nb_tex)--;

    while ((moniteurTextureFenetre->nb_tex) == 0) {
        pthread_cond_wait(&(moniteurTextureFenetre->cond), &(moniteurTextureFenetre->mutex));
    }

    pthread_mutex_unlock(&(moniteurTextureFenetre->mutex));
}


void finConsommerTexture() {
    pthread_mutex_lock(&(moniteurTextureFenetre->mutex));
    pthread_cond_signal(&(moniteurTextureFenetre->cond));
    pthread_mutex_unlock(&(moniteurTextureFenetre->mutex));
}


void debutDeposerTexture() {
    pthread_mutex_lock(&(moniteurTextureFenetre->mutex));
    (moniteurTextureFenetre->nb_tex)++;
    while ((moniteurTextureFenetre->nb_tex) == NBTEX) {
        pthread_cond_wait(&(moniteurTextureFenetre->cond), &(moniteurTextureFenetre->mutex));
    }
    pthread_mutex_unlock(&(moniteurTextureFenetre->mutex));
}


void finDeposerTexture() {
    pthread_mutex_lock(&(moniteurTextureFenetre->mutex));
    pthread_cond_signal(&(moniteurTextureFenetre->cond));
    pthread_mutex_unlock(&(moniteurTextureFenetre->mutex));
}
