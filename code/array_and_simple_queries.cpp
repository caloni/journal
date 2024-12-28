/* Solves
* https://www.hackerrank.com/challenges/array-and-simple-queries/problem
* without caring about limited resources =)
* @author Wanderley Caloni <wanderley.caloni@gmail.com>
* @date 2024-09-05
*/

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

#define TRACE(trace)


void ArrayPrint(int* A, int N)
{
    for (int n = 0; n < N; ++n)
    {
        cout << A[n] << " ";
    }
    cout << "\n";
}


void RawMemorySolution(const int N, const int M)
{
    int* A = (int*)malloc(sizeof(int) * N);
    int* A2 = (int*)malloc(sizeof(int) * N);

    for (int n = 0; n < N; ++n)
    {
        cin >> A[n];
    }

    for (int m = 0; m < M; ++m)
    {
        int type, i, j;
        cin >> type >> i >> j;
        int elements = j - i + 1;
        if (type == 1)
        {
            TRACE(cout << "copying " << elements << " from " << i << " to backup\n");
            memcpy(A2, &A[i - 1], sizeof(int) * elements);
            TRACE(ArrayPrint(A2, elements));
            TRACE(cout << "moving " << &A[i - 1] - A << " from 1 to " << elements + 1 << "\n");
            memmove(&A[elements], A, sizeof(int) * (&A[i - 1] - A));
            TRACE(ArrayPrint(A, N));
            TRACE(cout << "moving " << elements << " from backup to 1" << "\n");
            memmove(A, A2, sizeof(int) * elements);
            TRACE(ArrayPrint(A, N));
        }
        else if (type == 2)
        {
            TRACE(cout << "copying " << elements << " from " << i << " to backup\n");
            memcpy(A2, &A[i - 1], sizeof(int) * elements);
            TRACE(ArrayPrint(A2, elements));
            TRACE(cout << "moving " << N - j << " from " << j + 1 << " to " << i << "\n");
            memmove(&A[i - 1], &A[j], sizeof(int) * (N - j));
            TRACE(ArrayPrint(A, N));
            TRACE(cout << "moving " << elements << " from backup to " << N - elements + 1 << "\n");
            memmove(&A[N - elements], A2, sizeof(int) * elements);
            TRACE(ArrayPrint(A, N));
        }
        TRACE(cout << endl);
    }
    TRACE(cout << "output\n");

    int a = abs(A[0] - A[N - 1]);
    cout << a << "\n";
    ArrayPrint(A, N);
    cout << endl;
    free(A), free(A2);
}


void ArrayCopy(int* to, int* from, int elements)
{
    for (int i = 0; i < elements; ++i)
    {
        *to++ = *from++;
    }
}


void ArrayMove(int* to, int* from, int elements)
{
    int* buffer = (int*)malloc(sizeof(int) * elements);
    int* bufferp = buffer;

    for (int i = 0; i < elements; ++i)
    {
        *bufferp++ = *from++;
    }

    bufferp = buffer;
    for (int i = 0; i < elements; ++i)
    {
        *to++ = *bufferp++;
    }

    free(buffer);
}


void ArraySolution(const int N, const int M)
{
    int* A = (int*)malloc(sizeof(int) * N);
    int* A2 = (int*)malloc(sizeof(int) * N);

    for (int n = 0; n < N; ++n)
    {
        cin >> A[n];
    }

    for (int m = 0; m < M; ++m)
    {
        int type, i, j;
        cin >> type >> i >> j;
        int elements = j - i + 1;
        if (type == 1)
        {
            TRACE(cout << "copying " << elements << " from " << i << " to backup\n");
            ArrayCopy(A2, &A[i - 1], elements);
            TRACE(ArrayPrint(A2, elements));
            TRACE(cout << "moving " << &A[i - 1] - A << " from 1 to " << elements + 1 << "\n");
            ArrayMove(&A[elements], A, (int)(&A[i - 1] - A));
            TRACE(ArrayPrint(A, N));
            TRACE(cout << "moving " << elements << " from backup to 1" << "\n");
            ArrayMove(A, A2, elements);
            TRACE(ArrayPrint(A, N));
        }
        else if (type == 2)
        {
            TRACE(cout << "copying " << elements << " from " << i << " to backup\n");
            ArrayCopy(A2, &A[i - 1], elements);
            TRACE(ArrayPrint(A2, elements));
            TRACE(cout << "moving " << N - j << " from " << j + 1 << " to " << i << "\n");
            ArrayMove(&A[i - 1], &A[j], (N - j));
            TRACE(ArrayPrint(A, N));
            TRACE(cout << "moving " << elements << " from backup to " << N - elements + 1 << "\n");
            ArrayMove(&A[N - elements], A2, elements);
            TRACE(ArrayPrint(A, N));
        }
        TRACE(cout << endl);
    }
    TRACE(cout << "output\n");

    int a = abs(A[0] - A[N - 1]);
    cout << a << "\n";
    ArrayPrint(A, N);
    cout << endl;
    free(A), free(A2);
}


struct DoubleLinkedList
{
  int data;
  DoubleLinkedList* prev;
  DoubleLinkedList* next;
};


template<typename LinkedListT>
void LinkedListPrint(LinkedListT* begin)
{
    LinkedListT* curr = begin;
    while (curr->next != nullptr && curr->next != begin)
    {
        curr = curr->next;
        cout << curr->data << " ";
    }
    cout << "\n";
}


DoubleLinkedList* FindElementByPosition(DoubleLinkedList* begin, int i)
{
    while (i--)
    {
        begin = begin->next;
    }
    return begin;
}


void DoubleLinkedRemoveRange(DoubleLinkedList* i, DoubleLinkedList* j)
{
    DoubleLinkedList* prev = i->prev;
    DoubleLinkedList* next = j->next;
    prev->next = next;
    next->prev = prev;
}


void DoubleLinkedListMoveToFront(DoubleLinkedList* A, DoubleLinkedList* i, DoubleLinkedList* j)
{
    DoubleLinkedRemoveRange(i, j);
    DoubleLinkedList* begin = A->next;
    A->next = i;
    i->prev = A;
    j->next = begin;
    begin->prev = j;
}


void DoubleLinkedListMoveToBack(DoubleLinkedList*A, DoubleLinkedList* i, DoubleLinkedList* j)
{
    DoubleLinkedRemoveRange(i, j);
    DoubleLinkedList* end = A->prev;
    A->prev = j;
    j->next = A;
    i->prev = end;
    end->next = i;
}


void DoubleLinkedListSolution(const int N, const int M)
{
    DoubleLinkedList* A = new DoubleLinkedList;
    A->data = 0;
    A->prev = A->next = A;

    DoubleLinkedList* currItem = A;
    for (int n = 0; n < N; ++n)
    {
        DoubleLinkedList* newItem = new DoubleLinkedList;
        cin >> newItem->data;
        newItem->next = A;
        A->prev = newItem;
        newItem->prev = currItem;
        currItem->next = newItem;
        currItem = newItem;
    }

    for (int m = 0; m < M; ++m)
    {
        int type, i, j;
        cin >> type >> i >> j;
        int elements = j - i + 1;
        DoubleLinkedList* pi = FindElementByPosition(A, i);
        DoubleLinkedList* pj = FindElementByPosition(pi, j - i);
        if (type == 1)
        {
            TRACE(cout << "moving " << elements << " elements to front\n");
            DoubleLinkedListMoveToFront(A, pi, pj);
            TRACE(LinkedListPrint(A));
        }
        else if (type == 2)
        {
            TRACE(cout << "moving " << elements << " elements to back\n");
            DoubleLinkedListMoveToBack(A, pi, pj);
            TRACE(LinkedListPrint(A));
        }
        TRACE(cout << endl);
    }
    TRACE(cout << "output\n");

    int a = abs(A->next->data - A->prev->data);
    cout << a << "\n";
    LinkedListPrint(A);
    DoubleLinkedList* curr = A;
    do
    {
        DoubleLinkedList* todel = curr;
        curr = curr->next;
        delete todel;
    } while (curr->next != A);
    cout << endl;
}


int main(int argc, char* argv[])
{
    int N, M;
    cin >> N >> M;
    TRACE(cout << "\n*** running trace mode ON ***\n");
    //RawMemorySolution(N, M);
    //ArraySolution(N, M);
    DoubleLinkedListSolution(N, M);
    return 0;
}

