// a modified circular Queue implementation in C++: with overwriting when full.
// ref: https://www.programiz.com/dsa/circular-queue 
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
    bool empty = true;

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
        return ((front == (rear + 1) % size) && !empty);
    }

    bool isEmpty() {return empty;};

    int enQueue(int element)
    {
        int result = element;

        if (isFull())
        {
            cout << "Queue is Full --> Over-writing to the front" << endl;
            front = (front + 1) % size; // move the front the oldest data.
            result = -1; // that means overwriting was performed.
        }

        rear = (rear + 1) % size;
        items[rear] = element;
        empty = false;
       
        cout << element << " was inserted at " << rear << endl;
        return result;
    }

    int deQueue()
    {
        int element = items[front];
        if (empty) // --> do nothing
        {
            cout << "Queue is Empty --> do nothing" << endl;
            return (-1);
        }

        if (front == rear) // the last element --> empty = true
        {
            cout << "Removed the last element: " << element << endl;
            empty = true;
        }
        else
        {
            cout << "Removed the front [" << front << "] element: " << element << endl;
        }
        front = (front + 1) % size;
        return element;
    }

    void display()
    {
        if (empty)
        {
            cout << ">>.<<" << endl;
        }
        else
        {
            int i;
            cout << ">> ";
            for (i = front; i != rear; i = (i + 1) % size)
            {
                cout << items[i] << "[" << i << "] | ";
            }
            cout << items[i] << "[" << i << "] <<" << endl;
        }
    }
};

int main()
{
    Queue q(5);

    // deQueue is not possible on empty queue
    q.deQueue();

    // enQueue 5 elements
    q.enQueue(1);
    q.enQueue(2);
    q.enQueue(3);
    q.enQueue(4);
    q.enQueue(5);

    q.deQueue(); // --> Logical Error ???

    // 6th element can't be added to because the queue is full
    q.enQueue(6);
    q.enQueue(7);

    q.display();

    // deQueue removes element entered first i.e. 1
    q.deQueue();
    q.enQueue(8);
    q.enQueue(9);

    // Now we have just 4 elements
    q.display();

    q.deQueue();
    q.deQueue();
    
    q.enQueue(10);
    q.enQueue(11);
    q.enQueue(12);

    q.deQueue();
    q.deQueue();
    q.deQueue();
    q.deQueue();

    q.display();

    return 0;
}