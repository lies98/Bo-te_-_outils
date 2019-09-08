#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
void SDL_exitWE(const char *message){
  SDL_Log("ERROR: %s > %s \n",message,SDL_GetError());
  SDL_Quit();
  exit(EXIT_FAILURE);
}
int main(int argc,char **argv){
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    const char* WIDTH=NULL;
    const char* HEIGHT=NULL; 
    WIDTH = argv[3];
    HEIGHT = argv[4];
    if (argc != 5)
    {
    fprintf (stderr, "usage: %s IMAGE IMAGE DST [LARGEUR][LONGUEUR] \n", argv[0]);
    return EXIT_FAILURE;
    }
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
       SDL_exitWE("initialisation  SDL");
    
    
    
    if(SDL_CreateWindowAndRenderer(atoi(WIDTH),atoi(HEIGHT),0,&window,&renderer)!=0)
       SDL_exitWE("echec creation fenetre et rendu");
    

    
    SDL_Surface *image = NULL;
    const char* imgsrc =  NULL;
    const char* imgdst=NULL;
    SDL_Texture *texture = NULL;
    
    imgsrc=argv[1];
    
    image = SDL_LoadBMP(imgsrc);
    if(image==NULL){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_exitWE("impossible de charger l'image");
    } 
    texture = SDL_CreateTextureFromSurface(renderer,image);
    SDL_FreeSurface(image);
    if(texture==NULL){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_exitWE("impossible de creer la texture");
    }
    SDL_Rect rectangle;
    if(SDL_QueryTexture(texture,NULL,NULL,&rectangle.w,&rectangle.h) != 0){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_exitWE("impossible de charger la texture");
    }
    rectangle.w = atoi(WIDTH);
    rectangle.h = atoi(HEIGHT);
    rectangle.x = (atoi(WIDTH) -  rectangle.w) / 2;
    rectangle.y = (atoi(HEIGHT) -rectangle.h) / 2;
    
    if(SDL_RenderCopy(renderer,texture,NULL,&rectangle) != 0){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_exitWE("impossible d'afficher' la texture");
    }
    imgdst=argv[2];
    SDL_bool pl = SDL_TRUE;
    SDL_Surface* pScreenShot = SDL_CreateRGBSurface(0, atoi(WIDTH), atoi(HEIGHT), 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000); 

   if(pScreenShot) 
   { 
      // lis les pixels du render et les sauvegarde dans une surface
      SDL_RenderReadPixels(renderer, NULL, SDL_GetWindowPixelFormat(window), pScreenShot->pixels, pScreenShot->pitch); 
 
      SDL_SaveBMP(pScreenShot, imgdst); 

       
      SDL_FreeSurface(pScreenShot); 
   }
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

