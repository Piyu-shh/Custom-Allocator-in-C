#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define HEAP_CAPACITY 640000
#define CHUNK_LIST_CAPACITY 1024

typedef struct{
    void * start_ptr;
    size_t size;
}Heap_Chunk;

typedef struct 
{
    size_t count;
    Heap_Chunk list[CHUNK_LIST_CAPACITY];

}Heap_Chunk_List;



char heap[HEAP_CAPACITY]={0};
size_t heap_size=0;

//array to store the meta data of the allocated heap i.e. the pointer and the size of allocated memory
Heap_Chunk_List heap_allocated={0};


//array to store ptr which is free
Heap_Chunk_List heap_freed={.count=1,.list={[0]={.start_ptr=heap, .size=sizeof(heap)}}};

//temporary array to used uding merging freed ptrs 
Heap_Chunk_List heap_temp ={0};



int chunk_start_compare(const void *a, const void *b) {
    const Heap_Chunk *chunkA = (const Heap_Chunk *)a;
    const Heap_Chunk *chunkB = (const Heap_Chunk *)b;

    if (chunkA->start_ptr < chunkB->start_ptr) return -1;
    if (chunkA->start_ptr > chunkB->start_ptr) return 1;
    return 0;
}




int heap_chunk_list_find(const Heap_Chunk_List *chunk_list, void * ptr){

    for(int i =0;i<chunk_list->count;i++){
        if(ptr==chunk_list->list[i].start_ptr){
            return i;
        }
    }
    return -1;
}

void heap_chunk_list_merge(Heap_Chunk_List *chunk_list, Heap_Chunk_List *temp){

    temp->count=0;

    for (size_t i = 0; i < chunk_list->count; i++)  
    {
        Heap_Chunk chunk=chunk_list->list[i];
        if(temp->count>0){
            Heap_Chunk *top_chunk=&temp->list[temp->count-1];
            if(top_chunk->start_ptr+top_chunk->size==chunk.start_ptr){
                top_chunk->size+=chunk.size;
            }else{
                heap_chunk_list_insert(temp,chunk.start_ptr,chunk.size);
            }
        }
        else{
            heap_chunk_list_insert(temp,chunk.start_ptr,chunk.size);
        }
    }
    

}

void heap_chunk_list_insert(Heap_Chunk_List *chunk_list, void* start_ptr , size_t size){
    
    if(chunk_list->count<CHUNK_LIST_CAPACITY){
        chunk_list->list[chunk_list->count].start_ptr=start_ptr;
        chunk_list->list[chunk_list->count].size=size;

        for(size_t i =chunk_list->count;i>0 && chunk_list->list[i].start_ptr < chunk_list->list[i-1].start_ptr;i-- ){
            Heap_Chunk temp=chunk_list->list[i];
            chunk_list->list[i]=chunk_list->list[i-1];
            chunk_list->list[i-1]=temp;
        }

        chunk_list->count++;
    }
}

void heap_chunk_list_remove(Heap_Chunk_List *chunk_list, size_t index){

    assert(index<chunk_list->count);

    //check: if i remove this printf line , the lines inside the loop dont print 
    //printf("Removing index: %zu from chunk list with count: %zu\n", index, chunk_list->count);
    //fflush(stdout);
    for (size_t i = index; i < chunk_list->count-1; i++)
    {
              chunk_list->list[i]=chunk_list->list[i+1];
              //printf("removing pr : %p\n",chunk_list->list[i].start_ptr);
              //fflush(stdout);
    }
    chunk_list->count--;
    

    
}

//print all the heaps LIST data
void heap_chunk_list_dump(Heap_Chunk_List *chunk_list){

    printf("Total Allocated Chunks: %d \n\n",chunk_list->count);

    for(int i = 0;i<chunk_list->count;i++){
        printf("Start : %p , Size : %zu\n",chunk_list->list[i].start_ptr,chunk_list->list[i].size);
    }
}

void *heap_alloc(size_t size){

    heap_chunk_list_merge(&heap_freed,&heap_temp);
    heap_freed=heap_temp;

    if(size>0){
        for(size_t i=0;i<heap_freed.count;i++){
            if(heap_freed.list[i].size>=size){
                Heap_Chunk chunk=heap_freed.list[i];
                heap_chunk_list_remove(&heap_freed,i);
                size_t tail_size=chunk.size -size;
                if(tail_size>0){
                    heap_chunk_list_insert(&heap_freed,(char*)chunk.start_ptr+size,tail_size);
                }
                heap_chunk_list_insert(&heap_allocated,chunk.start_ptr,size);
                return chunk.start_ptr;
            }
        }
    }

        return NULL;

}



void *heap_free(void *ptr){

    if(ptr!=NULL){
    int index=heap_chunk_list_find(&heap_allocated,ptr);
    assert(index>=0);

    heap_chunk_list_insert(&heap_freed,heap_allocated.list[index].start_ptr,heap_allocated.list[index].size);
    heap_chunk_list_remove(&heap_allocated,index);
    }
    

}

void *heap_collect(){
    return NULL;
}

int main(){

    char* ptr[10];

    for (int i = 0; i < 10; i++)
    {
        ptr[i] =heap_alloc(1);
        

    }

    for (int i = 0; i < 10; i++)
    {
        if(i%2==0)
            heap_free(ptr[i]);
        

    }

printf("\n=== ALLOCATED ===\n");


heap_chunk_list_dump(&heap_allocated);

printf("\n=== FREED ===\n");
heap_chunk_list_dump(&heap_freed);
    

    return 0;
}