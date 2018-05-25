#include "scanner.h"
#include "garbage_collector.h"

void tInitToken(tToken *token){
	token->content = NULL;
	token->row = 0;				// potreba zavest globalni promennou, ktera bude urcovat na kterem jsme radku!
	token->state = T_START;		
	token->length = 0;
}

void tExtendToken(tToken *token, int c){
	token->content = (char*) gReAlloc(token->content,(sizeof(char) * (token->length + 2)));
	token->content[token->length] = c;
	token->content[++token->length] = '\0';

}

void *extendNumber(char *number, int c, int *length){

	if (number == NULL){
		if ((number = malloc(sizeof(char)* 2)) == NULL){
			return NULL;
		}
		number[0] = c;
		number[1] = '\0';
		*length = 1;
		return number;
	}
	else{
		if ((number = (char*)realloc(number, sizeof(char)*2 + *length+1)) == NULL){
			return NULL;
		}
		number[*length] = c;
		++(*length);
		number[*length] = '\0';
		
		return number;
	}
}

void freeNumber(char *number, int *length){
	*length = 0;
	free(number);
}


void tPutCharToStream(int c){		// pokud nactu dalsi znak a jiz nevyhovuje, musim jej "vratit" zpet pro dalsi cteni
	if (!(isspace(c))){
		ungetc(c, file);
	}
}

const char *KEYWORDS[20] = {		// Klicova slova
	"begin\0", "boolean\0", "do\0", "else\0", "end\0", "false\0", "find\0",
	"forward\0", "function\0", "if\0", "integer\0", "readln\0", "real\0",
	"sort\0", "string\0", "then\0", "true\0", "var\0", "while\0", "write\0"
};

tState checkKeyWord(tToken token){	
	for (int x = 0; x < 20; x++){
		if (!strcmp(KEYWORDS[x], token.content)){
		return T_KEYWORD;
		}
	}
	return T_IDENTIFICATOR;
}

tToken tGetToken(){

	tToken token;
	tState state = T_START;			// stav automatu
	int c;							// promenna, do ktere nacitam znaky
	tInitToken(&token);

	char *number = NULL;
	int length = 0;

	while (1){

		c = getc(file);

		switch (state){

			case T_START:{

								 if (isalpha(c) || c == '_'){
									 if (c >= 'A' && c <= 'Z')
										 c = tolower(c);				// interpret je case insensitive, vsechny znaky prevedu na male
									state = T_IDENTIFICATOR;
								 }
								 else if (c == ':'){
									 state = T_COLON;
								 }
								 else if (c == ';'){
									 state = T_SEMICOLON;
								 }
								 else if (c == '('){
									 state = T_LC;
								 }
								 else if (c == ')'){
									 state = T_RC;
								 }
								 else if (c == '/'){
									 state = T_DIV;
								 }
								 else if (c == '*'){
									 state = T_MUL;
								 }
								 else if (c == '-'){
									 state = T_MINUS;
								 }
								 else if (c == '+'){
									 state = T_PLUS;
								 }
								 else if (c == '='){
									 state = T_EQUAL;
								 }
								 else if (c == ','){
									 state = T_COMMA;
								 }
								 else if (c == '<'){
									 state = T_LESSER;
								 }
								 else if (c == '>'){
									 state = T_GREATER;
								 }
								 else if (c == '.'){
									 state = T_DOT;
								 }
								 else if (isdigit(c)){
									 state = T_INTEGER;
								 }
								 else if (c == '\''){
									 state = T_STRING;
									 break;
								 }
								 else if (c == '{'){
									 state = T_COMMENT;
									 break;
								 }
								 else if (isspace(c)){
									 state = T_START;
									 break;
								 }
								 else if (c == EOF){
									 token.state = T_EOF;	 
									 return token;
								 }
								 else{
									 token.state = T_ERR;
									 return token;
								
								 }


								 tExtendToken(&token, c);
								 break;

			}

			case T_IDENTIFICATOR:{

								 if (isalpha(c) || isdigit(c) || c == '_'){
									 if (c >= 'A' && c <= 'Z')
										 c = tolower(c);				// interpret je case insensitive, vsechny znaky prevedu na male
									 tExtendToken(&token, c);
									 break;
								 }
								 else{
									 token.state = checkKeyWord(token);
									 tPutCharToStream(c);
									 return token;
								 }

			}
			case T_COLON:{
							 if (c == '='){
								 state = T_ASSIGN;
								 tExtendToken(&token, c);
							 }
							 else{
								 token.state = T_COLON;
								 tPutCharToStream(c);
								 return token;
							 }
								 break;
			}

			case T_STRING:{
							  if (c == EOF){
								  token.state = T_ERR;
								  return token;
							  }

							  if (c == '\''){
								  state = T_STRING1;
								  break;
							  }
							  else{
								  tExtendToken(&token, c);
							  }
							  break;

			}

			case T_STRING1:{
							   if (c == '#'){
								   state = T_STRING2;
								   break;
							   }
							   else if (c == '\''){
								   tExtendToken(&token, c);
								   state = T_STRING;
								   break;
							   }
							   else{
								   token.state = T_STRING;
								   tPutCharToStream(c);
								   return token;
							   }
			}

			case T_STRING2:{
							   if (c == '0'){
								   state = T_STRING2;
								   break;
							   }
							   else if (c >= '1' && c <= '9'){
								   number = extendNumber(number, c, &length);
								   state = T_STRING3;
								   break;
							   }
			}
			case T_STRING3:{
							   if (isdigit(c)){
								   number = extendNumber(number, c, &length);
								   state = T_STRING3;
								   break;
							   }
							   else if (c == '\''){
								   int tmp = strtol(number, NULL,0);
								   if (tmp >= 1 && tmp <= 255){
										tExtendToken(&token, tmp);
										freeNumber(number, &length);
										number = NULL;
										state = T_STRING;
										break;
								   }
								   else{
									   freeNumber(number, &length);
									   number = NULL;
									   token.state = T_ERR;
									   return token;
								   }
							   }
							   else{
								   token.state = T_ERR;
								   return token;
							   }

			}

			case T_LESSER:{
							  if (c == '='){
								  state = T_LOE;
								  tExtendToken(&token, c);
							  }
							  else if (c == '>'){
								  state = T_NOTEQUAL;
								  tExtendToken(&token, c);
							  }
							  else{
								  token.state = T_LESSER;
								  tPutCharToStream(c);
								  return token;
							  }
							  break;
			}
			case T_GREATER:{
							  if (c == '='){
								  state = T_GOE;
								  tExtendToken(&token, c);
							  }
							  else{
								  token.state = T_GREATER;
								  tPutCharToStream(c);
								  return token;
							  }
							  break;
							  
			}
			case T_INTEGER:{
							   if (isdigit(c)){
								   tExtendToken(&token, c);
								   break;
							   }
							   else if (c == '.'){
								   state = T_REAL1;
								   tExtendToken(&token, c);
								   break;
							   }
							   else{
								   token.state = T_INTEGER;
								   tPutCharToStream(c);
								   return token;
							   }
			}
			case T_REAL1:{
							 if (!isdigit(c)){
								 token.state = T_ERR;
								 return token;
							 }
							 else{
								 state = T_REAL;
								 tExtendToken(&token, c);
								 break;
							 }
			}
			case T_REAL:{
							if (isdigit(c)){
								tExtendToken(&token, c);
								break;
							}
							if (c == 'e'){
								state = T_EXPREAL0;
								tExtendToken(&token, c);
								break;
							}
							else{
								token.state = T_REAL;
								tPutCharToStream(c);
								return token;
							}
			}
			case T_EXPREAL0:{
								if (c == '+'){
									state = T_EXPREAL;
									tExtendToken(&token, c);
									break;
								}
								else if (c == '-'){
									state = T_EXPREAL;
									tExtendToken(&token, c);
									break;
								}
								else if (isdigit(c)){
									state = T_EXPREAL1;
									tExtendToken(&token, c);
									break;
								}
								else if (c == EOF){
									token.state = T_ERR;
									return token;
								}
								else{
									token.state = T_ERR;
									return token;
								}
			}

			case T_EXPREAL:{
							   if (!isdigit(c)){
								   token.state = T_ERR;
								   return token;
							   }
							   else{
								   state = T_EXPREAL1;
								   tExtendToken(&token, c);
								   break;
							   }
			}

			case T_EXPREAL1:{
								if (isdigit(c)){
									tExtendToken(&token, c);
									break;
								}
								else{
									token.state = T_REAL;
									tPutCharToStream(c);
									return token;
								}
			}
			case T_COMMENT:{
							   if (c == '}'){
								   state = T_START;
							   }
							   if (c == EOF){
								   token.state = T_ERR;
								   return token;
							   }
							   break;
			}


			// konecne stavy
			case T_ASSIGN:
			case T_SEMICOLON:
			case T_LC:
			case T_RC:
			case T_DIV:
			case T_MUL:
			case T_MINUS:
			case T_PLUS:
			case T_EQUAL:
			case T_COMMA:
			case T_NOTEQUAL:
			case T_LOE:
			case T_GOE:
			case T_DOT:{
						  
								 token.state = state;
								 tPutCharToStream(c);
								 return token;
								 

			}

			default:{
				break;
			}

		}
	}
}