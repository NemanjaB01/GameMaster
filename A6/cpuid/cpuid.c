#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main()
{
    /* TODO: Find the processor's manufacturer ID string,
     *       the number of linear address bits, and the
     *       availability bits of the given features from the wiki
     *       using the cpuid instruction and inline assembly.
     *       Do not use builtin functions.
     *       Print the values to stdout in the given formats.
     *       Feel free to modify this file wherever you want.
     */
     char edx[8];
     char ebx[8];
     char ecx[8];
     
     asm("cpuid" : "=d"(edx), "=b"(ebx),"=c"(ecx) : "a"(0));
     char id[13];
     strcpy(id,ebx);
     strcat(id,edx);
     strcat(id, ecx);
     id[12] = '\0';

    printf("Manufacturer id: %s\n", id);

    unsigned int linear_address;
    asm("cpuid" : "=a"(linear_address) :"a"(80000008));
    
    linear_address = linear_address >> 8;
    linear_address = linear_address & 255;

    printf("Number of Linear Address Bits: %u\n", linear_address);

    /* printf("fpu: %u, apic: %u, acpi: %u, sse: %u, aes: %u, avx512: %u, smap: %u, vaes: %u\n",
               ..., ..., ..., ..., ..., ..., ..., ...); */
}
