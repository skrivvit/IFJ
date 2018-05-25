#include "precedence.h"
stack s1;
tableEntries entry = nula;
int varcount = 0;
symbolTablePtr ans=NULL;
name_stack s2;		// zásobník pro ukládání jmen identifikátorů
int condition;


void stackInit(stack *ptr){
	ptr->top = NULL;
}

void push(stack *ptr, tableEntries data){
	stackItem item;

	if ((item = malloc(sizeof(struct sItem))) == NULL){
		return;
	}
	item->data = data;
	item->ptr = ptr->top;
	ptr->top = item;
}



void pop(stack *ptr){
	stackItem item;

	if (ptr->top != NULL){
		item = ptr->top;
		ptr->top = item->ptr;
		free(item);
	}

}

tableEntries top(stack *ptr){
	if (ptr->top != NULL)
		return ptr->top->data;
	return error;
}

bool stackEmpty(stack *ptr){
	return ptr->top == NULL;
}

void freeStack(stack *ptr){
	stackItem tmp;

	while (ptr->top != NULL){
		tmp = ptr->top;
		ptr->top = tmp->ptr;
		free(tmp);
	}
}

void stackInit_N(name_stack *ptr){
	ptr->top = NULL;
}

void push_N(name_stack *ptr, char* data){
	id_name item;

	if ((item = malloc(sizeof(struct id_item))) == NULL){
		return;
	}
	item->name = data;
	item->ptr = ptr->top;
	ptr->top = item;
}



void pop_N(name_stack *ptr){
	id_name item;

	if (ptr->top != NULL){
		item = ptr->top;
		ptr->top = item->ptr;
		free(item);
	}
}

char* top_N(name_stack *ptr){
	if (ptr->top != NULL)
		return ptr->top->name;
	return "\0";
}

bool stackEmpty_N(name_stack *ptr){
	return ptr->top == NULL;
}

void freeStack_N(name_stack *ptr){
	id_name tmp;

	while (ptr->top != NULL){
		tmp = ptr->top;
		ptr->top = tmp->ptr;
		free(tmp);
	}
}

const tablePriorities precedenceTable[14][14] = {

	/*			 *	/  +  -  <  >  <= >= =  <> ID (  )  $  */
	/* * */		{G, G, G, G, G, G, G, G, G, G, L, L, G, G},
	/* / */		{G, G, G, G, G, G, G ,G, G, G, L, L, G, G},
	/* + */		{L, L, G, G, G, G, G, G, G, G, L, L, G, G},
	/* - */		{L, L, G, G, G, G, G, G, G, G, L, L, G, G},
	/* < */		{L, L, L, L, G, G, G, G, G, G, L, L, G, G},
	/* > */		{L, L, L, L, G, G, G, G, G, G, L, L, G, G},
	/* <= */	{L, L, L, L, G, G, G, G, G, G, L, L, G, G},
	/* >= */	{L, L, L, L, G, G, G, G, G, G, L, L, G, G},
	/* = */		{L, L, L, L, G, G, G, G, G, G, L, L, G, G},
	/* <> */	{L, L, L, L, G, G, G, G, G, G, L, L, G, G},
	/* ID */	{G, G, G, G, G, G, G, G, G, G, B, B, G, G},
	/* ( */		{L, L, L, L, L, L, L, L, L, L, L, L, E, B},
	/* ) */		{G, G, G, G, G, G, G, G, G, G, B, B, G, G},
	/* $ */		{L, L, L, L, L, L, L, L, L, L, L, L, B, B}
};

tableEntries getIndex(tToken *token,symbolTablePtr *symbolTable){


	switch (token->state)
	{
	case T_MUL:{
		return mul;
	}

	case T_DIV:{
		return divide;
	}

	case T_PLUS:{
		return plus;
	}

	case T_MINUS:{
		return minus;
	}

	case T_LESSER:{
		return lesser;
	}

	case T_GREATER:{
		return greater;
	}

	case T_LOE:{
		return loe;
	}

	case T_GOE:{
		return goe;
	}

	case T_EQUAL:{
		return equal;
	}

	case T_NOTEQUAL:{
		return notequal;
	}

	case T_LC:{
		return lc;
	}

	case T_RC:{
		return rc;
	}

	case T_SEMICOLON:{
		if(condition){
			result = SYNTAX_ERR;
			return error;
		}
		return dolar;
	}

	case T_INTEGER:{
		symbol *item = gMalloc(sizeof(symbol));
		item->key = gMalloc(sizeof(char)*10);
		sprintf(item->key,"#_%d",varcount++);
		item->type = tInt;
		item->isDefined = true;
		item->value.i = strtol(token->content,NULL,0);
		if (BTInsert(symbolTable, item->key, *item) != 0){
			result = SEM_ERR;
		}
		ans = BTSearch(symbolTable, item->key);
		push_N(&s2, item->key);
		return id;
	}

	case T_REAL :{
		symbol *item = gMalloc(sizeof(symbol));
		item->key = gMalloc(sizeof(char)*10);
		sprintf(item->key,"#_%d",varcount++);
		item->type = tReal;
		item->isDefined = true;
		item->value.r = strtod(token->content,NULL);
		if (BTInsert(symbolTable, item->key, *item) != 0){
			result = SEM_ERR;
		}
		ans = BTSearch(symbolTable, item->key);
		push_N(&s2, item->key);
		return id;
	}

	case T_STRING:{
		symbol *item = gMalloc(sizeof(symbol));
		item->key = gMalloc(sizeof(char)*10);
		sprintf(item->key,"#_%d",varcount++);
		item->type = tString;
		item->isDefined = true;
		item->value.s = token->content;
		if (BTInsert(symbolTable, item->key, *item) != 0){
			result = SEM_ERR;
		}
		ans = BTSearch(symbolTable, item->key);
		push_N(&s2, item->key);
		return id;
	}

	case T_IDENTIFICATOR : {
	  if (top(&s1) == id){
		  result = SYNTAX_ERR;
	      return error;
	 }

		ans = BTSearch(symbolTable,token->content);
		if(ans==NULL){
			result = SEM_ERR;
			return error;
		}

		if (ans->content.isFunction){
			result = SEM_ERR2;
			return error;
		}
		push_N(&s2,token->content);
		return id;
		
	}
	case T_KEYWORD:{
		if(condition == 1 && (strcmp(token->content,"then")== 0))
			return dolar;
		if(condition == 2 && (strcmp(token->content,"do")== 0))
			return dolar;
		if (condition == 3 && (strcmp(token->content, "end") == 0))
			return dolar;
		if (condition == 0 && (strcmp(token->content, "end") == 0))
			return dolar;

		if (!strcmp(token->content, "true\0") || !strcmp(token->content, "false\0")){
			symbol *item = gMalloc(sizeof(symbol));
			item->key = gMalloc(sizeof(char)* 10);
			sprintf(item->key, "#_%d", varcount++);
			item->type = tBool;
			item->isDefined = true;
			if (!strcmp(token->content, "true\0")){
				item->value.b = true;			
			}
			else{
				item->value.b = false;
			}
			if (BTInsert(symbolTable, item->key, *item) != 0){
				result = SEM_ERR;
			}
			ans = BTSearch(symbolTable, item->key);
			push_N(&s2, item->key);
			return id;
		}

		return error;
	}


	default:{
		return error;
	}

	}

}


tableEntries reduction(stack *stack, name_stack *n_stack, symbolTablePtr *symbolTable){
	tableEntries tmp;	//pomocná proměnná
	tableEntries item;	// návratová hodnota

	tInst inst;
	symbol *polozka = gMalloc(sizeof(symbol));
	polozka->key = gMalloc(sizeof(char)*10);
	symbolTablePtr pom1,pom2;



	tmp = top(stack);
	pop(stack);
	switch(tmp){

		case mul:{
			if(entry == nonterminal){
				entry = nula;
				if(top(stack) == nonterminal){
					pop(stack);
					pop(stack);
					item = top(stack);
					push(stack, nonterminal);
					pom1 = BTSearch(symbolTable,top_N(n_stack));
					pop_N(n_stack);
					pom2 = BTSearch(symbolTable,top_N(n_stack));
					sprintf(polozka->key,"#_%d",varcount++);
					switch(pom1->content.type){
					
						case tInt:{
							if(pom2->content.type == tInt){
								polozka->type = tInt;
								polozka->isDefined = true;
								if (BTInsert(symbolTable, polozka->key, *polozka))
									result = SEM_ERR;
								ans = BTSearch(symbolTable,polozka->key);
								inst = createInst(I_MUL, pom2, pom1, (void*) ans);
								break;
							}

							if(pom2->content.type == tReal){
								polozka->type = tReal;
								polozka->isDefined = true;
								if (BTInsert(symbolTable, polozka->key, *polozka))
									result = SEM_ERR;
								ans = BTSearch(symbolTable,polozka->key);
								inst = createInst(I_MUL, pom2, pom1, (void*) ans);
								break;
							}
							else{
								result = SEM_ERR2;							//chyba typové kompatibility
								return error;
							}
						}

						case tReal:{
							polozka->type = tReal;
							polozka->isDefined = true;
							if (BTInsert(symbolTable, polozka->key, *polozka))
								result = SEM_ERR;
						
							if(pom2->content.type == tInt){
								ans = BTSearch(symbolTable,polozka->key);
								inst = createInst(I_MUL, pom2, pom1,(void*) ans);
								break;
							}
							if(pom2->content.type == tReal){							
								ans = BTSearch(symbolTable,polozka->key);
								inst = createInst(I_MUL, pom2, pom1,(void*) ans);
								break;
							}
							else{
								result = SEM_ERR2;							//chyba typové kompatibility
								return error;
							}

						}

						case tString:
						case tBool:{
								result = SEM_ERR2;							//chyba typové kompatibility
								return error;
						}
					}
		
					pop_N(n_stack);
					push_N(&s2, polozka->key);
					instListInsert(inst_list_global,inst);
					return item;
				}
				else{
					result = SYNTAX_ERR;
					return error;
				}
			}
			else
				return error;
		}

		case divide:{
			if(entry == nonterminal){
				entry = nula;
				if(top(stack) == nonterminal){
					pop(stack);
					pop(stack);
					item = top(stack);
					push(stack, nonterminal);
					pom1 = BTSearch(symbolTable,top_N(n_stack));
					pop_N(n_stack);
					pom2 = BTSearch(symbolTable,top_N(n_stack));
					sprintf(polozka->key,"#_%d",varcount++);
					polozka->type = tReal;
					polozka->isDefined = true;
					if (BTInsert(symbolTable, polozka->key, *polozka))
						result = SEM_ERR;
					ans = BTSearch(symbolTable,polozka->key);

					switch(pom1->content.type){
					
						case tInt:{
							if(pom2->content.type == tInt){
								inst = createInst(I_DIV, pom2, pom1,(void*) ans);
								break;
							}

							if(pom2->content.type == tReal){
								inst = createInst(I_DIV, pom2, pom1,(void*) ans);
								break;
							}
							else{
								result = SEM_ERR2;							//chyba typové kompatibility
								return error;
							}
						}

						case tReal:{
							if(pom2->content.type == tInt){
								inst = createInst(I_DIV, pom2, pom1,(void*) ans);
								break;
							}
							if(pom2->content.type == tReal){							
								inst = createInst(I_DIV, pom2, pom1,(void*) ans);
								break;
							}
							else{
								result = SEM_ERR2;							//chyba typové kompatibility
								return error;
							}

						}

						case tString:
						case tBool:{
								result = SEM_ERR2;							//chyba typové kompatibility
								return error;
						}
					}

					pop_N(n_stack);
					push_N(&s2, polozka->key);
					instListInsert(inst_list_global,inst);
					return item;
				}
				else{
					result = SYNTAX_ERR;
					return error;
				}
			}
			else{
				result = SYNTAX_ERR;
				return error;
				}
		}

		case plus:{
			if(entry == nonterminal){
				entry = nula;
				if(top(stack) == nonterminal){
					pop(stack);
					pop(stack);
					item = top(stack);
					push(stack, nonterminal);
					pom1 = BTSearch(symbolTable,top_N(n_stack));
					pop_N(n_stack);
					pom2 = BTSearch(symbolTable,top_N(n_stack));
					sprintf(polozka->key,"#_%d",varcount++);

					switch(pom1->content.type){
					
						case tInt:{
							if(pom2->content.type == tInt){
								polozka->type = tInt;
								polozka->isDefined = true;
								if (BTInsert(symbolTable, polozka->key, *polozka))
									result = SEM_ERR;
								ans = BTSearch(symbolTable,polozka->key);
								inst = createInst(I_ADD, pom2, pom1,(void*) ans);
								break;
							}

							if(pom2->content.type == tReal){
								polozka->type = tReal;
								polozka->isDefined = true;
								if (BTInsert(symbolTable, polozka->key, *polozka))
									result = SEM_ERR;
								ans = BTSearch(symbolTable,polozka->key);
								inst = createInst(I_ADD, pom2, pom1,(void*) ans);
								break;
							}
							else{
								result = SEM_ERR2;							//chyba typové kompatibility
								return error;
							}
						}

						case tReal:{
							polozka->type = tReal;
							polozka->isDefined = true;
							if (BTInsert(symbolTable, polozka->key, *polozka))
								result = SEM_ERR;
							
							if(pom2->content.type == tReal){
								ans = BTSearch(symbolTable,polozka->key);
								inst = createInst(I_ADD, pom2, pom1,(void*) ans);
								break;
							}
								
							if(pom2->content.type == tInt){
								ans = BTSearch(symbolTable,polozka->key);
								inst = createInst(I_ADD, pom2, pom1,(void*) ans);
								break;
							}
							else{
								result = SEM_ERR2;							//chyba typové kompatibility
								return error;
							}
						}

						case tString:{
							if(pom2->content.type == tString){		// POLOZKA UPRAVENA !!!!!
								polozka->type = tString;
								polozka->isDefined = true;
								if (BTInsert(symbolTable, polozka->key, *polozka))
									result = SEM_ERR;
								ans = BTSearch(symbolTable,polozka->key);
								inst = createInst(I_CONC, pom2, pom1,(void*) ans);
								break;
							}
							else{
								result = SEM_ERR2;							//chyba typové kompatibility
								return error;
							}
						}

						case tBool:{
								result = SEM_ERR2;							//chyba typové kompatibility
								return error;
						}
					}


					pop_N(n_stack);
					push_N(&s2, polozka->key);
					instListInsert(inst_list_global,inst);
					return item;
				}
				else{
					result = SYNTAX_ERR;
					return error;
				}
			}
			else
				return error;
		}

		case minus:{
			if(entry == nonterminal){
				entry = nula;
				if(top(stack) == nonterminal){
					pop(stack);
					pop(stack);
					item = top(stack);
					push(stack, nonterminal);
					pom1 = BTSearch(symbolTable,top_N(n_stack));
					pop_N(n_stack);
					pom2 = BTSearch(symbolTable,top_N(n_stack));
					sprintf(polozka->key,"#_%d",varcount++);
					switch(pom1->content.type){
					
						case tInt:{
							if(pom2->content.type == tInt){
								polozka->type = tInt;
								polozka->isDefined = true;
								if (BTInsert(symbolTable, polozka->key, *polozka))
									result = SEM_ERR;
								ans = BTSearch(symbolTable,polozka->key);
								inst = createInst(I_SUB, pom2, pom1, (void*) ans);
								break;
							}

							if(pom2->content.type == tReal){
								polozka->type = tReal;
								polozka->isDefined = true;
								if (BTInsert(symbolTable, polozka->key, *polozka))
									result = SEM_ERR;
								ans = BTSearch(symbolTable,polozka->key);
								inst = createInst(I_SUB, pom2, pom1, (void*) ans);
								break;
							}
							else{
								result = SEM_ERR2;							//chyba typové kompatibility
								return error;
							}
						}

						case tReal:{
							polozka->type = tReal;

							if (BTInsert(symbolTable, polozka->key, *polozka))
								result = SEM_ERR;
						
							if(pom2->content.type == tInt){
								ans = BTSearch(symbolTable,polozka->key);
								inst = createInst(I_SUB, pom2, pom1,(void*) ans);
								break;
							}
							if(pom2->content.type == tReal){							
								ans = BTSearch(symbolTable,polozka->key);
								inst = createInst(I_SUB, pom2, pom1,(void*) ans);
								break;
							}
							else{
								result = SEM_ERR2;							//chyba typové kompatibility
								return error;
							}

						}

						case tString:
						case tBool:{
							return error;				// sémantická chyba
						}
					}
					pop_N(n_stack);
					push_N(&s2, polozka->key);
					instListInsert(inst_list_global,inst);
					return item;
				}
				else{
					result = SYNTAX_ERR;
					return error;
				}
			}
			else
				return error;
		}

		case lesser:{
			if(entry == nonterminal){
					if(stack->top->data != nonterminal){
						result = SYNTAX_ERR;			// neexistuje levá strana porovnání
						return error;
					}
					pop(stack);
					item = top(stack);
					push(stack, nonterminal);
					pom1 = BTSearch(symbolTable,top_N(n_stack));
					pop_N(n_stack);
					pom2 = BTSearch(symbolTable,top_N(n_stack));
					sprintf(polozka->key,"#_%d",varcount++);
					if(pom1->content.type != pom2->content.type){
						result = SEM_ERR2;							//chyba typové kompatibility
						return error;
					}
					polozka->type = tBool;
					polozka->isDefined = true;
					if (BTInsert(symbolTable, polozka->key, *polozka))
						result = SEM_ERR;
					ans = BTSearch(symbolTable,polozka->key);
					inst = createInst(I_LESSER, pom2, pom1, (void*) ans);
					pop_N(n_stack);
					push_N(&s2, polozka->key);
					instListInsert(inst_list_global,inst);
					return item;
			}
			else
				return error;
		}
		
		case greater:{
			if(entry == nonterminal){
					if(stack->top->data != nonterminal){
						result = SYNTAX_ERR;			// neexistuje levá strana porovnání
						return error;
					}
					pop(stack);
					item = top(stack);
					push(stack, nonterminal);
					pom1 = BTSearch(symbolTable,top_N(n_stack));
					pop_N(n_stack);
					pom2 = BTSearch(symbolTable,top_N(n_stack));
					sprintf(polozka->key,"#_%d",varcount++);
					if(pom1->content.type != pom2->content.type){
						result = SEM_ERR2;							//chyba typové kompatibility
						return error;
					}
					polozka->type = tBool;
					polozka->isDefined = true;
					if (BTInsert(symbolTable, polozka->key, *polozka))
						result = SEM_ERR;
					ans = BTSearch(symbolTable,polozka->key);
					inst = createInst(I_GREATER, pom2, pom1, (void*) ans);
					pop_N(n_stack);
					push_N(&s2, polozka->key);
					instListInsert(inst_list_global,inst);
					return item;
			}
			else
				return error;
		}
		
		case loe:{
			if(entry == nonterminal){
					if(stack->top->data != nonterminal){
						result = SYNTAX_ERR;			// neexistuje levá strana porovnání
						return error;
					}
					pop(stack);
					item = top(stack);
					push(stack, nonterminal);
					pom1 = BTSearch(symbolTable,top_N(n_stack));
					pop_N(n_stack);
					pom2 = BTSearch(symbolTable,top_N(n_stack));
					sprintf(polozka->key,"#_%d",varcount++);
					if(pom1->content.type != pom2->content.type){
						result = SEM_ERR2;							//chyba typové kompatibility
						return error;
					}
					polozka->type = tBool;
					polozka->isDefined = true;
					if (BTInsert(symbolTable, polozka->key, *polozka))
						result = SEM_ERR;
					ans = BTSearch(symbolTable,polozka->key);
					inst = createInst(I_LOE, pom2, pom1,(void*) ans);
					pop_N(n_stack);
					push_N(&s2, polozka->key);
					instListInsert(inst_list_global,inst);
					return item;
			}
			else
				return error;
		}
		
		case goe:{
			if(entry == nonterminal){
					if(stack->top->data != nonterminal){
						result = SYNTAX_ERR;			// neexistuje levá strana porovnání
						return error;
					}
					pop(stack);
					item = top(stack);
					push(stack, nonterminal);
					pom1 = BTSearch(symbolTable,top_N(n_stack));
					pop_N(n_stack);
					pom2 = BTSearch(symbolTable,top_N(n_stack));
					sprintf(polozka->key,"#_%d",varcount++);
					if(pom1->content.type != pom2->content.type){
						result = SEM_ERR2;							//chyba typové kompatibility
						return error;
					}
					polozka->type = tBool;
					polozka->isDefined = true;
					if (BTInsert(symbolTable, polozka->key, *polozka))
						result = SEM_ERR;
					ans = BTSearch(symbolTable,polozka->key);
					inst = createInst(I_GOE, pom2, pom1,(void*) ans);
					pop_N(n_stack);
					push_N(&s2, polozka->key);
					instListInsert(inst_list_global,inst);
					return item;
			}
			else
				return error;
		}
		
		case equal:{
			if(entry == nonterminal){
					if(stack->top->data != nonterminal){
						result = SYNTAX_ERR;			// neexistuje levá strana porovnání
						return error;
					}
					pop(stack);
					item = top(stack);
					push(stack, nonterminal);
					pom1 = BTSearch(symbolTable,top_N(n_stack));
					pop_N(n_stack);
					pom2 = BTSearch(symbolTable,top_N(n_stack));
					sprintf(polozka->key,"#_%d",varcount++);
					if(pom1->content.type != pom2->content.type){
						result = SEM_ERR2;							//chyba typové kompatibility
						return error;
					}
					polozka->type = tBool;
					polozka->isDefined = true;
					if (BTInsert(symbolTable, polozka->key, *polozka))
						result = SEM_ERR;
					ans = BTSearch(symbolTable,polozka->key);
					inst = createInst(I_EQUAL, pom2, pom1,(void*) ans);
					pop_N(n_stack);
					push_N(&s2, polozka->key);
					instListInsert(inst_list_global,inst);
					return item;
			}
			else{
				result = SYNTAX_ERR;			// neexistuje pravá strana porovnání
				return error;
			}
		}

		case notequal:{
			if(entry == nonterminal){
					if(stack->top->data != nonterminal){
						result = SYNTAX_ERR;			// neexistuje levá strana porovnání
						return error;
					}
					pop(stack);
					item = top(stack);
					push(stack, nonterminal);
					pom1 = BTSearch(symbolTable,top_N(n_stack));
					pop_N(n_stack);
					pom2 = BTSearch(symbolTable,top_N(n_stack));
					if(pom1->content.type != pom2->content.type){
						result = SEM_ERR2;							//chyba typové kompatibility
						return error;
					}					
					sprintf(polozka->key,"#_%d",varcount++);
					polozka->type = tBool;
					polozka->isDefined = true;
					if (BTInsert(symbolTable, polozka->key, *polozka))
						result = SEM_ERR;
					ans = BTSearch(symbolTable,polozka->key);
					inst = createInst(I_NOTEQUAL, pom2, pom1, (void*) ans);
					pop_N(n_stack);
					push_N(&s2, polozka->key);
					instListInsert(inst_list_global,inst);
					return item;
			}
			else{
				result = SYNTAX_ERR;			// neexistuje pravá strana porovnání
				return error;
			}
		}

		case id:{
			if(top(stack) == slesser){
				pop(stack);
				item = top(stack);
				push(stack,nonterminal);
				return item;
			
			}
			else{
				result = SYNTAX_ERR;
				return error;
			}
		}

		case rc:{
			if(top(stack)==nonterminal){
				pop(stack);
				if(top(stack)==lc){
					pop(stack);
					pop(stack);
					item = top(stack);
					push(stack, nonterminal);
					return item;
				}
				else{
					entry = nonterminal;
					return reduction(stack,n_stack,symbolTable);
				}
			}


		}

		case nonterminal:{
			entry = nonterminal;
			return reduction(stack,n_stack,symbolTable);
		}
		case lc:
		case error:
		case dolar:
		case nula:
		case slesser:{
			result = SYNTAX_ERR;			// neočekávaný znak
			return error;
		}

	
	}

	result = SYNTAX_ERR;
	return error;

}



		
int precedence(tToken *token, symbolTablePtr *symbolTable){
				// zasobnik pro ukladani terminalu a neterminalu

	tableEntries row;		// symbol na zasobniku (determinál)
	tableEntries col;		// symbol na vstupu

	stackInit(&s1);		// inicializace
	stackInit_N(&s2);

	row = dolar;
	push(&s1, row);


	do{
		col = getIndex(token, symbolTable);
		if (col == error){
			freeStack(&s1);										   	
			freeStack_N(&s2);
			return result;
		}



		switch (precedenceTable[row][col]){

		case B:{
				   	freeStack(&s1);			// pokud je v tabulce prazdno, jedna se o syntaktickou chybu								   	
					freeStack_N(&s2);
					return SYNTAX_ERR;
		}

		case G:{    						// Greater --> redukce
					row = reduction(&s1, &s2, symbolTable);
					if(row == error){	
						freeStack(&s1);
						freeStack_N(&s2);
						return result;
					}
					break;
		}
		case L:{							// Lesser --> < push 
					
					if(top(&s1) == nonterminal){

						if(col >= 4 && col <= 9){
							push(&s1, col);
							row = col;
						}
						else{
							pop(&s1);
					   		push(&s1, slesser);
					   		push(&s1, nonterminal);
					   		push(&s1, col);
					   		row = col;
				   		}
					}

					else{
					 	push(&s1, slesser);
						push(&s1, col);
						row = col;
					}
					*token = tGetToken();
					if (token->state == T_ERR){
						freeStack(&s1);
						freeStack_N(&s2);
						return LEX_ERR;
					}
					break;

		}

		case E:{
				push(&s1,col);
				row = col;
				*token = tGetToken();
				if (token->state == T_ERR){
					freeStack(&s1);
					freeStack_N(&s2);
					return LEX_ERR;
				}
			}
		}
	} while (row != dolar || col != dolar);

	freeStack(&s1);
	freeStack_N(&s2);
	return result;
}
