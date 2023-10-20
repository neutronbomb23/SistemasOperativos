#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <file_name> <string1> <string2> ...\n", argv[0]);
        exit(1);
    }

    // Abrir el archivo en modo escritura (reescribirá el contenido si ya existe)
    FILE *file = fopen(argv[1], "w");
    if (!file) {
        err(2, "The output file %s could not be opened", argv[1]);
    }

    // Recorrer los argumentos que representan cadenas y escribirlos en el archivo
    for (int i = 2; i < argc; i++) {
        // Escribir la cadena en el archivo
        fwrite(argv[i], sizeof(char), strlen(argv[i]) + 1, file); // +1 para incluir el '\0'
    }

    // Cerrar el archivo
    fclose(file);

    return 0;
}
/*Este programa realiza las siguientes acciones:

Comprueba que se hayan proporcionado al menos dos argumentos (nombre del archivo y al menos una cadena).
Abre el archivo especificado en modo escritura. Si el archivo ya existe, se sobrescribe su contenido.
Recorre las cadenas pasadas como argumentos y las escribe en el archivo. También se escribe el carácter de terminación \0 después de cada cadena.
Cierra el archivo y termina el programa.
Para compilar y ejecutar write_strings.c:

Compila el programa:

gcc -o write_strings write_strings.c

Ejecuta el programa write_strings:

./write_strings out.txt "Hello" "World" "OpenAI"
Esto creará (o sobrescribirá si ya existe) un archivo llamado out.txt y escribirá las cadenas "Hello", "World" y "OpenAI" en él, separadas por el carácter de terminación \0.*/