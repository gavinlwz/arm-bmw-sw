#include <string.h>

#include "debug.h"
#include "uart.h"

#include "cli.h"

#define CLI_MAX_ARGC    5

extern struct cli_program cli_programs[];

static void cmd_dispatch(int argc, char **argv) {
    int i;

    if (argc < 1)
        return;

    for (i = 0; cli_programs[i].name != NULL; i++) {
        if (strcmp(argv[0], cli_programs[i].name) == 0) {
            cli_programs[i].func(argc, argv);
            break;
        }
    }

    if (cli_programs[i].name == NULL) {
        debug_printf("unknown program \"%s\"\n", argv[0]);
        debug_printf("please type \"help\" for help\n");
    }
}

static int cmd_tokenize(char *s, char **argv, int max_argc) {
    char *token;
    int i;

    token = strtok(s, " ");
    for (i = 0; token != NULL && i < max_argc; i++) {
        argv[i] = token;
        token = strtok(NULL, " ");
    }

    return i;
}

void cli(void) {
    char buf[64];
    char *argv[CLI_MAX_ARGC];
    int argc;

    while (1) {

        uart_puts("\x1b[1;36marm-bmw\x1b[0m > ");
        uart_gets(buf, sizeof(buf), true);
        argc = cmd_tokenize(buf, argv, CLI_MAX_ARGC);
        cmd_dispatch(argc, argv);
    }
}

