#ifndef __CODEGEN_H
#define __CODEGEN_H

#include <stdio.h>
#include "symtab.h"

struct codegen_options
{
    unsigned int tape_size;
};

struct codegen_entry
{
    const char* name;
    void (*function)(struct symbol_data*, FILE*, struct codegen_options);
};

extern struct codegen_entry code_generators[];

#endif
