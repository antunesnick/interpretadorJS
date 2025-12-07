console.log("--- Teste de Expressoes com Lista Generalizada ---");

let a = 5;
let b = 10;
let resultado = 0;

resultado = a + b * 2;
console.log("Resultado de a + b * 2 (esperado 25):");
console.log(resultado);

resultado = (a + b) * 2;
console.log("Resultado de (a + b) * 2 (esperado 30):");
console.log(resultado);

let temp = resultado - 5; 
resultado = resultado - temp/a; 
console.log("Resultado de 30 - 25 / 5 (esperado 25): ", resultado);
console.log(resultado);

console.log("--- Teste de Integracao com Controle de Fluxo ---");
let i = 0;
while (i < 3) {
    console.log("Loop while, i vale:");
    console.log(i);

    if (i == 1) {
        console.log("i e igual a 1, dobrando o valor de 'a'.");
        a = a * 2; 
    }

    i = i + 1; 
}

console.log("Valor final de 'a' (esperado 10):");
console.log(a);
console.log("Valor final de 'i' (esperado 3):");
console.log(i);

for(let i=0; i<10; i++) 
{    
console.log("i = ", i); // 0 1 2 3 4 5 6 7 8 9 
} 
console.log("valorfinal");
console.log(i);

console.log("--- Fim dos Testes ---");