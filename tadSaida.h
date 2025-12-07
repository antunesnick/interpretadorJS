#ifndef TAD_SAIDA_H
#define TAD_SAIDA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NoSaida {
    char linha[256];
    struct NoSaida *prox;
} NoSaida;

typedef struct ListaSaida{
    NoSaida *inicio;
    NoSaida *fim;
} ListaSaida;

void initSaida(ListaSaida **s) {
    *s = (ListaSaida*)malloc(sizeof(ListaSaida));
    (*s)->inicio = NULL;
    (*s)->fim = NULL;
}

void insereSaida(ListaSaida *s, char *texto) {
    NoSaida *novo = (NoSaida*)malloc(sizeof(NoSaida));
    strcpy(novo->linha, texto);
    novo->prox = NULL;

    if (s->inicio == NULL) {
        s->inicio = novo;
        s->fim = novo;
    } else {
        s->fim->prox = novo;
        s->fim = novo;
    }
}

void liberaSaida(ListaSaida **s) {
    if (s != NULL && *s != NULL) {
        NoSaida *aux = (*s)->inicio;
        NoSaida *prox;
        while (aux != NULL) {
            prox = aux->prox;
            free(aux);
            aux = prox;
        }
        free(*s);
        *s = NULL;
    }
}

#endif
