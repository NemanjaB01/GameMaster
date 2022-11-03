#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <time.h>
#include <unistd.h> // remove this!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Implement the given function (see the wiki) in the sysv_abi.S file

// Feel free to change this file however you want, it will not affect
// your result on the test system (e.g. you can add more test here)
ssize_t relativeTimeSince(ssize_t from, int64_t *years, int64_t *days);

ssize_t relativeTimeSince_ref(ssize_t from, int64_t *years, int64_t *days)
{
    ssize_t t = time(NULL) - from;
    *years = t / (365 * 24 * 60 * 60);
    *days = t / (24 * 60 * 60) - *years * 365;
    return t;
}

int main()
{
    printf("Test start\n");

    {
        // Just simply the years and days from epoch time (1970-01-01)
        int64_t years, days;
        ssize_t from = 0;
        ssize_t rv = relativeTimeSince(from, &years, &days);

        assert(rv >= 0);
        printf("%ld seconds elapsed since the initial commit of sweb, amounting to %ld years and %ld days\n", rv, years, days);
    }

    {
        // The years and days since the initial commit of sweb
        int64_t years, days;
        ssize_t from = 1111508654; // the initial commit time of sweb
        ssize_t rv = relativeTimeSince(from, &years, &days);

        assert(rv >= 0);
        printf("%ld seconds elapsed since the initial commit of sweb, amounting to %ld years and %ld days\n", rv, years, days);
    }

    // Add more tests here to test your implementation...

    printf("Test exit\n");
    return 0;
}
