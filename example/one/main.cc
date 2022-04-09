#include <seegul/seegul.h>

const unsigned char data[] = { 0xB9, 0x00, 0x00, 0x00, 0x00, 0xE8, 0x07, 0x00, 0x00, 0x00, 0xB8, 0x01, 0x00, 0x00, 0x00, 0xCD, 0x80, 0xB8, 0x02, 0x00, 0x00, 0x00, 0xC3 };
const char data_[] = {'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0xa, 0};

int main(){

	Elf32 * elf = new Elf32();

	

	struct Elf32_section * shstrtab_section;
	struct Elf32_section * text_section;
	struct Elf32_section * strtab_section;
	struct Elf32_section * symtab_section;
	struct Elf32_section * data_section;
	struct Elf32_section * rel_text_section;
	SymTab32 * symbol_table = new SymTab32();
	RelTab32 * rel_tab = new RelTab32();
	StrTab32 * strtab = new StrTab32();
	StrTab32 * shstrtab = new StrTab32();
	Elf32_Sym * csym;
	


	elf->new_section(); // NULL
	shstrtab_section = elf->new_section(); // .shstrtab
	text_section = elf->new_section(); // .text
	strtab_section = elf->new_section(); // .strtab
	symtab_section = elf->new_section(); // .symtab
	data_section = elf->new_section(); // .data
	rel_text_section = elf->new_section(); // .rel.text

	elf->eheader->e_shstrndx = shstrtab_section->index;


// .shstrtab
// ===============================================

	shstrtab->new_string(".shstrtab");
	shstrtab->new_string(".text");
	shstrtab->new_string(".strtab");
	shstrtab->new_string(".symtab");
	shstrtab->new_string(".data");
	shstrtab->new_string(".rel.text");
	shstrtab->add_strtab(shstrtab_section);

	shstrtab_section->section->sh_name = shstrtab->get_string(".shstrtab");

// ===============================================


// .text
	text_section->data = (void*)data;
	text_section->data_sz = sizeof(data);
	text_section->section->sh_name = shstrtab->get_string(".text");
	text_section->section->sh_type = SHT_PROGBITS;
	text_section->section->sh_flags = SHF_ALLOC | SHF_EXECINSTR;
	text_section->section->sh_addralign = 4;


	
// .strtab
	
	strtab->new_string("temp.nash");
	strtab->new_string("_start");
	strtab->new_string("_msg");
	strtab->new_string("puts");

	strtab->add_strtab(strtab_section);
	strtab_section->section->sh_name = shstrtab->get_string(".strtab");
	
// .symtab
	symtab_section->section->sh_name = shstrtab->get_string(".symtab");
	symtab_section->section->sh_link = strtab_section->index;

	symbol_table->new_symbol(0, 0, 0, SHN_UNDEF);

	symbol_table->new_symbol(strtab->get_string("temp.nash"), STB_LOCAL, STT_FILE, SHN_ABS);

	// .text
	symbol_table->new_symbol(0, STB_LOCAL, STT_SECTION, text_section->index);
	// _start
	symbol_table->new_symbol(strtab->get_string("_start"), STB_GLOBAL, STT_NOTYPE, text_section->index);
	// .data
	symbol_table->new_symbol(0, STB_LOCAL, STT_SECTION, data_section->index);
	// msg
	symbol_table->new_symbol(18, STB_LOCAL, STT_NOTYPE, 5);
	
	// puts function
	csym = symbol_table->new_symbol(strtab->get_string("puts"), STB_GLOBAL, STT_NOTYPE, text_section->index);
	csym->st_value = 0x11;

	symbol_table->add_symtab(symtab_section);

// .data
	data_section->data_sz = sizeof(data_);
	data_section->data = (void*)data_;
	data_section->section->sh_name = shstrtab->get_string(".data");
	data_section->section->sh_type = SHT_PROGBITS;
	data_section->section->sh_flags = SHF_ALLOC | SHF_WRITE;
	data_section->section->sh_addralign = 4;


// .rel.text
	rel_tab->new_relocation(1, ELF32_R_INFO(4, R_386_32));

	rel_tab->add_reltab(rel_text_section);

	rel_text_section->section->sh_name = shstrtab->get_string(".rel.text");
	rel_text_section->section->sh_link = symtab_section->index;
	rel_text_section->section->sh_info = text_section->index;

	elf->write("temp.o");

	elf->done();

	return 0;
}