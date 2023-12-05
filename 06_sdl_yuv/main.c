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
