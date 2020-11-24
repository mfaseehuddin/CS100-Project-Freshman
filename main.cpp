#include <time.h>
#include <thread>
#include <chrono>
#include <conio.h>
#include <windows.h>
#include <iostream>
using namespace std;

const int SCREEN_HORIZONTAL = 150;
const int SCREEN_VERTICAL = 50;
char screen[SCREEN_VERTICAL][SCREEN_HORIZONTAL];
char prevScreen[SCREEN_VERTICAL][SCREEN_HORIZONTAL];

bool change = true;

class GameObject {
    public:
        string name;
        string graphic;
        int position[2];
        int width;
        GameObject(string, int, int, string, int);
        void Draw();
        void Move();
        void Clean();
};
GameObject::GameObject(string iname, int x, int y, string igraphic, int iwidth){
    name = iname;
    position[0] = x;
    position[1]= y;
    graphic = igraphic;
    width = iwidth;
}

void GameObject::Draw(){
    int cheight = 0;
    int x = 0;
    for(int objectAdd = 0; objectAdd <= graphic.length();){
            if(x == width){
                x = 0;
                cheight++;                
            }else{
                screen[position[1] + cheight][position[0] + x] = graphic[objectAdd];
                x++;
            }
        objectAdd++;
    }
    change = true;
}

void GameObject::Clean(){
    int cheight = 0;
    int x = 0;
    for(int objectAdd = 0; objectAdd <= graphic.length();){
            if(x == width){
                x = 0;
                cheight++;                
            }else{
                screen[position[1] + cheight][position[0] + x] = ' ';
                x++;
            }
        objectAdd++;
    }
    change = true;
}

//function prototypes
void setCursorPosition(int x, int y);
void drawScreen();
void gameRoutine();

//function defs
int main(){
    system("cls");

    memset(prevScreen, ' ', SCREEN_VERTICAL*SCREEN_HORIZONTAL);

    //drawing box
    for(int vertical = 0; vertical < SCREEN_VERTICAL; vertical++){
        for(int horizontal = 0; horizontal < SCREEN_HORIZONTAL; horizontal++){
            screen[vertical][horizontal] = ' ';
            if((horizontal == 0)||(horizontal == SCREEN_HORIZONTAL-1)){
                screen[vertical][horizontal] = '|';
            }
            if(vertical == 0 || vertical == SCREEN_VERTICAL-1){
                screen[vertical][horizontal] = '=';
            }
        }
    }
    
    gameRoutine();

    return 0;
}

void setCursorPosition(int x, int y){
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush();

    //checking for bounds
    if(x == 0 || x == SCREEN_VERTICAL || y == 0 || y == SCREEN_HORIZONTAL){
        return;
    }
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}






void gameRoutine(){
    //start     
        GameObject message("message", 30, 15, "PLEASE PRESS F11 FOR FULLSCREEN|THEN PRESS ENTER", 31);
        GameObject player("Faseeh", 100, 10, "  000 | 00 00|  000 |   |  |  /|\\ | / | \\|   |  |  / \\ | /   \\|", 6);
        player.Draw();
        message.Draw();
    //update
    while (1){
        //gameMechanics and definitions
        if(change){
            drawScreen();
        }

    }
    
}
void drawScreen(){
    for(int y = 0; y < SCREEN_VERTICAL; y++){
        for(int x = 0; x < SCREEN_HORIZONTAL; x++){
            if(prevScreen[y][x] == screen[y][x]){
                continue;
            }
            setCursorPosition(x,y);
            printf("%c", screen[y][x]);
           }
        printf("\n");
    }
    change = false;
    memcpy(prevScreen, screen, SCREEN_VERTICAL*SCREEN_HORIZONTAL);
}