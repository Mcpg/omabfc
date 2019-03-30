#include <stdio.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "codegen.h"

#define OMABFC_VERSION 010100

static struct option long_options[] =
{
    { "help",        no_argument,       NULL, 'h' },
    { "version",     no_argument,       NULL, 'v' },
    { "input-file",  required_argument, NULL, 'i' },
    { "output-file", required_argument, NULL, 'o' },
    { "code-gen",    required_argument, NULL, 'c' },
    { "tape-size",   required_argument, NULL, 't' },
    { NULL, 0, NULL, 0 }
};

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
    char* output_file_name = NULL;
    output_file = stdout;
    struct codegen_entry* code_generator = NULL;
    struct codegen_options cg_options = { 1000 };

    int option;
    while ((option = getopt_long(argc, argv, "hvi:o:c:t:", long_options, NULL)) != -1)
    {
        switch (option)
        {
            case 'h':
                printf("Usage: %s [options]\n", argv[0]);
                printf(" Available options:\n");
                printf(" --help|-h\tDisplays available options and exits\n");
                printf(" --version|-v\tDisplays omabfc version and exits\n");
                printf(" --input-file|-i\tRequired, specifies input Brianfuck file\n");
                printf(" --output-file|-o\tSpecifies output file for Assembly code. By default omabfc outputs to stdout\n");
                printf(" --code-gen|-c\tRequired, specifies used code generator\n");
                printf(" --tape-size|-t\tPicks size of allocated tape. Defaults to 1000.\n");
                printf(" Available code generators:\n");
                for (int i = 0; code_generators[i].name; i++)
                {
                    printf(" - %s\n", code_generators[i].name);
                }
                printf("\nomabfc version %06d\n", (int) OMABFC_VERSION);
                cleanup();
                exit(0);
                break;
            case 'v':
                printf("omabfc version %06d\n", (int) OMABFC_VERSION);
                cleanup();
                exit(0);
                break;
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
            case 'c':
                for (int i = 0; code_generators[i].name; i++)
                {
                    if (strcmp(optarg, code_generators[i].name) == 0)
                    {
                        code_generator = &code_generators[i];
                        break;
                    }
                }
                break;
            case 't':
                cg_options.tape_size = atoi(optarg);
                break;
        }
    }

    if (!input_file)
        error("Input file not specified\n");

    if (!code_generator)
        error("Code generator not specified or not found!\n");

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

    code_generator->function(parse(input), output_file, cg_options);
    free(input);
    return 0;
}
