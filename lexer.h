/* lexer.h: header file for lexer functions
 *
 * Copyright (C) 2018 Pauli Kauro
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

enum TokenType {
	/* keywords are prefixed with K */
	KRETURN,
	KGOTO, /* :D */
	KIF,
	KWHILE,
	LPAREN,
	RPAREN,
	LCURLY,
	RCURLY,
	PLUS,
	MINUS,
	AT,
	COLON,
	STRING,
	ILITERAL,
	/* FLITERAL, */
	IDENTIFIER
};

struct Token {
	enum TokenType type;
	union {
		char *value;
		int val_int;
	};
	size_t len;
	int line;
	int col;
	struct Token *next;
};

struct Token *tokenize(char *source);
void print_token_list(struct Token *token);
void free_token_list(struct Token *token);

