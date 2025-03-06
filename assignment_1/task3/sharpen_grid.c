#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>

#define IMG_HEIGHT (3000)
#define IMG_WIDTH (4000)
#define NUM_ROW_THREADS (6)
#define NUM_COL_THREADS (8)
#define IMG_H_SLICE (IMG_HEIGHT / NUM_ROW_THREADS)
#define IMG_W_SLICE (IMG_WIDTH / NUM_COL_THREADS)

typedef double FLOAT;

pthread_t threads[NUM_ROW_THREADS * NUM_COL_THREADS];
pthread_mutex_t file_lock;  // Mutex for safe file writing
pthread_barrier_t barrier;  // Barrier to synchronize threads

typedef struct _threadArgs {
    int thread_idx;
    int i;
    int j;
    int h;
    int w;
} threadArgsType;

threadArgsType threadarg[NUM_ROW_THREADS * NUM_COL_THREADS];

typedef unsigned int UINT32;
typedef unsigned long long int UINT64;
typedef unsigned char UINT8;

// PPM Edge Enhancement Code
UINT8 header[22];
UINT8 R[IMG_HEIGHT][IMG_WIDTH];
UINT8 G[IMG_HEIGHT][IMG_WIDTH];
UINT8 B[IMG_HEIGHT][IMG_WIDTH];
UINT8 convR[IMG_HEIGHT][IMG_WIDTH];
UINT8 convG[IMG_HEIGHT][IMG_WIDTH];
UINT8 convB[IMG_HEIGHT][IMG_WIDTH];

#define K 4.0
FLOAT PSF[9] = {-K/8.0, -K/8.0, -K/8.0, -K/8.0, K+1.0, -K/8.0, -K/8.0, -K/8.0, -K/8.0};

void *sharpen_thread(void *threadptr) {
    threadArgsType thargs = *((threadArgsType *)threadptr);
    int i, j;
    FLOAT temp = 0;

    for (i = thargs.i; i < (thargs.i + thargs.h); i++) {
        for (j = thargs.j; j < (thargs.j + thargs.w); j++) {
            if (i == 0 || j == 0 || i == IMG_HEIGHT - 1 || j == IMG_WIDTH - 1) continue; // Avoid boundary issues

            // Process Red Channel
            temp = 0;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    temp += PSF[(dx + 1) * 3 + (dy + 1)] * (FLOAT)R[i + dx][j + dy];
                }
            }
            convR[i][j] = (UINT8)(temp < 0.0 ? 0.0 : (temp > 255.0 ? 255.0 : temp));

            // Process Green Channel
            temp = 0;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    temp += PSF[(dx + 1) * 3 + (dy + 1)] * (FLOAT)G[i + dx][j + dy];
                }
            }
            convG[i][j] = (UINT8)(temp < 0.0 ? 0.0 : (temp > 255.0 ? 255.0 : temp));

            // Process Blue Channel
            temp = 0;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    temp += PSF[(dx + 1) * 3 + (dy + 1)] * (FLOAT)B[i + dx][j + dy];
                }
            }
            convB[i][j] = (UINT8)(temp < 0.0 ? 0.0 : (temp > 255.0 ? 255.0 : temp));
        }
    }

    pthread_barrier_wait(&barrier); // Ensure all threads finish processing before writing

    pthread_exit((void *)0);
}

int main(int argc, char *argv[]) {
    int fdin, fdout, bytesRead = 0, bytesLeft, i, j, thread_idx;
    
    if (argc < 3) {
        printf("Usage: sharpen input_file.ppm output_file.ppm\n");
        exit(-1);
    }

    if ((fdin = open(argv[1], O_RDONLY, 0644)) < 0) {
        printf("Error opening %s\n", argv[1]);
        exit(-1);
    }

    if ((fdout = open(argv[2], (O_RDWR | O_CREAT), 0666)) < 0) {
        printf("Error opening %s\n", argv[2]);
        exit(-1);
    }

    bytesLeft = 21;
    do {
        bytesRead = read(fdin, (void *)header, bytesLeft);
        bytesLeft -= bytesRead;
    } while (bytesLeft > 0);
    
    header[21] = '\0';

    // Read RGB data
    for (i = 0; i < IMG_HEIGHT; i++) {
        for (j = 0; j < IMG_WIDTH; j++) {
            read(fdin, (void *)&R[i][j], 1);
            read(fdin, (void *)&G[i][j], 1);
            read(fdin, (void *)&B[i][j], 1);
        }
    }

    printf("Source file %s read\n", argv[1]);
    close(fdin);

    // Initialize Barrier & Mutex
    pthread_barrier_init(&barrier, NULL, NUM_ROW_THREADS * NUM_COL_THREADS);
    pthread_mutex_init(&file_lock, NULL);

    // Create Threads
    for (thread_idx = 0; thread_idx < (NUM_ROW_THREADS * NUM_COL_THREADS); thread_idx++) {
        threadarg[thread_idx].i = (thread_idx / NUM_COL_THREADS) * IMG_H_SLICE;
        threadarg[thread_idx].j = (thread_idx % NUM_COL_THREADS) * IMG_W_SLICE;
        threadarg[thread_idx].h = IMG_H_SLICE;
        threadarg[thread_idx].w = IMG_W_SLICE;

        pthread_create(&threads[thread_idx], NULL, sharpen_thread, (void *)&threadarg[thread_idx]);
    }

    // Wait for Threads to Finish
    for (thread_idx = 0; thread_idx < (NUM_ROW_THREADS * NUM_COL_THREADS); thread_idx++) {
        pthread_join(threads[thread_idx], NULL);
    }

    printf("Writing output file %s\n", argv[2]);
    write(fdout, (void *)header, 21);

    // Write RGB data
    for (i = 0; i < IMG_HEIGHT; i++) {
        for (j = 0; j < IMG_WIDTH; j++) {
            pthread_mutex_lock(&file_lock);
            write(fdout, (void *)&convR[i][j], 1);
            write(fdout, (void *)&convG[i][j], 1);
            write(fdout, (void *)&convB[i][j], 1);
            pthread_mutex_unlock(&file_lock);
        }
    }

    printf("Output file %s written\n", argv[2]);
    close(fdout);

    // Destroy Mutex and Barrier
    pthread_mutex_destroy(&file_lock);
    pthread_barrier_destroy(&barrier);

    return 0;
}

