#ifndef TAD_LISTA_H
#define TAD_LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NoL {
	char token[128];
	struct NoL *prox;			
}NoL;


typedef struct Lista{
	struct Lista *ant;
	NoL *pTokens;	
	struct Lista *prox;
}Lista;


typedef struct pPrograma {
	Lista *inicio;
}Programa;


void initL(Programa **p) {
	*p = (Programa*)malloc(sizeof(Programa));
	(*p)->inicio = NULL;
}

NoL *novoNoL(char token[128]) {
	NoL *novoNo = (NoL*)malloc(sizeof(NoL));
	strcpy(novoNo->token, token);
	novoNo->prox = NULL;
	return novoNo;
}

Lista *novaLista() {
	Lista *novaLista = (Lista*)malloc(sizeof(Lista));
	novaLista->ant = NULL;
	novaLista->prox = NULL;
	novaLista->pTokens = NULL;
	return novaLista;
}

char isEmpty(Programa p) {
	return p.inicio == NULL;
}

void insereToken(Lista *l, NoL *token) {
	NoL *aux;
	if(l->pTokens == NULL) {
		l->pTokens = token;
	}
	else {
		aux = l->pTokens;
		while(aux->prox != NULL) {
			aux = aux->prox;
		}
		aux->prox = token;
	}
}


void insereLista(Programa **p, Lista *lista) {
	Lista *aux;
	if(isEmpty(**p)) {
		(*p)->inicio = lista;
	}
	else {
		aux = (*p)->inicio;
		while(aux->prox != NULL) {
			aux = aux->prox;
		}
		lista->ant = aux;
		aux->prox = lista;
	}
}

void liberaPrograma(Programa **p) {
	Lista *aux, *aux2;
	NoL *auxT, *auxT2;
	
	if(p != NULL && (*p) != NULL) {
		aux = (*p)->inicio;
		while(aux != NULL) {
			auxT = aux->pTokens;
			aux2 = aux->prox;
			while(auxT != NULL) {
				auxT2 = auxT->prox;
				free(auxT);
				auxT = auxT2;
			}
			free(aux);
			aux = aux2;
		}	
		free(*p);
		*p = NULL;	
	}
	
}
#endif






