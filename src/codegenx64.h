#ifndef __CODEGEN_H
#define __CODEGEN_H

#include <stdio.h>
#include "symtab.h"

void generate_code(struct symbol_data* data, FILE* output);

#endif