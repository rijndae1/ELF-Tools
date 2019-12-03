#include "strip-shdrs.h"

void errExit(char *errMsg) {
	fprintf(stderr, "%s\n", errMsg);
	exit(EXIT_FAILURE);
}

void readElf32Ehdr(int fd, Elf32_Ehdr *eh) {

	if(eh == NULL) {
		errExit("Elf32_Ehdr nulptr");
	}
	if(lseek(fd, (off_t)0, SEEK_SET) != (off_t)0) {
		errExit("Cant seek beginning of file!!");
	}
	if(read(fd, (void *)eh, (size_t)sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr)) {
		errExit("Cant read executable header!!");
	}
}

void readElf64Ehdr(int fd, Elf64_Ehdr *eh) {
	if(eh == NULL) {
		errExit("Elf64_Ehdr nulptr");
	}
	if(lseek(fd, (off_t)0, SEEK_SET) != (off_t)0) {
		errExit("Cant seek beginning of file!!");
	}
	if(read(fd, (void *)eh, (size_t)sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdr)) {
		errExit("Cant read executable header");
	}
}

bool checkElf(Elf32_Ehdr* eh) {
	const unsigned char magic[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};

	if(!memcmp(eh->e_ident, magic, sizeof(magic))) {
		printf("Format: ELF\n");
		return true;
	} else {
		printf("Not ELF\n");
		return false;
	}
}

bool is64bit(Elf32_Ehdr* eh) {
	if(eh->e_ident[EI_CLASS] == ELFCLASS64) {
		return true;
	}
	return false;
}

void writeElf64ShdrTbl(int fd, Elf64_Ehdr *eh64) {

	if(lseek(fd, (off_t)eh64->e_shoff, SEEK_SET) != (off_t)eh64->e_shoff) {
		errExit("Cant seek e_shoff");
	}
	char *buff =(char *)malloc(eh64->e_shentsize * eh64->e_shnum);
	if(buff == NULL) {
		errExit("Cant allocate memory");
	}
	for(int i = 0; i < eh64->e_shentsize * eh64->e_shnum; i++) {
		buff[i] = 0;
	}
	if(write(fd, buff, eh64->e_shentsize * eh64->e_shnum) != eh64->e_shentsize*eh64->e_shnum) {
		errExit("Error writing");
	}

}

void writeElf32ShdrTbl(int fd, Elf32_Ehdr *eh64) {

	if(lseek(fd, (off_t)eh64->e_shoff, SEEK_SET) != (off_t)eh64->e_shoff) {
		errExit("Cant seek e_shoff");
	}
	char *buff = (char *)malloc(eh64->e_shentsize * eh64->e_shnum);
	if(buff == NULL) {
		errExit("Cant allocate memory");
	}
	for(int i = 0; i < eh64->e_shentsize * eh64->e_shnum; i++) {
		buff[i] = 0;
	}
	if(write(fd, buff, eh64->e_shentsize * eh64->e_shnum) != eh64->e_shentsize*eh64->e_shnum) {
		errExit("Error writing");
	}

}