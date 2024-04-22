#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <sstream>
using namespace std;

class Element{
    private:
    long key;
    string text;

    public:
    Element (){
        key = -1; //liczba przypisywana elementom pustym
        text = "";
    }
    void SetElement (long key_in, string text_in){
        key = key_in;
        text = text_in;
    }

    int GetKey (){ return key;}
    string GetText (){ return text;}

    friend ostream& operator << (ostream & s, const Element & el){
        return s<<el.key<<" "<<el.text;
    }

    Element & operator = (const Element & el){
        if (&el != this){ this->SetElement (el.key, el.text);}
        return *this;
    }
};

void Add (long key_in, string text_in, vector <Element> &HashTable_in){
    int position = key_in % HashTable_in.size();

    if ((HashTable_in [position].GetKey() == -1) || (HashTable_in [position].GetKey() == -2)) HashTable_in [position].SetElement (key_in, text_in);
    else{
        unsigned int n = 0; //liczba prob znalezienia

        while (n < HashTable_in.size()){
            if (position == HashTable_in.size()-1) position = 0; //w przypadku dojscia do kranca tablicy
            if ((HashTable_in [position].GetKey() == -1) || (HashTable_in [position].GetKey() == -2)){
                HashTable_in [position].SetElement (key_in, text_in);
                break;
            }

            position++;
            n++;
        }

        if (n == HashTable_in.size()-1) cout<<"Nie ma miejsca w tablicy!\n";
    }
}

//sprawdza czy sa "dziury" w tablicy lub jest dostepne pasujace miejsce,
//jezeli tak to przesuwa elementy/wstawia na dostepne miejsce
void RehashTable (vector <Element> &HashTable_in, bool deleted){
    if (deleted){
        for (unsigned int i=0; i<HashTable_in.size(); i++){
            if ((HashTable_in [i].GetKey() != -1) && (HashTable_in [i].GetKey() != -2)){
                int position = (HashTable_in [i].GetKey() % HashTable_in.size());

                if (i != position){
                    if ((HashTable_in [position].GetKey() == -1) || (HashTable_in [position].GetKey() == -2)){
                        HashTable_in [position] = HashTable_in [i];
                        HashTable_in [i].SetElement (-2, "");
                    }
                    else{
                        position++;
                        while (position != i){
                            if ((HashTable_in [position].GetKey() == -1) || (HashTable_in [position].GetKey() == -2)){
                                HashTable_in [position] = HashTable_in [i];
                                HashTable_in [i].SetElement (-2, "");
                            }

                            position++;
                            if (position == HashTable_in.size()) position = 0;
                        }
                    }
                }
            }
        }
    }
}

void Delete (long key_in, vector <Element> &HashTable_in, bool &deleted){
    int position = key_in % HashTable_in.size();

    if (HashTable_in [position].GetKey() == -1){
        cout<<"W tablicy nie znajduje sie element o takim kluczu!";
        deleted = false;
    }

    else if (HashTable_in [position].GetKey() == key_in){
        HashTable_in [position].SetElement (-2, ""); //miejsca usuniete w tablicy sa oznaczone kluczem -2
        deleted = true;
    }

    else if (HashTable_in [position].GetKey() != key_in){
        position++;
        unsigned int n = 0; //liczba prob znalezienia

        while (n < HashTable_in.size()){
            if (HashTable_in [position].GetKey() == -1){
                cout<<"W tablicy nie znajduje sie element o takim kluczu!";
                deleted = false;
                break;
            }

            else if (HashTable_in [position].GetKey() == key_in){
                HashTable_in [position].SetElement (-2, "");
                deleted = true;
                break;
            }
            n++;
            position++;
            if (position == HashTable_in.size()) position = 0;
        }

        if (n == HashTable_in.size()-1){
            cout<<"Nie znaleziono takiego elementu!\n"; 
            deleted = false;
        }
    }
}

void Print (vector <Element> &HashTable_in){
    for (unsigned int i=0; i<HashTable_in.size(); i++){
        if ((HashTable_in [i].GetKey() != -1) && (HashTable_in [i].GetKey() != -2)){
            cout<<i<<" "<<HashTable_in [i]<<endl;
        }
    }
    cout<<endl;
}

int main(){
    string line, command, arg2; //zmienne sczytujace komendy
    int arg1;

    bool deleted = false; //zmienna pomocnicza do funkcji Rehash
    string lastCommand = ""; //w przypadku kilku deletow, rehashuje po wykonaniu ostatniego

    int n; //ilosc przypadkow testowych
    getline (cin, line);
    n = stoi (line);
    
    vector <Element> Table;
    while (n>0){
        while (getline (cin, line)){
            stringstream ss (line);
            ss >> command >> arg1 >> arg2;

            if ((lastCommand == "delete") && (command != "delete")){
                RehashTable (Table, deleted);
            }
            if (command == "size"){
                Table.resize (arg1);
            }
            if (command == "add"){
                Add (arg1, arg2, Table);
            }
            if (command == "delete"){
                Delete (arg1, Table, deleted);
            }
            if (command == "print"){
                Print (Table);
            }
            if (command == "stop"){
                Table.clear();
                break;
            }

            lastCommand = command;
        }
        n--;
    }
    return 0;
}