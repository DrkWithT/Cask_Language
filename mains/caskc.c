/**
 * @file caskc.c
 * @author Derek Tan
 * @brief Implements main compiling / test program for Cask. The VM is separate.
 * @date 2023-12-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/files.h"
#include "frontend/lexer.h"

#define CASK_APPNAME "Cask 0.1.0\nBy: Derek Tan"

#define CASK_OPTION_HELP "-h"
#define CASK_OPTION_VERSION "-v"
#define CASK_OPTION_FILE "-f"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s -[h|v|f] <file?> \n", argv[0]);
        return 1;
    }

    if (!strcmp(argv[1], CASK_OPTION_HELP))
    {
        printf("Cask Options:\nFor help, use -h\nFor version, use -v\nFor file usage, use -f\n");
        return 0;
    }

    if (!strcmp(argv[1], CASK_OPTION_VERSION))
    {
        printf("%s\n", CASK_APPNAME);
        return 0;
    }

    /// @note Omit `getopt()` error messages and replace missing arg values with '?'.
    opterr = 0;
    int temp_option = -1;
    bool invalid_option = false;
    char file_path[128];
    memset(file_path, '\0', 128);

    while (!invalid_option && (temp_option = getopt(argc, argv, "f:")) != -1)
    {
        switch (temp_option)
        {
        case 'f':
            if (strlen(optarg) <= 128)
            {
                strncpy(file_path, optarg, 128);
            }
            else
            {
                invalid_option = true;
            }
            break;
        default:
            invalid_option = true;
            break;
        }
    }

    if (invalid_option)
    {
        fprintf(stderr, "Usage: %s -[h|v|f] <file?>\n", argv[0]);
        return 1;
    }

    /// @note Test file loading for now.
    char *source_buffer = file_load(file_path);
    char *lexeme = NULL;

    if (!source_buffer)
    {
        fprintf(stderr, "%s [Error]: could not read file.\n", argv[0]);
        return 1;
    }

    /// @note Test lexer for now!
    Token temp_token;
    Lexer lexer;
    lexer_init(&lexer, source_buffer);

    do
    {
        temp_token = lexer_yield_token(&lexer);
        lexeme = token_stringify(&temp_token, source_buffer);

        if (lexeme != NULL)
        {
            printf("{type: %i, lexeme: %s}\n", temp_token.type, lexeme);
            free(lexeme);
        }
    } while (temp_token.type != CASK_EOF_TOKEN);

    free(source_buffer);

    return 0;
}
