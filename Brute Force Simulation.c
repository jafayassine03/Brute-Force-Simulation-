#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 6  // max password length

void bruteForce(char *target, int length) {
    char attempt[MAX_LEN + 1];
    int attempts = 0;
    long total = 1;

    for (int i = 0; i < length; i++) total *= 26;

    clock_t start = clock();

    for (int i = 0; i < total; i++) {
        int temp = i;

        for (int j = length - 1; j >= 0; j--) {
            attempt[j] = 'a' + (temp % 26);
            temp /= 26;
        }
        attempt[length] = '\0';

        attempts++;

        if (attempts % 1000 == 0) {
            printf("Progress: %.2f%%\n", (attempts * 100.0) / total);
        }

        if (strcmp(attempt, target) == 0) {
            clock_t end = clock();
            double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

            printf("\nPassword found: %s\n", attempt);
            printf("Attempts: %d\n", attempts);
            printf("Time taken: %.3f seconds\n", time_spent);
            return;
        }
    }

    printf("Password not found\n");
}

int main() {
    char password[MAX_LEN + 1];

    printf("Enter password (lowercase letters only, max %d): ", MAX_LEN);
    scanf("%s", password);

    int length = strlen(password);

    bruteForce(password, length);

    return 0;
}
