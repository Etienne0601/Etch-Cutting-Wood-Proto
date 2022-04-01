#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


/**
 * A struct to store the problem instance as defined by the user
 */
typedef struct {
    int num_stock_types;
    int num_shelf_types;
    int* stock_lengths;    // in increasing order
    int* stock_costs;      // position corresponding to type in stock_lengths
    int* shelf_lengths;    // in increasing order
    int* shelf_quantities; // position corresponding to type in shelf_lengths
} problemspec_t;

/**
 * A struct for a node in the linked list below
 * This is the unit that maps the key (cut pattern) to the value (stock type)
 */
typedef struct {
    // the length of cut_pattern is num_shelf_types
    int* cut_pattern;
    int stock_type; // a value between 0 and num_stock_type - 1 (inclusive)
    node_t* next;
} node_t;


/**
 * A struct for a linked list used in the hash map
 */
typedef struct {
    int size;
    node_t* front;
} LL_t;


/**
* A struct to store all the maximal cutting patterns for a given problem
 * instance, as well as information relevant to build the MIP model.
 *
 * A cutting pattern is a vector of length m, where m is num_shelf_types.
 * Each cutting pattern has an associated cost that it determined from the type
 * of stock it is cut from.
 *
 */
typedef struct {

    // I need a hash map that maps cutting patterns to stock types
    // For simplicity right now I'm going to have a fixed capacity of 100 buckets

    int num_stock_types;
    int num_shelf_types;
    int size;
    int capacity;
    LL_t* buckets;

} hashmap_t;


/**
 * computes the hash of the array of numbers
 */
int hash_vect(int * nums, int n) {

    int hash = 17;
    int i;

    for (i = 0; i < n; ++i) {
        hash = hash * 19 + nums[i];
    }

    return hash;
}


/**
 * Initializes the hashmap
 *
 * Returns 0 if successful, or -1 if it failed
 */
int init_hashmap(problemspec_t * prob_instance, hashmap_t * hashmap) {

    int i;

    // initialize the buckets array with a capacity of 100
    hashmap->buckets = (LL_t*) malloc(100 * sizeof(LL_t));
    hashmap->capacity = 100;
    hashmap->size = 0;
    hashmap->num_stock_types = prob_instance->num_stock_types;
    hashmap->num_shelf_types = prob_instance->num_shelf_types;

    if (hashmap->buckets == NULL) {
        fprintf(stderr, "malloc failed in init_hashmap\n");
        return -1;
    }

    for (i = 0; i < 100; ++i) {
        hashmap->buckets[i].size = 0;
        hashmap->buckets[i].front = NULL;
    }

    return 0;
}


/**
 * Inserts a cutting pattern and stock type into the hashmap.
 * If the cutting pattern already exists in the hashmap, then replace the stock
 * type with the new stock type if the new one is cheaper.
 *
 * Returns 0 if successful, or -1 if it failed
 */
int insert_hashmap(hashmap_t * hashmap, int * cut_pattern, int stock_type) {

    int hash, i;
    node_t* new_node;

    hash = hash_vect(cut_pattern, hashmap->num_shelf_types);
    hash %= 100;

    new_node = (node_t*) malloc(sizeof(node_t));

    if (hashmap->buckets[hash].size == 0) {
        // there's no elements in this bucket so cut_pattern doesn't yet exist
        // in the hashmap


        if (new_node == NULL) {
            fprintf(stderr, "malloc failed in insert_hashmap\n");
            return -1;
        }

        new_node->cut_pattern = cut_pattern;
        new_node->stock_type = stock_type;
        new_node->next = NULL;
        hashmap->buckets[hash].front = new_node;
        ++hashmap->buckets[hash].size;
        ++hashmap->size;
    }

    return 0;
}


/**
 * takes the dot product of two vectors of length n
 *
 * Returns 0 if successful, or -1 if it failed
 */
int dot_product(int * a, int * b, int n) {
    int i;
    int result = 0;

    if (n < 1) {
        fprintf(stderr, "Dot Product Error!\n");
        return -1;
    }

    for (i = 0; i < n; ++i) {
        result += a[i] * b[i];
    }

    return result;
}


/**
 * This function generates all the maximal cutting patterns available with the
 * given prob_instance and stores them in cut_patterns. Redundant maximal
 * cutting patterns that arise due to different stock sizes are ignored, and
 * the cheaper stock size for a given cutting pattern is preferred.
 *
 * How should I deal with these redundant maximal cutting patterns?
 * How about a map whose keys are the maximal cutting pattern ID and the values
 * are the corresponding cheapest stock type for that cutting pattern found so
 * far. Therefore, if the same maximal cutting pattern comes up again with a
 * different stock type, the value will be replaced with the new stock type if
 * it is cheaper.
 *
 * Returns 0 if successful, or -1 if it failed
 */
int cutting_patterns_gen(problemspec_t * prob_instance, hashmap_t * cut_patterns) {

    // iteration vars
    int i, j;

    /** loop over the different stock types to generate the maximal cutting
     *  patterns for each
     */
    for (i = 0; i < prob_instance->num_stock_types; ++i) {

    }

    return 0;
}

/**
 * process input file to get the data in the format we want
 *
 * k will be an int type
 * m with be an int type
 * the stock lengths will be int array in increasing order
 *     (will be later extended to fractional lengths)
 * the costs will be will be int array (in cents) in order corresponding to the
 *     indices in the stock lengths array
 * the shelf lengths will be int array in increasing order
 *     (will be later extended to fractional lengths)
 * the demanded quantity of shelves will be int array in order corresponding to
 *     the indices in the shelf lengths array
 *
 * stores the problemspec instance in prob_instance
 *
 * the stock_lengths, stock_costs, shelf_lengths, and shelf_quantities members
 * are dynamically allocated in this function and it's the responsiblity of the
 * caller to free
 *
 * Returns 0 if successful, or -1 if it failed
 */
int parse_input(problemspec_t * prob_instance, FILE * in_file) {

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
    printf("Retrieved the first line of length %d\n", (int)nread);

    // get integer k, the num_stock_types
    // maybe should switch from atoi to strtol later
    k = atoi(line);
    if (k == 0) {
        // atoi failed to convert the string to an integer
        fprintf(stderr, "atoi failed to convert string to integer\n");
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
    printf("Retrieved the second line of length %d\n", (int)nread);

    // get integer m, the num_shelf_types
    // maybe should switch from atoi to strtol later
    m = atoi(line);
    if (m == 0) {
        // atoi failed to convert the string to an integer
        fprintf(stderr, "atoi failed to convert string to integer\n");
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

    if (prob_instance->stock_lengths       == NULL
        || prob_instance->stock_costs      == NULL
        || prob_instance->shelf_lengths    == NULL
        || prob_instance->shelf_quantities == NULL) {

        fprintf(stderr, "malloc failed in parse_input\n");
        return -1;
    }

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
    printf("Retrieved the third line of length %d\n", (int)nread);

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
    printf("Retrieved the fourth line of length %d\n", (int)nread);

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
    printf("Retrieved the fifth line of length %d\n", (int)nread);

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
    printf("Retrieved the sixth line of length %ld\n", nread);

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


    // free the buffer allocated by getline
    free(line);

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

    parse_input(&prob_instance, fp);

    temp = fclose(fp);
    if (temp != 0) {
        // there was an error in closing the file
        fprintf(stderr, "Error closing input file \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    return 0;
}
