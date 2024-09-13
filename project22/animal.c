#include "animal.h"
#include <string.h>
#include <stdio.h>



declare_construct(Animal) {
    class_construct(Object, cthis, uper);
    printf("Animal construct\n");
}


declare_destruct(Animal) {
    printf("Animal destruct\n");
    class_destruct(Object, cthis, uper);
    return 1;
}

declare_new2(Animal, AnimalClass);

declare_del2(Animal, AnimalClass);


static void declare_func(AnimalClass, setName, Animal *cthis, const char* name) {
    strcpy(cthis->name, name);
}

static void declare_func(AnimalClass, setAge, Animal *cthis, int age) {
    cthis->age = age;
}

static void declare_func(AnimalClass, say, Animal *cthis) {
    printf("Hello! I am %s, I am %d years old\n", cthis->name, cthis->age);
}

static void declare_func(AnimalClass, eat, Animal *cthis) {
    printf("Animal need eat some thing\n");
}

static void declare_func(AnimalClass, talk, Animal *cthis) {
    printf("Animal can talk\n");    
}

static void declare_func(AnimalClass, init, Animal *cthis, const char* name, int age) {
    AnimalClass* animalClass = class_ptr2(AnimalClass, cthis);
    animalClass->setName(cthis, name);
    animalClass->setAge(cthis, age);
}


declare_construct(AnimalClass) {
    class_construct(ObjectClass, cthis, uperClass);
    func_init(AnimalClass, setName);
    func_init(AnimalClass, setAge);
    func_init(AnimalClass, say);
    func_init(AnimalClass, eat);
    func_init(AnimalClass, talk);
    func_init(AnimalClass, init);
    printf("AnimalClass construct\n");
}


declare_destruct(AnimalClass) {
    printf("AnimalClass destruct\n");
    class_destruct(ObjectClass, cthis, uperClass);
    return 1;
}

declare_new(AnimalClass);
declare_del(AnimalClass);


//////////////////////////

declare_construct(Fish) {
    class_construct(Animal, cthis, animal);
    printf("Fish construct\n");
}


declare_destruct(Fish) {
    printf("Fish destruct\n");
    class_destruct(Animal, cthis, animal);
    return 1;
}

declare_new2(Fish, FishClass);

declare_del2(Fish, FishClass);



static  void declare_func(FishClass, eat, Animal* cthis) {
    printf("fish eats grass\n");
}

static void declare_func(FishClass, talk, Animal* cthis) {
    printf("fish talk voice\n");
}

static void declare_func(FishClass, move, Fish* cthis) {
    printf("fish swims in water\n");
}

static void declare_func(FishClass, init, Fish* cthis, const char* name, int age) {
    AnimalClass* animalClass = class_ptr2(AnimalClass, cthis);
    animalClass->init(class_ptr(Animal, cthis), name, age);
}

declare_construct(FishClass) {
    class_construct(AnimalClass, cthis, animalClass);
    func_set(cthis, animalClass.eat, FishClass_eat);
    func_set(cthis, animalClass.talk, FishClass_talk);
    func_init(FishClass, init);
    func_init(FishClass, move);
    printf("FishClass construct\n");
}

declare_destruct(FishClass) {
    printf("FishClass destruct\n");
    class_destruct(AnimalClass, cthis, animalClass);
    return 1;
}

declare_new(FishClass);
declare_del(FishClass);

//////////////////////////

declare_construct(Dog) {
    class_construct(Animal, cthis, animal);
    printf("Dog construct\n");
}


declare_destruct(Dog) {
    printf("Dog destruct\n");
    class_destruct(Animal, cthis, animal);
    return 1;
}

declare_new2(Dog, DogClass);

declare_del2(Dog, DogClass);


static void declare_func(DogClass, eat, Animal* cthis) {
    printf("Dog eats bone\n");
}

static void declare_func(DogClass, talk, Animal* cthis) {
    printf("Dog talks\n");
}

static void declare_func(DogClass, move, Dog* cthis) {
    printf("Dog runs on ground\n");
}

static void declare_func(DogClass, init, Dog* cthis, const char* name, int age) {
    AnimalClass* animalClass = class_ptr2(AnimalClass, cthis);
    animalClass->setName(class_ptr(Animal, cthis), name);
    animalClass->setAge(class_ptr(Animal, cthis), age);
}

declare_construct(DogClass) {
    class_construct(AnimalClass, cthis, animalClass);
    func_set(cthis, animalClass.eat, DogClass_eat);
    func_set(cthis, animalClass.talk, DogClass_talk);
    func_init(DogClass, move);
    func_init(DogClass, init);
    printf("DogClass construct\n");
}

declare_destruct(DogClass) {
    printf("DogClass destruct\n");
    class_destruct(AnimalClass, cthis, animalClass);
    return 1;
}

declare_new(DogClass);
declare_del(DogClass);
