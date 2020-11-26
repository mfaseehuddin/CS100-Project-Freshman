#include <iostream>
#include <time.h>
using namespace std;

int main(){

    time_t ctime;

    ctime = time(NULL);
    
    cout << ctime;

    return 0;
}