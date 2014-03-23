#ifndef ASYNCBUFFER_INCLUDED
#define ASYNCBUFFER_INCLUDED
#include "itrbase.h"
#include <vector>
#include <QMutex>
using itr_container::CycleQueue;
using std::vector;

template<class T>
class AsyncBuffer
{
private:
    vector<T*> BufferList;
    CycleQueue<T*> BufferToRead;
    CycleQueue<T*> BufferToWrite;

    QMutex mutexRead;
    QMutex mutexWrite;

public:
    AsyncBuffer()
    {

    }
    void Init(S32 Capacity)
    {
        BufferToRead.Init(Capacity);
        BufferToWrite.Init(Capacity);
    }
    void AddBufferToList(T* obj)
    {
        BufferList.push_back(obj);
        SetBufferToWrite(obj);
    }

    void SetBufferToWrite(T* buffer)
    {
        mutexWrite.lock();
        BufferToWrite.Insert(buffer);
        mutexWrite.unlock();
    }

    T* GetBufferToWrite()
    {
        mutexWrite.lock();
        T* obj=NULL;
        BufferToWrite.Fetch(obj);
        mutexWrite.unlock();
        return obj;
    }
    T* GetBufferToRead()
    {
        mutexRead.lock();
        T* obj=NULL;
        BufferToRead.Fetch(obj);
        mutexRead.unlock();
        return obj;
    }
    void SetBufferTRead(T* buffer)
    {
        mutexRead.lock();
        BufferToRead.Insert(buffer);
        mutexRead.unlock();
    }
};


#endif // ASYNCBUFFER_INCLUDED
