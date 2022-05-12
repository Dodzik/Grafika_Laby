#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "font.h"
#include <iostream>
#include <math.h>
#include "Draw.h"

class ShaMake
{
public:
    ShaMake() = default;
    
    sf::RectangleShape* rectangle(int m_x_click, int m_y_click, int m_x_rel, int m_y_rel, sf::Color outline, sf::Color inside, char key) const{
        sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f((m_x_rel - m_x_click), (m_y_rel - m_y_click)));
        rect->setPosition(sf::Vector2f(m_x_click, m_y_click));
        rect->setOutlineColor(outline);
        rect->setOutlineThickness(1.0f);
        if (key == 'r')
            rect->setFillColor(sf::Color::Transparent);
        else if (key == 'a')
        {
            rect->setFillColor(inside);
        }
        return rect;
    }
    sf::VertexArray* line(int m_x_click, int m_y_click, int m_x_rel, int m_y_rel, sf::Color outline, sf::Color inside) const {
        sf::VertexArray* lin = new sf::VertexArray(sf::LinesStrip, 2);
        (*lin)[0].position = sf::Vector2f(m_x_click, m_y_click);
        (*lin)[0].color = outline;
        (*lin)[1].position = sf::Vector2f(m_x_rel, m_y_rel);
        (*lin)[1].color = inside;

        return lin;
    }
    sf::CircleShape* circle(int m_x_click, int m_y_click, int m_x_rel, int m_y_rel, sf::Color outline, sf::Color inside, char key) const {
        float radius = sqrt(pow(m_x_click - m_x_rel, 2) + pow(m_y_click - m_y_rel, 2)) / 2;

        sf::CircleShape* circ = new sf::CircleShape();
        circ->setPosition((m_x_rel - m_x_click) / 2 - radius + m_x_click, (m_y_rel - m_y_click) / 2 - radius + m_y_click);
        circ->setRadius(radius);
        circ->setOutlineColor(outline);
        circ->setPointCount((int)(radius));
        circ->setFillColor(sf::Color::Transparent);
        circ->setOutlineThickness(1.0f);
        if (key == 'c')
            circ->setFillColor(sf::Color::Transparent);
        else if (key == 'v')
        {
            circ->setFillColor(inside);
        }
        return circ;
    }
};