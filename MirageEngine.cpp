#include<iostream>
#include <windows.h>
#include <vector>
#include <conio.h>
#include <time.h>
#include <new>

using namespace std;

const int X = 0;
const int Y = 1;
const int X_MAX = 150;
const int Y_MAX = 30;
const int MAX_OBJECTS = 20;
const int REFRESH_TIME = 15;
const float DT = 0.1; 
const int X_BUFFER_FOR_OBJECT = 10;
int current_Enemies = 0;

bool scene_Change = true;
char current_Input = '0';
float gravity_vector[2] = {0,0.5};


//Class Definitions
//GameObject
class GameObject{
    public:
        string id;
        string graphic;
        int height;
        int width;
        int health = 100;
        int position[2] = {0,0};
        float velocity[2] = {0,0};
        float acceleration[2] = {0,0};
        bool single_Change = false;
        bool isRigidBody = false;
        GameObject(string i_id, int i_x, int i_y, string i_graphic, int i_height, int i_width);
        void Transform(int x, int y);
};
GameObject::GameObject(string i_id, int i_x, int i_y, string i_graphic, int i_height, int i_width){
    id = i_id;
    position[X] = i_x;
    position[Y] = i_y;
    graphic = i_graphic;
    height = i_height;
    width = i_width;
}
void GameObject::Transform(int x, int y){
        position[X] += x;
        position[Y] += y;
}
//GameObjectEnd


//SceneManagerClass
class SceneManager{
    public:
        int id;
        char scene[Y_MAX][X_MAX];
        vector<GameObject*> gameObjects;
        SceneManager();
        void AddObject(GameObject*);
        GameObject * getObject(string object_ID);
        void RemoveObject(string object_ID);
        void DrawOnScene(GameObject*); 
        void EraseFromScene(GameObject*);
        void Debug();
        void objectCreator(GameObject*);
        
};
SceneManager::SceneManager(){
    memset(scene, ' ', X_MAX*Y_MAX);
}
void SceneManager::AddObject(GameObject* object){
    gameObjects.push_back(object);
    DrawOnScene(object);
}
void SceneManager::RemoveObject(string object_ID){
    for(auto game_object = gameObjects.begin(); game_object != gameObjects.end(); ++game_object){
        if ((*game_object)->id == object_ID){
            EraseFromScene(*game_object);
            gameObjects.erase(game_object);
            //delete(*game_object);
            break;
        }
    }
}
GameObject* SceneManager::getObject(string object_ID){
    for(auto game_Object = gameObjects.begin(); game_Object != gameObjects.end(); ++game_Object){
        if((*game_Object)->id == object_ID){
            return(*game_Object);
        }
    }
    return NULL;
}
void SceneManager::DrawOnScene(GameObject* object){
    string& current_Graphic = object->graphic;
    int current_Height = 0;
    int x = 0;
    for(int graphicIndex = 0; graphicIndex < current_Graphic.length();){
        if(x == object->width){
            x = 0;
            current_Height++;
        }else{
            //if((object->position[Y]) >= 0 && (object->position[Y]) + current_Height < Y_MAX && (object->position[X]) + x >= 0 && (object->position[X]) + x < X_MAX)
                int currY = (object->position[Y]) + current_Height;
                int currX = (object->position[X]) + x;
                if(currY >= 0 && currX >= 0  &&  currX <= X_MAX && currY <= Y_MAX)
                {scene[currY][currX] = current_Graphic[graphicIndex];
                x++;
                graphicIndex++;}
                else{
                    scene[5][5] = 'E';
                    scene[5][6] = 'R';
                    scene[5][7] = 'R';
                    scene[5][8] = 'O';
                    scene[5][9] = 'R';
                    
                    return;
                }
        }
        
    }
    scene_Change = true;
   
}
void SceneManager::EraseFromScene(GameObject* object){
        int current_Height = 0;
    int x = 0;
    string& current_Graphic = object->graphic;
    for(int graphicIndex = 0; graphicIndex < current_Graphic.length();){
        if(x == object->width){
            x = 0;
            current_Height++;
        }else{
            
                int currY = (object->position[Y]) + current_Height;
                int currX = (object->position[X]) + x;
                if(currY >= 0 && currX >= 0  &&  currX <= X_MAX && currY <= Y_MAX){
                    scene[currY][currX] = ' ';
                    x++;
                    graphicIndex++;
                }else{
                    scene[5][5] = 'E';
                    scene[5][6] = 'R';
                    scene[5][7] = 'R';
                    scene[5][8] = 'O';
                    scene[5][9] = 'R';
                    
                    return;
                }
        }
        
    }
    scene_Change = true;
   
}
void SceneManager::Debug(){
    for(int y = 0; y < Y_MAX; y++){
        for(int x = 0; x < X_MAX; x++){
            scene[y][x] = 'X';
        }
    }
}
void SceneManager::objectCreator(GameObject* new_Object){
    AddObject(new_Object);
}
//SceneManagerClassEnd

//WindowCLASS
class Window{
    public:
        //Display is to output things to the screen
        Window();
        void SetActiveScene(SceneManager*);
        SceneManager* GetActiveScene();
        void SetCursorPosition(int x, int y);
        void DisplayActiveScreen(SceneManager*);
    private:    
        SceneManager* active_Scene;
        //this will act as prevScreenBuffer
        char prev_Screen_Buffer[Y_MAX][X_MAX];
};
Window::Window(){
    //initializing and clearing the prevScreenBuffer
    memset(prev_Screen_Buffer, ' ', Y_MAX*X_MAX);
    active_Scene = NULL;
}
void Window::SetActiveScene(SceneManager* new_Scene){
    active_Scene = new_Scene;
    new_Scene->gameObjects.reserve(50);
}
SceneManager* Window::GetActiveScene(){
    if(active_Scene != NULL){
        return active_Scene;
    }else{
        return NULL;
    }
}
void Window::SetCursorPosition(int x, int y){
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush();

    //checking for bounds
    if(x == 0 || x == X_MAX || y == 0 || y == Y_MAX){
        return;
    }
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}
void Window::DisplayActiveScreen(SceneManager* frame){
    for(int y = 0; y < Y_MAX; y++){
        for(int x = 0; x < X_MAX; x++){
            if(prev_Screen_Buffer[y][x] == frame->scene[y][x]){
                continue;
            }
            SetCursorPosition(x,y);
            printf("%c", frame->scene[y][x]);
           }
        printf("\n");
    }
    memcpy(prev_Screen_Buffer, frame->scene, X_MAX*Y_MAX);
    SetCursorPosition(1,1);
    printf("=");
    scene_Change = false;

}
//WindowCLASSEND

//GLOBAL VARIABLES && Object Instances
Window MainWindow;
SceneManager Scene_Game;
SceneManager Scene_Pause;
SceneManager Scene_Lose;
bool exit_Game = false;

//functionProts
void Transform(int x, int y, GameObject* target_Object, SceneManager* current_Scene){
    //erase the object
    current_Scene->EraseFromScene(target_Object);
    //Transform the object
    target_Object->Transform(x,y);
    //Draw the object back on the screen
    current_Scene->DrawOnScene(target_Object);
    //This gives a little buffer time to the computer
    //Sleep(REFRESH_TIME);
}


//Async windows Funcs
DWORD WINAPI Update(LPVOID lpParam){
    while(1){
        SceneManager* active_Scene = MainWindow.GetActiveScene();
        //PhysicsSystem
        for(auto game_object = active_Scene->gameObjects.begin(); game_object != active_Scene->gameObjects.end(); ++game_object){
            if((*game_object) != NULL){
                if((*game_object)->isRigidBody){

                    if((*game_object)->position[Y] < Y_MAX  - (*game_object)->height){
                        //updateThePhysic of the object
                        (*game_object)->velocity[X] += (*game_object)->acceleration[X] + gravity_vector[X];
                        (*game_object)->velocity[Y] += (*game_object)->acceleration[Y] + gravity_vector[Y];
                        int posX = (*game_object)->velocity[X]*DT;
                        int posY = (*game_object)->velocity[Y]*DT;
                        Transform(posX, posY, (*game_object), active_Scene);
                    }else{
                        (*game_object)->velocity[X] += (*game_object)->acceleration[X] + gravity_vector[X];
                        int posX = (*game_object)->velocity[X]*DT;
                        Transform(posX, 0, (*game_object), active_Scene);
                    }
                    if((*game_object)->position[X] <= X_BUFFER_FOR_OBJECT|| (*game_object)->position[X] >= X_MAX - (*game_object)->width - X_BUFFER_FOR_OBJECT){
                        (*game_object)->velocity[X] = -((*game_object)->velocity[X]);
                    }
                }
                if((*game_object)->single_Change){
                    MainWindow.GetActiveScene()->DrawOnScene((*game_object));
                    (*game_object)->single_Change = false;
                }
            }
        }
        //PhysicsSystemEnd
        if(scene_Change){
            MainWindow.DisplayActiveScreen(MainWindow.GetActiveScene());
        }
    //This gives a little buffer time to the computer
    Sleep(REFRESH_TIME);
    } 
}
DWORD WINAPI getAsyncInput(LPVOID lpParam){
    while(1){
        current_Input = getch();
        Sleep(100);
        current_Input = '0';
        Sleep(REFRESH_TIME*2);
        
    }
}
DWORD WINAPI bulletController(LPVOID lpParam){
    GameObject* main_Player = NULL;
    GameObject* bullet = NULL;
    GameObject* enemy = NULL;
    while(1){
        main_Player = MainWindow.GetActiveScene()->getObject("mainplayer");
        bullet = MainWindow.GetActiveScene()->getObject("Bullet");
        enemy = MainWindow.GetActiveScene()->getObject("E0");
        if(bullet != NULL && enemy != NULL){
            
            if(abs(bullet->position[X] - enemy->position[X]) < 2.5 && abs(bullet->position[Y] - enemy->position[Y]) < 2.5){
                MainWindow.GetActiveScene()->getObject("E0")->health += -30;
                }
            if(MainWindow.GetActiveScene()->getObject("E0")->health <= 0){
                GameObject Hit("Hit", enemy->position[X], enemy->position[Y]-3, "BOOM", 4,4);
                Hit.isRigidBody=true;
                MainWindow.GetActiveScene()->AddObject(&Hit);
                Sleep(400);
                MainWindow.GetActiveScene()->DrawOnScene(&Hit);
                MainWindow.GetActiveScene()->RemoveObject("Hit");
                MainWindow.GetActiveScene()->RemoveObject("E0");
                current_Enemies--;
                cout << abs(main_Player->position[X] - enemy->position[X]);
            }
        }
        if(enemy!=NULL && main_Player!= NULL){
            if(abs(main_Player->position[X] - enemy->position[X]) < 2&&abs(main_Player->position[Y] - enemy->position[Y]) < 2){
                current_Input ='~';     
            }
        }

        

        Sleep(REFRESH_TIME);
    }
}

//AsyncHelpFunc

//function protos
void gameRoutine();

//Program Entry Point
int main(){
    system("cls");
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
    srand(time(0));
    
    gameRoutine();
    system("cls");


    return 0;
}

void gameRoutine(){
    //begin the update Threads
    HANDLE AsyncUpdate;
    DWORD AsyncUpdateThreadID;
    AsyncUpdate = CreateThread(NULL, 0, Update, NULL, 0, &AsyncUpdateThreadID);
    //UpdateThreadStarted
    
    //Start    
        MainWindow.SetActiveScene(&Scene_Game);
        //Creating Bounding Box
        string boundingBox="";
        for(int y=0; y < Y_MAX; y++){
            for(int x=0; x < X_MAX; x++){

                if(y==0 || y== Y_MAX-1){
                    boundingBox.append("=");
                }else if(x==0 || x == X_MAX-1){
                    boundingBox.append("|");
                }else{
                    boundingBox.append(" ");
                }
            }
        }
        GameObject boundingBoxObject("boundingBox", 0,0, boundingBox, Y_MAX, X_MAX);
        Scene_Game.AddObject(&boundingBoxObject);
        GameObject Title("Title", 10, 2, "===========================================|  ________  __  __ ____ ___ _____ ____   || |__  / _ \\|  \\/  | __ )_ _| ____/ ___|  ||   / / | | | |\\/| |  _ \\| ||  _| \\___ \\  ||  / /| |_| | |  | | |_) | || |___ ___) | || /____\\___/|_|  |_|____/___|_____|____/  |===========================================", 5, 43);
        Scene_Game.AddObject(&Title);
        GameObject information("information", 10, 10, "Hello, Welcome to a Game Made in Mirage Engine.Press F11 to make the terminal Fullscreen.     Hold Y to Start.                               Look at all the controls and HAVE FUN.", 5, 47);
        Scene_Game.AddObject(&information);
        GameObject information2("information2", 10, 15, "Controls:     Fly Up     - WMove Left  - AMove Right - DFire Left  - VFire Right - B--------------Pause Game - P", 5, 14);
        Scene_Game.AddObject(&information2);
        GameObject information3("information3", 10, 25, "WATCH OUT FOR THE RANDOMLY SPAWNING ZOMBIES!", 5, 50);
        Scene_Game.AddObject(&information3);
        GameObject information4("information4", 10, 26, "IF THEY TOUCH YOU, YOU DIE AND THE GAME ENDS!", 5, 50);
        Scene_Game.AddObject(&information4);
        GameObject main_player("mainplayer", 70, 10, "  000  00 00  000    |    /|\\  / | \\   |    / \\  /   \\", 10, 6);
        Scene_Game.AddObject(&main_player);

        bool command_to_continue = false;

        while(!command_to_continue){
            if(current_Input == 'y'){
                
                Scene_Game.RemoveObject("information");
                Scene_Game.RemoveObject("information2");
                Scene_Game.RemoveObject("information3");
                Scene_Game.RemoveObject("information4");
                
                //animation
                for(int frame = 0; frame < 10; frame++){
                    Transform(1, 1, &Title, MainWindow.GetActiveScene());
                    Sleep(100);
                }
                Sleep(200);

                GameObject wiper("wiper", Title.position[X], Title.position[Y], "           ", 5,1);
                Scene_Game.AddObject(&wiper);
                
                for(int frame = 0; frame < 50; frame++){
                    Transform(1, 0, &wiper, MainWindow.GetActiveScene());
                    Sleep(100);
                }
                break;
            }else if(current_Input == 'p'){
                exit(0);
            }
            Sleep(REFRESH_TIME);
        }
    //Start End

    //Update
        //Public Context For The GameLoop
        //local game related variables

        /*string game_Info = "Player Health: ";
        string game_Info_Update = "";*/
        main_player.isRigidBody = true;
        int move_Speed = 1;
        string enemyID = "E0";
        srand(time(0));
        int rand_Cycle = 0;
        int cycle = 0;

        /*GameObject game_Info_Object("game_Info_Object", 5,5, game_Info+game_Info_Update, 1,20);
        Scene_Game.AddObject(&game_Info_Object);*/
        //Begin Bullet Spawning And Collision Checking
        HANDLE hBulletController;
        DWORD BulletControllerThreadID;

        hBulletController = CreateThread(NULL, 0, bulletController, NULL, 0, &BulletControllerThreadID);
        //GameLoop
        while(1){      
            //InputBasedChangeBLOCK
            if(current_Input == 'w' && main_player.position[Y] > 1){
                if(main_player.position[X] > 0 && main_player.position[X] < X_MAX && main_player.position[Y] > 0 && main_player.position[Y] < Y_MAX){
                    main_player.velocity[Y] = 0;main_player.velocity[X] = 0;
                    Transform(0,-move_Speed, &main_player, &Scene_Game);
                }
            }else if(current_Input == 's'){main_player.velocity[X] = 0;}
            else if(current_Input == 'd' && main_player.position[X] < X_MAX - main_player.width - 1){main_player.velocity[X] = 10;}
            else if(current_Input == 'a' && main_player.position[X] > 1){main_player.velocity[X] = -10;}
            else if(current_Input == 'p'){
                //exit sequence
                bool exit_Loop = false;

                while(!exit_Loop){
                    cout.flush();
                    MainWindow.SetActiveScene(&Scene_Pause);
                    system("cls");
                    GameObject GamePause("GamePause", 5, 5, "The Game is Paused. You may press E to Exit, and R to Resume.", 1, 50);
                    MainWindow.GetActiveScene()->AddObject(&GamePause);
                    Sleep(200);
                    bool change = false;
                    while(!change){
                        if(current_Input == 'e'){
                            exit_Game = true;
                            change = true;
                            return;
                        }else if (current_Input == 'r'){
                            MainWindow.SetActiveScene(&Scene_Game);
                            change = true;
                            exit_Loop = true;
                        }
                    }

                }
            }
            else if(current_Input == 'b'){
                GameObject* newDynBullet = new GameObject("Bullet", main_player.position[X], main_player.position[Y], "__",2,2);
                newDynBullet->isRigidBody=true;
                newDynBullet->velocity[X] = 50;
                MainWindow.GetActiveScene()->objectCreator(newDynBullet);
                Sleep(300);

                //get current enemy most probably on the right
                
                MainWindow.GetActiveScene()->RemoveObject("Bullet");  
            }
            else if(current_Input == 'v'){
                GameObject* newDynBullet = new GameObject("Bullet", main_player.position[X], main_player.position[Y], "__",2,2);
                newDynBullet->isRigidBody=true;
                newDynBullet->velocity[X] = -50;
                MainWindow.GetActiveScene()->objectCreator(newDynBullet);
                Sleep(300);
                MainWindow.GetActiveScene()->RemoveObject("Bullet");  
                
            }
            else if(current_Input == '~'){
                cout.flush();
                MainWindow.SetActiveScene(&Scene_Lose);
                system("cls");
                GameObject GameOver("GameOver", 5, 5, "===================================================|  __   _____  _   _   ____ ___ _____ ____    _   ||  \\ \\ / / _ \\| | | | |  _ \\_ _| ____|  _ \\  | |  ||   \\ V / | | | | | | | | | | ||  _| | | | | | |  ||    | || |_| | |_| | | |_| | || |___| |_| | |_|  ||    |_| \\___/ \\___/  |____/___|_____|____/  (_)  ||                                                 |=================================================== ", 7, 51);
                MainWindow.GetActiveScene()->AddObject(&GameOver);
                GameObject GameOverExit("GameOverExit", 5, 15, "SO YOU DIED? Awh. NOW YOU QUIT GAME OK?Now keep ctrl+c pressed to exit", 1, 39);
                MainWindow.GetActiveScene()->AddObject(&GameOverExit);
                Sleep(5000);
                while(1){
                    
                }
            }
            /*else if(current_Input == 'f'){
                MainWindow.GetActiveScene()->getObject("main_player")->health+=10;
                game_Info_Update = to_string(MainWindow.GetActiveScene()->getObject("game_Info_Object")->health);
                MainWindow.GetActiveScene()->getObject("game_Info_Object")->single_Change=true;
            }*/
            if(current_Enemies < 1 && cycle > rand_Cycle){
                    float random_Float = rand_Cycle/1000.0;
                    //range of x pos is XBUFFER --- XMAX-XBUFFER-WIDTH
                    
                    int random_X_Pos = ((X_MAX-X_BUFFER_FOR_OBJECT-10)*random_Float)+X_BUFFER_FOR_OBJECT;

                    GameObject* new_Enemy_Ptr = new GameObject(enemyID,random_X_Pos, 10, "  0 0  0   0 \\0|0/  \\{/    }     {     }    / \\  /   \\", 10, 6);
                    new_Enemy_Ptr->isRigidBody=true;
                    new_Enemy_Ptr->velocity[X]=10;
                    MainWindow.GetActiveScene()->objectCreator(new_Enemy_Ptr);
                    
                    current_Enemies++;                
                    Sleep(500);
                    cycle = 0;
                    rand_Cycle = rand()%1000;
            }
            cycle++;
            Sleep(REFRESH_TIME);           
        }
    //Per Frame Update
}