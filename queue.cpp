// Queue implementation in C++
#include <iostream>
using std::cout;
using std::endl;

class Queue
{
private:
    int *items = nullptr;
    int front = 0;
    int rear = -1;
    int size = 0;

public:
    Queue(int s) : size(s)
    {
        items = new int[size];
    }
    ~Queue()
    {
        delete[] items;
    }

    bool isFull()
    {
        return (rear >= size - 1);
    }

    bool isEmpty()
    {
        return (rear == -1);
    }

    bool enQueue(int element)
    {
        if (isFull())
        {
            cout << "Queue is Full !" << endl;
            return false;
        }
        ++rear;
        items[rear] = element;
        cout << element << " was inserted at " << rear << endl;
        return true;
    }

    int deQueue()
    {
        int element;
        if (isEmpty())
        {
            cout << "Queue is Empty !" << endl;
            return (-1);
        }
        if (front == rear) // the last element --> reset the Queue
        {
            element = items[front];
            cout << "Removed the last element: " << element << endl;
            front = 0;
            rear = -1;
        }
        else
        {
            element = items[front];
            cout << "Removed the front [" << front << "] element: " << element << endl;
            ++front;
        }
        return element;
    }

    void display()
    {
        if (isEmpty())
        {
            cout << "Empty." << endl;
        }
        else
        {
            for (size_t i = front; i <= rear; i++)
            {
                cout << items[i] << "[" << i << "] | ";
            }
            cout << endl;
        }
    }
};

int main()
{
    Queue q(10);

    // deQueue is not possible on empty queue
    q.deQueue();

    // enQueue 5 elements
    q.enQueue(1);
    q.enQueue(2);
    q.enQueue(3);
    q.enQueue(4);
    q.enQueue(5);

    q.deQueue(); // --> Logical Error in the above implementation!!!!

    // 6th element can't be added to because the queue is full
    q.enQueue(6);

    q.display();

    // deQueue removes element entered first i.e. 1
    q.deQueue();

    // Now we have just 4 elements
    q.display();

    q.deQueue();
    q.deQueue();
    q.deQueue();
    q.deQueue();
    q.deQueue();
    q.deQueue();

    return 0;
}