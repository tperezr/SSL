#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

int verificarCaracteres(char *s);
int isLetterHex(char c);
int esPalabra(const char *string);
int columna(int c, bool isHex, bool isDec, bool isOctal, bool signEnabled);
void countDec(bool *isDec, int *counterDec);
void countHex(bool *isHex, int *counterHex);
void countOctal(bool *isOctal, int *counterOctal);

int main()
{
    char s1[] = "+125&154&-1";
	if (!verificarCaracteres(s1)) 
	{
		printf("Caracteres invalidos\n");
		return 0;
	}
	if(esPalabra(s1)){
		printf("Pertenece al lenguaje\n");
		return 0;
	}
	printf("no pertenece al lenguaje\n"); 
	return 0;
}

int verificarCaracteres (char *s) {
	unsigned i;
	for (i=0; s[i]; i++)
	{
		if (!(isdigit(s[i]) 
            || s[i] == '+' 
			|| s[i] == '-'
            || isLetterHex(s[i])
			|| s[i] == '&'
			|| s[i] == 'x'
			|| s[i] == 'X')){
				return 0;
			}
		
	}
return 1;
}
/*Verifico si contiene caracteres hexadecimales, unicamente las letras*/
int isLetterHex(char c){
    c = toupper(c);
    if(!(c == 'A' || c == 'B'
        || c == 'C' || c == 'D'
        || c == 'E' || c == 'F')){
            return 0;
        }

    return 1;
}

void countDec(bool *isDec, int *counterDec){
	if(*isDec){
		*counterDec += 1;
	}
	*isDec = false;
}
void countHex(bool *isHex, int *counterHex){
	if(*isHex){
		*counterHex += 1;
	}
	*isHex = false;
}
void countOctal(bool *isOctal, int *counterOctal){
	if(*isOctal){
		*counterOctal += 1;
	}
	*isOctal = false;
}

int columna(int c, bool isHex, bool isDec, bool isOctal, bool signEnabled){
	if(isHex && ((c >= '0' && c <= '9') || isLetterHex(c))) {
		return 7;
	} else if (isDec && (c >= '0' && c <= '9')) {
		return 2;
	} else if(isOctal && (c >= '0' && c <= '7')){
		return 4;
	} else if(signEnabled && (c >= '1' && c <= '9')){ //decimal sin 0
		return 1;
	} else if(c >= '1' && c <= '7'){
		return 3;
	}
	
	switch (c){
		case '+':
			return 0;
		case '-':
			return 0;
		case '0':
			return 5;
		case 'x':
			return 6;
		case 'X':
			return 6;
		case '&':
			return 8;
		default:			
			return 9;
	}
}

/* Automata 1 */
int esPalabra(const char *string){
	static int tt[7][10] = {{1,6,6,3,6,4,6,6,6,6},
							{6,2,6,6,6,6,6,6,6,6},
							{6,6,2,6,6,6,6,6,0,6},
							{6,6,6,6,3,6,6,6,0,6},
							{6,6,6,6,6,6,5,6,6,6},
							{6,6,6,6,6,6,6,5,0,6},
							{6,6,6,6,6,6,6,6,6,6}};
	int e = 0; //estado inicial
	bool isHex = false; 
	bool isDec = false;
	bool isOctal = false;
	bool signEnabled = false;
	int counterHex = 0;
	int counterDec = 0;
	int counterOctal = 0;
	int c = string[0]; // primer caracter
	int s = 1; // signo
	unsigned i = 0;

	while(c != '\0'){
		e = tt[e][columna(c,isHex,isDec,isOctal,signEnabled)];
		switch (e) {
		case 0:
			countDec(&isDec, &counterDec);
			countHex(&isHex, &counterHex);
			countOctal(&isOctal, &counterOctal);
			break;
		case 1:
			if(c == '-'){
			 	s = -1;
			}
			signEnabled = true;
			break;

		case 2:
			if(c >= '1' && c <= '9' && !isDec) {
				isDec = true;
				signEnabled = false;
			}
			break;
		case 3:
			if(c >= '1' && c <= '7' && !isOctal) isOctal = true;
			break;

		case 4:
			//nada para hacer
			break;

		case 5:
			if((c == 'x' || c == 'X') && !isHex) isHex = true;
			break;

		default:
			break;
		}

		c = string[++i];
	}

	if(e == 3 || e == 5 || e == 2){
		countDec(&isDec, &counterDec);
		countHex(&isHex, &counterHex);
		countOctal(&isOctal, &counterOctal);

		printf("Cantidad de numeros decimales: %d \n",counterDec);
		printf("Cantidad de numeros hexadecimales: %d \n",counterHex);
		printf("Cantidad de numeros octales: %d \n",counterOctal);
		return 1;
	}

	return 0;
}