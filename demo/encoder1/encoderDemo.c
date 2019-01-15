#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "vencoder.h"
#include <time.h>

typedef struct _globalInfo{
	VideoEncoder* pVEn;
	int codecType;		//video encoder engine's codec type
	unsigned char *uv_tmp_buffer;
	VencBaseConfig baseConfig;
	VencAllocateBufferParam bufferParam;
	VencInputBuffer inputBuffer;
	FILE *in_file;
	FILE *out_file;
	
}globalInfo_t;

#define INPUT_FILE_PATH "./1080p_yuv420sp.yuv"

#define aDebugPrint(fmt, args...) printf("%s(line:%d)\n"fmt"\n", __func__, __LINE__, ##args)


int yu12_nv12(unsigned int width, unsigned int height, unsigned char *addr_uv, unsigned char *addr_tmp_uv)
{
	unsigned int i, chroma_bytes;
	unsigned char *u_addr = NULL;
	unsigned char *v_addr = NULL;
	unsigned char *tmp_addr = NULL;

	chroma_bytes = width*height/4;

	u_addr = addr_uv;
	v_addr = addr_uv + chroma_bytes;
	tmp_addr = addr_tmp_uv;

	printf("Notes: before yu12_nv12\n");

	for(i=0; i<chroma_bytes; i++)
	{
		*(tmp_addr++) = *(u_addr++);
		*(tmp_addr++) = *(v_addr++);
	}

	memcpy(addr_uv, addr_tmp_uv, chroma_bytes*2);	

	printf("Notes: yu12_nv12 done.\n");
	return 0;
}


int global_info_init(globalInfo_t* p)
{
	int ret = 0;
	if(!p)
	{
		ret = -EINVAL;
		printf("Error: Init global info pointer failed.\n");
	}else
	{
		bzero(p, sizeof(globalInfo_t));
		p->pVEn = NULL;
		//1)
		p->codecType = VENC_CODEC_H264;
		//2) 
		p->baseConfig.nInputWidth= 1920;	//input image width, unit is pixel
		p->baseConfig.nInputHeight = 1080;
		
		p->baseConfig.nStride = 1920 * 3/2;	//1920;	//input image's row width in the memory, 16 - byte aligned
		
		p->baseConfig.nDstWidth = 1920;	//before encoder, after scaled, image's width, if it doesn't need scaled, keep it same as the src size
		p->baseConfig.nDstHeight = 1080;
		p->baseConfig.eInputFormat = VENC_PIXEL_YUV420SP;	//input image format
		//4)
		if(p->baseConfig.eInputFormat == VENC_PIXEL_YUV420SP)
		{
			p->uv_tmp_buffer = (unsigned char*)malloc(p->baseConfig.nInputWidth * p->baseConfig.nInputHeight *3 / 2);
			if(NULL == p->uv_tmp_buffer)
			{
				printf("Error: malloc uv_tmp_buffer fail\n");
			}
		}
		//3)
		p->bufferParam.nSizeY = p->baseConfig.nInputWidth * p->baseConfig.nInputHeight;
		p->bufferParam.nSizeC = p->baseConfig.nInputWidth * p->baseConfig.nInputHeight * 3 / 2;
		p->bufferParam.nBufferNum = 1;
		//5)
		p->in_file = fopen(INPUT_FILE_PATH, "r");
		p->out_file = NULL;
	}

	return ret;
}

void global_info_exit(globalInfo_t * p)
{
	if(!p)
	{
		if(!p->uv_tmp_buffer)
		{
			free(p->uv_tmp_buffer);
			p->uv_tmp_buffer = NULL;
		}
		fclose(p->in_file);
		bzero(p, sizeof(globalInfo_t));
		p = NULL;
	}
}

int readImgToInputBuffer(globalInfo_t* p)
{
	int ret = 0;
	unsigned int size1, size2;

	if(p->in_file)
	{
		//recalibration the file seek
		fseek(p->in_file, 0L, SEEK_SET);
		size1 = fread(p->inputBuffer.pAddrVirY, 1, p->baseConfig.nInputWidth * p->baseConfig.nInputHeight, p->in_file);
		size2 = fread(p->inputBuffer.pAddrVirC, 1, p->baseConfig.nInputWidth * p->baseConfig.nInputHeight/2, p->in_file);
		//recover the file seek
		fseek(p->in_file, 0L, SEEK_SET);
		
		if(p->baseConfig.eInputFormat == VENC_PIXEL_YUV420SP)
		{
			if(!p->uv_tmp_buffer) ret = -EIO;
			else ret = yu12_nv12(p->baseConfig.nInputWidth, p->baseConfig.nInputHeight, p->inputBuffer.pAddrVirC, p->uv_tmp_buffer);
		}

	}else
	{
		ret = -EINVAL;
		printf("Error: p->in_file pointer is NULL.\n");
	}

	
}


int main(int argc, char* argv[])
{
	int ret = 0;
	globalInfo_t gInfo;
	
	global_info_init(&gInfo);
	
	//1. Create a new video encoder, return a none NULL pointer if succeed
	aDebugPrint();
	gInfo.pVEn = VideoEncCreate(gInfo.codecType);
	if(!gInfo.pVEn)
	{
		ret = -EIO;
		printf("Error: Create video encoder device failed.\n");
		goto out_main;
	}else
	{
		aDebugPrint();
		//2. init the video encoder
		ret = VideoEncInit(gInfo.pVEn, &gInfo.baseConfig);
		if(ret)
		{
			printf("Error: video encoder init failed.\n");
			goto out_cve;
		}else
		{
			aDebugPrint();
			//3. alloc the input image buffer, if use the interface below, 
			// the buffer is provided by the encoder, not the ve, once more copy will done
			ret = AllocInputBuffer(gInfo.pVEn, &gInfo.bufferParam);
			if(ret)
			{
				printf("Error: allocInputBuffer failed.\n");
				goto out_unive;
			}else
			{
				aDebugPrint();
				//4. get one input buffer
				ret = GetOneAllocInputBuffer(gInfo.pVEn, &gInfo.inputBuffer);
				if(ret)
				{
					printf("Error: Get one alloc input buffer failed.\n");
					goto out_aib;
				}else
				{
					aDebugPrint();
					//5.0 read img, and transform if necessary
					ret = readImgToInputBuffer(&gInfo);
					//5.1 if we use cpu to move img from dram to this buffer, we need to garantee the dram data sync from cache, 
					// refresh cache to keep data sync to dram
					ret = FlushCacheAllocInputBuffer(gInfo.pVEn, &gInfo.inputBuffer);
					if(ret)
					{
						printf("Error: flush cache input buffer failed.\n");
						goto out_goaib;
					}else
					{
						aDebugPrint();
						//6. add img to encoder device
						ret = AddOneInputBuffer(gInfo.pVEn, &gInfo.inputBuffer);
						if(ret)
						{
							printf("Error: add img to encoder device failed.\n");
							goto out_goaib;
						}else
						{
							aDebugPrint();
							//7. encoder
							ret = VideoEncodeOneFrame(gInfo.pVEn);
							switch(ret)
							{
							case VENC_RESULT_OK:
								printf("VENC_RESULT_OK.\n");
								break;
							case VENC_RESULT_ERROR:
								printf("Error: VENC_RESULT_ERROR.\n");
								break;
							case VENC_RESULT_NO_FRAME_BUFFER:
								printf("Error: VENC_RESULT_NO_FRAME_BUFFER.\n");
								break;
							case VENC_RESULT_BITSTREAM_IS_FULL:
								printf("Error: VENC_RESULT_NO_FRAME_BUFFER\n");
								break;
							default:
								printf("Error: unknown VENC failed.\n");
								break;
							}
						}
					}
				}
			}
		}
	}

out_goaib:
	aDebugPrint();
	ReturnOneAllocInputBuffer(gInfo.pVEn, &gInfo.inputBuffer);
out_aib:
	aDebugPrint();
	ReleaseAllocInputBuffer(gInfo.pVEn);
out_unive:
	aDebugPrint();
	VideoEncUnInit(gInfo.pVEn);
out_cve:
	aDebugPrint();
	VideoEncDestroy(gInfo.pVEn);
out_main:
	aDebugPrint();
	global_info_exit(&gInfo);
	return ret;
}
