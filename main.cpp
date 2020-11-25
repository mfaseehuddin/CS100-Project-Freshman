#include <time.h>
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
bool exitTheGame = false;

char currentInput = '0';
class GameObject {
    public:
        string name;
        string graphic;
        int position[2];
        int width;
        int height;
        GameObject(string, int, int, string, int);
        void Draw();
        void Move(int, int  );
        void Clean();
};
GameObject::GameObject(string iname, int x, int y, string igraphic, int iwidth){
    name = iname;
    position[0] = x;
    position[1]= y;
    graphic = igraphic;
    width = iwidth;
    Draw();
    change = true;
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
    height = cheight;
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
void GameObject::Move(int x, int y){
        Clean();
        position[0] += x;
        position[1] += y;
        Draw();
        change = true;
}



//function prototypes
void setCursorPosition(int x, int y);
void drawScreen();
void gameRoutine();
void getInput();
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

void gameRoutine(){
    //start     
        GameObject message("message", 50, 15," PLEASE PRESS F11 FOR FULLSCREEN  ZOOM TO FIT THE BOX              THEN PRESS Y", 32);
       
        bool ready = false;
        drawScreen();
        while(!ready){
            if(getch() == 'y' || getch() == 'Y'){
                ready = true;
            }
        }
        memset(screen, ' ', SCREEN_VERTICAL*SCREEN_HORIZONTAL);
        message.Clean();
    //update


    int movespeed = 5;
    GameObject player("Faseeh", 10, 40, "  000 | 00 00|  000 |   |  |  /|\\ | - | -|   |  |  / \\ | /   \\|", 6);
    string groundString = "";
    for(int i=0; i<=SCREEN_HORIZONTAL;i++){groundString.append("~");}
    GameObject ground("ground", 0, 49, groundString,150);
    
    GameObject attacker("Theif", 20, 30, "  0 0 | 0   0| \\0|0/|  \\{/ |   }  |   {  |   }  |  / \\ | /   \\|", 6);

    int timesChanged = 0;
    srand(time(0));
    int createArrowLeft = 0;
    int createArrowRight = 0;
    while(!exitTheGame){
        timesChanged++;
        drawScreen();
        if(timesChanged > 30){
            memset(screen, ' ', SCREEN_HORIZONTAL*SCREEN_VERTICAL);
            //refresh all
            attacker.Draw();
            ground.Draw();
            timesChanged=0;
        }
        if(timesChanged > 3){
            //attacker AI
            int attackerSpeedX = (rand()%10)/2;
            int attackerSpeedY = (rand()%10)/2;
            if(player.position[0] < attacker.position[0]){
                attackerSpeedX *= -1;
            }
            if(player.position[1] < attacker.position[1]){
                attackerSpeedY *= -1;
            }
            attacker.Move(attackerSpeedX, attackerSpeedY);
        }
        
    
        /*if(getch() == 'w'){
            player.Move(0,-movespeed/2);
        }else if(getch() == 'a'){
            player.Move(-movespeed, 0);
        }else if(getch() == 's'){
            player.Move(0, movespeed/2);
        }else if(getch() == 'd'){
            player.Move(movespeed,0);
        }else if(getch() == 'p'){
            //go to break or menu
            break;
        }else if(getch() == 'W'){
            player.Move(0,-movespeed*2);
        }else if(getch() == 'j'){
            createArrowLeft++;
        }
        if(createArrowLeft > 0){
            GameObject arrow("a", player.position[0], player.position[1]+4, "<----", 5);
            arrow.Move(5,0);
        }*/

        //print some info
        setCursorPosition(1,1);
        cout << "height of player: " << player.height;
        setCursorPosition(1,2);
        cout << "Player Position: " << player.position[0] << " , "<< player.position[1];
        Sleep(45);
    }
}
