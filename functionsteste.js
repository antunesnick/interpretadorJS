function calcula1(I, J, K) 
{ 
I=I+3; 
J=I+K; 
K=K+I+J; 
return K; 
} 
function calcula2(I, J, K) 
{ 
I=I+2; 
J=J+K; 
K=K+1; 
return K; 
} 
function executa(p1, p2, p3) 
{ 
p1 = calcula1(p3, p1, p2); 
p2 = calcula2(p2, p2, p2); 
return p1 + p2 + p3; 
}   
let A = 3; 
let B = 5; 
let C = 7; 
let res = executa(A, B, C); 
console.log("Resultado = ", res);