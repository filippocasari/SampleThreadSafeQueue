#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

// struct node - struttura di un nodo della coda thread-safe
typedef struct node {
    int value;
    struct node *next;
} node;

// struct Queue_r - struttura della coda thread-safe (usa una linked list)
typedef struct {
    node *front;
    node *rear;
    pthread_mutex_t mutex;
} Queue_r;

// qcreate() - crea una coda vuota
Queue_r* qcreate()
{
    // crea la coda
    Queue_r *queue = malloc(sizeof(Queue_r));

    // inizializza la coda
    queue->front = NULL;
    queue->rear  = NULL;
    pthread_mutex_init(&queue->mutex, NULL);
    return queue;
}

// enqueue() - aggiunge un elemento alla coda
void enqueue(Queue_r* queue, int value)
{
    // crea un nuovo nodo
    node *temp = malloc(sizeof(struct node));
    temp->value = value;
    temp->next  = NULL;

    // blocco l'accesso
    pthread_mutex_lock(&queue->mutex);

    // test se la coda è vuota
    if (queue->front == NULL) {
        // con la coda vuota front e rear coincidono
        queue->front = temp;
        queue->rear  = temp;
    }
    else {
        // aggiungo un elemento
        node *old_rear = queue->rear;
        old_rear->next = temp;
        queue->rear    = temp;
    }

    // sblocco l'accesso ed esco
    pthread_mutex_unlock(&queue->mutex);
}

// dequeue() - toglie un elemento dalla coda
bool dequeue(Queue_r* queue, int *value)
{
    // blocco l'accesso
    pthread_mutex_lock(&queue->mutex);

    // test se la coda è vuota
    node *front = queue->front;
    if (front == NULL) {
        // sblocco l'accesso ed esco
        pthread_mutex_unlock(&queue->mutex);
        return false;
    }

    // leggo il valore ed elimino l'elemento dalla coda
    *value = front->value;
    queue->front = front->next;
    free(front);

    // sblocco l'accesso ed esco
    pthread_mutex_unlock(&queue->mutex);
    return true;
}

// main() - funzione main
int main()
{
    // creo la coda
    Queue_r *my_queue = qcreate();

    // aggiungo un po' di elementi
    enqueue(my_queue, 10);
    enqueue(my_queue, 20);
    enqueue(my_queue, 30);
    enqueue(my_queue, 40);

    // mostro i risultati
    printf("la coda my_queue contiene: ");
    int qval;
    while (dequeue(my_queue, &qval))
        printf("%d ", qval);

    printf("\n");





    return 0;
}