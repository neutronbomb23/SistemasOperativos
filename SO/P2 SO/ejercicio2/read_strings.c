#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

char *loadstr(FILE *file) {
    long start_pos, end_pos;
    size_t length;
    char *result = NULL;
    int ch;

    // Recordar la posición actual del archivo
    start_pos = ftell(file);

    // Determinar la longitud de la cadena leyendo hasta el carácter '\0'
    while ((ch = fgetc(file)) != EOF && ch != '\0');
    if (ch == EOF) {
        fseek(file, start_pos, SEEK_SET);
        return NULL;
    }

    // Obtener la posición del '\0' para calcular la longitud
    end_pos = ftell(file);
    length = end_pos - start_pos;

    // Reservar memoria para la cadena más el carácter '\0'
    result = (char *)malloc(length + 1);
    if (!result) {
        err(1, "malloc failed");
    }

    // Regresar al inicio de la cadena y leerla en el buffer
    fseek(file, start_pos, SEEK_SET);
    fread(result, 1, length, file);

    // Asegurarse de que la cadena es terminada por '\0'
    result[length] = '\0';

    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        exit(1);
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        err(2, "The input file %s could not be opened", argv[1]);
    }

    char *str;
    while ((str = loadstr(file)) != NULL) {
        printf("%s\n", str);
        free(str); // Liberar la memoria reservada por loadstr
    }

    fclose(file);
    return 0;
}
/*Esta implementación:

Abre el archivo especificado para lectura.
Usa loadstr para leer las cadenas terminadas en '\0' del archivo.
Imprime cada cadena en la consola, seguida de un salto de línea.
Libera la memoria de cada cadena después de imprimir.
Cierra el archivo antes de terminar el programa.
La función loadstr():

Determina la longitud de la cadena al leer hasta el carácter '\0'.
Reserva memoria para la cadena.
Regresa al inicio de la cadena en el archivo y lee la cadena en el buffer reservado.
Asegura que la cadena esté terminada por '\0' y la devuelve.*/