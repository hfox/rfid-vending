#include <stdio.h>
#include <stdlib.h>

// Include deskey.h from the project to get the key size
#include "../deskey.h"

#define out(fp,str) fputs(str,fp)

void fail(char *msg)
{
	fprintf(stderr, "keygen: error: %s\n", msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	char *infn = NULL;
	char outfn[] = "deskey_auto_generated.c";
	FILE *infp = NULL;
	FILE *outfp = NULL;

	int c;
	int i = 0;

	if (argc != 2) {
		fail("Usage: keygen keyfile");
	}

	infn = argv[1];

	infp = fopen(infn, "r");
	if(!infp) {
		fail("Can not open input file");
	}

	outfp = fopen(outfn, "w");
	if (!outfp) {
		fail("Can not open output file");
	}

	out(outfp, "/* NOTE: This file is automatically generated! */\n");
	out(outfp, "/*       Editing this file is not recommended. */\n");
	out(outfp, "\n");

	fprintf(outfp, "char deskey[%d];\n", DES_KEY_SIZE);
	fprintf(outfp, "char deskey[%d] = {", DES_KEY_SIZE);

	while((c = fgetc(infp)) != EOF) {
		// Put a comma after previous field
		if(i > 0) {
			out(outfp, ", ");
		}
		// Newline and indentation before new rows
		if(i % 8 == 0) {
			out(outfp, "\n\t");
		}
		fprintf(outfp, "0x%02x", c);
		++i;
	}

	if (i != DES_KEY_SIZE) {
		fail("key file has wrong length");
	}

	out(outfp, "\n};\n");
	fclose(infp);
	fclose(outfp);
	return 0;
}

