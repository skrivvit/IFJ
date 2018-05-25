#ifndef _SCANNER
#define _SCANNER

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>


typedef enum{

	T_START,					// 00 - vychozi stav
	T_IDENTIFICATOR,			// 01 - identifikator
	T_COMMENT,					// 02 - komentar
	T_END,						// 03 - konec cteni
	T_COLON,					// 04 - operator :
	T_ASSIGN,					// 05 - operator prirazeni :=
	T_SEMICOLON,				// 06 - strednik
	T_LC,						// 07 - leva zavorka
	T_RC,						// 08 - prava zavorka
	T_STRING,					// 09 - string
	T_DIV,						// 10 - operator deleni /
	T_MUL,						// 11 - operator nasobeni *
	T_MINUS,					// 12 - operator odecitani -
	T_PLUS,						// 13 - operator scitani +
	T_COMMA,					// 14 - carka
	T_LESSER,					// 15 - operator <
	T_LOE,						// 16 - operator <=
	T_NOTEQUAL,					// 17 - operator <>
	T_GREATER,					// 18 - operator >
	T_GOE,						// 19 - operator >=
	T_KEYWORD,					// 20 - klicove slovo
	T_DOT,						// 21 - tecka
	T_INTEGER,					// 22 - integer
	T_REAL,						// 23 - real
	T_EOF,						// 24 - EOF
	T_ERR,						// 25 - error
	T_EQUAL,					// 26 - operator =
	T_EXPREAL0,					// 27 - pomocny stav
	T_EXPREAL,					// 28 - desetinne cislo v exponencionalnim tvaru
	T_EXPREAL1,
	T_REAL1,
	T_STRING1,
	T_STRING2,
	T_STRING3
	// jeste jich sem prijde mnohem vic :)


}tState;



typedef struct{

	tState state;		// informace o druhu tokenu
	char *content;		// obsah tokenu
	int row;			// radek na kterem se token nachazi
	int length;			// delka tokenu

}tToken;

extern FILE *file;

tToken tGetToken();
void tInitToken(tToken *token);
void tFreeToken(tToken *token);
void tExtendToken(tToken *token, int c);
tState checkKeyWord(tToken token);

#endif
