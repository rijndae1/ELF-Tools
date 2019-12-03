#include "strip-shdrs.h"

/* A simple tool that fills the ELF section headers table with 0s
thus making the job harder for other programs that use section headers 
(gdb, objdump, file,...) */

int main(int argc, char **argv) {

	int fd;
	Elf32_Ehdr eh;
	Elf64_Ehdr eh64;

	if(argc != 2) {
		errExit("Usage: ./parser <elf>");
	}

	fd = open(argv[1], O_RDWR);
	if(fd < 0) {
		errExit("Error opening file!!");
	}

	readElf32Ehdr(fd, &eh);

	if(!checkElf(&eh)) {
		errExit("Not an ELF");
	}

	if(is64bit(&eh)) {
		readElf64Ehdr(fd, &eh64);

		writeElf64ShdrTbl(fd, &eh64);
	} else {
		writeElf32ShdrTbl(fd, &eh);
	}
	puts("Done");

	return 0;
}
