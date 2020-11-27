#include<iostream>
#include <windows.h>
#include <vector>
#include <conio.h>
#include <time.h>
using namespace std;


const int X = 0;
const int Y = 1;
const int X_MAX = 150;
const int Y_MAX = 40;
const int MAX_OBJECTS = 20;
const int REFRESH_TIME = 15;
const int PHYSICS_REFRESH_TIME = 30;
const float DT = 0.1; 


bool change = true;
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
        int position[2];
        float velocity[2];
        float acceleration[2];
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
        change = true;
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
    int current_Height = 0;
    int x = 0;
    string& current_Graphic = object->graphic;
    for(int graphicIndex = 0; graphicIndex < current_Graphic.length();){
        if(x == object->width){
            x = 0;
            current_Height++;
        }else{
            scene[(object->position[Y]) + current_Height][(object->position[X]) + x] = current_Graphic[graphicIndex];
            x++;
            graphicIndex++;
        }
        
    }
    change = true;
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
            if((object->position[Y]) + current_Height > 0 && (object->position[X]) + x>0){
            scene[(object->position[Y]) + current_Height][(object->position[X]) + x] = ' ';
            x++;
            graphicIndex++;
            }
        }
        
    }
    change = true;
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
    change = false;

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
    change = true;
    //This gives a little buffer time to the computer
    Sleep(REFRESH_TIME);
}


//Async windows Funcs
DWORD WINAPI Update(LPVOID lpParam){
    while(1){
        if(change){
            /*SceneManager* current_Scene = MainWindow.GetActiveScene();
            for(auto game_object = (*current_Scene).gameObjects.begin(); game_object != (*current_Scene).gameObjects.end(); ++game_object){
                    (*current_Scene).DrawOnScene(*game_object);
                
            }*/
            MainWindow.DisplayActiveScreen(MainWindow.GetActiveScene());
        }

        SceneManager* active_Scene = MainWindow.GetActiveScene();
        
        for(auto game_object = active_Scene->gameObjects.begin(); game_object != active_Scene->gameObjects.end(); ++game_object){
            if((*game_object)->isRigidBody){
                if((*game_object)->position[Y] < Y_MAX  - (*game_object)->height){
                    //updateThePhysic of the object
                    (*game_object)->velocity[X] += (*game_object)->acceleration[X] + gravity_vector[X];
                    (*game_object)->velocity[Y] += (*game_object)->acceleration[Y] + gravity_vector[Y];

                    //dx = (*game_object)->velocity[X]*DT
                    //manually erasing the object
                    Transform((*game_object)->velocity[X]*DT, (*game_object)->velocity[Y]*DT, (*game_object), active_Scene);
                }
                if((*game_object)->position[X] <= 0 || (*game_object)->position[X] >= X_MAX){
                    (*game_object)->velocity[X] = -0.5*(*game_object)->velocity[X];
                }
            }
        }


    //This gives a little buffer time to the computer

    }
   
}
DWORD WINAPI getAsyncInput(LPVOID lpParam){
    while(1){
        current_Input = getch();
        Sleep(REFRESH_TIME);
        current_Input = '0';
    }
}
DWORD WINAPI UpdatePhysics(LPVOID lpParam){
    while(1){
        //StepWise Euler Equation is used to simulate gravity
        //All rididBodies are going to be updated
        SceneManager* active_Scene = MainWindow.GetActiveScene();
        
        for(auto game_object = active_Scene->gameObjects.begin(); game_object != active_Scene->gameObjects.end(); ++game_object){
            if((*game_object)->isRigidBody){
                if((*game_object)->position[Y] < (Y_MAX-1) - (*game_object)->height){
                    //updateThePhysic of the object
                    (*game_object)->velocity[X] += (*game_object)->acceleration[X];
                    (*game_object)->velocity[Y] += (*game_object)->acceleration[Y];

                    //dx = (*game_object)->velocity[X]*DT
                    //manually erasing the object
                    Transform((*game_object)->velocity[X]*DT, (*game_object)->velocity[Y]*DT, (*game_object), active_Scene);
                }
            }
        }

        Sleep(REFRESH_TIME*2);
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
    gameRoutine();

    return 0;
}

void gameRoutine(){
    
    Scene1.Debug();    
    //Creating Bounding Box
    //begin the update Threads
    HANDLE AsyncUpdate;
    DWORD AsyncUpdateThreadID;
    AsyncUpdate = CreateThread(NULL, 0, Update, NULL, 0, &AsyncUpdateThreadID);

    //UpdateThreadStarted
    
    //Start    
        MainWindow.SetActiveScene(&Scene1);
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
        
        GameObject main_player("mainplayer", 70, 10, "  000  00 00  000    |    /|\\  / | \\   |    / \\  /   \\", 11, 6);
        Scene1.AddObject(&main_player);

        bool command_to_continue = false;

        while(!command_to_continue){
            
            Transform(0,1,&main_player, &Scene1);
            Sleep(100);
            Transform(0,-1,&main_player, &Scene1);
            Sleep(100);
            if(current_Input == 'y'){
                break;
            }else if(current_Input == 'p'){
                exit(1);
            }

        }

        Scene1.RemoveObject("information");

    //Start End

    //Update
        //local game related variables
        main_player.isRigidBody = true;
        int move_Speed = 1;
        /*string ball_Id = "ball0";
        int number_of_balls = 65;
        bool new_ball = false;
        vector<GameObject>balls;*/

        while(1){
            if(current_Input == 'w' && main_player.position[Y] > 1){
                if(main_player.position[X] > 0 && main_player.position[X] < X_MAX && main_player.position[Y] > 0 && main_player.position[Y] < Y_MAX){
                //
                Transform(0,-move_Speed, &main_player, &Scene1);
                main_player.velocity[Y] = 0;
                }
            }else if(current_Input == 's' && main_player.position[Y] < Y_MAX - main_player.height){
                Transform(0,move_Speed, &main_player, &Scene1);
            }else if(current_Input == 'd'){
                Transform(move_Speed, 0, &main_player, &Scene1);
            }else if(current_Input == 'a' && main_player.position[X] > 1){
                Transform(-move_Speed, 0, &main_player, &Scene1);
            }else if(current_Input == 'p'){exit(0);}
            
            /*else if(current_Input == 'b'){
                //balls.emplace(GameObject(ball_Id, main_player.position[X], main_player.position[Y], ball_Id,1,5));
                ball_Id[ball_Id.length()-1] = number_of_balls;
                GameObject ball(ball_Id, main_player.position[X], main_player.position[Y], ball_Id,2,5);
                balls.push_back(ball);
                number_of_balls++;
                balls[balls.size()-1].isRigidBody=true;
                    balls[balls.size()-1].velocity[X]=50;
                MainWindow.GetActiveScene()->AddObject(&balls[balls.size()-1]);
                Sleep(2500);
                MainWindow.GetActiveScene()->RemoveObject(ball_Id);
                balls.pop_back();
            }*/


            Sleep(REFRESH_TIME);
        }
    //Per Frame Update

}