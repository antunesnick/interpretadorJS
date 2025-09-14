#include "tadLista.h"
#include "tadPilha.h"
#include "auxFunctions.h"

void handle_consoleLog(Lista *linhaAtual, Pilha *memoria) {
	//console.log -> ( -> "teste" ->) -> ;
	char *splitedString; // 
	NoL *aux;
	NoPilha *variavelEncontrada;
	
	aux = linhaAtual->pTokens;
	
	while(aux != NULL && strcmp(aux->token, "(") != 0 ) {
		aux = aux->prox;
	}
	aux = aux->prox;
	if(aux != NULL) {
		if(aux->token[0] == '"')  {  
			splitedString = splitString(aux->token);
			printf("%s\n", splitedString);
			free(splitedString);
		}
		else {
			variavelEncontrada = buscarVariavel(memoria, aux->token);
			
			if(variavelEncontrada != NULL) {
				if(variavelEncontrada->tipoValor == 0) {
					printf("%d\n", variavelEncontrada->valor.valorInt);
				}
				else if(variavelEncontrada->tipoValor == 1) {
					printf("%s\n", variavelEncontrada->valor.valorString);
				}
			}
			else {
				printf("Erro de referencia: '%s' não esta definida\n", aux->token);
			}		
		}
	}
}
	
void handle_let(Lista *linhaAtual, Pilha *memoria) {
	char sucesso = 0;
	 if (linhaAtual->pTokens->prox == NULL || linhaAtual->pTokens->prox->prox == NULL || linhaAtual->pTokens->prox->prox->prox == NULL) {
        printf("ERRO SINTAXE: Comando 'let' incompleto.\n");    
    } 
	else {
	    NoL *nomeVarToken = linhaAtual->pTokens->prox;
		NoL *valorVarToken = nomeVarToken->prox->prox;
		NoPilha *novaVariavel = (NoPilha*)malloc(sizeof(NoPilha));
		
		strcpy(novaVariavel->identificador, nomeVarToken->token);
	    novaVariavel->tipo = 0;
	    novaVariavel->ponteiroLinha = NULL;
	    novaVariavel->prox = NULL;
	    
	    char *valorString = valorVarToken->token;
	    
	    
	    
	    if(valorString[0] == '"') {
	    	novaVariavel->tipoValor = 1; // tipo_STRING;
	    	char *stringTratada = splitString(valorString);
	    	
	    	strcpy(novaVariavel->valor.valorString, stringTratada);
	    	free(stringTratada);
	    	printf("Debug: Var '%s' (string) declarada com valor '%s'.\n", novaVariavel->identificador, novaVariavel->valor.valorString);
	    	sucesso = 1;
	    }
		else if(isDigit(valorString[0])) {
			novaVariavel->tipoValor = 0;
			novaVariavel->valor.valorInt = atoi(valorString);
			printf("Debug: Var '%s' (int) declarada com valor %d.\n", novaVariavel->identificador, novaVariavel->valor.valorInt);
			sucesso = 1;
		}
		else {
			// Aqui entraria a lógica para let b = a;
	        // Teríamos que buscar 'a' na pilha, copiar seu tipo e valor para 'b'.
	        // Por enquanto, vamos sinalizar que não damos suporte.
	        printf("ERRO: Atribuição por variável ainda não suportada ('%s').\n", valorString);
	        free(novaVariavel); // Liberamos a memória, pois a operação falhou
		}
		if(sucesso) {
			push(memoria, novaVariavel);
		}	
	}
}
    
void handle_atribuicao(Lista *linhaAtual, Pilha *memoria) {
	NoL *nomeVarToken = linhaAtual->pTokens;
	NoL *valorToken = nomeVarToken->prox->prox;
	NoPilha *variavelEncontrada = buscarVariavel(memoria, nomeVarToken->token);
	char *valorString;
	
	if (variavelEncontrada == NULL) 
    	printf("Erro de referencia: '%s' não esta definida\n", nomeVarToken->token);
    else {
    	 valorString = valorToken->token;
    	 
    	 
    	 if(valorString[0] == '"') {
    	 	variavelEncontrada->tipoValor = 1;
    	 	strcpy(variavelEncontrada->valor.valorString, splitString(valorString));
    	 	printf("Debug: Var '%s' atualizada para (string) '%s'.\n", variavelEncontrada->identificador, variavelEncontrada->valor.valorString);
    	 }
    	 else if(isDigit(valorString[0])) {
    	 	variavelEncontrada->tipoValor = 0;
    	 	variavelEncontrada->valor.valorInt = atoi(valorString);
    	 	printf("Debug: Var '%s' atualizada para (int) %d.\n", variavelEncontrada->identificador, variavelEncontrada->valor.valorInt);
    	 }
    	 else {
    	 	//proximas
    	 }
    }
}
	

