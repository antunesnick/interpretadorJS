function calcula(x, y) 
{ 
return x + y; 
} 
let a = 5; 
let b = 10; 
let c = 0; 
c = calcula(a, b); 
console.log(a, b, c); 
a = calcula(7, a+b+c); 
console.log("7", a+b+c, a); 
c = calcula(a*b, a/b); 
console.log(a*b, a/b, c); 
a = calcula(b, b+b); 
let msg = 'resultado = '; 
console.log(msg, a, b, c); 