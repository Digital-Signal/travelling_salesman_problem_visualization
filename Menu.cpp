#include "Menu.h"

Menu::Menu()
{
	font.loadFromFile("C:/Windows/Fonts/Arial.ttf");

	// -----------------------------------------------------------------------

	label[0].setFont(font);
	label[0].setString("Display Options:");
	label[0].setCharacterSize(20);
	label[0].setFillColor(sf::Color::Black);
	label[0].setStyle(sf::Text::Bold | sf::Text::Underlined);
	label[0].setPosition(810, 5);

	// -----------------------------------------------------------------------

	menu[0].setFont(font);
	menu[0].setString("Generate Random Nodes");
	menu[0].setCharacterSize(20);
	menu[0].setFillColor(sf::Color::Green);
	menu[0].setStyle(sf::Text::Bold);
	menu[0].setPosition(810, 30);
	
	menu[1].setFont(font);
	menu[1].setString("Add Nodes");
	menu[1].setCharacterSize(20);
	menu[1].setFillColor(sf::Color::White);
	menu[1].setStyle(sf::Text::Bold);
	menu[1].setPosition(810, 55);

	menu[2].setFont(font);
	menu[2].setString("Clear Everything / Restart");
	menu[2].setCharacterSize(20);
	menu[2].setFillColor(sf::Color::White);
	menu[2].setStyle(sf::Text::Bold);
	menu[2].setPosition(810, 80);

	menu[3].setFont(font);
	menu[3].setString("Display Complete Graph");
	menu[3].setCharacterSize(20);
	menu[3].setFillColor(sf::Color::White);
	menu[3].setStyle(sf::Text::Bold);
	menu[3].setPosition(810, 105);

	// -----------------------------------------------------------------------

	label[1].setFont(font);
	label[1].setString("Algorithmic Solutions:");
	label[1].setCharacterSize(20);
	label[1].setFillColor(sf::Color::Black);
	label[1].setStyle(sf::Text::Bold | sf::Text::Underlined);
	label[1].setPosition(810, 130);

	// -----------------------------------------------------------------------

	menu[4].setFont(font);
	menu[4].setString("Brute Force (Exhaustive)");
	menu[4].setCharacterSize(20);
	menu[4].setFillColor(sf::Color::White);
	menu[4].setStyle(sf::Text::Bold);
	menu[4].setPosition(810, 155);

	menu[5].setFont(font);
	menu[5].setString("Random");
	menu[5].setCharacterSize(20);
	menu[5].setFillColor(sf::Color::White);
	menu[5].setStyle(sf::Text::Bold);
	menu[5].setPosition(810, 180);

	menu[6].setFont(font);
	menu[6].setString("Dynamic Programming (Memoization)");
	menu[6].setCharacterSize(20);
	menu[6].setFillColor(sf::Color::White);
	menu[6].setStyle(sf::Text::Bold);
	menu[6].setPosition(810, 205);

	menu[7].setFont(font);
	menu[7].setString("Nearest Neighbor (Greedy)");
	menu[7].setCharacterSize(20);
	menu[7].setFillColor(sf::Color::White);
	menu[7].setStyle(sf::Text::Bold);
	menu[7].setPosition(810, 230);

	menu[8].setFont(font);
	menu[8].setString("Convex Hull");
	menu[8].setCharacterSize(20);
	menu[8].setFillColor(sf::Color::White);
	menu[8].setStyle(sf::Text::Bold);
	menu[8].setPosition(810, 255);

	menu[9].setFont(font);
	menu[9].setString("2 opt (Remove Overlap)");
	menu[9].setCharacterSize(20);
	menu[9].setFillColor(sf::Color::White);
	menu[9].setStyle(sf::Text::Bold);
	menu[9].setPosition(810, 280);
	
	menu[10].setFont(font);
	menu[10].setString("Simulated Annealing");
	menu[10].setCharacterSize(20);
	menu[10].setFillColor(sf::Color::White);
	menu[10].setStyle(sf::Text::Bold);
	menu[10].setPosition(810, 305);

	menu[11].setFont(font);
	menu[11].setString("Genetic Algorithm");
	menu[11].setCharacterSize(20);
	menu[11].setFillColor(sf::Color::White);
	menu[11].setStyle(sf::Text::Bold);
	menu[11].setPosition(810, 330);

	menu[12].setFont(font);
	menu[12].setString("Nearest Insertion");
	menu[12].setCharacterSize(20);
	menu[12].setFillColor(sf::Color::White);
	menu[12].setStyle(sf::Text::Bold);
	menu[12].setPosition(810, 355);

	menu[13].setFont(font);
	menu[13].setString("Christofides Algorithm");
	menu[13].setCharacterSize(20);
	menu[13].setFillColor(sf::Color::White);
	menu[13].setStyle(sf::Text::Bold);
	menu[13].setPosition(810, 380);

	selectedItemIndex = 0;

	topRectangle.setSize(sf::Vector2f(400, 412));
	topRectangle.setOutlineColor(sf::Color(255, 0, 0));
	topRectangle.setOutlineThickness(-5);
	topRectangle.setFillColor(sf::Color(150, 150, 150));
	topRectangle.setPosition(800, 0);

	bottomRectangle.setSize(sf::Vector2f(400, 188));
	bottomRectangle.setOutlineColor(sf::Color(200, 200, 200));
	bottomRectangle.setOutlineThickness(-5);
	bottomRectangle.setFillColor(sf::Color(150, 150, 150));
	bottomRectangle.setPosition(800, 412);
}

void Menu::draw(sf::RenderWindow& window)
{
	window.draw(topRectangle);
	window.draw(bottomRectangle);

	for (int i = 0; i < ITEM_COUNT; i++)
	{
		window.draw(menu[i]);
	}

	window.draw(label[0]);
	window.draw(label[1]);
}

void Menu::MoveUp()
{
	if (selectedItemIndex - 1 >= 0)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(sf::Color::Green);
	}
}

void Menu::MoveDown()
{
	if (selectedItemIndex + 1 < ITEM_COUNT)
	{
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(sf::Color::Green);
	}
}