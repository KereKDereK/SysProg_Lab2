#include <sys/types.h>
#include <iostream>
#include <wait.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <time.h>
#include "check.hpp"
using namespace std;
 const char* fifatry =  "/tmp/fifotry";
 const char* fifares =  "/tmp/fifores";
 int cbt = 0;

void randValue(){
    int tmp = rand() % 10 + 1;


}

void writer(int potok, int buffer,const char* path){
    potok = open(path, O_WRONLY);
    write(potok, &buffer, sizeof(buffer));
    ftruncate(potok, 0);
    close(potok);
}
int reader (int potok,int buffer, const char* path) {
    potok = open (path, O_RDONLY);
    read(potok, &buffer, sizeof(buffer));
    close(potok);
    return buffer;
}

int randomaizer() {
    int a = rand() % 10 + 1;
    return a;
}
int checking(int potok,int &buffer,const char* path,const int value) {
    sleep(1);
    if (buffer == value) {
        cout << "Your try is true"<< endl;
        buffer = 1;
        writer(potok,buffer,path);
        return 1;
    }
    else if (buffer != value) {
        cout << "Your try is false"<< endl;
        buffer = 0;
        writer(potok,buffer,path);
        return 0;
    }
}

void gamer1 (int trying, int resulting){
        int tryes = 0;
        int buffer;
        int x;
        int condition = 0;
        int firstValue = randomaizer();
        do {
            ++tryes;
            cout << "-------------------------"<<tryes<<"------------------------" << endl;
            cout << "Number is: " << firstValue << endl;
            trying = open("/tmp/fifotry", O_RDONLY);
            x = check(read(trying, &buffer, sizeof(buffer)));
            condition = checking(resulting,buffer,fifares,firstValue);
            close (trying);
            cout << "-------------------------------------------------" << endl;
        } while (condition == 0);
        cout << "Total tryes count : " << tryes << endl;
}

void gamer2 (int trying, int resulting){
    int condition = 0;
    int check = 0;
    do {
        int secondValue = rand()%10+1;;
        sleep(1);
        secondValue = rand()%10+1;;
        writer(trying,secondValue,fifatry);
        cout << "Trying: " << secondValue << endl;
        check = reader(resulting,check,fifares);
        if (check == 1)
            ++condition;
    } while (condition == 0);
}








int main() {
    srand(time(0));
    pid_t pid = fork();
    unlink ("/tmp/fifotry");
    unlink ("/tmp/fifores");
    mkfifo ("/tmp/fifotry", S_IRUSR | S_IWUSR);
    mkfifo ("/tmp/fifores", S_IRUSR | S_IWUSR);
    int trying;
    int resulting;
    int table_turning = 0;
    int cbt = 1;

    if (pid) {
        do {
            cout << "Current attempt: " << cbt << endl;
            sleep(1);
            if (!table_turning) {
                ++cbt;
                writer(resulting, cbt, fifares);
                cout << "Current host: parent" << endl;
                gamer1(trying, resulting);
                table_turning = 1;
            } else {
                reader(resulting, cbt, fifares);
                cbt++;
                gamer2(trying, resulting);
                table_turning = 0;
            }
        } while (cbt != 3);
        wait(NULL);
        exit(0);
    }
    else {
        do {
            if (!table_turning) {
                reader(resulting, cbt, fifares);
                cbt++;
                gamer2(trying, resulting);
                table_turning = 1;
            } else {
                ++cbt;
                writer(resulting, cbt, fifares);
                cout << "Current host: child" << endl;
                gamer1(trying, resulting);
                table_turning = 0;
            }
        } while (cbt != 3);
        exit(0);
    }
    return 0;
}