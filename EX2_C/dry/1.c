char *partialCopyString(char *str, bool copy_even, int *out_len) {
    char *OUT; // *convention mistake* //
    if (copy_even) {
        out_len = malloc(sizeof(int));
        *out_len = strlen(str) / 2;
        OUT = malloc(*out_len);
        for (int i = 0; i < strlen(str); i += 2) {
            OUT[i / 2] = str[i + 1];
        }
    } else { // *convention mistake* //
        out_len = malloc(sizeof(int));
        *out_len = strlen(str) / 2 + strlen(str) % 2;
        OUT = malloc(*out_len);
        for (int i = 0; i < strlen(str); i += 2) {
            OUT[i / 2] = str[i];
        }
    }
    return OUT;
}
// 234122-305218750-4214050


// 1. Code duplication (not sure...)
// 2. Not cheking if pointers args are NULL.
// 3. Not checking for memory allocation failure
// 4. out_len is an integer, why does it require malloc?

// Convention mistakes:
// 1. 'OUT' is a variable, should be 'out'
// 2. in line 11 '{' should be in line 10.