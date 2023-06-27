#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_PATH_LENGTH 1000

int iLines(const char *fname) {
    f *f = fopen(fname, "r");
    if (f == NULL) {
        fprintf(stderr, "Error opening file: %s\n", fname);
        return 0;
    }

    int i = 0;
    char ch;
    while ((ch = fgetc(f)) != EOF) {
        if (ch == '\n') {
            i++;
        }
    }

    fclose(f);
    return i;
}

void traverseDir(const char *dirPath, int *linei) {
    DIR *dir = opendir(dirPath);
    if (dir == NULL) {
        fprintf(stderr, "Error opening directory: %s\n", dirPath);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {  
            char fPath[MAX_PATH_LENGTH];
            snprintf(fPath, sizeof(fPath), "%s/%s", dirPath, entry->d_name);

            if (strstr(entry->d_name, ".txt") != NULL) {                  *linei += iLines(fPath);
            }
        } else if (entry->d_type == DT_DIR) {  
if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                char subDirPath[MAX_PATH_LENGTH];
                snprintf(subDirPath, sizeof(subDirPath), "%s/%s", dirPath, entry->d_name);
                traverseDir(subDirPath, linei);
            }
        }
    }

    closedir(dir);
}

int main() {
    char dirPath[MAX_PATH_LENGTH];

    printf("Enter directory path: ");
    scanf("%s", dirPath);

    int linei = 0;
    traverseDir(dirPath, &linei);

    printf("Total line i: %d\n", linei);

    return 0;
}
