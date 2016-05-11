#ifndef STORAGE_H
#define	STORAGE_H

typedef struct storage {
    char ***data;
    int size;
    int allowed_memory_key_size; // this is set in .ini configuration file
    int allowed_memory_value_size; // this is set in .ini configuration file
    int memory_size;
    int allowed_memory_size; // this is set in .ini configuration file
} STORAGE;
void storage_init(STORAGE *storage, CONFIG *config);

void set_key(STORAGE *storage, char *key, char *value);
int get_key(STORAGE *storage, char *key);

int check_for_storage_size(STORAGE *storage);
void memory_size_used(STORAGE *storage, int memory);
int safe_member_initialize(STORAGE *storage, int member);

#endif	/* STORAGE_H */

