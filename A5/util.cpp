#include "malloc.cpp"

// DO NOT MODIFY THE EXISTING FUNCTIONS IN THIS FILE
// Your changed version will not be used by the testsystem.

void* snp::sbrk(intptr_t __delta)
{
  return ::sbrk(__delta);
}

int snp::brk(void* __addr)
{
  return ::brk(__addr);
}

void *calloc(size_t num_memb, size_t size_each) noexcept
{
  return snp::Memory::calloc(num_memb, size_each);
}

void *malloc(size_t size) noexcept
{
  return snp::Memory::malloc(size);
}

void free(void *ptr) noexcept
{
  snp::Memory::free(ptr);
}

extern "C" size_t malloc_called_count()
{
  return snp::Memory::malloc_called_count();
}

extern "C" size_t used_blocks_count()
{
  return snp::Memory::used_blocks_count();
}

#ifdef NEW_ENABLED

void* operator new(size_t size)
{
  return snp::Memory::_new(size);
}

void operator delete(void *address )
{
  snp::Memory::_delete(address);
}

void operator delete(void* address, std::size_t size)
{
  (void) size;
  snp::Memory::_delete(address);
}

void* operator new[] ( size_t size )
{
  return snp::Memory::_new(size);
}

void operator delete[] ( void* address )
{
  snp::Memory::_delete(address);
}

void operator delete [](void* address, std::size_t size)
{
  (void) size;
  snp::Memory::_delete(address);
}

#endif
