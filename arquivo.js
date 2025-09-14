

console.log("--- Iniciando Teste de Variaveis ---");


let idade = 25;
console.log("O valor da variavel 'idade' e:");
console.log(idade);


let mensagem = "Teste de string funcionando!";
console.log("Agora o valor da variavel 'mensagem':");
console.log(mensagem);


console.log("Testando shadowing de variaveis:");
let valor = 100;
console.log(valor);
let valor = 200; // Declara uma nova variavel 'valor' que fica no topo da pilha
console.log(valor);


valor = 150;
console.log(valor);

idade = 18;
console.log(idade);


console.log("--- Fim do Teste ---");