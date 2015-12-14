#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "stm32f4xx_crc.h"

/***************************************
copy
�����ļ����̵�ʵ��
***************************************/
#define BUFFER_SIZE  (1024*1024)
int copyfile(char * src,int len, char * dest)
{
	FILE *pdest = fopen(dest, "wb"); //ע�⣬���κ�"w"ģʽ���ļ������Ѿ����ڸ��ļ���������գ�Ȼ���ٿ�ʼ��������ļ���ȥ
	if (pdest == NULL)
	{
		printf("open %s error !\n", dest);
		return -1;
	}
	int writebytes=0;
	writebytes=fwrite(src, sizeof(char), len, pdest);
	fclose(pdest);

	return writebytes;
}

uint32_t get_uint32(uint8_t *data, uint8_t len, uint8_t add)
{
	uint32_t result = 0;

	for (uint8_t i = 0; i < 4; i++)
	{
		if (i < len)
		{
			result |= data[i] << (i * 8);
		}
		else
		{
			result |= add << (i * 8);
		}
	}

	return result;
}
uint32_t get_u8_crc(uint8_t *data, uint32_t len)
{
	uint32_t crc;
	uint32_t _4_len;
	uint32_t WORD_len;
	uint32_t last_word;

	CRC_enable();
	CRC_ResetDR();

	_4_len = len % sizeof(uint32_t);
	WORD_len = len / sizeof(uint32_t);

	crc = CRC_CalcBlockCRC((uint32_t *)data, WORD_len);
	if (0 != _4_len)
	{
		last_word = get_uint32(&data[len - _4_len], _4_len, 0xff);
		crc = CRC_CalcBlockCRC(&last_word, 1);
	}
	CRC_disable();

	return crc;
}
int main(int argc, char **argv)
{
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;
	char inputname[100];
	char newfilename[200];
	uint32_t filecrc=0;


	if (argv[1]==NULL)
	{
		printf("Please input file name\r\n");
		//exit(101);
	}

	memset(inputname, 0, sizeof(inputname));
	strcpy(inputname,"bongX2.bin");//argv[1]
	/* ��Ҫһ��byte��©�ض��������ļ���ֻ�ܲ��ö����Ʒ�ʽ�� */
	pFile = fopen(inputname, "rb");
	if (pFile == NULL)
	{
		fputs("File error", stderr);
		exit(1);
	}

	/* ��ȡ�ļ���С */
	fseek(pFile, 0, SEEK_END);
	lSize = ftell(pFile);
	rewind(pFile);

	/* �����ڴ�洢�����ļ� */
	buffer = (char*)malloc(sizeof(char)*lSize);
	if (buffer == NULL)
	{
		fputs("Memory error", stderr);
		exit(2);
	}

	/* ���ļ�������buffer�� */
	result = fread(buffer, 1, lSize, pFile);
	if (result != lSize)
	{
		fputs("Reading error", stderr);
		exit(3);
	}
	/* ���������ļ��Ѿ���buffer�У����ɱ�׼�����ӡ���� */
	//printf("%s", buffer);

	/* ������ʾ���ر��ļ����ͷ��ڴ� */
	fclose(pFile);
	filecrc= get_u8_crc(buffer, lSize);
	printf("%s size:%d crc:%d\r\n", inputname, lSize, filecrc);
	memset(newfilename,0,sizeof(newfilename));
	sprintf(newfilename,"%s-crc-%d.bin", inputname,filecrc);
	int newfilesize;
	newfilesize=copyfile(buffer, lSize, newfilename);
	printf("create:%s size:%d\r\n",newfilename, newfilesize);
	free(buffer);

	return 0;
}
