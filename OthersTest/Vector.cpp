#include<iostream>

using namespace std;

template <typename ElemType>

class Vector
{
private:
    int length;     // 长度
    int capacity;   // 最大存储容量

protected:
    ElemType *elem;  // 存储顺序表元素数组的指针


public:
    int size()const { retrun length;}

    bool empty() const { return !length;}
};

  

int main(int argc, char const *argv[])
{
    
    return 0;
}
