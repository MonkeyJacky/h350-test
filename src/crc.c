#include "crc.h"
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>

static unsigned int*  pcrc_table = NULL;
int crc_init(void)
{
    crc_deinit();
    pcrc_table = (unsigned int *)malloc(sizeof(int)*8*256);
    if(NULL == pcrc_table){
        printf("crc malloc error!\n");
		return 0;
    }

    int i, j;
    unsigned int c;
    for (i=0;i<256;i++)
    {
        for (c=i,j=0;j<8;j++)
            c = (c & 1) ? (c>>1)^0xEDB88320L : (c>>1);
        pcrc_table[i] = c;
    }
    return 1;
}

void crc_deinit(void)
{
    if(NULL != pcrc_table){
        free(pcrc_table);
        pcrc_table = NULL;
    }
}

unsigned int calc_crc32(unsigned int start_crc, unsigned char *addr, int size)
{
	int i;

	if(0 == crc_init())
		return 0;;


	for (i=0; i<size; i++)
		start_crc = pcrc_table[(unsigned char)start_crc ^ addr[i]] ^ (start_crc >> 8);

	return(start_crc);
}

int real_progress_cartoon_step = 0;

int check_file_crc(char *filename)
{
    if(0 == crc_init())
		return 0;;

#if 0
	if(NULL == pcrc_table){
        crc_deinit();
        return 0;
    }
#endif

	FILE *fp = fopen(filename, "rb");
	if(NULL == fp)
	{
		printf("CRC file open error!\n");
		crc_deinit();
		return 0;
	}

    unsigned char * temp = (unsigned char *)malloc(4096);
    if(NULL == temp){
		printf("CRC malloc error!\n");
        crc_deinit();
        return 0;
    }

    int start_crc = 0;
    int len = 0;

    do{
        len = fread(temp, 1, 4096, fp);
		if(0 == len)
			return 0;
        start_crc = calc_crc32(start_crc, temp, len);
    }while(4096 == len);

    free(temp);
    fclose(fp);
    crc_deinit();
    
	if(0 == start_crc){
        return 1;
    }

    return 0;
}

#if 1
int check_file_crc_new(const char *filename,const char *filename_crc)
{
	int file_size,crc_size;
	char * file_buff = NULL,*crc_buff = NULL;
	if(0 == crc_init())
		return 0;;

	//open config file
	FILE *fp = fopen(filename, "rb");
	if(NULL == fp)
	{
		printf("CRC file open error!\n");
		crc_deinit();
		return 0;
	}
	fseek(fp,0,SEEK_END);
	file_size = ftell(fp);
	file_buff = (char*)malloc(sizeof(char)*file_size+4);
	memset(file_buff,'\0',file_size+4);
	fseek(fp,0,SEEK_SET);
	fread(file_buff,1,file_size,fp);
	
	//open crc file
	FILE *fp_crc = fopen(filename_crc, "rb");
	if(NULL == fp_crc)
	{
		printf("CRC file open error!\n");
		crc_deinit();
		return 0;
	}
	fseek(fp_crc,0,SEEK_END);
	crc_size = ftell(fp_crc);
	if (crc_size != 4){
		printf("CRC size error!\n");
		remove(filename_crc);
		crc_deinit();
		return 0;
	}
	crc_buff = (char*)malloc(sizeof(char)*crc_size);
	memset(crc_buff,'\0',crc_size);
	fseek(fp_crc,0,SEEK_SET);
	fread(crc_buff,1,crc_size,fp_crc);

	memcpy(file_buff+file_size,crc_buff,crc_size);

	int start_crc = 0;
	int i = 0;
	int len_z = 0,len_y;
	len_z = (file_size + 4)/4096;
	len_y = (file_size + 4)%4096;
	printf("len_z  = %d len_y = %d\n",len_z,len_y);
	if (len_z > 0){
		for (i = 0 ;i < len_z;i++)
			start_crc = calc_crc32(start_crc, file_buff + i * 4096, 4096);
		if (len_y > 0)
			start_crc = calc_crc32(start_crc, file_buff + i * 4096, len_y);
	}else{
		start_crc = calc_crc32(start_crc, file_buff, len_y);
	}

	free(crc_buff);
	free(file_buff);
	fclose(fp);
	fclose(fp_crc);
	crc_deinit();

	if(0 == start_crc){
		return 1;
	}
	return 0;
}
#endif

 void add_crc_file(const char *filename)
{
	FILE *file;
	char *file_buff=NULL;
	int file_size;
	file = fopen(filename, "r+");
	fseek(file,0L,SEEK_END);
	file_size = ftell(file);
	file_buff = (char*)malloc(sizeof(char)*file_size);
	memset(file_buff,'\0',file_size);
	fseek(file,0L,SEEK_SET);
	fread(file_buff,1,file_size,file);

	//cal crc
	int start_crc = 0;
	start_crc = calc_crc32(start_crc, (unsigned char *)(file_buff),file_size);
	//write crc
	fwrite(&start_crc, sizeof(int), 1, file);
	//fprintf(file,"%x\n",start_crc);	

	fflush(file);
	fsync(fileno(file));
	fclose(file);
	if(file_buff) 
		free(file_buff);
}
