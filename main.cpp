/*
Main program for the virtual memory project.
Make all of your modifications to this file.
You may add or rearrange any code or data as you need.
The header files page_table.h and disk.h explain
how to use the page table and disk interfaces.
*/

#include "page_table.h"
#include "disk.h"
#include "program.h"

#include <cassert>
#include <iostream>
#include <string.h>
#include <stdlib.h>

#define MAX_BLOCKS 10000

using namespace std;

// Prototype for test program
typedef void (*program_f)(char *data, int length);

// Number of physical frames
int nframes;

// Pointer to disk for access from handlers
struct disk *disk = nullptr;

// Keep track of which blocks are in physical memory
int physicalMemory[MAX_BLOCKS];

// Simple handler for pages == frames
void page_fault_handler_example(struct page_table *pt, int page) {
	cout << "page fault on page #" << page << endl;

	// Print the page table contents
	cout << "Before ---------------------------" << endl;
	page_table_print(pt);
	cout << "----------------------------------" << endl;

	// Map the page to the same frame number and set to read/write
	// TODO - Disable exit and enable page table update for example
	// exit(1);
	page_table_set_entry(pt, page, page, PROT_READ | PROT_WRITE);

	// Print the page table contents
	cout << "After ----------------------------" << endl;
	page_table_print(pt);
	cout << "----------------------------------" << endl;
}

// TODO - Handler(s) and page eviction algorithms
// Rand handler. 
// **Abstract some of this page replacement functionality for ease in future
void page_fault_handler_rand(struct page_table *pt, int page) {
	// cout << "page fault on page #" << page << endl;
	// Print the page table contents
	// cout << "Before ---------------------------" << endl;
	// page_table_print(pt);
	// cout << "----------------------------------" << endl;
  
  // Handling because of improper permissions or because page fault?
  int frameNum = -1;
  int bits = -1;
  page_table_get_entry(pt, page, &frameNum, &bits);
  if (bits != 0) {//improper permissions
    page_table_set_entry(pt, page, frameNum, PROT_READ | PROT_WRITE);
    return;
  }
  // else, true page fault
  
  // see if there are any empty frames
  int firstOpenFrame = 0;
  for (firstOpenFrame; firstOpenFrame<nframes; firstOpenFrame++) {
    if (physicalMemory[firstOpenFrame] == -1)
      break;
  }
  if (firstOpenFrame != nframes) { // take empty frame if there is one
    // cout << "page: " << page << "firstOpen: " << firstOpenFrame << endl;
    page_table_set_entry(pt, page, firstOpenFrame, PROT_READ);
    physicalMemory[firstOpenFrame] = page;
  }
  else { // evict page at random
    int randFrame = rand() % page_table_get_nframes(pt);
    // cout << "evict " << randFrame << endl;
    frameNum = -1;
    bits = -1;
    page_table_get_entry(pt, physicalMemory[randFrame], &frameNum, &bits);
    // cout << "page: " << page << " frameNum: " << frameNum << " bits: " << bits << endl;
    if (bits & PROT_WRITE) { // write back to disk if dirty, otherwise do not
      disk_write(disk, physicalMemory[randFrame], page_table_get_physmem(pt)+randFrame*PAGE_SIZE);
    }
    disk_read(disk, page, page_table_get_physmem(pt)+randFrame*PAGE_SIZE);     
    page_table_set_entry(pt, page, randFrame, PROT_READ);
    page_table_set_entry(pt, physicalMemory[randFrame], 0, 0);
    physicalMemory[randFrame] = page;
  }

	// Print the page table contents
	// cout << "After ----------------------------" << endl;
	// page_table_print(pt);
	// cout << "----------------------------------" << endl;
}

int main(int argc, char *argv[]) {
	// Check argument count
	if (argc != 5) {
		cerr << "Usage: virtmem <npages> <nframes> <rand|fifo|custom> <sort|scan|focus>" << endl;
		exit(1);
	}

	// Parse command line arguments
	int npages = atoi(argv[1]);
	nframes = atoi(argv[2]);
	const char *algorithm = argv[3];
	const char *program_name = argv[4];

	// Validate the algorithm specified
	if ((strcmp(algorithm, "rand") != 0) &&
	    (strcmp(algorithm, "fifo") != 0) &&
	    (strcmp(algorithm, "custom") != 0) &&
      (strcmp(algorithm, "example") != 0)) {
		cerr << "ERROR: Unknown algorithm: " << algorithm << endl;
		exit(1);
	}

	// Validate the program specified
	program_f program = NULL;
	if (!strcmp(program_name, "sort")) {
		if (nframes < 2) {
			cerr << "ERROR: nFrames >= 2 for sort program" << endl;
			exit(1);
		}

		program = sort_program;
	}
	else if (!strcmp(program_name, "scan")) {
		program = scan_program;
	}
	else if (!strcmp(program_name, "focus")) {
		program = focus_program;
	}
	else {
		cerr << "ERROR: Unknown program: " << program_name << endl;
		exit(1);
	}

	// TODO - Any init needed
  for (int i=0; i<nframes; i++) {
    physicalMemory[i] = -1; 
  }
  // // idk why this doesn't work lmao
  // // void *algorithm_p(page_table*, int);
  // program_f algorithm_p = NULL;
	// if (!strcmp(algorithm, "example")) {
		// algorithm_p = page_fault_handler_example;
	// }
	// else if (!strcmp(algorithm, "rand")) {
		// algorithm_p = page_fault_handler_example;
	// }
	// else if (!strcmp(algorithm, "fifo")) {
		// algorithm_p = page_fault_handler_example;
	// }
	// else if (!strcmp(algorithm, "custom")) {
		// algorithm_p = page_fault_handler_example;
	// }
	// else {
		// cerr << "ERROR: Unknown algorithm: " << algorithm << endl;
		// exit(1);
	// }

	// Create a virtual disk
	disk = disk_open("myvirtualdisk", npages);
	if (!disk) {
		cerr << "ERROR: Couldn't create virtual disk: " << strerror(errno) << endl;
		return 1;
	}


	// Create a page table
  struct page_table *pt;
  int notImplemented = 0;
  if (!strcmp(algorithm, "rand")) {
    pt = page_table_create(npages, nframes, page_fault_handler_rand);
  }
  else if (!strcmp(algorithm, "example")) {
    pt = page_table_create(npages, nframes, page_fault_handler_example);
  }
  else {
    cout << "Algorithm " << algorithm << " not yet implemented. Using rand instead." << endl;
    pt = page_table_create(npages, nframes, page_fault_handler_rand);
    notImplemented = 1;
  }
	if (!pt) {
		cerr << "ERROR: Couldn't create page table: " << strerror(errno) << endl;
		return 1;
	}

	// Run the specified program
	char *virtmem = page_table_get_virtmem(pt);
	program(virtmem, npages * PAGE_SIZE);

	// Clean up the page table and disk
	page_table_delete(pt);
	disk_close(disk);
  
  if (notImplemented) {
    cout << "If you would like to use example, specify this secret keyword." << endl;
  }


	return 0;
}
