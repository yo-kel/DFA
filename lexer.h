
enum Token {
	INT, Float,
	ID,
	ADD, SUB, STAR, MUL, DIV, AND, OR, NOT,
	EQ, NE, GT, LT, GE, LE,
	TYPE,
	LP, RP, LB, RB, LC, RC,
	RETURN,
	IF, ELSE,
	WHILE
};

extern int token;
extern int line;