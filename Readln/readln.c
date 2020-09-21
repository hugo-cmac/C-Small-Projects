#include <stdio.h>
#include <unistd.h>

unsigned int readln(int fildes, void *buf){
	int i = 0, n = 0;
	while((n += read(fildes, buf, 65536 )) != 0) {
		while (n > i){
			if(*(char*)(buf+i) == '\n')       
            			return i;
			i++;
		}
	}
	return i;
}

int main(int argc, char const *argv[]){
	char buffer[256];

	int n = readln(0, buffer);
	printf("%s", buffer);

	return 0;
}
