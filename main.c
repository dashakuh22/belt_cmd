#include <stdio.h>
#include <string.h>
#include <bee2/crypto/base_belt.h>

void showHelp() {
    printf("Incorrect args, use: [-e|-d] \"message\" [-base10|-base64] \"password\"");
    printf("                 or: [-ef|-df] \"input_file.txt\" \"output_file.txt\" \"password\"");
}

int main(int argc, char *argv[]) {

    if (argc != 5) {
        showHelp();
        return -1;
    } else {
        char* mode = argv[1];

        if (strcmp(mode, "-e") != 0 && strcmp(mode, "-d") != 0 && strcmp(mode, "-ef") != 0 && strcmp(mode, "-df") != 0) {
            showHelp();
            return -1;
        }

        if (strcmp(mode, "-e") == 0 || strcmp(mode, "-d") == 0) {
            char* message = argv[2];
            char* base = argv[3];
            char* password = argv[4];

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

        if (strcmp(mode, "-ef") == 0 || strcmp(mode, "-df") == 0) {
            char* fin = argv[2];
            char* fout = argv[3];
            char* password = argv[4];

            printf("mode\t\t=\t%s\ninput\t\t=\t%s\noutput\t\t=\t%s\npassword\t=\t%s\n", mode, fin, fout, password);

            FILE* in = fopen(fin, "rb");
            FILE* out = fopen(fout, "wb");

            if (in == NULL) {
                printf("input file \"%s\" doesnt exist", fin);
            }

            if (out == NULL) {
                printf("output file \"%s\" cannot open file", fout);
            }

            fseek(in, 0L, SEEK_END);
            size_t size = ftell(in);
            fseek(in, 0L, SEEK_SET);

            void* fBuffer = memAlloc(size);
            fread(fBuffer, 1, size, in);

            char* destb64 = memAlloc(size);
            b64From(destb64, fBuffer, size);

            char* outputb64 = memAlloc(size);

            fclose(in);

            err_t r = ERR_OK;

            if (strcmp(mode, "-ef") == 0) {
                r = baseBeltFmtEnc("base64", outputb64, destb64, password, NULL);
            }

            if (strcmp(mode, "-df") == 0) {
                r = baseBeltFmtDecr("base64", outputb64, destb64, password, NULL);
            }

            b64To(fBuffer, &size, outputb64);
            fwrite(fBuffer, 1, size, out);

            if (r != ERR_OK) {
                memFree(fBuffer);
                memFree(destb64);
                memFree(outputb64);
                printf("\tError: %s", errMsg(r));
                return -1;
            } else {
                printf("\tOK");
            }

            memFree(fBuffer);
            memFree(destb64);
            memFree(outputb64);

            fclose(out);
            fclose(in);
        }

    }

    return 0;
}
