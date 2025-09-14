#include <stdlib.h>

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
	char *splitedString = (char*)malloc(126*sizeof(char));
	"teste";
	int i = 1;
	int j = 0;
	while(string[i] != '"') {
		splitedString[j] = string[i];
		i++;
		j++;
	}
	splitedString[j] = '\0';
	return splitedString;
}

