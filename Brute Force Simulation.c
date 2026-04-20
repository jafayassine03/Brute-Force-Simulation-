#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 6
#define CHECKPOINT_FILE "checkpoint.txt"
#define LOG_FILE "log.txt"
#define SESSION_FILE "session.txt"
#define PROGRESS_FILE "progress.txt"
#define MAX_ATTEMPTS 100000000

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

void saveCheckpoint(long long value, char *attempt) {
    FILE *file = fopen(CHECKPOINT_FILE, "w");
    if (file) {
        fprintf(file, "%lld %s", value, attempt);
        fclose(file);
    }
}

void logAttempt(char *attempt) {
    FILE *file = fopen(LOG_FILE, "a");
    if (file) {
        fprintf(file, "%s\n", attempt);
        fclose(file);
    }
}

void saveSession(long long attempts, double elapsed) {
    FILE *file = fopen(SESSION_FILE, "w");
    if (file) {
        time_t now = time(NULL);
        fprintf(file, "Attempts: %lld\nElapsed: %.2f seconds\nLast run: %s",
                attempts, elapsed, ctime(&now));
        fclose(file);
    }
}

void loadSession() {
    FILE *file = fopen(SESSION_FILE, "r");
    if (file) {
        char buffer[256];
        printf("Previous session summary:\n");
        while (fgets(buffer, sizeof(buffer), file)) {
            printf("%s", buffer);
        }
        fclose(file);
        printf("\n");
    }
}

void logProgress(long long attempts, char *attempt) {
    FILE *file = fopen(PROGRESS_FILE, "a");
    if (file) {
        time_t now = time(NULL);
        fprintf(file, "Attempt #%lld | Current: %s | Time: %s",
                attempts, attempt, ctime(&now));
        fclose(file);
    }
}

void bruteForce(char *target, char *charset, int charsetSize) {
    char attempt[MAX_LEN + 1];
    char lastAttempt[MAX_LEN + 1];

    loadSession();

    long long attempts = loadCheckpoint(lastAttempt);
    long long total = 0;

    int targetLen = strlen(target);

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

        if (len == resumeLen) {
            i = startIndex;
        }

        for (; i < limit; i++) {

            if (attempts >= MAX_ATTEMPTS) {
                printf("\nMax attempts reached. Stopping...\n");
                saveCheckpoint(attempts, attempt);
                clock_t end = clock();
                double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
                saveSession(attempts, time_spent);
                return;
            }

            long long temp = i;

            for (int j = len - 1; j >= 0; j--) {
                attempt[j] = charset[temp % charsetSize];
                temp /= charsetSize;
            }
            attempt[len] = '\0';

            attempts++;

            if (attempts % 1000 == 0) {
                logAttempt(attempt);
            }

            if (attempts % 5000 == 0) {
                saveCheckpoint(attempts, attempt);
            }

            if (attempts % 10000 == 0) {
                logProgress(attempts, attempt);
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

            if (strcmp(attempt, target) == 0) {
                clock_t end = clock();
                double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

                printf("\n\nPASSWORD FOUND!\n");
                printf("Password: %s\n", attempt);
                printf("Attempts: %lld\n", attempts);
                printf("Time taken: %.3f seconds\n", time_spent);

                remove(CHECKPOINT_FILE);
                saveSession(attempts, time_spent);
                return;
            }
        }
    }

    printf("\nPassword not found\n");
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    saveSession(attempts, time_spent);
}

int main() {
    char target[] = "abc";
    char charset[] = "abcdefghijklmnopqrstuvwxyz";
    int charsetSize = strlen(charset);

    bruteForce(target, charset, charsetSize);

    return 0;
}

