/***********************************
 * Quiz SllPalindrome -Source File *
 * Developer: Tanya                *
 *          Feb 24, 2022           *
 *                                 *
************************************/
#include <assert.h> /* asserts */
#include <stdio.h> /* printf */


typedef struct sll_node
{
    void *data;
    struct sll_node *next;
}sll_node_t;

int IsSllPalindrome(sll_node_t *head);
int SLLIsPalindrome(sll_node_t *head);
int CheckSLL(sll_node_t **, sll_node_t *);
static size_t SLLCountNodes(sll_node_t *head);
static sll_node_t *Flip(sll_node_t *head);
static int CheckFlipped(sll_node_t *head, sll_node_t *flip);


int main(void)
{
    /*int a = 2, b = 2, c = 0, d = 2, e = 2, f = 0, g = 2, h = 2;*/
    int a = 1, b = 2, c = 3, d = 5, e = 5, f = 3, g = 0, h = 1;
    
    sll_node_t *head = NULL;
    sll_node_t one;
    sll_node_t two; 
    sll_node_t three; 
    sll_node_t four; 
    sll_node_t five; 
    sll_node_t six;
    sll_node_t seven;
    sll_node_t eight;


    one.data = &a;
    one.next = &two;
    two.data = &b;
    two.next = &three;
    three.data = &c;
    three.next = &four;
    four.data = &d;
    four.next = &five;
    five.data = &e;
    five.next = &six;  
    six.data = &f;
    six.next = &seven;
    seven.data = &g;
    seven.next = &eight;
    eight.data = &h;
    eight.next = NULL;
    head = &one;


    (1 == SLLIsPalindrome(head))? printf("Palindrome!\n"): printf("NOT Palindrome\n");
     (1 == IsSllPalindrome(head))? printf("Palindrome!\n"): printf("NOT Palindrome\n");
    return 0;
}


int SLLIsPalindrome(sll_node_t *head)
{
    return CheckSLL(&head, head);
}

int CheckSLL(sll_node_t **start, sll_node_t *end)
{
    int result = 0;
    if(NULL == end)
    {
        return 1;
    }

    result =  CheckSLL(start, end->next) && ((*(int *)(*start)->data) == (*(int *)end->data));
    (*start) = (*start)->next;

    return result;
}



static size_t SLLCountNodes(sll_node_t *head)
{
    size_t count = 0;

    while(NULL != head)
    {
        ++count;
        head = head->next;
    }

    return count;
}

static sll_node_t *Flip(sll_node_t *head)
{
    sll_node_t *temp_node = NULL;
    sll_node_t *runner = NULL;
    sll_node_t *next_node = NULL;
    sll_node_t *new_head = NULL;

    assert(NULL != head);

    temp_node = NULL;
    runner = head;

    while(NULL != runner->next)
    {
        next_node = runner->next;
        runner->next = temp_node;
        temp_node = runner;
        runner = next_node;
    }

    new_head = runner;
    runner->next = temp_node;
    return new_head;

}

int IsSllPalindrome(sll_node_t *head)
{
    size_t idx = 0;
    size_t size = 0;
    int result = 0;
    sll_node_t *rev_head = head;
    
    assert(NULL != head);
    size = SLLCountNodes(head);

    size /= 2;

    for(; idx < size + 1; ++idx)
    {
        rev_head = rev_head->next;
    }
    rev_head = Flip(rev_head);

    result = CheckFlipped(head, rev_head);

    rev_head = Flip(rev_head);
    return result;

}

static int CheckFlipped(sll_node_t *head, sll_node_t *flip)
{
    while(NULL != head)
    {
        if((*(int *)head->data) == (*(int *)flip->data))
        {
            head = head->next;
            flip = flip->next;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}