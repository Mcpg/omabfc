#include <stdio.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "codegenx64.h"

#define OMABFC_VERSION 10000

static struct option long_options[] =
{
    { "version",     no_argument,       NULL, 'v' },
    { "input-file",  required_argument, NULL, 'i' },
    { "output-file", required_argument, NULL, 'o' }
};

static char* output_file_name = NULL;

static FILE* input_file;
static FILE* output_file;

static void cleanup()
{
    if (input_file != NULL && input_file != stdin)
        fclose(input_file);
    if (output_file != NULL && output_file != stdout)
        fclose(output_file);
}

static void error(const char* fmt, ...)
{
    va_list list;
    va_start(list, fmt);
    vfprintf(stderr, fmt, list);
    va_end(list);
    cleanup();
    exit(1);
}

int main(int argc, char* argv[])
{
    output_file = stdout;

    int option;
    while ((option = getopt_long(argc, argv, "vi:o:", long_options, NULL)) != -1)
    {
        switch (option)
        {
            case 'v':
                printf("omabfc version %06d\n", (int) OMABFC_VERSION);
                cleanup();
                exit(0);
            case 'o':
                output_file_name = malloc(strlen(optarg) + 1);
                memset(output_file_name, 0, strlen(optarg) + 1);
                strcpy(output_file_name, optarg);
                break;
            case 'i':
                input_file = fopen(optarg, "r");
                if (!input_file)
                    error("Couldn't open the input file!\n");
                break;
        }
    }

    if (!input_file)
        error("Input file not specified\n");

    if (output_file_name)
    {
        output_file = fopen(output_file_name, "w");
        if (!output_file)
            error("Couldn't open the output file!\n");
    }

    fseek(input_file, 0, SEEK_END);
    size_t input_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    char* input = malloc(input_size + 1);
    input[input_size] = '\0';

    if (fread(input, sizeof(char), input_size, input_file) != input_size)
        error("Couldn't read from the input file!\n");

    generate_code(parse(input), output_file);
    free(input);
    return 0;
}
