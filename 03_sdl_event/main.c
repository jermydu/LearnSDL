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
