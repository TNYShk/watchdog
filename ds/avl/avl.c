/**********************************************
 * AVL - Source File                          *
 * Developer: Tanya			                  *
 *          Jan 26, 2022                      *
 *                                            *
 * Reviewer:         	                      *
 **********************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */
#include <string.h> /* memmove */
#include <stdio.h>

#include "avl.h"

#define MAX(a,b) ((a > b)? (a) : (b))


typedef enum status
{
    FAILURE = -1,
    SUCCESS = 0,
    TRUES
}status_e;


enum children
{
    LEFT,
    RIGHT,
    NUM_OF_CHILDREN
};

typedef struct avl_node avl_node_t;

struct avl_node
{
    void *data;
    size_t height; 
    avl_node_t *children[NUM_OF_CHILDREN];
};


struct avl
{
    avl_node_t *root;
    avl_cmp_func_t cmp_func;
};


static void Destroy(avl_node_t *runner);
static size_t CountNodes(avl_node_t *runner);

static avl_node_t *Createnode(void *data);
static int InsertNode(avl_node_t *new, void *n_data, avl_cmp_func_t CmpFunc);
static int HasChildren(avl_node_t *node, int child);


avl_t *AVLCreate(avl_cmp_func_t CmpFunc)
{
    avl_t *tree = NULL;

    assert(NULL != CmpFunc);

    tree = (avl_t *)calloc(1,sizeof(avl_t));
    if (NULL == tree)
    {
        return NULL;
    }

    tree->cmp_func = CmpFunc;
    
    return tree;
}


void AVLDestroy(avl_t *tree)
{

    assert(NULL != tree);
    
    if (tree->root != NULL)
    {
        avl_node_t *runner = tree->root;
        Destroy(runner);
    }

    free(tree);
    
}

static void Destroy(avl_node_t *runner)
{
    if (runner == NULL)
    {
        return;
    }
    
    Destroy(runner->children[RIGHT]);
    Destroy(runner->children[LEFT]);
    
    printf("deleted node with data %d\n", *(int *)runner->data);
    

    free(runner);
}

static size_t CountNodes(avl_node_t *runner)
{
   
    if (runner == NULL)
    {
        return 0;
    }
    
    return (CountNodes(runner->children[RIGHT]) + 1 + (CountNodes(runner->children[LEFT])));

}

size_t AVLSize(const avl_t *avl)
{
    size_t counter = 0;
    assert(NULL != avl);
    
    if (avl->root != NULL)
    {
        avl_node_t *runner = avl->root;
        return CountNodes(runner);
    }
    return counter;
}

/*
static int WhichChild(avl_node_t *parent, avl_node_t *child)
{
    return (child == parent->children[RIGHT])
}


static int HasChild(avl_node_t *parent, int child)
{
    return (NULL != parent->children[child])
}


*/

size_t AVLHeight(const avl_t *tree)
{
    assert(NULL != tree);

    return (0 == AVLIsEmpty(tree)? tree->root->height : 0 );
}




int AVLIsEmpty(const avl_t *tree)
{
    assert(NULL != tree);

    return (NULL == tree->root);
}

static avl_node_t *Createnode(void *data)
{
    avl_node_t *node = (avl_node_t *)calloc(1,sizeof(avl_node_t));
    if (NULL == node)
    {
        return NULL;
    }
    
    node->data = data;
    node->height = 1;
    printf("check\n");

    return node;
}

static int InsertNode(avl_node_t *new, void *n_data, avl_cmp_func_t CmpFunc)
{
    int where = CmpFunc(n_data, new->data);

    int status = FAILURE;

    assert(where != 0);

    if (NULL == new->children[0 < where])
    {
        new->children[0 < where] = Createnode(n_data);
        if (new->children[0 < where] != NULL)
        {
            status =  SUCCESS;
        }
    }
    else
    {
        
        status = InsertNode(new->children[0 < where],n_data, CmpFunc);
    }

    printf("test\n");
    new->height = 1 + MAX(HasChildren(new,LEFT), HasChildren(new,RIGHT));

    return status;
}

static int HasChildren(avl_node_t *node, int child)
{
   return ( (node->children[child] == NULL) ? 0 : node->children[child]->height);
}

int AVLInsert(avl_t *tree, void *n_data)
{  
   
    if(NULL == tree->root)
    {
      tree->root = (avl_node_t*)calloc(1, sizeof(avl_node_t));
      if(tree->root == NULL)
      {
        return FAILURE;
      }
        
      tree->root->data = n_data;
      tree->root->height += 1;

      return SUCCESS;
    }
    else
    {
        avl_node_t *runner = tree->root;

        printf("line 236\n");

       return InsertNode(runner,n_data, tree->cmp_func);

    }
}












