#include "animal.h"
#include <string.h>
#include <stdio.h>


CTDE(IMoveable)
FUNC_SET(cthis, move, NULL);
ETOR

DTDE(IMoveable)
ETOR

static void Animal_setName(Animal* t, const char* name) {
    strcpy(t->name, name);
}

static void Animal_setAge(Animal* t, int age) {
    t->age = age;
}

static void Animal_say(Animal* t) {
    printf("Hello! I am %s, I am %d years old\n", t->name, t->age);
}

static void Animal_init(Animal* t, const char* name, int age) {
    t->setName(t, name);
    t->setAge(t, age);
}

static void Animal_eat(Animal* t) {
    printf("Animal need eat some thing\n");
}

static void Animal_talk(Animal* t) {
    printf("Animal can talk\n");    
}

CTDE(Animal)
FUNC_SET(cthis, setName, Animal_setName);
FUNC_SET(cthis, setAge, Animal_setAge);
FUNC_SET(cthis, say, Animal_say);
FUNC_SET(cthis, eat, Animal_eat);
FUNC_SET(cthis, talk, Animal_talk);
FUNC_SET(cthis, init, Animal_init);
ETOR

DTOR(Animal)
ETOR

static void Fish_eat(Animal* t) {
    printf("fish eats grass\n");
}

static void Fish_talk(Animal* t) {
    printf("fish talk voice\n");
}

static void Fish_move(IMoveable* t) {
    printf("fish swims in water\n");
}

static void Fish_init(Fish* t, const char* name, int age) {
    Animal* animal = UPER_PTR(Animal, t, animal);
    animal->setName(animal, name);
    animal->setAge(animal, age);
}

CTOR(Fish)
CALL_CTOR(Animal, cthis, animal);
CALL_CTOR(IMoveable, cthis, imove);
FUNC_SET(cthis, animal.eat, Fish_eat);
FUNC_SET(cthis, animal.talk, Fish_talk);
FUNC_SET(cthis, imove.move, Fish_move);
FUNC_SET(cthis, init, Fish_init);
ETOR

DTOR(Fish)
CALL_DTOR(Animal, cthis, animal);
CALL_DTOR(IMoveable, cthis, imove);
ETOR

static void Dog_eat(Animal* t) {
    printf("Dog eats bone\n");
}

static void Dog_talk(Animal* t) {
    printf("Dog talks\n");
}

static void Dog_move(IMoveable* t) {
    printf("Dog runs on ground\n");
}

static void Dog_init(Dog* t, const char* name, int age) {
    Animal* animal = UPER_PTR(Animal, t, animal);
    animal->setName(animal, name);
    animal->setAge(animal, age);
}

CTOR(Dog)
CALL_CTOR(Animal, cthis, animal);
CALL_CTOR(IMoveable, cthis, imove);
FUNC_SET(cthis, animal.eat, Dog_eat);
FUNC_SET(cthis, animal.talk, Dog_talk);
FUNC_SET(cthis, imove.move, Dog_move);
FUNC_SET(cthis, init, Dog_init);
ETOR

DTOR(Dog)
CALL_DTOR(Animal, cthis, animal);
CALL_DTOR(IMoveable, cthis, imove);
ETOR

static void Car_move(IMoveable* t)
{
    printf("A Car moves on road\n");
}

CTOR(Car)
CALL_CTOR(IMoveable, cthis, imove);
FUNC_SET(cthis, imove.move, Car_move);
ETOR

DTOR(Car)
CALL_DTOR(IMoveable, cthis, imove);
ETOR