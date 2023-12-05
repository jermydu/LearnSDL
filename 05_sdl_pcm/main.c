#include <stdio.h>
#include <SDL.h>

//每次读取2帧数据 一帧1024个采样点 2 通道 16bit
const int PCM_BUFFER_SIZE = 1024*2*2*2;

//音频PCM数据缓存
static Uint8* g_pAudioBuf = NULL;
//当前读取pos
static Uint8* g_pAudioPos= NULL;
//缓存结束位置
static Uint8* g_pAudioEnd = NULL;

//音频回调函数
void OnFillAudioPcm(void* udata,Uint8* stream,int len)
{
    //清空缓存
    SDL_memset(stream,0,len);
    //读取完毕
    if(g_pAudioPos >= g_pAudioEnd)
    {
        return;
    }

    //读取剩余的长度
    int remainBufferLen = g_pAudioEnd - g_pAudioPos;
    //读取的数据够了就读取预设长度，不够的时候剩余多少就读取多少
    len = len < remainBufferLen ? len : remainBufferLen;
    //拷贝数据到stream并调整音量
    SDL_MixAudio(stream,g_pAudioPos,len,SDL_MIX_MAXVOLUME/10);
    //移动缓存指针
    g_pAudioPos += len;
}

int WinMain()
{
    printf("SDL_playPCM\n");

    //ffplay .\testvideo1_48000_2_f32le.pcm -ar 48000 -ac 2 -f f32le
    const char* pAudioFilePath = "testvideo1_48000_2_f32le.pcm";
    //每次缓存长度
    size_t readBufferLen = 0;

    //初始化SDL
    SDL_Init(SDL_INIT_AUDIO);

    FILE* pAudioFile = fopen(pAudioFilePath,"rb");
    if(!pAudioFile)
    {
        printf("open audio file error\n");
        goto _FAIL;
    }

    g_pAudioBuf = (uint8_t*)malloc(PCM_BUFFER_SIZE);
    //音频参数设置
    SDL_AudioSpec spec;
    spec.freq = 48000;              //采样率
    spec.format = AUDIO_F32SYS;     //采样格式
    spec.channels = 2;              //通道
    spec.samples = 1024;            //每次读取的采样数量，多久产生一次回调
    spec.callback = OnFillAudioPcm; //回调函数
    spec.userdata = NULL;

    //打开音频设备
    if(SDL_OpenAudio(&spec, NULL))
    {
        printf("open audio device error:%s\n",SDL_GetError());
        goto _FAIL;
    }
    //播放
    SDL_PauseAudio(0);
    int dataCount = 0;   //读取的字节数
    while(1)
    {
        //从文件读取pcm数据
        readBufferLen = fread(g_pAudioBuf,1,PCM_BUFFER_SIZE,pAudioFile);
        if(readBufferLen == 0)
        {
            break;
        }
        dataCount += readBufferLen;
        printf("playing %10d bytes data\n",dataCount);
        //更新buffer结束位置
        g_pAudioEnd = g_pAudioBuf + readBufferLen;
        //更新buffer起始位置
        g_pAudioPos = g_pAudioBuf;

        while (g_pAudioPos < g_pAudioEnd)
        {
            //等待pcm数据消耗  1024/48000 = 0.0213333333333333
            SDL_Delay(2);
        }
    }
    printf("playing finish\n");
    SDL_CloseAudio();

_FAIL:
    if(g_pAudioBuf)
        free(g_pAudioBuf);

    if(pAudioFile)
        fclose(pAudioFile);

    //释放SDL
    SDL_Quit();

    return 0;
}
