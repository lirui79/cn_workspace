#ifndef ANIMAL_H_INCLUDED_
#define ANIMAL_H_INCLUDED_

#include "object.h"


#ifdef __cplusplus
extern "C"{
#endif



class(Animal) {
    Object   uper;
    char     name[256];
    int      age;
};

class_new(Animal);

class_del(Animal);

class(AnimalClass) {
    ObjectClass  uperClass;
    void class_func(Animal, setName, const char *name);
    void class_func(Animal, setAge, int age);
    void class_func(Animal, say);
    void class_func(Animal, eat);
    void class_func(Animal, talk);
    void class_func(Animal, init, const char* name, int age);
};

class_new(AnimalClass);

class_del(AnimalClass);

//////////////////////////////////////////////////////////////

class(Fish) {
    Animal     animal;
};

class_new(Fish);

class_del(Fish);


class(FishClass) {
    AnimalClass  animalClass;
    void class_func(Fish, init, const char* name, int age);
    void class_func(Fish, move);
};

class_new(FishClass);

class_del(FishClass);

//////////////////////////////////////////////////////////////


class(Dog) {
    Animal     animal;
};

class_new(Dog);

class_del(Dog);

class(DogClass) {
    AnimalClass animalClass;
    void class_func(Dog, init, const char* name, int age);
    void class_func(Dog, move);
};

class_new(DogClass);

class_del(DogClass);



#ifdef __cplusplus
}
#endif

#endif