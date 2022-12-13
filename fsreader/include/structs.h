//
// Created by vivi on 11.12.22.
//
#ifndef TEMPLATE_STRUCTS_H
#define TEMPLATE_STRUCTS_H

#include <cstdint>

#pragma pack(push, 1)

// DOS 3.00, 3.20 use slightly incompatible BPB here, future systems use DOS 3.31 BPB
//! See also http://jdebp.info/FGA/bios-parameter-block.html
struct BPB_DOS3x0_FAT_t {
// For reference:
// BPB 3.00
    uint16_t BPB_HiddSec;		// 0x01C; 2 bytes. Sectors on disk before this partition. 0 for non-partitioned disks.
    // Do not use if 0x013 == 0.
// BPB 3.20
    uint16_t BPB_TotHiddSec;	// 0x01E; Total logical sectors including hidden sectors. Do not use if 0x013 == 0.
    uint8_t  padding[58];
};

// Extended Boot Record or Extended BIOS Parameter Block (EBPB) for FAT12/16; DOS 4.00+; OS/2 1.00+
// (+ part of BPB, incompartible with DOS 3.20-)
struct EBPB_FAT_t {
    uint32_t BPB_HiddSec;       // 0x01C; 4 bytes. Sectors on disk before this partition -- the LBA of the beginning of the partition.
    //		  0 for non-partitioned disks. Do not use if 0x013 == 0.
    uint32_t BPB_TotSec32;		// 0x020; if more than 65535, then  0x013 == 0
    //----------------------------------------
    uint8_t  BS_DrvNum;			// 0x024; 0x00-0x7E for removable, 0x80-0xFE -- fixed disks. 0x7F and 0xFF for boot ROMs and so on.
    //		  For DOS 3.2 to 3.31 -- similar entry at 0x1FD.
    uint8_t  BS_ReservedOrNT;	// 0x025; For NT: bit 1: physical error, bit 0: was not properly unmounted, check it on next boot.
    uint8_t  BS_BootSig;		// 0x026; Extended boot signature. 0x29 -- contains all 3 next fields, (DOS 4.0, OS/2 1.2);
    //		  0x28 -- only BS_VolID
    uint32_t BS_VolID;			// 0x027; Volume serial number
    uint8_t  BS_VolLab[11];		// 0x02B; Partition volume label, should be padded by spaces. Should be equal to vol. label, but frequently is not
    char     BS_FilSysType[8];  // 0x036; FS Type, "FAT     ", "FAT12   ", "FAT16   ", "FAT32   ",
    //		  "The spec says never to trust the contents of this string for any use", but see: http://jdebp.info/FGA/determining-fat-widths.html
    uint8_t	 padding[62-34];
}; // Sizeof 0x22 = 34 + padding, sizeof tFAT_EBPB_FAT32 == 0x3E == 62

struct FAT_boot_sector_t {
    //--------------------------// Common part for the DOS 2.0+
    uint8_t BS_jmpBoot[3];        // 0x000
    uint8_t BS_OEMName[8];        // 0x003
    uint16_t BPB_bytesPerSec;    // 0x00B
    uint8_t BPB_SecPerClus;    // 0x00D
    uint16_t BPB_RsvdSecCnt;    // 0x00E
    uint8_t BPB_NumFATs;        // 0x010, often 1 or 2
    uint16_t BPB_RootEntCnt;    // 0x011; 0 for FAT32, but check at 0x042 signature to be 0x29 or 0x28.
    //		  MS-DOS supports 240 max for FDD and 512 for HDD
    uint16_t BPB_TotSec16;      // 0x013; 0 if more than 65535, then val in. BPB_TotSec32 at 0x020 is used. 0 for FAT32
    uint8_t BPB_MediaDescr;    // 0x015; same as 1-st byte of FAT. See https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system#FATID
    uint16_t BPB_SectorsPerFAT; // 0x016; FAT12/16, 0 for FAT32, it uses val at 0x024.
    //--------------------------// End of common part for the DOS 2.0+.
    uint16_t BPB_SecPerTrk;        // 0x018; If 0 -- reserved, not used. DOS 3.00+
    uint16_t BPB_NumHeads;        // 0x01A; DOS up to 7.10 have bug here, so 255 heads max. 0 -- reserved, not used. DOS 3.00+
    //--------------------------// Relies here on correct type punning. GCC supports it: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#Type%2Dpunning
    // Didn't found (yet?) in MSVC docs, but it's headers use type punning heavily. Clang -- unknown.
    union {
        BPB_DOS3x0_FAT_t BPB_DOS3x0;
        EBPB_FAT_t EBPB_FAT;
    };
    uint8_t remaining_part[420];
    uint16_t signature;         // 0x1FE; 0xAA55 (Little endian: signature[0] == 0x55, signature[1] == 0xAA)
};


#pragma pack(pop)

#pragma pack(push, 1)
struct FAT_dir_entry {
    uint8_t file_name[11];
    uint8_t attributes;
    uint8_t reserved;
    uint8_t creat_time_ms;
    uint16_t creat_time;
    uint16_t creat_date;
    uint16_t access_date;
    uint16_t high_bytes_first_clus;
    uint16_t modif_time;
    uint16_t modif_date;
    uint16_t low_bytes_first_clus;
    uint32_t file_size;
};
#pragma pack(pop)


#endif //TEMPLATE_STRUCTS_H
