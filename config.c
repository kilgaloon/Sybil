#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"

#include "config.h"


// clear memory locations for buffer and assign value
void clearMemoryOnAssigment(char *buffer, char *value) {
    memset(buffer, 0, (strlen(buffer) + 1) * (sizeof(buffer[0]))); 
    memset(value, 0, (strlen(value) + 1) * (sizeof(value[0])));
}

void setEnvironment(char *var, char *value) {
    char *env = malloc((strlen("SYBIL_") + strlen(var)) * sizeof(char));
    char *s = strdup(var);
   
    int i;
    for (i = 0;s[i] != '\0'; i++){
      s[i] = toupper(s[i]);
    }
    
    strcat(env,"SYBIL_");
    strcat(env,s);
    
    setenv(env,value,1);
    
    free(env);
    free(s);
}