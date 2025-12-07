#include <stdlib.h>
#include "tadLista.h"
#include "tadPilha.h"
#ifndef AUX_FUNCTIONS
#define AUX_FUNCTIONS
char isCharacter(char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return 1; 
    }
    return 0; 
}

char isDigit(char c) {
    char resultado = 0;
    if(c >= '0' && c <= '9') 
        resultado = 1;
    return resultado;
}

char *splitString(char string[128]) {
    char *splitedString = (char*)malloc(126 * sizeof(char));
    int i = 1;
    int j = 0;
    char delimitador = string[0];

    while (string[i] != '\0' && string[i] != delimitador && j < 125) {
        splitedString[j] = string[i];
        i = i + 1;
        j = j + 1;
    }
    splitedString[j] = '\0';
    return splitedString;
}

Lista* buscarFuncao(Programa *p, char *nome) {
    Lista *aux = p->inicio;
    NoL *primeiroToken;
    NoL *nomeFuncToken;
    char encontrou = 0;
    
    while(aux != NULL && !encontrou) {
        primeiroToken = aux->pTokens;
        if(primeiroToken != NULL && strcmp(primeiroToken->token, "function") == 0 ) {
            nomeFuncToken = primeiroToken->prox;
            if(strcmp(nomeFuncToken->token, nome) == 0) 
                encontrou = 1;
        }
        if(!encontrou)
            aux = aux->prox;
    }
    
    if(encontrou)
        return aux->prox;
        
    return NULL;
}

Lista* pularBloco(Lista *linhaPossivelDoAbre) {
    Lista *aux = linhaPossivelDoAbre;
    int nivelAninhamento = 0;
    int viuAbre = 0;
    int terminou = 0;

    while (aux != NULL && terminou == 0) {
        if (aux->pTokens != NULL) {
            NoL *tok = aux->pTokens;
            while (tok != NULL && terminou == 0) {
                if (strcmp(tok->token, "{") == 0) {
                    nivelAninhamento = nivelAninhamento + 1;
                    viuAbre = 1;
                } else if (strcmp(tok->token, "fim-func") == 0) {
                    if (nivelAninhamento > 0) {
                        nivelAninhamento = nivelAninhamento - 1;
                        if (nivelAninhamento == 0 && viuAbre == 1) {
                            terminou = 1;
                        }
                    }
                }
                tok = tok->prox;
            }
        }
        if (terminou == 0) {
            aux = aux->prox;
        }
    }

    if (aux == NULL) {
        if (linhaPossivelDoAbre != NULL) return linhaPossivelDoAbre->prox;
        return NULL;
    }
    return aux;
}

int getValorToken(NoL *token, Pilha *memoria) {
    if (isDigit(token->token[0])) {
        return atoi(token->token); 
    } else {
        NoPilha *var = buscarVariavel(memoria, token->token);
        if (var != NULL && var->tipoValor == 0) { 
            return var->valor.valorInt; 
        }
    }
    return 0; 
}

int resolve_condicao(Lista *linhaAtual, Pilha *memoria) {
    NoL *valor1_token = linhaAtual->pTokens->prox->prox;
    NoL *operador_token = valor1_token->prox;
    NoL *valor2_token = operador_token->prox;
    
    int valor1 = getValorToken(valor1_token, memoria);
    int valor2 = getValorToken(valor2_token, memoria);
    char *op = operador_token->token;

    printf("debug condicao: -> %d %s %d\n", valor1, op, valor2);

    if (strcmp(op, "==") == 0) return valor1 == valor2;
    if (strcmp(op, "!=") == 0) return valor1 != valor2;
    if (strcmp(op, ">") == 0) return valor1 > valor2;
    if (strcmp(op, "<") == 0) return valor1 < valor2;
    if (strcmp(op, ">=") == 0) return valor1 >= valor2;
    if (strcmp(op, "<=") == 0) return valor1 <= valor2;

    return 0;
}
#endif

