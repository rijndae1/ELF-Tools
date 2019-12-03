#ifndef PARSER_H
#define PARSER_H

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>
#include <stdbool.h>

void errExit(char *);
void readElf32Ehdr(int, Elf32_Ehdr*);
void readElf64Ehdr(int, Elf64_Ehdr*);
bool checkElf(Elf32_Ehdr*);
bool is64bit(Elf32_Ehdr*);
void printElf64Ehdr(Elf64_Ehdr*);
void readElf64ShdrTbl(int, Elf64_Ehdr*, Elf64_Shdr*);
char *readSection64(int, Elf64_Shdr*);
void printElf64ShdrTbl(int, Elf64_Ehdr*, Elf64_Shdr*);
void printSymTbl64(int, Elf64_Ehdr *, Elf64_Shdr[], int);
void printSym64(int, Elf64_Ehdr *, Elf64_Shdr[]);

#endif