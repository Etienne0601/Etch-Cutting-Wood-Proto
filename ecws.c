#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef unsigned int uint_t;


typedef struct {
    int num_stock_types;
    int num_shelf_types;
    int* stock_lengths;
    int* stock_costs;
    int* shelf_lengths;
    int* shelf_quantities;
} problemspec_t;


/**
 * process input file to get the data in the format we want
 *
 * k will be an int type
 * m with be an int type
 * the stock lengths will be int array
 *     (will be later extended to fractional lengths)
 * the costs will be will be uint_t array (in cents)
 * the shelf lengths will be int array
 *     (will be later extended to fractional lengths)
 * the demanded quantity of shelves will be int array
 *
 * stores the problemspec instance in prob_instance
 *
 * Returns 0 if successful, or -1 if failed
 */
int parse_input(FILE * in_file, problemspec_t * prob_instance) {

    char* line = NULL;  // to hold string output from getline
    size_t len = 0;
    ssize_t nread;

    // k and m are number of types of stock and shelves, respectively
    int k, m, i, temp;

    // these are used for reading the CSV formatted arrays in the input file
    char* endptr = NULL;
    char* numptr = NULL;

    // ========================================================================
    // read the first line from the input file
    nread = getline(&line, &len, in_file);
    if (nread == -1) {
        // getline failed to read this line
        fprintf(stderr, "getline failed to read the first line\n");
        // we should still free the buffer allocated by getline
        free(line);
        return -1;
    }

    // debugging output
    printf("Retrieved the first line of length %zd\n", nread);

    // get integer k, the num_stock_types
    // maybe should switch from atoi to strtol later
    k = atoi(line);
    if (k == 0) {
        // atoi failed to convert the string to an integer
        // free the buffer allocated by getline
        free(line);
        return -1;
    }

    // debugging output
    printf("k = %d\n", k);

    // finally store the number of stock types in prob_instance
    prob_instance->num_stock_types = k;

    // ========================================================================
    // read the second line from the input file
    nread = getline(&line, &len, in_file);
    if (nread == -1) {
        // getline failed to read this line
        fprintf(stderr, "getline failed to read the second line\n");
        // we should still free the buffer allocated by getline
        free(line);
        return -1;
    }

    // debugging output
    printf("Retrieved the second line of length %zd\n", nread);

    // get integer m, the num_shelf_types
    // maybe should switch from atoi to strtol later
    m = atoi(line);
    if (k == 0) {
        // atoi failed to convert the string to an integer
        // free the buffer allocated by getline
        free(line);
        return -1;
    }

    // debugging output
    printf("m = %d\n", m);

    // finally store the number of shelf types in prob_instance
    prob_instance->num_shelf_types = m;

    // ========================================================================

    // allocate memory for the int arrays
    prob_instance->stock_lengths    = (int*) malloc(k * sizeof(int));
    prob_instance->stock_costs      = (int*) malloc(k * sizeof(int));
    prob_instance->shelf_lengths    = (int*) malloc(m * sizeof(int));
    prob_instance->shelf_quantities = (int*) malloc(m * sizeof(int));

    // ========================================================================
    // read the third line from the input file
    // the third line is the stock lengths in CSV form
    nread = getline(&line, &len, in_file);
    if (nread == -1) {
        // getline failed to read this line
        fprintf(stderr, "getline failed to read the third line\n");
        // we should still free the buffer allocated by getline
        free(line);
        return -1;
    }

    // debugging output
    printf("Retrieved the third line of length %zd\n", nread);

    // we need to capture all k stock lengths
    numptr = line;
    for (i = 0; i < k; ++i) {
        temp = (int) strtol(numptr, &endptr, 10);

        if (temp == 0) {
            // strtol failed to read the number
            fprintf(stderr, "strtol failed to read a number\n");
            // we should free the buffer allocated by getline
            free(line);
            return -1;
        }

        // store the stock length in the array of the prob_instance
        prob_instance->stock_lengths[i] = temp;

        // move the numptr over to the next number
        numptr = endptr + 1;

        // debugging output
        printf("read the stock length num %d\n", temp);
    }

    // ========================================================================
    // read the fourth line from the input file
    // the fourth line is the stock costs in CSV form
    nread = getline(&line, &len, in_file);
    if (nread == -1) {
        // getline failed to read this line
        fprintf(stderr, "getline failed to read the fourth line\n");
        // we should still free the buffer allocated by getline
        free(line);
        return -1;
    }

    // debugging output
    printf("Retrieved the fourth line of length %zd\n", nread);

    // we need to capture all k stock costs
    numptr = line;
    for (i = 0; i < k; ++i) {
        temp = (int) strtol(numptr, &endptr, 10);

        if (temp == 0) {
            // strtol failed to read the number
            fprintf(stderr, "strtol failed to read a number\n");
            // we should free the buffer allocated by getline
            free(line);
            return -1;
        }

        // store the stock cost in the array of the prob_instance
        prob_instance->stock_costs[i] = temp;

        // move the numptr over to the next number
        numptr = endptr + 1;

        // debugging output
        printf("read the stock cost num %d\n", temp);
    }

    // ========================================================================
    // read the fifth line from the input file
    // the fifth line is the shelf lengths in CSV form
    nread = getline(&line, &len, in_file);
    if (nread == -1) {
        // getline failed to read this line
        fprintf(stderr, "getline failed to read the fifth line\n");
        // we should still free the buffer allocated by getline
        free(line);
        return -1;
    }

    // debugging output
    printf("Retrieved the fifth line of length %zd\n", nread);

    // we need to capture all m shelf lengths
    numptr = line;
    for (i = 0; i < m; ++i) {
        temp = (int) strtol(numptr, &endptr, 10);

        if (temp == 0) {
            // strtol failed to read the number
            fprintf(stderr, "strtol failed to read a number\n");
            // we should free the buffer allocated by getline
            free(line);
            return -1;
        }

        // store the shelf lengths in the array of the prob_instance
        prob_instance->shelf_lengths[i] = temp;

        // move the numptr over to the next number
        numptr = endptr + 1;

        // debugging output
        printf("read the shelf length num %d\n", temp);
    }

    // ========================================================================
    // read the sixth line from the input file
    // the sixth line is the shelf quantities in CSV form
    nread = getline(&line, &len, in_file);
    if (nread == -1) {
        // getline failed to read this line
        fprintf(stderr, "getline failed to read the sixth line\n");
        // we should still free the buffer allocated by getline
        free(line);
        return -1;
    }

    // debugging output
    printf("Retrieved the sixth line of length %zd\n", nread);

    // we need to capture all m shelf quantities
    numptr = line;
    for (i = 0; i < m; ++i) {
        temp = (int) strtol(numptr, &endptr, 10);

        if (temp == 0) {
            // strtol failed to read the number
            fprintf(stderr, "strtol failed to read a number\n");
            // we should free the buffer allocated by getline
            free(line);
            return -1;
        }

        // store the shelf quantities in the array of the prob_instance
        prob_instance->shelf_quantities[i] = temp;

        // move the numptr over to the next number
        numptr = endptr + 1;

        // debugging output
        printf("read the shelf quantities num %d\n", temp);
    }

    // ========================================================================


    // we parsed the input successfully
    return 0;
}


int main(int argc, char* argv[]) {

    FILE* fp;
    int temp;

    problemspec_t prob_instance;

    if (argc != 2) {
        // invalid number of command line args
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // open the input file for reading, the file is specified by the first
    // command line argument
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        // couldn't open the input file
        fprintf(stderr, "Error opening input file \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    parse_input(fp, &prob_instance);

    temp = fclose(fp);
    if (temp != 0) {
        // there was an error in closing the file
        fprintf(stderr, "Error closing input file \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    return 0;
}
