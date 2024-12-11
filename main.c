#include <stdio.h>
#include "lang.h"
#include "lexer.h"
#include "parser.h"
#include "unfold.h"

extern struct prog * root;
int yyparse();

int main(int argc, char **argv) {
    FILE *inputFile;
    FILE *outputFile;

    if (argc > 1) {
        inputFile = fopen(argv[1], "r");
        if (inputFile == NULL) {
            perror("Error opening input file");
            return 1;
        }
        yyin = inputFile;
    } else {
        yyin = stdin;
    }

    outputFile = fopen("result.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        return 1;
    }

    yyparse();

    if (argc > 1) {
        fclose(inputFile);
    } else {
        fclose(stdin);
    }

    // Redirect stdout to the output file
    FILE *original_stdout = stdout;
    stdout = outputFile;

    printf("-----program before unfolding-----\n\n");
    print_prog(root);

    printf("\n");

    printf("-----program after unfolding-----\n\n");
    print_prog(conv(root));

    // Restore stdout
    stdout = original_stdout;

    fclose(outputFile);

    return 0;
}