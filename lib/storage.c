#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../config.h"
#include "storage.h"
#include "log.h"

/**
 * Initialize new in-memory storage and assign configurations to it
 * @param STORAGE *storage
 * @param CONFIG *config
 */
void storage_init(STORAGE *storage, CONFIG *config) {
    // in next steps we will set storage settings
    storage->size = 0;
    storage->allowed_memory_key_size = config->allowed_memory_key_size; // get this from ini
    storage->allowed_memory_value_size = config->allowed_memory_value_size; // get this from ini
    storage->memory_size = 0;
    storage->allowed_memory_size = config->allowed_memory_size * 1024; // get this from ini
    
    // allocating memory for storage data
    storage->data = calloc(storage->allowed_memory_size, sizeof(char ***));
    //check did we succeeded to allocate memory for storage data
    if(storage->data == 0) {
        error_log("Failed to allocate memory for storage data.");
    }
    
}

/**
 * Setting key in storage
 * @param STORAGE *storage
 * @param char *key
 * @param char *value
 */
void set_key(STORAGE *storage, char *key, char *value) { 
    // check is there enough memory to be stored
    // and should we proceed with insertion of new key
    int proceed = check_for_storage_size(storage); 
    // if there is available memory for key, proceed
    // else give warning that there is no space left in memory allowed
    // for sybil
    if(proceed) {
        // allocated enough memory for next key that will be stored
        int fai = find_available_index(storage);
        int safe_continue = safe_member_initialize(storage, fai);
        int key_exists = get_key(storage, key, 0);
        // at this point we check is it safe to continue to insert data
        if(safe_continue && !key_exists) {
            strncpy(storage->data[storage->size][0], key, (strlen(key) + 1));
            strncpy(storage->data[storage->size][1], value, (strlen(value) + 1));

            storage->size++;

            int used_memory = ((strlen(key) + strlen(value)) * sizeof(char));
            memory_size_used(storage, used_memory);
        }

    }
}

/**
 * Function tries to find provided key 
 * @param STORAGE *storage 
 * @param char *key
 * @param int *prnt
 * @return int
 */
int get_key(STORAGE *storage, char *key, int prnt) {
    int i;
    //if(storage->size != 0) {
    for(i = 0; i <= storage->size; i++) {
        // we are checking is it null, because we use this function also
        // to search for available lost index
        if(storage->data[i][0] != NULL) {
            if(strcmp(storage->data[i][0], key) == 0) {
                if(prnt) { printf("%s", storage->data[i][1]); }
                
                return 1;
            } else {         
                if(prnt) { printf("Key does not exist!"); }

                return 0;
            }
        }
    }
    
    return 0;
}

int find_available_index(STORAGE *storage) {
    int i;
    //if(storage->size != 0) {
    for(i = 0; i <= storage->size; i++) {
        // we are checking is it null, because we use this function also
        // to search for available lost index
        if(i == storage->size) {
            return i;
        } else {
            if(storage->data[i][0] == NULL) {
                return i;
            }
        }
    }
    
    return 0;
}

// get key from storage linear search
int update_key(STORAGE *storage, char *key, char *value) {
    int proceed = check_for_storage_size(storage); 
    if(proceed) {
        int i;
        for(i = 0; i < storage->size; i++) {
            if(strcmp(storage->data[i][0], key) == 0) {

                int prev_val_mem_usage = strlen(storage->data[i][1]);

                strncpy(storage->data[i][1], value, (strlen(value) + 1));

                int used_memory = (strlen(value) - prev_val_mem_usage);
                memory_size_used(storage, used_memory);
                return 1;
            } else {
                printf("Key does not exist!");
                return 0;
            }
        }
    } else {
        error_log("No memory left to update key.");
        
    }
    
    return 0;
}

// get key from storage linear search
void delete_key(STORAGE *storage, char *key) {
    int i;
    for(i = 0; i < storage->size; i++) {
        if(strcmp(storage->data[i][0], key) == 0) {            
            char *key_ptr = storage->data[i][0];
            char *val_ptr = storage->data[i][1];
            
            int used_memory = (-(strlen(storage->data[i][0]) + strlen(storage->data[i][1])) * sizeof(char));
            memory_size_used(storage, used_memory);
            
            storage->data[i][0] = NULL;
            storage->data[i][1] = NULL;
            
            // storage size decrements
            storage->size--;
            // free memory on given keys            
            free(key_ptr);
            free(val_ptr);
            
            break;
        }
    }
}

// checking for storage size, is more storage available for storing data or no
int check_for_storage_size(STORAGE *storage) {
    if((storage->allowed_memory_size - storage->memory_size) >= 
       (storage->allowed_memory_key_size + storage->allowed_memory_value_size)) {
        return 1;
    } else {
        // log error "No more storage available"
        error_log("No more available memory size, check your configuration file.");
        return 0;
    }
}


// function calculate how much next key will take space in memory
// and increase memory_size that is used
void memory_size_used(STORAGE *storage, int memory) {
    storage->memory_size += memory;
}

// this functions is going step by step and tries to give memory for 
// certain locations, if memory isn't allocated will return failed flag
int safe_member_initialize(STORAGE *storage, int member) {       
    
    // allocating memory for storage data member
    storage->data[member] = calloc(storage->allowed_memory_key_size + storage->allowed_memory_value_size, sizeof(char));
    // checking is memory allocated for storage data member
    if(storage->data[member] != NULL) {
        // checking is memory allocated for storage data member
        storage->data[member][0] = calloc(storage->allowed_memory_key_size, sizeof(char));
        // if is null, memory allocation failed
        if(storage->data[member][0] != NULL) {
            // checking is memory allocated for storage data member
            storage->data[member][1] = calloc(storage->allowed_memory_value_size, sizeof(char));
            // if is null, memory allocation failed
            if(storage->data[member][1] != NULL) {
                // data allocation for next member is available
                // return flag that next member can be safely set
                return 1;
            } else {
               // this is place for function to write errors into log file
                error_log("Failed to allocate memory for storage data member value.");
                return 0; 
            }      

        } else {
            // this is place for function to write errors into log file
            error_log("Failed to allocate memory for storage data member key.");
            return 0;
        }


    // failure is here
    // not available to get memory for storage data
    } else {
        // this is place for function to write errors into log file
        error_log("Failed to allocate memory for storage data member.");
        return 0;
    }
    
}

