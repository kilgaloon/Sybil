#include "stdio.h"
#include "stdlib.h"

// sybil libraries
#include "lib/parser.h"
#include "lib/storage.h"

int main() {
    
    STORAGE storage;
    CONFIG *cnf = parseINI("/etc/sybil/sybil.ini");
    
    storage_init(&storage, cnf);
	
    set_key(&storage, "testKey", "something");
    
    return 0;
}