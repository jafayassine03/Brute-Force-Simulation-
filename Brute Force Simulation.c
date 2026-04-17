#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 6
#define CHECKPOINT_FILE "checkpoint.txt"
#define LOG_FILE "log.txt"

// Convert attempt string to index
long long attemptToIndex(char *attempt, char *charset, int charsetSize) {
    long long index = 0;
    int len = strlen(attempt);

    for (int i = 0; i < len; i++) {
        char *pos = strchr(charset, attempt[i]);
        if (!pos) return 0;

        int charIndex = pos - charset;
        index = index * charsetSize + charIndex;
    }

    return index;
}

// Load checkpoint
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

// NEW: log attempts
void logAttempt(char *attempt) {
    FILE *file = fopen(LOG_FILE, "a");
    if (file) {
        fprintf(file, "%s\n", attempt);
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

    long long startIndex = 0;
    int resumeLen = 0;

    if (lastAttempt[0] != '\0') {
        resumeLen = strlen(lastAttempt);
        startIndex = attemptToIndex(lastAttempt, charset, charsetSize) + 1;
        printf("Resuming from exact position...\n");
    }

    for (int len = 1; len <= targetLen; len++) {

        long long limit = 1;
        for (int i = 0; i < len; i++) limit *= charsetSize;

        long long i = 0;

        // Jump directly if resuming same length
        if (len == resumeLen) {
            i = startIndex;
        }

        for (; i < limit; i++) {

            long long temp = i;

            // Generate attempt string
            for (int j = len - 1; j >= 0; j--) {
                attempt[j] = charset[temp % charsetSize];
                temp /= charsetSize;
            }
            attempt[len] = '\0';

            attempts++;

            // NEW: log every 1000 attempts
            if (attempts % 1000 == 0) {
                logAttempt(attempt);
            }

            // Save checkpoint every 5000 attempts
            if (attempts % 5000 == 0) {
                saveCheckpoint(attempts, attempt);
            }

            // Timing
            clock_t now = clock();
            double elapsed = (double)(now - start) / CLOCKS_PER_SEC;
            double speed = attempts / (elapsed > 0 ? elapsed : 1);
            double remaining = (total - attempts) / (speed > 0 ? speed : 1);

            // Live progress
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

                remove(CHECKPOINT_FILE);
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
