#ifndef __CODEGEN_X86_64LIN_H
#define __CODEGEN_X86_64LIN_H

#include <stdio.h>
#include "../symtab.h"
#include "../codegen.h"

void generate_code_x86_64lin(struct symbol_data* data, FILE* output, struct codegen_options options);

#endif