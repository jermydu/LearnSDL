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
