#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
void SDL_exitWE(const char *message){
  SDL_Log("ERROR: %s > %s \n",message,SDL_GetError());
  SDL_Quit();
  exit(EXIT_FAILURE);
}
int main(int argc,char **argv){
    SDL_RendererFlip flipType = SDL_FLIP_NONE;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    const char* degres=NULL; 
    degres=argv[3];
    
    if (argc != 4)
    {
    fprintf (stderr, "usage: %s IMAGE IMAGE DST(.bmp) (degres de rotation) \n", argv[0]);
    return EXIT_FAILURE;
    }
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
       SDL_exitWE("initialisation  SDL");
    
    
    
    if(SDL_CreateWindowAndRenderer(800,600,0,&window,&renderer)!=0)
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
    rectangle.x = (800 -  rectangle.w) / 2;
    rectangle.y = (600 -rectangle.h) / 2;
    if(SDL_RenderCopyEx(renderer, texture, NULL, &rectangle,atof(degres),NULL,flipType)!= 0){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_exitWE("impossible d'afficher' la texture");
    }
    imgdst=argv[2];
    SDL_bool pl = SDL_TRUE;
    SDL_Surface* pScreenShot = SDL_CreateRGBSurface(0, 800, 600, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000); 

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

