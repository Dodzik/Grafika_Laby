// Calosc mozna dowolnie edytowac wedle uznania.
#include <iostream>
#include <stdio.h>
#include "font.h"
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Draw.h"
#pragma warning(disable : 4996)

class Menu : public sf::Drawable
{
private:
    sf::Font font;
    sf::Text* text;

    char* dummy = new char[2];
    sf::RectangleShape* rectangle;
    wchar_t* action = new wchar_t[2];
    sf::Texture* colors_texture;
    sf::Sprite* colors_sprite;
    sf::Uint8* colors_pixels;

    sf::RectangleShape rects;
    sf::Uint8 r;
    sf::Uint8 g;
    sf::Uint8 b;
    const unsigned int colors_size_x = 765;
    const unsigned int colors_size_y = 60;
    void draw_to_color_pixels(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b){
        colors_pixels[4 * (y * colors_size_x + x) + 0] = r;
        colors_pixels[4 * (y * colors_size_x + x) + 1] = g;
        colors_pixels[4 * (y * colors_size_x + x) + 2] = b;
        colors_pixels[4 * (y * colors_size_x + x) + 3] = 255;
    }

public:
    sf::Sprite sprite;
    sf::RenderTexture rend_textu;

    bool m_click = false;
    sf::RectangleShape rect_for_color = sf::RectangleShape(sf::Vector2f(35.f, 15.f));
    sf::RectangleShape rect_for_color_back = sf::RectangleShape(sf::Vector2f(35.f, 15.f));
    void save()
    {
        sprite.getTexture()->copyToImage().saveToFile("output.png");
    }
    char get_char(void)
    {
        return *dummy;
    }
    void load()
    {
        rend_textu.clear(sf::Color::Black);
        sf::Texture tex;
        tex.loadFromFile("output.png");
        rend_textu.draw(sf::Sprite(tex));
    }

    void actual(const char key){
        *dummy = key;
        dummy[1] = '\0';
        mbstowcs(action, dummy, 2);
    }
    sf::Color foreground_color;
    sf::Color background_color;
    sf::Color r_draw;
    sf::Color g_draw;
    sf::Color b_draw;
    void setColor(int m_x_click, int m_y_click, char c)
    {
        int cur_col = 4 * (m_y_click * colors_size_x + m_x_click);
        auto r_draw = colors_pixels[cur_col];
        auto g_draw = colors_pixels[cur_col + 1];
        auto b_draw = colors_pixels[cur_col + 2];
        if (c == 'f')
        {
            foreground_color = sf::Color(r_draw, g_draw, b_draw, 255);
            rect_for_color.setFillColor(foreground_color);
        }
        else
        {
            background_color = sf::Color(r_draw, g_draw, b_draw, 255);
            rect_for_color_back.setFillColor(background_color);
        }
    }

    Menu()
    {
        rect_for_color.setFillColor(foreground_color);
        rect_for_color.setOutlineColor(foreground_color);
        rect_for_color.setSize(sf::Vector2f(35, 30));
        rect_for_color.setPosition(sf::Vector2f(765, 0));
        rect_for_color_back.setFillColor(background_color);
        rect_for_color_back.setOutlineColor(background_color);
        rect_for_color_back.setSize(sf::Vector2f(35, 30));
        rect_for_color_back.setPosition(sf::Vector2f(765, 30));
        *action = NULL;
        font.loadFromMemory(font_data, font_data_size);
        text = new sf::Text;
        text->setFont(font);
        text->setCharacterSize(12);
        text->setFillColor(sf::Color::White);
        foreground_color = sf::Color(0, 0, 0);
        background_color = sf::Color(0, 0, 0);
        rectangle = new sf::RectangleShape(sf::Vector2f(796, 536));
        rectangle->setFillColor(sf::Color::Transparent);
        rectangle->setOutlineColor(sf::Color::White);
        rectangle->setOutlineThickness(1.0f);
        rectangle->setPosition(2, 62);
        rend_textu.create(800, 600);
        rend_textu.clear();
        sprite.setTexture(rend_textu.getTexture());

        unsigned int x, y;
        colors_pixels = new sf::Uint8[colors_size_x * colors_size_y * 4];
        for (x = 0; x < 255; x++)
            for (y = 0; y < 30; y++)
            {
                draw_to_color_pixels(x, y, x, 255, 0);
                draw_to_color_pixels(x + 255, y, 255, 255 - x, 0);
                draw_to_color_pixels(x + 510, y, 255, 0, x);
                draw_to_color_pixels(254 - x, y + 30, 0, 255, 255 - x);
                draw_to_color_pixels(509 - x, y + 30, 0, x, 255);
                draw_to_color_pixels(764 - x, y + 30, 255 - x, 0, 255);
            }

        colors_texture = new sf::Texture();
        colors_texture->create(colors_size_x, colors_size_y);
        colors_texture->update(colors_pixels);

        colors_sprite = new sf::Sprite();
        colors_sprite->setTexture(*colors_texture);
        colors_sprite->setPosition(1, 1);
        sprite.setTexture(rend_textu.getTexture());
    };

    ~Menu()
    {
        delete[] dummy;
        delete[] action;
    }
    void outtextxy(sf::RenderTarget& target, float x, float y, wchar_t* str) const
    {
        text->setPosition(x, y);
        text->setString(str);
        target.draw(*text);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        outtextxy(target, 5, 600, (wchar_t*)L"f - wybor koloru rysowania");
        outtextxy(target, 5, 615, (wchar_t*)L"b - wybor koloru wypenniania");
        outtextxy(target, 5, 630, (wchar_t*)L"l - rysowanie linii");
        outtextxy(target, 200, 600, (wchar_t*)L"r - rysowanie prostokata");
        outtextxy(target, 200, 615, (wchar_t*)L"a - rysowanie wypelnionego prostok�ta");
        outtextxy(target, 200, 630, (wchar_t*)L"c - rysowanie okregu");
        outtextxy(target, 340, 630, (wchar_t*)L"v - rysowanie kola");
        outtextxy(target, 470, 600, (wchar_t*)L"w - zapis do pliku");
        outtextxy(target, 470, 615, (wchar_t*)L"o - odczyt z pliku");
        outtextxy(target, 470, 630, (wchar_t*)L"esc - wyjscie");
        outtextxy(target, 650, 615, (wchar_t*)L"Aktualne:");
        outtextxy(target, 710, 615, action);
        target.draw(*rectangle);
        target.draw(*colors_sprite);
        target.draw(rect_for_color);
        target.draw(rect_for_color_back);
    }
};

int main(){
    sf::RenderWindow window(sf::VideoMode(800, 650), "GFK Lab 01", sf::Style::Titlebar | sf::Style::Close);
    sf::Event event;
    Menu menu;
    Draw draw;
    window.setFramerateLimit(60);

    while (window.isOpen()){
        window.clear(sf::Color::Black);

        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
            switch (event.type){
                case (sf::Event::KeyPressed):{
                   switch (event.key.code){
                        case (sf::Keyboard::Escape):{
                            window.close();
                            break;
                        }
                        case (sf::Keyboard::F):{
                            menu.actual('f');
                            draw.action('f');
                            break;
                        }
                        case (sf::Keyboard::B):{
                            menu.actual('b');
                            draw.action('b');
                            break;
                        }
                        case (sf::Keyboard::L):{
                            menu.actual('l');
                            draw.action('l');
                            break;
                        }                        
                        
                        case (sf::Keyboard::R): {
                            menu.actual('r');
                            draw.action('r');
                            break;
                        }
                        case (sf::Keyboard::A): {
                            menu.actual('a');
                            draw.action('a');
                            break;
                        }
                        case (sf::Keyboard::W):{
                            menu.actual('w');
                            draw.save_f();
                            break;
                        }
                        case (sf::Keyboard::O):{
                            menu.actual('o');
                            draw.load_f();
                            break;
                        }
                        case (sf::Keyboard::V): {
                            menu.actual('v');
                            draw.action('v');
                            break;
                        }
                        case (sf::Keyboard::C): {
                            menu.actual('c');
                            draw.action('c');
                            break;
                        }
                    }
                    break;
                }
                case (sf::Event::MouseButtonPressed):{
                    menu.m_click = true;
                    draw.m_x_click = event.mouseButton.x;
                    draw.m_y_click = event.mouseButton.y;
                    draw.m_x_rel = event.mouseButton.x;
                    draw.m_y_rel = event.mouseButton.y;
                    draw.m_click = true;
                        if (menu.get_char() == 'f'){
                            if (event.mouseButton.x <= 765 && event.mouseButton.y <= 60)
                                menu.setColor(draw.m_x_click, draw.m_y_click, 'f');
                        }
                        else if (menu.get_char() == 'b'){
                            if (event.mouseButton.x <= 765 && event.mouseButton.y <= 60)
                                menu.setColor(draw.m_x_click, draw.m_y_click, 'b');
                        }
                        draw.action(menu.get_char());
                        break;
                }
                case (sf::Event::MouseButtonReleased):{
                    draw.m_x_rel = event.mouseButton.x;
                    draw.m_y_rel = event.mouseButton.y;
                    draw.rend_textu.draw(draw);
                    draw.rend_textu.display();
                    menu.m_click = false;
                    draw.m_click = false;
                    break;
                }
                case (sf::Event::MouseMoved):{
                    if (menu.m_click){
                        draw.m_x_rel = event.mouseMove.x;
                        draw.m_y_rel = event.mouseMove.y;
                    }
                    break;
            }
            }
        }
        draw.outline = sf::Color(menu.foreground_color);
        draw.inside = sf::Color(menu.background_color);

        window.draw(draw.sprite);
        window.draw(draw);


        window.draw(menu);
        window.display();
        
    }
    return 0;
}