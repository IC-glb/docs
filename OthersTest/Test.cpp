#include <iostream>
#include <cstdlib>

/**
 * @brief   通过malloc()和free()来演示C++中的动态内存分配
 * @note    malloc()成功时返回一个void指针，否则返回NULL
 * @return  0
 */
int main()
{

    int *p = (int *)malloc(sizeof(int));
    *p = 10;
    std::cout << *p << std::endl;
}
