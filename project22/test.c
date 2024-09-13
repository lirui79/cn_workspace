
#include "animal.h"
#include <string.h>
#include <stdio.h>

int main()
{    
    DogClass  *dogClass = NULL;
    FishClass *fishClass = NULL;
    Fish* fish = Fish_new();
    Dog* dog   = Dog_new();       // 创建狗对象
    int i = 0;

    Animal* animals[2] = { 0 };     // 初始化动物容器（这里是Animal指针数组)

    dogClass = class_ptr2(DogClass, dog);
    fishClass = class_ptr2(FishClass, fish);
    // 初始化鱼对象的昵称为：小鲤鱼，年龄为：1岁
    fishClass->init(fish, "little gold fish", 1);
    fishClass->move(fish);

    // 将fish指针转型为Animal类型指针，并赋值给animals数组的第一个成员
    animals[0] = class_ptr(Animal, fish);   

    // 初始化狗对象的昵称为：牧羊犬，年龄为：2岁
    dogClass->init(dog, "big dog", 2);
    dogClass->move(dog);

    // 将dog指针转型为Animal类型指针，并赋值给animals数组的第二个成员
    animals[1]  = class_ptr(Animal, dog);    

    // 循环打印动物容器内的动物信息
    for(i=0; i<2; i++)
    {
        Animal* animal = animals[i];
        AnimalClass *animalClass = class_ptr2(AnimalClass, animal);
        animalClass->eat(animal);
        animalClass->talk(animal);
        animalClass->say(animal);
    }

    Fish_del(fish);
    Dog_del(dog);

    return 0;
}
