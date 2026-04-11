#include <stdio.h>
#include <string.h>
#include <time.h>

int main() {
    char password[] = "abc";
    char attempt[4];
    int attempts = 0;
    int total = 26 * 26 * 26; // total combinations

    // Start timing
    clock_t start = clock();

    for (char a = 'a'; a <= 'z'; a++) {
        for (char b = 'a'; b <= 'z'; b++) {
            for (char c = 'a'; c <= 'z'; c++) {
                attempt[0] = a;
                attempt[1] = b;
                attempt[2] = c;
                attempt[3] = '\0';
                attempts++;

                // Show progress every 1000 attempts
                if (attempts % 1000 == 0) {
                    printf("Progress: %.2f%%\n", (attempts * 100.0) / total);
                }

                if (strcmp(attempt, password) == 0) {
                    clock_t end = clock();
                    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

                    printf("Password found: %s\n", attempt);
                    printf("Attempts: %d\n", attempts);
                    printf("Time taken: %.3f seconds\n", time_spent);
                    return 0;
                }
            }
        }
    }

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Password not found\n");
    printf("Attempts: %d\n", attempts);
    printf("Time taken: %.3f seconds\n", time_spent);
    return 0;
}

