#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef union TipoValor {
	int valorInt;
	char valorString[128];
} TipoValor;


typedef struct NoPilha{
	char identificador[50];
	TipoValor valor;
	int tipo; //0 Valor 1 endereço
	int tipoValor; //0 TIPO_INT 1 TIPO_STRING
	struct Lista *ponteiroLinha;
	struct NoPilha *prox;
	
} NoPilha;

typedef struct Pilha{
	NoPilha *topo;
}Pilha;


void initP(Pilha **p) {
	(*p) = (Pilha*)malloc(sizeof(Pilha));
	(*p)->topo = NULL;
}

NoPilha *novoNoPilha(char identificador[50], TipoValor valor, int tipo, struct Lista *linha) {
    NoPilha *novo = (NoPilha*)malloc(sizeof(NoPilha));
    novo->tipo = tipo;
    novo->valor = valor; 
    novo->ponteiroLinha = NULL;
    novo->prox = NULL; 
    if (tipo == 0) { 
        strcpy(novo->identificador, identificador);
        novo->valor = valor;
    } 
	else {
        strcpy(novo->identificador, "[endereco]");
        novo->ponteiroLinha = linha;
    }
    
    return novo;
}

void push(Pilha *p, NoPilha *n) {
    if(p != NULL && n != NULL) {
        n->prox = p->topo;
        p->topo = n;
    }
}

void pop(Pilha *p, NoPilha **rem) {
    *rem = NULL;
    if(p != NULL && p->topo != NULL) {
        *rem = p->topo;
        p->topo = p->topo->prox;
        (*rem)->prox = NULL;
    }
}

void liberaPilha(Pilha **p) {
    NoPilha *aux, *prox;
    if(p != NULL && *p != NULL) {
        aux = (*p)->topo;
        while(aux != NULL) {
            prox = aux->prox;
            free(aux);
            aux = prox;
        }
        free(*p);
        *p = NULL;
    }
}

NoPilha* buscarVariavel(Pilha *p, char *nome) {
	NoPilha *aux = NULL;
	char encontrou = 0;
	
	if(p != NULL) {
		aux = p->topo;
		while(aux != NULL && !encontrou) {
			if(aux->tipo == 0 && strcmp(aux->identificador, nome) == 0)
				encontrou = 1;
			else 
				aux = aux->prox;
		}
	}
	return aux;	
}

