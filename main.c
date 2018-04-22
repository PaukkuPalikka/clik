/* main.c: the entry point of the compiler
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


#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"

int main(int argc, char **argv) {
	int retval = EXIT_FAILURE;
	/* TODO: make it accept a third argument */
	if (argc != 2) {
		printf("usage: %s sourcefile\n", argv[0]);
		return retval;
	}

	FILE *f = fopen(argv[1], "r");
	if (f == NULL) {
		perror("fopen() failed");
		return retval;
	}

	long size;
	if (fseek(f, 0, SEEK_END) == -1) {
		perror("fseek() failed");
		goto out_close;
	}

	size = ftell(f);
	if (size == -1) {
		perror("ftell() failed");
		goto out_close;
	}

	if (fseek(f, 0, SEEK_SET) == -1) {
		perror("fseek() failed");
		goto out_close;
	}

	if (size == 0) {
		printf("source file is empty; ignoring\n");
		retval = EXIT_SUCCESS;
		goto out_close;
	}

	char *buffer = malloc(size + 1);
	if (buffer == NULL) {
		perror("malloc() failed");
		goto out_free;
	}

	fread(buffer, 1, size, f);
	if (ferror(f)) {
		perror("fread() failed");
		goto out_free;
	}

	/* null terminate the string */
	buffer[size] = '\0';

	/* now we finally have the source code read into the buffer */

	printf("\nsource dump\n%s\n", buffer);

	/*tokens = tokenize(buffer);
	printf("\nlexer dump\n");
	print_token(tokens);

	ast = parse(tokens);
	printf("\nparser dump\n");
	print_tree(ast);*/

	retval = EXIT_SUCCESS;

out_free:
	free(buffer);

out_close:
	/* maybe this should be done immediately after reading unless error */
	fclose(f);
	return retval;
}
