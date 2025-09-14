#include "handles.h"



void parser(char linhaAtual[256 + 1], Lista *l) {
    int i = 0; 
    int j = 0;
 	
	char charToken[2];
    char subString[128];
    NoL *token;

    while (linhaAtual[i] != '\0') {
        
        if (linhaAtual[i] == ' ' || linhaAtual[i] == '\n' || linhaAtual[i] == '\t') {
            i++;
        
        } 
		else if (linhaAtual[i] == '(' || linhaAtual[i] == ')' || 
                   linhaAtual[i] == '{' || linhaAtual[i] == '}' || 
                   linhaAtual[i] == '[' || linhaAtual[i] == ']' || 
                   linhaAtual[i] == ',' || linhaAtual[i] == ';') 
		{
			if(linhaAtual[i] == '}') 
	    		token = novoNoL("fim-func");
			else 
			{
	    		charToken[0] = linhaAtual[i];
	        	charToken[1] = '\0';
	        	token = novoNoL(charToken);
	    	}
	    	insereToken(l, token);
	        
        	i++;
        } 
		else if(linhaAtual[i] == '"') {
			j = 0;
            // copia a aspa inicial
            subString[j++] = linhaAtual[i++];
            // copia tudo ate a prox aspa ou o fim da linha
            while (linhaAtual[i] != '"' && linhaAtual[i] != '\0') {
                subString[j++] = linhaAtual[i++];
            }
            // se encontrou a aspa final copia tmb
            if (linhaAtual[i] == '"') {
                subString[j++] = linhaAtual[i++];
            }
            subString[j] = '\0';
            token = novoNoL(subString);
            insereToken(l, token);
		}
		else {
            j = 0; 
            
            while (linhaAtual[i] != ' ' && linhaAtual[i] != '\n' && linhaAtual[i] != '\t' &&
                   linhaAtual[i] != '(' && linhaAtual[i] != ')' &&
                   linhaAtual[i] != '{' && linhaAtual[i] != '}' &&
                   linhaAtual[i] != '[' && linhaAtual[i] != ']' &&
                   linhaAtual[i] != ',' && linhaAtual[i] != ';' &&
                   linhaAtual[i] != '\0') {
                
                subString[j] = linhaAtual[i];
                j++;
                i++;
            }
            subString[j] = '\0';
            if (j > 0) { 
                token = novoNoL(subString);
                insereToken(l, token);
            }
        }
    }
}

void Tokenizer(char nome[128], Programa **p) {
	
	FILE *ptr_arq = fopen(nome, "r");
	char linhaAtual[256+1];
	Lista *l;
	
	if(ptr_arq == NULL) 
		printf("ERRO AO ABRIR O ARQUIVO\n");
	

	while(!feof(ptr_arq))
	{
		fgets(linhaAtual, sizeof(linhaAtual), ptr_arq);
		l = novaLista();
		parser(linhaAtual, l);
		insereLista(p, l);
	}	
}


void executarLinha(Lista **linhaAtual, Pilha *memoria) {
	
	NoL *primeiroToken;
	NoL *segundoToken;
	
	if(*linhaAtual != NULL && (*linhaAtual)->pTokens != NULL) {
		
		primeiroToken = (*linhaAtual)->pTokens;
		segundoToken = primeiroToken->prox;
		
		if(strcmp(primeiroToken->token, "console.log") == 0) {
			handle_consoleLog(*linhaAtual, memoria);
		}
		else if(strcmp(primeiroToken->token, "let") == 0) {
			handle_let(*linhaAtual, memoria);
		}
		else if(segundoToken != NULL && strcmp(segundoToken->token, "=") == 0) {
			handle_atribuicao(*linhaAtual, memoria);
		}
		
		
		
		
	}
	
	
}






int main() {
    Programa *p;
    Pilha *memoria;

    initL(&p);
    initP(&memoria); 

    Tokenizer("arquivo.js", &p);

    Lista *aux;
    aux = p->inicio;
    while(aux != NULL) {
        executarLinha(&aux, memoria); 
        aux = aux->prox;
    }
    
    liberaPrograma(&p);
    liberaPilha(&memoria);
    
    return 0;
}

