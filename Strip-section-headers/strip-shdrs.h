#ifndef STRIP_H
#define STRIP_H

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
void writeElf64ShdrTbl(int fd, Elf64_Ehdr *eh64);
void writeElf32ShdrTbl(int fd, Elf32_Ehdr *eh64);

#endif