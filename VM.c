/**
 * Lab 6      : Virtual Memory
 * Programmer : Drew Rife
 * Course     : CMPE 320
 * Section    : 2(11-12:50pm)
 * Instructor : S. Lee
 */

#include "VM.h"

int main(int argc, char *argv[]) {

  if(argc == 3) {
    /* open addresses text file and backing store bin file */
    FILE *bin_file = fopen(argv[1], "rb");
    FILE *address_file = fopen(argv[2], "r");

    char buffer[PAGE_SIZE];  // buffer to be used whenever needed - has PAGE_SIZE (256)
    
    int page_table[PAGE_SIZE];
    memset(page_table, -1, PAGE_SIZE*sizeof(int));  // fill page table array with -1 (initialise)

    TLB tlb_info[TLB_SIZE];
    memset(page_table, -1, TLB_SIZE*sizeof(char));  // convert to single-byte char
    
    int i;
    /* scan the file and read the address into virtual address -> Make sure there is an actual value there (TRUE) */
    while(fscanf(address_file, "%d", &virtual_address) == true) {
      address_count++;  // address was found if it entered the while loop
     
      /* get the page number and the offset from the virtual address just read in */
      page_number = virtual_address ^ PAGE_MASK;  // XOR for bitwise & of inverse of PAGE_MASK
      page_number = page_number >> OFFSET_BITS;
      offset = virtual_address & PAGE_MASK;
      hit = -1;

      i = 0;
      /* search in the tlb to see if page number is already stored */
      while(i < current_tlb_size) {
        if(tlb_info[i].page_number == page_number) {
          hit = tlb_info[i].frame_number;
          physical_address = (PAGE_SIZE*hit) + offset;
        }
        i++;
      }

      /* read from the BACKINGSTORE.bin file */
      if(page_table[page_number] == -1) {
        fseek(bin_file, page_number*PAGE_SIZE, SEEK_SET);
        fread(buffer, sizeof(char), PAGE_SIZE, bin_file);
        page_table[page_number] = frame;
        
        for(i = 0; i < PAGE_SIZE; i++) {
          phys_mem[frame*PAGE_SIZE + i] = buffer[i];
        }

        page_fault++; // increase page fault
        frame++;      // increase frame

        /* page fault occurred */
        if(current_tlb_size == TLB_SIZE) {
          current_tlb_size--;
        }

        /* go forward in the TLB -------> FIFO */
        for(tlb_position = current_tlb_size; tlb_position > 0; tlb_position--) {
          tlb_info[tlb_position].page_number = tlb_info[tlb_position-1].page_number;
          tlb_info[tlb_position].frame_number = tlb_info[tlb_position-1].frame_number;
        }

        /* TLB can still be filled */
        if(current_tlb_size < TLB_SIZE) {
          current_tlb_size++;
        }

        tlb_info[0].page_number = page_number;
        tlb_info[0].frame_number = page_table[page_number];

        /* set the physical address to the (frame * PAGE_SIZE) + offset */
        physical_address = page_table[page_number]*PAGE_SIZE + offset;
      }
      else if(hit != -1) {
        tlb_hit++;
      }
      else {
        /* set the physical address to the (frame * PAGE_SIZE) + offset */
        physical_address = page_table[page_number]*PAGE_SIZE + offset;
      }

      /* set value to the value stored in the physical address of the physical memory */
      value = phys_mem[physical_address];
      printf("Virtual address: %d Physical Address: %d Value: %d \n", virtual_address, physical_address, value);
    }
    tlb_hit--;

    /* calculate the page_fault_rate and tlb_hit_rate */
    page_fault_rate = page_fault*1.0f / address_count;
    tlb_hit_rate = tlb_hit*1.0f / address_count;

    /* print results */
    printf("Number of Translated Addresses = %d\n", address_count);
    printf("Page Faults = %d\n", page_fault);
    printf("Page Fault Rate = %.03f\n", page_fault_rate);
    printf("TLB Hits = %d\n", tlb_hit);
    printf("TLB Hit Rate %.03f\n", tlb_hit_rate);

    /* close the files */
    fclose(address_file);
    fclose(bin_file);
    return 0;
  }
  else {
    printf("Error: you didn't input the correct parameters!\n");
    return 1;
  }
}