#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
void SDL_exitWE(const char *message){
  SDL_Log("ERROR: %s > %s \n",message,SDL_GetError());
  SDL_Quit();
  exit(EXIT_FAILURE);
}
Uint8 fluminosite(Uint8 c, int n)
{
    if(c + n > 255)
        return 255;
    else if(c + n < 0)
        return 0;
    else
        return c + n;
}

void luminosite (SDL_Surface* image,int n)
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
      color.r = fluminosite(color.r, n);
      color.g = fluminosite(color.g, n);
      color.b = fluminosite(color.b, n);
      pixels[y * image->w + x] = SDL_MapRGB(image->format, color.r, color.g, color.b);
      }
   }
  
}
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
void negatif (SDL_Surface* image)
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
        pixels[y * image->w + x] = SDL_MapRGB(image->format, 255 - color.r, 255 - color.g, 255 - color.b);
        }
    }
}
int main(int argc, const char ** argv)
{   
    
    SDL_bool quit = SDL_FALSE;
    SDL_Event event;
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
       SDL_exitWE("initialisation  SDL");
    SDL_Window * window = SDL_CreateWindow("SDL2 Grayscale",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    const char* img_src =  NULL;
    const char* imgdst = NULL;
    if (argc < 2)
    {
    fprintf (stderr, "usage: %s IMAGE(obligatoire) \n", argv[0]);
    return EXIT_FAILURE;
    }
    img_src=argv[1];
    
    SDL_Surface * image = SDL_LoadBMP(img_src);
    if(image==NULL){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_exitWE("impossible de charger l'image");
    } 
    SDL_Texture * texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,image->w, image->h);
    if(texture==NULL){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_exitWE("impossible de creer la texture");
    }
    Uint32 * pixels = (Uint32 *)image->pixels;
    SDL_Rect rectangle;
    SDL_RendererFlip flipType = SDL_FLIP_NONE;
    double degres=0;
    while (!quit)
    {   
        SDL_UpdateTexture(texture, NULL, image->pixels,
            image->w * sizeof(Uint32));
        image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
        Uint32* pixels = (Uint32 *)image->pixels;
        SDL_Color color;
        int n = 2;
        int m = -2;
        double c = 1.5;
        double v = 0.5;
        int f = 2;
        int d= -2;
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
                quit = SDL_TRUE;
                break;
           case SDL_KEYDOWN:
             switch (event.key.keysym.sym)
            {
             case SDLK_g:
                grayscale(image);
                if(argc > 2){
                imgdst=argv[2];
                grayscale(image);
                SDL_SaveBMP(image,imgdst);
                }
                break; 
            case SDLK_n:
               negatif(image);
               if(argc > 2){
                imgdst=argv[2];
                grayscale(image);
                SDL_SaveBMP(image,imgdst);
               }
                break; 
             case SDLK_l:
                luminosite(image,n);
                if(argc > 2){
                imgdst=argv[2];
                luminosite(image,n);
                SDL_SaveBMP(image,imgdst);
                }
                break;
             case SDLK_m:
                luminosite(image,m);
                if(argc > 2){
                imgdst=argv[2];
                luminosite(image,m);
                SDL_SaveBMP(image,imgdst);
                }
                break; 
             case SDLK_c:
                contraste(image,c);
                if(argc > 2){
                imgdst=argv[2];
                contraste(image,c);
                SDL_SaveBMP(image,imgdst);
                }
                break;
             case SDLK_v:
                contraste(image,v);
                if(argc > 2){
                imgdst=argv[2];
                contraste(image,v);
                SDL_SaveBMP(image,imgdst);
                }
                break;
             case SDLK_f:
                flou (image,f);
                if(argc > 2){
                imgdst=argv[2];
                flou (image,f);
                SDL_SaveBMP(image,imgdst);
                }
                break;
            case SDLK_h:
               flipType = SDL_FLIP_HORIZONTAL;
                break;
            case SDLK_x:
               flipType = SDL_FLIP_VERTICAL;
                break;
            case SDLK_a:
               flipType = SDL_FLIP_NONE;
                break; 
            case SDLK_k:
                degres=degres+90;
                break;          
            
            }
            break;
        }
         rectangle.w=640;
         rectangle.h=480;
         rectangle.x = (640 -  rectangle.w) / 2;
         rectangle.y = (480 -  rectangle.h) / 2;
        if(SDL_RenderCopyEx(renderer, texture, NULL, &rectangle,degres,NULL,flipType) != 0){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_exitWE("impossible d'afficher' la texture");
       }
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
