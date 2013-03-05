#ifndef __CRC_H__
#define __CRC_H__

#define YOHU_CRC_FILE "/usr/etc/yohu_desktop/crc_tmp.ini"

#ifdef __cplusplus
extern "C" {
#endif

int  crc_init(void);

void crc_deinit(void);

unsigned int calc_crc32(unsigned int start_crc, unsigned char *addr, int size);

int check_file_crc(char* filename);

void add_crc_file(const char *filename);
#ifdef __cplusplus
}
#endif

#endif // __CRC_H__


