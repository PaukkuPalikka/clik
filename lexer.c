/* lexer.c: the lexer and associated helper functions
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"

struct Token *tokenize(char *source) {
	/* start token, previous token */
	struct Token *st = NULL;
	struct Token *pt = NULL;

	/* current character */
	char *cc = source;
	int line = 0;
	/* start of the current line */
	char *col = cc;

	/* assuming source is null-terminated */
	/* TODO: check for null in other places if needed */
	while (*cc != '\0') {
		/* skip whitespace */
		while (isspace(*cc)) {
			if (*cc == '\n') {
				line++;
				col = cc;
			}
			cc++;
		}

		/* comments */
		if (*cc == '#') {
			while (*cc != '\n')
				cc++;
			continue;
		}

		struct Token *ct = malloc(sizeof(struct Token));
		if (ct == NULL) {
			perror("malloc() failed");
			if (st != NULL) free_token_list(st);
			return NULL;
		}
		memset(ct, 0, sizeof(struct Token));

		/* this is probably not needed (it is memset to 0 anyway) */
		ct->next = NULL;
		
		/* save the first token */
		if (pt == NULL)
			st = ct;

		/* maintain links between nodes */
		if (pt != NULL)
			pt->next = ct;
		pt = ct;

		/* factored out from the switch */
		ct->value = cc;
		ct->len = 1;

		if (strncmp(cc, "return", 6) == 0) {
			ct->type = KRETURN;
			cc += 6;
			ct->len = 6;
		} else if (strncmp(cc, "goto", 4) == 0) {
			ct->type = KGOTO;
			cc += 4;
			ct->len = 4;
		} else if (strncmp(cc, "if", 2) == 0) {
			ct->type = KIF;
			cc += 2;
			ct->len = 2;
		} else if (strncmp(cc, "while", 5) == 0) {
			ct->type = KWHILE;
			cc += 5;
			ct->len = 5;
		} else
		switch (*cc) {
		case '(':
			ct->type = LPAREN;
			break;
		case ')':
			ct->type = RPAREN;
			break;
		case '{':
			ct->type = LCURLY;
			break;
		case '}':
			ct->type = RCURLY;
			break;
		case '+':
			ct->type = PLUS;
			break;
		case '-':
			ct->type = MINUS;
			break;
		case '@':
			ct->type = AT;
			break;
		case ':':
			ct->type = COLON;
			break;
		case '"':
			ct->type = STRING;
			/* skip the opening " */
			cc++;
			ct->value = cc;
			while (*cc != '"') {
				if (*cc == '\n') {
					line++;
					col = cc;
				}
				cc++;
				/* check for escape sequences and skip them */
				if (*cc == '\\') {
					cc++;
				}
			}
			/* calculate the length */
			ct->len = cc - ct->value;
			break;
		default:
			/* other checks here */
			if (isdigit(*cc)) {
				/* integer literal */
				/* no hex or oct support for now :( */
				ct->val_int = *cc - '0';
				ct->type = ILITERAL;
				while (isdigit(*(cc + 1))) {
					/* I know there is atoi */
					ct->val_int = ct->val_int * 10 + *++cc - '0';
				}
			} else if (isalpha(*cc)) {
				/* identifier */
				ct->type = IDENTIFIER;
				while (isalpha(*(cc + 1)) || isdigit(*(cc + 1)))
					cc++;
				ct->len = cc - ct->value + 1;
			}
			/* TODO: other stuff maybe */
		}
		ct->line = line;
		ct->col = cc - col; /* TODO: check if this works */
		col = cc;
		cc++;
	}
	return st;
}

void print_token_list(struct Token *token) {
	while (token->next != NULL) {
		if (token->type == ILITERAL)
			printf("%d\n", token->val_int);
		else
			printf("%.*s\n", token->len, token->value);
		token = token->next;
	}
}

void free_token_list(struct Token *token) {
	struct Token *next;
	while ((next = token->next) != NULL) {
		free(token);
		token = next;
	}
}

