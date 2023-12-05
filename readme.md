# SDL使用详细说明

# 官网 https://www.libsdl.org/


关于SDL

Simple DirectMedia Layer是一个跨平台开发库，旨在通过OpenGL和Direct3D提供对音频、键盘、鼠标、操纵杆和图形硬件的低级别访问。它被视频播放软件、模拟器和流行游戏使用，包括Valve的获奖目录和许多Humble Bundle游戏。

SDL正式支持Windows、macOS、Linux、iOS和Android。对其他平台的支持可以在源代码中找到。

SDL是用C编写的，与C++一起使用，并且有可用于其他几种语言的绑定，包括C#和Python。

SDL2.0是在zlib许可证下发布的。此许可证允许您在任何软件中自由使用SDL。

# 1. 开发环境

1. qt5.15.2 mingw_64 
2.  SDL2-2.23.5  x64 

下载地址 https://github.com/libsdl-org/SDL/releases/tag/release-2.28.5

![img](https://cdn.nlark.com/yuque/0/2023/png/29411390/1700975690166-7ff60585-446a-4307-a3ea-c6a82548375e.png)

# 2. 创建qt测试项目

## 2.1. 创建C项目

![img](https://cdn.nlark.com/yuque/0/2023/png/29411390/1700975787392-360fe7ca-e471-4858-ad3d-43fef9cd6b43.png)

![img](https://cdn.nlark.com/yuque/0/2023/png/29411390/1700976275627-df0942ea-8e41-43ce-a407-6badb3035841.png)

## 2.2. 配置.pro

```bash
#生成exe 目录
DESTDIR = $$PWD/../Bin

#include
INCLUDEPATH += $$PWD/../SDL2-2.28.5/include
#lib
LIBS +=   $$PWD/../SDL2-2.28.5/lib/x64/SDL2.lib
```

## 2.3. 测试代码显示一个简单窗口

```c
#include <stdio.h>
#include <SDL.h>

//SDL 中 #define main SDL_main 导致main找不到入口
#undef main
int main()
{
    printf("SDL_Basic\n");

    SDL_Window* pWindow = NULL;

    //初始化SDL
    SDL_Init(SDL_INIT_VIDEO);

    //创建窗口
    pWindow = SDL_CreateWindow("SDL_HELLO",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
                               640,480,SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if(!pWindow)
    {
        printf("create window error:%s\n",SDL_GetError());
        return 1;
    }

    //延迟5秒
    SDL_Delay(5000);

    //销毁窗口
    if(pWindow)
   	 	SDL_DestroyWindow(pWindow);

    //释放SDL
    SDL_Quit();

    return 0;
}
```

# 3. 使用Render和Texture

窗口显示用到的API

- SDL_Init()							初始化SDL
- SDL_CreateWindow				创建窗口 SDL_Window
- SDL_CreateRenderer				创建渲染器SDL_Renderer
- SDL_CreateTexture					创建渲染器SDL_Texture
- SDL_UpdateTexture					设置纹理数据
- SDL_RenderCopy					将纹理数据拷贝给渲染器
- SDL_RenderPresent					显示渲染数据
- SDL_Delay						延时
- SDL_Quit							释放SDL	

```c
#include <stdio.h>
#include <SDL.h>

int WinMain()
{
    int run = 1;
    printf("SDL_Window\n");

    SDL_Window* pWindow = NULL;         //窗口
    SDL_Renderer* pRender = NULL;       //渲染器
    SDL_Texture* pTexture = NULL;       //纹理
    SDL_Rect rectangle;                 //矩形
    rectangle.w = 100;
    rectangle.h = 100;

    //初始化SDL
    SDL_Init(SDL_INIT_VIDEO);

    //创建窗口
    pWindow = SDL_CreateWindow("SDL_HELLO",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
                               640,480,SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(!pWindow)
        return 1;

    //创建渲染器
    pRender = SDL_CreateRenderer(pWindow,-1,0);
    if(!pRender)
        return 1;

    //创建纹理
    pTexture = SDL_CreateTexture(pRender,SDL_PIXELFORMAT_ABGR8888,SDL_TEXTUREACCESS_TARGET,
                                 640,480);
    if(!pTexture)
        return 1;

    int showCount = 0;
    while(run)
    {
        rectangle.x = rand() % 500;
        rectangle.y = rand() % 400;

        //设置渲染目标
        SDL_SetRenderTarget(pRender,pTexture);
        //纹理背景色
        SDL_SetRenderDrawColor(pRender,234,200,111,255);
        //清屏
        SDL_RenderClear(pRender);

        //绘制矩形
        SDL_RenderDrawRect(pRender,&rectangle);
        //设置矩形颜色
        SDL_SetRenderDrawColor(pRender,222,100,3,255);
        //填充矩形
        SDL_RenderFillRect(pRender,&rectangle);

        //设置渲染目标为默认窗口
        SDL_SetRenderTarget(pRender,NULL);
        //拷贝纹理到CPU
        SDL_RenderCopy(pRender,pTexture,NULL,NULL);

        //渲染输出
        SDL_RenderPresent(pRender);

        SDL_Delay(500);
        if(showCount++ > 30)
            run = 0;
    }

    if(pTexture)
        SDL_DestroyTexture(pTexture);
    if(pRender)
        SDL_DestroyRenderer(pRender);
    if(pWindow)
        SDL_DestroyWindow(pWindow);
    SDL_Quit();

    return 0;
}
```

# 4. SDL事件

事件相关API

- SDL_WaitEvent			等待一个事件  阻塞
- SDL_PollEvent				轮询获取事件 非阻塞  如果时间队列中有待处理事件，返回1;如果没有可处理事件，则返回0
- SDL_PushEvent			发送一个事件 
- SDL_PumpEvents			将硬件设备产生的事件放入事件队列
- SDL_PeepEvents			从事件队列提取一个事件

**注意：经本人测试   下载的最新的 SDL2-2.28.5 接收不到键盘消息，而用 SDL2-2.0.10 可以**

```c
#include <stdio.h>
#include <SDL.h>

#define MY_EVENT_QUIT (SDL_USEREVENT + 100)

int WinMain()
{
    printf("SDL_Window\n");

    SDL_Window* pWindow = NULL;         //窗口
    SDL_Renderer* pRender = NULL;       //渲染器

    //初始化SDL
    SDL_Init(SDL_INIT_VIDEO);

    //创建窗口
    pWindow = SDL_CreateWindow("SDL_HELLO",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
                               640,480,SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(!pWindow)
        return 1;

    //创建渲染器
    pRender = SDL_CreateRenderer(pWindow,-1,0);
    if(!pRender)
        return 1;

    SDL_SetRenderDrawColor(pRender,255,100,100,255);
    SDL_RenderClear(pRender);
    SDL_RenderPresent(pRender);

    //事件
    SDL_Event event;
    int nExit = 0;
    while(1)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_KEYDOWN:       //键盘事件
                switch (event.key.keysym.sym)   //键值
                {
                    case SDLK_a:
                        printf("key down a\n");
                        break;
                    case SDLK_s:
                        printf("key down s\n");
                        break;
                    case SDLK_d:
                        printf("key down d\n");
                        break;
                    case SDLK_f:
                        printf("key down f\n");
                        break;
                    case SDLK_q:
                        printf("key down q and push quit event\n");
                        SDL_Event eventQ;
                        eventQ.type = MY_EVENT_QUIT;
                        //发送退出事件
                        SDL_PushEvent(&eventQ);
                        break;
                    default:
                        printf("key down 0x%x\n",event.key.keysym.sym);
                        break;
                }
                break;
            case SDL_MOUSEWHEEL:        //鼠标滚轮
                printf("mouse wheel event\n");
                break;
            case SDL_MOUSEBUTTONDOWN:    //鼠标点击事件
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    printf("mouse click left\n");
                }
                else if(event.button.button == SDL_BUTTON_RIGHT)
                {
                    printf("mouse click right\n");
                }else
                {
                    printf("mouse click %d\n",event.button.button);
                }
                break;
            case SDL_MOUSEMOTION:          //鼠标移动事件
                //printf("mouse movie (%d,%d)\n",event.button.x,event.button.y);
                break;
            case MY_EVENT_QUIT:
                printf("receive quit event\n");
                nExit = 1;
                break;
            default:
                break;
        }
        if(nExit)
            break;
    }

    if(pRender)
        SDL_DestroyRenderer(pRender);
    if(pWindow)
        SDL_DestroyWindow(pWindow);
    SDL_Quit();

    return 0;
}
```

# 5. 多线程

多线程相关API

- SDL_CreateThread			创建线程
- SDL_WaitThread			等待线程
- SDL_CreateMutex/SDL_DestroyMutex	创建/销毁互斥锁
- SDL_LockMutex/SDL_UnlockMutex	加锁/取消锁
- SDL_CreateCond/SDL_DestroyCond	创建/销毁条件变量（信号量）
- SDL_CondWait/SDL_CondSignal		等待/通知条件变量（信号量）				

此函数允许线程在等待条件变量时解锁互斥锁，从而让其他线程可以继续访问共享资源

**当一个线程调用 SDL_CondWait() 函数时，它会先解锁互斥锁（把自己的锁解开，所以使用SDL_CondWait函数之前，必须先获取到锁），然后一直等待条件变量的信号（SDL_CondSignal或SDL_CondBroadcast），直到信号被发送给该条件变量。当线程接收到信号后，它会重新获取互斥锁（重新获取就是尝试获取，如果其他线程不释放该锁，即使收到信号也不会向下执行。），以访问共享资源。**

```c
#include <stdio.h>
#include <SDL.h>
#include <unistd.h>

//互斥锁
SDL_mutex* pLock = NULL;
//信号量
SDL_cond* pCond = NULL;

int threadTaskA()
{
    printf("threadA-------start\n");
    SDL_LockMutex(pLock);   //加锁
    printf("threadA-------lockMutex\n");

    //等待信号量并且释放lock
    printf("threadA-------CondWait and unlock\n");
    SDL_CondWait(pCond,pLock);
    int second = 10;
    //收到信号量后 加锁 继续执行
    printf("threadA-------receive Cond\n");
    while(second)
    {
        printf("threadA-------%d\n",second);
        sleep(1);
        --second;
    }

    SDL_UnlockMutex(pLock);   //解锁
    printf("threadA-------unlockMutex\n");
    printf("threadA-------end\n");
    return 0;
}

int threadTaskB()
{
    printf("threadB-------start\n");
    SDL_LockMutex(pLock);   //加锁
    printf("threadB-------lockMutex\n");

    int second = 10;
    while(second)
    {
        printf("threadB-------%d\n",second);
        sleep(1);
        --second;
    }

    //发送信号量
    SDL_CondSignal(pCond);
    printf("threadB-------send Cond\n");

    SDL_UnlockMutex(pLock);   //解锁
    printf("threadB-------unlockMutex\n");
    printf("threadB-------end\n");
    return 0;
}

int WinMain()
{
    printf("main-------\n");
    pLock = SDL_CreateMutex();
    pCond = SDL_CreateCond();

    SDL_Thread* pThreadA = SDL_CreateThread(threadTaskA,"threadTaskA",NULL);
    if(!pThreadA)
        printf("%s\n",SDL_GetError());

    sleep(5);
    SDL_Thread* pThreadB = SDL_CreateThread(threadTaskB,"threadTaskA",NULL);
    if(!pThreadB)
        printf("%s\n",SDL_GetError());

    //等待线程退出
    SDL_WaitThread(pThreadB,NULL);
    SDL_WaitThread(pThreadA,NULL);

    SDL_DestroyMutex(pLock);
    SDL_DestroyCond(pCond);

    printf("main-------end\n");

    return 0;
}
```

# 6. 读取pcm数据

pcm操作相关API

-  SDL_OpenAudio(SDL_AudioSpec * desired,SDL_AudioSpec * obtained)  打开音频设备
- SDL_OpenAudioDevice(NULL, 0, desired, obtained, SDL_AUDIO_ALLOW_ANY_CHANGE)  新版打开音频设备
- SDL_PauseAudio	播放/暂停
- SDL_MixAudio 	混合音频buffer

```c
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
```

# 7. 读取yuv

```c
#include <stdio.h>
#include <SDL.h>
#include <string.h>

//自定义消息类型
#define FRESH_VIDEO_EVENT (SDL_USEREVENT + 1)  //绘制刷新
#define QUIT_EVENT (SDL_USEREVENT + 2)  //退出

//定义分辨率
#define YUV_WIDTH       640
#define YUV_HEIGHT      480

//YUV格式
#define YUV_FROMAT  SDL_PIXELFORMAT_IYUV

int nThreadExit = 0;   //线程退出

//刷新视频线程
int FreshVideoThread(void* data)
{
    while(!nThreadExit)
    {
        SDL_Event event;
        event.type = FRESH_VIDEO_EVENT;
        SDL_PushEvent(&event);
        SDL_Delay(40);
    }

    nThreadExit = 0;
    //退出
    SDL_Event event;
    event.type = QUIT_EVENT;
    SDL_PushEvent(&event);

    return 0;
}

int WinMain()
{
    //初始化 SDL
    if(SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf( stderr, "Could not initialize SDL - %s\n", SDL_GetError());
        return -1;
    }

    // SDL
    SDL_Event event;                            // 事件
    SDL_Rect rect;                              // 矩形
    SDL_Window *window = NULL;                  // 窗口
    SDL_Renderer *renderer = NULL;              // 渲染
    SDL_Texture *texture = NULL;                // 纹理
    SDL_Thread *videoThread = NULL;            // 请求刷新线程
    uint32_t pixFormat = YUV_FROMAT;            // YUV420P，即是SDL_PIXELFORMAT_IYUV

    // 分辨率
    // 1. YUV的分辨率
    int videoWidth = YUV_WIDTH;
    int videoHeight = YUV_HEIGHT;
    // 2.显示窗口的分辨率
    int winWidth = YUV_WIDTH;
    int winHeight = YUV_WIDTH;

    // YUV文件句柄
    FILE *videoFd = NULL;
    const char *yuvPath = "testvideo1_yuv420p_640x480.yuv";

    size_t videoBuffLen = 0;

    uint8_t *videoBuf = NULL; //读取数据后先把放到buffer里面

    //YUV420P格式
    uint32_t y_frame_len = videoWidth * videoHeight;
    uint32_t u_frame_len = videoWidth * videoHeight / 4;
    uint32_t v_frame_len = videoWidth * videoHeight / 4;
    uint32_t yuv_frame_len = y_frame_len + u_frame_len + v_frame_len;

    //创建窗口
    window = SDL_CreateWindow("YUV Player",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              videoWidth, videoHeight,
                              SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    if(!window)
    {
        fprintf(stderr, "SDL: could not create window, err:%s\n",SDL_GetError());
        goto _FAIL;
    }
    // 基于窗口创建渲染器
    renderer = SDL_CreateRenderer(window, -1, 0);
    // 基于渲染器创建纹理
    texture = SDL_CreateTexture(renderer,
                                pixFormat,
                                SDL_TEXTUREACCESS_STREAMING,
                                videoWidth,
                                videoHeight);

    // 分配空间
    videoBuf = (uint8_t*)malloc(yuv_frame_len);
    if(!videoBuf)
    {
        fprintf(stderr, "Failed to alloce yuv frame space!\n");
        goto _FAIL;
    }

    // 打开YUV文件
    videoFd = fopen(yuvPath, "rb");
    if( !videoFd )
    {
        fprintf(stderr, "Failed to open yuv file\n");
        goto _FAIL;
    }
    // 创建请求刷新线程
    videoThread = SDL_CreateThread(FreshVideoThread,
                                   NULL,
                                   NULL);

    while (1)
    {
        // 收取SDL系统里面的事件
        SDL_WaitEvent(&event);
        if(event.type == FRESH_VIDEO_EVENT) // 画面刷新事件
        {
            videoBuffLen = fread(videoBuf, 1, yuv_frame_len, videoFd);
            if(videoBuffLen <= 0)
            {
                fprintf(stderr, "Failed to read data from yuv file!\n");
                goto _FAIL;
            }
            // 设置纹理的数据
            SDL_UpdateTexture(texture, NULL, videoBuf, videoWidth);

            // 显示区域，可以通过修改w和h进行缩放
            rect.x = 0;
            rect.y = 0;
            float w_ratio = winWidth * 1.0 /videoWidth;
            float h_ratio = winHeight * 1.0 /videoHeight;
            rect.w = videoWidth * w_ratio;
            rect.h = videoHeight * h_ratio;

            // 清除当前显示
            SDL_RenderClear(renderer);
            // 将纹理的数据拷贝给渲染器
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            // 显示
            SDL_RenderPresent(renderer);
        }
        else if(event.type == SDL_WINDOWEVENT)
        {
            //If Resize
            SDL_GetWindowSize(window, &winWidth, &winHeight);
            printf("SDL_WINDOWEVENT win_width:%d, win_height:%d\n",winWidth,
                   winHeight );
        }
        else if(event.type == SDL_QUIT) //退出事件
        {
            nThreadExit = 1;
        }
        else if(event.type == QUIT_EVENT)
        {
            break;
        }
    }

    _FAIL:
    nThreadExit = 1;      // 保证线程能够退出
    // 释放资源
    if(videoThread)
        SDL_WaitThread(videoThread, NULL); // 等待线程退出
    if(videoBuf)
        free(videoBuf);
    if(videoFd)
        fclose(videoFd);
    if(texture)
        SDL_DestroyTexture(texture);
    if(renderer)
        SDL_DestroyRenderer(renderer);
    if(window)
        SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
```