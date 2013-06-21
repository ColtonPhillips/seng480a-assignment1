// Tom Gibson 
// Colton Phillips
/* Util.h */

#ifndef UTILH
#define UTILH

#define die(...) do { printf(__VA_ARGS__); fflush(stdout); exit(1); } while (0)

#define got_here printf("got here %d\n", __LINE__);

#define MAX_NUMBER_OF_SLOTS 20
#define MAX_BUFFER_SIZE 200

#define TRUE 1
#define FALSE 0 

#define sizeof_array(x,type) sizeof(x)/sizeof(type)
#define sizeof_int_array(x) sizeof_array(x,int)

#endif
