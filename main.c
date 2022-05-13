#include <stdio.h>
#include <string.h>
#include <bee2/crypto/base_belt.h>

int main(int argc, char *argv[]) {

    if (argc != 5) {
        printf("Incorrect args, use: [-e|-d] \"message\" [-base10|-base64] \"password\"");
        return -1;
    } else {
        char* mode = argv[1];
        char* message = argv[2];
        char* base = argv[3];
        char* password = argv[4];

        if (strcmp(mode, "-e") != 0 && strcmp(mode, "-d") != 0) {
            printf("\tIllegal mode argument: \"%s\". Use [-e|-d]", mode);
            return -1;
        }

        if (strcmp(base, "-base10") != 0 && strcmp(base, "-base64") != 0) {
            printf("\tIllegal base argument: \"%s\". Use [-base10|-base64]", base);
            return -1;
        }

        printf("mode\t\t=\t%s\nmessage\t\t=\t%s\nbase\t\t=\t%s\npassword\t=\t%s\n", mode, message, base, password);

        size_t destLen = strlen(message) - 1;
        char* dest = memAlloc(destLen);
        err_t r = ERR_OK;

        if (strcmp(mode, "-e") == 0) {
            r = baseBeltFmtEnc(base + 1, dest, message, password, NULL);
        }

        if (strcmp(mode, "-d") == 0) {
            r = baseBeltFmtDecr(base + 1, dest, message, password, NULL);
        }

        if (r != ERR_OK) {
            memFree(dest);
            printf("\tError: %s", errMsg(r));
            return -1;
        } else {
            printf("\tResult is \"%s\"", dest);
        }

        memFree(dest);

    }

    return 0;
}
