#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "config.h"


// clear memory locations for buffer and assign value
void clearMemoryOnAssigment(char *buffer, char *value) {
    memset(buffer, 0, strlen(buffer) * (sizeof(buffer[0]))); 
    memset(value, 0, strlen(value) * (sizeof(value[0])));
}