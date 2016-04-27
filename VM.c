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

#ifndef PAGE_NUMBER_MASK
#define PAGE_NUMBER_MASK 65280;
#endif // PAGE_NUMBER_MASK

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

typedef struct TLB {
	unsigned long long int page_num;
	unsigned long long int frame_num;
}TLB;

void find_address(unsigned long long int address, int current_line_num, int num_entries);
unsigned long long int calculate_physical_address(int frame_num, int offset);
void free_list();


ADDRESS_INFO * list_of_addresses;
TLB tlb[PAGE_SIZE];

int num_entries;
int address_count = 0;
int page_fault_count = 0;
int tlb_hit = 0;
double tlb_rate = 0;
int hit = 0;
int tlb_index = 0;

int page_table[PAGE_SIZE];



int main(int argc, char * argv[]) {

	char address[50];
	int buffer = 50;
	int i;

	memset(page_table, -1, 256*sizeof(int));
	memset(page_table, -1, 16*sizeof(char));

	if(argc == 3) {
		char text_string[50];
		char bin_string[50];
		FILE *bin_file = fopen(argv[1], "rb");	   // open the bin file
		FILE *address_file = fopen(argv[2], "r");  // open the addresses file

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
		while(fgets(address, buffer, address_file)) {

			if(num_entries-1 == 15) {
				num_entries = 0;
			}
			find_address(atol(address), i, num_entries);
			i++;
			num_entries++;
		}

		fclose(address_file);
		fclose(bin_file);
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
 * @param address
 */
void find_address(unsigned long long int address, int current_line_num, int num_entries) {

	list_of_addresses[current_line_num].virt_address = address;
	list_of_addresses[current_line_num].page_number	= address >> OFFSET_BITS;
	list_of_addresses[current_line_num].offset = address % PAGE_SIZE;
	list_of_addresses[current_line_num].frame_number = num_entries;
	list_of_addresses[current_line_num].physical_addr = calculate_physical_address(num_entries, list_of_addresses[current_line_num].offset);
	

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