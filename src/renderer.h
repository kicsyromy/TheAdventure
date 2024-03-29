#pragma once

#include <SDL2/SDL.h>

#include <cstdint>
#include <string_view>
#include <vector>

struct Color
{
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a;
};

class Renderer final
{
private:
    class Image
    {
    public:
        Image(SDL_Renderer *renderer, const std::uint8_t *data, std::size_t size);
        ~Image();

        Image(const Image &) = delete;
        Image(Image &&);
        Image &operator=(const Image &) = delete;
        Image &operator=(Image &&);

        std::int32_t width;
        std::int32_t height;

        SDL_Texture *texture;
    };

public:
    explicit Renderer(SDL_Window *window);
    ~Renderer();

    Renderer(const Renderer &)            = delete;
    Renderer(Renderer &&)                 = delete;
    Renderer &operator=(const Renderer &) = delete;
    Renderer &operator=(Renderer &&)      = delete;

    int32_t width() const;
    int32_t height() const;

    void         set_color(const Color &color);
    void         clear();
    void         put_pixel(std::int32_t x, std::int32_t y);
    void         draw_line(std::int32_t x0, std::int32_t y0, std::int32_t x1, std::int32_t y1);
    void         draw_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height);
    void         fill_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height);
    void         draw_image(std::int32_t image_id,
                            std::int32_t src_x,
                            std::int32_t src_y,
                            std::int32_t dest_x,
                            std::int32_t dest_y,
                            bool         flip_x      = false,
                            bool         flip_y      = false,
                            std::int32_t src_width   = -1,
                            std::int32_t src_height  = -1,
                            std::int32_t dest_width  = -1,
                            std::int32_t dest_height = -1);
    std::int32_t load_image(const std::uint8_t *data,
                            std::size_t         size,
                            std::int32_t       *width  = nullptr,
                            std::int32_t       *height = nullptr);

    void present();

private:
    SDL_Renderer      *m_renderer{ nullptr };
    std::vector<Image> images;

    SDL_Texture *m_backbuffer{ nullptr };
};
