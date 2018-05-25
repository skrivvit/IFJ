#ifndef _INTERPRET
#define _INTERPRET

#include "garbage_collector.h"
#include "ilist.h"
#include "ial.h"
#include "tmp.h"
#include <ctype.h>

typedef struct copy{
	symbolTablePtr param1;
	symbolTablePtr param2;
}*copyParams;

int interpr(tInstList instlist);

#endif
