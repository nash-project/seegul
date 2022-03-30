#pragma once

enum EI{
        EI_MAG0 = 0,
        EI_MAG1,
        EI_MAG2,
        EI_MAG3,
        EI_CLASS,
        EI_DATA,
        EI_VERSION,
        EI_OSABI,
        EI_ABIVERSION,
        EI_PAD,
};

#define ELF32_ST_BIND(info)          ((info) >> 4)
#define ELF32_ST_TYPE(info)          ((info) & 0xf)
#define ELF32_ST_INFO(bind, type)    (((bind)<<4)+((type)&0xf))

#define STB_LOCAL 0
#define STB_GLOBAL 1
#define STB_WEAK 2
#define STB_LOOS 10
#define STB_HIOS 12
#define STB_LOPROC 13
#define STB_HIPROC 15

#define STT_NOTYPE 0
#define STT_OBJECT 1
#define STT_FUNC 2
#define STT_SECTION 3
#define STT_FILE 4
#define STT_COMMON 5
#define STT_TLS 6
#define STT_LOOS 10
#define STT_HIOS 12
#define STT_LOPROC 13
#define STT_SPARC_REGISTER 13
#define STT_HIPROC 15

#define STV_DEFAULT 0
#define STV_INTERNAL 1
#define STV_HIDDEN 2
#define STV_PROTECTED 3
#define STV_EXPORTED 4
#define STV_SINGLETON 5
#define STV_ELIMINATE 6

#define SHN_UNDEF 0
#define SHN_LORESERVE 0xff00
#define SHN_LOPROC 0xff00
#define SHN_BEFORE 0xff00
#define SHN_AFTER 0xff01
#define SHN_HIPROC 0xff1f
#define SHN_ABS 0xfff1
#define SHN_COMMON 0xfff2
#define SHN_HIRESERVE 0xffff 

#define ELFOSABI_NONE           0       //No extensions or unspecified
#define ELFOSABI_HPUX           1       //Hewlett-Packard HP-UX
#define ELFOSABI_NETBSD         2       //NetBSD
#define ELFOSABI_LINUX          3       //Linux
#define ELFOSABI_SOLARIS        6       //Sun Solaris
#define ELFOSABI_AIX            7       //AIX
#define ELFOSABI_IRIX           8       //IRIX
#define ELFOSABI_FREEBSD        9       //FreeBSD
#define ELFOSABI_TRU64          10      //Compaq TRU64 UNIX
#define ELFOSABI_MODESTO        11      //Novell Modesto
#define ELFOSABI_OPENBSD        12      //Open BSD
#define ELFOSABI_OPENVMS        13      //Open VMS
#define ELFOSABI_NSK            14      //Hewlett-Packard Non-Stop Kernel

#define SHF_WRITE       0x1
#define SHF_ALLOC       0x2
#define SHF_EXECINSTR   0x4
#define SHF_MERGE       0x10
#define SHF_STRINGS     0x20
#define SHF_INFO_LINK   0x40
#define SHF_LINK_ORDER  0x80
#define SHF_OS_NONCONFORMING    0x100
#define SHF_GROUP       0x200
#define SHF_TLS         0x400
#define SHF_MASKOS      0x0ff00000
#define SHF_MASKPROC    0xf0000000

#define ELFCLASSNONE 0
#define ELFCLASS32 1
#define ELFCLASS64 2

#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

#define ELFOSABI_SYSV 0
#define ELFOSABI_HPUX 1
#define ELFOSABI_STANDALONE 255

#define ET_NONE 0
#define ET_REL 1 // relocatable file
#define ET_EXEC 2 // executable file
#define ET_DYN 3 // shared object file
#define ET_CORE 4 // core file
#define ET_LOOS 0xfe00 // operating system-specific
#define ET_HIOS 0xfeff // operating system specific
#define ET_LOPROC 0xff00 // processor-specific
#define ET_HIPROC 0xffff // processor-specific

#define SHT_NULL        0
#define SHT_PROGBITS    1
#define SHT_SYMTAB      2
#define SHT_STRTAB      3
#define SHT_RELA        4
#define SHT_HASH        5
#define SHT_DYNAMIC     6
#define SHT_NOTE        7
#define SHT_NOBITS      8
#define SHT_REL         9
#define SHT_SHLIB       10
#define SHT_DYNSYM      11
#define SHT_INIT_ARRAY  14
#define SHT_FINI_ARRAY  15
#define SHT_PREINIT_ARRAY       16
#define SHT_GROUP       17
#define SHT_SYMTAB_SHNDX        18
#define SHT_LOOS        0x60000000
#define SHT_HIOS        0x6fffffff
#define SHT_LOPROC      0x70000000
#define SHT_HIPROC      0x7fffffff
#define SHT_LOUSER      0x80000000
#define SHT_HIUSER      0xffffffff

#define EM_X86 0x03
#define EM_AMD_X86_64 0x3E

#define EV_NONE 0
#define EV_CURRENT 1

#define EI_NIDENT 16

#define ELF32_R_SYM(info)             ((info)>>8)
#define ELF32_R_TYPE(info)            ((unsigned char)(info))
#define ELF32_R_INFO(sym, type)       (((sym)<<8)+(unsigned char)(type))

enum RtT_Types {
        R_386_NONE              = 0, // No relocation
        R_386_32                = 1, // Symbol + Offset
        R_386_PC32              = 2  // Symbol + Offset - Section Offset
};


typedef unsigned int Elf32_Addr;
typedef unsigned short Elf32_Half;
typedef unsigned int Elf32_Off;
typedef signed int Elf32_Sword;
typedef unsigned int Elf32_Word;

typedef struct{ // elf file header
        unsigned char   e_ident[EI_NIDENT];
        Elf32_Half      e_type;
        Elf32_Half      e_machine;
        Elf32_Word      e_version;
        Elf32_Addr      e_entry;
        Elf32_Off       e_phoff;
        Elf32_Off       e_shoff;
        Elf32_Word      e_flags;
        Elf32_Half      e_ehsize;
        Elf32_Half      e_phentsize;
        Elf32_Half      e_phnum;
        Elf32_Half      e_shentsize;
        Elf32_Half      e_shnum;
        Elf32_Half      e_shstrndx;
} Elf32_Ehdr;

typedef struct { // program header
        Elf32_Word      p_type;
        Elf32_Off       p_offset;
        Elf32_Addr      p_vaddr;
        Elf32_Addr      p_paddr;
        Elf32_Word      p_filesz;
        Elf32_Word      p_memsz;
        Elf32_Word      p_flags;
        Elf32_Word      p_align;
} Elf32_Phdr;

typedef struct{ // section header
	Elf32_Word	sh_name;
	Elf32_Word	sh_type;
	Elf32_Word	sh_flags;
	Elf32_Addr	sh_addr;
	Elf32_Off	sh_offset;
	Elf32_Word	sh_size;
	Elf32_Word	sh_link;
	Elf32_Word	sh_info;
	Elf32_Word	sh_addralign;
	Elf32_Word	sh_entsize;
} Elf32_Shdr;

typedef struct {
        Elf32_Word      st_name;
        Elf32_Addr      st_value;
        Elf32_Word      st_size;
        unsigned char   st_info;
        unsigned char   st_other;
        Elf32_Half      st_shndx;
} Elf32_Sym;


typedef struct {
        Elf32_Addr              r_offset;
        Elf32_Word              r_info;
} Elf32_Rel;
 
typedef struct {
        Elf32_Addr              r_offset;
        Elf32_Word              r_info;
        Elf32_Sword             r_addend;
} Elf32_Rela;