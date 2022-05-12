#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "font.h"
#include <cmath>
#include<string>


#define PI 3.14159265

void setText(float x, float y, std::string str, sf::Text* text, sf::RenderTarget& dummy) {
	text->setString(str);
	text->setPosition(x, y);
	dummy.draw(*text);
}
class Circle : public sf::CircleShape {
private:

	sf::Font font;
	sf::Text* text;
	sf::Uint8* pixels = new sf::Uint8[250 * 250 * 4];
	sf::Texture* texture = new sf::Texture();
	sf::CircleShape* mixer = new sf::CircleShape(125.0);
	sf::CircleShape* shape = new sf::CircleShape(125.0);

	inline void draw_to_color_pixels(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b) {
		pixels[4 * (y * 250 + x) + 0] = r;
		pixels[4 * (y * 250 + x) + 1] = g;
		pixels[4 * (y * 250 + x) + 2] = b;
		pixels[4 * (y * 250 + x) + 3] = 255;
	}

	virtual float linePos() = 0;

	virtual void convert(float other[3], float rgb[3]) = 0;

public:

	sf::Color mixColor;
	sf::BlendMode mix;
	float x, y;
	float line_pos = 0.5f;

	Circle(float x, float y) : x(x), y(y) {
		texture->create(250, 250);
		shape->setTexture(texture);
		shape->setPosition(x, y);
		mixer->setPosition(x, y);
		font.loadFromMemory(font_data, font_data_size);
		text = new sf::Text;
		text->setFont(font);
		text->setCharacterSize(13);
		text->setFillColor(sf::Color::Black);
	}

	void createTexture(float a) {
		float other[3];
		float rgb[3];
		for (int i = -125; i < 125; i++) {
			for (int j = -125; j < 125; j++) {
				float radius = sqrt(static_cast<float>((i * i + j * j)));
				float angle = fmodf(atan2f(static_cast<float>(j), static_cast<float> (i)) * 180.0f / PI + 180.0f, 360.0f);
				if (radius <= 125.0f) {
					other[0] = radius * 0.007f;
					other[1] = angle;
					other[2] = a;
					convert(other, rgb);
					draw_to_color_pixels(125 - i, 125 + j, static_cast<unsigned char>(rgb[0]), static_cast<unsigned char>(rgb[1]), static_cast<unsigned char>(rgb[2]));
				}
			}
		}

		texture->update(pixels);
	}

	void setMix(sf::Color color, float pos) {
		line_pos = pos;
		mixColor = color;
		mixColor.a = static_cast<sf::Uint8>(linePos() * 255);
		mixer->setFillColor(mixColor);
	}

	virtual void draw(sf::RenderTarget& dummy, sf::RenderStates states) const {
		dummy.draw(*shape);
		dummy.draw(*mixer, sf::RenderStates(mix));
	}

	sf::Text* getText() const {
		return text;
	}
};

class hslCircle : public Circle {
private:
	void convert(float HSL[3], float RGB[3]) {
		float c = (1.0f - fabsf(2.0f * HSL[2] - 1.0f)) * HSL[0];
		float q = c * (1.0f - fabsf(fmodf(HSL[1] / 60.0f, 2.0f) - 1.0f));
		float m = HSL[2] - c / 2.0f;
		if (HSL[1] < 60) {
			RGB[0] = (c + m) * 255;
			RGB[1] = (q + m) * 255;
			RGB[2] = (0 + m) * 255;
		}
		else if (HSL[1] < 120) {
			RGB[0] = (q + m) * 255;
			RGB[1] = (c + m) * 255;
			RGB[2] = (0 + m) * 255;
		}
		else if (HSL[1] < 180) {
			RGB[0] = (0 + m) * 255;
			RGB[1] = (c + m) * 255;
			RGB[2] = (q + m) * 255;
		}
		else if (HSL[1] < 240) {
			RGB[0] = (0 + m) * 255;
			RGB[1] = (q + m) * 255;
			RGB[2] = (c + m) * 255;
		}
		else if (HSL[1] < 300) {
			RGB[0] = (q + m) * 255;
			RGB[1] = (0 + m) * 255;
			RGB[2] = (c + m) * 255;
		}
		else if (HSL[1] < 360) {
			RGB[0] = (c + m) * 255;
			RGB[1] = (0 + m) * 255;
			RGB[2] = (q + m) * 255;
		}
	}

	float linePos() {
		if (line_pos <= 0.5f) {
			mix = sf::BlendMode(sf::BlendMode::Factor::SrcAlpha, sf::BlendMode::Factor::DstAlpha, sf::BlendMode::ReverseSubtract);
		}
		else {
			mix = sf::BlendAdd;
		}
		return fabsf(line_pos - 0.5f) * 2.0f;
	}

public:
	hslCircle(float x, float y) : Circle(x, y) {
		createTexture(0.5f);
		setMix(sf::Color::White, line_pos);
	}

	virtual void draw(sf::RenderTarget& dummy, sf::RenderStates states) const {
		Circle::draw(dummy, states);
	}
};

class hsvCircle : public Circle {
private:
	void convert(float HSV[3], float RGB[3]) {
		float c = HSV[2] * HSV[0];
		float q = c * (1.0f - fabsf(fmodf(HSV[1] / 60.0f, 2.0f) - 1.0f));
		float m = HSV[2] - c;
		if (HSV[1] < 60) {
			RGB[0] = (c + m) * 255;
			RGB[1] = (q + m) * 255;
			RGB[2] = (0 + m) * 255;
		}
		else if (HSV[1] < 120) {
			RGB[0] = (q + m) * 255;
			RGB[1] = (c + m) * 255;
			RGB[2] = (0 + m) * 255;
		}
		else if (HSV[1] < 180) {
			RGB[0] = (0 + m) * 255;
			RGB[1] = (c + m) * 255;
			RGB[2] = (q + m) * 255;
		}
		else if (HSV[1] < 240) {
			RGB[0] = (0 + m) * 255;
			RGB[1] = (q + m) * 255;
			RGB[2] = (c + m) * 255;
		}
		else if (HSV[1] < 300) {
			RGB[0] = (q + m) * 255;
			RGB[1] = (0 + m) * 255;
			RGB[2] = (c + m) * 255;
		}
		else if (HSV[1] < 360) {
			RGB[0] = (c + m) * 255;
			RGB[1] = (0 + m) * 255;
			RGB[2] = (q + m) * 255;
		}
	}

	float linePos() {
		return 1.0f - line_pos;
	}

public:
	hsvCircle(float x, float y) : Circle(x, y) {
		mix = sf::BlendAlpha;
		createTexture(1.0f);
		setMix(sf::Color::Black, line_pos);
	}

	virtual void draw(sf::RenderTarget& dummy, sf::RenderStates states) const {
		Circle::draw(dummy, states);
	}
};

class cmyCircle : public Circle {
private:
	void convert(float CMY[3], float RGB[3]) {
		RGB[0] = (1.0f - CMY[0]) * 255.0f;
		RGB[1] = (1.0f - CMY[1] * 0.00275f) * 255.0f;
		RGB[2] = (1.0f - CMY[2]) * 255.0f;
	}

	float linePos() {
		return line_pos;
	}

public:
	cmyCircle(float x, float y) : Circle(x, y) {
		mix = sf::BlendMode(sf::BlendMode::Factor::SrcAlpha, sf::BlendMode::Factor::DstAlpha, sf::BlendMode::ReverseSubtract);
		createTexture(0.0f);
		setMix(sf::Color::Blue, line_pos);
	}

	virtual void draw(sf::RenderTarget& dummy, sf::RenderStates states) const {
		Circle::draw(dummy, states);
	}
};


class rgbCircle : public Circle {
private:
	void convert(float rgb[3], float RGB[3]) {
		RGB[0] = rgb[0] * 255;
		RGB[1] = rgb[1] * 0.7f;
		RGB[2] = rgb[2] * 255;
	}

	float linePos() {
		return line_pos;
	}

public:
	rgbCircle(float x, float y) : Circle(x, y) {
		mix = sf::BlendAdd;
		createTexture(0.0f);
		setMix(sf::Color::Blue, line_pos);
	}

	virtual void draw(sf::RenderTarget& dummy, sf::RenderStates states) const {
		Circle::draw(dummy, states);
	}
};


class Menu : public sf::Drawable, public sf::Vector2f {
private:
	sf::Text* mainText;
	int sizeX = 40;
	int sizeY = 255;
	float lineSlide = 0.5f;
	sf::Uint8* slidePixel = new sf::Uint8[sizeX * sizeY * 4];
	sf::Texture* slideTexture = new sf::Texture();
	sf::RectangleShape* slider = new sf::RectangleShape(sf::Vector2f(static_cast<float>(sizeX), static_cast<float>(sizeY)));
	sf::VertexArray* slideLine = new sf::VertexArray(sf::Lines, 2);
	Circle* circHSL;
	Circle* circHSV;
	Circle* circCMY;
	Circle* circRGB;

	inline void draw_to_color_pixels(int x, int y, int z) {
		slidePixel[4 * (y * sizeX + x) + 0] = y;
		slidePixel[4 * (y * sizeX + x) + 1] = y;
		slidePixel[4 * (y * sizeX + x) + 2] = y;
		slidePixel[4 * (y * sizeX + x) + 3] = 255;
	}

public:
	Menu() {
		circHSL = new hslCircle(25.0, 25.0);
		circHSV = new hsvCircle(325.0, 25.0);
		circCMY = new cmyCircle(25.0, 325.0);
		circRGB = new rgbCircle(325.0, 325.0);
		mainText = circCMY->getText();

		for (int x = 1; x <= sizeX; x++) {
			for (int y = 1; y <= sizeY; y++) {
				draw_to_color_pixels(x, y, y);
			}
		}
		slideTexture->create(sizeX, sizeY);
		slideTexture->update(slidePixel);

		slider->setTexture(slideTexture);
		slider->setPosition(610, 10);
		slider->setOutlineThickness(1.0f);
		slider->setOutlineColor(sf::Color::Black);

		(*slideLine)[0].color = sf::Color::Black;
		(*slideLine)[1].color = sf::Color::Black;
	}

	virtual void draw(sf::RenderTarget& dummy, sf::RenderStates states) const {
		dummy.draw(*circHSV);
		dummy.draw(*circRGB);
		dummy.draw(*circCMY);
		dummy.draw(*circHSL);
		dummy.draw(*slider);
		(*slideLine)[0].position = sf::Vector2f(598, 11 + lineSlide * 255);
		(*slideLine)[1].position = sf::Vector2f(605, 11 + lineSlide * 255);
		dummy.draw(*slideLine);
		(*slideLine)[0].position = sf::Vector2f(656, 11 + lineSlide * 255);
		(*slideLine)[1].position = sf::Vector2f(663, 11 + lineSlide * 255);
		dummy.draw(*slideLine);
	}
	void updatePos(float pos) {
		lineSlide = pos;
		circHSL->setMix(circHSL->mixColor, pos);
		circHSV->setMix(circHSV->mixColor, pos);
		circCMY->setMix(circCMY->mixColor, pos);
		circRGB->setMix(circRGB->mixColor, pos);
	}
	sf::Text* getMText() const {
		return mainText;
	}
};

int main() {
	sf::RenderWindow window(sf::VideoMode(750, 600), "GFK Lab 02", sf::Style::Titlebar | sf::Style::Close);
	sf::Clock clock;
	sf::Time time = sf::Time::Zero;
	unsigned int FPS = 0, frame_counter = 0;
	Menu menu;
	bool mPressed = false;
	float scHSL_HSV = 0.510;
	float scCMY = 51.0;
	float scRGB = 130;
	sf::Text* shScale = menu.getMText();
	clock.restart().asMilliseconds();

	while (window.isOpen()) {
		sf::Event event;
		window.clear(sf::Color::Cyan);

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			else if (event.key.code == sf::Keyboard::Escape) {
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (event.mouseButton.x >= 610 and event.mouseButton.x <= 650 and event.mouseButton.y >= 10 and event.mouseButton.y <= 265) {
						menu.updatePos((event.mouseButton.y - 10) / 255.0f);
						mPressed = true;
					}
				}
			}
			else if (event.type == sf::Event::MouseMoved) {
				if (mPressed) {
					if (event.mouseMove.x >= 610 and event.mouseMove.x <= 650 and event.mouseMove.y >= 10 and event.mouseMove.y <= 265) {
						menu.updatePos((event.mouseMove.y - 10.0) / 255.0f);
						scHSL_HSV = ((event.mouseMove.y - 10.0) / 255.0f);
						scCMY = ((event.mouseMove.y - 10.0) / 255.0f) * 100.0;
						scRGB = (event.mouseMove.y - 10.0);
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				mPressed = false;
			}
		}
		window.draw(menu);
		setText(25, 33, "HSL", shScale, window);
		setText(235, 265, "L=", shScale, window);
		setText(250, 265, std::to_string(scHSL_HSV), shScale, window);
		setText(325, 33, "HSV", shScale, window);
		setText(530, 265, "V=", shScale, window);
		setText(545, 265, std::to_string(scHSL_HSV), shScale, window);
		setText(25, 333, "CMY", shScale, window);
		setText(235, 565, "Y=", shScale, window);
		setText(250, 565, std::to_string(scCMY), shScale, window);
		setText(320, 565, "%", shScale, window);
		setText(325, 333, "RGB", shScale, window);
		setText(530, 565, "B=", shScale, window);
		setText(545, 565, std::to_string(static_cast<int>(scRGB)), shScale, window);
	

		if (clock.getElapsedTime().asSeconds() >= 1.0f) {
			FPS = (unsigned int)((float)frame_counter / clock.getElapsedTime().asSeconds());
			clock.restart();
			frame_counter = 0;
		}
		frame_counter++;
		setText(675, 330, "FPS", shScale, window);
		setText(675, 355, std::to_string(FPS), shScale, window);
		window.display();
	}
}