#include "mallocator.h"
#include "mtallocator.h"
#include "pallocator.h"
#include "allocator.h"
#include <vector>
#include <iostream>
#include <chrono>

#define INT 1
#define FLOAT 2
#define DOUBLE 3
#define CLASS  4

#ifndef MyAllocator
    #define MyAllocator std::allocator
#endif

class vecWrapper
{
public:
	vecWrapper() {
		m_pVec = NULL;
		m_type = INT;
	}
	virtual ~vecWrapper()	{
	}
public:
	void setPointer(int type, void *pVec) { m_type = type; m_pVec = pVec; }
	virtual void visit(int index) = 0;
	virtual int size() = 0;
	virtual void resize(int size) = 0;
	virtual bool checkElement(int index, void *value) = 0;
	virtual void setElement(int idex, void *value) = 0;
protected: 
	int m_type;
	void *m_pVec;
};

template<class T>
class vecWrapperT : public vecWrapper
{
public:
	vecWrapperT(int type, std::vector<T, MyAllocator<T> > *pVec)
	{ 
		m_type = type;
		m_pVec = pVec;
	}
	virtual ~vecWrapperT() {
		if (m_pVec)
			delete ((std::vector<T, MyAllocator<T> > *)m_pVec);
	}
public:
	virtual void visit(int index)
	{
		[[maybe_unused]] T temp = (*(std::vector<T, MyAllocator<T> > *)m_pVec)[index];
	}
	virtual int size()
	{
		return ((std::vector<T, MyAllocator<T> > *)m_pVec)->size();
	}
	virtual void resize(int size)
	{
		((std::vector<T, MyAllocator<T> > *)m_pVec)->resize(size);
	}
	virtual bool checkElement(int index, void *pValue)
	{
		T temp = (*(std::vector<T, MyAllocator<T> > *)m_pVec)[index];
		if (temp == (*((T *)pValue)))
			return true;
		else
			return false;
	}

	virtual void setElement(int index, void *value)
	{
		(*(std::vector<T, MyAllocator<T> > *)m_pVec)[index] = *((T *)value);
	}
};

class myObject
{
public:
	myObject() : m_X(0), m_Y(0) {}
	myObject(int t1, int t2) :m_X(t1), m_Y(t2) {}
	myObject(const myObject &rhs) { m_X = rhs.m_X; m_Y = rhs.m_Y;}
	~myObject() { /*std::cout << "my object destructor called" << std::endl;*/ }
	bool operator == (const myObject &rhs)
	{
		if ((rhs.m_X == m_X) && (rhs.m_Y == m_Y))
			return true;
		else
			return false;
	}

    void output() {
        std::cout << m_X << " " << m_Y << std::endl;
    }
protected:
	int m_X;
	int m_Y;
};

class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    std::chrono::duration<double, std::milli> duration;

public:
    Timer() {
        start_time = std::chrono::high_resolution_clock::now();
    }
    ~Timer() {
        end_time = std::chrono::high_resolution_clock::now();
        duration = end_time - start_time;
        std::cout << "Time elapsed: " << duration.count() << " ms" << std::endl;
    }
};

#define TESTSIZE 10000

int main()
{
	vecWrapper **testVec;
	testVec = new vecWrapper*[TESTSIZE];

    Timer *timer = new Timer();

    int TEST_TIMES = 1;

    while (TEST_TIMES--) {
        int tIndex, tSize;
        //test allocator
        for (int i = 0; i < TESTSIZE - 4; i++)
        {
            tSize = (int)((float)rand()/(float)RAND_MAX * 100) + 1;
            vecWrapperT<int> *pNewVec = new vecWrapperT<int>(INT, new std::vector<int, MyAllocator<int>>(tSize));
            testVec[i] = (vecWrapper *)pNewVec;
        }

        for (int i = 0; i < 4; i++)
        {
            tSize = (int)((float)rand() / (float)RAND_MAX * 10000) + 1;
            vecWrapperT<myObject> *pNewVec = new vecWrapperT<myObject>(CLASS, new std::vector<myObject, MyAllocator<myObject>>(tSize));
            testVec[TESTSIZE - 4 + i] = (vecWrapper *)pNewVec;
        }
        
        //test resize
        for (int i = 0; i < 100; i++)
        {
            tIndex = (int)((float)rand() / (float)RAND_MAX * (float)TESTSIZE);
            tSize = (int)((float)rand() / (float)RAND_MAX * (float)TESTSIZE) + 1;
            testVec[tIndex]->resize(tSize);
        }

        //test assignment
        tIndex = (int)((float)rand() / (float)RAND_MAX * (TESTSIZE - 4 - 1));
        int tIntValue = 10;
        testVec[tIndex]->setElement(testVec[tIndex]->size()/2, &tIntValue);
        if (!testVec[tIndex]->checkElement(testVec[tIndex]->size()/ 2, &tIntValue))
            std::cout<< "incorrect assignment in vector %d\n" << tIndex << std::endl;

        tIndex = TESTSIZE - 4 + 3;
        myObject tObj(11,15);
        testVec[tIndex]->setElement(testVec[tIndex]->size()/2, &tObj);
        if (!testVec[tIndex]->checkElement(testVec[tIndex]->size()/2, &tObj))
            std::cout << "incorrect assignment in vector %d\n" << tIndex << std::endl;
        
        myObject tObj1(13, 20);
        if (!testVec[tIndex]->checkElement(testVec[tIndex]->size() / 2, &tObj1))
            std::cout << "incorrect assignment in vector " << tIndex << " for object (13,20)" << std::endl;

        for (int i = 0; i < TESTSIZE; i++)
            delete testVec[i];
    }

    delete timer;

	delete []testVec;
    return 0;
	
}

