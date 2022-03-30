#pragma once
#include <seegul/elf.h>
#include <vector>
#include <string>
#include <map>

struct Elf32_section{
	Elf32_Shdr * section;
	void *data;
	int data_sz;
	int index;
};

class Elf32{

public:
	Elf32();
	void write(std::string);
	Elf32_Ehdr *eheader;

	std::vector<struct Elf32_section*> sections;

	struct Elf32_section *new_section();
	void done();

};

class SymTab32{
public:
	SymTab32();
	Elf32_Sym * new_symbol(int name,unsigned char info, int section_index);
	void add_symtab(Elf32_section*);
private:
	std::vector<Elf32_Sym*> symtab;
};

class RelTab32{
public:
	Elf32_Rel * new_relocation(int, int);
	void add_reltab(Elf32_section*);
private:
	std::vector<Elf32_Rel*> reltab;
};

class StrTab32{
public:
	void new_string(std::string);
	void add_strtab(Elf32_section*);
	int get_string(std::string);
private:
	std::vector<std::string> strtab;
};