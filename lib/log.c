#include "stdio.h"

#include "log.h"


void error_log(char *error_message) {
    FILE *fp = fopen("/tmp/sybil/error.log","ab");
    if(fp != NULL) {
        fputs(error_message, fp);  
        fputs("\n\n", fp);
        fclose(fp); 
    }
   
}
