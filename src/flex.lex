%option noyywrap nounput noinput

%{
#include <cstdlib>
#include <iostream>
#include <string>
#include "ast.hpp"
#include "statement.hpp"

#include "parser.tab.hpp"

%}

%%

"LET" return var_token;
"FUNCTION" return fun_token;
"RETURN" return return_token;
"IF" return if_token;
"ELSE" return else_token;

"PRINT" return print_token;
"WHILE" return while_token;

"DO" return do_token;
"FOR" return for_token;

"==" return eq_token;
"!=" return ne_token;
"<=" return le_token;
">=" return ge_token;

"AND" return andl_token;
"OR" return orl_token;
"NOT" return notl_token;

"BEGIN" return begin_block_token;
"END" return end_block_token;

"INT" return int_type_token;
"STRING" return string_type_token;

[a-zA-Z_][a-zA-Z_0-9]* {
  yylval.string_value = new std::string(yytext);
  return id_token;
}

-?[0-9]+ {
  yylval.int_value = atoi(yytext);
  return int_token;
}

\".+?\" {
    yylval.string_value = new std::string(yytext);
    return str_token;
}

[-=(),;%+*/<>\n:] return *yytext;

[ \t\r] {}

. {
  std::cerr << "ERROR(lex), unknown character: '" << *yytext << "'" << std::endl;
  exit(EXIT_FAILURE);
}

%%
