#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../config.h"
#include "storage.h"



void storage_init(STORAGE *storage, CONFIG *config) {
    // in next steps we will set storage settings
    storage->size = 0;
    storage->allowed_memory_key_size = config->allowed_memory_key_size; // get this from ini
    storage->allowed_memory_value_size = config->allowed_memory_value_size; // get this from ini
    storage->memory_size = 0;
    storage->allowed_memory_size = config->allowed_memory_size * 1024; // get this from ini
    
    safe_member_initialize(storage, 0);
}

void set_key(STORAGE *storage, char *name, char *value) { 
    // check is there enough memory to be stored
    // and should we proceed with insertion of new key
    int proceed = check_for_storage_size(storage); 
    // if there is available memory for key, proceed
    // else give warning that there is no space left in memory allowed
    // for sybil
    if(proceed) {
        // allocated enough memory for next key that will be stored
        int safe_continue = safe_member_initialize(storage, storage->size);
        // at this point we check is it safe to continue to insert data
        if(safe_continue) {
            strncpy(storage->data[storage->size][0], value, strlen(value + 1));
            strncpy(storage->data[storage->size][1], name, strlen(name + 1));

            storage->size++;

            int used_memory = ((strlen(name) + strlen(value)) * sizeof(char));
            memory_size_used(storage, used_memory);
        } else {
            // place for logging errors
        }
        
    } else {
        // place for logging errors
    }
}

int check_for_storage_size(STORAGE *storage) {
    if((storage->allowed_memory_size - storage->memory_size) >= 
       (storage->allowed_memory_key_size + storage->allowed_memory_value_size)) {
        return 1;
    } else {
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
    // allocating memory for storage data
    storage->data = calloc(storage->allowed_memory_size, sizeof(char));
    // check did we succeeded to allocate memory for storage data
    if(storage->data != NULL) {
        // allocating memory for storage data member
        storage->data[member] = calloc(storage->allowed_memory_key_size + storage->allowed_memory_value_size, sizeof(char));
        // checking is memory allocated for storage data member
        if(storage->data[member] != NULL) {
            // checking is memory allocated for storage data member
            storage->data[member][0] = calloc(storage->allowed_memory_key_size, sizeof(char));
            // if is null, memory allocation failed
            if(storage->data[member][0] == NULL)
                // this is place for function to write errors into log file
                return 0;
            
            // checking is memory allocated for storage data member
            storage->data[member][1] = calloc(storage->allowed_memory_value_size, sizeof(char));
            // if is null, memory allocation failed
            if(storage->data[member][1] == NULL)
                // this is place for function to write errors into log file
                return 0;
            
            // data allocation for next member is available
            // return flag that next member can be safely set
            return 1;
           
        // failure is here
        // not available to get memory for storage data
        } else {
            // this is place for function to write errors into log file
            return 0;
        }
        
    // failure is here
    // not available to get memory for storage data
    } else {
        // this is place for function to write errors into log file
        return 0;
    }
}

