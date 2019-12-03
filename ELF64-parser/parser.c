#include "parser.h"

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

void printElf64Ehdr(Elf64_Ehdr* eh64) {
	printf("ELF header:\n");
	printf("Magic: ");
	for(int i = 0; i < EI_NIDENT; i++) {
		printf("%.2x ", eh64->e_ident[i]);
	}
	printf("\n");

	printf("Class: ELF64\n");

	printf("Data encoding: ");
	switch(eh64->e_ident[EI_DATA]) {
		case ELFDATA2LSB:
			printf("Little endian\n");
			break;
		case ELFDATA2MSB:
			printf("Big endian\n");
			break;
		default:
			printf("Invalid\n");
	}

	printf("Version: ");
	if(eh64->e_ident[EI_VERSION] == EV_CURRENT) {
		printf("1 (Current)\n");
	} else {
		printf("\n");
	}

	printf("OS ABI: ");
	switch(eh64->e_ident[EI_OSABI])
	{
		case ELFOSABI_SYSV:
			printf("UNIX System V ABI\n");
			break;

		case ELFOSABI_HPUX:
			printf("HP-UX\n");
			break;

		case ELFOSABI_NETBSD:
			printf("NetBSD\n");
			break;

		case ELFOSABI_LINUX:
			printf("Linux\n");
			break;

		case ELFOSABI_SOLARIS:
			printf("Sun Solaris\n");
			break;

		case ELFOSABI_AIX:
			printf("IBM AIX\n");
			break;

		case ELFOSABI_IRIX:
			printf("SGI Irix\n");
			break;

		case ELFOSABI_FREEBSD:
			printf("FreeBSD\n");
			break;

		case ELFOSABI_TRU64:
			printf("Compaq TRU64 UNIX\n");
			break;

		case ELFOSABI_MODESTO:
			printf("Novell Modesto\n");
			break;

		case ELFOSABI_OPENBSD:
			printf("OpenBSD\n");
			break;

		case ELFOSABI_ARM_AEABI:
			printf("ARM EABI\n");
			break;

		case ELFOSABI_ARM:
			printf("ARM\n");
			break;

		case ELFOSABI_STANDALONE:
			printf("Standalone (embedded) app\n");
			break;

		default:
			printf("Unknown (0x%x)\n", eh64->e_ident[EI_OSABI]);
			break;
	}

	printf("ABI Version: %d\n", eh64->e_ident[EI_ABIVERSION]);

	printf("Filetype: ");
	switch(eh64->e_type)
	{
		case ET_NONE:
			printf("N/A (0x0)\n");
			break;

		case ET_REL:
			printf("Relocatable\n");
			break;

		case ET_EXEC:
			printf("Executable\n");
			break;

		case ET_DYN:
			printf("Shared Object\n");
			break;
		default:
			printf("Unknown (0x%x)\n", eh64->e_type);
			break;
	}

	printf("Machine: ");
	switch(eh64->e_machine)
	{
		case EM_NONE:
			printf("None (0x0)\n");
			break;

		case EM_386:
			printf("INTEL x86 (0x%x)\n", EM_386);
			break;

		case EM_X86_64:
			printf("AMD x86_64 (0x%x)\n", EM_X86_64);
			break;

		case EM_AARCH64:
			printf("AARCH64 (0x%x)\n", EM_AARCH64);
			break;

		default:
			printf(" 0x%x\n", eh64->e_machine);
			break;
	}

	printf("Entry point %#08llx\n", eh64->e_entry);

	printf("ELF Header size: %#x\n", eh64->e_ehsize);

	// program header table contains e_phnum entries of size e_phentsize each
	printf("start of program header: %#llx (bytes into file)\n", eh64->e_phoff);
	printf("Size of program header entry: %#x (bytes)\n", eh64->e_phentsize);
	printf("Num of program header entries: %#x\n", eh64->e_phnum);

	// section header table contains e_shnum entries of size e_shentsize each
	printf("Start of section header: %#llx (bytes into file)\n", eh64->e_shoff);
	printf("Size of section header entry: %#x (bytes)\n", eh64->e_shentsize);
	printf("Num of section header entries: %#x\n", eh64->e_shnum);
	// table that contains the name of the sections
	printf("Section header string table index: %#x\n", eh64->e_shstrndx);

	int ef = eh64->e_flags;
	/* 0x00 on Intel */
	printf("Flags: 0x%x\n", ef);
	/* For ARM */
	if(ef & EF_ARM_RELEXEC)
		printf(",RELEXEC ");

	if(ef & EF_ARM_HASENTRY)
		printf(",HASENTRY ");

	if(ef & EF_ARM_INTERWORK)
		printf(",INTERWORK ");

	if(ef & EF_ARM_APCS_26)
		printf(",APCS_26 ");

	if(ef & EF_ARM_APCS_FLOAT)
		printf(",APCS_FLOAT ");

	if(ef & EF_ARM_PIC)
		printf(",PIC ");

	if(ef & EF_ARM_ALIGN8)
		printf(",ALIGN8 ");

	if(ef & EF_ARM_NEW_ABI)
		printf(",NEW_ABI ");

	if(ef & EF_ARM_OLD_ABI)
		printf(",OLD_ABI ");

	if(ef & EF_ARM_SOFT_FLOAT)
		printf(",SOFT_FLOAT ");

	if(ef & EF_ARM_VFP_FLOAT)
		printf(",VFP_FLOAT ");

	if(ef & EF_ARM_MAVERICK_FLOAT)
		printf(",MAVERICK_FLOAT ");
	printf("\n");
}

void readElf64ShdrTbl(int fd, Elf64_Ehdr *eh64, Elf64_Shdr *shTbl) {

	if(lseek(fd, (off_t)eh64->e_shoff, SEEK_SET) != (off_t)eh64->e_shoff) {
		errExit("Cant seek e_shoff");
	}
	for(int i = 0; i < eh64->e_shnum; i++) {
		if(read(fd, (void *)&(shTbl[i]), eh64->e_shentsize) != eh64->e_shentsize) {
			errExit("Error reading section header table");
		}
	}
}

char *readSection64(int fd, Elf64_Shdr* sh) {
	char *sect = malloc(sh->sh_size);
	if(sect == NULL) {
		errExit("Error allocating memory for a section");
	}
	if(lseek(fd, (off_t)sh->sh_offset, SEEK_SET) != (off_t)sh->sh_offset) {
		errExit("Error seeking section offset");
	}
	if(read(fd, (void *)sect, sh->sh_size) != sh->sh_size) {
		errExit("Error reading section");
	}
	return sect;
}

void printElf64ShdrTbl(int fd, Elf64_Ehdr* eh64, Elf64_Shdr* shTbl) {

	char *shStrTbl;
	int flg = 0;

	shStrTbl = readSection64(fd, &(shTbl[eh64->e_shstrndx]));

	/* 
	The secion header table is an array of ElfXX_Ehdr structs
	located at e_shoff
	has e_num entries 
	each entry is of size e_shentsize 
	*/
	printf("\nSection headers table:\n");
	// Loop over the structs and print their info
	for(int i = 0; i < eh64->e_shnum; i++) {
		printf("Section [%d]:\n", i+1);
		printf("\tName: %s:\n", shTbl[i].sh_name + shStrTbl);

		// type
		printf("\tType: ");
		// Only common types implemented
		switch(shTbl[i].sh_type) {
			case SHT_NULL:
				printf("NULL\n");
				break;
			case SHT_PROGBITS:
				printf("PROGBITS\n");
				break;
			case SHT_SYMTAB:
				printf("SYMTAB\n");
				break;
			case SHT_STRTAB:
				printf("STRTAB\n");
				break;
			case SHT_RELA:
				printf("RELA\n");
				break;
			case SHT_HASH:
				printf("HASH\n");
				break;
			case SHT_DYNAMIC:
				printf("DYNAMIC\n");
				break;
			case SHT_NOBITS:
				printf("NOBITS\n");
				break;
			case SHT_REL:
				printf("REL\n");
				break;
			case SHT_DYNSYM:
				printf("DYNSYM\n");
				break;
			case SHT_NOTE:
				printf("NOTE\n");
				break;
			case SHT_GNU_HASH:
				printf("GNUHASH\n");
				break;
			case SHT_GNU_verneed:
				printf("VERNEED\n");
				break;
			case SHT_GNU_versym:
				printf("VERSYM\n");
				break;
			default:
				printf("%#8x\n", shTbl[i].sh_type);
		}

		// flags
		flg = shTbl[i].sh_flags;
		printf("\tFlags: ");
		if(flg & SHF_WRITE)
			putchar('W');
		if(flg & SHF_ALLOC)
			putchar('A');
		if(flg & SHF_EXECINSTR)
			putchar('X');
		if(flg & SHF_MERGE)
			putchar('M');
		if(flg & SHF_STRINGS)
			putchar('S');
		if(flg & SHF_INFO_LINK)
			putchar('I');
		if(flg & SHF_LINK_ORDER)
			putchar('L');
		if(flg & SHF_OS_NONCONFORMING)
			putchar('O');
		if(flg & SHF_GROUP)
			putchar('G');
		if(flg & SHF_TLS)
			putchar('T');
		if(flg & SHF_COMPRESSED)
			putchar('C');
		if(flg & SHF_MASKOS)
			putchar('o');
		if(flg & SHF_MASKPROC)
			putchar('p');
		if(flg & SHF_ORDERED)
			putchar('s');
		if(flg & SHF_EXCLUDE)
			putchar('E');
		putchar('\n');
		
		// Addr, offset, size,...
		printf("\tAddress: 0x%016llx\n", shTbl[i].sh_addr);
		printf("\tOffset: 0x%06llx\n", shTbl[i].sh_offset);
		printf("\tSize: 0x%08llx\n", shTbl[i].sh_size);
		printf("\tAlignment (multiple of): %lld\n", shTbl[i].sh_addralign);
		printf("\tLink: %d\n", shTbl[i].sh_link);
		printf("\tInfo: %d\n", shTbl[i].sh_info);
		printf("\tES (if it holds a table): 0x%02llx", shTbl[i].sh_entsize);
		putchar('\n');
	}
	printf("Flags key: W (write), A (alloc), X (execute), M (merge), S (strings), I (info),\nL (link order), O (extra OS processing required), G (group), T (TLS),\nC (compressed), x (unknown), o (OS specific), E (exclude), p (processor specific)\n");
}

void printSymTbl64(int fd, Elf64_Ehdr *eh64, Elf64_Shdr shTbl[], int tblIndex) {
	Elf64_Sym* symTbl;
	char *strTbl;
	int symCount;

	symTbl = (Elf64_Sym*)readSection64(fd, &shTbl[tblIndex]);
	int strTblNdx = shTbl[tblIndex].sh_link;
	strTbl = readSection64(fd, &shTbl[strTblNdx]);

	symCount = (shTbl[tblIndex].sh_size/sizeof(Elf64_Sym));
	printf("Found %d symbols\n", symCount);

	for(int i = 0; i < symCount; i++) {
		printf("%s ", (strTbl + symTbl[i].st_name));
		printf("0x%08llx ", symTbl[i].st_value);
		printf("0x%02x ", ELF32_ST_BIND(symTbl[i].st_info));
		printf("0x%02x\n", ELF32_ST_TYPE(symTbl[i].st_info));
	}
}

void printSym64(int fd, Elf64_Ehdr *eh64, Elf64_Shdr shTbl[]) {
	for(int i = 0; i < eh64->e_shnum; i++) {
		if((shTbl[i].sh_type == SHT_SYMTAB) || (shTbl[i].sh_type == SHT_DYNSYM)) {
			printf("\nSection [%d]\n", i);
			printSymTbl64(fd, eh64, shTbl, i);
		}
	}
}