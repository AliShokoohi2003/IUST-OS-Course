#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>

#define NUM_CUSTOMERS 5
#define NUM_TRANSACTION 6
#define INIT_STOCK 100

typedef struct
{
    double amount;
    int customer_id;
} transaction_t;

double safeBox = 1000.0;

double customer[NUM_CUSTOMERS];

pthread_mutex_t customerMutex[NUM_CUSTOMERS];
pthread_mutex_t safeBoxMutex = PTHREAD_MUTEX_INITIALIZER;

    
void* withdraw(void* arg){
    transaction_t* transaction = (transaction_t*)arg;

    pthread_mutex_lock(&customerMutex[transaction->customer_id]);

    if (customer[transaction->customer_id] >= transaction->amount) {
        customer[transaction->customer_id] -= transaction->amount;
        printf("Customer %d withdrew %.2f. New balance: %.2f\t", transaction->customer_id, transaction->amount, customer[transaction->customer_id]);
    } else {

        pthread_mutex_lock(&safeBoxMutex);
        if (customer[transaction->customer_id] > 0)
        safeBox -=(transaction->amount - customer[transaction->customer_id]);
        else
        safeBox -= transaction->amount;

        if (safeBox < 0)
        {
            printf("Transaction refused! there isn't enough safe box value");
            safeBox +=(transaction->amount - customer[transaction->customer_id]);

        }
        else{
        double temp = customer[transaction->customer_id];
        customer[transaction->customer_id] -= transaction->amount;
        if (temp > 0)
        printf("Customer %d withdrew %.2f from safe-box. New balance: %.2f\t", transaction->customer_id, transaction->amount - fabs(temp), customer[transaction->customer_id]);
        else
        printf("Customer %d withdrew %.2f from safe-box. New balance: %.2f\t", transaction->customer_id, transaction->amount, customer[transaction->customer_id]);
        pthread_mutex_unlock(&safeBoxMutex);

        
    }}

    pthread_mutex_unlock(&customerMutex[transaction->customer_id]);

    pthread_mutex_lock(&safeBoxMutex);
    printf("safe-box value: %.2f\n", safeBox);
    pthread_mutex_unlock(&safeBoxMutex);
    return NULL;
}


void* deposit(void* arg){
    transaction_t* transaction = (transaction_t*)arg;

    pthread_mutex_lock(&customerMutex[transaction->customer_id]);
    pthread_mutex_lock(&safeBoxMutex);

    if(customer[transaction->customer_id] < 0)
    {
        if (fabs(transaction->amount) > fabs(customer[transaction->customer_id]))
        {
            safeBox += fabs(customer[transaction->customer_id]);
        }
        else
        {
            safeBox += fabs(fabs(transaction->amount) - fabs(customer[transaction->customer_id]));
        }
        
    }

    customer[transaction->customer_id] += transaction->amount;

    printf("Customer %d stock was charged by %.2f. New balance: %.2f\tsafe-box value: %.2f\n", transaction->customer_id, transaction->amount, customer[transaction->customer_id], safeBox);

    pthread_mutex_unlock(&safeBoxMutex);
    pthread_mutex_unlock(&customerMutex[transaction->customer_id]);
    return NULL;
}

int main(){
    srand(time(NULL));

    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        customer[i] = INIT_STOCK;
        pthread_mutex_init(&customerMutex[i], NULL);
    }
    pthread_t threads[NUM_TRANSACTION];

    for (int i = 0; i < NUM_TRANSACTION; i++) {
        transaction_t* transaction = malloc(sizeof(transaction_t));
        transaction->customer_id = rand() % NUM_CUSTOMERS;
        transaction->amount = (rand() % 1000) + 1;

        if (rand() % 2 == 0)
            pthread_create(&threads[i], NULL, withdraw, (void*)transaction);
        else
            pthread_create(&threads[i], NULL, deposit, (void*)transaction);
    }

    for (int i = 0; i < NUM_TRANSACTION; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_mutex_destroy(&customerMutex[i]);
    }

    return 0;
}
