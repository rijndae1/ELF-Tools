#include "NXbit.h"

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

bool checkElf(Elf32_Ehdr* eh) {
	const unsigned char magic[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};

	if(!memcmp(eh->e_ident, magic, sizeof(magic))) {
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


void readElf32PhdrTbl(int fd, Elf32_Ehdr *eh32, Elf32_Phdr *phTbl) {
	if(lseek(fd, (off_t)eh32->e_phoff, SEEK_SET) != (off_t)eh32->e_phoff) {
		errExit("Cant seek e_phoff");
	}
	for(int i = 0; i < eh32->e_phnum; i++) {
		if(read(fd, phTbl+i, eh32->e_phentsize) != eh32->e_phentsize) {
			errExit("Error reading phogram header table");
		}
	}
}

bool queryXbit32(Elf32_Ehdr *eh32, Elf32_Phdr *phTbl) {
	// Query X bit of the stack segment
	for(int i = 0; i < eh32->e_phnum; i++) {

		// Search for the stack segemnt
		if((phTbl+i)->p_type == PT_GNU_STACK) {
			if((phTbl+i)->p_flags & PF_X)
				return true;
			return false;
		}
	}
	return false;
}

void setXbit32(int fd, Elf32_Ehdr *eh32, Elf32_Phdr *phTbl) {
	for(int i = 0; i < eh32->e_phnum; i++) {
		if((phTbl+i)->p_type == PT_GNU_STACK) {
			off_t offset = eh32->e_phoff + sizeof(Elf32_Phdr) * i + 3*sizeof(Elf32_Word) + 2*sizeof(Elf32_Addr) + sizeof(Elf32_Off); 
			Elf32_Word newFlags = (phTbl+i)->p_flags | 0x00000001;
			if(lseek(fd, offset, SEEK_SET) != offset) {
				errExit("Error seeking flags offset");
			}

			if(write(fd, &newFlags, sizeof(Elf32_Word)) != sizeof(Elf32_Word)) {
				errExit("Error writing new flags");
			}
		}
	}
}

void clearXbit32(int fd, Elf32_Ehdr *eh32, Elf32_Phdr *phTbl) {
	int mask = 1;

	for(int i = 0; i < eh32->e_phnum; i++) {
		if((phTbl+i)->p_type == PT_GNU_STACK) {
			off_t offset = eh32->e_phoff + sizeof(Elf32_Phdr) * i + 3*sizeof(Elf32_Word) + 2*sizeof(Elf32_Addr) + sizeof(Elf32_Off); 
			Elf32_Word newFlags = ((phTbl+i)->p_flags & ~mask);			
			if(lseek(fd, offset, SEEK_SET) != offset) {
				errExit("Error seeking flags offset");
			}

			if(write(fd, &newFlags, sizeof(Elf32_Word)) != sizeof(Elf32_Word)) {
				errExit("Error writing new flags");
			}
		}
	}
}

