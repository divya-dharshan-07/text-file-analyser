

#include <stdio.h>
#include <ctype.h>

typedef struct {
    long characters;
    long words;
    long lines;
    long frequency[26];
} Analysis;

void analyzeFile(FILE *file, Analysis *a) {
    int ch, inWord = 0, lastChar = -1;

    while ((ch = fgetc(file)) != EOF) {
        unsigned char c = (unsigned char)ch;
        a->characters++;
        lastChar = ch;

        if (ch == '\n')
            a->lines++;

        if (isspace(c)) {
            inWord = 0;
        } else if (!inWord) {
            a->words++;
            inWord = 1;
        }

        if (isalpha(c)) {
            int lower = tolower(c);
            if (lower >= 'a' && lower <= 'z')
                a->frequency[lower - 'a']++;
        }
    }

    if (a->characters > 0 && lastChar != '\n')
        a->lines++;
}

void displayReport(FILE *out, const Analysis *a) {
    long max = 0;
    int i;

    for (i = 0; i < 26; i++)
        if (a->frequency[i] > max)
            max = a->frequency[i];

    fprintf(out, "\n=== TEXT FILE CONTENT ANALYZER ===\n");
    fprintf(out, "Characters: %ld\n", a->characters);
    fprintf(out, "Words     : %ld\n", a->words);
    fprintf(out, "Lines     : %ld\n", a->lines);

    fprintf(out, "\nLetter Frequencies:\n");
    for (i = 0; i < 26; i++) {
        if (a->frequency[i] > 0)
            fprintf(out, "%c : %ld\n",
                    'a' + i, a->frequency[i]);
    }

    if (max == 0) {
        fprintf(out, "\nNo English letters found.\n");
    } else {
        fprintf(out, "\nMost frequent letter(s): ");
        for (i = 0; i < 26; i++) {
            if (a->frequency[i] == max)
                fprintf(out, "%c ", 'a' + i);
        }
        fprintf(out, "\nMaximum frequency: %ld\n", max);
    }
}

int main(void) {
    char filename[256];
    FILE *input, *report;
    Analysis a = {0};

    printf("Enter input filename: ");
    if (scanf("%255s", filename) != 1) {
        printf("Invalid filename input.\n");
        return 1;
    }

    input = fopen(filename, "rb");
    if (input == NULL) {
        printf("Error: Cannot open input file.\n");
        return 1;
    }

    analyzeFile(input, &a);
    fclose(input);

    displayReport(stdout, &a);

    report = fopen("report.txt", "w");
    if (report == NULL) {
        printf("\nCould not create report.txt\n");
        return 1;
    }

    displayReport(report, &a);
    fclose(report);

    printf("\nReport saved to report.txt\n");
    return 0;
}
