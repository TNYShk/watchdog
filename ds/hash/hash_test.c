/**********************************************
 * HASH - Source File                         *
 * Developer: Tanya			                  *
 *          Feb 01, 2022                      *
 *                                            *
 * Reviewer: Ori    	                      *
 **********************************************/
#include <assert.h> /* assert */
#include <stdio.h>
#include <stdlib.h>
#include "hash.h" /* programs header*/

#define WORDSDICT (102401)
#define WORDLENMAX (26)
#define PATH ("/home/tanya/git/ds/hash/words")


void CreateDestroy();
void OccupyHotel();
static const void *GetKey(const void *data);
static size_t hash_func(const void *key);
int CompareData(const void *key1, const void *key2);
int PrintForEachString(void *value, void *param);
static void GetWords(void *word);
static size_t hash_func99(const void *key);

typedef struct dict
{
    const void *key;
    char data[WORDLENMAX];
}dict_t;

int main(void)
{
    
    CreateDestroy();
    OccupyHotel();
    
   /* GetWords("aaa");*/
    return 0;
}

/* printf("read? %ld\n", fread(dictionary[i].data, WORDLENMAX, read, fp1));*/

static void GetWords(void *word)
{
    
    size_t i = 0;
   
    hash_t *hashy = NULL;
    dict_t *dictionary = NULL;
    void *spell = NULL;

    FILE *fp1 = fopen("words", "r"); 
    
    hashy = HashCreate(999, &GetKey, &CompareData, &hash_func99);
    
    dictionary = (dict_t *)calloc(WORDSDICT,sizeof(dict_t));
    if(dictionary == NULL)
    {
        HashDestroy(hashy);
        fclose(fp1);
    }
    else
    {
       
        for(fgets(dictionary[i].data, WORDLENMAX,fp1); i < WORDSDICT; ++i )
        {
            HashInsert(hashy, &(dictionary[i].data));
            dictionary[i].key = GetKey(dictionary[i].data);
        }

        printf(" currently %ld rooms taken\n", HashSize(hashy));
        
        spell = HashFind(hashy, GetKey(word));
        (spell == NULL)? printf("bad word"): printf("found! %s\n", *(char **)&spell);

        fclose(fp1);
        free(dictionary);
        HashDestroy(hashy);
    }
   
    
}





















void CreateDestroy()
{

    hash_t *hashy = NULL;
    hashy = HashCreate(10, &GetKey, &CompareData, &hash_func);
    assert(1 == HashIsEmpty(hashy));
    assert(0 == HashSize(hashy));
    HashDestroy(hashy);

}

void OccupyHotel()
{
    hash_t *hashy = NULL;
    void *param = NULL;
    int x = 0;

    hashy = HashCreate(99, &GetKey, &CompareData, &hash_func);
    (1 == HashIsEmpty(hashy))? printf("Empty Hash Hotel\n") : printf("NOT empty Hotel\n");
    
    assert(0 == HashInsert(hashy, "tanya"));
    HashInsert(hashy, "Anya");
    HashInsert(hashy, "Fanya");
    HashInsert(hashy, "Aanya");
    HashInsert(hashy, "Banya");
    HashInsert(hashy, "Tatyanna");
    printf(" currently %ld rooms taken\n", HashSize(hashy));
    assert(HashSize(hashy) == 6);

    printf("\nCurrent Hotel Occupants:\n");
    HashForEach(hashy, &PrintForEachString, &x );
    printf("\n");

    printf("\nCheckout, 2 guests leaving\n");
    HashRemove(hashy, GetKey("tanya"));
    HashRemove(hashy, GetKey("Banya"));
    assert(HashSize(hashy) == 4);

    printf("\nCurrent Hotel Occupants:\n");
    HashForEach(hashy, &PrintForEachString, &x );
    printf("\n\nCalling Guests to Lobby:\n");
    param = HashFind(hashy, GetKey("Anya"));
    printf("found! %s\n", *(char **)&param);
    assert(param != NULL);

    param = HashFind(hashy, GetKey("tanya"));
    assert(param == NULL);
    printf("guest tanya not found -  left the building \n");
    

    HashDestroy(hashy);

}




static const void *GetKey(const void *data)
{
    size_t hash = 5381;
    int cc = 0;

    while (cc = (*(char *)data))
    {
        hash = ((hash << 5) + (hash)) + cc;
        ++(*(char **)&data);
    }

    return *(void**)&hash;
}

static size_t hash_func(const void *key)
{
    size_t hash_index = 0; /* number of rooms*/
    int cc = 0;

    hash_index = ((*(size_t *)&key) % 10);
    
    return hash_index;
}

static size_t hash_func99(const void *key)
{
    size_t hash_index = 0; /* number of rooms*/
    int cc = 0;

    hash_index = ((*(size_t *)&key) % 100);
    
    return hash_index;
}

int CompareData(const void *key1, const void *key2)
{
    return (*(size_t *)&key1 - *(size_t *)&key2);
}

int PrintForEachString(void *value, void *param)
{
    (void)param;

    printf("%s ", (char *)value);

    return 0;
}