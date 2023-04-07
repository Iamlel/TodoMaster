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

typedef struct node {
    int line;
    int ch;
    char *data;
    struct node *next;
} Node;

typedef struct list {
    Node *head;
} List;

Node *createnode(int line, int ch, char *data) {
    Node *newNode = malloc(sizeof(Node));
    if (!newNode) { return NULL; }

    newNode->line = line;
    newNode->ch   = ch;
    newNode->data = strdup(data);
    newNode->next = NULL;
    return newNode;
}

List *makelist() {
    List *list = malloc(sizeof(List));
    if (!list) { return NULL; }
    list->head = NULL;
    return list;
}

void display(List *list, FILE *fp, char text[]) {
    if (list->head == NULL) { return; }
    fprintf(fp, "-----%s-----\n", text);

    int x = 1;
    for (Node *current = list->head; current != NULL; current = current->next) {
        fprintf(fp, "%d. %s (L%d C%d) %s", x, text, current->line, current->ch, current->data);
        x++;
    }
    fprintf(fp, "\n");
}

void add(int line, int ch, char *data, List *list){
    Node *current = NULL;
    if (list->head == NULL) {
        list->head = createnode(line, ch, data);
    } else {
        current = list->head;
        while (current->next!=NULL) {
            current = current->next;
        }
        current->next = createnode(line, ch, data);
    }
}

void destroy(List *list){
    Node *current = list->head;
    Node *next = current;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

int main(int argc, char *argv[]) { // todomaster.c <file> -o <file> | main -
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
    if (!(fw && fp)) { PRINT_ERROR("File can't be opened.", 2); }
    if (!(line = malloc(len))) { PRINT_ERROR("Unable to allocate memory for the line buffer.", 12); }

    int linecount = 1;
    List *todolist = makelist();
    List *fixmelist = makelist();

    while (fgets(chunk, sizeof(chunk), fp)) {
        len_used = strlen(line);
        chunk_used = strlen(chunk);

        if (len - len_used < chunk_used) {
            len = len << 1;
            if (!(line = realloc(line, len))) {
                free(line);
                PRINT_ERROR("Unable to reallocate memory for the line buffer.", 12);
            }
        }

        strncpy(line + len_used, chunk, len - len_used);
        len_used += chunk_used;

        if (line[len_used - 1] == '\n') {
            if (strstr(line, "TODO")) {
                pos = strstr(line, "TODO");
                add(linecount, (int) (pos - line) + 1, pos + 4, todolist);
                line[0] = '\0';
            } else if (strstr(line, "FIXME")) {
                pos = strstr(line, "FIXME");
                add(linecount, (int) (pos - line) + 1, pos + 5, fixmelist);
                line[0] = '\0';
            }
            linecount++;
        }
    }

    free(line);
    display(todolist, fw, "TODO");
    display(fixmelist, fw, "FIXME");

    fclose(fp);
    fclose(fw);
    destroy(fixmelist);
    destroy(todolist);

    return 0;
}
