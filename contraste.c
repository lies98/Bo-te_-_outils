#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>


Uint8 fcontraste(Uint8 c, double n)
{
    if(c <= 255 / 2)
        return (Uint8)( (255/2) * SDL_pow((double) 2 * c / 255, n));
    else
        return 255 - fcontraste(255 - c, n);
}
void contraste (SDL_Surface* image,double n)
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
      color.r = fcontraste(color.r, n);
      color.g = fcontraste(color.g, n);
      color.b = fcontraste(color.b, n);
      pixels[y * image->w + x] = SDL_MapRGB(image->format, color.r, color.g, color.b);
      }
   }
}
void SDL_exitWE(const char *message){
  SDL_Log("ERROR: %s > %s \n",message,SDL_GetError());
  SDL_Quit();
  exit(EXIT_FAILURE);
}
int main(int argc,const char **argv){
    const char* img_src =  NULL;
    const char* imgdst = NULL;
    const char* n=NULL;
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
       SDL_exitWE("initialisation  SDL");
    if (argc < 4)
    {
    fprintf (stderr, "usage: %s IMAGE_SRC IMAGE_DST INTENSITE \n", argv[0]);
    return EXIT_FAILURE;
    }
    img_src=argv[1];
    SDL_Surface * image = SDL_LoadBMP(img_src);
    if(image==NULL)
        SDL_exitWE("impossible de charger l'image");
    image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
    imgdst=argv[2];
    n = argv[3];
    contraste(image,atof(n));
    SDL_SaveBMP(image,imgdst);
    
  return 0;
}
