#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "font.h"
#include <iostream>
#include "ShaMake.h"
class Draw : public sf::Drawable
{

public:
    Draw(){
        rend_textu.create(800, 600);
        rend_textu.clear();
        sprite.setTexture(rend_textu.getTexture());
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const{

        if (m_click){
            if (key_act == 'l'){
                sf::VertexArray* line = shape.line(m_x_click, m_y_click, m_x_rel, m_y_rel, outline, inside);
                target.draw(*line);
                delete line;
            }

            if (key_act == 'r'){
                sf::RectangleShape* rectangle = shape.rectangle(m_x_click, m_y_click, m_x_rel, m_y_rel, outline, inside, 'r');
                target.draw(*rectangle);
                delete rectangle;
            }

            if (key_act == 'a'){
                sf::RectangleShape* rectangle = shape.rectangle(m_x_click, m_y_click, m_x_rel, m_y_rel, outline, inside, 'a');
                target.draw(*rectangle);
                delete rectangle;
            }

            if (key_act == 'c') {
                sf::CircleShape* circle = shape.circle(m_x_click, m_y_click, m_x_rel, m_y_rel, outline,inside,'c');
                target.draw(*circle);
                delete circle;
            }
            if (key_act == 'v') {
                sf::CircleShape* circle = shape.circle(m_x_click, m_y_click, m_x_rel, m_y_rel, outline, inside, 'v');
                target.draw(*circle);
                delete circle;
            }
        }
    }

    void action(char key){
        switch (key){
            case 'f':{
                key_act = 'f';
              break;
            }
           case 'b':{
               key_act = 'b';
                break;
            }
            case 'l':{
                key_act = 'l';
                break;
            }
            case 'r':{
                key_act = 'r';
                break;
            }
            case 'a':{
                key_act = 'a';
                break;
            }
            case 'c':{
                key_act = 'c';
                break;
            }
            case 'v': {
                key_act = 'v';
                break;
            }
        }
    }
    void load_f(void) {
        sf::Texture texture;
        texture.loadFromFile("out.png");
        rend_textu.draw(sf::Sprite(texture));
    }
    void save_f() {
        const sf::Texture* dummy = sprite.getTexture();
        sf::Image imageFromTexture = dummy->copyToImage();
        imageFromTexture.saveToFile("out.png");
    }
    int m_x;
    int m_y;
    int m_x_click;
    int m_y_click;
    int m_x_rel;
    int m_y_rel;
    ShaMake shape;
    sf::Sprite sprite;
    sf::RenderTexture rend_textu;
    sf::Color outline;
    sf::Color inside;
    bool m_click = false;

private:
    char key_act;
    const unsigned int colors_size_x = 765;
};
