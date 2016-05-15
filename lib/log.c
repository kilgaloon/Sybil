#include "stdio.h"
#include "stdlib.h"

#include "../config.h"
#include "log.h"

void error_log(char *error_message) {
    FILE *fp = fopen(ERROR_LOG,"a");
    if(fp != NULL) {
        fputs(error_message, fp);  
        fputs("\n\n", fp);
        fclose(fp); 
    }   
}
