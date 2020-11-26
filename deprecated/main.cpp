#include <time.h>
#include <chrono>
#include <conio.h>
#include <windows.h>
#include <iostream>


using namespace std;

const int REFRESH_TIME = 10;
const float GRAVITY[2] = {0, 1};
const int X = 0;
const int Y = 1;
const int MAX_OBJECTS_PER_SCENE = 20;
const int SCREEN_HORIZONTAL = 190;
const int SCREEN_VERTICAL = 65;
char screen[SCREEN_VERTICAL][SCREEN_HORIZONTAL];
char prevScreen[SCREEN_VERTICAL][SCREEN_HORIZONTAL];

bool change = true;
int refresh = 0;
bool exitTheGame = false;

char currentInput = '0';

long gameTime = 0;

class GameObject {
    public:
        string name;
        string graphic;
        int position[2];
        int width;
        int height;
        bool alive = true;
        bool rigidBody;
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
}
void GameObject::Draw(){
    int cheight = 0;
    int x = 0;
    for(int graphicAddress = 0; graphicAddress < graphic.length();){
            if(x == width){
                x = 0;
                cheight++;                
            }else{
                screen[position[1] + cheight][position[0] + x] = graphic[graphicAddress];
                x++;
            }
        graphicAddress++;
    }
    change = true;
    height = cheight;
}
void GameObject::Clean(){
    int cheight = 0;
    int x = 0;
    for(int graphicAddress = 0; graphicAddress < graphic.length();){
            if(x == width){
                x = 0;
                cheight++;                
            }else{
                screen[position[1] + cheight][position[0] + x] = ' ';
                x++;
            }
        graphicAddress++;
    }
    change = true;
}
void GameObject::Move(int x, int y){
        //Clean();
        position[X] += x;
        position[Y] += y;
        //Draw();
        change = true;
}


class SceneManager{
    public:
        string name;
        GameObject* gameObjects[MAX_OBJECTS_PER_SCENE];
        int LastObjectAddress = 0;
        void addObject(GameObject*);
        void removeObject(GameObject*);
};
void SceneManager::addObject(GameObject* gameObjectPointer){
    gameObjects[LastObjectAddress] = (gameObjectPointer);
    LastObjectAddress++;
}
void SceneManager::removeObject(GameObject* gameObjectPointer){
    gameObjectPointer->alive = false;
}


SceneManager MainScene;
//function prototypes
void setCursorPosition(int x, int y);
void drawScreen();
void gameRoutine();


//async windows funcs
DWORD WINAPI getAsyncInput(LPVOID lpParam){
    while(1){
        currentInput = getch();
        Sleep(REFRESH_TIME*4);
        currentInput = '0';
    }
}
DWORD WINAPI updatePhysicsAndScreen(LPVOID lpParam){
    while(1){
        for(int objectAddress = 0; objectAddress < MainScene.LastObjectAddress; objectAddress++){
            if(MainScene.gameObjects[objectAddress]->rigidBody){
                    if(((MainScene.gameObjects[objectAddress]->position[Y] + MainScene.gameObjects[objectAddress]->height + 1) < SCREEN_VERTICAL)  &&  ((MainScene.gameObjects[objectAddress]->position[Y]) > 1) && (MainScene.gameObjects[objectAddress]->acceleration[X] !=0 || MainScene.gameObjects[objectAddress]->acceleration[Y] !=0)){
                    //MainScene.gameObjects[objectAddress]->Clean();
                    MainScene.gameObjects[objectAddress]->velocity[X] += MainScene.gameObjects[objectAddress]->acceleration[X] * REFRESH_TIME/500.0;
                    MainScene.gameObjects[objectAddress]->velocity[Y] += MainScene.gameObjects[objectAddress]->acceleration[Y] * REFRESH_TIME/500.0; 

                    MainScene.gameObjects[objectAddress]->position[X] += MainScene.gameObjects[objectAddress]->velocity[X];
                    MainScene.gameObjects[objectAddress]->position[Y] += MainScene.gameObjects[objectAddress]->velocity[Y];
                   // MainScene.gameObjects[objectAddress]->Draw();
                }else{
                    MainScene.gameObjects[objectAddress]->rigidBody = false;
                }
            }
        }

        if(change){
            drawScreen();
            setCursorPosition(1,1);
            cout << "x";
        }
        
        Sleep(REFRESH_TIME);
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
    memset(screen, ' ', SCREEN_VERTICAL*SCREEN_HORIZONTAL);
    for(int ObjectsAddress = 0; ObjectsAddress < MainScene.LastObjectAddress; ObjectsAddress++){
        if(MainScene.gameObjects[ObjectsAddress]->alive){
            MainScene.gameObjects[ObjectsAddress]->Draw();
        }
    }
    
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
    /*    
        string borders;
        //creation of borders;
        for(int y = 0; y < SCREEN_VERTICAL; y++){
            for(int x = 0; x < SCREEN_HORIZONTAL; x++){
                
            }
        }
        GameObject ScreenBorders("ScreenBorders", 0,0, borders, SCREEN_HORIZONTAL-1);
        MainScene.addObject(&ScreenBorders);
    */
        GameObject message("message", 50, 15," PLEASE PRESS F11 FOR FULLSCREEN  ZOOM TO FIT THE BOX              THEN PRESS Y", 32);
        MainScene.addObject(&message);
        bool ready = false;
        drawScreen();
        while(!ready){
            if(currentInput == 'y'){
                ready = true;
            }
        }
        memset(screen, ' ', SCREEN_VERTICAL*SCREEN_HORIZONTAL);
        MainScene.removeObject(&message);
    

    //update
       

    string groundString = "";
    for(int i=0; i<=SCREEN_HORIZONTAL;i++){groundString.append("~");}
    GameObject ground("ground", 0, SCREEN_VERTICAL-1, groundString,SCREEN_HORIZONTAL);
    MainScene.addObject(&ground);
    
    int timesChanged = 0;
 

    float time = 0;
    float dt = 1;

    {//this is thread creation and im a little confused
        HANDLE hAsyncUpdatePhysicsAndScreenThread;
        DWORD AsyncUpdatePhysicsAndScreenThreadID;

        hAsyncUpdatePhysicsAndScreenThread= CreateThread(
            NULL,
            0,
            updatePhysicsAndScreen,
            NULL,
            0,
            &AsyncUpdatePhysicsAndScreenThreadID
        );
    //thread creation
    }
    int movespeed = 1;
    GameObject player("Faseeh", 10, 65, "***************", 4);
    MainScene.addObject(&player);
    // MainScene.addObject(&TopLeftAnchor);
    
    
    while(!exitTheGame){
        if(currentInput == 'p'){
            break;
        }
        else if(currentInput == 'w' && player.position[Y] > 1){
            player.Move(0,-movespeed);
        }
        
        else if(currentInput == 's' && player.position[Y] < SCREEN_VERTICAL - player.height - 1){
            player.Move(0,movespeed);
        }
        
        else if(currentInput == 'a' && player.position[X] > 0){
            player.Move(-movespeed,0);
        }
        
        else if(currentInput == 'd' && player.position[X] < SCREEN_HORIZONTAL-player.width - 1){
            player.Move(movespeed,0);
        }
        else if(currentInput == 'b'){
            player.alive = false;
        }else if(currentInput == 'c'){
            player.alive = true;
        }
        
        Sleep(REFRESH_TIME);
        refresh++;
    }
}
