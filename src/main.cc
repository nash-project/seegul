#include <seegul/seegul.h>




const char shstrtab[] = {0, '.', 's', 'h', 's', 't', 'r', 't', 'a', 'b', 0, 
									'.', 't', 'e', 'x','t',0,
									'.', 's', 't','r','t','a','b', 0,
									'.','s','y','m','t', 'a', 'b', 0,
									'.', 'd', 'a', 't', 'a', 0,
									'.','r','e','l','.','t','e','x','t',0};

const char strtab[] = {0, 't','e','m','p','.','n','a','s','h', 0, 
								'_','s', 't', 'a', 'r', 't', 0, 'm', 's', 'g', 0};

const unsigned char data[] = {0xb8, 1, 0, 0, 0, 0xcd, 0x80}; 
const char data_[] = {'h','e','l','l','o','\0'};

int main(){

	Elf * elf = new Elf();

	elf->eheader->e_shstrndx = 1;

	struct Elf_section * shstrtab_section;
	struct Elf_section * text_section;
	struct Elf_section * strtab_section;
	struct Elf_section * symtab_section;
	struct Elf_section * data_section;
	struct Elf_section * rel_text_section;
	Elf32_Sym symbol_table[6];
	Elf32_Rel relocation_table[1];


	elf->new_section(); // NULL
	shstrtab_section = elf->new_section(); // .shstrtab
	text_section = elf->new_section(); // .text
	strtab_section = elf->new_section(); // .strtab
	symtab_section = elf->new_section(); // .symtab
	data_section = elf->new_section(); // .data
	rel_text_section = elf->new_section(); // .rel.text


// .shstrtab
// ===============================================
	shstrtab_section->data = (void*)shstrtab;
	shstrtab_section->data_sz = sizeof(shstrtab);
	shstrtab_section->section->sh_name = 1;
	shstrtab_section->section->sh_type = SHT_STRTAB;
	shstrtab_section->section->sh_addralign = 1;
// ===============================================


// .text
	text_section->data = (void*)data;
	text_section->data_sz = sizeof(data);
	text_section->section->sh_name = 11;
	text_section->section->sh_type = SHT_PROGBITS;
	text_section->section->sh_flags = SHF_ALLOC | SHF_EXECINSTR;


	
// .strtab
	strtab_section->data = (void*)strtab;
	strtab_section->data_sz = sizeof(strtab);
	strtab_section->section->sh_name = 17;
	strtab_section->section->sh_addralign = 1;
	strtab_section->section->sh_type = SHT_STRTAB;
	strtab_section->section->sh_flags = 0;


	
// .symtab
	symtab_section->section->sh_name = 25;
	symtab_section->section->sh_type = SHT_SYMTAB;
	symtab_section->section->sh_addralign = 4;
	symtab_section->section->sh_flags = 0;
	symtab_section->section->sh_link = 3;
	symtab_section->section->sh_info =  ELF32_ST_INFO(STB_LOCAL,5);
	symtab_section->section->sh_entsize = sizeof(Elf32_Sym);

// symbol table 
	symbol_table[0].st_name = 0;
	symbol_table[0].st_value = 0;
	symbol_table[0].st_size = 0;
	symbol_table[0].st_info = 0;
	symbol_table[0].st_other = 0;
	symbol_table[0].st_shndx = SHN_UNDEF;
	// orignal file name
	symbol_table[1].st_name = 1;
	symbol_table[1].st_value = 0;
	symbol_table[1].st_size = 0;
	symbol_table[1].st_info = ELF32_ST_INFO(STB_LOCAL, STT_FILE); // This is the value that st_info must have (because of TIS ELF)
	symbol_table[1].st_other = 0;
	symbol_table[1].st_shndx = SHN_ABS;  // The section where the symbol is
	// .text
	symbol_table[2].st_name = 0;
	symbol_table[2].st_value = 0;
	symbol_table[2].st_size = 0;
	symbol_table[2].st_info = ELF32_ST_INFO(STB_LOCAL, STT_SECTION);
	symbol_table[2].st_other = 0;
	symbol_table[2].st_shndx = 2;  // The section where the symbol is
	// .data
	symbol_table[3].st_name = 0;
	symbol_table[3].st_value = 0;
	symbol_table[3].st_size = 0;
	symbol_table[3].st_info = ELF32_ST_INFO(STB_LOCAL, STT_SECTION);
	symbol_table[3].st_other = 0;
	symbol_table[3].st_shndx = 5;
	
	// msg
	symbol_table[4].st_name = 18;  // Offset in the "strtab" section where the name start
	symbol_table[4].st_value = 0;
	symbol_table[4].st_size = 0;
	symbol_table[4].st_info = ELF32_ST_INFO(STB_LOCAL, STT_NOTYPE);
	symbol_table[4].st_other = 0;
	symbol_table[4].st_shndx = 4;  // The section where the symbol is


	// _start
	symbol_table[5].st_name = 11;  // Offset in the "strtab" section where the name start
	symbol_table[5].st_value = 0;
	symbol_table[5].st_size = 0;
	symbol_table[5].st_info = ELF32_ST_INFO(STB_GLOBAL, STT_NOTYPE);
	symbol_table[5].st_other = 0;
	symbol_table[5].st_shndx = 2;  // The section where the symbol is

	symtab_section->data = (void*)symbol_table; 
	symtab_section->data_sz = sizeof(symbol_table);


// .data
	data_section->data_sz = sizeof(data_);
	data_section->data = (void*)data_;
	data_section->section->sh_name = 33;
	data_section->section->sh_type = SHT_PROGBITS;
	data_section->section->sh_flags = SHF_ALLOC | SHF_WRITE;
	data_section->section->sh_addralign = 4;


// .rel.text
	relocation_table[0].r_offset = 1;
	relocation_table[0].r_info = ELF32_R_INFO(5, R_386_32);

	rel_text_section->data = (void*)relocation_table;
	rel_text_section->data_sz = sizeof(relocation_table);
	rel_text_section->section->sh_addralign = 4;
	rel_text_section->section->sh_name = 39;
	rel_text_section->section->sh_type = SHT_REL;
	rel_text_section->section->sh_link = 4;
	rel_text_section->section->sh_info = 2;
	rel_text_section->section->sh_entsize = sizeof(Elf32_Rel);


	elf->write("temp.o");

	elf->done();

	return 0;
}