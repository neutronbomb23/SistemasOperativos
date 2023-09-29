#include <stdio.h>

#define N 10           // Define el tamaño del array a 10
#define MAXELEM 5000   // No se utiliza en este código
#define MAXVALID 100   // No se utiliza en este código

// Función que imprime en la salida estándar el contenido del array v de tamaño size.
void imprimeArray(int v[],int size)
{
	int i;
	printf("-------------------\n");
	for (i=0;i<size;i++)
		printf("%d ",v[i]);
	printf("\n\n");
}

// Función que intenta copiar el contenido del array fuente en un array destino.
void copyArray(int src[],int dst[],int size)
{
		for (int i = 0; i < size; i++)
    	{
        	dst[i] = src[i];
			//Utilizar la funcuón memcpy(dst,src,size);
    	}	
 	//dst = src;    // Esto no copia los contenidos del array. Solo hace que el puntero dst apunte a src.
}

/*
void tmo()  
{
	int x = -1;
	int a[4] = {0,1,2,3};
	int b = 10000;
	int c = -1;
	int i;

	for  (i=4;i<MAXVALID;i++)
		a[i]=i;

	printf("x %d b %d c %d\n", x,b,c);
}
*/ //Segmentation fault, intenta acceder a una zona del kernel, donde no tiene permiso. Violación de segmento. DA ERROR

int main()
{
	int A[N] = {4,3,8,5,6,9,0,1,7,2};  // Array inicializado con valores.
	int B[N];                          // Array vacío.

	//tmo();  // Esta función está comentada y no se ejecuta.
	copyArray(A,B,N);   // Intenta copiar el array A en B.
	imprimeArray(B,N);  // Imprime el array B.
}
