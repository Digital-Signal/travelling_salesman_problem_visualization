#pragma once

#include "SFML/Graphics.hpp"

#define ITEM_COUNT 14

class Menu
{

public:
	Menu();
	void draw(sf::RenderWindow& window);
	void MoveUp();
	void MoveDown();
	int  GetPressedItem() { return selectedItemIndex; }

private:
	int selectedItemIndex;
	sf::Font font;
	sf::Text menu[ITEM_COUNT];
	sf::RectangleShape topRectangle;
	sf::RectangleShape bottomRectangle;
	sf::Text label[2];

};