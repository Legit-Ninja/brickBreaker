#include"gameObjects.h"

// Add a time for input and output
// Add bounces from still boxes

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

void overlap(vector<GameObject*> go ) { //Reassures all stillboxes are not overlapping
    for (int i = 3; i<go.size(); i++){ 
        for (int j = 0; j < 7; j++)     //makes sure it doesn't overlap any of the positions
        {
            while (dynamic_cast<stillBox *>(go[i-1])->getX() == // Captured the getX() from still box to compare to others
                    dynamic_cast<stillBox *>(go[i])->getX()+j)     
            {

                dynamic_cast<stillBox *>(go[i])->newX(); // makes the stillbox in a new place
                dynamic_cast<stillBox *>(go[i])->newY();

            }
        }
    } 
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
    }
    overlap(gObjects);
    int key = 0;
    int blocksHit = 0;
    while (gameOver(gObjects, blocksHit) != true) //makes sure the game isn't over 
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
        gObjects[i]->draw();
        gObjects[i]->move();
        }

 		for ( int x=0; x < 25; x++ ) {
		   /* Use _getch to throw key away. */
			if ( _kbhit() ) {
				key =  _getch();
			}
			Sleep(5);
		}
    system("cls");
    ShowConsoleCursor(false);
    }
    ShowConsoleCursor(true);
    return 0;
}

