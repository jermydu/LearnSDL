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
