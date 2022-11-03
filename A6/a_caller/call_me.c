#include <stdio.h>
#include <stdint.h>

// There is nothing to do in this file.
// Changes in this file will not be considered in the test system.

// This function has to be called.
/**
 * @brief returns the count of numbers greater than or equal lower and smaller than or equal to upper
 *
 * @param number_array the array to search
 * @param array_len the length of the array to search
 * @param lower the lower bound
 * @param upper the upper bound
 * @param first_match index of first match
 * @return uint32_t, count of numbers in range
 */
uint32_t countInRange(const double *number_array, uint32_t array_len, double lower, double upper, uint32_t *first_match)
{
    printf("Function called with parameters: number_array=%p, array_len=%u, lower=%f, upper=%f, first_match=%p\n",
           number_array, array_len, lower, upper, first_match);

    *first_match = array_len;

    uint32_t cnt = 0;
    for (uint32_t i = 0; i < array_len; ++i)
    {
        if (lower <= number_array[i] && number_array[i] <= upper)
        {
            if (cnt == 0)
            {
                *first_match = i;
            }
            cnt++;
        }
    }

    return cnt;
}