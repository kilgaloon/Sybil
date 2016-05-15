#include "stdio.h"
#include "stdlib.h"

// sybil libraries
#include "lib/parser.h"
#include "lib/storage.h"
#include "lib/log.h"
#include "config.h"

int main() {
    
    STORAGE storage;
    CONFIG *cnf = parseINI("/etc/sybil/sybil.ini");
    
    storage_init(&storage, cnf);
    
    return 0;
}