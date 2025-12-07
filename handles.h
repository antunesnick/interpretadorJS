#ifndef HANDLES_H
#define HANDLES_H

#include "tadLista.h"
#include "tadPilha.h"
#include "auxFunctions.h"
#include "tadSaida.h"
#include "tadListaGen.h"

Lista* handle_doWhile(Lista *linhaAtual, Pilha *memoria);
Lista* handle_doWhileWhile(Lista *linhaAtual, Pilha *memoria);
Lista* handle_for(Lista *linhaAtual, Pilha *memoria);

Lista* handle_atribuicaoFunc(Lista *linhaAtual, Pilha *memoria, Programa *p);
Lista* handle_funcCall(Lista *linhaAtual, Pilha *memoria, Programa *p);
Lista* handle_return(Lista *linhaAtual, Pilha *memoria);
void   handle_consoleLog(Lista *linhaAtual, Pilha *memoria, ListaSaida *saida);

void handle_consoleLog(Lista *linhaAtual, Pilha *memoria, ListaSaida *saida) {
    char linhaSaida[1024];
    char bufferTemporario[256];
    NoL *tokenAtual;
    NoPilha *variavel;
    int printed;

    Lista *itens[32];
    int quantidadeItens;
    Lista *listaItem;

    int nivelParen;
    int nivelColchete;
    int nivelChave;

    int erroEncontrado;
    int indice;

    linhaSaida[0] = '\0';
    printed = 0;
    quantidadeItens = 0;
    listaItem = NULL;
    nivelParen = 0;
    nivelColchete = 0;
    nivelChave = 0;
    erroEncontrado = 0;

    tokenAtual = linhaAtual->pTokens;
    while (tokenAtual != NULL && strcmp(tokenAtual->token, "(") != 0) {
        tokenAtual = tokenAtual->prox;
    }
    if (tokenAtual != NULL) {
        tokenAtual = tokenAtual->prox;
    }

    while (tokenAtual != NULL && erroEncontrado == 0) {
        if (strcmp(tokenAtual->token, ")") == 0 &&
            nivelParen == 0 && nivelColchete == 0 && nivelChave == 0) {

            if (listaItem != NULL && listaItem->pTokens != NULL && quantidadeItens < 32) {
                itens[quantidadeItens] = listaItem;
                quantidadeItens = quantidadeItens + 1;
            }
            listaItem = NULL;

            tokenAtual = NULL;
        } else {
            if (strcmp(tokenAtual->token, "(") == 0) {
                nivelParen = nivelParen + 1;
            } else if (strcmp(tokenAtual->token, ")") == 0) {
                if (nivelParen > 0) {
                    nivelParen = nivelParen - 1;
                }
            } else if (strcmp(tokenAtual->token, "[") == 0) {
                nivelColchete = nivelColchete + 1;
            } else if (strcmp(tokenAtual->token, "]") == 0) {
                if (nivelColchete > 0) {
                    nivelColchete = nivelColchete - 1;
                }
            } else if (strcmp(tokenAtual->token, "{") == 0) {
                nivelChave = nivelChave + 1;
            } else if (strcmp(tokenAtual->token, "}") == 0) {
                if (nivelChave > 0) {
                    nivelChave = nivelChave - 1;
                }
            }

            if (strcmp(tokenAtual->token, ",") == 0 &&
                nivelParen == 0 && nivelColchete == 0 && nivelChave == 0) {

                if (listaItem != NULL && listaItem->pTokens != NULL && quantidadeItens < 32) {
                    itens[quantidadeItens] = listaItem;
                    quantidadeItens = quantidadeItens + 1;
                }
                listaItem = NULL;
            } else {
                if (listaItem == NULL) {
                    listaItem = novaLista();
                }
                insereToken(listaItem, novoNoL(tokenAtual->token));
            }

            tokenAtual = tokenAtual->prox;
        }
    }

    if (listaItem != NULL && listaItem->pTokens != NULL && quantidadeItens < 32) {
        itens[quantidadeItens] = listaItem;
        quantidadeItens = quantidadeItens + 1;
        listaItem = NULL;
    }

    indice = 0;
    while (indice < quantidadeItens) {
        Lista *argumento;
        NoL *primeiroToken;
        NoL *proximoToken;
        int resultado;

        argumento = itens[indice];
        primeiroToken = NULL;
        proximoToken = NULL;

        if (argumento != NULL) {
            primeiroToken = argumento->pTokens;
        }
        if (primeiroToken != NULL) {
            proximoToken = primeiroToken->prox;
        }

        if (erroEncontrado == 0) {
            if (printed > 0) {
                strcat(linhaSaida, " ");
            }

            if (primeiroToken != NULL &&
                (primeiroToken->token[0] == '"' || primeiroToken->token[0] == '\'') &&
                proximoToken == NULL) {

                int tamanhoToken;
                int posOrigem;
                int posDestino;
                char conteudo[256];

                tamanhoToken = (int)strlen(primeiroToken->token);
                posOrigem = 1;
                posDestino = 0;
                while (posOrigem < tamanhoToken - 1 && posDestino < 255) {
                    conteudo[posDestino] = primeiroToken->token[posOrigem];
                    posDestino = posDestino + 1;
                    posOrigem = posOrigem + 1;
                }
                conteudo[posDestino] = '\0';

                strcat(linhaSaida, conteudo);
                printed = printed + 1;
            }
            else if (primeiroToken != NULL && proximoToken == NULL && isCharacter(primeiroToken->token[0])) {
                variavel = buscarVariavel(memoria, primeiroToken->token);
                if (variavel != NULL) {
                    if (variavel->tipoValor == 0) {
                        sprintf(bufferTemporario, "%d", variavel->valor.valorInt);
                        strcat(linhaSaida, bufferTemporario);
                    } else {
                        strcat(linhaSaida, variavel->valor.valorString);
                    }
                    printed = printed + 1;
                } else {
                    sprintf(bufferTemporario, "ReferenceError: %s is not defined", primeiroToken->token);
                    strcpy(linhaSaida, bufferTemporario);
                    erroEncontrado = 1;
                }
            }
            else if (argumento != NULL) {
                insereToken(argumento, novoNoL(";"));
                resultado = resolverExpressaoComListaGen(argumento->pTokens, memoria);
                sprintf(bufferTemporario, "%d", resultado);
                strcat(linhaSaida, bufferTemporario);
                printed = printed + 1;
            }
        }

        indice = indice + 1;
    }

    insereSaida(saida, linhaSaida);
}

void handle_let(Lista *linhaAtual, Pilha *memoria) {
    if (linhaAtual->pTokens == NULL ||
        linhaAtual->pTokens->prox == NULL ||
        linhaAtual->pTokens->prox->prox == NULL ||
        linhaAtual->pTokens->prox->prox->prox == NULL) {
        printf("ERRO SINTAXE: Comando 'let' incompleto.\n");
    }
    else {
        NoL *nomeVarToken = linhaAtual->pTokens->prox;
        NoL *tokenDireita = nomeVarToken->prox->prox;

        NoPilha *novaVariavel = (NoPilha*)malloc(sizeof(NoPilha));
        strcpy(novaVariavel->identificador, nomeVarToken->token);
        novaVariavel->tipo = 0;
        novaVariavel->ponteiroLinha = NULL;
        novaVariavel->prox = NULL;
        novaVariavel->is_const = 0;

        if (tokenDireita != NULL &&
            (tokenDireita->token[0] == '"' || tokenDireita->token[0] == '\'')) {

            int tamanhoToken = (int)strlen(tokenDireita->token);
            int posOrigem = 1;
            int posDestino = 0;

            novaVariavel->tipoValor = 1;
            while (posOrigem < tamanhoToken - 1 &&
                   posDestino < (int)sizeof(novaVariavel->valor.valorString) - 1) {
                novaVariavel->valor.valorString[posDestino] = tokenDireita->token[posOrigem];
                posDestino = posDestino + 1;
                posOrigem = posOrigem + 1;
            }
            novaVariavel->valor.valorString[posDestino] = '\0';

            printf("debug: var '%s' declarada com valor string '%s'.\n",
                   novaVariavel->identificador, novaVariavel->valor.valorString);
        }
        else {
            int resultado = resolverExpressaoComListaGen(tokenDireita, memoria);
            novaVariavel->tipoValor = 0;
            novaVariavel->valor.valorInt = resultado;
            printf("debug: car '%s' declarada com valor int %d.\n",
                   novaVariavel->identificador, resultado);
        }
        push(memoria, novaVariavel);
    }
}

void handle_incremento(Lista *linhaAtual, Pilha *memoria) {
    NoL *nomeVarToken = NULL;
    NoL *opToken = NULL;
    NoPilha *variavel;

    if (linhaAtual != NULL) {
        nomeVarToken = linhaAtual->pTokens;
    }
    if (nomeVarToken != NULL) {
        opToken = nomeVarToken->prox;
    }

    if (nomeVarToken == NULL || opToken == NULL || strcmp(opToken->token, "++") != 0) {
        printf("ERRO SINTAXE: incremento invalido.\n");
    } else {
        variavel = buscarVariavel(memoria, nomeVarToken->token);
        if (variavel == NULL) {
            printf("ReferenceError: '%s' nao esta definida\n", nomeVarToken->token);
        } else if (variavel->is_const) {
            printf("TypeError: tentativa de alterar const '%s'\n", variavel->identificador);
        } else if (variavel->tipoValor != 0) {
            printf("TypeError: operador '++' exige inteiro em '%s'\n", variavel->identificador);
        } else {
            variavel->valor.valorInt = variavel->valor.valorInt + 1;
            printf("Debug: Var '%s' atualizada para (int) %d.\n",
                   variavel->identificador, variavel->valor.valorInt);
        }
    }
}


Lista* handle_atribuicaoFunc(Lista *linhaAtual, Pilha *memoria, Programa *p) {
    NoL *primeiroToken = linhaAtual->pTokens;
    NoL *nomeDestinoToken = NULL;
    NoL *nomeFuncaoToken  = NULL;
    int  linhaComecaComLet = 0;

    if (primeiroToken != NULL && strcmp(primeiroToken->token, "let") == 0) {
        linhaComecaComLet = 1;
        if (primeiroToken->prox != NULL) {
            nomeDestinoToken = primeiroToken->prox;
            if (nomeDestinoToken->prox != NULL) {
                nomeFuncaoToken = nomeDestinoToken->prox->prox;
            }
        }
    } else {
        nomeDestinoToken = primeiroToken;
        if (nomeDestinoToken != NULL && nomeDestinoToken->prox != NULL) {
            nomeFuncaoToken = nomeDestinoToken->prox->prox;
        }
    }

    if (nomeDestinoToken == NULL || nomeFuncaoToken == NULL) {
        printf("ERRO: atribuicao com chamada invalida.\n");
        return linhaAtual->prox;
    }
    if (nomeFuncaoToken->prox == NULL || strcmp(nomeFuncaoToken->prox->token, "(") != 0) {
        printf("ERRO: atribuicao com chamada invalida (faltou '(').\n");
        return linhaAtual->prox;
    }

    char nomeFuncao[64];
    char nomeDestino[50];
    strcpy(nomeFuncao, nomeFuncaoToken->token);
    strcpy(nomeDestino,  nomeDestinoToken->token);

    /* se for 'let', garanta que a variável destino exista antes do salto */
    if (linhaComecaComLet) {
        NoPilha *jaExiste = buscarVariavel(memoria, nomeDestino);
        if (jaExiste == NULL) {
            NoPilha *nova = (NoPilha*)malloc(sizeof(NoPilha));
            strcpy(nova->identificador, nomeDestino);
            nova->tipo = 0;
            nova->tipoValor = 0;
            nova->valor.valorInt = 0;
            nova->is_const = 0;
            nova->ponteiroLinha = NULL;
            nova->prox = NULL;
            push(memoria, nova);
        }
    }

    Lista *enderecoPrimeiraLinhaDoCorpo = buscarFuncao(p, nomeFuncao);
    if (enderecoPrimeiraLinhaDoCorpo == NULL) {
        printf("ReferenceError: %s is not a function\n", nomeFuncao);
        return linhaAtual->prox;
    }
    if (enderecoPrimeiraLinhaDoCorpo->pTokens != NULL &&
        strcmp(enderecoPrimeiraLinhaDoCorpo->pTokens->token, "{") == 0 &&
        enderecoPrimeiraLinhaDoCorpo->prox != NULL) {
        enderecoPrimeiraLinhaDoCorpo = enderecoPrimeiraLinhaDoCorpo->prox;
    }

    /* separar argumentos reais */
    Lista *listaArgumentos[16];
    int quantidadeArgumentos = 0;
    {
        NoL *tokenCursor = nomeFuncaoToken->prox;
        int nivelParenteses = 0;
        Lista *argumentoAtual = NULL;

        while (tokenCursor != NULL && strcmp(tokenCursor->token, ";") != 0) {
            if (strcmp(tokenCursor->token, "(") == 0) {
                if (nivelParenteses == 0) argumentoAtual = novaLista();
                else insereToken(argumentoAtual, novoNoL("("));
                nivelParenteses = nivelParenteses + 1;
            } else if (strcmp(tokenCursor->token, ")") == 0) {
                nivelParenteses = nivelParenteses - 1;
                if (nivelParenteses == 0) {
                    if (quantidadeArgumentos < 16) {
                        listaArgumentos[quantidadeArgumentos] = argumentoAtual;
                        quantidadeArgumentos = quantidadeArgumentos + 1;
                    }
                    argumentoAtual = NULL;
                } else {
                    insereToken(argumentoAtual, novoNoL(")"));
                }
            } else if (strcmp(tokenCursor->token, ",") == 0 && nivelParenteses == 1) {
                if (quantidadeArgumentos < 16) {
                    listaArgumentos[quantidadeArgumentos] = argumentoAtual;
                    quantidadeArgumentos = quantidadeArgumentos + 1;
                }
                argumentoAtual = novaLista();
            } else {
                if (argumentoAtual == NULL) argumentoAtual = novaLista();
                insereToken(argumentoAtual, novoNoL(tokenCursor->token));
            }
            tokenCursor = tokenCursor->prox;
        }

        int i = 0;
        while (i < quantidadeArgumentos) {
            insereToken(listaArgumentos[i], novoNoL(";"));
            i = i + 1;
        }
    }

    /* nomes dos parâmetros formais */
    char nomesFormais[16][64];
    int  quantidadeFormais = 0;
   
        Lista *linhaCabecalhoFuncao = enderecoPrimeiraLinhaDoCorpo->ant;
        if (linhaCabecalhoFuncao != NULL && linhaCabecalhoFuncao->pTokens != NULL &&
            strcmp(linhaCabecalhoFuncao->pTokens->token, "{") == 0 &&
            linhaCabecalhoFuncao->ant != NULL) {
            linhaCabecalhoFuncao = linhaCabecalhoFuncao->ant;
        }
        if (linhaCabecalhoFuncao != NULL && linhaCabecalhoFuncao->pTokens != NULL) {
            NoL *t = linhaCabecalhoFuncao->pTokens;
            while (t != NULL && strcmp(t->token, "(") != 0) t = t->prox;
            if (t != NULL) t = t->prox;
            while (t != NULL && strcmp(t->token, ")") != 0 && quantidadeFormais < 16) {
                if (strcmp(t->token, ",") != 0) {
                    strcpy(nomesFormais[quantidadeFormais], t->token);
                    quantidadeFormais = quantidadeFormais + 1;
                }
                t = t->prox;
            }
        }
    

 
        NoPilha *marcadorRetorno = (NoPilha*)malloc(sizeof(NoPilha));
        strcpy(marcadorRetorno->identificador, nomeDestino);
        marcadorRetorno->tipo = 1;
        marcadorRetorno->tipoValor = 0;
        marcadorRetorno->is_const = 0;
        marcadorRetorno->ponteiroLinha = linhaAtual->prox;
        marcadorRetorno->prox = NULL;
        push(memoria, marcadorRetorno);
        printf("debug: chamando funcao '%s' destino '%s', pulando para o corpo e salvando retorno.\n",
               nomeFuncao, nomeDestino);


 
        int i = 0;
        while (i < quantidadeFormais && i < quantidadeArgumentos) {
            NoPilha *parametroLocal = (NoPilha*)malloc(sizeof(NoPilha));
            strcpy(parametroLocal->identificador, nomesFormais[i]);
            parametroLocal->tipo = 0;
            parametroLocal->is_const = 0;
            parametroLocal->ponteiroLinha = NULL;
            parametroLocal->prox = NULL;

            NoL *a1 = listaArgumentos[i]->pTokens;
            NoL *a2 = NULL;
            if (a1 != NULL) a2 = a1->prox;

            if (a1 != NULL &&
                (a1->token[0] == '"' || a1->token[0] == '\'') &&
                (a2 == NULL || strcmp(a2->token, ";") == 0)) {
                int tam = (int)strlen(a1->token);
                int o = 1;
                int d = 0;
                parametroLocal->tipoValor = 1;
                while (o < tam - 1 && d < (int)sizeof(parametroLocal->valor.valorString) - 1) {
                    parametroLocal->valor.valorString[d] = a1->token[o];
                    d = d + 1;
                    o = o + 1;
                }
                parametroLocal->valor.valorString[d] = '\0';
                printf("debug: param '%s' declarado string '%s'.\n",
                       parametroLocal->identificador, parametroLocal->valor.valorString);
            } else {
                int r = resolverExpressaoComListaGen(listaArgumentos[i]->pTokens, memoria);
                parametroLocal->tipoValor = 0;
                parametroLocal->valor.valorInt = r;
                printf("debug: param '%s' declarado int %d.\n",
                       parametroLocal->identificador, parametroLocal->valor.valorInt);
            }

            push(memoria, parametroLocal);
            i = i + 1;
        }

    return enderecoPrimeiraLinhaDoCorpo;
}


void handle_atribuicao(Lista *linhaAtual, Pilha *memoria) {
    NoL *nomeVarToken = linhaAtual->pTokens;
    NoL *tokenDireita = nomeVarToken->prox->prox;
    NoPilha *variavelEncontrada = buscarVariavel(memoria, nomeVarToken->token);
    int resultado;

    if (variavelEncontrada == NULL) {
        printf("ReferenceError: %s is not defined\n", nomeVarToken->token);
    }
    else if (variavelEncontrada->is_const) {
        printf("TypeError: Assignment to constant variable.\n");
    }
    else {
        resultado = resolverExpressaoComListaGen(tokenDireita, memoria);
        variavelEncontrada->tipoValor = 0;
        variavelEncontrada->valor.valorInt = resultado;
        printf("debug: Var '%s' atualizada para int %d.\n", variavelEncontrada->identificador, resultado);
    }
}

void handle_const(Lista *linhaAtual, Pilha *memoria) {
    NoL *nomeVarToken = linhaAtual->pTokens->prox;
    NoL *valorVarToken = nomeVarToken->prox->prox;
    NoPilha *novaVariavel = (NoPilha*)malloc(sizeof(NoPilha));

    strcpy(novaVariavel->identificador, nomeVarToken->token);
    novaVariavel->tipo = 0;
    novaVariavel->ponteiroLinha = NULL;
    novaVariavel->prox = NULL;

    novaVariavel->is_const = 1;
    char *valorString = valorVarToken->token;
    char sucesso = 0;

    if (valorString[0] == '"') {
        novaVariavel->tipoValor = 1;
        sucesso = 1;
    } else if (isDigit(valorString[0])) {
        novaVariavel->tipoValor = 0;
        novaVariavel->valor.valorInt = atoi(valorString);
        sucesso = 1;
    } else {
        printf("ERRO: Atribuição por variável ainda não suportada ('%s').\n", valorString);
        free(novaVariavel);
    }

    if (sucesso) {
        printf("debug: const '%s' declarada.\n", novaVariavel->identificador);
        push(memoria, novaVariavel);
    }
}



Lista* handle_funcCall(Lista *linhaAtual, Pilha *memoria, Programa *p) {
    NoL *primeiroToken = linhaAtual->pTokens;
    if (primeiroToken == NULL || !isCharacter(primeiroToken->token[0])) {
        printf("ReferenceError: chamada invalida.\n");
        if (linhaAtual != NULL) return linhaAtual->prox;
        return NULL;
    }

    char nomeFuncao[64];
    strcpy(nomeFuncao, primeiroToken->token);

    Lista *enderecoFuncao = buscarFuncao(p, nomeFuncao);
    if (enderecoFuncao == NULL) {
        printf("ReferenceError: %s is not a function\n", nomeFuncao);
        if (linhaAtual != NULL) return linhaAtual->prox;
        return NULL;
    }

    Lista *listaArgumentos[16];
    int qtdArgumentos = 0;
    {
        NoL *t = primeiroToken->prox;
        int nivelPar = 0;
        Lista *atual = NULL;

        while (t != NULL && strcmp(t->token, ";") != 0) {
            if (strcmp(t->token, "(") == 0) {
                if (nivelPar == 0) {
                    atual = novaLista();
                } else {
                    insereToken(atual, novoNoL("("));
                }
                nivelPar = nivelPar + 1;
            } else if (strcmp(t->token, ")") == 0) {
                nivelPar = nivelPar - 1;
                if (nivelPar == 0) {
                    if (qtdArgumentos < 16) {
                        listaArgumentos[qtdArgumentos] = atual;
                        qtdArgumentos = qtdArgumentos + 1;
                    }
                    atual = NULL;
                } else {
                    insereToken(atual, novoNoL(")"));
                }
            } else if (strcmp(t->token, ",") == 0 && nivelPar == 1) {
                if (qtdArgumentos < 16) {
                    listaArgumentos[qtdArgumentos] = atual;
                    qtdArgumentos = qtdArgumentos + 1;
                }
                atual = novaLista();
            } else {
                if (atual == NULL) atual = novaLista();
                insereToken(atual, novoNoL(t->token));
            }
            t = t->prox;
        }
    }

    Lista *cabecalhoFuncao = NULL;
    {
        Lista *linhaAntesDoCorpo = enderecoFuncao->ant;
        if (linhaAntesDoCorpo != NULL && linhaAntesDoCorpo->pTokens != NULL &&
            strcmp(linhaAntesDoCorpo->pTokens->token, "{") == 0) {
            if (linhaAntesDoCorpo->ant != NULL) {
                cabecalhoFuncao = linhaAntesDoCorpo->ant;
            } else {
                cabecalhoFuncao = linhaAntesDoCorpo;
            }
        } else {
            cabecalhoFuncao = enderecoFuncao->ant;
        }
    }

    {
        NoL *tk = NULL;
        if (enderecoFuncao != NULL && enderecoFuncao->pTokens != NULL) {
            tk = enderecoFuncao->pTokens;
        }
        while (tk != NULL && strcmp(tk->token, "{") != 0) {
            tk = tk->prox;
        }
        if (tk != NULL && strcmp(tk->token, "{") == 0) {
            if (enderecoFuncao->prox != NULL) {
                enderecoFuncao = enderecoFuncao->prox;
            }
        }
    }

    NoPilha *noRetorno = (NoPilha*)malloc(sizeof(NoPilha));
    strcpy(noRetorno->identificador, "RET");
    noRetorno->tipo = 1;
    noRetorno->tipoValor = 0;
    noRetorno->is_const = 0;
    noRetorno->ponteiroLinha = linhaAtual->prox;
    noRetorno->prox = NULL;
    push(memoria, noRetorno);

    printf("debug: chamando funcao '%s' pulando para o endereco e salvando retorno.\n", nomeFuncao);

    {
        char nomesFormais[16][64];
        int qtdFormais = 0;

        if (cabecalhoFuncao != NULL && cabecalhoFuncao->pTokens != NULL) {
            NoL *t = cabecalhoFuncao->pTokens->prox;
            while (t != NULL && strcmp(t->token, "(") != 0) t = t->prox;
            if (t != NULL && strcmp(t->token, "(") == 0) t = t->prox;

            while (t != NULL && strcmp(t->token, ")") != 0) {
                if (isCharacter(t->token[0])) {
                    strcpy(nomesFormais[qtdFormais], t->token);
                    qtdFormais = qtdFormais + 1;
                }
                t = t->prox;
            }
        }

        int idx = 0;
        while (idx < qtdFormais && idx < qtdArgumentos) {
            NoPilha *parametroLocal = (NoPilha*)malloc(sizeof(NoPilha));
            strcpy(parametroLocal->identificador, nomesFormais[idx]);
            parametroLocal->tipo = 0;
            parametroLocal->is_const = 0;
            parametroLocal->ponteiroLinha = NULL;
            parametroLocal->prox = NULL;

            NoL *primeiroDoArg = NULL;
            NoL *segundoDoArg = NULL;
            if (listaArgumentos[idx] != NULL) {
                primeiroDoArg = listaArgumentos[idx]->pTokens;
                if (primeiroDoArg != NULL) segundoDoArg = primeiroDoArg->prox;
            }

            if (primeiroDoArg != NULL &&
                (primeiroDoArg->token[0] == '"' || primeiroDoArg->token[0] == '\'') &&
                (segundoDoArg == NULL || strcmp(segundoDoArg->token, ";") == 0)) {

                parametroLocal->tipoValor = 1;
                {
                    int tam = (int)strlen(primeiroDoArg->token);
                    int posOri = 1;
                    int posDest = 0;
                    while (posOri < tam - 1 &&
                           posDest < (int)sizeof(parametroLocal->valor.valorString) - 1) {
                        parametroLocal->valor.valorString[posDest] = primeiroDoArg->token[posOri];
                        posDest = posDest + 1;
                        posOri   = posOri + 1;
                    }
                    parametroLocal->valor.valorString[posDest] = '\0';
                }
                printf("debug: param '%s' declarado com valor string '%s'.\n",
                       parametroLocal->identificador, parametroLocal->valor.valorString);
            } else {
                int resultadoInt = resolverExpressaoComListaGen(listaArgumentos[idx]->pTokens, memoria);
                parametroLocal->tipoValor = 0;
                parametroLocal->valor.valorInt = resultadoInt;
                printf("debug: param '%s' declarado com valor int %d\n",
                       parametroLocal->identificador, parametroLocal->valor.valorInt);
            }

            push(memoria, parametroLocal);
            idx = idx + 1;
        }
    }

    return enderecoFuncao;
}

Lista* handle_return(Lista *linhaAtual, Pilha *memoria) {
    int valorRetornado = 0;
    NoL *tokenExpressao = NULL;

    if (linhaAtual != NULL && linhaAtual->pTokens != NULL) {
        tokenExpressao = linhaAtual->pTokens->prox; 
        if (tokenExpressao != NULL && strcmp(tokenExpressao->token, ";") != 0) {
            valorRetornado = resolverExpressaoComListaGen(tokenExpressao, memoria);
        }
    }

    Lista   *enderecoDeRetorno = NULL;
    NoPilha *noRemovido = NULL;
    char     encontrou = 0;
    char     nomeDestino[50];
    nomeDestino[0] = '\0';

    while (memoria->topo != NULL && !encontrou) {
        pop(memoria, &noRemovido);
        if (noRemovido != NULL) {
            if (noRemovido->tipo == 1) {
                enderecoDeRetorno = noRemovido->ponteiroLinha;
                strcpy(nomeDestino, noRemovido->identificador); /* se veio de atribuicao, é o nome da var de destino */
                encontrou = 1;
                printf("Debug: 'return' encontrado. Voltando para a linha salva.\n");
            } else {
                printf("Debug: Removendo a variavel local '%s' da pilha.\n", noRemovido->identificador);
            }
            free(noRemovido);
            noRemovido = NULL;
        }
    }

    if (!encontrou) {
        printf("ERRO: 'return' chamado fora de uma funcao.\n");
        return NULL;
    }

    if (nomeDestino[0] != '\0') {
        NoPilha *variavelDestino = buscarVariavel(memoria, nomeDestino);
        if (variavelDestino != NULL && variavelDestino->is_const == 0) {
            variavelDestino->tipoValor = 0;
            variavelDestino->valor.valorInt = valorRetornado;
            printf("Debug: Var '%s' atualizada para (int) %d via return.\n",
                   variavelDestino->identificador, valorRetornado);
        } else if (variavelDestino == NULL) {
            printf("ReferenceError: %s is not defined\n", nomeDestino);
        } else {
            printf("TypeError: Assignment to constant variable.\n");
        }
    }

    return enderecoDeRetorno;
}



Lista* handle_if(Lista *linhaAtual, Pilha *memoria) {
    if (resolve_condicao(linhaAtual, memoria)) {
        printf("debug IF: condicao VERDADEIRA entrando no bloco.\n");
        return linhaAtual->prox;
    } else {
        printf("debug IF: Condicao FALSA pulando o bloco.\n");
        return pularBloco(linhaAtual);
    }
}

Lista* handle_fimDeBloco(Lista *linhaAtual, Pilha *memoria) {
    NoPilha *marcador;

    if (memoria->topo != NULL &&
        memoria->topo->tipo == 5 &&
        memoria->topo->ponteiroLinha == linhaAtual) {

        NoL *tokenBuscaWhile = NULL;
        if (linhaAtual->pTokens != NULL) {
            tokenBuscaWhile = linhaAtual->pTokens->prox;
            while (tokenBuscaWhile != NULL && strcmp(tokenBuscaWhile->token, "while") != 0) {
                tokenBuscaWhile = tokenBuscaWhile->prox;
            }
        }
        if (tokenBuscaWhile != NULL) {
            NoL *tokensOriginaisDaLinha = linhaAtual->pTokens;
            linhaAtual->pTokens = tokenBuscaWhile;
            Lista *proxima = handle_doWhileWhile(linhaAtual, memoria);
            linhaAtual->pTokens = tokensOriginaisDaLinha;
            return proxima;
        }

        return linhaAtual->prox;
    }

    if (memoria->topo != NULL &&
        memoria->topo->tipo == 7 &&
        memoria->topo->ponteiroLinha == linhaAtual) {

        NoPilha *marcadorFimAtual     = memoria->topo;
        NoPilha *marcadorIncremento   = NULL;
        NoPilha *marcadorInicioCabec  = NULL;

        if (marcadorFimAtual->prox != NULL && marcadorFimAtual->prox->tipo == 8) {
            marcadorIncremento = marcadorFimAtual->prox;
            if (marcadorIncremento->prox != NULL && marcadorIncremento->prox->tipo == 6) {
                marcadorInicioCabec = marcadorIncremento->prox;
            }
        }

        if (marcadorIncremento != NULL && marcadorIncremento->ponteiroLinha != NULL) {
            Lista *linhaDeIncremento = (Lista*) marcadorIncremento->ponteiroLinha;
            NoL *primeiroTokenInc = linhaDeIncremento->pTokens;
            NoL *segundoTokenInc  = NULL;
            if (primeiroTokenInc != NULL) {
                segundoTokenInc = primeiroTokenInc->prox;
            }

            printf("debug FIM_DE_BLOCO: fim FOR executando incremento e voltando cabecalho.\n");
            if (primeiroTokenInc != NULL && strcmp(primeiroTokenInc->token, "let") == 0) {
                handle_let(linhaDeIncremento, memoria);
            } else if (segundoTokenInc != NULL && strcmp(segundoTokenInc->token, "++") == 0) {
                handle_incremento(linhaDeIncremento, memoria);
            } else if (segundoTokenInc != NULL &&
                       (strcmp(segundoTokenInc->token, "=") == 0 || strcmp(segundoTokenInc->token, "+=") == 0)) {
                handle_atribuicao(linhaDeIncremento, memoria);
            }
        } else {
            printf("debug FIM_DE_BLOCO: fim do for (sem incremento) voltando cabecalho.\n");
        }

        pop(memoria, &marcador);
        free(marcador);

        if (marcadorInicioCabec != NULL) {
            return marcadorInicioCabec->ponteiroLinha;
        } else {
            printf("ERRO: Marcadores de FOR inconsistentes.\n");
            return linhaAtual->prox;
        }
    }

    if (memoria->topo != NULL &&
        memoria->topo->tipo == 3 &&
        memoria->topo->ponteiroLinha == linhaAtual) {

        pop(memoria, &marcador);
        free(marcador);

        if (memoria->topo != NULL && memoria->topo->tipo == 2) {
            printf("debug FIM_DE_BLOCO: fim de iteracao de WHILE voltando cabecalho.\n");
            return memoria->topo->ponteiroLinha;
        }
    }

    if (linhaAtual->prox != NULL &&
        linhaAtual->prox->pTokens != NULL &&
        strcmp(linhaAtual->prox->pTokens->token, "else") == 0) {

        printf("debug: fim de bloco IF verdadeiro pulando o ELSE\n");
        return pularBloco(linhaAtual->prox);
    }

    return linhaAtual->prox;
}

Lista* handle_while(Lista *linhaAtual, Pilha *memoria) {
    NoPilha *marcStart;
    NoPilha *marcEnd;
    Lista *fimBloco;

    printf("debug WHILE: testando condicao.\n");

    if (resolve_condicao(linhaAtual, memoria)) {
        if (memoria->topo == NULL ||
            !(memoria->topo->tipo == 2 && memoria->topo->ponteiroLinha == linhaAtual)) {

            marcStart = (NoPilha*) malloc(sizeof(NoPilha));
            marcStart->identificador[0] = '\0';
            marcStart->tipo = 2;
            marcStart->tipoValor = 0;
            marcStart->is_const = 1;
            marcStart->valor.valorInt = 0;
            marcStart->ponteiroLinha = linhaAtual;
            marcStart->prox = NULL;
            push(memoria, marcStart);
        }

        fimBloco = pularBloco(linhaAtual);

        if (!(memoria->topo != NULL &&
              memoria->topo->tipo == 3 &&
              memoria->topo->ponteiroLinha == fimBloco)) {

            marcEnd = (NoPilha*) malloc(sizeof(NoPilha));
            marcEnd->identificador[0] = '\0';
            marcEnd->tipo = 3;
            marcEnd->tipoValor = 0;
            marcEnd->is_const = 1;
            marcEnd->valor.valorInt = 0;
            marcEnd->ponteiroLinha = fimBloco;
            marcEnd->prox = NULL;
            push(memoria, marcEnd);
        }

        return linhaAtual->prox;
    } else {
        if (memoria->topo != NULL && memoria->topo->tipo == 3) {
            pop(memoria, &marcEnd);
            free(marcEnd);
        }
        if (memoria->topo != NULL &&
            memoria->topo->tipo == 2 &&
            memoria->topo->ponteiroLinha == linhaAtual) {
            pop(memoria, &marcStart);
            free(marcStart);
        }

        printf("debug WHILE: Condicao FALSA pulando o loop.\n");
        return pularBloco(linhaAtual);
    }
}

Lista* handle_doWhile(Lista *linhaAtual, Pilha *memoria) {
    NoPilha *marcStart;
    NoPilha *marcEnd;

    Lista *linhaDoAbre = linhaAtual;
    Lista *fimBloco;

    char encontrouAbreChaves = 0;

    if (linhaDoAbre->pTokens != NULL) {
        NoL *cursorTokens = linhaDoAbre->pTokens;
        while (cursorTokens != NULL && encontrouAbreChaves == 0) {
            if (strcmp(cursorTokens->token, "{") == 0) {
                encontrouAbreChaves = 1;
            }
            cursorTokens = cursorTokens->prox;
        }
    }

    if (encontrouAbreChaves == 0 &&
        linhaAtual->prox != NULL &&
        linhaAtual->prox->pTokens != NULL) {

        linhaDoAbre = linhaAtual->prox;

        NoL *cursorTokensProxima = linhaDoAbre->pTokens;
        while (cursorTokensProxima != NULL && encontrouAbreChaves == 0) {
            if (strcmp(cursorTokensProxima->token, "{") == 0) {
                encontrouAbreChaves = 1;
            }
            cursorTokensProxima = cursorTokensProxima->prox;
        }
    }

    if (encontrouAbreChaves == 0) {
        printf("ERRO SINTAXE: bloco do 'do' sem '{'.\n");
        return linhaAtual->prox;
    }

    marcStart = (NoPilha*) malloc(sizeof(NoPilha));
    marcStart->identificador[0] = '\0';
    marcStart->tipo = 4;
    marcStart->tipoValor = 0;
    marcStart->is_const = 1;
    marcStart->valor.valorInt = 0;
    marcStart->ponteiroLinha = linhaDoAbre;
    marcStart->prox = NULL;
    push(memoria, marcStart);

    fimBloco = pularBloco(linhaDoAbre);
    marcEnd = (NoPilha*) malloc(sizeof(NoPilha));
    marcEnd->identificador[0] = '\0';
    marcEnd->tipo = 5;
    marcEnd->tipoValor = 0;
    marcEnd->is_const = 1;
    marcEnd->valor.valorInt = 0;
    marcEnd->ponteiroLinha = fimBloco;
    marcEnd->prox = NULL;
    push(memoria, marcEnd);

    return linhaDoAbre->prox;
}

Lista* handle_doWhileWhile(Lista *linhaAtual, Pilha *memoria) {
    NoPilha *marcEnd;
    NoPilha *marcStart;

    if (memoria->topo == NULL || memoria->topo->tipo != 5) {
        return linhaAtual->prox;
    }

    marcEnd = memoria->topo;

    if (resolve_condicao(linhaAtual, memoria)) {
        if (memoria->topo->prox != NULL && memoria->topo->prox->tipo == 4) {
            marcStart = memoria->topo->prox;
            printf("debug DO_WHILE: condicao VERDADEIRA. repetindo bloco.\n");
            return marcStart->ponteiroLinha->prox;
        } else {
            printf("ERRO: Marcadores do DO-WHILE inconsistentes\n");
            return linhaAtual->prox;
        }
    } else {
        pop(memoria, &marcEnd);
        free(marcEnd);
        if (memoria->topo != NULL && memoria->topo->tipo == 4) {
            pop(memoria, &marcStart);
            free(marcStart);
        } else {
            printf("ERRO: marcador do-start ausente no final do DO-WHILE\n");
        }
        printf("debug DO_WHILE: condicao FALSA. Saindo do bloco\n");
        return linhaAtual->prox;
    }
}

Lista* handle_for(Lista *linhaAtual, Pilha *memoria) {
    NoPilha *marcStart;
    NoPilha *marcEnd;
    NoPilha *marcInc;

    Lista *linhaDoAbre = linhaAtual;
    Lista *fimBloco;

    Lista *linhaIncTmp = NULL;

    NoL *tokenAtual = linhaAtual->pTokens;

    while (tokenAtual != NULL && strcmp(tokenAtual->token, "(") != 0) {
        tokenAtual = tokenAtual->prox;
    }
    if (tokenAtual == NULL) {
        printf("ERRO SINTAXE: for sem '('.\n");
        return linhaAtual->prox;
    }
    tokenAtual = tokenAtual->prox;

    NoL *iniInicio = tokenAtual;
    NoL *iniFim    = tokenAtual;
    while (iniFim != NULL && strcmp(iniFim->token, ";") != 0) {
        iniFim = iniFim->prox;
    }
    if (iniFim == NULL) {
        printf("ERRO SINTAXE: for sem ';' no init.\n");
        return linhaAtual->prox;
    }

    NoL *condInicio = iniFim->prox;
    NoL *condFim    = condInicio;
    while (condFim != NULL &&
           strcmp(condFim->token, ";") != 0 &&
           strcmp(condFim->token, ")") != 0) {
        condFim = condFim->prox;
    }
    if (condFim == NULL) {
        printf("ERRO SINTAXE: for sem ';' ou ')' na condicao.\n");
        return linhaAtual->prox;
    }

    NoL *incInicio;
    if (strcmp(condFim->token, ";") == 0) {
        incInicio = condFim->prox;
    } else {
        incInicio = condFim;
    }
    NoL *incFim = incInicio;
    while (incFim != NULL && strcmp(incFim->token, ")") != 0) {
        incFim = incFim->prox;
    }
    if (incFim == NULL) {
        printf("ERRO SINTAXE: for sem ')'.\n");
        return linhaAtual->prox;
    }

    {
        char encontrouAbreChaves = 0;

        if (linhaDoAbre->pTokens != NULL) {
            NoL *cursorTokensLinha = linhaDoAbre->pTokens;
            while (cursorTokensLinha != NULL && encontrouAbreChaves == 0) {
                if (strcmp(cursorTokensLinha->token, "{") == 0) {
                    encontrouAbreChaves = 1;
                }
                cursorTokensLinha = cursorTokensLinha->prox;
            }
        }

        if (encontrouAbreChaves == 0 &&
            linhaAtual->prox != NULL &&
            linhaAtual->prox->pTokens != NULL) {

            linhaDoAbre = linhaAtual->prox;

            NoL *cursorTokensProxLinha = linhaDoAbre->pTokens;
            while (cursorTokensProxLinha != NULL && encontrouAbreChaves == 0) {
                if (strcmp(cursorTokensProxLinha->token, "{") == 0) {
                    encontrouAbreChaves = 1;
                }
                cursorTokensProxLinha = cursorTokensProxLinha->prox;
            }
        }

        if (encontrouAbreChaves == 0) {
            printf("ERRO SINTAXE: for sem '{'.\n");
            return linhaAtual->prox;
        }
    }

    {
        char jaEmpilhado = 0;

        if (memoria->topo != NULL &&
            memoria->topo->tipo == 8 &&
            memoria->topo->prox != NULL &&
            memoria->topo->prox->tipo == 6 &&
            memoria->topo->prox->ponteiroLinha == linhaAtual) {
            jaEmpilhado = 1;
        }

        if (jaEmpilhado == 0 &&
            memoria->topo != NULL &&
            memoria->topo->tipo == 7 &&
            memoria->topo->prox != NULL &&
            memoria->topo->prox->tipo == 8 &&
            memoria->topo->prox->prox != NULL &&
            memoria->topo->prox->prox->tipo == 6 &&
            memoria->topo->prox->prox->ponteiroLinha == linhaAtual) {
            jaEmpilhado = 1;
        }

        if (jaEmpilhado == 0) {

            if (iniInicio != iniFim) {
                Lista linhaInit;
                linhaInit.pTokens = NULL;
                linhaInit.prox = NULL;

                NoL *ultimoTokenInit = NULL;
                NoL *cursorTokensInit = iniInicio;

                while (cursorTokensInit != iniFim && cursorTokensInit != NULL) {
                    NoL *novoToken = (NoL*) malloc(sizeof(NoL));
                    strcpy(novoToken->token, cursorTokensInit->token);
                    novoToken->prox = NULL;

                    if (linhaInit.pTokens == NULL) {
                        linhaInit.pTokens = novoToken;
                    } else {
                        ultimoTokenInit->prox = novoToken;
                    }
                    ultimoTokenInit = novoToken;
                    cursorTokensInit = cursorTokensInit->prox;
                }

                NoL *primeiroTokenInit = linhaInit.pTokens;
                NoL *segundoTokenInit  = NULL;
                if (primeiroTokenInit != NULL) {
                    segundoTokenInit = primeiroTokenInit->prox;
                }

                printf("debug FOR: executando inicializacao.\n");
                if (primeiroTokenInit != NULL && strcmp(primeiroTokenInit->token, "let") == 0) {
                    handle_let(&linhaInit, memoria);
                } else if (segundoTokenInit != NULL && strcmp(segundoTokenInit->token, "++") == 0) {
                    handle_incremento(&linhaInit, memoria);
                } else if (segundoTokenInit != NULL &&
                           (strcmp(segundoTokenInit->token, "=") == 0 ||
                            strcmp(segundoTokenInit->token, "+=") == 0)) {
                    handle_atribuicao(&linhaInit, memoria);
                }

                NoL *cursorLibInit = linhaInit.pTokens;
                while (cursorLibInit != NULL) {
                    NoL *proximoLibInit = cursorLibInit->prox;
                    free(cursorLibInit);
                    cursorLibInit = proximoLibInit;
                }
            }

            marcStart = (NoPilha*) malloc(sizeof(NoPilha));
            marcStart->identificador[0] = '\0';
            marcStart->tipo = 6;
            marcStart->tipoValor = 0;
            marcStart->is_const = 1;
            marcStart->valor.valorInt = 0;
            marcStart->ponteiroLinha = linhaAtual;
            marcStart->prox = NULL;
            push(memoria, marcStart);

            if (incInicio != incFim) {
                linhaIncTmp = (Lista*) malloc(sizeof(Lista));
                linhaIncTmp->pTokens = NULL;
                linhaIncTmp->prox = NULL;

                NoL *ultimoTokenInc = NULL;
                NoL *cursorTokensInc = incInicio;

                while (cursorTokensInc != incFim && cursorTokensInc != NULL) {
                    NoL *novoToken = (NoL*) malloc(sizeof(NoL));
                    strcpy(novoToken->token, cursorTokensInc->token);
                    novoToken->prox = NULL;

                    if (linhaIncTmp->pTokens == NULL) {
                        linhaIncTmp->pTokens = novoToken;
                    } else {
                        ultimoTokenInc->prox = novoToken;
                    }
                    ultimoTokenInc = novoToken;
                    cursorTokensInc = cursorTokensInc->prox;
                }
            }

            marcInc = (NoPilha*) malloc(sizeof(NoPilha));
            marcInc->identificador[0] = '\0';
            marcInc->tipo = 8;
            marcInc->tipoValor = 0;
            marcInc->is_const = 1;
            marcInc->valor.valorInt = 0;
            marcInc->ponteiroLinha = linhaIncTmp;
            marcInc->prox = NULL;
            push(memoria, marcInc);
        }
    }

    fimBloco = pularBloco(linhaDoAbre);

    if (!(memoria->topo != NULL &&
          memoria->topo->tipo == 7 &&
          memoria->topo->ponteiroLinha == fimBloco)) {

        marcEnd = (NoPilha*) malloc(sizeof(NoPilha));
        marcEnd->identificador[0] = '\0';
        marcEnd->tipo = 7;
        marcEnd->tipoValor = 0;
        marcEnd->is_const = 1;
        marcEnd->valor.valorInt = 0;
        marcEnd->ponteiroLinha = fimBloco;
        marcEnd->prox = NULL;
        push(memoria, marcEnd);
    }

    {
        int condicaoEhVerdadeira = 1;

        if (condInicio != condFim) {
            Lista linhaCondTmp;
            linhaCondTmp.pTokens = NULL;
            linhaCondTmp.prox = NULL;

            NoL *ultimoTokenCond = NULL;

            NoL *tokWhile = (NoL*) malloc(sizeof(NoL));
            strcpy(tokWhile->token, "while");
            tokWhile->prox = NULL;
            linhaCondTmp.pTokens = tokWhile;
            ultimoTokenCond = tokWhile;

            NoL *tokAbre = (NoL*) malloc(sizeof(NoL));
            strcpy(tokAbre->token, "(");
            tokAbre->prox = NULL;
            ultimoTokenCond->prox = tokAbre;
            ultimoTokenCond = tokAbre;

            NoL *cursorTokensCond = condInicio;
            while (cursorTokensCond != condFim && cursorTokensCond != NULL) {
                NoL *novoToken = (NoL*) malloc(sizeof(NoL));
                strcpy(novoToken->token, cursorTokensCond->token);
                novoToken->prox = NULL;

                ultimoTokenCond->prox = novoToken;
                ultimoTokenCond = novoToken;
                cursorTokensCond = cursorTokensCond->prox;
            }

            NoL *tokFecha = (NoL*) malloc(sizeof(NoL));
            strcpy(tokFecha->token, ")");
            tokFecha->prox = NULL;
            ultimoTokenCond->prox = tokFecha;

            printf("debug FOR: testando condicao.\n");
            condicaoEhVerdadeira = resolve_condicao(&linhaCondTmp, memoria);

            NoL *lib = linhaCondTmp.pTokens;
            while (lib != NULL) {
                NoL *proxLib = lib->prox;
                free(lib);
                lib = proxLib;
            }
        }

        if (condicaoEhVerdadeira) {
            printf("debug FOR: condicao VERDADEIRA. Entrando no bloco.\n");
            return linhaDoAbre->prox;
        } else {
            printf("debug FOR: condicao FALSA. Pulando o bloco.\n");

            NoPilha *marcadorParaLiberar;

            if (memoria->topo != NULL && memoria->topo->tipo == 7) {
                pop(memoria, &marcadorParaLiberar);
                free(marcadorParaLiberar);
            }
            if (memoria->topo != NULL && memoria->topo->tipo == 8) {
                Lista *linhaParaLiberar = (Lista*) memoria->topo->ponteiroLinha;
                pop(memoria, &marcadorParaLiberar);

                if (linhaParaLiberar != NULL) {
                    NoL *cursorLibLinha = linhaParaLiberar->pTokens;
                    while (cursorLibLinha != NULL) {
                        NoL *proximoLibLinha = cursorLibLinha->prox;
                        free(cursorLibLinha);
                        cursorLibLinha = proximoLibLinha;
                    }
                    free(linhaParaLiberar);
                }
                free(marcadorParaLiberar);
            }
            if (memoria->topo != NULL &&
                memoria->topo->tipo == 6 &&
                memoria->topo->ponteiroLinha == linhaAtual) {
                pop(memoria, &marcadorParaLiberar);
                free(marcadorParaLiberar);
            }

            return pularBloco(linhaDoAbre)->prox;
        }
    }
}

#endif

