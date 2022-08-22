// Stack implementation using array in Cpp
#include <stdlib.h>
#include <iostream>
using std::cout;
using std::endl;

class Stack
{
public:
    Stack(int s): size(s) 
    {
        items = new int[size];
    }
    ~Stack()
    {
        delete [] items;
    }
    bool isFull()
    {
        return (top == size - 1);
    }
    bool isEmpty()
    {
        return (top == -1);
    }
    bool push(int newItem)
    {
        if (isFull()) return false;
        ++top;
        items[top] = newItem;
        return true;
    }
    int* pop()
    {
        if (isEmpty()) return nullptr;
        --top;
        return &items[top+1];
    }
    int* peek()
    {
        if (isEmpty()) return nullptr;
        return &items[top];
    }
    void print()
    {
        cout << "Stack: ";
        for (size_t i = 0; i < size; i++)
        {
            cout << items[i] << " | ";
        }
        cout << endl;
    }

    int length()
    {
        return top + 1;
    }
private:
    int size = 0;
    int* items = nullptr;
    int top = -1; // top position of the stack, - 1 means empty stack.
};

int main()
{
    Stack st(10);
    for (size_t i = 0; i < 8; i++)
    {
        if (st.isFull()) 
        {
            cout << "Stack is Full" << endl;
            break;
        }
        st.push(i+1);
    }
    st.print();
    cout << "The value at top of current stack is: " << *st.peek() << endl;
    for (size_t i = 0; i < 13; i++)
    {
        if (st.isEmpty())
        {
            cout << endl << "Stack is Empty" << endl;
            break;
        }
        cout << *st.pop() << " ";
    }
    cout << endl;

    return 0;
}