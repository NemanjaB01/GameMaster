#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

// Implement the function call in assembly in the TODO section below.
// Do not change the functionality or the signature of the function to be called.
// Do not modify this file outside the TODO section.

// Function to be called:
uint32_t countInRange(const double *number_array, uint32_t array_len, double lower, double upper, uint32_t *first_match);

int main(int argc, char **argv)
{
    setbuf(stdout, NULL); // Disable buffering

    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <number1,number2,..> <lower> <upper>\n", argv[0]);
        return 1;
    }

    char *numbers_string = argv[1];

    uint32_t array_len = 0;
    for (size_t i = 0; i < strlen(numbers_string); i++)
    {
        if (numbers_string[i] == ',')
        {
            array_len++;
        }
    }
    array_len++;

    if (array_len == 0)
    {
        fprintf(stderr, "Error: length of numbers == 0\n");
        return 1;
    }

    double numbers[array_len];
    char *number_str;
    for (uint32_t i = 0; (number_str = strtok(numbers_string, ",")); i++, numbers_string = NULL)
    {
        numbers[i] = atof(number_str);
    }

    double lower = atof(argv[2]);
    double upper = atof(argv[3]);

    uint32_t nr_matches = (uint32_t)-1;
    uint32_t first_match = -1;

    printf("Calling function with parameters: number_array=%p, array_len=%u, lower=%f, upper=%f, first_match=%p\n",
           numbers, array_len, lower, upper, &first_match);

    // Use (only!) GCC inline assembly to call `countInRange`. Always consider the expected data types.
    // Parameters: numbers, array_len, lower, upper, &first_match
    // Store the return value in nr_matches
    // You may want to use movq to set up the registers for floating-point arguments, 
    // as there are no proper constraints for double parameters!

    // ########## TODO START ##########
    // System V AMD64 ABI implementation (64-bit)

    asm("mov %1,%%rdi\n"
        "mov %2,%%esi\n"
        "movq %3,%%xmm0\n"
        "movq %4,%%xmm1\n"
        "mov %5,%%rdx\n"
        "call countInRange\n"
        "mov %%eax, %0\n"
        : "=r"(nr_matches)
        : "r"(numbers),"r"(array_len),"r"(lower),"r"(upper),"r"(&first_match)
        : "rax","rdi","xmm0","xmm1","rdx","rsi","cc","memory");

    // ########## TODO END ##########

    printf("Function returned: %d in range.\n", nr_matches);
    printf("First match index is %u.\n", first_match);

    return 0;
}
