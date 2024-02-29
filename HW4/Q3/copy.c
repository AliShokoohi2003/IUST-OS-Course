#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 2048

typedef struct {    
    FILE *source;
    FILE *destination;
    char buffer[BUFFER_SIZE];
    size_t dataSize;
    pthread_mutex_t mutex;
    int readFlag;
} FileData;

void *readerThread(void *data);
void *writerThread(void *data);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        return EXIT_FAILURE;
    }

    FILE *source = fopen(argv[1], "rb");
    if (source == NULL) {
        printf("Error opening source file\n");
        return EXIT_FAILURE;
    }

    FILE *destination = fopen(argv[2], "wb");
    if (destination == NULL) {
        printf("Error opening destination file\n");
        fclose(source);
        return EXIT_FAILURE;
    }

    FileData fileData = {
        .source = source,
        .destination = destination,
        .dataSize = 0,
        .readFlag = 0
    };

    if (pthread_mutex_init(&fileData.mutex, NULL) != 0) {
        perror("Error initializing mutex\n");
        fclose(source);
        fclose(destination);
        return EXIT_FAILURE;
    }

    pthread_t reader, writer;

    if (pthread_create(&reader, NULL, readerThread, (void *)&fileData) != 0) {
        printf("Error creating reader thread\n");
        return EXIT_FAILURE;
    }

    if (pthread_create(&writer, NULL, writerThread, (void *)&fileData) != 0) {
        printf("Error creating writer thread\n");
        return EXIT_FAILURE;
    }

    pthread_join(reader, NULL);
    pthread_join(writer, NULL);

    fclose(source);
    fclose(destination);
    pthread_mutex_destroy(&fileData.mutex);

    return 0;
}

void *readerThread(void *data) {
    FileData *fileData = (FileData *)data;

    while (!feof(fileData->source)) {
        pthread_mutex_lock(&fileData->mutex);

        if (fileData->dataSize == 0) {
            fileData->dataSize = fread(fileData->buffer, 1, BUFFER_SIZE, fileData->source);
            if (fileData->dataSize == 0 && ferror(fileData->source)) {
                printf("Error reading from source file");
                pthread_mutex_unlock(&fileData->mutex);
                break;
            }
        }

        if (fileData->dataSize > 0) {
            pthread_mutex_unlock(&fileData->mutex);
        }
    }

    fileData->readFlag = 1;
    return NULL;
}

void *writerThread(void *data) {
    FileData *fileData = (FileData *)data;

    while (1) {
        pthread_mutex_lock(&fileData->mutex);

        if (fileData->dataSize > 0) {
            if (fwrite(fileData->buffer, 1, fileData->dataSize, fileData->destination) != fileData->dataSize) {
                printf("Error writing to destination file");
                pthread_mutex_unlock(&fileData->mutex);
                break;
            }
            fileData->dataSize = 0;

            pthread_mutex_unlock(&fileData->mutex);
        } else {
            pthread_mutex_unlock(&fileData->mutex);

            if (fileData->readFlag) {
                break;
            }
        }
    }

    return NULL;
}

