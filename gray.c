#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
void SDL_exitWE(const char *message){
  SDL_Log("ERROR: %s > %s \n",message,SDL_GetError());
  SDL_Quit();
  exit(EXIT_FAILURE);
}
void grayscale(SDL_Surface* image)
{
  int y,x;
  Uint32 * pixels = (Uint32 *)image->pixels;
  SDL_Color color;
     for (int y = 0; y < image->h; y++)
    {
      for (int x = 0; x < image->w; x++)
      {   
                        
        Uint32 pixel = pixels[y * image->w + x];
        SDL_GetRGB(pixel, image->format, &color.r, &color.g, &color.b);
        Uint8 gris = 0.212671f * color.r + 0.715160f * color.g + 0.072169f * color.b;
        pixels[y * image->w + x] = SDL_MapRGB(image->format, gris, gris, gris);
      }
    } 
}
int main(int argc,const char **argv){
    const char* img_src =  NULL;
    const char* imgdst = NULL;
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
       SDL_exitWE("initialisation  SDL");
    if (argc < 3)
    {
    fprintf (stderr, "usage: %s IMAGE_SRC IMAGE_DST \n", argv[0]);
    return EXIT_FAILURE;
    }
    img_src=argv[1];
    SDL_Surface * image = SDL_LoadBMP(img_src);
    if(image==NULL)
        SDL_exitWE("impossible de charger l'image");
    image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
    imgdst=argv[2];
    grayscale(image);
    SDL_SaveBMP(image,imgdst);
    
  return 0;
}
  

