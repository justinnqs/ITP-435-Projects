%{
#include "Node.h"
#include <iostream>
#include <string>
extern int yylex();
extern void yyerror(const char* s);

// Global for the main block
NBlock* gMainBlock = nullptr;

// Disable the warnings that Bison creates
#pragma warning(disable: 4065)
%}

/* Add one union member for each Node* type */
%union {
    Node* node;
	NBlock* block;
	NStatement* statement;
	NNumeric* numeric;
	NBoolean* boolean;
	std::string* string;
	int token;
}

%error-verbose

/* Terminal symbols */
%token <string> TINTEGER
%token <token> TLBRACE TRBRACE TSEMI TLPAREN TRPAREN
%token <token> TMAIN TROTATE TFORWARD TBOOLEAN TIF TELSE TATTACK TRANGEDATTACK
%token <boolean> TISHUMAN TISZOMBIE TISPASSABLE TISRANDOM TISWALL

/* Statements */
%type <block> main_loop block
%type <statement> statement rotate attack ranged_attack ifelse forward
%type <boolean> bool is_zombie is_human is_passable is_random is_wall

/* Expressions */
%type <numeric> numeric

%%

main_loop	: TMAIN TLBRACE block TRBRACE { std::cout << "Main entry point found!" << std::endl; ($3)->SetMainBlock(); gMainBlock = $3;}
;

block		: statement { std::cout << "Single statement" << std::endl; $$ = new NBlock(); ($$)->AddStatement($1); }

/* TODO: Add support for multiple statements in a block */
        | block statement {
         std::cout << "Multiple statements" << std::endl;
          ($1)->AddStatement($2);
          $$ = $1;
         }
;

statement	: rotate TSEMI | ifelse | bool TSEMI | attack TSEMI | ranged_attack TSEMI | forward TSEMI
;
rotate		: TROTATE TLPAREN numeric TRPAREN { std::cout << "Rotate command" << std::endl; $$ = new NRotate($3); }
;
numeric		: TINTEGER { std::cout << "Numeric value of " << *($1) << std::endl; $$ = new NNumeric(*($1)); }
;
bool		:  is_zombie | is_human | is_passable | is_random | is_wall
;
ifelse	  : TIF TLPAREN bool TRPAREN TLBRACE block TRBRACE TELSE TLBRACE block TRBRACE  { std::cout << "If Else command" << std::endl; $$ = new NIfElse($3, $6, $10); }
;
attack	  : TATTACK TLPAREN TRPAREN   { std::cout << "Attack command" << std::endl; $$ = new NAttack(); }
;
ranged_attack		: TRANGEDATTACK TLPAREN TRPAREN   { std::cout << "RANGED ATTACK command" << std::endl; $$ = new NRangedAttack(); }
;
is_zombie	: TISZOMBIE TLPAREN numeric TRPAREN { $$ = new IsZombie($3); }
;
is_human	: TISHUMAN TLPAREN numeric TRPAREN  { $$ = new IsHuman($3); }
;
is_passable : TISPASSABLE TLPAREN TRPAREN		{ $$ = new IsPassable(); }
;
is_random	: TISRANDOM TLPAREN TRPAREN			{ $$ = new IsRandom(); }
;
is_wall		: TISWALL TLPAREN TRPAREN			{ $$ = new IsWall; }
;
forward		: TFORWARD TLPAREN TRPAREN { std::cout << "Forward command" << std::endl; $$ = new NForward(); }
;

%%
