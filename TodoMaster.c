#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define program "TodoMaster"

#define PRINT_ERROR(message, error) {\
    printf("Error: %s\n", message);\
    exit(error);\
}

#define PRINT_HELP() {\
    printf("Usage: ./todomaster <file> [options]\n");\
    printf("Options:\n");\
    printf("\t-h, --help   \t\t\tShow this help.\n");\
    printf("\t-o, --output \t\t\tChoose the output file name.\n");\
}

int main(int argc, char *argv[]) { // main.c <file> -o <file> | main -
    if (argc < 2) {
        PRINT_HELP()
        PRINT_ERROR("Not enough arguments given.", -1);
    }

    if (!(strcmp(argv[1], "-h") && strcmp(argv[1], "--help"))) {
        PRINT_HELP();
        return 0;
    }

    char output[256] = {0};
    for (int x = 2; x < argc; x++) {
        if (!(strcmp(argv[x], "-h") && strcmp(argv[x], "--help"))) {
            PRINT_HELP();
            return 0;
        } else if (!(strcmp(argv[x], "-o") && strcmp(argv[x], "--output"))) {
            strcpy(output, argv[x + 1]);
        }
    }

    if (!output[0]) { strcat(strcpy(output, argv[1]), ".TodoMaster"); }

    FILE *fp = fopen(argv[1], "r");
    FILE *fw = fopen(output, "w");
    char chunk[128];
    char *line, *pos;

    size_t len_used, chunk_used, len = sizeof(chunk);
    if (!(fw && fp)) { PRINT_ERROR("1File can't be opened.", 2); }
    if (!(line = malloc(len))) { PRINT_ERROR("Unable to allocate memory for the line buffer.", 12); }

    int linecount = 1, todocount = 1;
    while (fgets(chunk, sizeof(chunk), fp)) {
        len_used = strlen(line);
        chunk_used = strlen(chunk);

        if (len - len_used < chunk_used) {
            len *= 2;
            if (!(line = realloc(line, len))) {
                free(line);
                PRINT_ERROR("Unable to reallocate memory for the line buffer.", 12);
            }
        }

        strncpy(line + len_used, chunk, len - len_used);
        len_used += chunk_used;

        if (line[len_used - 1] == '\n') {
            if (pos = strstr(line, "TODO")) {
                fprintf(fw, "%d. TODO (L%d C%d) %s", todocount, linecount, (int) (pos - line) + 1, pos + 4);
                todocount++;
                line[0] = '\0';
            }
            linecount++;
        }
    }

    fclose(fp);
    fclose(fw);
    free(line);
    return 0;
}
