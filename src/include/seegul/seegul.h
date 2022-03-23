#pragma once
#include <seegul/elf.h>
#include <vector>
#include <string>

struct Elf_section{
	Elf32_Shdr * section;
	void *data;
	int data_sz;

};

class Elf{

public:
	Elf();
	void write(std::string);
	Elf32_Ehdr *eheader;

	std::vector<struct Elf_section*> sections;

	struct Elf_section *new_section();
	void done();

};