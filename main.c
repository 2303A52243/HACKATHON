#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX_WORD_LEN 100
#define NUM_SENSITIVE 6

const char *sensitiveWords[NUM_SENSITIVE] = {
    "name", "address", "email", "phone", "dob", "id"
};

int isSensitive(const char *word) {
    for (int i = 0; i < NUM_SENSITIVE; i++) {
        if (strcasecmp(word, sensitiveWords[i]) == 0)
            return 1;
    }
    return 0;
}

void redact(char *word, int level) {
    int len = strlen(word);
    if (level == 1) {
        for (int i = 1; i < len; i++) word[i] = '*';
    } else if (level == 2) {
        for (int i = 0; i < len; i++) word[i] = '*';
    } else if (level == 3) {
        strcpy(word, "[REDACTED]");
    }
}

void processLine(char *line, FILE *out, int level) {
    char temp[MAX_LINE];
    int i = 0, j = 0;

    while (line[i]) {
        if (isalnum(line[i])) {
            temp[j++] = line[i];
        } else {
            temp[j] = '\0';
            if (j > 0) {
                if (isSensitive(temp)) {
                    redact(temp, level);
                }
                fprintf(out, "%s", temp);
                j = 0;
            }
            fprintf(out, "%c", line[i]);
        }
        i++;
    }

    if (j > 0) {
        temp[j] = '\0';
        if (isSensitive(temp)) {
            redact(temp, level);
        }
        fprintf(out, "%s", temp);
    }
}

int main() {
    char inputFile[100], outputFile[100];
    int level;
    FILE *in, *out;
    char line[MAX_LINE];

    printf("Enter input file name: ");
    scanf("%s", inputFile);
    printf("Enter output file name: ");
    scanf("%s", outputFile);
    printf("Enter redaction level (1 to 3): ");
    scanf("%d", &level);
    if (level < 1) level = 1;
    if (level > 3) level = 3;

    in = fopen(inputFile, "r");
    out = fopen(outputFile, "w");

    if (!in || !out) {
        printf("Error opening file.\n");
        return 1;
    }

    while (fgets(line, sizeof(line), in)) {
        processLine(line, out, level);
    }

    fclose(in);
    fclose(out);

    printf("Redacted output saved to %s\n", outputFile);
    return 0;
}

