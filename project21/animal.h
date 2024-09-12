#ifndef ANIMAL_H_INCLUDED_
#define ANIMAL_H_INCLUDED_

#include "class.h"


#ifdef __cplusplus
extern "C"{
#endif

class(IMoveable) {
//    void (*move)(IMoveable* cthis);//         // Move行为
    void class_func(IMoveable, move);
//void (*speed)(IMoveable* cthis);//         // Move行为
    void func_ptr(speed, IMoveable* cthis);
};


class(Animal) {
    IMoveable  imove;

    char name[128];     // 动物的昵称(假设小于128个字符)
    int  age;            // 动物的年龄
    
    void class_func(Animal, setName, const char *name);
    void func_ptr(setAge, Animal *cthis, int age);
//    void (*setName)(Animal *cthis, const char *name);
//    void (*setAge)(Animal *cthis, int age);
    void (*say)(Animal *cthis);
    void (*eat)(Animal *cthis);
    void (*talk)(Animal *cthis);
    void (*init)(Animal *cthis, const char* name, int age);
};

class_new(Animal);

class_del(Animal);


class(Fish)
{
    Animal     animal;        // 继承Animal抽象类

    void (*init)(Fish *cthis, const char* name, int age);
};

class_new(Fish);

class_del(Fish);


class(Dog)
{
    Animal     animal;        // 继承Animal抽象类

    void (*init)(Dog *cthis, const char* name, int age);
};

class_new(Dog);

class_del(Dog);


class(Car)
{
    IMoveable  imove;         // 实现IMoveable接口
};

class_new(Car);

class_del(Car);


#ifdef __cplusplus
}
#endif

#endif