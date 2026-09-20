#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <chrono>
#include <vector>
#include <ctime>

using namespace std;

bool kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }
    return false;
}

char getch_nonblock() {
    return getchar();
}

vector<vector<vector<int>>> SHAPES = {
    // I
    {{0,0,0,0},
     {1,1,1,1},
     {0,0,0,0},
     {0,0,0,0}},
    // O
    {{0,0,0,0},
     {0,1,1,0},
     {0,1,1,0},
     {0,0,0,0}},
    // T
    {{0,0,0,0},
     {0,1,0,0},
     {1,1,1,0},
     {0,0,0,0}},
    // S
    {{0,0,0,0},
     {0,1,1,0},
     {1,1,0,0},
     {0,0,0,0}},
    // Z
    {{0,0,0,0},
     {1,1,0,0},
     {0,1,1,0},
     {0,0,0,0}},
    // J
    {{0,0,0,0},
     {1,0,0,0},
     {1,1,1,0},
     {0,0,0,0}},
    // L
    {{0,0,0,0},
     {0,0,1,0},
     {1,1,1,0},
     {0,0,0,0}}
};

vector<vector<int>> rotate(vector<vector<int>> shape) {
    vector<vector<int>> rotated(4, vector<int>(4, 0));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            rotated[j][3 - i] = shape[i][j];
        }
    }
    return rotated;
}

int main(){
    vector<int> x;
    vector<int> y;
    bool stop = false, flag = false, flag2 = false, flag3 = false, flag4 = false;
    srand(time(0));
    int height = 20, wide = 10, X = 2, Y = -4;
    int fig = rand() % 7;
    int score = 0;
    vector<vector<int>> currentShape = SHAPES[fig];

    while(stop == false){
        Y++;
        cout << "\033[2J\033[H" << flush;

        if(kbhit() == true){
            char key = getch_nonblock();

            if (key == 'q'){
                stop = true;
            }

            if(key == 'a'){
                if(flag3 == false){
                    X--;
                }
            }

            if(key == 'd'){
                if(flag4 == false){
                    X++;
                }
            }

            if (key == 'w') {
                if (key == 'w') {
    vector<vector<int>> rotated = rotate(currentShape);

    bool canHere = true;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (rotated[i][j] == 1) {
                int fx = X + j;
                int fy = Y + i;
                if (fx < 1 || fx >= wide - 1) canHere = false;
                if (fy >= height - 1) canHere = false;
                for (int k = 0; k < x.size(); k++)
                    if (x[k] == fx && y[k] == fy) canHere = false;
            }

    if (canHere == true) {
        currentShape = rotated;
    }
    else {
        bool canRight = true;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (rotated[i][j] == 1) {
                    int fx = X + 1 + j;
                    int fy = Y + i;
                    if (fx < 1 || fx >= wide - 1) canRight = false;
                    if (fy >= height - 1) canRight = false;
                    for (int k = 0; k < x.size(); k++)
                        if (x[k] == fx && y[k] == fy) canRight = false;
                }

        bool canLeft = true;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                if (rotated[i][j] == 1) {
                    int fx = X - 1 + j;
                    int fy = Y + i;
                    if (fx < 1 || fx >= wide - 1) canLeft = false;
                    if (fy >= height - 1) canLeft = false;
                    for (int k = 0; k < x.size(); k++)
                        if (x[k] == fx && y[k] == fy) canLeft = false;
                }

        if (canRight == true) {
            X++;
            currentShape = rotated;
        }
        else if (canLeft == true) {
            X--;
            currentShape = rotated;
        }
    }
}
            }
        }

        flag3 = false;
        flag4 = false;

        for(int i = 0; i < height; i++){
            for(int j = 0; j < wide; j++){
                flag2 = false;

                if(i == height - 1){
                    cout << "¯¯";
                }
                else if(i == 0){
                    cout << "__";
                }
                else if(j == 0){
                    cout << "| ";
                }
                else if(j == wide - 1){
                    cout << " |";
                }
                else if (i >= Y && i < Y + 4 && j >= X && j < X + 4) {
                    int shapeRow = i - Y;
                    int shapeCol = j - X;
                    if (shapeRow >= 0 && shapeRow < 4 && shapeCol >= 0 && shapeCol < 4) {
                        if (currentShape[shapeRow][shapeCol] == 1) {
                            cout << "[]";

                            if(i + 1 == 19){
                                flag = true;
                            }
                            if(j - 1 == 0){
                                flag3 = true;
                            }
                            if(j + 1 == 9){
                                flag4 = true;
                            }

                            for(int k = 0; k < y.size(); k++){
                                if(y[k] == i && x[k] == j - 1){
                                    flag3 = true;
                                }
                                if(y[k] == i && x[k] == j + 1){
                                    flag4 = true;
                                }
                                if(y[k] == i + 1 && j == x[k]){
                                    flag = true;
                                }
                            }
                        }
                        else {
                            for(int k = 0; k < x.size(); k++){
                                if(x[k] == j && y[k] == i){
                                    cout << "[]";
                                    flag2 = true;
                                }
                            }
                            if(flag2 == false){
                                cout << "  ";
                            }
                        }
                    }
                }
                else {
                    for(int k = 0; k < x.size(); k++){
                        if(x[k] == j && y[k] == i){
                            cout << "[]";
                            flag2 = true;
                        }
                    }
                    if(flag2 == false){
                        cout << "  ";
                    }
                }
            }
            cout << "\n";
        }

        if(flag == true){
            for (int sy = 0; sy < 4; sy++) {
                for (int sx = 0; sx < 4; sx++) {
                    if (currentShape[sy][sx] == 1) {
                        int newX = X + sx;
                        int newY = Y + sy;
                        bool exists = false;
                        for (int k = 0; k < x.size(); k++) {
                            if (x[k] == newX && y[k] == newY) {
                                exists = true;
                                break;
                            }
                        }
                        if (exists == false) {
                            x.push_back(newX);
                            y.push_back(newY);
                        }
                    }
                }
            }

            for (int i = height - 2; i >= 1; i--) {
                bool full = true;
                for (int j = 1; j < wide - 1; j++) {
                    bool found = false;
                    for (int k = 0; k < x.size(); k++) {
                        if (x[k] == j && y[k] == i) {
                            found = true;
                            break;
                        }
                    }
                    if (found == false) {
                        full = false;
                        break;
                    }
                }
                if (full == true) {
                    vector<int> nx, ny;
                    for (int k = 0; k < x.size(); k++) {
                        if (y[k] != i) {
                            nx.push_back(x[k]);
                            ny.push_back(y[k]);
                        }
                    }
                    for (int k = 0; k < ny.size(); k++) {
                        if (ny[k] < i) ny[k] = ny[k] + 1;
                    }
                    x = nx;
                    y = ny;
                    score = score + 10;
                    i = i + 1;
                }
            }

            fig = rand() % 7;
            currentShape = SHAPES[fig];
            Y = -4;
            flag = false;

            bool canSpawn = true;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (currentShape[i][j] == 1) {
                    for (int k = 0; k < x.size(); k++) {
                            if (x[k] == X + j && y[k] == Y + i) canSpawn = false;
                        }
                }
                }
            }
            if (canSpawn == false) stop = true;
        }

        if(fig == 0 && X >= 6){
            X = 5;
        }
        if(fig != 0 && X > 6){
            X = 6;
        }
        if(X <= 0 && fig != 1){
            X = 1;
        }
        if(X < 0 && fig == 1){
            X = 0;
        }

        cout << "Счёт: " << score << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    return 0;
}
