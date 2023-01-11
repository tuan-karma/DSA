// Singly linked list: need run-time dynamic storage
// ref: [Low Level Programming's video](https://www.youtube.com/watch?v=dti0F7w3yOQ)

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    void *next;
    int data;
} Node;

Node *head = NULL;

// add a node to the list
Node *addNode(int data)
{
    Node *new = malloc(sizeof(Node));
    if (new == NULL)
        return NULL;

    if (head == NULL) // the list is empty
    {
        new->data = data;
        head = new;
        new->next = NULL;
    }
    else // the list is not empty
    {
        new->data = data;
        new->next = head;
        head = new;
    }

    return new;
}

int removeNode(int data)
{
    Node *current = head;
    Node *prev = head;

    while (current != NULL)
    {
        if (current->data == data)
        {
            if (current == head)
            {
                head = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            free(current);
            return 1;
        }
        prev = current;
        current = current->next;
    }

    return 0;
}

// insert a node into the position in the list
Node *insertNode(int data, int position)
{
    Node *current = head;
    while (current != NULL && position != 0)
    {
        position--;
    }

    if (position !=0)
    {
        printf("Requested position too far into list\n");
        return NULL;
    }

    Node *new = malloc(sizeof(Node));
    if (new == NULL)
        return NULL;

    new -> data = data;
    new -> next = current -> next;
    current -> next = new;
    return new;
}


void printList()
{
    Node *current = head;
    while (current != NULL)
    {
        printf("%d->", current->data);
        current = current->next;
    }

    printf("\n");
    return;
}

void printMenu()
{
    printf("You have following options:\n");
    printf("\t1. Add a node to the list.\n");
    printf("\t2. Remove a node from the list.\n");
    printf("\t3. Insert a node to the list.\n");
    printf("\t4. Print your list\n");
    printf("\t5. Quit.\n");
}

int main(int argc, char **argv)
{
    int option = -1;
    int data;
    int position = 0;

    while (option != 5)
    {
        printMenu();
        int num_received = scanf("%d", &option);
        if (num_received == 1 && option > 0 && option <= 5)
        {
            switch (option)
            {
            case 1:
                printf("What data should I add to the beginning of the list?\n");
                scanf("%d", &data);
                Node *new = addNode(data);
                break;
            case 2:
                printf("What data should I remove from the list?\n");
                scanf("%d", &data);
                if (!removeNode(data))
                    printf("Element not found\n");
                break;
            case 3:
                printf("What data should I insert into the list?\n");
                scanf("%d", &data);
                printf("What position should I insert the data into the list?\n");
                scanf("%d", &position);
                insertNode(data, position);
                break;
            case 4:
                printList();
                break;
            case 5:
                break;
            }
        }
    }

    return 0;
}