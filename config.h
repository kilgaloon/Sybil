#ifndef CONFIG_H
#define CONFIG_H


typedef struct configuration_settings {
    int allowed_memory_size;
    int allowed_memory_key_size;  
    int allowed_memory_value_size;
    int allowed_cache_size;
} CONFIG;

void clearMemoryOnAssigment(char *buffer, char *value);


#endif