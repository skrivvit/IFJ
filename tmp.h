#ifndef _TMP
#define _TMP

#define SYNTAX_OK 0		// vse OK
#define LEX_ERR 1		// lexikalni chyba
#define SYNTAX_ERR 2	// chybna syntaxe struktury programu
#define SEM_ERR 3		// semanticka chyba v programu - nedefinovana funkce/redefinice ...
#define SEM_ERR2 4		// semanticka chyba typove kompatibility, pripadne spatny pocet ci typy parametru volani funkce
#define SEM_ERR3 5		// ostatni semanticke chyby
#define RUN_ERR 6		// behova chyba pri nacitani hodnoty ze vstupu
#define RUN_ERR2 7		// behova chyba pri praci s neinicializovanou promennou
#define RUN_ERR3 8		// behova chyba deleni nulou
#define RUN_ERR4 9		// ostatni behove chyby
#define INT_ERR 99		// interni chyba, neovlivnena programem, napriklad spatne parametry, neuspech alokace pameti, ...

#endif