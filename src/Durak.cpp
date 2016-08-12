//============================================================================
// Name        : Durak.cpp
// Author      : Kenneth Sandbum
// Version     :
// Copyright   : Copyright Kenneth Sandbum
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <bitset>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

sf::Texture back;

const char *stringValues[] = {"two",   "three", "four", "five", "six",
                              "seven", "eight", "nine", "ten",  "jack",
                              "queen", "king",  "ace"};
const char *intStringValues[] = {"2", "3",  "4",    "5",     "6",    "7",  "8",
                                 "9", "10", "jack", "queen", "king", "ace"};
const char *stringSuits[] = {"spades", "hearts", "clubs", "diamonds"};

class Card : public sf::Drawable {
public:
  enum Value {
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
  enum Suit {
      Spade = 0,
      Heart = 1,
      Club = 2,
      Diamond = 3 };
  Card(Value v, Suit s, bool FaceUp) {
    faceup = FaceUp;
    value = v;
    suit = s;
    const std::string str(std::string("Cards/")
                              .append(intStringValues[static_cast<int>(value)])
                              .append("_of_")
                              .append(stringSuits[static_cast<int>(suit)])
                              .append(".png"));
    std::cout << str << ", value =" <<  static_cast<int>(value)<< ", suit = " <<  static_cast<int>(suit) << std::endl;
    if (!face.loadFromFile(str)) { // TODO error
      std::cout << "Couldn't load card img" << std::endl;
    } else {
	updateTexture();
      std::cout << "Left: " << sprite.getLocalBounds().left
                << ", width: " << sprite.getLocalBounds().width
                << ", top: " << sprite.getLocalBounds().top
                << ", height: " << sprite.getLocalBounds().height << std::endl;
    }
  }
    Card(const Card &c):face(c.face), sprite(c.sprite) {
    faceup = c.faceup;
    value = c.value;
    suit = c.suit;
    updateTexture();
    std::cout << "Left: " << sprite.getLocalBounds().left
                << ", width: " << sprite.getLocalBounds().width
                << ", top: " << sprite.getLocalBounds().top
                << ", height: " << sprite.getLocalBounds().height << std::endl;

  }
    
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // You can draw other high-level objects
    target.draw(sprite, states);
    // ... or use the low-level API
    // states.texture = &m_texture;
    // target.draw(m_vertices, states);
    // ... or draw with OpenGL directly
    // glBegin(GL_QUADS);
    //...
    // glEnd();
  }
    void updateTexture(){
	if (faceup) {
	    face.setSmooth(true);
	    sprite.setTexture(face);
	    std::cout << "loaded Face, ";
	}
	else {
	    back.setSmooth(true);
	    sprite.setTexture(back);
	    std::cout << "loaded Back, ";
	}	
    }
  void scale(int X, int Y) {
      std::cout << "Scaling:" << X / sprite.getLocalBounds().width <<
	  ", " << Y / sprite.getLocalBounds().height 
          << std::endl;
      sprite.scale(1 / sprite.getScale().x, 1 / sprite.getScale().y);
      sprite.scale((X / sprite.getLocalBounds().width),(Y / sprite.getLocalBounds().height));
      sprite.setOrigin(sprite.getLocalBounds().width / 2,
		       sprite.getLocalBounds().height / 2);
  }
  void rotate(float deg) { sprite.setRotation(sprite.getRotation() + deg); }
  void setPos(float x, float y) { sprite.setPosition(x, y); }
  sf::FloatRect getBounds() { return sprite.getLocalBounds(); }
  bool isFaceUp() { return faceup; }
  void Flip() {
    faceup = !faceup;
    updateTexture();
  }
  void setFaceUp(bool faceUp) {
    faceup = faceUp;
    updateTexture();
  }
  Suit getSuit() { return suit; }
  Value getValue() { return value; }
  sf::Vector2f getScale(){return sprite.getScale();}

private:
  Value value;
  Suit suit;
  sf::Texture face;
  sf::Sprite sprite;
  bool faceup;
};

class CardCompare{
    Card *k;
public:
    CardCompare(Card& c){
	k = new Card(c);
    }
    bool operator()(Card* i, Card* j){
	/*if(i->getSuit() == k->getSuit() && j->getSuit() != k->getSuit()){
	    return false;
	}
	else if(i->getSuit() != k->getSuit() && j->getSuit() == k->getSuit()){
	    return true;
	}
	else if (i->getSuit() == j->getSuit()){*/
	    


	
	if(static_cast<int>((i->getSuit())) == static_cast<int>(k->getSuit()) && static_cast<int>(j->getSuit()) != static_cast<int>(k->getSuit())){
	    return false;
	}
	if(static_cast<int>(i->getSuit()) != static_cast<int>(k->getSuit()) && static_cast<int>(j->getSuit()) == static_cast<int>(k->getSuit())){
	    return true;
	}
	/*if(static_cast<int>(i->getSuit()) == static_cast<int>(k->getSuit()) && static_cast<int>(j->getSuit()) == static_cast<int>(k->getSuit())){
	    return static_cast<int>(i->getValue()) < static_cast<int>(j->getValue());
	    }*/
	else{
	    if(static_cast<int>(i->getSuit()) == static_cast<int>(j->getSuit())){
		return static_cast<int>(i->getValue()) < static_cast<int>(j->getValue());
	    }
	    else{
		return static_cast<int>(i->getSuit()) < static_cast<int>(j->getSuit());

	    }
	}
    }
};


class Hand : public sf::Drawable {
public:
    enum Direction{left, right, up, down};
private:
    std::vector<Card*> Cards;
    sf::FloatRect area;
    bool horizontal;
    bool visible;
    Direction direction;
    Card kozer;
public:
    Hand(bool Visible, bool Horizontal, sf::FloatRect Area, Direction d, Card Kozer) : area(Area), direction(d), kozer(Kozer){
	horizontal = Horizontal;
	visible = Visible;
    }
    void setGrowth(Direction d){
	direction = d;
    }
    void updatePositions(){
	
	if(Cards.size() == 0)
	    return;
	if(!horizontal){
	    bool layers = area.height / Cards.size() > area.width * 0.8f;
	    std::cout << "area.height / Cards.size() = " << area.height / Cards.size() << ",  area.width * 0.8f = " <<  area.width * 0.8f << ", area.width = " << area.width << std::cout; 
	    int k = 0;
	    int j = 0;
	    for(std::vector<Card*>::iterator i = Cards.begin(); i != Cards.end(); i++){
		if(layers){
		    int mult = 0;
		    if(direction == left)
			mult = -1;
		    if(direction == right)
			mult = 1;
		    k = mult * static_cast<int>(j / (area.height / area.width*0.8f));
		}
		std::cout << "k = " << k <<std::endl;
		(*i)->setPos(area.left + area.width / 2 + k * area.width, area.top + area.height / (2 * Cards.size()) + j * area.height / Cards.size() );
		j++;
		    }
	}
	else{
	    bool layers = area.width / Cards.size() < area.height * 0.8f;
	    int k = 0;
	    int j = 0;
	    if(layers){
		    int mult = 0;
		    if(direction == up)
			mult = -1;
		    if(direction == down)
			mult = 1;
		    k = mult * static_cast<int>(j / (area.width / area.height*0.8f));
		    std::cout << "k = " << k <<std::endl;
		}
	    for(std::vector<Card*>::iterator i = Cards.begin(); i != Cards.end(); i++){
		(*i)->setPos(area.left + area.width / (2 * Cards.size()) + j * area.width / Cards.size(), area.top + area.height / 2 +  k * area.height);
		j++;
		    }
	}
    }
    void scale(sf::FloatRect Area) {
	if(horizontal){
	    int w = area.height * 0.8f;
/*
	    if(area.width / Cards.size() < Area.height * 0.8f){
		w =  Area.height * 0.8f;
	    }
	    else{
		w = area.width / Cards.size();
	    }
*/
	    for (std::vector<Card*>::iterator i = Cards.begin(); i != Cards.end(); i++) {
		(*i)->scale(w, Area.height);
	    }
	}
	else{
	    int h = 0;
	    if(area.width / Cards.size() < Area.height * 0.8f){
		h =  Area.width * 0.8f;
	    }
	    else{
		h = area.height / Cards.size();
	    }
	    for (std::vector<Card*>::iterator i = Cards.begin(); i != Cards.end(); i++) {
		(*i)->scale(Area.width, h);
	    }
	}
    }
    bool canBeat(Card attack, Card kozer) {
	for (std::vector<Card*>::iterator i = Cards.begin(); i != Cards.end(); i++) {
	    if ((*i)->getSuit() == attack.getSuit() && (*i)->getValue() > attack.getValue())
		return true;
	    if (attack.getSuit() != kozer.getSuit()) {
		if ((*i)->getSuit() == kozer.getSuit())
		    return true;
	    }
	}
	return false;
    }
    void takeCard(Card& c){
	Card * temp = new Card(c);
	Cards.push_back(temp);
	Cards.back()->rotate((horizontal?0.0f:90.0f));
	Cards.back()->setFaceUp(visible);
	std::sort(Cards.begin(), Cards.end(), CardCompare(kozer));
	scale(area);
	updatePositions();
    }
    //Card giveCard
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
	// You can draw other high-level objects
	for (std::vector<Card*>::const_iterator i = Cards.begin(); i != Cards.end();
	     i++) {
	    target.draw(**i, states);
	}
    }
    ~Hand(){
	for(std::vector<Card*>::const_iterator i = Cards.begin(); i != Cards.end(); i++){
		delete *i;
	    }
    }
};

    class Deck : public sf::Drawable {
	std::vector<Card> Cards;
	Card kozer;
	Card Back;
	
    public:
  Deck(bool under6)
      : kozer(Card::Ace, Card::Spade, true),
        Back(Card::Ace, Card::Spade, false) {

    srand(time(NULL));
    int numcards = (under6 ? 52 : 36);
    std::bitset<52> b;
    if (!under6) {
      for (int i = 36; i < 52; i++) {
        b.set(i);
      }
    }
    while (static_cast<int>(b.count()) < 52) {
      int i = rand() % numcards;
      if (!b[i]) {
        b.set(i);
	int v = 0;
        int s = 0;
	if(under6){
	    v = i % 13;
	    s = i / 13;
	}
	else{
	    v = i % 9 + 4;
	    s = i / 9;
	}
        Card temp(static_cast<Card::Value>(v), static_cast<Card::Suit>(s),
                  false);
        Cards.push_back(temp);
      }
    }
    kozer = Card(Cards.front());
    kozer.setFaceUp(true);
    
    //kozer.rotate(90.0f);
  }
  Card getKozer(){
      return kozer;
  }
  void scale(int WindowX, int WindowY) {
    for (std::vector<Card>::iterator i = Cards.begin(); i != Cards.end(); i++) {
      i->scale(WindowX / 15, WindowX / 15);
    }
    Back.scale(WindowX /15, WindowX /15);
    std::cout << "scaling kozer" << std::endl;
    kozer.scale(WindowX /15, WindowX/15);
  }
  void setPos(float x, float y) {
    for (std::vector<Card>::iterator i = Cards.begin(); i != Cards.end(); i++) {
      i->setPos(x, y);
    }
    Back.setPos(x, y);
    kozer.setPos(x + kozer.getBounds().width * kozer.getScale().x, y);
  }
  int size(){
      return Cards.size();
  }
  bool empty(){
      return Cards.empty();
  }
  Card takeTop() {
      if(Cards.size()==0){
	  return kozer;
      }
      Card temp(Cards.back());
      Cards.pop_back();
      return temp;
  }
  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const {
      // You can draw other high-level objects

      target.draw(kozer, states);
      if (Cards.size() > 0) {
	  target.draw(Back, states);
      }
  }
};
class Table : public sf::Drawable {
    private:
    std::vector<Card> attack;
    std::vector<Card> defend;
    Deck deck;
    std::vector<Hand> players;
public:
    Table(bool under6, int numOfPlayers, int WindowX, int WindowY): deck(under6){
	deck.scale(WindowX, WindowY);
    }
    
};

int main() {
  int WindowX, WindowY;

  std::string str("Cards/cardback1.png");

  std::cout << str << std::endl;

  if (!back.loadFromFile(str)) { // TODO error
    std::cout << "Couldn't load cardback img" << std::endl;
  }
  WindowX = 700;
  WindowY = 500;
  //Card c(Card::Seven, Card::Spade, true);
  //c.rotate(90.0f);
  Deck deck(true);
  Hand h(true, false, sf::FloatRect(100.0f, 100.0f, static_cast<float>(WindowX / 5), static_cast<float>(WindowY)), Hand::down, deck.getKozer());
  //h.takeCard(c);
  
  // Create the main window
  sf::VideoMode v = sf::VideoMode::getDesktopMode();

  sf::RenderWindow window(sf::VideoMode(v.width, v.height), "SFML window");
  WindowX = v.width;
  WindowY = v.height;
  //c.scale(WindowX, WindowY);
  //c.rotate(90.0f);
  //c.setPos(50.0f, 50.0f);
  deck.scale(WindowX, WindowY);
  deck.setPos(WindowX / 1.5, WindowY / 1.5);
  while (window.isOpen()) {
    // Process events
    sf::Event event;
    while (window.pollEvent(event)) {
      // Close window : exit
      if (event.type == sf::Event::Closed)
        window.close();
      if(event.type == sf::Event::KeyPressed){
	 
	  switch(event.key.code){
	  case sf::Keyboard::Space:
	      if(!deck.empty()){
		  Card temp = deck.takeTop();
		  h.takeCard(temp);
	      }
	      break;
	      
	  default:
	      break;
	  }
      }
    }
    // Clear screen
    window.clear(sf::Color(85, 120, 47, 160));
    window.draw(h);
    window.draw(deck);
    window.display();
  }
  return EXIT_SUCCESS;
}
