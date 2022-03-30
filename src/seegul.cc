#include <stdio.h>
#include <stdlib.h>
#include <seegul/seegul.h>
#include <cstring>
#include <iostream>
#include <algorithm>


Elf32_Ehdr * create_ehdr(){
    Elf32_Ehdr * header = (Elf32_Ehdr*)calloc(sizeof(Elf32_Ehdr), 1);

    if (header == NULL){
        return NULL;
    }


    header->e_ident[EI_MAG0] = 0x7f;
    header->e_ident[EI_MAG1] = 'E';
    header->e_ident[EI_MAG2] = 'L';
    header->e_ident[EI_MAG3] = 'F';
    // set the object to 32 bit
    header->e_ident[EI_CLASS] = ELFCLASS32;
    header->e_ident[EI_DATA] = ELFDATA2LSB;
    // set the file version
    header->e_ident[EI_VERSION] = EV_CURRENT;
    // set sysv abi
    header->e_ident[EI_OSABI] = ELFOSABI_SYSV;
    header->e_ident[EI_ABIVERSION] = ELFOSABI_NONE;
    header->e_ident[EI_PAD] = 0; // needs to be changed
    header->e_shentsize = sizeof(Elf32_Shdr);
    // we default to relocatable
    header->e_type = ET_REL;
    header->e_machine = EM_X86;
    header->e_version = EV_CURRENT;
    header->e_ehsize = sizeof(Elf32_Ehdr);
    header->e_phentsize = sizeof(Elf32_Phdr);
    header->e_phnum = 0;

    return header;
}

Elf32::Elf32(){

    eheader = create_ehdr();

    if (eheader == NULL){
        printf("Failed to make new elf");
        return;
    }

}

struct Elf32_section *Elf32::new_section(){
    struct Elf32_section * section = (struct Elf32_section*)calloc(sizeof(struct Elf32_section),1);
    Elf32_Shdr * section_h = (Elf32_Shdr*)calloc(sizeof(Elf32_Shdr),1);

    if (section == NULL || section_h == NULL){
        printf("Failed to create new section\n");
        return NULL;
    }
    section->section = section_h;
    section_h->sh_link = 0;

    sections.push_back(section);
    section->index = sections.size()-1;
    return section;
}


void Elf32::write(std::string file){
    FILE* fout;

    fout = fopen(file.c_str(), "wb");
    if (fout == NULL){
        printf("Failed to open file\n");
        return;
    }

    eheader->e_shnum = sections.size();
    eheader->e_shoff = sizeof(Elf32_Ehdr);

    int start_address_of_data = sizeof(Elf32_Shdr) * sections.size();
    int current_address_of_data = start_address_of_data;

    fwrite(eheader,sizeof(Elf32_Ehdr), 1, fout); // write the header at the start of the file;

    int offset_index = 1;

    for (auto section : sections){
        if (section->data_sz == 0){
            fwrite(section->section, sizeof(Elf32_Shdr), 1, fout);
            offset_index++;
        }
        else{

            section->section->sh_addr = current_address_of_data;
            section->section->sh_size = section->data_sz;
            section->section->sh_offset = current_address_of_data+sizeof(Elf32_Ehdr);

            current_address_of_data += section->data_sz;

            fwrite(section->section, sizeof(Elf32_Shdr), 1, fout);

            offset_index++;


        }
    }

    for (auto section : sections){
        if (section->data_sz != 0){
            fwrite(section->data, section->data_sz, 1, fout);
        }
    }

}


void Elf32::done(){
    for (auto section : sections){
        free(section->section);
        free(section);
    }
}
SymTab32::SymTab32(){}
Elf32_Sym * SymTab32::new_symbol(int name,unsigned char info, int section_index){
    Elf32_Sym *entry = (Elf32_Sym*)calloc(sizeof(Elf32_Sym),1);
    if (entry == NULL){
        return NULL;
    }
    entry->st_name = name;
    entry->st_info = info;
    entry->st_shndx = section_index;
    symtab.push_back(entry);
    return entry;
}

void SymTab32::add_symtab(Elf32_section*section){
    Elf32_Sym* _symtab =  (Elf32_Sym*)malloc(sizeof(Elf32_Sym)*symtab.size());
    int idx = 0;
    for (auto entry: symtab){
        memcpy((void*)&_symtab[idx], entry, sizeof(Elf32_Sym));
        free(entry);
        idx++;
    }
    section->data = _symtab;
    section->data_sz = sizeof(Elf32_Sym)*symtab.size();
    section->section->sh_entsize = sizeof(Elf32_Sym);
    section->section->sh_addralign = 4;
    section->section->sh_type = SHT_SYMTAB;
}

Elf32_Rel * RelTab32::new_relocation(int offset, int info){
    Elf32_Rel * entry = (Elf32_Rel*)malloc(sizeof(Elf32_Rel));
    reltab.push_back(entry);
    entry->r_info = info;
    entry->r_offset = offset;
    return entry;
}
void RelTab32::add_reltab(Elf32_section * section){
    Elf32_Rel* _reltab =  (Elf32_Rel*)malloc(sizeof(Elf32_Rel)*reltab.size());
    int idx = 0;
    for (auto entry : reltab){
        memcpy((void*)&_reltab[idx], entry, sizeof(Elf32_Rel));
        free(entry);
        idx++;
    }
    section->data = _reltab;
    section->data_sz = sizeof(Elf32_Rel)*reltab.size();
    section->section->sh_entsize = sizeof(Elf32_Rel);
    section->section->sh_addralign = 4;
    section->section->sh_type = SHT_REL;
    section->section->sh_entsize = sizeof(Elf32_Rel);
}
void StrTab32::new_string(std::string string){
    strtab.push_back(string);
}
int StrTab32::get_string(std::string str){
    int strtab_index = 1;
    bool found = false;
    for (auto _str: strtab){
        if (str == _str)
        {
            found = true;
            break;
        }
        strtab_index += _str.length()+1;
    }
    if (!found){
        return 0;
    }
    return strtab_index;
}
void StrTab32::add_strtab(Elf32_section * section){
    int strtab_size = 0;
    int index = 0;
    for (auto str : strtab){
        strtab_size += str.length()+1;
    }
    strtab_size += 1;
    unsigned char *_strtab = (unsigned char*)malloc(strtab_size);
    _strtab[index] = 0;
    index++;
    for (auto str : strtab){
        for (int v = 0; v < str.length(); v++){
            _strtab[index] = str[v];
            index++;
        }
        _strtab[index] = 0;
        index++;
    }
    section->data_sz = strtab_size;
    section->data = _strtab;
    section->section->sh_type = SHT_STRTAB;
    section->section->sh_addralign = 1;
}