#include<iostream>
#include <windows.h>
#include <vector>
#include <conio.h>
#include <time.h>
using namespace std;


const int X = 0;
const int Y = 1;
const int X_MAX = 150;
const int Y_MAX = 30;
const int MAX_OBJECTS = 20;
const int REFRESH_TIME = 15;
const float DT = 0.1; 
const int X_BUFFER_FOR_OBJECT = 10;


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
        int position[2] = {0,0};
        float velocity[2] = {0,0};
        float acceleration[2] = {0,0};
        bool exists = true;
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
        void RemoveObject(string object_ID);
        void DrawOnScene(GameObject*); 
        void EraseFromScene(GameObject*);
        void Debug();
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
            break;
        }
    }
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
    cout << "x";
    scene_Change = false;

}
//WindowCLASSEND

//GLOBAL VARIABLES
Window MainWindow;
SceneManager Scene1;


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
        }
        //PhysicsSystem
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

//User Defined Async Funcs for GameService
        vector<GameObject> balls;
        string ball_Id = "ball0";
        int number_of_balls = 65;
        bool bulletFire = false;
        bool destroyBullet = false;

        int enemy_x = X_MAX - 50;
        int enemy_y = 10;

        int main_Player_Position[2]= {0,0};

        int Enemy_Number = 65;
        string Enemy_Spawn_ID = "enemy0";
        bool Spawn_Enemy = false;
        vector<GameObject> Enemy_Spawn_Manager;

DWORD WINAPI enemyController(LPVOID lpParam){
    while(1){
        if(Spawn_Enemy){
            
            //EnemyIDSetting And Incrementing
            Enemy_Spawn_ID[Enemy_Spawn_ID.length()-1] = Enemy_Number;
            Enemy_Number++;

            //Producing Random Enemy COORDS


            //Producing Enemy And Putting in 
            GameObject new_Enemy(Enemy_Spawn_ID, enemy_x, enemy_y, "  0 0  0   0 \\0|0/  \\{/    }     {     }    / \\  /   \\", 10, 6);
            new_Enemy.isRigidBody = true;

            Enemy_Spawn_Manager.push_back(new_Enemy);
            
            MainWindow.GetActiveScene()->AddObject(&Enemy_Spawn_Manager[Enemy_Spawn_Manager.size()-1]);
            Spawn_Enemy = false;
        }
        

        //Enemy Kill and Collision Detection
        int nballs = balls.size();
        int nenemies = Enemy_Spawn_Manager.size();

        if(nballs > 0 && nenemies > 0){
            int ballX = (*balls.begin()).position[X];
            int enemyX = (*Enemy_Spawn_Manager.begin()).position[X]; 
            int enemyY = (*Enemy_Spawn_Manager.begin()).position[Y];
            if(ballX >= enemyX){
                GameObject hit("hit", enemyX-1, enemyY, "ouch",2,1 );
                MainWindow.GetActiveScene()->AddObject(&hit);
                Sleep(200);        
                MainWindow.GetActiveScene()->RemoveObject("hit");
                MainWindow.GetActiveScene()->RemoveObject(Enemy_Spawn_ID);
                Enemy_Spawn_Manager.pop_back();
                Sleep(100);
                GameObject hit2("hit2", enemyX, enemyY, "haw mujhe mar diya",2,20 );
                MainWindow.GetActiveScene()->AddObject(&hit2);
                Sleep(100);
                MainWindow.GetActiveScene()->RemoveObject("hit2");
            }
        }
        
        Sleep(50);
    }
}

DWORD WINAPI bulletController(LPVOID lpParam){
    while(1){
        if(bulletFire){
            ball_Id[ball_Id.length()-1] = number_of_balls;
            number_of_balls++;

            GameObject ball(ball_Id, main_Player_Position[X], main_Player_Position[Y], "____", 2, 5);
            ball.isRigidBody=true;
            ball.velocity[X]=50;
            
            balls.push_back(ball);
            
            MainWindow.GetActiveScene()->AddObject(&balls[balls.size()-1]);
            bulletFire = false;
            Sleep(500);


            
            MainWindow.GetActiveScene()->RemoveObject(ball_Id);
                balls.pop_back();

            
        }
        //Check for collision with enemy

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

    return 0;
}

void gameRoutine(){
    //begin the update Threads
    HANDLE AsyncUpdate;
    DWORD AsyncUpdateThreadID;
    AsyncUpdate = CreateThread(NULL, 0, Update, NULL, 0, &AsyncUpdateThreadID);
    //UpdateThreadStarted
    
    //Start    
        MainWindow.SetActiveScene(&Scene1);
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
        Scene1.AddObject(&boundingBoxObject);

        GameObject information("information", 10, 10, "Hello, Welcome to a Game Made in Mirage Engine.Press F11 to make the terminal Fullscreen.     Hold Y to Start.                               Press P at anytime to quit the game.", 5, 47);
        Scene1.AddObject(&information);
        
        GameObject main_player("mainplayer", 70, 10, "  000  00 00  000    |    /|\\  / | \\   |    / \\  /   \\", 10, 6);
        Scene1.AddObject(&main_player);

        bool command_to_continue = false;

        while(!command_to_continue){
            if(current_Input == 'y'){
                break;
            }else if(current_Input == 'p'){
                exit(0);
            }
            Sleep(REFRESH_TIME);
        }
        Scene1.RemoveObject("information");
    //Start End

    //Update
        //local game related variables
        main_player.isRigidBody = true;
        int move_Speed = 1;
       

        //Public Context For The GameLoop
        


        //Begin Enemy Spawning And Handling
        HANDLE hEnemyController;
        DWORD EnemyControllerThreadID;

        hEnemyController = CreateThread(NULL, 0, enemyController, NULL, 0, &EnemyControllerThreadID);
        //Begin Bullet Spawning And Collision Checking
        HANDLE hBulletController;
        DWORD BulletControllerThreadID;

        hBulletController = CreateThread(NULL, 0, bulletController, NULL, 0, &BulletControllerThreadID);
        

        //GameLoop
        while(1){
            
            //InputBasedChangeBLOCK
            if(current_Input == 'w' && main_player.position[Y] > 1){
                if(main_player.position[X] > 0 && main_player.position[X] < X_MAX && main_player.position[Y] > 0 && main_player.position[Y] < Y_MAX){
                    main_player.velocity[Y] = 0;
                    Transform(0,-move_Speed, &main_player, &Scene1);
                }
            }else if(current_Input == 's'){main_player.velocity[X] = 0;}
            else if(current_Input == 'd' && main_player.position[X] < X_MAX - main_player.width - 1){main_player.velocity[X] = 10;}
            else if(current_Input == 'a' && main_player.position[X] > 1){main_player.velocity[X] = -10;}
            else if(current_Input == 'p'){exit(0);}
            else if(current_Input == 'b'){
                main_Player_Position[X] = main_player.position[X];
                main_Player_Position[Y] = main_player.position[Y];
                bulletFire = true;
                Sleep(500);
            }else if(current_Input == 'e'){
                main_Player_Position[X] = main_player.position[X];
                main_Player_Position[Y] = main_player.position[Y];
                Spawn_Enemy = true;
                Sleep(500);
            }
            

            Sleep(REFRESH_TIME);
            
        }
    //Per Frame Update

}