#ifndef _FileSystem_H
#define _FileSystem_H


//------- Public Constants  -----------------------------------------
#define Disk_Error (0xF0)
#define No_Disk_Error (0)
#define more_entries (0x8000)   
#define no_entry_found (0x80000000)  // msb set to indicate error
#define directory_bit  (0x10000000)  // lsb of first nibble (bit28)

#define MBR_RelSectors (0x01C6)
#define BPB_BytesPerSector (0x0B)
#define BPB_SectorsPerCluster (0x0D)
#define BPB_RsvdSectorCount (0x0E)
#define BPB_NumberFATs (0x10)
#define BPB_RootEntryCount (0x11)
#define BPB_TotalSectors16 (0x13)
#define BPB_FATsize16 (0x16)
#define BPB_TotalSectors32 (0x20)
#define BPB_FATsize32 (0x24)
#define BPB_RootCluster (0x2C)


// ------ Public function prototypes -------------------------------
uint8_t read_value_8(uint16_t offset, uint8_t * array_name);

uint16_t read_value_16(uint16_t offset, uint8_t * array_name);

uint32_t read_value_32(uint16_t offset, uint8_t * array_name);

uint8_t mount_drive(uint8_t * array_name);

uint32_t first_sector (uint32_t Cluster_num);

uint32_t find_next_clus(uint32_t Cluster_num, uint8_t * array_name);

uint8_t open_file(uint32_t Cluster, uint8_t * array_in);


#endif