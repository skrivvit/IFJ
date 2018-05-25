#ifndef _PARSER
#define _PARSER

#include <stdio.h>
#include "scanner.h"
#include "ial.h"
#include "precedence.h"
#include "garbage_collector.h"
#include "interpret.h"

extern int condition;
extern symbolTablePtr ans;
extern int varcount;

int parse(symbolTablePtr *symbolTable);
int varType(tToken *token, symbolTablePtr *symbolTable, symbol *s);
int params(tToken *token, symbolTablePtr *symbolTable, symbol *sym, symbolTablePtr test);
int defFunction(tToken *token, symbolTablePtr *symbolTable);




#endif