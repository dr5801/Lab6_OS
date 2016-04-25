/**
 * Lab 6 	  : Virtual Memory
 * Programmer : Drew Rife & Alec Waddelow
 * Course 	  : CMPE 320
 * Section 	  : 2(11-12:50pm)
 * Instructor : S. Lee
 */

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#ifndef TLB_SIZE
#define TLB_SIZE 16
#endif // TLB_SIZE

#ifndef PAGES
#define PAGES 256
#endif // PAGES

#ifndef PAGE_MASK
#define PAGE_MASK 255
#endif // PAGE_MASK

#ifndef PAGE_SIZE
#define PAGE_SIZE 256
#endif // PAGE_SIZE

#ifndef OFFSET_BITS
#define OFFSET_BITS 8
#endif // OFFSET_BITS

#ifndef OFFSET_MARK
#define OFFSET_MARK 255
#endif // OFFSET_MARK

void Find_Address(unsigned long long int address);

int main() {

	FILE *fp = fopen("deps/addresses.txt", "r");  // open the file
	char buffer[50];
	unsigned long long int address;
	int i;

	i = 1;
	while(!feof(fp)) {
		fscanf(fp, "%s", buffer);
		address = atol(buffer);
		Find_Address(address);
		i++;
	}

	fclose(fp);
	return 0;
}

void Find_Address(unsigned long long int address) {
	// address = MASK_BITS(address);

	printf("Virtual address : %llu ", address);

	/* offset 8 bits to find page number */
	unsigned long long int page_number = address >> OFFSET_BITS;
	printf("Page Number : %llu ", page_number);

	/* mod address by PAGE_SIZE to find the offset */
	unsigned long long int offset_number = address % PAGE_SIZE;
	printf("Offset = %llu\n\n", offset_number);

}