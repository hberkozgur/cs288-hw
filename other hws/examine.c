#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

void print_memory_pointer(void *ptr, size_t size) {
    unsigned char *p = (unsigned char *)ptr;
    for (int i = size - 1; i >= 0; i--) {
        printf("%02x ", p[i]);
    }
}

void print_memory_value(void *ptr, size_t size) {
    unsigned char *p = (unsigned char *)ptr;
    for (int i = size - 1; i >= 0; i--) {
        printf("%02x(%c) ", p[i], isprint(p[i]) ? p[i] : '.');
    }
}
int main(int argc, char **argv) {
    printf("argv    | ");
    print_memory_pointer(&argv, sizeof(argv));
    printf("| %p\n\n", (void *)&argv);

    for (int i = 0; i < argc; i++) {
        printf("argv[%d] | ", i);
        print_memory_pointer(&argv[i], sizeof(argv[i]));
        printf("| %p\n", (void *)&argv[i]);
    }

    printf("\n");

    unsigned char *start = (unsigned char *)argv[0];
    while ((uintptr_t)start % 8 != 0) {
        start--;
    }

    unsigned char *end = (unsigned char *)argv[argc - 1];
    end += strlen(argv[argc - 1]) + 1;
    while ((uintptr_t)end % 8 != 0) {
        end++;
    }

    for (unsigned char *p = start; p < end; p += 8) {
        printf("       | ");
        print_memory_value(p, 8);
        printf("| %p\n", (void *)p);
    }

    return 0;
}