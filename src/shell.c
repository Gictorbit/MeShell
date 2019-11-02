#include "shell.h"

static void clean_up(Token* t) {
    while (t) {
        Token* temp = t->next;
        free(t);
        t = temp;
    }
}

void start_shell(Shell* sh) {
    // Always use heap for cwd, because we need to free it in cd
    sh->cwd = (char*)malloc(sizeof(char) * 2);
    strcpy(sh->cwd, "/");
    while (1) {
        printf(YELLOW "MeShell " NO_COLOR "%s" GREEN " >>> " NO_COLOR, sh->cwd);
        char line[100];
        char* readl = fgets(line, 99, stdin);

        // Check for EOF (Ctrl-D)
        if (readl == NULL) break;
        // Empty input
        if (*readl == '\n') continue;

        if (strchr(readl, '%') != NULL) {
            puts(RED "Error: Character '%' is not allowed" NO_COLOR);
            continue;
        }
        // Remove trailing newline
        readl[strcspn(readl, "\r\n")] = 0;

        Token* tokens = NULL;
        char* err = NULL;

        int size = tokenize(readl, &tokens, &err);
        if (size == 0) {
            if (err) {
                printf(RED "Error: %s\n" NO_COLOR, err);
            }
            clean_up(tokens);
            continue;
        }
        err = NULL;
        run_command(tokens, sh, &err);
        if (err) {
            printf(RED "Error: %s\n" NO_COLOR, err);
        }

        clean_up(tokens);
    }
}
