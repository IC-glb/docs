# include <iostream>

using namespace std;

class Animal
{
    public:
        virtual void Sound(){
            cout<<"Animal makes a sound"<<endl;
        }
};

class Dog : public Animal
{
    public:
    void Sound() override{
        std::cout << "Dog barks" << std::endl;
    }
};

class Cat : public Animal {
public:
    void Sound() override {
        std::cout << "Cat meows" << std::endl;
    }
};

int main(int argc, char const *argv[])
{
    Animal* animal;
    Animal an;
    Dog dog;
    Cat cat;


    an.Sound();

    animal = &dog;
    animal->Sound();  // 输出 "Dog barks"

    animal = &cat;
    animal->Sound();  // 输出 "Cat meows"

    return 0;
}
