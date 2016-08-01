//============================================================================
// Name        : Durak.cpp
// Author      : Kenneth Sanders
// Version     :
// Copyright   : Copyright Kenneth Sanders
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
const char* stringValues[] = {
  "two",  "three", "four", "five",  "six",
  "seven", "eight", "nine", "ten",   "jack", "queen", "king", "ace"
  };
const char* intStringValues[] = {
  "2",  "3", "4", "5",  "6",
  "7", "8", "9", "10",   "jack", "queen", "king", "ace"
  };
const char* stringSuits[] =
  {
  "spades", "hearts", "clubs", "diamonds"
  };
class Card : public sf::Drawable{
public:
	enum Value{
		Two = 0,
		Three = 1,
		Four = 2,
		Five = 3,
		Six = 4,
		Seven = 5,
		Eight = 6,
		Nine = 7,
		Ten = 8,
		Jack = 9,
		Queen = 10,
		King = 11,
		Ace = 12
	};
	enum Suit{
		Spade = 0,
		Heart = 1,
		Club = 2,
		Diamond = 3
	};
	Card(Value v, Suit s){
			value = v;
			suit  = s;
			const std::string str(std::string("Cards/").append(intStringValues[static_cast<int>(value)]).append("_of_").
							append(stringSuits[static_cast<int>(suit)]).append(".png"));
			std::cout << str <<std::endl;
			if(!face.loadFromFile(str)){//TODO error
				std::cout << "Couldn't load card img" << std::endl;
			}
			else{
				sprite.setTexture(face);
				std::cout << "Left: " << sprite.getLocalBounds().left << ", width: " << sprite.getLocalBounds().width << ", top: " << sprite.getLocalBounds().top << ", height: " << sprite.getLocalBounds().height << std::endl;
			}
		}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	    {
	        // You can draw other high-level objects
	        target.draw(sprite, states);
	        // ... or use the low-level API
	        //states.texture = &m_texture;
	        //target.draw(m_vertices, states);
	        // ... or draw with OpenGL directly
	        //glBegin(GL_QUADS);
	        //...
	        //glEnd();
	    }
private:
	Value value;
	Suit suit;
	sf::Texture face;
	const sf::Texture back;
	sf::Sprite sprite;
	bool faceup;

};



int main()
{
	Card c(Card::Seven, Card::Spade);
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
        window.clear();
        window.draw(c);
        // Draw the sprite
      //  window.draw(sprite);
        // Draw the string
        //window.draw(text);
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}
