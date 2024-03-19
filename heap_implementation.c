#include<stdio.h>
#include<stddef.h>

#define MAX_SIZE_OF_MEMORY 15000

char main_memory[MAX_SIZE_OF_MEMORY];

typedef struct meta_data
{
    size_t size;
    int data_status;
    struct meta_data *next;
} meta_data;

meta_data *start_ptr = (void*)(main_memory);

void initialize_memory();
void *allocate(size_t required_size);
void split_block(meta_data *temp, size_t required_size);
void deallocate(void *dptr);
void merge_block();
void print_memory();

void initialize_memory()
{   
    start_ptr->size = MAX_SIZE_OF_MEMORY - sizeof(meta_data);
    start_ptr->data_status = 1;
    start_ptr->next = NULL;
}

void *allocate(size_t required_size)
{
    meta_data *temp;
    if (start_ptr->size == 0)
    {
        printf("Initializing the memory\n");
        initialize_memory();
    }
    temp = (void*)main_memory;
    void *result = NULL;
    while ((temp->next != NULL) && (temp->size < required_size || temp->data_status == 0))
    {
        temp = temp->next;
        printf("One meta_data block checked\n");
    }
    if (temp->size == required_size)
    {
        printf("Perfect meta_data block is found\n");
        temp->data_status = 0;
        result = (void*)temp++;
    }
    else if (temp->size > required_size)
    {
        printf("Splitting the larger meta_data block\n");
        split_block(temp, required_size);
        result = (void*)temp++;
    }
    else
    {
        printf("Unable to allocate the memory\n");
    }
    return result;
}

void split_block(meta_data *temp, size_t required_size)
{
    meta_data *next_free_block = (void*)((void*)temp + required_size + sizeof(meta_data));
    next_free_block->size = temp->size - required_size - sizeof(meta_data);
    next_free_block->data_status = 1;
    temp->data_status = 0;
    temp->size = required_size;
    next_free_block->next = temp->next;
    temp->next = next_free_block;  
}

void deallocate(void *dptr)
{
    if (((void *)main_memory <= dptr) && (dptr <= (void *)(main_memory + MAX_SIZE_OF_MEMORY)))
    {
        meta_data *curr = dptr;
        curr->data_status = 1;
        printf("Memory block deallocated successfully\n");
        merge_block();
    }
    else
    {
        printf("Please provide a valid pointer allocated by My_Alloc\n");
    }
}

void merge_block()
{
    meta_data *curr;
    meta_data *prev;
    curr = (void *)start_ptr;

    while (curr != NULL && curr->next != NULL)
    {
        if (curr->data_status == 1 && curr->next->data_status == 1)
        {
            curr->size += (curr->next->size) + sizeof(meta_data);
            curr->next = curr->next->next;
        }
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }
}

void print_memory()
{
    meta_data *temp = start_ptr;
    printf("Memory Layout:\n");

    while (temp != NULL)
    {
        printf("Size: %zu, Status: %d\n", temp->size, temp->data_status);
        temp = temp->next;
    }
    printf("\n");
}

int main()
{
    initialize_memory();
    
    int *p = (int *)allocate(150 * sizeof(int));
    print_memory();
    char *q = (char *)allocate(200 * sizeof(char));
    print_memory();
    int *r = (int *)allocate(1000 * sizeof(int));
    print_memory();
    deallocate(p);
    print_memory();
    int *w = (int *)allocate(100 * sizeof(int));
    print_memory();
    int *k = (int *)allocate(500 * sizeof(int));
    print_memory();
    int *n = (int *)allocate(20 * sizeof(int));
    print_memory();
    int *f = (int *)allocate(5000 * sizeof(int));
    print_memory();
    printf("Allocation and deallocation done successfully!\n");
    return 0;
}
