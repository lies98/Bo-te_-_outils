#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>

static Uint32 moyenne(SDL_Surface *surface, int i, int j, int n)
{
    const int hauteur_initiale = SDL_max(i - n, 0);
    const int largeur_initiale = SDL_max(j - n, 0);
    const int hauteur_finale = SDL_min(i + n, surface->h - 1);
    const int largeur_finale= SDL_min(j + n, surface->w - 1);
    const int nb_pixel = ((hauteur_finale - hauteur_initiale) * (largeur_finale - largeur_initiale));
    const Uint32 *p = surface->pixels;

    Uint32 somme_r = 0, somme_g = 0, somme_b = 0;
    SDL_Color color;

    for (i = hauteur_initiale; i < hauteur_finale ; i++)
        for(j = largeur_initiale; j < largeur_finale; j++)
        {
            SDL_GetRGB(p[i * surface->w + j], surface->format, &color.r, &color.g, &color.b);
            somme_r += color.r;
            somme_g += color.g;
            somme_b += color.b;
        }

    return SDL_MapRGB(surface->format, somme_r / nb_pixel, somme_g / nb_pixel, somme_b / nb_pixel);
}
void flou (SDL_Surface* image,int n)
{
  int y,x;
  Uint32 * pixels = (Uint32 *)image->pixels;
  SDL_Color color;
   for (int y = 0; y < image->h; y++)
   {
     for (int x = 0; x < image->w; x++)
     {   
       Uint32 pixel = pixels[y * image->w + x];
       pixels[y * image->w + x] = moyenne(image, y, x, n);
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
    flou(image,atoi(n));
    SDL_SaveBMP(image,imgdst);
    
  return 0;
}
