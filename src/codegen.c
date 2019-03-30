#include "codegen.h"
#include "codegen/cgx86_64lin.h"
#include "codegen/cgx86_16dos.h"

#include <stddef.h>

struct codegen_entry code_generators[] =
{
    { "x86_64lin", generate_code_x86_64lin },
    { "x86_16dos", generate_code_x86_16dos },
    { NULL, NULL } /* Double NULL entry indicates end of the code generator array */
};
