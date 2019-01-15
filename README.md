### 20190115 
by Henry
#### Problems for rebuilding the proj

##### Num1: Segment fault for encoder demo app from AllWinner
###### issues ref:
[1.] [Vencoder demo does not work - segfault / missing code ](https://github.com/allwinner-zh/media-codec/issues/9)

##### solution:
added AllWinner no GPL libs to A83T /usr/lib

|libs name
|---
| libcedar_plugin_venc.so
| libcedar_plugin_vd_others.so

##### Num2: h264 arguments problems
```
warning: CedarX <vencoder.c:339>: venc_ctx->curEncInputbuffer.pAddrPhyY: (nil), maybe not right
```
##### solution:
It's caused by closing void* MemAdapterGetPhysicAddressCpu(void* pVirtualAddress), use it


##### Num3: h264enc wait interrupt timeout
###### ref forum: 
[1.] [CC-A80 linux cedarx libs](http://www.cubie.cc/forum.php?mod=viewthread&tid=4194)

```
error  : awplayer <h264/h264enc.c:1518>: h264 encoder wait interrupt overtime
error  : CedarX <BitstreamManager.c:275>: nUnReadFrameNum == 0.
error  : CedarX <BitstreamManager.c:322>: no valid frame.
```

##### solution:

try to add individual ion module for the proj and encoder demo, but still have some problems: 

Error: VideoEncodeOneFrame() return -1



### 20181128
Add decode demo from <wangjinwu@egova.com.cn> by Henry, but DecodeVideoStream return ret:5(It's libs bug or wrong configuration for streamInfo in demo app)

### 20181112
Add ion files by Henry <haoxiansen@zhitongits.com.cn>, not test for decoder

### 20181105
Based on: CedarX-12.06.2015 --- 
The new CedarX library from Allwinner with Chinese documentation and example C application...

#### Notes:
* 1. using ion mem instead of VE own mem for VPU(VE), here is the reason from Linux kernel codes: 

```bash
	/*cedar_ve.c*/
    case IOCTL_GET_ENV_INFO:
	{
		struct cedarv_env_infomation env_info;
		env_info.phymem_start = 0; // do not use this interface ,ve get phy mem form ion now
		env_info.phymem_total_size = 0;//ve_size = 0x04000000 
		env_info.address_macc = 0;
		if (copy_to_user((char *)arg, &env_info, sizeof(struct cedarv_env_infomation)))
			return -EFAULT;
	}
```

### About Current CedarX Proj

##### 1. How to compile the proj
```bash
cd src/
vi execAutoCmpCedarX.sh
//1. config your Host PC env "PATH" for "CROSS_COMPILE"
//2. modify your "TOOLCHAIN_PATH" or something else
./execAutoCmpCedarX.sh
```
##### 2. Where is the compiled libs
```bash
src/install/lib/
```
##### 3. How to use ion or modified other configuration
<html>
Modified <strong>src/base/include/Cdxconfig.h</strong>, rebuild the libs
</html>

```bash 
#define CONFIG_MEMORY_DRIVER    OPTION_MEMORY_DRIVER_ION	//ve
```

##### 4. How to get more debug msg or close it
<html>
Modified <strong>src/base/include/log.h</strong> , rebuild the libss
</html>

```bash
#define loge(fmt, arg...) \
	    do { \
			        AWLOG(LOG_LEVEL_ERROR, "\033[40;31m"fmt"\033[0m", ##arg) ; \
			        CdxBTDump(); \
			    } while (0)
    
#define logw(fmt, arg...) AWLOG(LOG_LEVEL_WARNING, fmt, ##arg)
#define logi(fmt, arg...) AWLOG(LOG_LEVEL_WARNING, fmt, ##arg)
#define logd(fmt, arg...) AWLOG(LOG_LEVEL_WARNING, fmt, ##arg)
#define logv(fmt, arg...) AWLOG(LOG_LEVEL_WARNING, fmt, ##arg)

```
### refs:
##### 1. docs
[1.] [sunxi-A83T](http://linux-sunxi.org/A83T)

[2.] [Video  Engine (include opensource data)](http://linux-sunxi.org/Video_Engine)

[3.] [CedarX (include registers/official data)](http://linux-sunxi.org/CedarX)

##### 2. codes
[1.0] [A83T media-codec (original)](https://github.com/allwinner-zh/media-codec)

[1.1] [A83T CedarX (new)](https://github.com/fsebentley/CedarX-12.06.2015)

[1.3] [A83T encoder (current & include ion)](https://github.com/andrewz1/cedarx_encoder)

[2.] [tina proj For Android](https://github.com/tinalinux/package)

[3.] [kernel mem](https://github.com/juanfont/linux-sunxi/commit/e8937b50bc749cf1069b6881e0189831381d342b)




# BoxCodecEngine

智泊盒V3视频编解码引擎

