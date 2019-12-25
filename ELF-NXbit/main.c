#include "NXbit.h"

int main(int argc, char **argv) {

	int fd = -1;
	Elf32_Ehdr *eh32 = NULL;
	bool X = false;
	int opt = 0;
	char *path = NULL;
	int action = 1;

	while((opt = getopt(argc, argv, "q:s:c:")) != -1) {
		switch(opt) {
			case 'q': {
				action = 1;
				path = optarg;
				break;
			}
			case 's': {
				action = 2;
				path = optarg;
				break;
			}
			case 'c': {
				action = 3;
				path = optarg;
				break;
			}
			case '?': {
				printf("Usage: %s -[option] <file>\n"
					"options: q (query X bit) s (set X bit) c (clear X bit)\n", argv[0]);
				break;
			}
			default: {
				printf("Invalid arguments");
			}
		}
	}

	if(argc != 3) {
		printf("Usage: %s -[option] <file>\n"
				"options: q (query X bit) s (set X bit) c (clear X bit)\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	fd = open(path, O_RDWR);
	if(fd < 0) {
		errExit("Error opening file!!");
	}

	eh32 = malloc(sizeof(Elf32_Ehdr));
	if(eh32 == NULL) {
		errExit("Error allocating memory for Ehdr");
	}
	readElf32Ehdr(fd, eh32);
	if(!checkElf(eh32)) {
		errExit("Exiting!!");
	}

	if(!is64bit(eh32)) {
		Elf32_Phdr *phTbl = malloc(eh32->e_phentsize * eh32->e_phnum);
		readElf32PhdrTbl(fd, eh32, phTbl);
		if(action == 1) {
			X = queryXbit32(eh32, phTbl);
			if(X) {
				printf("Stack is executable\n");
			} else {
				printf("Stack is NOT executable\n");
			}
		} else if(action == 2) {
			setXbit32(fd, eh32, phTbl);
		} else {
			clearXbit32(fd, eh32, phTbl);
		}
	} else {
		errExit("Only supports 32-bit ELF executables");
	}

	return 0;
}
