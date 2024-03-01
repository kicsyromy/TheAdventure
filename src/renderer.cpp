#include "renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Renderer::Image::Image(SDL_Renderer *renderer, const std::uint8_t *data, std::size_t size)
{
    std::int32_t channels;
    auto         pixels =
        stbi_load_from_memory(data, static_cast<std::int32_t>(size), &width, &height, &channels, 4);

    assert(pixels != nullptr);
    assert(channels == 4);

    auto *surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels,
                                                       width,
                                                       height,
                                                       32,
                                                       width * 4,
                                                       SDL_PIXELFORMAT_RGBA32);
    assert(surface != nullptr);

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    assert(texture != nullptr);

    SDL_FreeSurface(surface);
    stbi_image_free(pixels);
}

Renderer::Image::~Image()
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
    }
}

Renderer::Image::Image(Image &&other)
  : width{ other.width }
  , height{ other.height }
  , texture{ other.texture }
{
    other.texture = nullptr;
}

Renderer::Image &Renderer::Image::operator=(Image &&other)
{
    width   = other.width;
    height  = other.height;
    texture = other.texture;

    other.texture = nullptr;

    return *this;
}

Renderer::Renderer(SDL_Window *window)
  : m_renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED) }
{
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    std::int32_t width;
    std::int32_t height;
    SDL_GetRendererOutputSize(m_renderer, &width, &height);
    m_backbuffer = SDL_CreateTexture(m_renderer,
                                     SDL_PIXELFORMAT_RGBA8888,
                                     SDL_TEXTUREACCESS_TARGET,
                                     width,
                                     height);
    SDL_SetRenderTarget(m_renderer, m_backbuffer);
}

Renderer::~Renderer()
{
    SDL_DestroyTexture(m_backbuffer);
    SDL_DestroyRenderer(m_renderer);
}

std::int32_t Renderer::width() const
{
    std::int32_t width;
    SDL_GetRendererOutputSize(m_renderer, &width, nullptr);
    return width;
}

std::int32_t Renderer::height() const
{
    std::int32_t height;
    SDL_GetRendererOutputSize(m_renderer, nullptr, &height);
    return height;
}

void Renderer::set_color(const Color &color)
{
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
}

void Renderer::clear()
{
    SDL_RenderClear(m_renderer);
}

void Renderer::put_pixel(std::int32_t x, std::int32_t y)
{
    SDL_RenderDrawPoint(m_renderer, x, y);
}

void Renderer::draw_line(std::int32_t x0, std::int32_t y0, std::int32_t x1, std::int32_t y1)
{
    SDL_RenderDrawLine(m_renderer, x0, y0, x1, y1);
}

void Renderer::draw_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height)
{
    SDL_Rect rect{ x, y, width, height };
    SDL_RenderDrawRect(m_renderer, &rect);
}

void Renderer::fill_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height)
{
    SDL_Rect rect{ x, y, width, height };
    SDL_RenderFillRect(m_renderer, &rect);
}

void Renderer::draw_image(std::int32_t image_id,
                          std::int32_t src_x,
                          std::int32_t src_y,
                          std::int32_t dest_x,
                          std::int32_t dest_y,
                          bool         flip_x,
                          bool         flip_y,
                          std::int32_t src_width,
                          std::int32_t src_height,
                          std::int32_t dest_width,
                          std::int32_t dest_height)
{
    const Image &image = images.at(image_id);

    if (src_width == -1)
    {
        src_width = image.width - src_x;
    }
    if (src_height == -1)
    {
        src_height = image.height - src_y;
    }

    if (dest_width == -1)
    {
        dest_width = src_width;
    }
    if (dest_height == -1)
    {
        dest_height = src_height;
    }

    SDL_RendererFlip flip = SDL_FLIP_NONE;

    if (flip_x)
    {
        flip = SDL_FLIP_HORIZONTAL;
    }

    if (flip_y)
    {
        flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_VERTICAL);
    }

    const auto srcRect  = SDL_Rect{ src_x, src_y, src_width, src_height };
    const auto destRect = SDL_Rect{ dest_x, dest_y, dest_width, dest_height };
    SDL_RenderCopyEx(m_renderer, image.texture, &srcRect, &destRect, 0, nullptr, flip);
}

std::int32_t Renderer::load_image(const std::uint8_t *data,
                                  std::size_t         size,
                                  std::int32_t       *width,
                                  std::int32_t       *height)
{
    images.emplace_back(this->m_renderer, data, size);

    if (width != nullptr)
    {
        *width = images.back().width;
    }

    if (height != nullptr)
    {
        *height = images.back().height;
    }

    return static_cast<std::int32_t>(images.size()) - 1;
}

void Renderer::present()
{
    SDL_SetRenderTarget(m_renderer, nullptr);

    std::int32_t width;
    std::int32_t height;
    SDL_GetRendererOutputSize(m_renderer, &width, &height);

    const auto srcRect  = SDL_Rect{ 0, 0, 640, 368 };
    const auto destRect = SDL_Rect{ 0, 0, width, height };
    SDL_RenderCopy(m_renderer, m_backbuffer, &srcRect, &destRect);

    SDL_RenderPresent(m_renderer);

    SDL_SetRenderTarget(m_renderer, m_backbuffer);
}

void Renderer::update_backbuffer_size()
{
    std::int32_t width;
    std::int32_t height;
    SDL_GetRendererOutputSize(m_renderer, &width, &height);
    SDL_DestroyTexture(m_backbuffer);
    m_backbuffer = SDL_CreateTexture(m_renderer,
                                     SDL_PIXELFORMAT_RGBA8888,
                                     SDL_TEXTUREACCESS_TARGET,
                                     width,
                                     height);
    SDL_SetRenderTarget(m_renderer, m_backbuffer);
}
