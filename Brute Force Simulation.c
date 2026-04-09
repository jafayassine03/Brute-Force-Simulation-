#include <stdio.h>
#include <string.h>

int main() {
    char password[] = "abc";
    char attempt[4];
    int attempts = 0;

    for (char a = 'a'; a <= 'z'; a++) {
        for (char b = 'a'; b <= 'z'; b++) {
            for (char c = 'a'; c <= 'z'; c++) {
                attempt[0] = a;
                attempt[1] = b;
                attempt[2] = c;
                attempt[3] = '\0';
                attempts++;
                if (strcmp(attempt, password) == 0) {
                    printf("Password found: %s\n", attempt);
                    printf("Attempts: %d\n", attempts);
                    return 0;
                }
            }
        }
    }

    printf("Password not found\n");
    printf("Attempts: %d\n", attempts);
    return 0;
}
