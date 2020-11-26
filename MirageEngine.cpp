
#include<iostream>
#include <windows.h>
#include <vector>
#include <conio.h>
#include <time.h>
using namespace std;


const int X = 0;
const int Y = 1;
const int X_MAX = 150;
const int Y_MAX = 50;
const int MAX_OBJECTS = 20;
const int REFRESH_TIME = 15;

bool change = true;
char current_Input = '0';



//Class Definitions
//GameObject
class GameObject{
    public:
        string id;
        string graphic;
        int height;
        int width;
        int position[2];
        bool state_Changed = true;
        GameObject(string i_id, int i_x, int i_y, string i_graphic, int i_width);
        void Transform(int x, int y);
};
GameObject::GameObject(string i_id, int i_x, int i_y, string i_graphic, int i_width){
    id = i_id;
    position[X] = i_x;
    position[Y] = i_y;
    graphic = i_graphic;
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
            scene[object->position[Y] + current_Height][object->position[X] + x] = current_Graphic[graphicIndex];
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
            scene[object->position[Y] + current_Height][object->position[X] + x] = ' ';
            x++;
            graphicIndex++;
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
    
    //Sleep(REFRESH_TIME);
    SetCursorPosition(1,1);
    cout << "x";
    change = false;

}
//WindowCLASSEND

//GLOBAL VARIABLES
Window MainWindow;
SceneManager Scene1;

//functionProts
//Async windows Funcs
DWORD WINAPI Update(LPVOID lpParam){
    while(1){
        if(change){
            SceneManager* current_Scene = MainWindow.GetActiveScene();
            for(auto game_object = (*current_Scene).gameObjects.begin(); game_object != (*current_Scene).gameObjects.end(); ++game_object){
                    (*current_Scene).DrawOnScene(*game_object);
                
            }
            MainWindow.DisplayActiveScreen(current_Scene);
        }
        Sleep(REFRESH_TIME);
    }
}
DWORD WINAPI getAsyncInput(LPVOID lpParam){
    while(1){
        current_Input = getch();
        Sleep(REFRESH_TIME);
        current_Input = '0';
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
    
    //Scene1.Debug();    
    //Creating Bounding Box
    //begin the update Thread
    {
    HANDLE AsyncUpdate;
    DWORD AsyncUpdateThreadID;
    AsyncUpdate = CreateThread(NULL, 0, Update, NULL, 0, &AsyncUpdateThreadID);
    }
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
        GameObject boundingBoxObject("boundingBox", 0,0, boundingBox, X_MAX);
        Scene1.AddObject(&boundingBoxObject);

        GameObject information("information", 10, 10, "Hello, Welcome to a Game Made in Mirage Engine.Press F11 to make the terminal Fullscreen.     Hold Y to Start.                               Press P at anytime to quit the game.", 47);
        Scene1.AddObject(&information);
        
        GameObject main_player("mainplayer", 70, 10, "  000  00 00  000    |    /|\\  / | \\   |    / \\  /   \\", 6);
        Scene1.AddObject(&main_player);

        bool command_to_continue = false;

        while(!command_to_continue){
            
            Sleep(100);
            Scene1.EraseFromScene(&main_player);
            main_player.Transform(0,-1);
            if(current_Input == 'y'){
                break;
            }else if(current_Input == 'p'){
                exit(1);
            }
            Sleep(100);
            Scene1.EraseFromScene(&main_player);
            main_player.Transform(0,1);

            if(current_Input == 'y'){
                break;
            }
        }

        Scene1.RemoveObject("information");

    //Start End

    //Update
        while(1){
            if(current_Input == 'w'){
                Scene1.EraseFromScene(&main_player);
                Sleep(30);
                main_player.Transform(0,-1);
            }else if(current_Input == 's'){
                Scene1.EraseFromScene(&main_player);
                Sleep(30);
                main_player.Transform(0,1);
            }else if(current_Input == 'd'){
                Scene1.EraseFromScene(&main_player);
                Sleep(30);
                main_player.Transform(1,0);
            }else if(current_Input == 'a'){
                Scene1.EraseFromScene(&main_player);
                Sleep(30);
                main_player.Transform(-1,0);
            }else if(current_Input == 'p'){break;}
            Sleep(REFRESH_TIME/2);
        }
    //Per Frame Update

}