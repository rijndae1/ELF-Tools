#include "parser.h"

/* A simple ELF64 parser that I wrote then I was first learning the ELF file format */
int main(int argc, char **argv) {

	int fd;
	// ELF32 header
	Elf32_Ehdr eh;

	if(argc != 2) {
		errExit("Usage: ./parser <elf>");
	}

	fd = open(argv[1], O_RDONLY);
	if(fd < 0) {
		errExit("Error opening file!!");
	}

	readElf32Ehdr(fd, &eh);
	if(!checkElf(&eh)) {
		errExit("Exiting!!");
	}

	if(is64bit(&eh)) {
		// ELF64 header
		Elf64_Ehdr eh64;
		// Section headers table
		// array of Elf64_Shdr structs
		Elf64_Shdr *shTbl;

		// Read and display ELF64 header
		readElf64Ehdr(fd, &eh64);
		printElf64Ehdr(&eh64);

		shTbl = (Elf64_Shdr*)malloc(eh64.e_shentsize * eh64.e_shnum);
		if(shTbl == NULL) {
			errExit("Failed to alloc mem for Elf64_Shdr");
		}
		// Read and display section headers
		readElf64ShdrTbl(fd, &eh64, shTbl);
		printElf64ShdrTbl(fd, &eh64, shTbl);

		printSym64(fd, &eh64, shTbl);
	} else {
		printf("32-bit\n");
	}

	return 0;
}
