#include <iostream>
#include <vector>

using namespace std;

int main(){
    vector<int> data;

    for(int i = 0; i< 10; i++ ){
        data.push_back(i);
    }
    
    for(auto i = data.begin(); i != data.end(); ++i){
        cout << *i << " "; 
    }
    cout << endl;

    int removalIndex = 2;

    data.erase(data.begin()+removalIndex);

    for(auto i = data.begin(); i != data.end(); ++i){
        cout << *i << " "; 
    }

    return 0;
}