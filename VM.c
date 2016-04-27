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
#include <stdbool.h>

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

#ifndef NUMBER_OF_ADDRESSES
#define NUMBER_OF_ADDRESSES 1000
#endif // NUMBER_OF_ADDRESSES

typedef struct ADDRESS_INFO{
	unsigned long long int virt_address;
	unsigned long long int page_number;
	unsigned long long int offset;
	unsigned long long int frame_number;
	unsigned long long int physical_addr;
}ADDRESS_INFO;

void find_address(unsigned long long int address, int current_line_num, int num_entries);
unsigned long long int calculate_physical_address(int frame_num, int offset);
void free_list();

ADDRESS_INFO * list_of_addresses;
int num_entries;

// unsigned long long int page_number;
// unsigned long long int offset;
// long long int frame_number;
// long long int physical_addr;

// unsigned long long int addresses

int main(int argc, char * argv[]) {

	char address[50];
	int buffer = 50;
	int i;

	if(argc == 3) {
		char text_string[50];
		char bin_string[50];
		FILE *fp = fopen("deps/addresses.txt", "r");  // open the file

		list_of_addresses = malloc(sizeof(ADDRESS_INFO) * NUMBER_OF_ADDRESSES+2);

		for(i = 0; i < NUMBER_OF_ADDRESSES; i++) {
			list_of_addresses[i].virt_address = 0;
			list_of_addresses[i].page_number = 0;
			list_of_addresses[i].offset = 0;
			list_of_addresses[i].frame_number = 0;
			list_of_addresses[i].physical_addr = 0;
		}
		
		i = 0;
		num_entries = 0;
		while(fgets(address, buffer, fp)) {

			if(num_entries-1 == 15) {
				num_entries = 0;
			}
			find_address(atol(address), i, num_entries);
			printf("Virtual address: %llu ", list_of_addresses[i].virt_address);
			printf("Physical address: %llu ", list_of_addresses[i].physical_addr);
			printf("Frame Number: %llu ", list_of_addresses[i].frame_number);
			printf("Offset : %llu\n", list_of_addresses[i].offset);
			i++;
			num_entries++;
		}

		// for(i = 0; i < NUMBER_OF_ADDRESSES; i++) {
		// }

		fclose(fp);
		free_list();
		return 0;
	}
	else {
		printf("Error: You did not input the correct sequence!\n");
		return 1;
	}
}

/**
 * calculates the page number, offset and the physical address
 * @param address [description]
 */
void find_address(unsigned long long int address, int current_line_num, int num_entries) {

	/* offset 8 bits to find page number */
	list_of_addresses[current_line_num].virt_address = address;
	list_of_addresses[current_line_num].page_number	= address >> OFFSET_BITS;
	list_of_addresses[current_line_num].offset = address % PAGE_SIZE;
	list_of_addresses[current_line_num].frame_number = num_entries;
	list_of_addresses[current_line_num].physical_addr = calculate_physical_address(num_entries, list_of_addresses[current_line_num].offset);
	// page_number = address >> OFFSET_BITS;
	// printf("Page Number : %llu ", list_of_addresses[current_line_num].page_number);

	/* mod address by PAGE_SIZE to find the offset */
	// offset = address % PAGE_SIZE;
	// printf("Offset = %llu\n\n", list_of_addresses[current_line_num].offset);

}

unsigned long long int calculate_physical_address(int frame_num, int offset) {
	unsigned long long int calc = frame_num * PAGE_SIZE + offset;	
	return (frame_num * PAGE_SIZE + offset);
}

/**
 * free list of addresses so no memory leaks occur
 */
void free_list() {
	free(list_of_addresses);
}