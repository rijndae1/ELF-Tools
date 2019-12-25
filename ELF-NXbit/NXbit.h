#ifndef NXBIT_H
#define NXBIT_H

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>
#include <stdbool.h>

void errExit(char *);
bool is64bit(Elf32_Ehdr*);
void readElf32Ehdr(int, Elf32_Ehdr*);
bool checkElf(Elf32_Ehdr*);

void readElf32PhdrTbl(int, Elf32_Ehdr *, Elf32_Phdr *);
bool queryXbit32(Elf32_Ehdr *, Elf32_Phdr *);
void setXbit32(int, Elf32_Ehdr *, Elf32_Phdr *);
void clearXbit32(int, Elf32_Ehdr *, Elf32_Phdr *);

#endif
