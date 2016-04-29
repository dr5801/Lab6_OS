/**
 * Lab 6      : Virtual Memory
 * Programmer : Drew Rife
 * Course     : CMPE 320
 * Section    : 2(11-12:50pm)
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

#ifndef VM_H
#define VM_H

#ifndef TLB_SIZE
#define TLB_SIZE 16
#endif // TLB_SIZE

#ifndef PAGE_MASK
#define PAGE_MASK 255
#endif // PAGE_MASK

#ifndef PAGE_SIZE
#define PAGE_SIZE 256
#endif // PAGE_SIZE

#ifndef OFFSET_BITS
#define OFFSET_BITS 8
#endif // OFFSET_BITS

#ifndef PHYSICAL_MEMORY_SIZE
#define PHYSICAL_MEMORY_SIZE 65536
#endif // PHYSICAL_MEMORY_SIZE

/* Translation Lookaside Buffer*/
typedef struct TLB {
  unsigned int page_number;
  unsigned int frame_number;
}TLB;

int hit;                             // indicates the tlb was found
int value;                           // value stored from frame
int frame;                           // indicates the frame
int offset;                          // calculated offset from PAGE_MASK
int tlb_hit;                         // total number of tlb hits
int page_fault;                      // total number of page faults
int page_number;                     // page number calculated by OFFSET_BITS
int tlb_position;                    // indicates the current position of the tlb
int address_count;                   // total number of addresses counted
int virtual_address;                 // virtual address from addresses.txt
int current_tlb_size;                // indicates how much space has been taken up in the tlb
int physical_address;                // the actual physical address in memory
int phys_mem[PHYSICAL_MEMORY_SIZE];  // physical memory of program
double tlb_hit_rate;                 // calculated hit rate        
double page_fault_rate;              // calculated fault rate

#endif // VM_H