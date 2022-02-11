//--- TERMINAL SNAKE (JACK BARNES 23-APR-2020) ----------------------------------//

#include <iostream>
#include <string>
#include <Windows.h>    //needed for the curser function and sleep
#include <stdlib.h>     // for srand, rand
#include <time.h>       // for time (generates seed for random number)
using namespace std;

//Declaration of Snake class variables and functions
class Snake
{
private:
	short xPos, yPos;							//xPos and yPos always refer to the position of the snake's head.
	short body = 0;								//body counts how many body parts the snake has
	string direction;							//Stores the current direction that the snake is heading in
	short snake_coordinates[526][2] = { 0 };	//2D array storing positions of the snake, where index 0 is the head and anything afterward is part of the body. col 0 = x, col 1 = y

public:
	Snake();									//Constructor

	void setXpos(short new_pos);				//controls x and y positions of snake head
	void setYpos(short new_pos);

	void updateBody();							//Increments the body variable
	short countBody();							//Returns the body variable

	void setDirection(string theWay);			//Sets direction of the snake
	string getDirection();						//Returns the direction

	short getXpos(short index);					//Returns position of any snake segment (delegated by index)
	short getYpos(short index);

	void updateBodyX(short index, short x);		//Updates positions of the snakes body segments
	void updateBodyY(short index, short y);
};

//Implementation of Snake class functions
Snake::Snake()
{
	//Constructor for Snake object
}

void Snake::setXpos(short new_pos)
{
	snake_coordinates[0][0] = new_pos;
	return;
}
void Snake::setYpos(short new_pos)
{
	snake_coordinates[0][1] = new_pos;
	return;
}

void Snake::updateBody()
{
	body++;
	return;
}
short Snake::countBody()
{
	return body;
}

void Snake::setDirection(string theWay)
{
	direction = theWay;
	return;
}
string Snake::getDirection()
{
	return direction;
}

short Snake::getXpos(short index)
{
	return snake_coordinates[index][0];
}
short Snake::getYpos(short index)
{
	return snake_coordinates[index][1];
}

void Snake::updateBodyX(short index, short x)
{
	snake_coordinates[index][0] = x;
	return;
}
void Snake::updateBodyY(short index, short y)
{
	snake_coordinates[index][1] = y;
	return;
}


//Function for setting the cursers position
void gotoxy(unsigned short x, unsigned short y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//Generates the fence of the game
void createFence(short height, short length)
{
	//top
	for (short i = 0; i < length; i++)
	{
		cout << "-";
	}
	//left
	cout << endl;
	for (short i = 0; i < height; i++)
	{
		cout << "|" << endl;
	}
	//right
	gotoxy(length, 0);
	for (short i = 0; i < height + 1; i++)
	{
		cout << "|";
		gotoxy(length, i + 1);
	}
	//bottom
	gotoxy(0, height);
	for (short i = 0; i < length; i++)
	{
		cout << "-";
	}
	//corners
	gotoxy(0, 0); cout << "*"; 				//top left
	gotoxy(length, 0); cout << "*"; 		//top right
	gotoxy(0, height); cout << "*"; 		//bottom left
	gotoxy(length, height); cout << "*"; 	//bottom right

	return;
}


int main()
{
	//Fence parameters
	short fenceHeight = 23;
	short fenceLength = 23;

	bool gameLoop = true; 			//the loop of the game
	short newPos = 0; 				//new position for head of the snake
	COORD oldHeadPos = { 0,0 };		//keeps an archive of the snake heads old positions for updating the snakes body

	//Apple details
	short appleXpos, appleYpos;
	bool appleGenerated = false;
	
	//Set the random seed
	srand(time(NULL));

	//Generate the fence
	createFence(fenceHeight, fenceLength);

	//Initialise snake, put in middle of the board
	Snake player;
	player.setXpos(fenceHeight / 2); player.setYpos(fenceLength / 2);
	gotoxy(player.getXpos(0), player.getYpos(0)); cout << "0";

	while (gameLoop == true)
	{
		//GENERATE AN APPLE
		if (appleGenerated == false) 						//if there is not already an apple...
		{
			appleXpos = rand() % (fenceLength - 1) + 1;		//Generate random apple coordinates
			appleYpos = rand() % (fenceHeight - 1) + 1;
			gotoxy(appleXpos, appleYpos);					//Print apple
			cout << "@";
			appleGenerated = true;
			gotoxy(player.getXpos(0) + 1, player.getYpos(0)); //Return curser to player
		}

		//MOVEMENT
	//Determines what direction the player wants, checks for keypresses
	//needs a clause for if the snake has a body it can't go backwards anymore
		if (GetKeyState('W') & 0x8000)
		{
			player.setDirection("up");
		}
		if (GetKeyState('S') & 0x8000)
		{
			player.setDirection("down");
		}
		if (GetKeyState('A') & 0x8000)
		{
			player.setDirection("left");
		}
		if (GetKeyState('D') & 0x8000)
		{
			player.setDirection("right");
		}

	//Snake head movement: Sets the snake in that direction.
		if (player.getDirection() == "up")
		{
			oldHeadPos = { player.getXpos(0), player.getYpos(0) };		//stores the old positions away
			player.setYpos(player.getYpos(0) - 1); 						//calculates new position
			cout << "\b "; 												//erases old head from the console
			gotoxy(player.getXpos(0), player.getYpos(0)); cout << "0"; 	//prints new position
		}
		if (player.getDirection() == "down")
		{
			oldHeadPos = { player.getXpos(0), player.getYpos(0) };		//stores the old positions away
			player.setYpos(player.getYpos(0) + 1); 						//calculates new position
			cout << "\b "; 												//erases old head from the console
			gotoxy(player.getXpos(0), player.getYpos(0)); cout << "0"; 	//prints new position
		}
		if (player.getDirection() == "left")
		{
			oldHeadPos = { player.getXpos(0), player.getYpos(0) };		//stores the old positions away
			player.setXpos(player.getXpos(0) - 1); 						//calculates new position
			cout << "\b "; 												//erases old head from the console
			gotoxy(player.getXpos(0), player.getYpos(0)); cout << "0"; 	//prints new position
		}
		if (player.getDirection() == "right")
		{
			oldHeadPos = { player.getXpos(0), player.getYpos(0) };		//stores the old positions away
			player.setXpos(player.getXpos(0) + 1); 						//calculates new position
			cout << "\b "; 												//erases old head from the console
			gotoxy(player.getXpos(0), player.getYpos(0)); cout << "0"; 	//prints new position
		}

	//Snake body movement
		if (player.countBody() > 0)
		{
			COORD new_body, old_body;		//For the snakes body, used to calculate the new positions by storing away the old ones, and having the snake "follow" the succeeding body part
			for (short i = 1; i <= player.countBody(); i++)
			{
				//The first segment of the snake is set to the position of where the head WAS
				if (i == 1)
				{
					gotoxy(player.getXpos(i), player.getYpos(i)); cout << "\b";		//erase old position

					new_body.X = player.getXpos(i);									//store old positions to pass down
					new_body.Y = player.getYpos(i);

					player.updateBodyX(i, oldHeadPos.X);							//set new position to the old head position	
					player.updateBodyY(i, oldHeadPos.Y);

					gotoxy(player.getXpos(i), player.getYpos(i)); cout << "0";		//write new position
				}
				//All segments afterward daisy chain off of the one before it
				else
				{
					old_body.X = player.getXpos(i);									//old position stored so we can pass it down to the new pos after calculating this one
					old_body.Y = player.getYpos(i);

					gotoxy(player.getXpos(i), player.getYpos(i)); cout << "\b ";	//erase old position

					player.updateBodyX(i, new_body.X);								//set new position to new_body, which stores the position of the segment before
					player.updateBodyY(i, new_body.Y);

					gotoxy(player.getXpos(i), player.getYpos(i)); cout << "0";		//write new position

					new_body.X = old_body.X;										//sets new_body such that we can pass it down to the next snake index
					new_body.Y = old_body.Y;
				}
			}
		}

		//COLLISION
	//Check for collision with wall
		if (player.getXpos(0) == 0 || player.getYpos(0) == 0 || player.getXpos(0) == fenceLength || player.getYpos(0) == fenceHeight)
		{
			gameLoop = false;
		}

	//Check for collision with the head and any other body parts. Should just be able to loop through the coordinates array (start from 1, that is the 2nd index), to see if the current head position matches any of the others
			//set gameLoop to false if so.

	//Check collision with apple: add a new part to the snake
		if (player.getXpos(0) == appleXpos && player.getYpos(0) == appleYpos)
		{
			player.updateBody();					//add to the body
			appleGenerated = false;					//so the code to generate a new apple will run
		}

		Sleep(200);
	}

	gotoxy(0, fenceLength + 1); //refresh curser
	cout << "You ate " << player.countBody() << " apple(s)." << endl;
	return 0;
}