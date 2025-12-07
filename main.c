#include <conio.h>
#include "handles.h"
#include "tadSaida.h"

void parser(char linhaAtual[257], Lista *l) {
    int i = 0;
    int j = 0;
    int k = 0;
    int w = 0;
    int tokenEspecialFeito = 0;

    char nome[16];
    char charToken[2];
    char subString[128];
    NoL *token;

    while (linhaAtual[i] != '\0') {
        if (linhaAtual[i] == ' ' || linhaAtual[i] == '\n' || linhaAtual[i] == '\t') {
            i = i + 1;
        }
        else if (linhaAtual[i] == '(' || linhaAtual[i] == ')' ||
                 linhaAtual[i] == '{' || linhaAtual[i] == '}' ||
                 linhaAtual[i] == '[' || linhaAtual[i] == ']' ||
                 linhaAtual[i] == ',' || linhaAtual[i] == ';' ||
                 linhaAtual[i] == '.') {

            if (linhaAtual[i] == '}') {
                token = novoNoL("fim-func");
            } else {
                charToken[0] = linhaAtual[i];
                charToken[1] = '\0';
                token = novoNoL(charToken);
            }
            insereToken(l, token);
            i = i + 1;
        }
        else if (linhaAtual[i] == '=' || linhaAtual[i] == '<' ||
                 linhaAtual[i] == '>' || linhaAtual[i] == '!' ||
                 linhaAtual[i] == '+' || linhaAtual[i] == '&' ||
                 linhaAtual[i] == '|' || linhaAtual[i] == '*' ||
                 linhaAtual[i] == '%' || linhaAtual[i] == '/') {

            subString[0] = linhaAtual[i];
            i = i + 1;

            if (subString[0] == '+') {
                if (linhaAtual[i] == '+' || linhaAtual[i] == '=') {
                    subString[1] = linhaAtual[i];
                    i = i + 1;
                    subString[2] = '\0';
                } else {
                    subString[1] = '\0';
                }
            }
            else if (subString[0] == '&') {
                if (linhaAtual[i] == '&') {
                    subString[1] = linhaAtual[i];
                    i = i + 1;
                    subString[2] = '\0';
                } else {
                    subString[1] = '\0';
                }
            }
            else if (subString[0] == '|') {
                if (linhaAtual[i] == '|') {
                    subString[1] = linhaAtual[i];
                    i = i + 1;
                    subString[2] = '\0';
                } else {
                    subString[1] = '\0';
                }
            }
            else if (subString[0] == '!') {
                if (linhaAtual[i] == '=') {
                    subString[1] = linhaAtual[i];
                    i = i + 1;
                    subString[2] = '\0';
                } else {
                    subString[1] = '\0';
                }
            }
            else if (subString[0] == '*') {
                if (linhaAtual[i] == '*') {
                    subString[1] = linhaAtual[i];
                    i = i + 1;
                    subString[2] = '\0';
                } else {
                    subString[1] = '\0';
                }
            }
            else if (subString[0] == '/' || subString[0] == '%') {
                subString[1] = '\0';
            }
            else {
                if (linhaAtual[i] == '=') {
                    subString[1] = linhaAtual[i];
                    i = i + 1;
                    subString[2] = '\0';
                } else {
                    subString[1] = '\0';
                }
            }

            token = novoNoL(subString);
            insereToken(l, token);
        }
        else if (linhaAtual[i] == '"') {
            j = 0;
            subString[j] = linhaAtual[i]; j = j + 1; i = i + 1;
            while (linhaAtual[i] != '"' && linhaAtual[i] != '\0') {
                subString[j] = linhaAtual[i]; j = j + 1; i = i + 1;
            }
            if (linhaAtual[i] == '"') {
                subString[j] = linhaAtual[i]; j = j + 1; i = i + 1;
            }
            subString[j] = '\0';
            token = novoNoL(subString);
            insereToken(l, token);
        }
        else if (linhaAtual[i] == '\'') {
            j = 0;
            subString[j] = linhaAtual[i]; j = j + 1; i = i + 1;
            while (linhaAtual[i] != '\'' && linhaAtual[i] != '\0') {
                subString[j] = linhaAtual[i]; j = j + 1; i = i + 1;
            }
            if (linhaAtual[i] == '\'') {
                subString[j] = linhaAtual[i]; j = j + 1; i = i + 1;
            }
            subString[j] = '\0';
            token = novoNoL(subString);
            insereToken(l, token);
        }
        else {
            j = 0;
            while (linhaAtual[i] != '\0' &&
                   linhaAtual[i] != ' ' && linhaAtual[i] != '\n' && linhaAtual[i] != '\t' &&
                   linhaAtual[i] != '(' && linhaAtual[i] != ')' &&
                   linhaAtual[i] != '{' && linhaAtual[i] != '}' &&
                   linhaAtual[i] != '[' && linhaAtual[i] != ']' &&
                   linhaAtual[i] != ',' && linhaAtual[i] != ';' &&
                   linhaAtual[i] != '=' && linhaAtual[i] != '<' &&
                   linhaAtual[i] != '>' && linhaAtual[i] != '!' &&
                   linhaAtual[i] != '+' && linhaAtual[i] != '&' &&
                   linhaAtual[i] != '|' && linhaAtual[i] != '*' &&
                   linhaAtual[i] != '%' && linhaAtual[i] != '/' &&
                   linhaAtual[i] != '.') {
                subString[j] = linhaAtual[i];
                j = j + 1;
                i = i + 1;
            }
            subString[j] = '\0';

            tokenEspecialFeito = 0;
            if (j > 0) {
                if (strcmp(subString, "console") == 0) {
                    k = i;
                    while (linhaAtual[k] == ' ' || linhaAtual[k] == '\t') {
                        k = k + 1;
                    }
                    if (linhaAtual[k] == '.') {
                        k = k + 1;
                        w = 0;
                        while (linhaAtual[k] != '\0' &&
                               ((linhaAtual[k] >= 'a' && linhaAtual[k] <= 'z') ||
                                (linhaAtual[k] >= 'A' && linhaAtual[k] <= 'Z'))) {
                            if (w < 15) {
                                nome[w] = linhaAtual[k];
                                w = w + 1;
                            }
                            k = k + 1;
                        }
                        if (w < 16) {
                            nome[w] = '\0';
                        } else {
                            nome[15] = '\0';
                        }

                        if (strcmp(nome, "log") == 0) {
                            token = novoNoL("console.log");
                            insereToken(l, token);
                            i = k;
                            tokenEspecialFeito = 1;
                        }
                    }
                }

                if (tokenEspecialFeito == 0) {
                    token = novoNoL(subString);
                    insereToken(l, token);
                }
            }
        }
    }
}

void Tokenizer(char nome[128], Programa **p) {
    FILE *ptr_arq = fopen(nome, "r");
    char linhaAtual[256+1];
    Lista *l;
    
    if(ptr_arq == NULL) {
        printf("ERRO AO ABRIR O ARQUIVO\n");
    }
    else {
        while(fgets(linhaAtual, sizeof(linhaAtual), ptr_arq) != NULL)
        {
            l = novaLista();
            parser(linhaAtual, l);
            
            if (l->pTokens != NULL) {
                insereLista(p, l);
            } else {
                free(l);
            }
        }
        fclose(ptr_arq);
    }
}

Lista* executarLinha(Lista *linhaAtual, Pilha *memoria, Programa *p, ListaSaida *saida) {
    if (linhaAtual == NULL || linhaAtual->pTokens == NULL) {
        return NULL;
    }

    Lista *proximaLinha = linhaAtual->prox;

    NoL *primeiroToken = linhaAtual->pTokens;
    NoL *segundoToken  = primeiroToken->prox;

    if (linhaAtual != NULL && linhaAtual->pTokens != NULL) {
        primeiroToken = linhaAtual->pTokens;
        segundoToken  = primeiroToken->prox;

        if (strcmp(primeiroToken->token, "function") == 0) {
            proximaLinha = pularBloco(linhaAtual);
        }
        else if (strcmp(primeiroToken->token, "console.log") == 0) {
            handle_consoleLog(linhaAtual, memoria, saida);
        }
        else if (strcmp(primeiroToken->token, "let") == 0) {
   		 NoL *nomeVarToken = primeiroToken->prox;
    	NoL *tokenDireita = NULL;
    	if (nomeVarToken != NULL && nomeVarToken->prox != NULL) {
        tokenDireita = nomeVarToken->prox->prox;
   		 }
    	if (tokenDireita != NULL &&
        isCharacter(tokenDireita->token[0]) &&
        tokenDireita->prox != NULL &&
        strcmp(tokenDireita->prox->token, "(") == 0) {
        proximaLinha = handle_atribuicaoFunc(linhaAtual, memoria, p);
    	} else
		{
       	handle_let(linhaAtual, memoria);
		}
		}
        else if (segundoToken != NULL && strcmp(segundoToken->token, "++") == 0) {
            handle_incremento(linhaAtual, memoria);
        }
        else if (segundoToken != NULL && strcmp(segundoToken->token, "+=") == 0) {
            handle_atribuicao(linhaAtual, memoria);
        }
        else if (segundoToken != NULL && strcmp(segundoToken->token, "=") == 0) {
            NoL *tokenAposIgual = segundoToken->prox;
            if (tokenAposIgual != NULL &&
                isCharacter(tokenAposIgual->token[0]) &&
                tokenAposIgual->prox != NULL &&
                strcmp(tokenAposIgual->prox->token, "(") == 0) {
                proximaLinha = handle_atribuicaoFunc(linhaAtual, memoria, p);
            } else {
                handle_atribuicao(linhaAtual, memoria);
            }
        }
        else if (strcmp(primeiroToken->token, "const") == 0) {
            handle_const(linhaAtual, memoria);
        }
        else if (strcmp(primeiroToken->token, "fim-func") == 0) {
            proximaLinha = handle_fimDeBloco(linhaAtual, memoria);
        }
        else if (strcmp(primeiroToken->token, "return") == 0) {
            proximaLinha = handle_return(linhaAtual, memoria);
        }
        else if (strcmp(primeiroToken->token, "if") == 0) {
            proximaLinha = handle_if(linhaAtual, memoria);
        }
        else if (strcmp(primeiroToken->token, "do") == 0) {
            proximaLinha = handle_doWhile(linhaAtual, memoria);
        }
        else if (strcmp(primeiroToken->token, "while") == 0) {
            if (memoria->topo != NULL && memoria->topo->tipo == 5) {
                proximaLinha = handle_doWhileWhile(linhaAtual, memoria);
            } else {
                proximaLinha = handle_while(linhaAtual, memoria);
            }
        }
        else if (strcmp(primeiroToken->token, "for") == 0) {
            proximaLinha = handle_for(linhaAtual, memoria);
        }
        else {
            proximaLinha = handle_funcCall(linhaAtual, memoria, p);
        }
    }

    return proximaLinha;
}


void desenha_linha_codigo(NoL* token) {
    while(token != NULL) {
        printf("%s ", token->token);
        token = token->prox;
    }
    printf("\n");
}

void desenha_tela_principal(Programa *p, Lista *linhaAtual, Lista* ultimaLinha) {
    system("cls");
    Lista* aux = p->inicio;
    printf("ARQUIVO: [nome_do_arquivo.js]\n----------------------------------------\n");
    while(aux != NULL) {
        if (aux == linhaAtual) printf(">> ");
        else if (aux == ultimaLinha) printf("-> ");
        else printf("   ");
        
        desenha_linha_codigo(aux->pTokens);
        aux = aux->prox;
    }
    printf("----------------------------------------\n");
    printf("F1-Sair | F7-Abrir | F8-Executar | F9-Memoria | F10-Tela\n");
}

void mostra_memoria(Pilha *memoria) {
    system("cls");
    printf("--- PILHA DE MEMORIA (RAM) ---\n");
    printf("IDENTIFICADOR\t\tTIPO\t\tVALOR\n");
    printf("----------------------------------------\n");
    NoPilha* aux = memoria->topo;
    while(aux != NULL) {
        printf("%s\t\t", aux->identificador);
        if (aux->tipo == 0) {
            if(aux->tipoValor == 0) printf("int\t\t%d\n", aux->valor.valorInt);
            else printf("string\t\t%s\n", aux->valor.valorString);
        } else if (aux->tipo == 1) {
             printf("Endereco\t%p\n", aux->ponteiroLinha);
        } else {
             printf("Loop\t\t%p\n", aux->ponteiroLinha);
        }
        aux = aux->prox;
    }
    printf("----------------------------------------\n");
    printf("Pressione qualquer tecla para voltar...");
    getch();
}

void mostra_tela_saida(ListaSaida *saida) {
    system("cls");
    printf("--- TELA DE SAIDA (CONSOLE.LOG) ---\n");
    printf("--------------------------------------------------\n");

    if (saida == NULL || saida->inicio == NULL) {
        printf("(Nenhuma saida registrada)\n");
    } else {
        NoSaida *aux = saida->inicio;
        while (aux != NULL) {
            printf("%s\n", aux->linha);
            aux = aux->prox;
        }
    }

    printf("--------------------------------------------------\n");
    printf("Pressione qualquer tecla para voltar...");
    getch();
}
	
int main() {
    Programa *p = NULL;
    Pilha *memoria = NULL;
    ListaSaida *tela_saida = NULL;

    Lista *linhaAtual = NULL;
    Lista *ultimaLinha = NULL;
    
    char tecla = 0;
    int rodando = 1;

    initL(&p);
    initP(&memoria);
    initSaida(&tela_saida);

    printf("Interpretador JS - Pressione F7 para abrir um arquivo.\n");

    while(rodando) {
        if (kbhit()) {
            tecla = getch();

            if (tecla == 0 || tecla == 224) {
                tecla = getch();

                switch(tecla) {
                    case 59:
                        rodando = 0;
                        break;
                    
                    case 65:
                        liberaPrograma(&p);
                        liberaPilha(&memoria);
                        liberaSaida(&tela_saida);
                        initL(&p);
                        initP(&memoria);
                        initSaida(&tela_saida);
                        
                        char nomeArquivo[100];
                        printf("Digite o nome do arquivo .js: ");
                        scanf("%s", nomeArquivo);
                        
                        Tokenizer(nomeArquivo, &p);
                        linhaAtual = p->inicio;
                        ultimaLinha = NULL;
                        desenha_tela_principal(p, linhaAtual, ultimaLinha);
                        break;

                    case 66:
                        if (linhaAtual != NULL) {
                            Lista* proxima = executarLinha(linhaAtual, memoria, p, tela_saida);
                            ultimaLinha = linhaAtual;
                            linhaAtual = proxima;
                            desenha_tela_principal(p, linhaAtual, ultimaLinha);
                        }
                        break;

                    case 67:
                        mostra_memoria(memoria);
                        desenha_tela_principal(p, linhaAtual, ultimaLinha);
                        break;

                    case 68:
                        mostra_tela_saida(tela_saida);
                        desenha_tela_principal(p, linhaAtual, ultimaLinha);
                        break;
                }
            }
        }
    }

    liberaPrograma(&p);
    liberaPilha(&memoria);
    liberaSaida(&tela_saida);
    return 0;
}

