#ifndef SNP_MEMORY_H_
#define SNP_MEMORY_H_

#include "unistd.h"
#include "stdlib.h"

namespace snp {
  class Memory
  {
  public:
    // DO NOT MODIFY
    static void *calloc(size_t num_memb, size_t size_each);
    static void *malloc(size_t size);
    static void free(void *ptr);

    // DO NOT MODIFY
    static void *_new(size_t size);
    static void _delete(void *ptr);

    // Feel free to add new members to this class.

  };

  // DO NOT MODIFY
  void* sbrk(intptr_t __delta);
  int brk(void* __addr);
}




// DO NOT CHANGE ANYTHING BELOW THIS LINE
void *malloc(size_t size) noexcept;
void *calloc(size_t num_memb, size_t size_each) noexcept;
void free(void *ptr) noexcept;
void* operator new(size_t size);
void operator delete(void *address ) noexcept;
void* operator new[] ( size_t size );
void operator delete[] ( void* address ) noexcept;

#endif /* SNP_MEMORY_H_ */