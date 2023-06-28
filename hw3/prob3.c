#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


int count_lines(char *filename) {
    FILE *fp = fopen(filename, "r");
    int count = 0;
    char ch;

    if (fp == NULL) {
        printf("Cannot open file %s", filename);
        return 0;
    }

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }

    fclose(fp);
    return count;
}

int traverse_directory(char *dir_name) {
    DIR *dir;
    struct dirent *entry;
    int total_lines = 0;

    dir = opendir(dir_name);

    if (dir == NULL) {
        printf("Cannot open directory %s", dir_name);
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        char path[1001];

        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);
            total_lines += traverse_directory(path);
        } else {
            char *ext = strrchr(entry->d_name, '.');

            if (ext != NULL && strcmp(ext, ".txt") == 0) {
                snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);
                total_lines += count_lines(path);
            }
        }
    }

    closedir(dir);
    return total_lines;
}

int main() {
    char *dir_name = ".";
    int total_lines = traverse_directory(dir_name);

    printf("Total number of lines across all text files (.txt): %d\n", total_lines);

    return 0;
}
