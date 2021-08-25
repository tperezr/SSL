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
    char s1[] = "cadena a evaluar";
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

/* Automata */
int esPalabra(const char *string){
	static int tt[7][10] = {{1,6,6,3,6,4,6,6,6,6},
							{6,2,6,6,6,6,6,6,6,6},
							{6,6,1,6,6,6,6,6,0,6},
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
	int counterNumbers = 0;
	int c = string[0]; // primer caracter
	unsigned i = 0;
    int operation = 0;
    float finalValue[100];
    float resultados[100];
	float value = 0;
	int sign = 1;

	while(c != '\0'){
		e = tt[e][columna(c,isHex,isDec,isOctal,signEnabled)];
		switch (e) {
		case 0:
            if(isDec){
                for (int x = 0; x < counterNumbers; x++)
				{
					value += resultados[x];
				}
				finalValue[counterDec] = value;
				value = 0;
				counterNumbers = 0;
            } 
			countDec(&isDec, &counterDec);
			countHex(&isHex, &counterHex);
			countOctal(&isOctal, &counterOctal);
			break;
		case 1:
			signEnabled = true;
			if(c == '-') sign = -1;
            operation = c;
			break;
		case 2:
			if(signEnabled) {
				isDec = true;
				signEnabled = false;
				if(operation == '*'){
					resultados[counterNumbers-1] = resultados[counterNumbers-1] * (c - '0');					
				} else {
					resultados[counterNumbers] = (c - '0')*sign;
					counterNumbers++;
				}
				
			}
			sign = 1;
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

	if(e == 2 || e == 3 || e == 5){
        if(isDec && operation == '*'){
            resultados[counterNumbers] += c - '0';
        } else if (isDec){
			resultados[counterNumbers] = c - '0';
		}
		if(isDec) {
			for (int x = 0; x < counterNumbers; x++)
				{
					value += resultados[x];
				}
				finalValue[counterDec] = value;
		}
		countDec(&isDec, &counterDec);
		countHex(&isHex, &counterHex);
		countOctal(&isOctal, &counterOctal);

		printf("Cantidad de numeros decimales: %d \n",counterDec);
		printf("Cantidad de numeros hexadecimales: %d \n",counterHex);
		printf("Cantidad de numeros octales: %d \n",counterOctal);
        for (int i = 0; i < counterDec; i++)
        {
            printf("Calculo decimal Nº %d: %f \n",i+1,finalValue[i]);
        }
		return 1;
	}

	return 0;
}

int columna(int c, bool isHex, bool isDec, bool isOctal, bool signEnabled){
	if(isHex && ((c >= '0' && c <= '9') || isLetterHex(c))) {
		return 7;
	} else if(isOctal && (c >= '0' && c <= '7')){
		return 4;
	} else if(signEnabled && (c >= '1' && c <= '9')){ //decimal sin 0
		return 1;
	} else if(c >= '1' && c <= '7'){
		return 3;
	} else if(isDec && (c == '-' || c == '+' || c == '*')){
        return 2;
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

int verificarCaracteres (char *s) {
	unsigned i;
	for (i=0; s[i]; i++)
	{
		if(!(isdigit(s[i]) 
            || s[i] == '+' 
			|| s[i] == '-'
            || isLetterHex(s[i])
			|| s[i] == '&'
			|| s[i] == 'x'
			|| s[i] == 'X'
            || s[i] == '*')){
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