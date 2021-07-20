#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#ifndef DEFINEDVALUES
#define DEFINEDVALUES


#define DEFAULTBLOCKSX 20
#define DEFAULTBLOCKSY 20
#define DEFAULTBLOCKSIZE 18





struct
{
    int blocksx, blocksy, blocksize, nodecount, headdirx,
    headdiry, ndirs, nmaxdirs; //nodecount = snake body part count

}   values = {DEFAULTBLOCKSX, DEFAULTBLOCKSY, DEFAULTBLOCKSIZE, 1};


struct
{
    int x,y;
    SDL_Rect rect;


}food = {5,5, (SDL_Rect){5*DEFAULTBLOCKSIZE,5*DEFAULTBLOCKSIZE,DEFAULTBLOCKSIZE,DEFAULTBLOCKSIZE} };



typedef struct
{
    unsigned char color[3];
    int x, y;
    SDL_Rect rect;



}   node;



typedef struct
{
    char x,y;


}   dir;




#endif


#define debugi(x)  printf("%d\n",x)


int screenx, screeny;
dir *dirs;
node *snake;
SDL_Window* win;
SDL_Renderer* renderer;
dir lastdir;
char ridi;
int emtiyaz;

void keyboard() {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // check for messages
            switch (event.type) {
                // exit if the window is closed
            case SDL_QUIT:
                exit(0);
                break;
                // check for keypresses
            case SDL_KEYDOWN:
                switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
                        values.headdirx=-1;
                        values.headdiry=0;
                        break;
                    case SDLK_RIGHT:
                        values.headdirx=1;
                        values.headdiry=0;
                        break;
                    case SDLK_UP:
                        values.headdirx=0;
                        values.headdiry=-1;
                        break;
                    case SDLK_DOWN:
                        values.headdirx=0;
                        values.headdiry=1;
                        break;

                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                break;
            default:
                break;
            }
        } // end of message processing
}









void addnode(int x, int y, unsigned char color[])
{
    if (values.nodecount < values.blocksx * values.blocksy)
    {
        snake[values.nodecount].rect.x = x * values.blocksize;
        snake[values.nodecount].rect.y = y * values.blocksize;
        snake[values.nodecount].rect.h = snake[values.nodecount].rect.w = values.blocksize;

        snake[values.nodecount].x = x;
        snake[values.nodecount].y = y;/*
        snake[values.nodecount-1].color[0]=color[0];
        snake[values.nodecount-1].color[1]=color[1];
        snake[values.nodecount-1].color[2]=color[2];*/

        snake[values.nodecount].color[0]=255;
        snake[values.nodecount].color[1]=0;
        snake[values.nodecount].color[2]=0;


    }
    else
    {
        fprintf(stderr,"ERROR: NODE VALUE EXCEEDED");
    }

}


dir pop_back()
{
    values.ndirs-=1;


    //not likely to happen but ....
    if (values.ndirs < 0)
    {
        fprintf(stderr, "ridam behet dige masir nadarim");
    }
    return dirs[values.ndirs];



}


void push_back(dir _dir)
{
    dirs[values.ndirs].x = _dir.x;
    dirs[values.ndirs].y = _dir.y;
    values.ndirs++;

}



void push_front(dir _dir)
{
    int i;

    dir *temp = (dir*) malloc(sizeof(dir) * values.ndirs);



    for(i = 0; i <values.ndirs;i++){
        temp[i].x = dirs[i].x;
        temp[i].y = dirs[i].y;

    }

    dirs[0] = _dir;


    for(i = 1; i <=values.ndirs;i++){

        dirs[i].x = temp[i-1].x;
        dirs[i].y = temp[i-1].y;


    }

    free(temp);
    values.ndirs++;

}



void setup()
{
    emtiyaz=0;
    ridi = 0;
    snake = (node *) malloc(sizeof (node*) * values.blocksx * values.blocksy);

    values.headdirx=1;
    values.headdiry=0;





    values.ndirs = 1;
    values.nmaxdirs = values.blocksx * values.blocksy;


    snake[0].x = 1;
    snake[0].y = 1;
    snake[0].color[1]=255;
    snake[0].rect.x = values.blocksize;
    snake[0].rect.y = values.blocksize;
    snake[0].rect.h = snake[0].rect.w = values.blocksize;


    dirs = (dir*) malloc(sizeof(dir) * values.blocksx * values.blocksy);

    dirs[0].x = 1;
    dirs[0].y = 0;

    /*
    for (int i = 0;i<values.blocksx ;i++)
    {
        blocks[i] = (block*) malloc(sizeof(block)*values.blocksy);
    }*/

    screenx = values.blocksx*values.blocksize;
    screeny = values.blocksy*values.blocksize;

}

void drawgrid(SDL_Renderer * renderer)
{




    static int i;
    for (i = 0;i<=values.blocksx ;i++)
    {
        //draw x-changing grids

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, i * values.blocksize, 0, i * values.blocksize, screeny);
    }

    for (i = 0;i<=values.blocksy ;i++)
    {
        //draw y-changing grids

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, 0, i * values.blocksize, screenx,i * values.blocksize );


    }


}


void drawplayer(node player)
{
    //draw player



    SDL_SetRenderDrawColor(renderer, player.color[0], player.color[1], player.color[2], SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect( renderer, &player.rect );







}


void drawfood()
{

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect( renderer, &food.rect );




}
char khordi;

void movesnake()
{

    static int i,j;

    push_front((dir) {values.headdirx, values.headdiry} );







    lastdir = pop_back();





    for (i = j = 0;i < values.nodecount && j <values.ndirs;i++,j++)
    {
        snake[i].x += dirs[j].x;
        snake[i].y += dirs[j].y;

        if (snake[i].x > values.blocksx)
        {
            snake[i].x = 0;
        }

        if (snake[i].x < 0)
        {
            snake[i].x=values.blocksx;
        }

        if (snake[i].y > values.blocksy)
        {
            snake[i].y = 0;
        }

        if (snake[i].y < 0)
        {
            snake[i].y= values.blocksy;
        }

        snake[i].rect.x = snake[i].x * values.blocksize;
        snake[i].rect.y = snake[i].y * values.blocksize;
    }

}





void game()
{
    static int i;

    keyboard();





    if (snake[0].x==food.x && snake[0].y == food.y)
    {


            srand(time(0));
            printf("khordish\n emtiyaz ---> %d\n",++emtiyaz);
            khordi = 1;




    }

    if (khordi)
    {


        addnode(snake[values.nodecount-1].x,snake[values.nodecount-1].y,snake[0].color);
        push_back(lastdir);


        food.x =  rand() % values.blocksx;;
        food.y = rand() % values.blocksy;

        food.rect.x = food.x * values.blocksize;
        food.rect.y = food.y * values.blocksize;




    }

    if (!ridi){
        movesnake();
    }

    if (khordi)
    {
        values.nodecount++;
        khordi =0;
    }

    drawfood();





    for (i = 0;i < values.nodecount;i++)
    {
        if (i !=0 && snake[i].x == snake[0].x && snake[i].y == snake[0].y && !ridi)
        {
            puts("ridi tamoom shod hala beband baziyo");
            ridi = 1;
        }

        drawplayer(snake[i]);



    }


}

void main_loop()
{

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);



        Uint64 start = SDL_GetPerformanceCounter();

        game();
        //drawgrid(renderer);

        SDL_RenderPresent(renderer);
        Uint64 end = SDL_GetPerformanceCounter();

        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

        SDL_Delay((int)(100.666f - elapsedMS));




}

int main()
{
    setup();


    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}
    atexit(SDL_Quit);


    win = SDL_CreateWindow("A line", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenx+1, screeny+1, 0);
    renderer = SDL_CreateRenderer(win, -1, 0);

   while(1)
   {
        main_loop();
   }

   // emscripten_set_main_loop(main_loop, 0, 1);







    return 0;
}
