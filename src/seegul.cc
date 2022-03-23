#include <stdio.h>
#include <stdlib.h>
#include <seegul/seegul.h>


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


/*
int main()
{
    FILE * fout;

    Elf32_Ehdr *elf_header;
    Elf32_Shdr *NULL_Section; // this should all be null
    Elf32_Shdr *strtab_Section;
    Elf32_Shdr *data_section;

    printf("%d\n", strtab_size);
    fout = fopen("output.elf", "wb");

    if (fout == NULL){
        printf("Failed to open output.elf\n");
        return 1;
    }

    if ((elf_header = create_ehdr()) == NULL){
        printf("Failed to create new header\n");
        return 1;
    }

    if ((NULL_Section = create_shdr()) == NULL){
        printf("Failed to create new section header\n");
        return 1;
    }
    if ((strtab_Section = create_shdr()) == NULL){
        printf("Failed to create new section header\n");
        return 1;
    }
    if ((data_section = create_shdr()) == NULL){
        printf("Failed to create new section header\n");
        return 1;
    }
    elf_header->e_shstrndx = strtab_index;    // Point to the shstrtab section
    elf_header->e_shnum = section_n;
    elf_header->e_shoff = sizeof(Elf32_Ehdr);

    strtab_Section->sh_name = 11;
    strtab_Section->sh_type = SHT_STRTAB;
    strtab_Section->sh_offset = ((sizeof(Elf32_Shdr))*2)+sizeof(Elf32_Ehdr);
    strtab_Section->sh_addr = sizeof(Elf32_Shdr)*section_n;
    strtab_Section->sh_size = strtab_size;


    data_section->sh_name = 19;
    strtab_Section->sh_offset = ((sizeof(Elf32_Shdr))*3)+sizeof(Elf32_Ehdr);


    fwrite(elf_header, sizeof(Elf32_Ehdr), 1, fout);
    fwrite(NULL_Section, sizeof(Elf32_Shdr), 1, fout);
    fwrite(strtab_Section, sizeof(Elf32_Shdr), 1, fout);
    fwrite(data_section, sizeof(Elf32_Shdr), 1, fout);
    fwrite(strtab, strtab_size, 1, fout);

    return 0;
}*/


Elf::Elf(){

    eheader = create_ehdr();

    if (eheader == NULL){
        printf("Failed to make new elf");
        return;
    }

}

struct Elf_section *Elf::new_section(){
    struct Elf_section * section = (struct Elf_section*)calloc(sizeof(struct Elf_section),1);
    Elf32_Shdr * section_h = (Elf32_Shdr*)calloc(sizeof(Elf32_Shdr),1);

    if (section == NULL || section_h == NULL){
        printf("Failed to create new section\n");
        return NULL;
    }
    section->section = section_h;
    section_h->sh_link = 0;

    sections.push_back(section);
    return section;
}


void Elf::write(std::string file){
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


void Elf::done(){
    for (auto section : sections){
        free(section->section);
        free(section);
    }
}