#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 6
#define CHECKPOINT_FILE "checkpoint.txt"

long long loadCheckpoint(char *lastAttempt) {
    FILE *file = fopen(CHECKPOINT_FILE, "r");
    long long value = 0;

    if (file) {
        fscanf(file, "%lld %s", &value, lastAttempt);
        fclose(file);
        printf("Resuming from attempt #%lld (last: %s)\n", value, lastAttempt);
    } else {
        lastAttempt[0] = '\0';
    }

    return value;
}

// Save checkpoint
void saveCheckpoint(long long value, char *attempt) {
    FILE *file = fopen(CHECKPOINT_FILE, "w");
    if (file) {
        fprintf(file, "%lld %s", value, attempt);
        fclose(file);
    }
}

void bruteForce(char *target, char *charset, int charsetSize) {
    char attempt[MAX_LEN + 1];
    char lastAttempt[MAX_LEN + 1];

    long long attempts = loadCheckpoint(lastAttempt);
    long long total = 0;

    int targetLen = strlen(target);

    // Calculate total combinations
    for (int len = 1; len <= targetLen; len++) {
        long long t = 1;
        for (int i = 0; i < len; i++) t *= charsetSize;
        total += t;
    }

    clock_t start = clock();
    long long currentCount = 0;

    if (lastAttempt[0] != '\0') {
        printf("Last tried combination: %s\n", lastAttempt);
    }

    for (int len = 1; len <= targetLen; len++) {
        long long limit = 1;
        for (int i = 0; i < len; i++) limit *= charsetSize;

        for (long long i = 0; i < limit; i++) {

            // Skip tested attempts
            if (currentCount < attempts) {
                currentCount++;
                continue;
            }

            long long temp = i;

            for (int j = len - 1; j >= 0; j--) {
                attempt[j] = charset[temp % charsetSize];
                temp /= charsetSize;
            }
            attempt[len] = '\0';

            attempts++;
            currentCount++;

            // Save checkpoint every 5000 attempts
            if (attempts % 5000 == 0) {
                saveCheckpoint(attempts, attempt);
            }

            
            clock_t now = clock();
            double elapsed = (double)(now - start) / CLOCKS_PER_SEC;
            double speed = attempts / (elapsed > 0 ? elapsed : 1);
            double remaining = (total - attempts) / (speed > 0 ? speed : 1);

            if (attempts % 2000 == 0) {
                printf("Progress: %.2f%% | Speed: %.0f/s | ETA: %.1fs | Current: %s\r",
                       (attempts * 100.0) / total,
                       speed,
                       remaining,
                       attempt);
                fflush(stdout);
            }

            // Check password
            if (strcmp(attempt, target) == 0) {
                clock_t end = clock();
                double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

                printf("\n\nPASSWORD FOUND!\n");
                printf("Password: %s\n", attempt);
                printf("Attempts: %lld\n", attempts);
                printf("Time taken: %.3f seconds\n", time_spent);

                remove(CHECKPOINT_FILE); // delete checkpoint
                return;
            }
        }
    }

    printf("\nPassword not found\n");
}

int main() {
    char target[] = "abc";             
    char charset[] = "abcdefghijklmnopqrstuvwxyz";
    int charsetSize = strlen(charset);

    bruteForce(target, charset, charsetSize);

    return 0;
}
