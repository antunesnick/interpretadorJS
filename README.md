-----
# 📜 Interpretador de JavaScript em C

Este projeto consiste em um interpretador funcional para uma linguagem baseada em JavaScript, desenvolvido em C puro. O sistema implementa seu próprio analisador léxico (tokenizer), gerenciamento de memória via pilha e avaliador de expressões matemáticas utilizando listas generalizadas.

O projeto inclui uma interface visual no terminal (TUI) para depuração passo-a-passo, visualização da memória RAM (Pilha) e saída do console.


## 🚀 Funcionalidades Suportadas

O interpretador suporta um subconjunto da sintaxe do JavaScript:

  * **Variáveis e Constantes:** Declaração com `let` e `const` (tipos suportados: `int` e `string`).
  * **Estruturas de Controle:**
      * `if` / `else`
      * `while`
      * `do ... while`
      * `for` (com inicialização, condição e incremento)
  * **Funções:** Declaração de funções, passagem de parâmetros e chamadas recursivas.
  * **Expressões Matemáticas:** Avaliação de expressões com precedência e parênteses (`+`, `-`, `*`, `/`).
  * **Entrada/Saída:** Comando `console.log` para impressão.
  * **Interface de Depuração:** Visualização do código, memória e console em tempo real.

## 🏗️ Estruturas de Dados (TADs)

O núcleo do projeto baseia-se na implementação manual de Estruturas de Dados Abstratas (TADs), conforme ilustrado nos diagramas do projeto:

### 1\. TAD Lista (`tadLista.h`)

Responsável por armazenar o código fonte tokenizado.

  * **Estrutura:** Lista duplamente encadeada.
  * **Uso:** Cada nó da lista representa uma linha de código, e cada linha contém uma sub-lista de tokens.

### 2\. TAD Pilha (`tadPilha.h`)

Simula a memória RAM e a Call Stack (Pilha de Execução).

  * **Uso:** Armazena:
      * **Variáveis Locais:** `int` ou `string`.
      * **Contexto de Execução:** Endereços de retorno para controle de fluxo (loops e chamadas de função).
      * **Marcadores de Escopo:** Para saber quando limpar variáveis ao sair de um bloco `{ ... }`.

### 3\. TAD Lista Generalizada (`tadListaGen.h`)

Implementação de uma **Lista Generalizada** para avaliação de expressões matemáticas complexas.

  * **Uso:** Permite criar estruturas aninhadas para resolver expressões com múltiplos níveis de parênteses (ex: `(2 + (3 * 4))`), convertendo a expressão linear em uma estrutura hierárquica antes de calcular o resultado.

### 4\. TAD Saída (`tadSaida.h`)

Gerencia o buffer de saída do `console.log`.

## ⚙️ Como Compilar e Rodar

### Pré-requisitos

Este projeto utiliza a biblioteca `conio.h` (especificamente para `kbhit`, `getch` e cores), sendo nativo para ambientes **Windows** (MinGW, Dev-C++, Code::Blocks).

### Compilação

```bash
gcc main.c -o interpretadorJS
```

### Execução

1.  Crie um arquivo de script com a extensão `.js` (ex: `teste.js`) no mesmo diretório.
2.  Execute o programa:
    ```bash
    ./interpretadorJS
    ```
3.  No menu principal, digite o nome do arquivo para carregar.

## 🎮 Controles da Interface

O interpretador possui uma interface interativa controlada pelas teclas de função:

| Tecla | Ação | Descrição |
| :--- | :--- | :--- |
| **F7** | **Abrir** | Carrega um arquivo `.js` e faz a tokenização. |
| **F8** | **Executar** | Executa a próxima linha de instrução (Step-over). |
| **F9** | **Memória** | Exibe o estado atual da Pilha (variáveis e endereços). |
| **F10**| **Console** | Exibe a saída gerada pelos comandos `console.log`. |
| **F1** | **Sair** | Encerra o interpretador. |

## 🧩 Exemplo de Código Suportado

O interpretador é capaz de rodar algoritmos como o Fatorial:

```javascript
function fatorial(n) {
    if (n == 0) {
        return 1;
    } else {
        let resultado = n * fatorial(n - 1);
        return resultado;
    }
}

let num = 5;
let res = fatorial(num);
console.log("O fatorial de 5 e", res);
```

-----

*Projeto desenvolvido para a disciplina de Estrutura de Dados.*

-----
