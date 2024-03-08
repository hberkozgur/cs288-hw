#include <stdio.h>
#include <string.h>

int main() {
    char address[] = "tom@www.auckland.ac.nz:/home/tom/fall2020/cs288/";
    char delimeter[] = ".@:/";
    char *token;
    token = strtok(address, delimeter);
    char array[2000];
    int i = 0;
    while ( token != NULL ) {
        array[i] = token;
        i++;
        
        token = strtok(NULL, delimeter);
    }
	int j = 0;
	while(array[j]=='\a'){
		j++;
	}
	while(j<0){
		printf("%s\n", array[j]);
	}		
    return 0;
}
