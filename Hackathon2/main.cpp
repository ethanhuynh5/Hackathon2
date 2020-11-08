#include "Header.h"
#include <iostream>
#include <string.h>
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <cctype>
#include <math.h>
#include <vector>
#include <fstream>
#include <numeric>
#include <chrono>
#define MAXN 100001
#define ll long long
sf::Image i0, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10;
sf::Sprite s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10;
sf::Texture t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
sf::Sprite sprites[11];
vector<vector<Plant>> candidate;
//cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n'); 
struct Plant;
using namespace std;
int WIDTH = 800;
int HEIGHT = 800;
const int MAX_D = 30;
const int MIN_D = 5;
int ROW = 8;
int COL = 8;
string board[MAX_D][MAX_D];
//Plant plantArray[MAX_D][MAX_D];
vector<Plant> selectedPlants;
vector<Plant> plantList;
bool shades[MAX_D][MAX_D] = { false };
vector<sf::Color> colors;
vector<int> amounts(11);
vector<vector<int>> randColors;
vector<vector<Plant>> allPerms;
vector<Plant> largeArray;
void incrementColor(sf::Color& c);

void selectDimensions();

void printSDBoard(sf::RenderWindow& window);

void displayDimensions(sf::RenderWindow& window, int r, int c);

void mainScreen();

void heapPermutation(Plant* a, int size, int n);

struct button {
	//parameters for now
	button(){
	}
	button(int xCoord, int yCoord, int width, int height, sf::Color c, sf::Color hov) {
		x = xCoord;
		y = yCoord;
		w = width;
		h = height;
		color = c;
		hover = hov;
		currentColor = c;
		rect = sf::RectangleShape(sf::Vector2f(w, h));
	}
	bool clicked(int clickX, int clickY) {
		return (x <= clickX && clickX <= x + w) && (y <= clickY && clickY <= y + h);
	}
	void hoverButton(int mouseX, int mouseY) {
		if ((x <= mouseX && mouseX <= x + w) && (y <= mouseY && mouseY <= y + h)) {
			currentColor = hover;
		}
		else {
			currentColor = color;
		}

	}
	void display(sf::RenderWindow& window) {
		rect.setFillColor(currentColor);
		rect.setPosition(x, y);
		window.draw(rect);
	}
	
	int getX() const{
		return x;
	}
	int getY() const{
		return y;
	}
	/*void setConnectedPlant(Plant p) {
		connectedPlant = p;
	}
	Plant getConnectedPlant() const {
		return connectedPlant;
	}*/
private:
	int x, y, w, h;
	sf::RectangleShape rect;
	sf::Color color, hover, currentColor;
	//Plant connectedPlant;
};
struct incButton {
	//parameters for now
	incButton();
	incButton(int xCoord, int yCoord, int width, int height, sf::Color c, sf::Color hov) {
		x = xCoord;
		y = yCoord;
		w = width;
		h = height;
		color = c;
		hover = hov;
		currentColor = c;
		rect = sf::RectangleShape(sf::Vector2f(w, h));
	}
	bool clicked(int clickX, int clickY) {
		return (x <= clickX && clickX <= x + w) && (y <= clickY && clickY <= y + h);
	}
	int getX() {
		return x;
	}
	int getY() {
		return y;
	}
	void hoverButton(int mouseX, int mouseY) {
		if ((x <= mouseX && mouseX <= x + w) && (y <= mouseY && mouseY <= y + h)) {
			currentColor = hover;
		}
		else {
			currentColor = color;
		}

	}
	void display(sf::RenderWindow& window) {
		rect.setFillColor(currentColor);
		rect.setOutlineColor(hover);
		rect.setOutlineThickness(5);
		rect.setPosition(x, y);
		window.draw(rect);
	}
	//void setConnectedButton(button b) {
	//	connectedButton = b;
	//}
	//void setType(bool b) {
	//	type = b;
	//}
	//button getConnectedButton() const {
	//	return connectedButton;
	//}
private:
	int x, y, w, h;
	//Plant connectedPlant;
	button connectedButton;
	sf::RectangleShape rect;
	sf::Color color, hover, currentColor;
	bool type;
};
struct Plant {

    public:
        Plant();
        Plant(string name, double spacing, double sunValue = 0) {
            this -> name = name;
            this -> spacing = spacing;
			this -> sunValue = sunValue;
        }

        string getName() const {
            return name;
        }

		double getSpacing() const {
			return spacing;
		}

		double getSunValue() const {
			return sunValue;
		}
		void setNumber(int num){
			number = num;
		}
		int getNumber() {
			return number;
		}
    private:
        string name;
        double spacing;
		double sunValue; //higher the value is, the more the plant depends on sun  
		int number;
};

Plant::Plant() {
    name = "";
    spacing = 0;
	sunValue = 0;
}
Plant plantArray[MAX_D][MAX_D];

//misc. methods
void incrementColor(sf::Color& c) {
	vector<int> col;
	col.push_back(c.r);
	col.push_back(c.g);
	col.push_back(c.b);
	int maxColor = *max_element(col.begin(), col.end());
	int minColor = *min_element(col.begin(), col.end());;
	if (col[0] == maxColor) {
		if (col[2] != minColor) {
			--col[2];
		}
		else if (col[1] == maxColor) {
			--col[0];
		}
		else {
			++col[1];
		}
	}
	else if (col[1] == maxColor) {
		if (col[0] != minColor) {
			--col[0];
		}
		else if (col[2] == maxColor) {
			--col[1];
		}
		else {
			++col[2];
		}
	}
	else if (col[2] == maxColor) {
		if (col[1] != minColor) {
			--col[1];
		}
		else if (col[0] == maxColor) {
			--col[2];
		}
		else {
			++col[0];
		}
	}
	c.r = col[0];
	c.g = col[1];
	c.b = col[2];

}
//select area of farm
void startingScreen() {
	sf::Image im;
	im.loadFromFile("images/plantBackground.jpg");
	sf::Texture tex;
	tex.loadFromImage(im);
	sf::Sprite spr;
	spr.setTexture(tex);
	spr.setPosition(0, 0);
	sf::RenderWindow window(sf::VideoMode(1200, 675), "Smart Planter AI");
	//1200*675
	

	sf::Color white = sf::Color(255, 255, 255, 200);
	sf::Color black = sf::Color(170, 170, 170, 255);
	sf::Color green = sf::Color(242, 192, 70, 255);
	sf::Color yellowHover = sf::Color(96, 125, 252, 255);
	sf::Color yellow = sf::Color(96, 125, 252, 155);
	button startButton(1000,500,150,75, yellow, yellowHover);
	//plantArray 

	sf::Text text;
	text.setString(std::string("START"));
	text.setCharacterSize(50);
	text.setFillColor(white);
	text.setStyle(sf::Text::Bold);
	
	sf::Text title;
	title.setString(std::string("Smart-Planter AI"));
	title.setCharacterSize(100);
	title.setFillColor(black);
	title.setStyle(sf::Text::Bold);
	sf::Font font;
	if (!font.loadFromFile("fonts/ostrich-regular.ttf"))
	{
		cout << "FONT FAILED";
	}
	text.setFont(font);
	text.setPosition(startButton.getX()+20, startButton.getY()+5);

	title.setFont(font);
	title.setPosition(50, 100);
	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			int mouseX = sf::Mouse::getPosition(window).x;
			int mouseY = sf::Mouse::getPosition(window).y;
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed) {
				if (startButton.clicked(mouseX, mouseY)) {
					window.close();
				}
			}
			startButton.hoverButton(mouseX, mouseY);

			
		}
		window.clear();
		window.draw(spr);
		startButton.display(window);
		window.draw(text);
		window.draw(title);
		window.display();

	}
}
void selectDimensions()
{
	sf::RenderWindow selectDimensions(sf::VideoMode(WIDTH, HEIGHT), "Select Dimensions");

	sf::Text text;

	sf::Color black(0, 0, 0, 255);

	text.setString(std::string("SELECT AREA"));

	text.setCharacterSize(50);

	text.setFillColor(black);


	//text.setScale(sf::Vector2f(1.0 / text.getLocalBounds().width * textScaleX, 1.0 / text.getLocalBounds().height) * textScaleY);

	text.setStyle(sf::Text::Bold);

	sf::Font font;
	if (!font.loadFromFile("fonts/ostrich-regular.ttf"))
	{
		cout << "font faled";
		return;
	}

	text.setFont(font);
	text.setPosition(WIDTH / 2 - 50, 50);

	int prevR = 0;
	int prevC = 0;
	while (selectDimensions.isOpen())
	{
		sf::Event event;
		printSDBoard(selectDimensions);
		while (selectDimensions.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				selectDimensions.close();


			int mouseX = sf::Mouse::getPosition(selectDimensions).x;
			int mouseY = sf::Mouse::getPosition(selectDimensions).y;


			int c = (int)floor((((double)(mouseX) / (double)(WIDTH)) * (double)MAX_D));
			int r = (int)floor((((double)(mouseY) / (double)(HEIGHT)) * (double)MAX_D));

			if (r < 5 || c < 5)
			{
				text.setString(" " + to_string(c + 1) + " X " + to_string(r + 1) + "\nINVALID");
			}
			else
			{
				text.setString(" " + to_string(c + 1) + " X " + to_string(r + 1));
			}

			if (r != prevR || c != prevC)
			{
				selectDimensions.clear();
				printSDBoard(selectDimensions);
				displayDimensions(selectDimensions, r, c);
				selectDimensions.draw(text);
				selectDimensions.display();
				prevR = r;
				prevC = c;
			}



			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (r >= MIN_D - 1 && c >= MIN_D - 1)
				{
					ROW = r + 1;
					COL = c + 1;
					if (ROW < COL)
					{
						HEIGHT = (int)round((double)(ROW) * (double)(HEIGHT) / (double)(COL));
					}
					if (ROW > COL)
					{
						WIDTH = (int)round((double)(COL) * (double)(WIDTH) / (double)(ROW));
					}
					selectDimensions.clear();
					selectDimensions.close();
				}
			}
		}


	}
}
void printSDBoard(sf::RenderWindow& window)
{

	for (int row = 0; row < MAX_D; row++)
	{
		for (int col = 0; col < MAX_D; col++)
		{
			//colors for "board" : (139,227,112), (231,225,73)
			//sf::Color white = sf::Color(255, 255, 255, 200);
			//sf::Color black = sf::Color(170, 170, 170, 200);
			//sf::Color green = sf::Color(242, 192, 70, 255);
			//sf::Color yellow = sf::Color(96, 125, 252, 255);

			sf::RectangleShape rect(sf::Vector2f((double)WIDTH / MAX_D, (double)HEIGHT / MAX_D));
			////sf::RectangleShape rect(sf::Vector2f((double)WIDTH / COL, (double)HEIGHT / ROW));
			//if ((row + col) % 2 == 0)
			//{
			//	rect.setFillColor(green);
			//}
			//else
			//{
			//	rect.setFillColor(yellow);
			//}
			rect.setFillColor(colors[randColors[row][col]]);

			int xCoord = (int)(col / (double)MAX_D * WIDTH);
			int yCoord = (int)(row / (double)MAX_D * HEIGHT);
			rect.setPosition(xCoord, yCoord);
			window.draw(rect);
		}
	}
}
void loadImage() {
	/*
	plantList.push_back(appleTree);
	plantList.push_back(peachTree);
	plantList.push_back(pearTree);
	plantList.push_back(plumTree);
	plantList.push_back(cherryTree);
	plantList.push_back(citrusTree);
	plantList.push_back(brocolli);
	plantList.push_back(cabbage);
	plantList.push_back(corn);
	plantList.push_back(pumpkin);
	plantList.push_back(zucchini);
	*/
	i0.loadFromFile("images/apple.png");
	i1.loadFromFile("images/peach.png");
	i2.loadFromFile("images/pear.png");
	i3.loadFromFile("images/plum.png");
	i4.loadFromFile("images/cherry.png");
	i5.loadFromFile("images/citrus.png");
	i6.loadFromFile("images/brocolli.png");
	i7.loadFromFile("images/cabbage.png");
	i8.loadFromFile("images/corn.png");
	i9.loadFromFile("images/pumpkin.png");
	i10.loadFromFile("images/zucchini.png");
	double pieceScalingX = 1.0 / 60.0 * (94.2 / (100.0 * COL)) * (double)WIDTH;
	double pieceScalingY = 1.0 / 60.0 * (94.2 / (100.0 * ROW)) * (double)HEIGHT;
	pieceScalingX = pieceScalingY = 1.0;
	t0.loadFromImage(i0);
	s0.setTexture(t0);
	s0.setScale(sf::Vector2f(pieceScalingX, pieceScalingY));
	sprites[0] = s0;
	t1.loadFromImage(i1);
	s1.setTexture(t1);
	s1.setScale(sf::Vector2f(pieceScalingX, pieceScalingY));
	sprites[1] = s1;
	t2.loadFromImage(i2);
	s2.setTexture(t2);
	s2.setScale(sf::Vector2f(pieceScalingX, pieceScalingY));
	sprites[2] = s2;
	t3.loadFromImage(i3);
	s3.setTexture(t3);
	s3.setScale(sf::Vector2f(pieceScalingX, pieceScalingY));
	sprites[3] = s3;
	t4.loadFromImage(i4);
	s4.setTexture(t4);
	s4.setScale(sf::Vector2f(pieceScalingX, pieceScalingY));
	sprites[4] = s4;
	t5.loadFromImage(i5);
	s5.setTexture(t5);
	s5.setScale(sf::Vector2f(pieceScalingX, pieceScalingY));
	sprites[5] = s5;
	t6.loadFromImage(i6);
	s6.setTexture(t6);
	s6.setScale(sf::Vector2f(pieceScalingX, pieceScalingY));
	sprites[6] = s6;
	t7.loadFromImage(i7);
	s7.setTexture(t7);
	s7.setScale(sf::Vector2f(pieceScalingX, pieceScalingY));
	sprites[7] = s7;
	t8.loadFromImage(i8);
	s8.setTexture(t8);
	s8.setScale(sf::Vector2f(pieceScalingX, pieceScalingY));
	sprites[8] = s8;
	t9.loadFromImage(i9);
	s9.setTexture(t9);
	s9.setScale(sf::Vector2f(pieceScalingX, pieceScalingY));
	sprites[9] = s9;
	t10.loadFromImage(i10);
	s10.setTexture(t10);
	s10.setScale(sf::Vector2f(pieceScalingX, pieceScalingY));
	sprites[10] = s10;

}
void drawShade(bool shades[MAX_D][MAX_D],sf::RenderWindow& window) {
	sf::Color shade = sf::Color(0, 0, 0, 255);
	for (int row = 0; row < ROW; ++row) {
		for (int col = 0; col < COL; ++col) {
			if (shades[row][col]) {
				int xCoord = (int)(col / (double)COL * WIDTH);
				int yCoord = (int)(row / (double)ROW * HEIGHT);
				sf::RectangleShape rect(sf::Vector2f(WIDTH / COL, HEIGHT / ROW));
				rect.setFillColor(shade);
				rect.setPosition(xCoord, yCoord);
				window.draw(rect);
			}
		}
	}
}
void printBoard(sf::RenderWindow& window)
{
	//vector<sf::Color> cols;
	sf::Color white = sf::Color(220, 220, 220, 255);
	sf::Color black = sf::Color(150, 150, 150, 255);
	sf::Color brown = sf::Color(139, 69, 19);
	sf::RectangleShape bg(sf::Vector2f(WIDTH, HEIGHT));
	bg.setFillColor(brown);
	
	for (int row = 0; row < ROW; row++)
	{
		for (int col = 0; col < COL; col++)
		{

			sf::RectangleShape rect(sf::Vector2f(WIDTH / COL, HEIGHT / ROW));
			/*if ((row + col) % 2 == 0)
			{
				rect.setFillColor(white);
			}
			else
			{
				rect.setFillColor(black);
			}*/
			rect.setFillColor(colors[randColors[row][col]]);
			int xCoord = (int)(col / (double)COL * WIDTH);
			int yCoord = (int)(row / (double)ROW * HEIGHT);
			rect.setPosition(xCoord, yCoord);
			window.draw(rect);
		}
	}
}
void displayDimensions(sf::RenderWindow& window, int r, int c)
{
	int opacity = 100;
	sf::Color green = sf::Color(0, 255, 0, opacity);
	sf::Color red = sf::Color(255, 0, 0, opacity);
	sf::Color yellow = sf::Color(255, 255, 0, opacity);


	int thickness = 5;
	sf::RectangleShape rect(sf::Vector2f((double)WIDTH / MAX_D * (c + 1) - 2.0 * thickness, (double)HEIGHT / MAX_D * (r + 1) - 2.0 * thickness));
	rect.setOutlineThickness(thickness);


	if (c < MIN_D - 1 && r < MIN_D - 1)
	{
		rect.setFillColor(red);
		rect.setOutlineColor(sf::Color(255, 0, 0, 255));
	}
	else
	{
		rect.setFillColor(green);
		rect.setOutlineColor(sf::Color(0, 255, 100, 255));
	}

	rect.setPosition(thickness, thickness);
	window.draw(rect);

}
//select plants
void selectShade() {
	sf::RenderWindow selectShade(sf::VideoMode(WIDTH, HEIGHT), "Select Shade");
	sf::Text text;
	sf::Color black(0, 0, 0, 255);
	text.setString(std::string("SELECT SHADED AREAS"));
	text.setCharacterSize(50);
	text.setFillColor(black);
	
	
	//text.setScale(sf::Vector2f(1.0 / text.getLocalBounds().width * textScaleX, 1.0 / text.getLocalBounds().height) * textScaleY);
	//
	text.setStyle(sf::Text::Bold);
	sf::Font font;
	if (!font.loadFromFile("fonts/ostrich-regular.ttf"))
	{
		cout << "font faled";
		return;
	}

	text.setFont(font);
	text.setPosition(WIDTH / 2 - 50, 50);
	sf::Color red(255, 0, 0, 150);
	sf::Color redHover(255, 0, 0, 255);
	button exit{ 0,0,50,20,red,redHover };
	while (selectShade.isOpen())
	{
		selectShade.clear();
		printBoard(selectShade);
		drawShade(shades, selectShade);
		exit.display(selectShade);
		selectShade.display();
		
		sf::Event event;
		while (selectShade.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				selectShade.close();


			if (event.type == sf::Event::MouseButtonPressed)
			{
				
				int clickX = sf::Mouse::getPosition(selectShade).x;
				int clickY = sf::Mouse::getPosition(selectShade).y;
				if (exit.clicked(clickX, clickY)) {
					selectShade.close();
					break;
				}
				int c = (int)floor((((double)(clickX) / (double)(WIDTH)) * COL));
				int r = (int)floor((((double)(clickY) / (double)(HEIGHT)) * ROW));
				shades[r][c] = !shades[r][c];
				selectShade.clear();
				printBoard(selectShade);
				drawShade(shades, selectShade);
				//exit.display(selectShade);
				selectShade.display();
			}
		}
	}
}
void selectPlants() {
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SELECT PLANT");
	sf::Color white(255, 255, 255, 150);
	sf::Color black(0, 0, 0, 150);
	sf::Color hover(0, 0, 0, 255);
	sf::Color yellow(255, 255, 0, 255);
	sf::Color red(255, 0, 0, 150);
	sf::Color redHover(255, 0, 0, 255);
	sf::Color green(0, 255, 0, 255);
	sf::Color whiteHover(255, 255, 255, 255);
	int buttonWidth = 60;
	int buttonHeight = 60;
	vector<button> buttons;
	vector<incButton> decButtons;
	vector<incButton> incButtons;
	int x = 200;
	int y = 10;

	
	

	for (int i = 0; i < 6; i++) {
		button b = { x,y,buttonWidth,buttonHeight,yellow,yellow };
		
		
		buttons.push_back(b);
		incButton dec = { x - 100,y,buttonWidth, buttonHeight, red, yellow };
		incButton inc = { x + 100,y,buttonWidth, buttonHeight, green, yellow };
		decButtons.push_back(dec);
		incButtons.push_back(inc);
		y += 100;
	}
	x = 600;
	y = 10;
	for (int i = 0; i < 5; i++) {
		button b = { x,y,buttonWidth,buttonHeight,yellow,yellow };
		
		buttons.push_back(b);
		incButton dec = { x - 100,y,buttonWidth, buttonHeight, red, yellow };
		incButton inc = { x + 100,y,buttonWidth, buttonHeight, green, yellow };
		decButtons.push_back(dec);
		incButtons.push_back(inc);
		y += 100;
	}
	sf::Font font;
	if (!font.loadFromFile("fonts/ostrich-regular.ttf"))
	{
		cout << "FONT FAILED";
	}
	button exitButton = { 700, 700, buttonWidth, buttonHeight, red,redHover };
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event))
		{
			int mouseX = sf::Mouse::getPosition(window).x;
			int mouseY = sf::Mouse::getPosition(window).y;
			exitButton.hoverButton(mouseX,mouseY);
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				int clickX = sf::Mouse::getPosition(window).x;
				int clickY = sf::Mouse::getPosition(window).y;
				if(exitButton.clicked(clickX, clickY)){
					window.close();
					//exit
				}
				for (int i = 0; i < buttons.size(); i++) {
					incButton dec = decButtons.at(i);
					incButton inc = incButtons.at(i);
					if (dec.clicked(clickX, clickY)) {
						if(amounts[i]>0)
						--amounts[i];
						for (int o = 0; o < amounts.size(); ++o) {
							cout << amounts[o] << " ";
						}
						cout << endl;
						break;
					}
					if (inc.clicked(clickX, clickY)) {
						++amounts[i];
						for (int o = 0; o < amounts.size(); ++o) {
							cout << amounts[o] << " ";
						}
						cout << endl;
						break;
					}

				}
				
			}
			//b.hoverButton(mouseX, mouseY);
		}

		window.clear();
		//window.draw(shape);
		exitButton.display(window);
		for (int i = 0; i < buttons.size(); i++) {

			button b = buttons.at(i);
			b.display(window);
			incButton dec = decButtons.at(i);
			incButton inc = incButtons.at(i);
			dec.display(window);
			inc.display(window);
			sprites[i].setPosition(b.getX(), b.getY());
			window.draw(sprites[i]);
		//	window.draw(texts[i]);
		}
		//
		for (int i = 0; i < 11; ++i) {
			sf::Text text;
			//text.setString(to_string(amounts[i]));
			if (amounts[i] > 0) {
				text.setString("-		  " + to_string(amounts[i]));
			}
			else {
				text.setString("-		  +");
			}
			text.setCharacterSize(75);
			text.setFillColor(sf::Color(255,255,255,255));
			text.setStyle(sf::Text::Bold);
			text.setFont(font);
			text.setPosition(decButtons[i].getX()+19, decButtons[i].getY()-18);
			window.draw(text);
		}
		sf::Text X;
		X.setString("X");
		
		X.setCharacterSize(75);
		X.setFillColor(sf::Color(255, 255, 255, 255));
		X.setStyle(sf::Text::Bold);
		X.setFont(font);
		X.setPosition(exitButton.getX() + 19, exitButton.getY() - 18);
		window.draw(X);
		window.display();
	}


}
void printPlants(sf::RenderWindow& window) {
	double pieceScalingX = 1.0 / 60.0 * (94.2 / (100.0 * COL)) * (double)WIDTH;
	double pieceScalingY = 1.0 / 60.0 * (94.2 / (100.0 * ROW)) * (double)HEIGHT;
	for (int i = 0; i < 11; ++i) {
		sprites[i].setScale(sf::Vector2f(pieceScalingX, pieceScalingY));
	}
	for (int row = 0; row < 15; row++) {
		for(int col=0; col<15; col++){
			Plant p = plantArray[row][col];
			if (p.getSpacing() != 0) {
				int xCoord = (int)(col / (double)COL * WIDTH);
				int yCoord = (int)(row / (double)ROW * HEIGHT);
				Plant appleTree("Apple Tree", 30);
	Plant peachTree("Peach Tree", 20);
	Plant pearTree("Pear Tree", 20);
	Plant plumTree("Plum Tree", 20);
	Plant cherryTree("Cherry Tree", 15);
	Plant citrusTree("Citrus Tree", 8);
	Plant brocolli("Brocolli", 2);
	Plant cabbage("Cabbage", 1);
	Plant corn("Corn", 1);
	Plant pumpkin("Pumpkins", 5);
	Plant zucchini("Zucchini", 3);
	int index;
				if(p.getName()== "Apple Tree"){
					index = 0;
				}
				if (p.getName() == "Peach Tree") {
					index = 1;
				}
				if (p.getName() == "Pear Tree") {
					index = 2;
				}
				if (p.getName() == "Plum Tree") {
					index = 3;
				}
				if (p.getName() == "Cherry Tree") {
					index = 4;
				}
				if (p.getName() == "Citrus Tree") {
					index = 5;
				}
				if (p.getName() == "Brocolli") {
					index = 6;
				}
				if(p.getName()=="Cabbage"){
					index = 7;
				}
				
				if(p.getName()=="Corn"){
					index = 8;
				}
				
				if(p.getName()=="Pumpkins"){
					index = 9;
				}

				if(p.getName()=="Zucchini"){
					index = 10;
				}
				sprites[index].setPosition(xCoord, yCoord);
				window.draw(sprites[index]);
			}
			
			
		}
	}
}

void drawBoard() {
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Final Board");
	//plantArray 
	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		printBoard(window);
		drawShade(shades,window);
		printPlants(window);
		window.display();

	}
}

bool inBound(int r, int c) {
	return (r >= 0 && r <= ROW - 1) && (c >= 0 && c <= COL - 1);
}
int scanArea(int row, int col, vector<vector<Plant>> plants) {
	int tot = 0;
	for (int r = row - plants[row][col].getSpacing(); r <= row + plants[row][col].getSpacing(); ++r) {
		for (int c = col - plants[row][col].getSpacing(); c <= col + plants[row][col].getSpacing(); ++c) {
			if (!inBound(r, c)||(r==row&&c==col)) {
				continue;
			}
			if (plants[row][col].getSpacing() != 0) {
				//compute dist from plant
				tot -= max(abs(r - row), abs(c - col));
			}
		}
	}
	return tot;
}
int evaluatePosition(vector<vector<Plant>> plants) {
	int tot = 0;
	for (int r = 0; r < ROW; ++r) {
		for (int c = 0; c < COL; ++c) {
			if (plants[r][c].getSpacing() != 0) {
				if (!shades[r][c]) {
					tot += plants[r][c].getSunValue();
				}
				tot += scanArea(r, c,plants);
			}
		}
	}
	return tot;
}


//main screen is just testing, do not mess with
void mainScreen() {
	sf::RenderWindow window(sf::VideoMode(800, 800), "WINDOW");
	int red, green, blue;
	red = green = blue = 255;
	int t = 0;
	sf::Color white(255, 255, 255, 150);
	sf::Color black(0, 0, 0, 150);
	sf::Color hover(0, 0, 0, 255);
	sf::Color yellow(255, 255, 0, 255);
	sf::Color whiteHover(255, 255, 255, 255);
	//sf::Color background(255, 75, 75, 255);
	sf::Color background(0, 0, 0, 255);

	sf::RectangleShape rect(sf::Vector2f(800, 800));

	button b = { 100,50,50,50,yellow,black };
	auto start = chrono::steady_clock::now();

	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			int mouseX = sf::Mouse::getPosition(window).x;
			int mouseY = sf::Mouse::getPosition(window).y;
			b.hoverButton(mouseX, mouseY);
			if (event.type == sf::Event::MouseButtonPressed)
			{
				int clickX = sf::Mouse::getPosition(window).x;
				int clickY = sf::Mouse::getPosition(window).y;
				if (b.clicked(clickX, clickY)) {
					cout << "clicked";
				}
			}
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		auto end = chrono::steady_clock::now();
		double elapsed_time = double(chrono::duration_cast<chrono::nanoseconds> (end - start).count());
		if (elapsed_time >= 3e7) {
			//incrementColor(background);
			start = chrono::steady_clock::now();
		}
		rect.setFillColor(background);
		window.clear();
		window.draw(rect);
		//testGlow(window);
		b.display(window);
		window.display();
	}
}

void algorithm( int width, int height) {  
	//Plant plantArray[MAX_D][MAX_D];
	//sort selected plants by spacing, lowest to highest 
	int numPlants = selectedPlants.size();
	int i = selectedPlants.size() - 1;
	int plantNum = selectedPlants.at(i).getNumber();
	int currentSpacing = selectedPlants.at(i).getSpacing();
	int r = 0;
	int c = 0;
	while(numPlants > 0) {
        //cout << i << endl;
		if(shades[r][c] == true){
            if(c + 1 >= width){
				if(r + 1 >= height){
					r = r;
					c = c;
				}
				else{
					r++;
                	c=0;
				}
                
            }
            else{
                c++;
           }
            continue;
        }

		plantArray[r][c] = selectedPlants.at(i);
		if(c + currentSpacing + 1 >= width){
			if(r + currentSpacing + 1 >= height){
				r = height - 1;
			}
			else{
				r = r + currentSpacing + 1;
			}
			c = 0;
		}
		else{
			c = c + currentSpacing + 1;
		}
		plantNum--;
        //cout << plantNum << endl;
		if(plantNum == 0){
			numPlants--;
            if(numPlants <= 0){
                break;
            }
			i--;
			plantNum = selectedPlants.at(i).getNumber();
			currentSpacing = selectedPlants.at(i).getSpacing();
		}
        //cout << numPlants << endl;

	}

	
}

//void permutation(vector<int> v,int size,int n) {
//	if (size == 1) {
//		vector<int> temp;
//		for (int i = 0; i < temp.size(); ++i) {
//			cout << temp[i] << " ";
//		}
//		allPerms.push_back(temp);
//		return;
//	}
//	for (int i = 0; i < size; ++i) {
//		permutation(v, size - 1, n);
//		if (size % 2 == 1) {
//
//		}
//	}
//}
//void 

void swap(vector<Plant>& arr, int a, int b){
    Plant temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}
void heapPermutation(vector<Plant> a, int size, int n)
{
    // if size becomes 1 then prints the obtained
    // permutation
	cout << "size=" << size << endl;
    if (size == 1) {
		allPerms.push_back(a);
		for (int i = 0; i < a.size(); ++i) {
			cout << a[i].getName() << " ";
		}
		cout << endl;
        return;
    }
 
    for (int i = 0; i < size; i++) {
        heapPermutation(a, size - 1, n);
 
        
        if (size % 2 == 1){
		
            swap(a, 0, size - 1);
		}

        else{
            swap(a, i, size - 1);
		
		}
    }
}
void generateAllPermutations() {
	Plant arr[MAX_D][MAX_D];
	const int r = ROW;
	const int c = COL;
	const int length = r * c;
	Plant permutatedArray[30 * 30];
	
	int i = 0;
	int index = 0;
	for (int i = 0; i < amounts.size(); i++) {
		int num = amounts.at(i);
		Plant p = selectedPlants.at(i);
		for (int j = 0; j < num; j++) {
			largeArray.push_back(p);
			index++;
		}
	}
	for (int i = index; i < ROW * COL; i++) {
		Plant p("", 0);
		largeArray.push_back(p);
	}
	//for(int i=0; i<largeArray.size(); i++){
	//	cout << largeArray[i].getName() << " ";
	//}
	heapPermutation(largeArray, ROW*COL, ROW*COL);
}


void initialize() {
	srand(time(0));
	sf::Color b1(0, 204, 0, 255);
	sf::Color b2(0, 153, 0, 255);
	sf::Color b3(0, 175, 0, 255);
	sf::Color b4(0, 255, 0, 255);
	colors.push_back(b1);
	colors.push_back(b2);
	colors.push_back(b3);
	colors.push_back(b4);

	Plant appleTree("Apple Tree", 4);
	Plant peachTree("Peach Tree", 3);
	Plant pearTree("Pear Tree", 3);
	Plant plumTree("Plum Tree", 3);
	Plant cherryTree("Cherry Tree", 2);
	Plant citrusTree("Citrus Tree", 1);
	Plant brocolli("Brocolli", 1);
	Plant cabbage("Cabbage", 1);
	Plant corn("Corn", 1);
	Plant pumpkin("Pumpkins", 1);
	Plant zucchini("Zucchini", 1);
	appleTree.setNumber(0);
	peachTree.setNumber(1);
	pearTree.setNumber(2);
	plumTree.setNumber(3);
	cherryTree.setNumber(4);
	citrusTree.setNumber(5);
	brocolli.setNumber(6);
	cabbage.setNumber(7);
	corn.setNumber(8);
	pumpkin.setNumber(9);
	zucchini.setNumber(10);
	
	plantList.push_back(appleTree);
	plantList.push_back(peachTree);
	plantList.push_back(pearTree);
	plantList.push_back(plumTree);
	plantList.push_back(cherryTree);
	plantList.push_back(citrusTree);
	plantList.push_back(brocolli);
	plantList.push_back(cabbage);
	plantList.push_back(corn);
	plantList.push_back(pumpkin);
	plantList.push_back(zucchini);
	selectedPlants.push_back(appleTree);
	selectedPlants.push_back(peachTree);
	selectedPlants.push_back(pearTree);
	selectedPlants.push_back(plumTree);
	selectedPlants.push_back(cherryTree);
	selectedPlants.push_back(citrusTree);
	selectedPlants.push_back(brocolli);
	selectedPlants.push_back(cabbage);
	selectedPlants.push_back(corn);
	selectedPlants.push_back(pumpkin);
	selectedPlants.push_back(zucchini);
	
	for (int row = 0; row < MAX_D; ++row) {
		vector<int> temp;
		for (int col = 0; col < MAX_D; ++col) {
			temp.push_back(rand() % colors.size());
		}
		randColors.push_back(temp);
	}
	
}

void bestCombination() {
	//this method will fill up plantArray with the most optimal positions using min max
	int index = 0; //index of best combination
	int maxPoints = INT32_MIN;

	for (int i = 0; i < allPerms.size(); ++i) {
		if (evaluatePosition(candidate) > maxPoints) {
			for (int r = 0; r < ROW; ++r) {
				for (int c = 0; c < COL; ++c) {
					plantArray[r][c] = candidate[r][c];
				}
			}
			maxPoints = evaluatePosition(candidate);
		}
	}
}
int main() {
	
	initialize();
	loadImage();
	startingScreen();
	selectDimensions();
	selectShade();
	selectPlants();

	
	//printBoard();

	//example test cases
	//Plant p {};
	//get plant data from text file
	//vector<Plant> plantList;

	//set correct numbers for each selected plants using amounts array 
	
	
	//algorithm(ROW, COL);
	int width = 15;
	int height = 15;
	generateAllPermutations();
	bestCombination();
	//cout << "draw";
	drawBoard();
	for (int r = 0; r < ROW; ++r) {
		for (int c = 0; c < COL; ++c) {
			cout << plantArray[r][c].getName() << " ";
		}
		cout << endl;
	}

}


//no