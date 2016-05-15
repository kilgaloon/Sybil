#ifndef CONFIG_H
#define CONFIG_H

#define ERROR_LOG getenv("SYBIL_ERROR_LOG")


typedef struct configuration_settings {
    int allowed_memory_size;
    int allowed_memory_key_size;  
    int allowed_memory_value_size;
    int allowed_cache_size;
    int max_storage_instances;
} CONFIG;

void clearMemoryOnAssigment(char *buffer, char *value);
void setEnvironment(char *var, char *value);


#endif