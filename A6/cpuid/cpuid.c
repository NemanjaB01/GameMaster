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

    unsigned int linear_address, fpu, apic, acpi, sse, aes, avx512, smap, vaes ,value;
    asm("movl $0x80000008, %%eax\n"
        "cpuid" : "=eax"(linear_address)::"cc");
    
    linear_address = linear_address >> 8;
    linear_address = linear_address & 255;

    printf("Number of Linear Address Bits: %u\n", linear_address);

    asm("movl $0x01, %%eax\n"
        "cpuid": "=edx"(value),"=ecx"(aes)::"cc","eax","edx","ecx");
    fpu = value & 1;
    acpi = value >> 22;
    acpi = acpi & 1;
    sse = value >> 25;
    sse = sse & 1;
    aes = aes >> 25;
    aes = aes & 1;

    asm("movl $0x01, %%eax\n"
        "cpuid": "=eax"(value):: "cc");
    apic = value >> 9;
    apic = apic & 1;

    asm("movl $0x07, %%eax\n"
        "movl $0x00, %%ecx\n"
        "cpuid": "=ebx"(smap),"=ecx"(value)::"cc", "ebx","ecx","eax");
    smap = value >> 20;
    smap = smap & 1;
    vaes = value >> 9;
    vaes = vaes & 1;
    avx512 = value >> 6;
    avx512 = avx512& 1;
    

    printf("fpu: %u, apic: %u, acpi: %u, sse: %u, aes: %u, avx512: %u, smap: %u, vaes: %u\n",
            fpu,apic,acpi,sse,aes,avx512,smap,vaes);
}
