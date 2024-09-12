#include "animal.h"
#include <string.h>
#include <stdio.h>


declare_construct(IMoveable) {
    func_set(cthis, move, NULL);
    printf("IMoveable construct\n");
}

declare_destruct(IMoveable) {
    printf("IMoveable destruct\n");
    return 1;
}

static void declare_func(Animal, setName, Animal *cthis, const char* name) {
    strcpy(cthis->name, name);
}

static void declare_func(Animal, setAge, Animal *cthis, int age) {
    cthis->age = age;
}

static void declare_func(Animal, say, Animal *cthis) {
    printf("Hello! I am %s, I am %d years old\n", cthis->name, cthis->age);
}

static void declare_func(Animal, init, Animal *cthis, const char* name, int age) {
    cthis->setName(cthis, name);
    cthis->setAge(cthis, age);
}

static void declare_func(Animal, eat, Animal *cthis) {
    printf("Animal need eat some thing\n");
}

static void declare_func(Animal, talk, Animal *cthis) {
    printf("Animal can talk\n");    
}

declare_construct(Animal) {
    class_construct(IMoveable, cthis, imove);
    func_init(Animal, setName);
    func_init(Animal, setAge);
    func_init(Animal, say);
    func_init(Animal, eat);
    func_init(Animal, talk);
    func_init(Animal, init);
    printf("Animal construct\n");
}


declare_destruct(Animal) {
    printf("Animal destruct\n");
    class_destruct(IMoveable, cthis, imove);
    return 1;
}

declare_new(Animal);
declare_del(Animal);

static  void declare_func(Fish, eat, Animal* cthis) {
    printf("fish eats grass\n");
}

static void declare_func(Fish, talk, Animal* cthis) {
    printf("fish talk voice\n");
}

static void declare_func(Fish, move, IMoveable* cthis) {
    printf("fish swims in water\n");
}

static void declare_func(Fish, init, Fish* cthis, const char* name, int age) {
    Animal* animal = class_mem(Animal, cthis, animal);
    animal->setName(animal, name);
    animal->setAge(animal, age);
}

declare_construct(Fish) {
    class_construct(Animal, cthis, animal);
    func_set(cthis, animal.eat, Fish_eat);
    func_set(cthis, animal.talk, Fish_talk);
    func_set(cthis, animal.imove.move, Fish_move);
    func_init(Fish, init);
    printf("Fish construct\n");
}

declare_destruct(Fish) {
    printf("Fish destruct\n");
    class_destruct(Animal, cthis, animal);
    return 1;
}

declare_new(Fish);
declare_del(Fish);


static void declare_func(Dog, eat, Animal* cthis) {
    printf("Dog eats bone\n");
}

static void declare_func(Dog, talk, Animal* cthis) {
    printf("Dog talks\n");
}

static void declare_func(Dog, move, IMoveable* cthis) {
    printf("Dog runs on ground\n");
}

static void declare_func(Dog, init, Dog* cthis, const char* name, int age) {
    Animal* animal = class_mem(Animal, cthis, animal);
    animal->setName(animal, name);
    animal->setAge(animal, age);
}

declare_construct(Dog) {
    class_construct(Animal, cthis, animal);
    func_set(cthis, animal.eat, Dog_eat);
    func_set(cthis, animal.talk, Dog_talk);
    func_set(cthis, animal.imove.move, Dog_move);
    func_init(Dog, init);
    printf("Dog construct\n");
}

declare_destruct(Dog) {
    printf("Dog destruct\n");
    class_destruct(Animal, cthis, animal);
    return 1;
}

declare_new(Dog);
declare_del(Dog);

static void declare_func(Car, move, IMoveable* cthis) {
    printf("A Car drives on road\n");
}

declare_construct(Car) {
    class_construct(IMoveable, cthis, imove);
    func_set(cthis, imove.move, Car_move);
    printf("Car construct\n");
}

declare_destruct(Car) {
    printf("Car destruct\n");
    class_destruct(IMoveable, cthis, imove);
    return 1;
}

declare_new(Car);
declare_del(Car);
