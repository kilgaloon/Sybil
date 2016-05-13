#include "stdio.h"
#include "stdlib.h"

// sybil libraries
#include "lib/parser.h"
#include "lib/storage.h"
#include "lib/log.h"

int main() {
    
    STORAGE storage;
    CONFIG *cnf = parseINI("/etc/sybil/sybil.ini");
    
    storage_init(&storage, cnf);
    
    // test

    set_key(&storage, "testKey", "something");
    set_key(&storage, "testKey1", "something");
    
    delete_key(&storage, "testKey1");
    
    get_key(&storage, "testKey", 1);
    
    return 0;
}