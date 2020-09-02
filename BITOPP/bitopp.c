#include <stdio.h>

int bitfilling(unsigned char *stream, int *bitsLeft, int pos, int value, int size){
	
	while (*bitsLeft <= size){
		size -= *bitsLeft;
		*bitsLeft = 8;
		stream[pos] |= (value>>size);
		pos++;
	}

	if (size){
		*bitsLeft -= size;
		stream[pos] |= value<<*bitsLeft;
	}

	return pos;
}

int bitreading(unsigned char *stream, int *bitsLeft, int *pos, int size){
    unsigned int n = 0;
    int mask = 255;

    while (*bitsLeft <= size){
        size -= *bitsLeft;
        n |= (((stream[*pos] & (mask >> (8-*bitsLeft)))) << size);
        *bitsLeft = 8;
        *pos = *pos + 1;
    }

    if (size){
        mask >>= (8 - *bitsLeft);
        *bitsLeft -= size;
        n |= (stream[*pos] & mask) >> *bitsLeft;
    }

    return n;
}

void printBinary(unsigned char *stream, int nbytes) {
  	for (int i = 0; i < nbytes; i++) {
		for (int b = 0; b < 8; b++){
			if((stream[i] >> (7-b)) & 1)
				printf("1");
			else
				printf("0");
		}
		getchar();
  	}
}

int main(int argc, char const *argv[]){
    unsigned char stream[256] = {0};
    int pos = 0, bits = 8;

    pos = bitfilling(stream, &bits, pos, 30, 5);
    pos = bitfilling(stream, &bits, pos, 14, 4);
    pos = bitfilling(stream, &bits, pos,  6, 3);

    pos = 0;
    bits = 8;

	int n = bitreading(stream, &bits, &pos, 5);
    printf("-> %d\n", n);

    n = bitreading(stream, &bits, &pos, 4);
    printf("-> %d\n", n);

    n = bitreading(stream, &bits, &pos, 3);
    printf("-> %d\n", n);

    return 0;
}
