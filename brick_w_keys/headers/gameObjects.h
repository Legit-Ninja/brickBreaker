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
        double vy,vx;
    public:
        GameObject() { // Makes gameobject for the ball and still box.

            //  defualt values
            pos.X = 0;
            pos.Y = 0;
            vx = 0.0;
            vy = 0.0;
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
            pos.Y = (rand() %  10) + 1;   
            while (this->pos.X+7 > 40) { // If the still box prints half way through window, randomize it again.
                pos.X = rand() % MAX_X;
            }
            stillBoxShape = "[/////]";
         // look at this (get it?? hehe)
        

            
        }
        short getX() { return pos.X; }
        short getY() { return pos.X; }
        void newX() {this->pos.X = rand() % MAX_X;}
        void newY() {this->pos.Y = rand() & MAX_X;}
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
            vx = 1;
            userBoxShape = "[IIIII]";
        }
        void draw() {
            GameObject::draw();
            cout << userBoxShape;
        }
        void move(int key) {
           /* if ( key != -2 ) {
                cout << "You entered: " << key << endl;
               //exit(0);
            }  */
            if (key == 75) // left arrow key
            {
                this-> pos.X = pos.X-=vx; // moves userbox left
            }
            else if(key == 77)     //right arrow key
            {
                this-> pos.X = pos.X+=vx; // moves userbox right
            }
            if (pos.X+7 >= MAX_X) { this-> pos.X = MAX_X-7;  }
            if ( pos.X <= 0 )     { this-> pos.X = 0;   }
            
        }
};

class Ball : public GameObject {
    protected:
        char ballShape_;
    public:
        Ball(char ballShape) { 
            ballShape_ = ballShape;
            pos.X = 20;
            pos.Y = 17;
            vx = 1.0;
            vy = -1.0;
        }
        void flipVy()    //used when ball hits userbox
        {
            vy = vy*-1.0;
            this->pos.Y = 17;
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
            if (pos.X >= MAX_X) { vx *= -1.0; pos.X = MAX_X - 1; }
            if ( pos.X <= 0 )   { vx *= -1.0;	pos.X = 0; 		   }
            if ( pos.Y <= 0 )    { vy *= -1.0;	pos.Y = 0; 		   }
            if (pos.Y >= 19) { 
                cout << "game over" << endl;
                exit(0); 
                }
        }

};