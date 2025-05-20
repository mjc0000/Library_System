#include <stdio.h>
#include "fileio.h"

int saveDataToFile(const void* data, size_t size, size_t count, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }
    
    size_t written = fwrite(data, size, count, file);
    fclose(file);
    
    return (written == count);
}

int loadDataFromFile(void* data, size_t size, size_t maxCount, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return 0;
    }
    
    size_t read = fread(data, size, maxCount, file);
    fclose(file);
    
    return read;
} 