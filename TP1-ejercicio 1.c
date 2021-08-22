#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

int verificarCaracteres(char *s);
int isLetterHex(char c);
int esPalabra(const char *cadena);
int columna(int c, bool esHex, bool esDec, bool esOctal, bool signEnabled);
void countDec(bool *esDec, int *counterDec);
void countHex(bool *esHex, int *counterHex);
void countOctal(bool *esOctal, int *counterOctal);

int main()
{
    char s1[] = "+125&154&-1";
	if (!verificarCaracteres(s1)) 
	{
		printf("Caracteres invalidos\n");
		getchar();
		return 0;
	}
	if(esPalabra(s1)){
		printf("Pertenece al lenguaje\n");
		getchar();
		return 0;
	}
	printf("no pertenece al lenguaje\n"); 
	getchar();
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

void countDec(bool *esDec, int *counterDec){
	if(*esDec){
		*counterDec += 1;
	}
	*esDec = false;
}
void countHex(bool *esHex, int *counterHex){
	if(*esHex){
		*counterHex += 1;
	}
	*esHex = false;
}
void countOctal(bool *esOctal, int *counterOctal){
	if(*esOctal){
		*counterOctal += 1;
	}
	*esOctal = false;
}

int columna(int c, bool esHex, bool esDec, bool esOctal, bool signEnabled){
	if(esHex && ((c >= '0' && c <= '9') || isLetterHex(c))) {
		return 7;
	} else if (esDec && (c >= '0' && c <= '9')) {
		return 2;
	} else if(esOctal && (c >= '0' && c <= '7')){
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
int esPalabra(const char *cadena){
	static int tt[7][10] = {{1,6,6,3,6,4,6,6,6,6},
							{6,2,6,6,6,6,6,6,6,6},
							{6,6,2,6,6,6,6,6,0,6},
							{6,6,6,6,3,6,6,6,0,6},
							{6,6,6,6,6,6,5,6,6,6},
							{6,6,6,6,6,6,6,5,0,6},
							{6,6,6,6,6,6,6,6,6,6}};
	int e = 0; //estado inicial
	bool esHex = false; 
	bool esDec = false;
	bool esOctal = false;
	bool signEnabled = false;
	int counterHex = 0;
	int counterDec = 0;
	int counterOctal = 0;
	int c = cadena[0]; // primer caracter
	int s = 1; // signo
	unsigned i = 0;

	while(c != '\0'){
		e = tt[e][columna(c,esHex,esDec,esOctal,signEnabled)];
		switch (e) {
		case 0:
			countDec(&esDec, &counterDec);
			countHex(&esHex, &counterHex);
			countOctal(&esOctal, &counterOctal);
			break;
		case 1:
			if(c == '-'){
			 	s = -1;
			}
			signEnabled = true;
			break;

		case 2:
			if(c >= '1' && c <= '9' && !esDec) {
				esDec = true;
				signEnabled = false;
			}
			break;
		case 3:
			if(c >= '1' && c <= '7' && !esOctal) esOctal = true;
			break;

		case 4:
			//nada para hacer
			break;

		case 5:
			if((c == 'x' || c == 'X') && !esHex) esHex = true;
			break;

		default:
			break;
		}

		c = cadena[++i];
	}
	countDec(&esDec, &counterDec);
	countHex(&esHex, &counterHex);
	countOctal(&esOctal, &counterOctal);

	if(e == 3 || e == 5 || e == 2){
		printf("Cantidad de numeros decimales: %d \n",counterDec);
		printf("Cantidad de numeros hexadecimales: %d \n",counterHex);
		printf("Cantidad de numeros octales: %d \n",counterOctal);
		return 1;
	}

	return 0;
}




/* Automata 1 
int esPalabra(const char *cadena){
	static int tt[7][10] = {{1,6,6,3,6,4,6,6,6,6},
							{6,2,6,6,6,6,6,6,6,6},
							{6,6,2,6,6,6,6,6,0,6},
							{6,6,6,6,3,6,6,6,0,6},
							{6,6,6,6,6,6,5,6,6,6},
							{6,6,6,6,6,6,6,5,0,6},
							{6,6,6,6,6,6,6,6,6,6}};
	int e = 0; //estado inicial
	bool esHex = false; 
	bool esDec = false;
	bool esOctal = false;
	bool signEnabled = false;
	int counterHex = 0;
	int counterDec = 0;
	int counterOctal = 0;
	int c = cadena[0]; // primer caracter
	int s = 1; // signo
	unsigned i = 0;
	int aux = 0;

	while(c != '\0'){
		e = tt[e][columna(c,esHex,esDec,esOctal,signEnabled)];
		switch (e) {
		case 0:
			if(c == '-'){
			 	s = -1;
			}
			signEnabled = true;
			break;

		case 1:
			if(c >= '1' && c <= '9') esDec = true;
			break;

		case 2: 
			if(c == '&') counterDec++; esDec = false;
			break;

		case 3:
			if(c >= '1' && c <= '7' && !esOctal) esOctal = true;
			if(c == '&') counterOctal++; esOctal = false;
			break;

		case 4:
			aux = cadena[i+1];
			if(c == '0' && (aux == 'x' || aux == 'X')) esHex = true;
			break;

		case 5:
			if(c == '&') counterHex++; esHex = false;
			break;
			
		default:
			break;
		}

		c = cadena[++i];
	}
	printf("%d",counterDec);
	printf("%d",counterHex);
	printf("%d",counterOctal);
	getchar();
}*/