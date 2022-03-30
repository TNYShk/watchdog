#include <stdlib.h>  /* size_t */
#include <stdio.h> /* printf*/
#include <string.h> /* memset*/

typedef struct object object_t;
typedef struct class class_t;
typedef struct animal animal_t;
typedef struct dog dog_t;
typedef struct cat cat_t;
typedef struct legendary_animal la_t;

typedef void (*vf_t)(void *);
typedef char* (*vfchar_t)(void *);  


enum func_names
{
    ToString = 0,
    HashCode = 1,
    Finalize = 2,
    SeyHello = 3,
    ShowCounter = 4,
    GetNumMaster = 5
};

enum boolean
{
    FALSE = 0,
    TRUE = 1
};

struct class
{
    char *name;
    size_t class_size;
    class_t* parent;
    vf_t (*vtable)[];
};

struct object
{
    class_t *meta;
};

struct animal
{
    object_t object;
    int num_legs;
    int num_masters;
    int id;    
};

struct dog
{
    animal_t animal;
    int num_legs;
};

struct cat
{
    animal_t animal;
    char *colors;
    int num_masters;
};

struct legendary_animal
{
    cat_t cat;
};

char *ObjectToString(void *obj);
size_t ObjectHashCode(void *obj);
void ObjectFinalize(void *obj);
object_t *ObjectCreate(class_t *meta);

void AnimalStaticBlock(void);

void AnimalNonStaticBlock(void);
void AnimalLoad(void);

void AnimalCtor(animal_t *this);
void AnimalCtorInt(animal_t *this, int);

void AnimalSeyHello(void *);
void AnimalShowCounter(void *);
int AnimalGetNumMaster(void *);
char *AnimalToString(void *);
void AnimalFinalize(void *);

void DogCtor(dog_t *this);
void DogStaticBlock(void);
void DogNonStaticBlock(void);

void DogSeyHello(void *obj);
char *DogToString(void *obj);
void DogFinalize(void *obj);

void CatCtor(cat_t *this);
void CatStaticBlock(void);
void CatCtorColor(cat_t *this, char *color);
void CatFinalize(void *obj);
char *CatToString(void *obj);

void LACtor(la_t *this);
void LAStaticBlock(void);
void LASeyHello(void *obj);
void LAFinalize(void *obj);
char *LAToString(void *obj);

void foo(animal_t *obj);
static void Test();
 
static int AnimalCounter = FALSE;
static int static_flag_animal = FALSE;
static int static_flag_animal1 = FALSE;
static int static_flag_dog = FALSE;
static int static_flag_cat = FALSE;
static int static_flag_la = FALSE;


char buffer[BUFSIZ] = {0};
static size_t hash_code = 0;
animal_t *animal_arr[5] = {0}; 


vf_t object_vt[] = {(vf_t)&ObjectToString, (vf_t)&ObjectHashCode, &ObjectFinalize};

vf_t animal_vt[] = {(vf_t)&AnimalToString, (vf_t)&ObjectHashCode, &AnimalFinalize, 
                    &AnimalSeyHello, &AnimalShowCounter, (vf_t)&AnimalGetNumMaster};
vf_t dog_vt[] = {(vf_t)&DogToString, (vf_t)&ObjectHashCode, &DogFinalize, 
                 &DogSeyHello, &AnimalShowCounter, (vf_t)&AnimalGetNumMaster};
vf_t cat_vt[] = {(vf_t)&CatToString, (vf_t)&ObjectHashCode, &CatFinalize, 
                &AnimalSeyHello, &AnimalShowCounter, (vf_t)&AnimalGetNumMaster};
vf_t la_vt[] = {(vf_t)&LAToString, (vf_t)&ObjectHashCode, &LAFinalize, 
                &LASeyHello, &AnimalShowCounter, (vf_t)&AnimalGetNumMaster};

class_t Object_metadata = {"Object", sizeof(object_t), NULL, &object_vt};
class_t animal_metadata = {"Animal", sizeof(animal_t), &Object_metadata, &animal_vt};
class_t dog_metadata = {"Dog", sizeof(dog_t), &animal_metadata, &dog_vt};
class_t cat_metadata = {"Cat", sizeof(cat_t), &animal_metadata, &cat_vt};
class_t la_metadata = {"LegendaryAnimal", sizeof(la_t), &cat_metadata, &la_vt};

int main(void)
{
   Test();

    return 0;
}

static void Test()
{
     size_t i = 0;
    animal_t *animal = NULL;
    dog_t *dog = NULL;
    cat_t *cat = NULL;
    la_t *la = NULL;
    

    animal = (animal_t *)ObjectCreate(&animal_metadata);
    AnimalCtor(animal);

    dog = (dog_t *)ObjectCreate(&dog_metadata);
    DogCtor(dog);

    cat = (cat_t *)ObjectCreate(&cat_metadata);
    CatCtor(cat);

    la = (la_t *)ObjectCreate(&la_metadata);
    LACtor(la);

    (*animal->object.meta->vtable)[4]((void *)animal); 

    printf("%d\n", animal->id);
    printf("%d\n", ((animal_t *)dog)->id);
    printf("%d\n", ((animal_t *)cat)->id);
    printf("%d\n", ((animal_t *)la)->id);

    animal_arr[0] = (animal_t *)ObjectCreate(&dog_metadata);
    DogCtor((dog_t *)animal_arr[0]);
    animal_arr[1] = (animal_t *)ObjectCreate(&cat_metadata);
    CatCtor((cat_t *)animal_arr[1]);
    animal_arr[2] = (animal_t *)ObjectCreate(&cat_metadata);
    CatCtorColor((cat_t *)animal_arr[2], "white");
    animal_arr[3] = (animal_t *)ObjectCreate(&la_metadata);
    LACtor((la_t *)animal_arr[3]);
    animal_arr[4] = (animal_t *)ObjectCreate(&animal_metadata);
    AnimalCtor((animal_t *)animal_arr[4]);

    for (i = 0; i < 5; ++i)
    {
        (*animal_arr[i]->object.meta->vtable)[SeyHello]((void *)animal_arr[i]);
        printf("%d\n", ((vfchar_t (*))(*animal_arr[i]->object.meta->vtable))[GetNumMaster]((void *)animal_arr[i]));
    }

    for (i = 0; i < 5; ++i)
    {
        foo(animal_arr[i]);
    }

    for (i = 0; i < 5; ++i)
    {
        (*animal_arr[i]->object.meta->vtable)[Finalize]((void *)animal_arr[i]);
    }

    (*animal->object.meta->parent->vtable)[Finalize](animal);
    (*dog->animal.object.meta->parent->parent->vtable)[Finalize](dog);
    (*cat->animal.object.meta->parent->parent->vtable)[Finalize](cat);
    (*la->cat.animal.object.meta->parent->parent->parent->vtable)[Finalize](la);
}

void foo(animal_t *obj)
{
    printf("%s\n", ((vfchar_t (*))(*obj->object.meta->vtable))[ToString]((void *)obj));
}

char *ObjectToString(void *obj)
{
    char *name = NULL;
    char buufer[24] = {0};
    char *toString = NULL;

    memset(buffer, 0 ,BUFSIZ);
    memset(buufer,0, 24);

    name = ((object_t *)obj)->meta->name;

    strcpy(buffer, name);
    strcat(buffer, "@");
    sprintf(buufer, "%p", obj);
    strcat(buffer, buufer);
    
    toString = buffer;
    return toString;      
}

size_t ObjectHashCode(void *obj)
{
    static size_t hash = 31; 
    printf("ObjectHashCode");
    hash <<=  ((animal_t *)obj)->id;
    return hash;
}

void ObjectFinalize(void *obj)
{

    if (NULL != obj)
    {
        free(obj); obj = NULL;
    }
}

object_t *ObjectCreate(class_t *meta)
{
    object_t *obj = (object_t *)calloc(meta->class_size, sizeof(size_t));
    if (NULL == obj)
    {
        return NULL;
    }

    obj->meta = meta;
    return obj;
}

void AnimalStaticBlock(void)
{
    if(!static_flag_animal)
    {
        printf("Static block Animal 1\n");
         printf("Static block Animal 2\n");
        static_flag_animal = TRUE;
    }
}

void AnimalStaticBlock2(void)
{
    if (!static_flag_animal1)
    {
        printf("Static block Animal 2\n");
        static_flag_animal1 = TRUE;
    }
}

void AnimalNonStaticBlock(void)
{
    printf("Instance initialization block Animal\n");
}

void AnimalCtor(animal_t *this)
{
    AnimalLoad();
    printf("Animal Ctor\n");

    this->id = ++AnimalCounter;
    this->num_legs = 5;
    this->num_masters = 1;

    (*this->object.meta->vtable)[SeyHello]((void *)this);
    (*this->object.meta->vtable)[ShowCounter]((void *)this);
    
    printf("%s\n", ((vfchar_t (*))(*this->object.meta->vtable))[ToString]((void *)this));
    
    printf("%s\n", ((vfchar_t (*))object_vt)[ToString](this));
}

void AnimalCtorInt(animal_t *this, int num_masters)
{
    AnimalLoad();
    printf("Animal Ctor int\n");
    this->id = ++AnimalCounter;
    this->num_masters = num_masters;
    this->num_legs = 5;
}

void AnimalLoad(void)
{
    AnimalStaticBlock();

   

    AnimalNonStaticBlock();
}

void AnimalSeyHello(void *obj)
{
    printf("Animal Hello!\n");
    printf("I have %d legs\n", ((animal_t *)obj)->num_legs);
}

void AnimalShowCounter(void *obj)
{
    (void)obj;
    printf("counter %d\n", AnimalCounter);
}

int AnimalGetNumMaster(void *obj)
{
    return ((animal_t *)obj)->num_masters;
}

char *AnimalToString(void *obj)
{
    char buufer[2048] = {'\0'};
    char *toString = NULL;
    
    memset(buffer, 0, BUFSIZ);
    memset(buufer, 0, 2048);
    
   

    strcpy(buffer, "Animal with ID: ");
    sprintf(buufer, "%d", ((animal_t *)obj)->id);
    strcat(buffer, buufer);
    
    toString = buffer;
    return toString;  
}

void AnimalFinalize(void *obj)
{
    printf("finalize Animal with ID: %d\n", ((animal_t *)obj)->id);
    ((*((animal_t *)obj)->object.meta->parent->vtable)[Finalize](obj));
}

void DogStaticBlock(void)
{
    if(!static_flag_dog)
    {
        printf("Static block Dog\n");
        static_flag_dog = TRUE;
    }
}

void DogNonStaticBlock(void)
{
    printf("Instance initialization block Dog\n");
}

void DogSeyHello(void *obj)
{
    printf("Dog Hello!\n");
    printf("I hava %d legs\n", ((dog_t *)obj)->animal.num_legs);
}

char *DogToString(void *obj)
{
    char buufer[16] = {'\0'};
    char *toString = NULL;
   
    memset(buffer, 0, BUFSIZ);
    memset(buufer, 0, 16);
    
    strcpy(buffer, "Dog with ID: ");
    sprintf(buufer, "%d", ((dog_t *)obj)->animal.id);
    strcat(buffer, buufer);
    
    toString = buffer;
    return toString;  
}

void DogFinalize(void *obj)
{
    printf("finalize Animal with ID: %d\n", ((dog_t *)obj)->animal.id);
    ((*((dog_t *)obj)->animal.object.meta->parent->parent->vtable)[Finalize](obj));
}

void DogCtor(dog_t *this)
{
    DogStaticBlock();
    AnimalCtorInt(&this->animal, 2);
    this->num_legs = 4;
    this->animal.num_legs = 4;
    DogNonStaticBlock();

    printf("Dog Ctor\n");
}

void CatCtor(cat_t *this)
{
    CatStaticBlock();
    CatCtorColor(this, "black");
    printf("Cat Ctor\n");
    this->num_masters = 2;
}

void CatCtorColor(cat_t *this, char *color)
{
    AnimalCtor(&this->animal);
    this->colors = color;
    printf("Cat Ctor with color: %s\n", this->colors);
}

void CatStaticBlock(void)
{
    if (!static_flag_cat)
    {
        printf("Static block Cat\n");
        static_flag_cat = TRUE;
    }
}

char *CatToString(void *obj)
{
    char buufer[16] = {'\0'};
    char *toString = NULL;
   
    memset(buffer, 0, BUFSIZ);
    memset(buufer, 0, 16);
    
    strcpy(buffer, "Cat with ID: ");
    sprintf(buufer, "%d", ((cat_t *)obj)->animal.id);
    strcat(buffer, buufer);
    
    toString = buffer;
    return toString;  
}

void CatFinalize(void *obj)
{
    printf("finalize Animal with ID: %d\n", ((cat_t *)obj)->animal.id);
    ((*((cat_t *)obj)->animal.object.meta->parent->parent->vtable)[Finalize](obj));
}

void LACtor(la_t *this)
{
    LAStaticBlock();
    CatCtor(&this->cat);
    printf("Legendary Ctor\n");
}

void LAStaticBlock(void)
{
    if (!static_flag_la)
    {
        printf("Static block Legendary Animal\n");
        static_flag_la = TRUE;
    }
}

void LASeyHello(void *obj)
{
    printf("%s ",((la_t *)obj)->cat.animal.object.meta->name);
    printf(" Hello!\n");
}

void LAFinalize(void *obj)
{
    printf("finalize LegendaryAnimal with ID: %d\n", ((la_t *)obj)->cat.animal.id);
    ((*((la_t *)obj)->cat.animal.object.meta->parent->parent->parent->vtable)[Finalize](obj));
}

char *LAToString(void *obj)
{
    char buufer[2048] = {'\0'};
    char *toString = NULL;
   
    memset(buffer, 0, BUFSIZ);
    memset(buufer, 0, 2048);
    
    strcpy(buffer, "LegendaryAnimal with ID: ");
    sprintf(buufer, "%d", ((la_t *)obj)->cat.animal.id);
    strcat(buffer, buufer);
    
    toString = buffer;
    return toString;  
}
