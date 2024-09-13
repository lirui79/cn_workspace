#include "object.h"
#include <string.h>
#include <stdio.h>



declare_construct(ObjectClass) {
    printf("ObjectClass construct\n");
}

declare_destruct(ObjectClass) {
    printf("ObjectClass destruct\n");
    return 1;
}

declare_new(ObjectClass);
declare_del(ObjectClass);


declare_construct(Object) {
    cthis->_objectClass = NULL;
    printf("Object construct\n");
}

declare_destruct(Object) {
    printf("Object destruct\n");
    if (cthis->_objectClass == NULL)
        return 2;
    ObjectClass_del(cthis->_objectClass);
    cthis->_objectClass = NULL;
    return 1;
}

/*
Object* Object_new() {
    ObjectClass *objClass = NULL;
    Object *cthis = (Object*)malloc(sizeof(Object));
    if(!cthis) {
        return NULL;
    }
    Object_construct(cthis);
    objClass = ObjectClass_new();
    if (!objClass) {
        Object_del(cthis);
        return NULL;
    }
    Object_set(cthis, objClass);
    return cthis;
}//*/

declare_new2(Object, ObjectClass);

declare_del(Object);


void Object_set(Object *obj, ObjectClass *objClass) {
    obj->_objectClass = objClass;
}


void Object_clear(Object *obj) {
    obj->_objectClass = NULL;
}