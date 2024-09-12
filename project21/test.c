
#include "animal.h"
#include <string.h>
#include <stdio.h>

int main()
{    
    Fish* fish = Fish_new();
    Dog* dog = Dog_new();       // 创建狗对象
    Car* car = Car_new();       // 创建车子对象

    Animal* animals[2] = { 0 };     // 初始化动物容器（这里是Animal指针数组)
    IMoveable* moveObjs[3] = { 0 }; // 初始化可移动物体容器（这里是IMoveable指针数组)

    int i = 0;                  // i和j是循环变量
    int j = 0;

    // 初始化鱼对象的昵称为：小鲤鱼，年龄为：1岁
    fish->init(fish, "little gold fish", 1);          

    // 将fish指针转型为Animal类型指针，并赋值给animals数组的第一个成员
    animals[0] = class_ptr(Animal, fish);   

    // 初始化狗对象的昵称为：牧羊犬，年龄为：2岁
    dog->init(dog, "big dog", 2);            

    // 将dog指针转型为Animal类型指针，并赋值给animals数组的第二个成员
    animals[1]  = class_ptr(Animal, dog);    

    // 将fish指针转型为IMoveable接口类型指针，并赋值给moveOjbs数组的第一个成员
    moveObjs[0] = class_ptr(IMoveable, fish);

    // 将dog指针转型为IMoveable接口类型指针，并赋值给moveOjbs数组的第二个成员
    moveObjs[1] = class_ptr(IMoveable, dog);    

    // 将car指针转型为IMoveable接口类型指针，并赋值给moveOjbs数组的第三个成员
    moveObjs[2] = class_ptr(IMoveable, car);  

    // 循环打印动物容器内的动物信息
    for(i=0; i<2; i++)
    {
        Animal* animal = animals[i];
        animal->eat(animal);
        animal->talk(animal);
        animal->say(animal);
    }

    // 循环打印可移动物体容器内的可移动物体移动方式的信息
    for(j=0; j<3; j++)
    {
        IMoveable* moveObj = moveObjs[j];
        moveObj->move(moveObj);
    }

    Fish_del(fish);
    Dog_del(dog);
    Car_del(car);

    return 0;
}
