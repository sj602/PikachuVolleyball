#include "TextureManager.h"

TTF_Font* Sans = nullptr;

// for image
SDL_Texture* TextureManager::LoadTexture(const char *texture)
{
    SDL_Surface *tempSurface = IMG_Load(texture);
    if(tempSurface == NULL)
        std::cout << "image not loaded: " << SDL_GetError() << std::endl;
    
    SDL_Texture *tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    
    return tex;
};

// for fonts
SDL_Texture* TextureManager::LoadFont(const char *str)
{
    TTF_Init();
    if(!Sans)
        Sans = TTF_OpenFont("fonts/OdibeeSansRegular.ttf", 24);
        if(!Sans)
            std::cout << "fonts error: " << TTF_GetError() << std::endl;
    SDL_Color Red = {220, 20, 60};
    
    SDL_Surface* tempSurface = TTF_RenderText_Solid(Sans, str, Red);
    if(tempSurface == NULL)
        std::cout << "fonts not loaded: " << SDL_GetError() << std::endl;
    
    SDL_Texture* message = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    
    return message;
};
