// This should have all the imports you need. Please don't import iostream.
#include <cstddef>
#include <pthread.h>
#include "memory.h"
#include <new>
#include "string.h"

#define ADDITIONAL_SIZE 24000
#define WORD "malloc"

pthread_mutex_t malloc_mutex = PTHREAD_MUTEX_INITIALIZER;
namespace snp
{
// TODO Student
typedef struct Heap
{
  const char* word_;
  bool free_;
  size_t size_of_block_;
  size_t size_;
  bool available_;
  struct Heap* next_;
}Heap;


void* system_break = NULL;
Heap* root = NULL;
size_t malloc_count = 0;

void mergeFreeBlocks()
{
  Heap* tmp = root;

  while(tmp->next_ != NULL)
  {
    if((tmp->available_ == true) && (tmp->next_->available_ == true))
    {
      Heap* temp = tmp->next_->next_;
      tmp->size_of_block_ = (tmp->size_of_block_ + tmp->next_->size_of_block_);
      tmp->size_ = (tmp->next_->size_ + tmp->size_ + 1*(sizeof(Heap))) ;
      tmp->free_ = false;
      tmp->next_ = temp;
      continue;
    }
    tmp = tmp->next_;
  }
}

Heap* splitBlocks(Heap* used_block, size_t size)
{
  Heap* tmp = used_block->next_;
  size_t address = (size_t)used_block + size*sizeof(size_t)+  1*sizeof(Heap);
  Heap* splited_block = (Heap*)address;
  splited_block->available_ = true;
  splited_block->free_ = false;
  splited_block->size_ = used_block->size_ - size ;
  splited_block->size_of_block_ = splited_block->size_*sizeof(size_t)+1*sizeof(Heap);
  splited_block->next_ = tmp;
  splited_block->word_ = WORD;

  used_block->available_ = false;
  used_block->free_ = false;
  used_block->size_ = size;
  used_block->size_of_block_ -= splited_block->size_of_block_;
  used_block->next_ = splited_block;

  return used_block;
}

Heap* checkWhichBlockIsTheBest(Heap* tmp, Heap* free_block, size_t size)
{
  if(tmp->size_ == size)
    return tmp;
  else if((free_block != NULL) && (free_block->size_of_block_ > tmp->size_of_block_)
    && (tmp->size_of_block_ > (size*(sizeof(size_t)) + 1*sizeof(Heap))))
    free_block = tmp;
  else if((free_block != NULL) && (free_block->size_of_block_ < (size*(sizeof(size_t)) + 1*sizeof(Heap)))
    && (tmp->size_of_block_ > (size*(sizeof(size_t)) + 1*sizeof(Heap))))
    free_block = tmp;
  else if((free_block != NULL) && (tmp->size_of_block_ > free_block->size_of_block_)
    && (free_block->size_of_block_ < (size*(sizeof(size_t)) + 1*sizeof(Heap))))
    free_block = tmp;
  else if ((free_block == NULL) && (tmp->size_of_block_ > (size*sizeof(size_t) + 1*sizeof(Heap))))
    free_block = tmp;
  else if((free_block == NULL) && (tmp->size_of_block_ <= (size*sizeof(size_t) +1*sizeof(Heap))))
    free_block = tmp;

  return free_block;
}

Heap* checkForfreeBlock(size_t size)
{
  Heap* free_block = NULL;
  Heap* tmp = root;
  if(tmp->available_ == true)
  {
    free_block = checkWhichBlockIsTheBest(tmp, free_block, size);
    if(free_block->size_ == size)
      return free_block;
  }

  while(tmp->next_ != NULL)
  {
    if(tmp->next_->available_ == true)
    {
      free_block = checkWhichBlockIsTheBest(tmp->next_, free_block, size);
      if(free_block->size_ == size)
        return free_block;
    }
    tmp = tmp->next_;
  }

  return free_block;
}

Heap* increaseProgramBreak(Heap* new_block, Heap* used_block, size_t size)
{
  if(used_block->next_ != NULL)
  {
    while(used_block->next_ != NULL)
      used_block = used_block->next_;
  }
  size_t new_program_break =((size_t)used_block + used_block->size_*sizeof(Heap) + size*sizeof(size_t) + 2*sizeof(Heap));
  snp::brk((void*)new_program_break);
  size_t address = (size_t)used_block + 1*sizeof(Heap) + size*sizeof(size_t);
  new_block = (Heap*)address;
  new_block->size_of_block_ = (size*sizeof(size_t) + 1*sizeof(Heap));
  new_block->word_ = WORD;
  new_block->available_ = false;
  new_block->free_ = false;
  new_block->next_ = NULL;
  new_block->size_ = size;
  used_block->next_ = new_block;
  return new_block;
}

void *Memory::malloc(size_t size)
{
  pthread_mutex_lock(&malloc_mutex);
  malloc_count++;
  pthread_mutex_unlock(&malloc_mutex);
  if(size == 0)
    return NULL;

  if(root == NULL)
  {
    system_break = snp::sbrk(0);
    root = (Heap*)snp::sbrk(2*sizeof(Heap) + size*sizeof(size_t) + 1*ADDITIONAL_SIZE);
    if(root == (void*)-1)
      return NULL;
    size_t new_address = (size_t)root +1*sizeof(Heap) + size*sizeof(size_t);
    Heap* free_block = (Heap*)new_address;
    free_block->size_ = (ADDITIONAL_SIZE/sizeof(size_t));
    free_block->next_ = NULL;
    free_block->available_ = true;
    free_block->free_ = false;
    free_block->word_ = WORD;
    free_block->size_of_block_ = ADDITIONAL_SIZE+1*sizeof(Heap);

    root->size_of_block_ = (size*sizeof(size_t) + 1*sizeof(Heap));
    root->size_ = size;
    root->available_ = false;
    root->free_ = false;
    root->next_ = free_block;
    root->word_ = WORD;
    return root + sizeof(Heap);
  }
  mergeFreeBlocks();
  Heap* used_block = checkForfreeBlock(size);

  if(used_block != NULL)
  {
    if((used_block->available_ == true) && (used_block->size_ == (size)))
    {
      used_block->free_ = false;
      used_block->available_ = false;
    }
    else if((used_block->available_ == true) && ((used_block->size_of_block_) > (size*sizeof(size_t) + 1*sizeof(Heap))))
      used_block = splitBlocks(used_block, size);
    else
    {
      Heap* new_block = NULL;
      new_block = increaseProgramBreak(new_block, used_block->next_, size);
      return new_block + sizeof(Heap);
    }
    
    return used_block + sizeof(Heap);
  }
  else
  {
    Heap* tmp = root;
    while(tmp->next_ != NULL)
      tmp = tmp->next_;
    Heap* new_heap = (Heap*)snp::sbrk(size*sizeof(size_t)+1*sizeof(Heap)+ADDITIONAL_SIZE);
    if(new_heap == (void*) -1)
      return NULL;

    size_t new_address = (size_t)new_heap +1*sizeof(Heap) + size*sizeof(size_t);
    Heap* free_block = (Heap*)new_address;
    free_block->size_ = (ADDITIONAL_SIZE/sizeof(size_t)- 1*sizeof(Heap));
    free_block->next_ = NULL;
    free_block->available_ = true;
    free_block->free_ = false;
    free_block->word_ = WORD;
    free_block->size_of_block_ = ADDITIONAL_SIZE;
    
    new_heap->size_of_block_ = (size*sizeof(size_t) + 1*sizeof(Heap));
    new_heap->available_ = false;
    new_heap->free_ = false;
    new_heap->size_ = size;
    new_heap->next_ = free_block;
    new_heap->word_ = WORD;
    tmp->next_ = new_heap;
  
    return  new_heap + sizeof(Heap);
  }

  (void) size;
  return 0;
}

void checkIfBlockCanBeFree()
{
  Heap* temp = root;
  size_t decrease_break_point = 0;
  Heap* block_end = NULL;
  size_t num_of_frees = 0;
  size_t num_of_blocks = 1;
  if((temp->available_ == true) && (temp->free_ == true))
  {
    decrease_break_point+= temp->size_of_block_;
    num_of_frees++;
  }
  while(temp->next_ != NULL)
  {
    if((temp->next_->available_ == true)&&(temp->next_->free_==true))
    {
      decrease_break_point += temp->next_->size_of_block_;
      block_end = temp;
      num_of_frees++;
    }
    else
    {
      decrease_break_point = 0;
      block_end = NULL;
    }
    num_of_blocks++;
    temp = temp->next_;
  }
  if(block_end == NULL)
    return;
  block_end->next_ = NULL;
  if(num_of_frees == num_of_blocks)
    root = NULL;

  sbrk(-decrease_break_point);

}

void Memory::free(void *ptr)
{
  Heap* temp = root;
  Heap* free_ptr = NULL;  
  if(ptr == (temp+sizeof(Heap)))
  {
    if(root->free_ == true)
      exit(-1);
    else if(root->next_ == NULL)
    {
      root->free_ = true;
      root->available_ = true;
      snp::brk(system_break);
    }
    else
    {
      root->free_ = true;
      root->available_ = true;
      checkIfBlockCanBeFree();
    }
    return;
  }
  while(temp->next_ != NULL)
  {
    free_ptr = temp->next_ + sizeof(Heap);
    if(ptr == free_ptr)
    {
      if(temp->next_->free_ == true)
        exit(-1);
      temp->next_->available_ = true;
      temp->next_->free_ = true;
      checkIfBlockCanBeFree();
      return;
    }  
    temp = temp->next_;
  } 
  
  exit(-1);
  (void) ptr;
}

void *Memory::calloc(size_t num_memb, size_t size_each)
{
  (void) num_memb;
  (void) size_each;
  return 0;
}

size_t Memory::malloc_called_count() noexcept
{
  return malloc_count;
}

size_t Memory::used_blocks_count() noexcept
{
  size_t used_blocks = 0;
 
  if(root== NULL)
    return used_blocks;
  Heap* tmp = root;
  if(tmp->available_ == false)
    used_blocks++;
  while(tmp->next_ != NULL)
  {
    if(tmp->next_->available_ == false)
      used_blocks++;
    
    tmp = tmp->next_;
  }

  return used_blocks;
}


// TODO Student End
}
