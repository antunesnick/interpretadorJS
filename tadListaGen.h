#ifndef TAD_LISTA_GEN_H
#define TAD_LISTA_GEN_H

#include <stdio.h>
#include <stdlib.h>
#include "tadLista.h"
#include "tadPilha.h"
#include "auxFunctions.h"

typedef union Info {
	float valor;
	char operador[3];
} Info;

typedef struct Atomo {
    int tipoAtomo;
    Info info;
} Atomo;

typedef union {
    Atomo atomo;
    struct NodoGen *sublista;
} Dado;

typedef struct NodoGen {
    int tipo;
    Dado dado;
    struct NodoGen *prox;
} NodoGen;

void liberarListaGen(NodoGen **cabeca) {
    if (!(cabeca == NULL || *cabeca == NULL)) {	
	    NodoGen* atual = *cabeca;
	    while (atual != NULL) {
	        NodoGen* proximo = atual->prox;
	        if (atual->tipo == 1) {
	            liberarListaGen(&(atual->dado.sublista));
	        }
	        free(atual);
	        atual = proximo;
	    }
	    *cabeca = NULL;
	}
}

float avaliarLista(NodoGen **cabeca) {
    if (cabeca == NULL || *cabeca == NULL) return 0;

    NodoGen *p, *ant;
    
    for (p = *cabeca; p != NULL; p = p->prox) {
        if (p->tipo == 1) {
            float sub_resultado = avaliarLista(&(p->dado.sublista));
            p->tipo = 0;
            p->dado.atomo.tipoAtomo = 1;
            p->dado.atomo.info.valor = sub_resultado;
        }
    }

    p = *cabeca;
    ant = NULL;
    while (p != NULL && p->prox != NULL) {
        NodoGen* op_node = p->prox;
        if (op_node->tipo == 0 && op_node->dado.atomo.tipoAtomo == 2 && 
           (op_node->dado.atomo.info.operador[0] == '*' || op_node->dado.atomo.info.operador[0] == '/')) 
        {
            NodoGen* val2_node = op_node->prox;
            float v1 = p->dado.atomo.info.valor;
            float v2 = val2_node->dado.atomo.info.valor;
            float res = (op_node->dado.atomo.info.operador[0] == '*') ? v1 * v2 : v1 / v2;
            
            p->dado.atomo.info.valor = res;
            p->prox = val2_node->prox;
            free(op_node);
            free(val2_node);
            p = *cabeca; 
            ant = NULL;
        } else {
            ant = p;
            p = p->prox;
        }
    }

    p = *cabeca;
    ant = NULL;
    while (p != NULL && p->prox != NULL) {
        NodoGen* op_node = p->prox;
        if (op_node->tipo == 0 && op_node->dado.atomo.tipoAtomo == 2 && 
           (op_node->dado.atomo.info.operador[0] == '+' || op_node->dado.atomo.info.operador[0] == '-')) 
        {
            NodoGen* val2_node = op_node->prox;
            float v1 = p->dado.atomo.info.valor;
            float v2 = val2_node->dado.atomo.info.valor;
            float res = (op_node->dado.atomo.info.operador[0] == '+') ? v1 + v2 : v1 - v2;
            
            p->dado.atomo.info.valor = res;
            p->prox = val2_node->prox;
            free(op_node);
            free(val2_node);
            p = *cabeca;
            ant = NULL;
        } else {
            ant = p;
            p = p->prox;
        }
    }

    return (*cabeca)->dado.atomo.info.valor;
}

NodoGen* construirLista(NoL **token, Pilha *memoria) {
    if (*token == NULL || strcmp((*token)->token, ";") == 0) {
        return NULL;
    }

    NodoGen *cabeca = NULL, *cauda = NULL;

    while (*token != NULL && strcmp((*token)->token, ")") != 0 && strcmp((*token)->token, ";") != 0) {
        NodoGen *novo = (NodoGen*)malloc(sizeof(NodoGen));
        novo->prox = NULL;

        if (strcmp((*token)->token, "(") == 0) {
            *token = (*token)->prox;
            novo->tipo = 1;
            novo->dado.sublista = construirLista(token, memoria);
        } 
        else {
            novo->tipo = 0;
            if (strchr("+-*/", (*token)->token[0]) && strlen((*token)->token) == 1) {
                novo->dado.atomo.tipoAtomo = 2;
                strcpy(novo->dado.atomo.info.operador, (*token)->token);
            } 
            else {
                novo->dado.atomo.tipoAtomo = 1;
                novo->dado.atomo.info.valor = getValorToken(*token, memoria);
            }
        }

        if (cabeca == NULL) {
            cabeca = cauda = novo;
        } else {
            cauda->prox = novo;
            cauda = novo;
        }

        if (*token != NULL) {
            *token = (*token)->prox;
        }
    }

    return cabeca;
}

int resolverExpressaoComListaGen(NoL *token, Pilha *memoria) {
    if (token == NULL) 
		return 0;

    NodoGen *cabeca = construirLista(&token, memoria);
    float resultado = avaliarLista(&cabeca);
    liberarListaGen(&cabeca);
    return (int)resultado;
}

#endif

