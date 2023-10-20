#include <stdio.h>
#include <getopt.h>
#include <unistd.h> /* for getopt() */
#include <stdlib.h> /* for EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>
#include "defs.h"

/* Assume lines in the text file are no larger than 100 chars */
#define MAXLEN_LINE_FILE 100

/* Function to print student records from a text file in a user-friendly format */
int print_text_file(char *path)
{
    // Open the file for reading
    FILE *fp = fopen(path, "r");
    if (!fp)
    {
        perror("Error opening file"); // Display an error if the file cannot be opened
        return EXIT_FAILURE;
    }

    int num_records;
    // Read the number of student records from the first line of the file
    fscanf(fp, "%d\n", &num_records);

    // Variables to store student record data
    char line[MAXLEN_LINE_FILE];
    int student_id;
    char NIF[11]; // 1 extra character for the null terminator
    char first_name[MAXLEN_LINE_FILE];
    char last_name[MAXLEN_LINE_FILE];

    // Loop through and read each student record
    for (int i = 0; i < num_records; i++)
    {
        // Read a line from the file
        if (fgets(line, MAXLEN_LINE_FILE, fp))
        {
            // Parse the student record data from the line
            sscanf(line, "%d:%10[^:]:%[^:]:%[^\n]", &student_id, NIF, first_name, last_name);

            // Print the student record in a user-friendly format
            printf("[Entry #%d]\n", i);
            printf("\tstudent_id=%d\n", student_id);
            printf("\tNIF=%s\n", NIF);
            printf("\tfirst_name=%s\n", first_name);
            printf("\tlast_name=%s\n\n", last_name);
        }
    }

    // Close the file
    fclose(fp);

    return EXIT_SUCCESS;
}

int print_binary_file(char *path) {
    FILE *bin_fp = fopen(path, "rb");
    if (!bin_fp) {
        perror("Error opening binary file");
        return EXIT_FAILURE;
    }

    int num_records;
    fread(&num_records, sizeof(int), 1, bin_fp); // Lee el número de registros

    student_t student;
    char temp_first_name[MAXLEN_LINE_FILE];
    char temp_last_name[MAXLEN_LINE_FILE];

    for (int i = 0; i < num_records; i++) {
        // Lee cada campo del registro desde el archivo binario
        fread(&student.student_id, sizeof(int), 1, bin_fp);
        fread(student.NIF, sizeof(char), MAX_CHARS_NIF + 1, bin_fp);

        // Lee el first_name hasta encontrar el carácter terminador
        int j = 0;
        do {
            fread(&temp_first_name[j], sizeof(char), 1, bin_fp);
        } while (temp_first_name[j++] != '\0');
        student.first_name = temp_first_name;

        // Lee el last_name hasta encontrar el carácter terminador
        j = 0;
        do {
            fread(&temp_last_name[j], sizeof(char), 1, bin_fp);
        } while (temp_last_name[j++] != '\0');
        student.last_name = temp_last_name;

        // Imprime la información del estudiante
        printf("[Entry #%d]\n", i);
        printf("\tstudent_id=%d\n", student.student_id);
        printf("\tNIF=%s\n", student.NIF);
        printf("\tfirst_name=%s\n", student.first_name);
        printf("\tlast_name=%s\n\n", student.last_name);
    }

    fclose(bin_fp);

    return EXIT_SUCCESS;
}


// Escribe registros de estudiantes en un archivo binario desde un archivo de texto
int write_binary_file(char *input_file, char *output_file) {
    FILE *txt_fp = fopen(input_file, "r");
    if (!txt_fp) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    FILE *bin_fp = fopen(output_file, "wb");
    if (!bin_fp) {
        perror("Error opening output file");
        fclose(txt_fp);
        return EXIT_FAILURE;
    }

    int num_records;
    fscanf(txt_fp, "%d\n", &num_records); // Lee el número de registros

    // Escribe el número de registros en el archivo binario
    fwrite(&num_records, sizeof(int), 1, bin_fp);

    char line[MAXLEN_LINE_FILE];
    student_t student;

    for (int i = 0; i < num_records; i++) {
        if (fgets(line, MAXLEN_LINE_FILE, txt_fp)) {
            student.first_name = (char*)malloc(MAXLEN_LINE_FILE * sizeof(char));
            student.last_name = (char*)malloc(MAXLEN_LINE_FILE * sizeof(char));

            sscanf(line, "%d:%10[^:]:%[^:]:%[^\n]", &student.student_id, student.NIF, student.first_name, student.last_name);
            
            // Escribe cada campo del registro en el archivo binario
            fwrite(&student.student_id, sizeof(int), 1, bin_fp);
            fwrite(student.NIF, sizeof(char), MAX_CHARS_NIF + 1, bin_fp);
            fwrite(student.first_name, sizeof(char), strlen(student.first_name) + 1, bin_fp);
            fwrite(student.last_name, sizeof(char), strlen(student.last_name) + 1, bin_fp);

            // Libera memoria
            free(student.first_name);
            free(student.last_name);
        }
    }

    fclose(txt_fp);
    fclose(bin_fp);

    printf("%d student records written successfully to binary file %s\n", num_records, output_file);
    return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{
    int ret_code, opt;
    struct options options;

    // Initialize default command-line options
    options.input_file = NULL;
    options.output_file = NULL;
    options.action = NONE_ACT;
    ret_code = 0;

    // Parse command-line options
    while ((opt = getopt(argc, argv, "hi:po:b")) != -1)
    {
        switch (opt)
        {
        // Print help message
        case 'h':
            fprintf(stderr, "Usage: %s [ -h | -p | -i file ]\n", argv[0]);
            exit(EXIT_SUCCESS);

        // Get input file name
        case 'i':
            options.input_file = optarg;
            break;

        // Set action to print student records from a text file
        case 'p':
            options.action = PRINT_TEXT_ACT;
            break;

        // Get output file name and set action to write student records to a binary file
        case 'o':
            options.output_file = optarg;
            options.action = WRITE_BINARY_ACT;
            break;

        // Set action to print student records from a binary file
        case 'b':
            options.action = PRINT_BINARY_ACT;
            break;

        default:
            fprintf(stderr, "Usage: %s [ -h | -p | -i file ]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Ensure an input file has been specified
    if (options.input_file == NULL)
    {
        fprintf(stderr, "Must specify one record file as an argument of -i\n");
        exit(EXIT_FAILURE);
    }

    // Handle the specified action
    switch (options.action) {
        case NONE_ACT:
            fprintf(stderr, "Must indicate one of the following options: -p, -o, -b \n");
            exit(EXIT_FAILURE);

        case PRINT_TEXT_ACT:
            ret_code = print_text_file(options.input_file);
            break;

        case WRITE_BINARY_ACT:
            ret_code = write_binary_file(options.input_file, options.output_file);
            break;

        case PRINT_BINARY_ACT:
            ret_code = print_binary_file(options.input_file);
            break;

        default:
            break;
    }

    // Exit the program with the appropriate return code
    exit(ret_code);
}