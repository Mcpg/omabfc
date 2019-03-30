#include "symtab.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* The symtab generator should be rewritten. I think there might be some memory
   leaks here and there, it requires careful investigation, or a total rewrite.
   
   Even if the code is just 90 lines, I find it hard to see how everything
   flows here. I guess that's bad? */

/* Compiling code that starts with a loop creates a SYMBOL_NONE in the table,
   which really shouldn't be there. */

static int is_symbol(char c)
{
    return c == SYMBOL_VINC ||
            c == SYMBOL_VDEC ||
            c == SYMBOL_PINC ||
            c == SYMBOL_PDEC ||
            c == SYMBOL_OUT ||
            c == SYMBOL_IN ||
            c == SYMBOL_LSTART ||
            c == SYMBOL_LEND;
}

static void parse_loop(char* bfcode, int* i, struct symbol_data* loop_symbol);

static void parse_symbol(char* bfcode, int* i, struct symbol_data** target_val)
{
    char c = bfcode[*i];
    struct symbol_data* target = *target_val;

    if (target->type == c && c != SYMBOL_LSTART)
    {
        target->amount++;
    }
    else
    {
        if (target->type == SYMBOL_NONE && c != SYMBOL_LSTART)
        {
            target->type = c;
            target->amount = 1;
            target->next = NULL;
            target->loop_data.loop_start = NULL;
            return;
        }

        struct symbol_data* ndata = (struct symbol_data*) malloc(sizeof(struct symbol_data));
        ndata->type = c;
        ndata->amount = 1;
        ndata->next = NULL;

        target->next = ndata;
        (*target_val) = ndata;

        if (c == SYMBOL_LSTART)
        {
            (*i)++;
            parse_loop(bfcode, i, ndata);
        }
    }
}

static void parse_loop(char* bfcode, int* i, struct symbol_data* loop_symbol)
{
    loop_symbol->loop_data.loop_start = (struct symbol_data*) malloc(sizeof(struct symbol_data));
    struct symbol_data* current = loop_symbol->loop_data.loop_start;

    current->type = SYMBOL_NONE;

    for (; bfcode[*i] && bfcode[*i] != SYMBOL_LEND; (*i)++)
    {
        char c = bfcode[*i];
        if (!is_symbol(c))
            continue;

        parse_symbol(bfcode, i, &current);
    }
}

struct symbol_data* parse(char* bfcode)
{
    struct symbol_data* first = (struct symbol_data*) malloc(sizeof(struct symbol_data));
    struct symbol_data* current = first;

    first->type = SYMBOL_NONE;

    for (int i = 0; bfcode[i]; i++)
    {
        char c = bfcode[i];
        if (!is_symbol(c) || c == SYMBOL_LEND)
            continue;

        parse_symbol(bfcode, &i, &current);
    }

    return first;
}
