#ifndef __SYMTAB_H
#define __SYMTAB_H

enum symbol_type
{
    SYMBOL_NONE   = 0,
    SYMBOL_VINC   = '+',
    SYMBOL_VDEC   = '-',
    SYMBOL_PINC   = '>',
    SYMBOL_PDEC   = '<',
    SYMBOL_OUT    = '.',
    SYMBOL_IN     =  ',',
    SYMBOL_LSTART = '[',
    SYMBOL_LEND   = ']'
};

struct loop_data
{
    struct symbol_data* loop_start;
};

struct symbol_data
{
    enum symbol_type type;
    unsigned int amount;
    struct symbol_data* next;
    struct loop_data loop_data;
};

struct symbol_data* parse(char* bfcode);

#endif