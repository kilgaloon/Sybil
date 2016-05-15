#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "unistd.h"
#include "assert.h"
#include "ctype.h"

// sybil libraries
#include "parser.h"
#include "../config.h"

// load and set rules for parser
RULES loadRules_ini() {
	RULES parser_rule;

	parser_rule.comment = '#';
	parser_rule.end_of_line = '\n';
	parser_rule.equals = '=';
	parser_rule.end_of_assigment = ';';

	return parser_rule;
}

CONFIG *parseINI(char *inipath) {
    // define configuration struct
    // to store configuration data
    CONFIG *config;
    config = malloc(sizeof(CONFIG));

    char *ap[7];
    ap[0] = "";
    ap[1] = "allowed_memory_value_size";
    ap[2] = "allowed_memory_size";
    ap[3] = "allowed_cache_size";
    ap[4] = "max_storage_instances";
    ap[5] = "allowed_memory_key_size";
    ap[6] = "error_log";
    
    // SEGMENTATION FAULT IS HAPPENING WHEN INI FILE HAVE VALUES THAT ARENT PROVIDED IN AP ARRAY

    // array that holds memory address of configuration variables
    int *as[7];  
    as[0] = NULL;
    as[1] = &config->allowed_memory_value_size;
    as[2] = &config->allowed_memory_size;
    as[3] = &config->allowed_cache_size;
    as[4] = &config->max_storage_instances;
    as[5] = &config->allowed_memory_key_size;
    as[6] = NULL;

    int len_of_ap = sizeof(ap)/sizeof(ap[0]);
    int len_of_as = sizeof(as)/sizeof(as[0]);

    // assumption and must, ap need to be equal to as
    // they correspond to each other, so they need to be 
    // same length, and also values from as need to be addresses
    // of corresponding values in ap. Otherwise program will end
    // gracefully
    assert(len_of_ap == len_of_as);

    // load parser rules
    RULES rules = loadRules_ini();

    FILE *fp;

    if( access( inipath, F_OK ) == -1 ) {
     // file doesn't exist
        printf("No file at given location. Terminating!");
        exit(EXIT_FAILURE);
    }

    // file exists
    fp = fopen(inipath, "r");

    // seek to the end of file
    fseek(fp, 0, SEEK_END);
    int sz = ftell(fp); // get file size
    fseek(fp, 0, SEEK_SET); // return file pointer to start

    char *contents = calloc(sz, sizeof(char));
    
    if(contents == NULL) {
        printf("Could not allocate memory for reading file!");
        exit(EXIT_FAILURE);
    }


    if(fp) {
        char *cont = calloc(sz, sizeof(char));              
        char *assign_value = calloc(255, sizeof(char));

        while((*contents = getc(fp)) != EOF) {
            // set static var to check did comment started
            static int comment_started = 0;
            //static int chars_in_comments = 0;
            // parser rules

            // while going through ini file
            // we find occurence of opening comment sign '#'
            // we set comment_started to 1 if its not already set to 1
            if(*contents == rules.comment && comment_started == 0) {
                comment_started = 1;
            }

//            if(comment_started == 1) {
//                ++chars_in_comments;
//            }

            // if we find occurence of closing comment
            // '\n' new line, since new line means that comment finished
            // we set comment_started to 0 and proceed further
            if(*contents == rules.end_of_line) {
                comment_started = 0;
            }

            //here we know, is statement comment or is assignment to configuration
            if(!comment_started && !isspace(*contents) && *contents != rules.end_of_line) {
                static int assigment = 0;

                if(strcmp(contents,";") == 0) {
                    assigment = 0;
                }

                if(assigment) {
                    strcat(assign_value, contents);
                }

                if(strcmp(contents,"=") == 0) {
                    assigment = 1;
                }          

                if(assigment == 0) {                                                         
                    
                    strcat(cont, contents);

                    if(strlen(assign_value) > 0) {
                        char *value = strdup(assign_value);
                        int i = 0;
                        for(; i <= len_of_ap; i++) {
                            if(strncmp(cont,ap[i],(strlen(cont) - 1)) == 0) {
                                int *ptr = as[i];

                                int convert = atoi(value);
                                if(!convert) {
                                    setEnvironment(ap[i],assign_value);
                                } else {
                                    *ptr = convert;
                                }

                                clearMemoryOnAssigment(cont, assign_value);
                                i = len_of_ap;

                            }
                            
                        }
                        // free strdup
                        free(value);
                    }
                }              
            }
        }
        
        fclose(fp); 
        
        // free memory
        cont = NULL;
        assign_value = NULL;
        
        free(cont);
        free(assign_value);
    }

    // free allocated memory
    free(contents);

    return config;
}





