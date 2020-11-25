#include <time.h>
#include <chrono>
#include <conio.h>
#include <windows.h>
#include <iostream>


using namespace std;

const float GRAVITY[2] = {0, 1};
const int SCREEN_HORIZONTAL = 230;
const int SCREEN_VERTICAL = 75;
char screen[SCREEN_VERTICAL][SCREEN_HORIZONTAL];
char prevScreen[SCREEN_VERTICAL][SCREEN_HORIZONTAL];

bool change = true;
int refresh = 0;
bool exitTheGame = false;

char currentInput = '0';
class GameObject {
    public:
        string name;
        string graphic;
        int position[2];
        int width;
        int height;
        float mass;
        float velocity[2];
        float acceleration[2];
        GameObject(string, int, int, string, int);
        void Draw();
        void Move(int, int);
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

//async windows funcs
DWORD WINAPI getAsyncInput(LPVOID lpParam){
    while(1){
        currentInput = getch();
        Sleep(100);
        currentInput = '0';
    }
}
DWORD WINAPI updateScreen(LPVOID lpParam){
    while(1){
        if(change){
            drawScreen();
            setCursorPosition(1,1);
            cout << "x";
        }
    }
}
//function defs
int main(){
    system("cls");

    //this is thread creation and im a little confused
        HANDLE hAsyncInputThread;
        DWORD AsyncInputThreadID;

        hAsyncInputThread = CreateThread(
            NULL,
            0,
            getAsyncInput,
            NULL,
            0,
            &AsyncInputThreadID
        );
    //thread creation

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
            if(currentInput == 'y'){
                ready = true;
            }
        }
        memset(screen, ' ', SCREEN_VERTICAL*SCREEN_HORIZONTAL);
        message.Clean();
    
    //update
    //int movespeed = 5;
    //GameObject player("Faseeh", 10, 40, "  000 | 00 00|  000 |   |  |  /|\\ | - | -|   |  |  / \\ | /   \\|", 6);
    string groundString = "";
    for(int i=0; i<=SCREEN_HORIZONTAL;i++){groundString.append("~");}
    GameObject ground("ground", 0, SCREEN_VERTICAL-1, groundString,SCREEN_HORIZONTAL);
    
    //GameObject attacker("Theif", 100, 40, "  0 0 | 0   0| \\0|0/|  \\{/ |   }  |   {  |   }  |  / \\ | /   \\|", 6);
    int timesChanged = 0;
    srand(time(0));
    int createArrowLeft = 0;
    int createArrowRight = 0;
    
    /*
    while(!exitTheGame){
        timesChanged++;
        drawScreen();
        if(timesChanged > 30){
            memset(screen, ' ', SCREEN_HORIZONTAL*SCREEN_VERTICAL);
            //refresh all
            //player.Draw();
            //attacker.Draw();
            ground.Draw();
            timesChanged=0;
        }
        if(timesChanged > 10){
            //attacker AI
            int attackerSpeedX = (rand()%10)/2;
            int attackerSpeedY = (rand()%10)/2;
            if(player.position[0] < attacker.position[0]){
                attackerSpeedX *= -1;
            }
            if(player.position[1] < attacker.position[1]){
                attackerSpeedY *= -1;
            }
            attacker.Move(attackerSpeedX, 0);
        }
        
    
        if(currentInput == 'w'){
            player.Move(0,-movespeed/2);
        }else if(currentInput == 'a'){
            player.Move(-movespeed, 0);
        }else if(currentInput == 's'){
            player.Move(0, movespeed/2);
        }else if(currentInput == 'd'){
            player.Move(movespeed,0);
        }else if(currentInput == 'p'){
            //go to break or menu
            break;
        }else if(currentInput == 'W'){
            player.Move(0,-movespeed*2);
        }else{
            
        }
      

        //print some info
        setCursorPosition(1,1);
        cout << "height of player: " << player.height;
        setCursorPosition(1,2);
        cout << "Player Position: " << player.position[0] << " , "<< player.position[1];
        Sleep(45);
    }*/

    float time = 0;
    float dt = 1;

    GameObject ball("Ball1", 10, 40, "**|**", 2);
    
    //this is thread creation and im a little confused
        HANDLE hAsyncDrawScreenThread;
        DWORD AsyncDrawScreenThreadID;

        hAsyncDrawScreenThread = CreateThread(
            NULL,
            0,
            updateScreen,
            NULL,
            0,
            &AsyncDrawScreenThreadID
        );
    //thread creation

    GameObject player("Faseeh", 10, 65, "  000 | 00 00|  000 |   |  |  /|\\ | - | -|   |  |  / \\ | /   \\|", 6);
    GameObject TopLeftAnchor("Anchor", 1,1,"+",1);
    
    while(!exitTheGame){
        if(currentInput == 'p'){
            break;
        }
        else if(currentInput == 'w'){
            player.Move(0,-1);
        }
        
        else if(currentInput == 's'){
            player.Move(0,1);
        }
        
        else if(currentInput == 'a'){
            player.Move(-1,0);
        }
        
        else if(currentInput == 'd'){
            player.Move(1,0);
        }

        if(refresh > 5){
            memset(screen, ' ', SCREEN_HORIZONTAL*SCREEN_VERTICAL);
            //refresh all
            player.Draw();
            ground.Draw();
            ball.Draw();
            refresh = 0;
        }

        Sleep(30);
        refresh++;
    }
}
