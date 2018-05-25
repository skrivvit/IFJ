#include "interpret.h"
#include <string.h>


int prepareToRead(){
	int c = getchar();
	while (c != EOF && isspace(c)){
		if (c == '\n')
			return c;
		c = getchar();
	}

	if (c == EOF)
		return c;
	if (!isspace(c))
		ungetc(c, stdin);
	return 0;
}

int cleanAfterRead(){
	int c = getchar();
	while (c != EOF  &&  c != '\n'){
		c = getchar();
	}
	if (c == EOF)
		return EOF;
	return 0;



}

int getSign(){
	int c = getchar();
	if (c == '-')
		return -1;
	else
		ungetc(c, stdin);
	return 1;
}



int readInt(int *retval){
	*retval = 0;
	int sign = getSign();
	int c = getchar();
	char vysl[40];
	int i = 0;
	vysl[i] = 0;
	while (c != EOF && !isspace(c)){
		vysl[i] = c;
		i++;
		if (c == '.' || c == 'e'){
			result = RUN_ERR;
			break;
		}
		if (!isdigit(c)){
			printf("ahoj\n");
			result = RUN_ERR;
			break;
		}
		if (i == 39){
			result = INT_ERR;
			break;
		}
		c = getchar();
	}
	if (c == '\n' || c == EOF)
		ungetc(c, stdin);
	vysl[i] = '\0';
	if (i == 0)
		result = RUN_ERR;
	else
		*retval = sign * strtol(vysl, NULL, 0);

	return result;


}



int readReal(double *retval){
	*retval = 0;
	bool dot = false;
	bool e = false;
	int sign = getSign();
	int c = getchar();
	char vysl[40];
	int i = 0;
	int ctrli = 0;
	int ctrle = 0;
	vysl[i] = 0;

	while (c != EOF && !isspace(c)){
		vysl[i] = c;
		if (c == '.'){
			if (dot || e){
				result = RUN_ERR;
				break;
			}
			dot = true;
			ctrli = i;
		}
		else if (c == 'e'){
			if (e || !dot){
				result = RUN_ERR;
				break;
			}
			e = true;
			ctrle = i;
		}
		else if (!isdigit(c)){
			result = RUN_ERR;
			break;
		}
		i++;


		if (i == 39){
			result = INT_ERR;
			break;
		}
		c = getchar();
	}
	if (c == '\n' || c == EOF)
		ungetc(c, stdin);
	vysl[i] = '\0';

	if (ctrli == 0 && ctrle == 0)
		result = RUN_ERR;
	if (i == 0 || ctrli == i || ctrle == i)
		result = RUN_ERR;
	if (!dot && !e)
		result = RUN_ERR;
	if (dot && e && (ctrle - ctrli == 1 || ctrle - ctrli == -1))
		result = RUN_ERR;

	if (result == SYNTAX_OK)
		*retval = sign * strtod(vysl, 0);

	return result;


}

int readString(char **retval){
	int c = getchar();
	int CUR_MAX = 666;
	char *buffer = (char*)gMalloc(CUR_MAX);
	int count = 0;
	int length = 0;

	while ((c != '\n') && (c != EOF)) {
		if (count == CUR_MAX) {
			CUR_MAX *= 2;
			buffer = gReAlloc(buffer, CUR_MAX);
		}
		buffer[length] = c;
		length++;
		count++;
		c = getchar();
	}
	buffer[length] = '\0';
	if (length == 0)
		return RUN_ERR;
	*retval = buffer;
	return result;
}


int interpr(tInstList instlist){


	while (1)
	{
		instListSucc(&instlist);
		tInst tmp = instlist.Active->instruction;


		switch (tmp.type)
		{
		case I_STOP:
			return 0;
			break;

		case I_ADD: {
						if (((((symbolTablePtr)tmp.addr1)->content.isDefined) != true) || ((((symbolTablePtr)tmp.addr2)->content.isDefined) != true))
							return RUN_ERR2;
						if (((symbolTablePtr)tmp.addr1)->content.type == tInt){
							if (((symbolTablePtr)tmp.addr2)->content.type == tInt)
								((symbolTablePtr)tmp.addr3)->content.value.i = ((symbolTablePtr)tmp.addr1)->content.value.i + ((symbolTablePtr)tmp.addr2)->content.value.i;
							else
								((symbolTablePtr)tmp.addr3)->content.value.r = ((symbolTablePtr)tmp.addr1)->content.value.i + ((symbolTablePtr)tmp.addr2)->content.value.r;
						}
						else if (((symbolTablePtr)tmp.addr1)->content.type == tReal){
							if (((symbolTablePtr)tmp.addr2)->content.type == tInt)
								((symbolTablePtr)tmp.addr3)->content.value.r = ((symbolTablePtr)tmp.addr1)->content.value.r + ((symbolTablePtr)tmp.addr2)->content.value.i;
							else
								((symbolTablePtr)tmp.addr3)->content.value.r = ((symbolTablePtr)tmp.addr1)->content.value.r + ((symbolTablePtr)tmp.addr2)->content.value.r;
						}
						break;
		}

		case I_SUB: {
						if (((((symbolTablePtr)tmp.addr1)->content.isDefined) != true) || ((((symbolTablePtr)tmp.addr2)->content.isDefined) != true))
							return RUN_ERR2;
						if (((symbolTablePtr)tmp.addr1)->content.type == tInt){
							if (((symbolTablePtr)tmp.addr2)->content.type == tInt)
								((symbolTablePtr)tmp.addr3)->content.value.i = ((symbolTablePtr)tmp.addr1)->content.value.i - ((symbolTablePtr)tmp.addr2)->content.value.i;
							else
								((symbolTablePtr)tmp.addr3)->content.value.r = ((symbolTablePtr)tmp.addr1)->content.value.i - ((symbolTablePtr)tmp.addr2)->content.value.r;
						}
						else if (((symbolTablePtr)tmp.addr1)->content.type == tReal){
							if (((symbolTablePtr)tmp.addr2)->content.type == tInt)
								((symbolTablePtr)tmp.addr3)->content.value.r = ((symbolTablePtr)tmp.addr1)->content.value.r - ((symbolTablePtr)tmp.addr2)->content.value.i;
							else
								((symbolTablePtr)tmp.addr3)->content.value.r = ((symbolTablePtr)tmp.addr1)->content.value.r - ((symbolTablePtr)tmp.addr2)->content.value.r;
						}
						break;
		}

		case I_MUL: {
						if (((((symbolTablePtr)tmp.addr1)->content.isDefined) != true) || ((((symbolTablePtr)tmp.addr2)->content.isDefined) != true))
							return RUN_ERR2;
						if (((symbolTablePtr)tmp.addr1)->content.type == tInt){
							if (((symbolTablePtr)tmp.addr2)->content.type == tInt)
								((symbolTablePtr)tmp.addr3)->content.value.i = ((symbolTablePtr)tmp.addr1)->content.value.i * ((symbolTablePtr)tmp.addr2)->content.value.i;
							else
								((symbolTablePtr)tmp.addr3)->content.value.r = ((symbolTablePtr)tmp.addr1)->content.value.i * ((symbolTablePtr)tmp.addr2)->content.value.r;
						}
						else if (((symbolTablePtr)tmp.addr1)->content.type == tReal){
							if (((symbolTablePtr)tmp.addr2)->content.type == tInt)
								((symbolTablePtr)tmp.addr3)->content.value.r = ((symbolTablePtr)tmp.addr1)->content.value.r * ((symbolTablePtr)tmp.addr2)->content.value.i;
							else
								((symbolTablePtr)tmp.addr3)->content.value.r = ((symbolTablePtr)tmp.addr1)->content.value.r * ((symbolTablePtr)tmp.addr2)->content.value.r;
						}
						break;
		}

		case I_DIV: {
						if (((((symbolTablePtr)tmp.addr1)->content.isDefined) != true) || ((((symbolTablePtr)tmp.addr2)->content.isDefined) != true))
							return RUN_ERR2;
						if (((symbolTablePtr)tmp.addr2)->content.value.i == 0)
							return RUN_ERR3;
						if (((symbolTablePtr)tmp.addr1)->content.type == tInt){
							if (((symbolTablePtr)tmp.addr2)->content.type == tInt)
								((symbolTablePtr)tmp.addr3)->content.value.r = ((symbolTablePtr)tmp.addr1)->content.value.i / ((symbolTablePtr)tmp.addr2)->content.value.i;
							else
								((symbolTablePtr)tmp.addr3)->content.value.r = ((symbolTablePtr)tmp.addr1)->content.value.i / ((symbolTablePtr)tmp.addr2)->content.value.r;
						}
						else if (((symbolTablePtr)tmp.addr1)->content.type == tReal){
							if (((symbolTablePtr)tmp.addr2)->content.type == tInt)
								((symbolTablePtr)tmp.addr3)->content.value.r = ((symbolTablePtr)tmp.addr1)->content.value.r / ((symbolTablePtr)tmp.addr2)->content.value.i;
							else
								((symbolTablePtr)tmp.addr3)->content.value.r = ((symbolTablePtr)tmp.addr1)->content.value.r / ((symbolTablePtr)tmp.addr2)->content.value.r;
						}
						break;
		}

		case I_CONC: {
						 if (((((symbolTablePtr)tmp.addr1)->content.isDefined) != true) || ((((symbolTablePtr)tmp.addr2)->content.isDefined) != true))
							 return RUN_ERR2;

						 if ((((symbolTablePtr)tmp.addr1))->content.value.s == NULL && (((symbolTablePtr)tmp.addr2))->content.value.s != NULL){
							 ((symbolTablePtr)tmp.addr3)->content.value.s = ((symbolTablePtr)tmp.addr2)->content.value.s;
							 break;
						 }
						 if ((((symbolTablePtr)tmp.addr2))->content.value.s == NULL && (((symbolTablePtr)tmp.addr1))->content.value.s != NULL){
							 ((symbolTablePtr)tmp.addr3)->content.value.s = ((symbolTablePtr)tmp.addr1)->content.value.s;
							 break;
						 }
						 if ((((symbolTablePtr)tmp.addr2))->content.value.s == NULL && (((symbolTablePtr)tmp.addr1))->content.value.s == NULL){
							 ((symbolTablePtr)tmp.addr3)->content.value.s = NULL;
							 break;
						 }
						 if (tmp.addr1 == tmp.addr2){
							 char* tmpstr = malloc(strlen(((symbolTablePtr)tmp.addr2)->content.value.s) + 1);
							 strcpy(tmpstr, ((symbolTablePtr)tmp.addr2)->content.value.s);
							 ((symbolTablePtr)tmp.addr1)->content.value.s = gReAlloc(((symbolTablePtr)tmp.addr1)->content.value.s, strlen(((symbolTablePtr)tmp.addr1)->content.value.s)*2  + 1);
							 ((symbolTablePtr)tmp.addr3)->content.value.s = strcat(((symbolTablePtr)tmp.addr1)->content.value.s, tmpstr);
							 free(tmpstr);

						 }
						 else{
							 ((symbolTablePtr)tmp.addr1)->content.value.s = gReAlloc(((symbolTablePtr)tmp.addr1)->content.value.s, strlen(((symbolTablePtr)tmp.addr1)->content.value.s) + strlen(((symbolTablePtr)tmp.addr2)->content.value.s) + 1);
							 ((symbolTablePtr)tmp.addr3)->content.value.s = strcat(((symbolTablePtr)tmp.addr1)->content.value.s, ((symbolTablePtr)tmp.addr2)->content.value.s);
						 }
						 break;
		}


		case I_LESSER: {
						   if (((((symbolTablePtr)tmp.addr1)->content.isDefined) != true) || ((((symbolTablePtr)tmp.addr2)->content.isDefined) != true))
							   return RUN_ERR2;
						   if ((((symbolTablePtr)tmp.addr1)->content.type == tString) && (((symbolTablePtr)tmp.addr2)->content.type == tString)){
							   if (strcmp(((symbolTablePtr)tmp.addr1)->content.value.s, ((symbolTablePtr)tmp.addr2)->content.value.s) < 0){
								   ((symbolTablePtr)tmp.addr3)->content.value.b = true;
								   break;
							   }
							   else{
								   ((symbolTablePtr)tmp.addr3)->content.value.b = false;
								   break;
							   }
						   }

						   if ((((symbolTablePtr)tmp.addr1)->content.type == tInt) && (((symbolTablePtr)tmp.addr2)->content.type == tInt)){
							   if (((symbolTablePtr)tmp.addr1)->content.value.i < ((symbolTablePtr)tmp.addr2)->content.value.i){
								   ((symbolTablePtr)tmp.addr3)->content.value.b = true;
								   break;
							   }
							   else{
								   ((symbolTablePtr)tmp.addr3)->content.value.b = false;
								   break;
							   }
						   }

						   if ((((symbolTablePtr)tmp.addr1)->content.type == tBool) && (((symbolTablePtr)tmp.addr2)->content.type == tBool)){
							   if (((symbolTablePtr)tmp.addr1)->content.value.r < ((symbolTablePtr)tmp.addr2)->content.value.r){
								   ((symbolTablePtr)tmp.addr3)->content.value.b = true;
								   break;
							   }
							   else{
								   ((symbolTablePtr)tmp.addr3)->content.value.b = false;
								   break;
							   }
						   }

						   if ((((symbolTablePtr)tmp.addr1)->content.type == tReal) && (((symbolTablePtr)tmp.addr2)->content.type == tReal)){
							   if (((symbolTablePtr)tmp.addr1)->content.value.r < ((symbolTablePtr)tmp.addr2)->content.value.r){
								   ((symbolTablePtr)tmp.addr3)->content.value.b = true;
								   break;
							   }
							   else{
								   ((symbolTablePtr)tmp.addr3)->content.value.b = false;
								   break;
							   }
						   }
						   return SEM_ERR2;
						   break;
		}

		case I_GREATER: {
							if (((((symbolTablePtr)tmp.addr1)->content.isDefined) != true) || ((((symbolTablePtr)tmp.addr2)->content.isDefined) != true))
								return RUN_ERR2;
							if ((((symbolTablePtr)tmp.addr1)->content.type == tString) && (((symbolTablePtr)tmp.addr2)->content.type == tString)){
								if (strcmp(((symbolTablePtr)tmp.addr1)->content.value.s, ((symbolTablePtr)tmp.addr2)->content.value.s) > 0){
									((symbolTablePtr)tmp.addr3)->content.value.b = true;
									break;
								}
								else{
									((symbolTablePtr)tmp.addr3)->content.value.b = false;
									break;
								}
							}

							if ((((symbolTablePtr)tmp.addr1)->content.type == tInt) && (((symbolTablePtr)tmp.addr2)->content.type == tInt)){
								if (((symbolTablePtr)tmp.addr1)->content.value.i > ((symbolTablePtr)tmp.addr2)->content.value.i){
									((symbolTablePtr)tmp.addr3)->content.value.b = true;
									break;
								}
								else{
									((symbolTablePtr)tmp.addr3)->content.value.b = false;
									break;
								}
							}

							if ((((symbolTablePtr)tmp.addr1)->content.type == tBool) && (((symbolTablePtr)tmp.addr2)->content.type == tBool)){
								if (((symbolTablePtr)tmp.addr1)->content.value.r > ((symbolTablePtr)tmp.addr2)->content.value.r){
									((symbolTablePtr)tmp.addr3)->content.value.b = true;
									break;
								}
								else{
									((symbolTablePtr)tmp.addr3)->content.value.b = false;
									break;
								}
							}

							if ((((symbolTablePtr)tmp.addr1)->content.type == tReal) && (((symbolTablePtr)tmp.addr2)->content.type == tReal)){
								if (((symbolTablePtr)tmp.addr1)->content.value.r > ((symbolTablePtr)tmp.addr2)->content.value.r){
									((symbolTablePtr)tmp.addr3)->content.value.b = true;
									break;
								}
								else{
									((symbolTablePtr)tmp.addr3)->content.value.b = false;
									break;
								}
							}
							return SEM_ERR2;

							break;
		}

		case I_LOE: {
						if (((((symbolTablePtr)tmp.addr1)->content.isDefined) != true) || ((((symbolTablePtr)tmp.addr2)->content.isDefined) != true))
							return RUN_ERR2;
						if ((((symbolTablePtr)tmp.addr1)->content.type == tString) && (((symbolTablePtr)tmp.addr2)->content.type == tString)){
							if (strcmp(((symbolTablePtr)tmp.addr1)->content.value.s, ((symbolTablePtr)tmp.addr2)->content.value.s) <= 0){
								((symbolTablePtr)tmp.addr3)->content.value.b = true;
								break;
							}
							else{
								((symbolTablePtr)tmp.addr3)->content.value.b = false;
								break;
							}
						}

						if ((((symbolTablePtr)tmp.addr1)->content.type == tInt) && (((symbolTablePtr)tmp.addr2)->content.type == tInt)){
							if (((symbolTablePtr)tmp.addr1)->content.value.i <= ((symbolTablePtr)tmp.addr2)->content.value.i){
								((symbolTablePtr)tmp.addr3)->content.value.b = true;
								break;
							}
							else{
								((symbolTablePtr)tmp.addr3)->content.value.b = false;
								break;
							}
						}

						if ((((symbolTablePtr)tmp.addr1)->content.type == tBool) && (((symbolTablePtr)tmp.addr2)->content.type == tBool)){
							if (((symbolTablePtr)tmp.addr1)->content.value.r <= ((symbolTablePtr)tmp.addr2)->content.value.r){
								((symbolTablePtr)tmp.addr3)->content.value.b = true;
								break;
							}
							else{
								((symbolTablePtr)tmp.addr3)->content.value.b = false;
								break;
							}
						}

						if ((((symbolTablePtr)tmp.addr1)->content.type == tReal) && (((symbolTablePtr)tmp.addr2)->content.type == tReal)){
							if (((symbolTablePtr)tmp.addr1)->content.value.r <= ((symbolTablePtr)tmp.addr2)->content.value.r){
								((symbolTablePtr)tmp.addr3)->content.value.b = true;
								break;
							}
							else{
								((symbolTablePtr)tmp.addr3)->content.value.b = false;
								break;
							}
						}
						break;
		}

		case I_GOE:{
					   if (((((symbolTablePtr)tmp.addr1)->content.isDefined) != true) || ((((symbolTablePtr)tmp.addr2)->content.isDefined) != true))
						   return RUN_ERR2;
					   if ((((symbolTablePtr)tmp.addr1)->content.type == tString) && (((symbolTablePtr)tmp.addr2)->content.type == tString)){
						   if (strcmp(((symbolTablePtr)tmp.addr1)->content.value.s, ((symbolTablePtr)tmp.addr2)->content.value.s) >= 0){
							   ((symbolTablePtr)tmp.addr3)->content.value.b = true;
							   break;
						   }
						   else{
							   ((symbolTablePtr)tmp.addr3)->content.value.b = false;
							   break;
						   }
					   }

					   if ((((symbolTablePtr)tmp.addr1)->content.type == tInt) && (((symbolTablePtr)tmp.addr2)->content.type == tInt)){
						   if (((symbolTablePtr)tmp.addr1)->content.value.i >= ((symbolTablePtr)tmp.addr2)->content.value.i){
							   ((symbolTablePtr)tmp.addr3)->content.value.b = true;
							   break;
						   }
						   else{
							   ((symbolTablePtr)tmp.addr3)->content.value.b = false;
							   break;
						   }
					   }

					   if ((((symbolTablePtr)tmp.addr1)->content.type == tBool) && (((symbolTablePtr)tmp.addr2)->content.type == tBool)){
						   if (((symbolTablePtr)tmp.addr1)->content.value.r >= ((symbolTablePtr)tmp.addr2)->content.value.r){
							   ((symbolTablePtr)tmp.addr3)->content.value.b = true;
							   break;
						   }
						   else{
							   ((symbolTablePtr)tmp.addr3)->content.value.b = false;
							   break;
						   }
					   }

					   if ((((symbolTablePtr)tmp.addr1)->content.type == tReal) && (((symbolTablePtr)tmp.addr2)->content.type == tReal)){
						   if (((symbolTablePtr)tmp.addr1)->content.value.r >= ((symbolTablePtr)tmp.addr2)->content.value.r){
							   ((symbolTablePtr)tmp.addr3)->content.value.b = true;
							   break;
						   }
						   else{
							   ((symbolTablePtr)tmp.addr3)->content.value.b = false;
							   break;
						   }
					   }
					   return SEM_ERR2;
					   break;
		}
		case I_NOTEQUAL:{
							if (((((symbolTablePtr)tmp.addr1)->content.isDefined) != true) || ((((symbolTablePtr)tmp.addr2)->content.isDefined) != true))
								return RUN_ERR2;
							if ((((symbolTablePtr)tmp.addr1)->content.type == tString) && (((symbolTablePtr)tmp.addr2)->content.type == tString)){
								if (strcmp(((symbolTablePtr)tmp.addr1)->content.value.s, ((symbolTablePtr)tmp.addr2)->content.value.s) != 0){
									((symbolTablePtr)tmp.addr3)->content.value.b = true;
									break;
								}
								else{
									((symbolTablePtr)tmp.addr3)->content.value.b = false;
									break;
								}
							}

							if ((((symbolTablePtr)tmp.addr1)->content.type == tInt) && (((symbolTablePtr)tmp.addr2)->content.type == tInt)){
								if (((symbolTablePtr)tmp.addr1)->content.value.i != ((symbolTablePtr)tmp.addr2)->content.value.i){
									((symbolTablePtr)tmp.addr3)->content.value.b = true;
									break;
								}
								else{
									((symbolTablePtr)tmp.addr3)->content.value.b = false;
									break;
								}
							}

							if ((((symbolTablePtr)tmp.addr1)->content.type == tBool) && (((symbolTablePtr)tmp.addr2)->content.type == tBool)){
								if (((symbolTablePtr)tmp.addr1)->content.value.r != ((symbolTablePtr)tmp.addr2)->content.value.r){
									((symbolTablePtr)tmp.addr3)->content.value.b = true;
									break;
								}
								else{
									((symbolTablePtr)tmp.addr3)->content.value.b = false;
									break;
								}
							}

							if ((((symbolTablePtr)tmp.addr1)->content.type == tReal) && (((symbolTablePtr)tmp.addr2)->content.type == tReal)){
								if (((symbolTablePtr)tmp.addr1)->content.value.r != ((symbolTablePtr)tmp.addr2)->content.value.r){
									((symbolTablePtr)tmp.addr3)->content.value.b = true;
									break;
								}
								else{
									((symbolTablePtr)tmp.addr3)->content.value.b = false;
									break;
								}
							}
							return SEM_ERR2;
							break;
		}


		case I_EQUAL:{
						 if (((((symbolTablePtr)tmp.addr1)->content.isDefined) != true) || ((((symbolTablePtr)tmp.addr2)->content.isDefined) != true))
							 return RUN_ERR2;
						 if ((((symbolTablePtr)tmp.addr1)->content.type == tString) && (((symbolTablePtr)tmp.addr2)->content.type == tString)){
							 if (strcmp(((symbolTablePtr)tmp.addr1)->content.value.s, ((symbolTablePtr)tmp.addr2)->content.value.s) == 0){
								 ((symbolTablePtr)tmp.addr3)->content.value.b = true;
								 break;
							 }
							 else{
								 ((symbolTablePtr)tmp.addr3)->content.value.b = false;
								 break;
							 }
						 }

						 if ((((symbolTablePtr)tmp.addr1)->content.type == tInt) && (((symbolTablePtr)tmp.addr2)->content.type == tInt)){
							 if (((symbolTablePtr)tmp.addr1)->content.value.i == ((symbolTablePtr)tmp.addr2)->content.value.i){
								 ((symbolTablePtr)tmp.addr3)->content.value.b = true;
								 break;
							 }
							 else{
								 ((symbolTablePtr)tmp.addr3)->content.value.b = false;
								 break;
							 }
						 }

						 if ((((symbolTablePtr)tmp.addr1)->content.type == tBool) && (((symbolTablePtr)tmp.addr2)->content.type == tBool)){
							 if (((symbolTablePtr)tmp.addr1)->content.value.r == ((symbolTablePtr)tmp.addr2)->content.value.r){
								 ((symbolTablePtr)tmp.addr3)->content.value.b = true;
								 break;
							 }
							 else{
								 ((symbolTablePtr)tmp.addr3)->content.value.b = false;
								 break;
							 }
						 }

						 if ((((symbolTablePtr)tmp.addr1)->content.type == tReal) && (((symbolTablePtr)tmp.addr2)->content.type == tReal)){
							 if (((symbolTablePtr)tmp.addr1)->content.value.r == ((symbolTablePtr)tmp.addr2)->content.value.r){
								 ((symbolTablePtr)tmp.addr3)->content.value.b = true;
								 break;
							 }
							 else{
								 ((symbolTablePtr)tmp.addr3)->content.value.b = false;
								 break;
							 }
						 }
						 return SEM_ERR2;
						 break;
		}

		case I_READLN: {
						   int i;
						   double r;
						   char* s;
						   if (((symbolTablePtr)tmp.addr3)->content.type == tInt){
							   prepareToRead();
							   result = readInt(&i);
							   cleanAfterRead();
							   if (result != SYNTAX_OK)
								   return result;
							   else{
								   ((symbolTablePtr)tmp.addr3)->content.value.i = i;
								   ((symbolTablePtr)tmp.addr3)->content.isDefined = true;
							   }
							   break;
						   }
						   if (((symbolTablePtr)tmp.addr3)->content.type == tReal){
							   prepareToRead();
							   result = readReal(&r);
							   cleanAfterRead();
							   if (result != SYNTAX_OK)
								   return result;
							   else{
								   ((symbolTablePtr)tmp.addr3)->content.value.r = r;
								   ((symbolTablePtr)tmp.addr3)->content.isDefined = true;
							   }
							   break;
						   }
						   if (((symbolTablePtr)tmp.addr3)->content.type == tString){
							   result = readString(&s);
							   if (result != SYNTAX_OK)
								   return result;
							   else{
								   ((symbolTablePtr)tmp.addr3)->content.value.s = s;
								   ((symbolTablePtr)tmp.addr3)->content.isDefined = true;
							   }
							   break;
						   }
						   break;
		}
		case I_ASSIGN: {
						   if ((((symbolTablePtr)tmp.addr1)->content.isDefined) != true)
							   return RUN_ERR2;
						   if (((symbolTablePtr)tmp.addr1)->content.type == tInt){
							   ((symbolTablePtr)tmp.addr3)->content.value.i = ((symbolTablePtr)tmp.addr1)->content.value.i;
							   ((symbolTablePtr)tmp.addr3)->content.isDefined = true;
						   }
						   if (((symbolTablePtr)tmp.addr1)->content.type == tReal){
							   ((symbolTablePtr)tmp.addr3)->content.value.r = ((symbolTablePtr)tmp.addr1)->content.value.r;
							   ((symbolTablePtr)tmp.addr3)->content.isDefined = true;
						   }
						   if (((symbolTablePtr)tmp.addr1)->content.type == tString){
							   ((symbolTablePtr)tmp.addr3)->content.value.s = ((symbolTablePtr)tmp.addr1)->content.value.s;
							   ((symbolTablePtr)tmp.addr3)->content.isDefined = true;
						   }
						   if (((symbolTablePtr)tmp.addr1)->content.type == tBool){
							   ((symbolTablePtr)tmp.addr3)->content.value.b = ((symbolTablePtr)tmp.addr1)->content.value.b;
							   ((symbolTablePtr)tmp.addr3)->content.isDefined = true;
						   }
						   break;

		}

		case I_WRITE:{
						 if ((((symbolTablePtr)tmp.addr1)->content.isDefined) != true)
							 return RUN_ERR2;
						 if (((symbolTablePtr)tmp.addr1)->content.type == tInt){
							 printf("%d", ((symbolTablePtr)tmp.addr1)->content.value.i);
						 }
						 if (((symbolTablePtr)tmp.addr1)->content.type == tReal){
							 printf("%g", ((symbolTablePtr)tmp.addr1)->content.value.r);
						 }
						 if (((symbolTablePtr)tmp.addr1)->content.type == tString){
							 if (((symbolTablePtr)tmp.addr1)->content.value.s == NULL){
								 break;
							 }
							 else{
								 printf("%s", ((symbolTablePtr)tmp.addr1)->content.value.s);
							 }
						 }
						 if (((symbolTablePtr)tmp.addr1)->content.type == tBool){
							 if (((symbolTablePtr)tmp.addr1)->content.value.b == true){
								 printf("TRUE");
							 }
							 else if (((symbolTablePtr)tmp.addr1)->content.value.b == false){
								 printf("FALSE");
							 }
						 }

						 break;
		}

			case I_IFGOTO:{
						  if ((((symbolTablePtr)tmp.addr1)->content.isDefined) != true)
							  return RUN_ERR2;
						  if (((symbolTablePtr)tmp.addr1)->content.value.b == false){
							  instlist.Active = ((tItem*)tmp.addr2);
							  break;
						  }

						  break;
			}
			case I_GOTO:{
							instlist.Active = ((tItem*)tmp.addr1);
							break;
			}
			case I_CALL:{
							if (((symbolTablePtr)tmp.addr1)->content.isDefined == false)
								return SEM_ERR;
							result = interpr(((symbolTablePtr)tmp.addr1)->content.lInstList);
							if (result != SYNTAX_OK)
								return result;
							break;
			}
			case I_LENGTH:{
							  if (((symbolTablePtr)tmp.addr1)->content.isDefined == false)
								  return RUN_ERR2;
							  ((symbolTablePtr)tmp.addr3)->content.value.i = length(((symbolTablePtr)tmp.addr1)->content.value.s);
							  ((symbolTablePtr)tmp.addr3)->content.isDefined = true;
							  break;
			}
			case I_COPY : {
							  if (((symbolTablePtr)tmp.addr1)->content.isDefined == false)
								  return RUN_ERR2;
							  if (((copyParams)tmp.addr2)->param1->content.isDefined == false)
								  return RUN_ERR2;
							  if (((copyParams)tmp.addr2)->param2->content.isDefined == false)
								  return RUN_ERR2;

							  if (((symbolTablePtr)tmp.addr1)->content.value.s == NULL){
								  ((symbolTablePtr)tmp.addr3)->content.value.s = NULL;
								  ((symbolTablePtr)tmp.addr3)->content.isDefined = true;
								  break;
							  }


							  int tmp2 = strlen(((symbolTablePtr)tmp.addr1)->content.value.s);

							  if ((((copyParams)tmp.addr2)->param1->content.value.i) > tmp2){
								  ((symbolTablePtr)tmp.addr3)->content.value.s = NULL;
							  }
							  else{

								((symbolTablePtr)tmp.addr3)->content.value.s = copy(((symbolTablePtr)tmp.addr1)->content.value.s,
									  ((copyParams)tmp.addr2)->param1->content.value.i,
									((copyParams)tmp.addr2)->param2->content.value.i);

								  ((symbolTablePtr)tmp.addr3)->content.isDefined = true;
							  }
							  break;
			}
			case I_FIND:{
							if (((symbolTablePtr)tmp.addr1)->content.isDefined == false)
								return RUN_ERR2;
							if (((symbolTablePtr)tmp.addr2)->content.isDefined == false)
								return RUN_ERR2;

							if (((symbolTablePtr)tmp.addr2)->content.value.s == NULL){
								((symbolTablePtr)tmp.addr3)->content.value.i = 1;
								((symbolTablePtr)tmp.addr3)->content.isDefined = true;
								break;
							}
							if (((symbolTablePtr)tmp.addr1)->content.value.s == NULL){
								((symbolTablePtr)tmp.addr3)->content.value.i = 0;
								((symbolTablePtr)tmp.addr3)->content.isDefined = true;
								break;
							}


							((symbolTablePtr)tmp.addr3)->content.value.i = find(((symbolTablePtr)tmp.addr1)->content.value.s, ((symbolTablePtr)tmp.addr2)->content.value.s);
							((symbolTablePtr)tmp.addr3)->content.isDefined = true;
							break;
			}
			case I_SORT:{
							if (((symbolTablePtr)tmp.addr1)->content.isDefined == false)
								return RUN_ERR2;

							if  (((symbolTablePtr)tmp.addr1)->content.value.s == NULL){
								((symbolTablePtr)tmp.addr3)->content.value.s = NULL;
								((symbolTablePtr)tmp.addr3)->content.isDefined = true;
								break;
							}

							char *pole = gMalloc(sizeof(char)* (strlen(((symbolTablePtr)tmp.addr1)->content.value.s)));
							pole = strcpy(pole, ((symbolTablePtr)tmp.addr1)->content.value.s);
							pole = sort(pole);
							((symbolTablePtr)tmp.addr3)->content.value.s = pole;
							((symbolTablePtr)tmp.addr3)->content.isDefined = true;



							break;
			}

		default:{
					break;
		}
		}
	}
}

