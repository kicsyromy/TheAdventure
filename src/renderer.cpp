#include "renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Renderer::Image::Image(SDL_Renderer *renderer, const std::string_view &path)
{
    std::int32_t channels;
    pixels = stbi_load(path.data(), &width, &height, &channels, 4);

    assert(pixels != nullptr);
    assert(channels == 4);

    auto *surface = SDL_CreateRGBSurfaceFrom(pixels, width, height, 32, width * 4, 0, 0, 0, 0);
    assert(surface != nullptr);

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    assert(texture != nullptr);
    SDL_FreeSurface(surface);
}

Renderer::Image::~Image()
{
    SDL_DestroyTexture(texture);
    stbi_image_free(pixels);
}

Renderer::Renderer(SDL_Window *window)
  : renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) }
{}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(renderer);
}

void Renderer::set_color(const Color &color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void Renderer::put_pixel(std::int32_t x, std::int32_t y)
{
    SDL_RenderDrawPoint(renderer, x, y);
}

void Renderer::draw_line(std::int32_t x0, std::int32_t y0, std::int32_t x1, std::int32_t y1)
{
    SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
}

void Renderer::draw_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height)
{
    SDL_Rect rect{ x, y, width, height };
    SDL_RenderDrawRect(renderer, &rect);
}

void Renderer::fill_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height)
{
    SDL_Rect rect{ x, y, width, height };
    SDL_RenderFillRect(renderer, &rect);
}

void Renderer::draw_image(std::int32_t image_id,
                          std::int32_t x,
                          std::int32_t y,
                          std::int32_t width,
                          std::int32_t height)
{
    const Image &image = images.at(image_id);

    if (width == -1)
    {
        width = image.width;
    }
    if (height == -1)
    {
        height = image.height;
    }

    const auto destRect = SDL_Rect{ x, y, width, height };
    SDL_RenderCopy(renderer, image.texture, nullptr, &destRect);
}

std::int32_t Renderer::load_image(const std::string_view &path)
{
    images.emplace_back(this->renderer, path);
    return static_cast<std::int32_t>(images.size()) - 1;
}
