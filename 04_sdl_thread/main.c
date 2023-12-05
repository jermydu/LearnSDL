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
