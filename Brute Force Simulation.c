#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 6

void bruteForce(char *target, char *charset, int charsetSize) {
    char attempt[MAX_LEN + 1];
    int attempts = 0;
    long long total = 0;

    int targetLen = strlen(target);

    for (int len = 1; len <= targetLen; len++) {
        long long t = 1;
        for (int i = 0; i < len; i++) t *= charsetSize;
        total += t;
    }

    clock_t start = clock();

    for (int len = 1; len <= targetLen; len++) {
        long long limit = 1;
        for (int i = 0; i < len; i++) limit *= charsetSize;

        for (long long i = 0; i < limit; i++) {
            long long temp = i;

            for (int j = len - 1; j >= 0; j--) {
                attempt[j] = charset[temp % charsetSize];
                temp /= charsetSize;
            }
            attempt[len] = '\0';

            attempts++;

            // timing
            clock_t now = clock();
            double elapsed = (double)(now - start) / CLOCKS_PER_SEC;
            double speed = attempts / (elapsed > 0 ? elapsed : 1);

            double remaining = (total - attempts) / (speed > 0 ? speed : 1);

            if (attempts % 2000 == 0) {
                printf("Progress: %.2f%% | Speed: %.0f attempts/sec | ETA: %.1f sec\n",
                       (attempts * 100.0) / total, speed, remaining);
            }

            if (strcmp(attempt, target) == 0) {
                clock_t end = clock();
                double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

                printf("\nPASSWORD FOUND!\n");
                printf("Password: %s\n", attempt);
                printf("Attempts: %d\n", attempts);
                printf("Time taken: %.3f seconds\n", time_spent);
                return;
            }
        }
    }

    printf("Password not found\n");
}

int main() {
    char password[MAX_LEN + 1];
    char charset[100];

    printf("Enter charset (example: abcdef...): ");
    scanf("%s", charset);

    printf("Enter password (lowercase letters only, max %d): ", MAX_LEN);
    scanf("%s", password);

    int charsetSize = strlen(charset);

    bruteForce(password, charset, charsetSize);

    return 0;
}
