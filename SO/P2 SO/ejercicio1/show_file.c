#include <err.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    FILE* file = NULL;
    unsigned char buffer;  // Buffer to store the byte we read
    size_t bytesRead, bytesWritten;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
        exit(1);
    }

    /* Open file */
    if ((file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "The input file %s could not be opened\n", argv[1]);
        exit(2);
    }

    /* Read file byte by byte */
    while ((bytesRead = fread(&buffer, 1, 1, file)) == 1) {
        /* Write byte to stdout */
        bytesWritten = fwrite(&buffer, 1, 1, stdout);

        if (bytesWritten != 1) {
            fclose(file);
            fprintf(stderr, "fwrite() failed!!\n");
            exit(3);
        }
    }

    fclose(file);
    return 0;
}

// Explicacion: 
/*El programa show_file.c toma el nombre de un archivo como argumento, lo abre en modo de lectura, lee el contenido byte a byte usando getc(),
y muestra dicho contenido en la salida estándar usando putc().

Para cambiar el código de modo que use fread() en lugar de getc() y fwrite() en lugar de putc(), es necesario realizar las siguientes modificaciones:

fread() lee bloques de datos de un archivo, por lo que debemos especificar el tamaño del bloque 
y el número de bloques que queremos leer. En este caso, queremos leer byte a byte, por lo que el tamaño del bloque es 1 
y el número de bloques es también 1.

fwrite() escribe bloques de datos en un archivo o en la salida estándar. Así que, de manera similar a fread(), debemos especificar el tamaño del bloque y el número de bloques que queremos escribir.

Tanto fread() como fwrite() devuelven el número de bloques leídos/escritos con éxito. Así que, en caso de error o al final del archivo, este número será menor que el número de bloques que queríamos leer/escribir.*/