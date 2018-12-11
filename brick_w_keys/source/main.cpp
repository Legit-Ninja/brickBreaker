#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <typeinfo>
#include <windows.h> 
#include <Wincon.h>
#include <stdio.h>
#include <conio.h>
using namespace std;

// Stuff for output to console screen
HANDLE hStdout, hStdin; 
CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility  {/////}
    SetConsoleCursorInfo(out, &cursorInfo);
}

const int MAX_X = 40;
const int MAX_Y = 20;

class GameObject {
    protected:
        COORD pos;
        int vy,vx;
    public:
        GameObject() { // Makes gameobject for the ball and still box.

            //  defualt values
            pos.X = 0;
            pos.Y = 0;
            vx = 0;
            vy = 0;
        }
         virtual void draw() {	SetConsoleCursorPosition(hStdout,pos);}

         COORD get_pos() {
            return pos;
         }

         virtual void move() {}
};

class stillBox : public GameObject {
    protected:
        string stillBoxShape;
    public:
        stillBox() {
        
           // randomly generates stillBox in window's upper half
                pos.X = rand() % MAX_X;
                pos.Y = (rand() %  10 ) + 10;   //DOES THE COMPUTER START AT TOP LEFT??
                while (this->pos.X+7 > 40) { // If the still box prints half way through window, randomize it again.
                pos.X = rand() % MAX_X;
                stillBoxShape = "[/////]";
                // look at this (get it?? hehe)
            }
            
        }
        short getX() { return pos.X; }
        short getY() { return pos.X; }
        string getShape()
        {
            return stillBoxShape;
        }
        void draw() {
             GameObject::draw();
             cout << stillBoxShape;
        }
        void hit()
        {
            stillBoxShape = "       ";
        }
};

class userBox : public GameObject {
    protected:
        string userBoxShape;
    public:
        userBox() {
            pos.X = 17; // Place box in center of screen on bottom
            pos.Y = 18;
            vx = 2;
            userBoxShape = "[IIIII]";
        }
        void draw() {
            GameObject::draw();
            cout << userBoxShape;
        }
        void move(int key) {
            if ( key != -2 ) {
                cout << "You entered: " << key << endl;
                exit(0);
            }
            if (key == 75) // left arrow key
            {
                this-> pos.X = pos.X-=vx; //moves userbox left
            }
            else if( key == 77)     //right arrow key
            {
                this-> pos.X = pos.X+=vx; // moves userbox right
            }
            
        }
};

class Ball : public GameObject {
    protected:
        char ballShape_;
    public:
        Ball(char ballShape) { //: ballShape_(ballShape) {}
            ballShape_ = ballShape;
            pos.X = 20;
            pos.Y = 17;
            vx = 1;
            vy = -1;
        }
        void flipVy()    //used when ball hits userbox
        {
            vy = vy*-1;
        }
        short getY() { return pos.Y; }
        void draw() {
            GameObject::draw();
            cout << ballShape_;
        }
        void move() {
            pos.X += vx;
            pos.Y += vy;
            // KEEPS THE BALL INSIDE WINDOW EXCEPT THE BOTTOM
            if (pos.X >= MAX_X) { vx *= -1; pos.X = MAX_X - 1; }
            if ( pos.X <= 0 )   { vx *= -1;	pos.X = 1; 		   }
            if ( pos.Y <= 0 )    { vy *= -1;	pos.Y = 1; 		   }
            if (pos.Y >= 18) { 
                cout << "game over" << endl;
                exit(0); 
                }
        }

};

bool collide(COORD ballPos, COORD blockPos)   //compares a block's position with the ball's
{
    for (int i = 0; i < 7; i++)   //checks to see if ball collides with any of the block's positions
    {
        if (ballPos.X == blockPos.X && ballPos.Y == blockPos.Y)     // If they collide return true
        {
            return true;
        }
        blockPos.X++;
    }
    return false;
} 
bool gameOver(vector<GameObject*> go, int& blocksHit)     // returns true if all 10 stillBoxes have been hit
{
    for (int i = 0; i < go.size(); i++)
    {
        if (typeid(go[i]) == typeid(stillBox))  //checks to see if the gameobject is a still box
        {
            dynamic_cast<stillBox *>(go[i]);
            if ( dynamic_cast<stillBox *>(go[i])->getShape() == "       ")
            {
                blocksHit++;
            }
        }
    }
    if(blocksHit == 10)
    {
        return true;
    }
    return false;
}



int main() 
{
    srand(unsigned(time(NULL)));
    vector<GameObject*> gObjects;
    
    // Get handles to STDIN and STDOUT. 
    hStdin = GetStdHandle(STD_INPUT_HANDLE); 
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    if (hStdin == INVALID_HANDLE_VALUE || hStdout == INVALID_HANDLE_VALUE) 
    {
        MessageBox(NULL, TEXT("GetStdHandle"), TEXT("Console Error"), 
            MB_OK);
        return 1;
    }
    
    gObjects.push_back(new userBox());
    gObjects.push_back(new Ball('O'));
    for (int i = 0; i < 10; i++)
    {
        gObjects.push_back(new stillBox());
       /* for (j =0; j < gObjects.size(); j++)
        {
            if ()
        } */
    }
    int key;
    int blocksHit = 0;
    while (gameOver(gObjects, blocksHit) != true) //makes sure the game isn't over MAIN GAME LOOP
    {  
        for (int i = 0; i < gObjects.size(); i++)       // updates game objects
        {   
            //cout << i << endl;
        //cin >> key;
            for (int j = i+1; j < gObjects.size(); j++ )    // checks for collisons
            {
                if (collide(gObjects[i]->get_pos(), gObjects[j]->get_pos()) == true)    // if the two objects we are looking at collide
                {
                    if (typeid(*gObjects[i]) == typeid(stillBox) && typeid(*gObjects[j]) == typeid(Ball) || //checks to see if objects were stillbox & ball
                    typeid(*gObjects[j]) == typeid(stillBox) && typeid(*gObjects[i]) == typeid(Ball))    
                    {  
                        if (typeid(*gObjects[i]) == typeid(stillBox)) // finds which one was the still box and calls "hit"
                        {
                            dynamic_cast<stillBox *>(gObjects[i])->hit();
                        }
                        else
                        {
                            dynamic_cast<stillBox *>(gObjects[j])->hit();
                        }
                    }

                    if (typeid(*gObjects[i]) == typeid(userBox) && typeid(*gObjects[j]) == typeid(Ball) || //checks to see if objects were userBox & ball
                    typeid(*gObjects[j]) == typeid(userBox) && typeid(*gObjects[i]) == typeid(Ball))
                    {
                        if (typeid(*gObjects[i]) == typeid(Ball)) // finds which one was the ball and calls "flipVy"
                        {
                            dynamic_cast<Ball*>(gObjects[i])->flipVy();
                        }
                        else
                        {
                            dynamic_cast<Ball *>(gObjects[j])->flipVy();
                        }
                    }
                }
            }
        dynamic_cast<userBox *>(gObjects[0])->move(key);    // i know the first game object is the userbox, so call the special move function
        //cout << "did i make it here?" << endl; //very confused by the outputs to terminal
        gObjects[i]->draw();
        //cout << dynamic_cast<Ball *>(gObjects[1])->getY() << endl;  //says my balls's y position is 27 yet exits as if it's 28?
        gObjects[i]->move();
        }

 		for ( int x=0; x < 12; x++ ) {
		   /* Use _getch to throw key away. */
			if ( _kbhit() ) {
				key =  _getch();
			}
			Sleep(10);
		}

    system("cls");
    ShowConsoleCursor(false);
    }
    ShowConsoleCursor(true);
    return 0;
}
