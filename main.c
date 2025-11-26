
//////////////all the approaches are in the same file i commented out all other approaches when running one of them
//leen alqazaqi 1220380 sec1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
typedef struct node *PtrToNode;
typedef PtrToNode Stack;
typedef struct node *rear; //the rear pointer
//for the queue
struct node {
    char *word; //string
    struct node *Next;
};
struct sortedNodes {
    char *word;
    long int count;
};

//queues functions
int qIsEmpty(struct node *Q) {
    return Q->Next == NULL;
}

void Enqueue(char *Word, struct node *Q, rear *r) {
    struct node *temp;
    temp = (struct node *) malloc(sizeof(struct node));
    temp->word = (char *) malloc(strlen(Word) + 1);
    strcpy(temp->word, Word);
    if (qIsEmpty(Q)) {
        Q->Next = temp;
        temp->Next = NULL;
        *r = temp; //the problem is in this line it does not initialize the r//fixed
        // printf ( "  %s",temp->Data);
    } else {
        (*r)->Next = temp;
        //        printf("test");
        *r = temp;
        temp->Next = NULL;
    }

}

char *Dequeue(struct node *Q) { //returns the node dequeued
    struct node *temp;
    char *returned;

    if (qIsEmpty(Q)) {
        printf("The queue is empty\n");
        return NULL;
    }
    temp = Q->Next;
    returned = (char *) malloc(strlen(temp->word) + 1);
    strcpy(returned, temp->word);
    //printf("ola");
    Q->Next = temp->Next;
    free(temp);

    return returned;
}

void QMakeEmpty(struct node *Q) {
    while (!qIsEmpty(Q))
        Dequeue(Q);
}

struct node *CreateQueue() {
    struct node *Q;
    Q = (struct node *) malloc(sizeof(struct node));
    if (Q == NULL) {
        printf("Out of Space");
        return NULL;
    }
    Q->Next = NULL;
    QMakeEmpty(Q);

    return Q;
}

//merge count based
void merge(struct sortedNodes *arr, int left, int mid, int right) { //rewrite it
    int n1 = mid - left + 1;
    int n2 = right - mid;


    struct sortedNodes *L = malloc(n1 * sizeof(struct sortedNodes));
    struct sortedNodes *R = malloc(n2 * sizeof(struct sortedNodes));

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    // Merging the arrays
    while (i < n1 && j < n2) {
        if (L[i].count >= R[j].count) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of L[], if any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }


    free(L);
    free(R);
}


void mergeSort(struct sortedNodes *arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Recursively sort the two halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}

void printSortedNodes(struct sortedNodes *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("Word: %s, Count: %d\n", arr[i].word, (int) arr[i].count);
    }
}

void printArray(struct sortedNodes *array, int size) {
    for (int i = 0; i < size; i++) {
        if (array[i].word != NULL) {
            printf("Word: %s, Count: %d\n", array[i].word, (int) array[i].count);
        } else {
            printf("Word: (null), Count: %d\n", (int) array[i].count);
        }
    }
}

void naiveFrequency(struct sortedNodes *uniqueWords, struct sortedNodes *TheWords, int count) {
    int uniquewordscount = 0;
    for (int k = 0; k < count; k++) {
        int found = 0;
        for (int m = 0; m < uniquewordscount; m++) {
            if (uniqueWords[m].word && !strcasecmp(uniqueWords[m].word, TheWords[k].word)) {
                found = 1;
                uniqueWords[m].count++;

                break;
            }
        }
        if (!found) {
            uniqueWords[uniquewordscount].word = malloc(strlen(TheWords[k].word) + 1);

            strcpy(uniqueWords[uniquewordscount].word, TheWords[k].word);
            uniqueWords[uniquewordscount].count = 1;
            uniquewordscount++;
        }


    }


}

///////////////////////////////////////////
struct sortedNodes *Naive(struct sortedNodes *TheWords, int count) {
    struct timespec start, end;
    int n = 0;
    struct sortedNodes *uniqueWords = malloc(count * sizeof(struct sortedNodes));
    while (n < count) {
        uniqueWords[n].count = 1;
        uniqueWords[n].word = NULL;
        n++;
    }
    //////////////////////////////////////////////
        clock_gettime(CLOCK_MONOTONIC, &start);
    naiveFrequency(uniqueWords, TheWords, count);
        clock_gettime(CLOCK_MONOTONIC, &end);
        double cpu_time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Time taken by parallel part: %f seconds\n", cpu_time_used);
    ///////////////////////////////////////////////
    int finalSize = 0;
    int k = 0;
    while (uniqueWords[k].word != NULL) {
        finalSize++;
        k++;
    }
    int i = 0;
    struct sortedNodes *final = malloc(finalSize * sizeof(struct sortedNodes));
    while (i < finalSize) {
        final[i].count = uniqueWords[i].count;
        final[i].word = malloc(strlen(uniqueWords[i].word) + 1);
        strcpy(final[i].word, uniqueWords[i].word);
        i++;
    }
    mergeSort(final, 0, finalSize - 1);

    struct sortedNodes *TOP10 = malloc(10 * sizeof(struct sortedNodes));
    for (int z = 0; z < 10; z++) {
        TOP10[z].count = final[z].count;
        TOP10[z].word = malloc(strlen(final[z].word) + 1);
        strcpy(TOP10[z].word, final[z].word);
    }
    printArray(TOP10, 10);

    return TOP10;
}

/////////////////////////////////

void multiprocessingFrequency(struct sortedNodes *chunk, int count, int PipeWrite) {
    struct sortedNodes *uniqueWords = malloc(count * sizeof(struct sortedNodes));
    int local = 0;

    // Count word frequencies in the chunk
    for (int k = 0; k < count; k++) {
        int found = 0;
        for (int m = 0; m < local; m++) {
            if (uniqueWords[m].word && !strcasecmp(uniqueWords[m].word, chunk[k].word)) {
                found = 1;
                uniqueWords[m].count++;
                break;
            }
        }
        if (!found) {
            uniqueWords[local].word = malloc(strlen(chunk[k].word) + 1);
            strcpy(uniqueWords[local].word, chunk[k].word);
            uniqueWords[local].count = 1;
            local++;
        }
    }
    //printf("test1");
    // Send results to parent process
    write(PipeWrite, &local, sizeof(local));
    for (int i = 0; i < local; i++) {
        int wordLength = strlen(uniqueWords[i].word) + 1;
        write(PipeWrite, &wordLength, sizeof(wordLength));
        write(PipeWrite, uniqueWords[i].word, wordLength);
        write(PipeWrite, &uniqueWords[i].count, sizeof(uniqueWords[i].count));
        free(uniqueWords[i].word);
    }
    free(uniqueWords);

    //printf("test1");
    exit(0);
}

void MergeChildrenResults(struct sortedNodes *final, struct sortedNodes *childResults, int *finalCount, int childCount) {
    for (int i = 0; i < childCount; i++) {
        int found = 0;
        for (int j = 0; j < *finalCount; j++) {
            if (!strcasecmp(final[j].word, childResults[i].word)) {
                final[j].count += childResults[i].count;
                found = 1;
                break;
            }
        }
        if (!found) {
            final[*finalCount].word = malloc(strlen(childResults[i].word) + 1);
            strcpy(final[*finalCount].word, childResults[i].word);
            final[*finalCount].count = childResults[i].count;
            (*finalCount)++;
        }
    }
}

struct sortedNodes *MultiProcessing(struct sortedNodes *TheWords, int count, int numofchildren) {
    int chunk_size = count / numofchildren;
    int OrdPipes[numofchildren][2]; //pipe for each child
    for (int i = 0; i < numofchildren; i++) {
        if (pipe(OrdPipes[i]) == -1) {
            printf("Pipe creation failed");

        }
        //printf("test2");

        pid_t pid = fork();
        if (pid < 0) {
            printf("Fork failed");

        } else if (pid == 0) { // Child process
            close(OrdPipes[i][0]); // Close read end in child
            struct sortedNodes *chunk = TheWords + i * chunk_size;
            int size = (i == numofchildren - 1) ? count - i * chunk_size : chunk_size;
            multiprocessingFrequency(chunk, size,OrdPipes[i][1]);
        } else {
            close(OrdPipes[i][1]); // Close write end in parent
        }
    }


    struct sortedNodes *FrequencyCalculated = malloc(count * sizeof(struct sortedNodes));
    int total = 0;

    for (int i = 0; i < numofchildren; i++) {
        int local;
        read(OrdPipes[i][0], &local, sizeof(local));
        struct sortedNodes *childResults = malloc(local * sizeof(struct sortedNodes));
        for (int j = 0; j < local; j++) {
            int wordLength;
            //to read the results on the pipe end.....
            read(OrdPipes[i][0], &wordLength, sizeof(wordLength));
            childResults[j].word = malloc(wordLength);
            read(OrdPipes[i][0], childResults[j].word, wordLength);
            read(OrdPipes[i][0], &childResults[j].count, sizeof(childResults[j].count));
        }
        close(OrdPipes[i][0]);
        //printf("test1");

        MergeChildrenResults(FrequencyCalculated, childResults, &total, local);
        for (int j = 0; j < local; j++) {
            free(childResults[j].word);
        }
        free(childResults);
    }

    for (int i = 0; i < numofchildren; i++) {
        wait(NULL);
    }
    //printf("leenTestleen");
///////////////////////////sequential part///////////////////////
    struct sortedNodes *final = malloc(total * sizeof(struct sortedNodes));
    for (int i = 0; i < total; i++) {
        final[i].word = malloc(strlen(FrequencyCalculated[i].word) + 1);
        strcpy(final[i].word, FrequencyCalculated[i].word);
        final[i].count = FrequencyCalculated[i].count;
        free(FrequencyCalculated[i].word);
    }
    free(FrequencyCalculated);

    mergeSort(final, 0, total - 1);

    struct sortedNodes *TOP10 = malloc(10 * sizeof(struct sortedNodes));
    for (int i = 0; i < 10 && i < total; i++) {
        TOP10[i].word = malloc(strlen(final[i].word) + 1);
        strcpy(TOP10[i].word, final[i].word);
        TOP10[i].count = final[i].count;
    }
    free(final);
    printArray(TOP10, 10);

    return TOP10;
}


/////////////////////////////////////////////////////////
struct ThreadArgs {
    struct sortedNodes *chunk;
    int chunkSize;
    struct sortedNodes *result;
    int *finalCount;
    pthread_mutex_t *mutex; // to prevent race condition
};

void *threadFrequencyCalculation(void *args) {
    struct ThreadArgs *threadargs = (struct ThreadArgs *) args;
    struct sortedNodes *chunk = threadargs->chunk;
    int chunkSize = threadargs->chunkSize;

    //unique words for each thread alone
    struct sortedNodes *localResult = malloc(chunkSize * sizeof(struct sortedNodes));
    int local = 0;

    for (int k = 0; k < chunkSize; k++) {
        int found = 0;
        for (int m = 0; m < local; m++) {
            if (localResult[m].word && !strcasecmp(localResult[m].word, chunk[k].word)) {
                found = 1;
                localResult[m].count++;
                break;
            }
        }
        if (!found) {
            localResult[local].word = malloc(strlen(chunk[k].word) + 1);
            strcpy(localResult[local].word, chunk[k].word);
            localResult[local].count = 1;
            local++;
        }
    }

    // Merge results to get the final one
    pthread_mutex_lock(threadargs->mutex);

  //  printf("test");
    for (int i = 0; i < local; i++) {
        int found = 0;
        for (int j = 0; j < *(threadargs->finalCount); j++) {
            if (!strcasecmp(threadargs->result[j].word, localResult[i].word)) {
                threadargs->result[j].count += localResult[i].count;
                //  printf("test");
                found = 1;
                break;
            }
        }
        if (!found) {
            threadargs->result[*(threadargs->finalCount)].word = malloc(strlen(localResult[i].word) + 1);
            strcpy(threadargs->result[*(threadargs->finalCount)].word, localResult[i].word);
            threadargs->result[*(threadargs->finalCount)].count = localResult[i].count;
            (*(threadargs->finalCount))++;
        }
    }
    pthread_mutex_unlock(threadargs->mutex);


    for (int i = 0; i < local; i++) {
        free(localResult[i].word);
    }
    free(localResult);
    //  printf("test");

    pthread_exit(NULL);
}


struct sortedNodes *MultiThreading(struct sortedNodes *TheWords, int count, int numofthreads) {
    int num_threads = numofthreads;
    int chunk_size = count / num_threads;
    pthread_t threads[num_threads];
    struct ThreadArgs args[num_threads];
    struct sortedNodes *FrequncyCalculated = malloc(count * sizeof(struct sortedNodes));
    int finalCount = 0;

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    //divide the data into chunks each worked on by a thread
    for (int i = 0; i < num_threads; i++) {
        args[i].chunk = TheWords + i * chunk_size;
        if (i == num_threads - 1) {
            //  printf("test");
            args[i].chunkSize = count - i * chunk_size;
        } else {
            args[i].chunkSize = chunk_size;
        }
        args[i].result = FrequncyCalculated;
        args[i].finalCount = &finalCount;
        args[i].mutex = &mutex;
 //printf("leentest);
        pthread_create(&threads[i], NULL, threadFrequencyCalculation, &args[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    //  printf("test");
    pthread_mutex_destroy(&mutex);


    mergeSort(FrequncyCalculated, 0, finalCount - 1);
    struct sortedNodes *TOP10 = malloc(10 * sizeof(struct sortedNodes));
    for (int z = 0; z < 10 && z < finalCount; z++) {
        TOP10[z].count = FrequncyCalculated[z].count;
        TOP10[z].word = malloc(strlen(FrequncyCalculated[z].word) + 1);
        strcpy(TOP10[z].word, FrequncyCalculated[z].word);
    }

    free(FrequncyCalculated);

    printArray(TOP10, 10);
    return TOP10;



}

int main(void) {
    int j = 0;
    struct node *Q = NULL;
    rear r = NULL;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    Q = CreateQueue();
    //  printf("test");
    FILE *in = fopen("/home/leen/projects/OSproject/dataset.txt", "r");
    if (in == NULL)
        printf("cannot open the file");

    char buffer[1024]; // Buffer to store each word

    // Read words from the file
    while (fscanf(in, "%1023s", buffer) == 1) {

        Enqueue(buffer, Q, &r);
    }
    fclose(in);
    struct node *pointer = Q;
    int count = 0;
    int size = 0;

//find number of words in file
    while (pointer->Next != NULL) {
        count++;
        pointer = pointer->Next;
    }
    // printf("%d", count);
    struct sortedNodes *TheWords = malloc(count * sizeof(struct sortedNodes));
    pointer = Q->Next;
    int i = 0;
    //the words is an array with all words as they are in the file
    while (i < count && pointer != NULL) {
        TheWords[i].count = 1;
        TheWords[i].word = malloc(strlen(pointer->word) + 1);
        strcpy(TheWords[i].word, pointer->word);
        pointer = pointer->Next;
        i++;
    }

//
//    clock_gettime(CLOCK_MONOTONIC, &start);
    Naive(TheWords, count);
//        clock_gettime(CLOCK_MONOTONIC, &end);
//
//    double cpu_time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
//    printf("Time taken by naive approach: %f seconds\n", cpu_time_used);

////////////////////////////////////
//    int numofchildren = 2;
//    clock_gettime(CLOCK_MONOTONIC, &start);
//    MultiProcessing(TheWords, count,numofchildren);
//    clock_gettime(CLOCK_MONOTONIC, &end);
//
//    double cpu_time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
//    printf("Time taken by multiprocessing approach 2 children: %f seconds\n", cpu_time_used);

///////////////////////////////////////////
//    int numofchildren2 = 4;
//    clock_gettime(CLOCK_MONOTONIC, &start);
//    MultiProcessing(TheWords, count,numofchildren2);
//            clock_gettime(CLOCK_MONOTONIC, &end);
//
//    double cpu_time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
//    printf("Time taken by multiprocessing approach 4 children: %f seconds\n", cpu_time_used);


///////////////////////////////////////////////
//    int numofchildren3 = 6;
//    clock_gettime(CLOCK_MONOTONIC, &start);
//    MultiProcessing(TheWords, count, numofchildren3);
//            clock_gettime(CLOCK_MONOTONIC, &end);
//
//    double cpu_time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
//    printf("Time taken by multiprocessing approach 6 children: %f seconds\n", cpu_time_used);


//////////////////////////////////////////////////////
//    int numofchildren4 = 8;
//    clock_gettime(CLOCK_MONOTONIC, &start);
//    MultiProcessing(TheWords, count, numofchildren4);
//    clock_gettime(CLOCK_MONOTONIC, &end);
//
//    double cpu_time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
//    printf("Time taken by multiprocessing approach 8 children: %f seconds\n", cpu_time_used);

///////////////////////////////////////////

//    int numofthreads =2;
//    clock_gettime(CLOCK_MONOTONIC, &start);
//    MultiThreading(TheWords, count,numofthreads);
//    clock_gettime(CLOCK_MONOTONIC, &end);
//
//    double cpu_time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
//    printf("Time taken by multithreading for 2 threads: %f seconds\n", cpu_time_used);

//////////////////////////////////////////////

//    int numofthreads3 =4;
//    clock_gettime(CLOCK_MONOTONIC, &start);
//    MultiThreading(TheWords, count,numofthreads3);
//    clock_gettime(CLOCK_MONOTONIC, &end);
//
//    double cpu_time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
//    printf("Time taken by multithreading for 4 threads: %f seconds\n", cpu_time_used);

////////////////////////////////////////////////////

//    int numofthreads2 =6;
//    clock_gettime(CLOCK_MONOTONIC, &start);
//    MultiThreading(TheWords, count,numofthreads2);
//    clock_gettime(CLOCK_MONOTONIC, &end);
//
//    double cpu_time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
//    printf("Time taken by multithreading for 6 threads: %f seconds\n", cpu_time_used);

///////////////////////////////////////////////////

//    int numofthreads4 =8;
//    clock_gettime(CLOCK_MONOTONIC, &start);
//    MultiThreading(TheWords, count,numofthreads4);
//    clock_gettime(CLOCK_MONOTONIC, &end);
//
//    double cpu_time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
//    printf("Time taken by multithreading for 8 threads: %f seconds\n", cpu_time_used);

        clock_gettime(CLOCK_MONOTONIC, &end);

    double cpu_time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Time taken by naive approach: %f seconds\n", cpu_time_used);
    return 0;


}

