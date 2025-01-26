#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h> 
#include <chrono> 
#include <iomanip>

using namespace std;
using namespace std::chrono;

#define LC(A, B) pipes[A][last_full(pipes, A)] == pipes[B][last_full(pipes, B)] 

void clear_screen() {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}

int show_stopwatch(duration<double> elapsed) {
    int minutes = elapsed.count() / 60;
    int seconds = elapsed.count() - minutes * 60;
    cout << "Time: " << minutes << ":" << fixed << setprecision(2) << seconds << endl;
    return 0;
}

int full(int** pipes, int a) {
    for (int i = 0; i < 4; i++) {
        if (pipes[a][i] == 0) {
            return 0;
        }
    }
    return 1;
}

int half_closed(int** pipes, int a) {
    int base = pipes[a][0];
    for (int i = 1; i < 4; i++) {
        if (pipes[a][i] != base) {
            return 0;
        }
    }
    return 1;
}

int all_closed(int** pipes) {
    for (int i = 0; i < 10; i++) {
        if (!half_closed(pipes, i)) return 0;
    }
    return 1;
}

int first_free(int** pipes, int a) {
    for (int i = 0; i < 4; i++) {
        if (pipes[a][i] == 0) {
            return i;
        }
    }
    return -1;
}

int last_full(int** pipes, int a) {
    for (int i = 3; i >= 0; i--) {
        if (pipes[a][i] != 0) {
            return i;
        }
    }
    return -1;
}

int move_possible(int** pipes, int a, int b) {
    int ret = 0;
    if (
        first_free(pipes, a) != 0 &&
        !full(pipes, b) &&
        first_free(pipes, b) == 0
    ) ret = 1;
    else if (
        first_free(pipes, a) != 0 &&
        !full(pipes, b) &&
        first_free(pipes, b) != 0 &&
        LC(a,b)
    ) ret = 1;
    if (half_closed(pipes, a)) ret = 0;

    return ret;
}

int move(int** pipes, int a, int b) {
    if (move_possible(pipes, a, b)) {
        pipes[b][first_free(pipes, b)] = pipes[a][last_full(pipes, a)];
        pipes[a][last_full(pipes, a)] = 0;
        if (pipes[a][last_full(pipes, a)] == pipes[b][last_full(pipes, b)]) {
            if (move_possible(pipes, a, b)) {
                move(pipes, a, b);
            }
        }
    }
    return 0;
}

int base(int** pipes) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 4; j++) {
            pipes[i][j] = 0;
        }
    }
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            pipes[i][j] = i + 1;
        }
    }
    return 0;
}

int shuffle(int** pipes) {
    srand(time(0));
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            int a = rand() % 6;
            int b = rand() % 4;
            int tmp = pipes[i][j];
            pipes[i][j] = pipes[a][b];
            pipes[a][b] = tmp;
        }
    }
    return 0;
}

int print_pipes(int** pipes) {
    for (int i = 0; i < 10; i++) {
        if (i == 9) {
            cout << "10:";
        } else {
            cout << i + 1 << ": ";
        }
        
        for (int j = 0; j < 4; j++) {
            switch(pipes[i][j]) {
                case 0: cout << " - "; break;
                case 1: cout << " R "; break;
                case 2: cout << " G "; break;
                case 3: cout << " B "; break;
                case 4: cout << " Y "; break;
                case 5: cout << " P "; break;
                case 6: cout << " W "; break;
            }
        }
        cout << '\n';
    }
    return 0;
}

int main() {
    int** pipes = new int*[10];
    int steps = 0; 
    for (int i = 0; i < 10; i++) {
        pipes[i] = new int[4];
    }

    auto start = high_resolution_clock::now(); 

    while (true) {
        clear_screen();
        
        base(pipes);
        shuffle(pipes);

        while (true) {
            clear_screen(); 
            auto now = high_resolution_clock::now();
            auto elapsed = duration_cast<duration<double>>(now - start);
            show_stopwatch(elapsed); 
            cout << "Steps: " << steps << endl;
            print_pipes(pipes);
            int a, b;
            cout << "Move from pipe A to B: ";
            cin >> a >> b;
            if (a == b || a > 10 || b > 10 || a < 1 || b < 1 || !move_possible(pipes, a-1, b-1)) {
                cout << "Not possible \n";
                Sleep(3000); 
            } else {
                move(pipes, a-1, b-1);
                steps++;
            }
            if (all_closed(pipes)) {
                print_pipes(pipes);
                cout << "WON!"<<endl;
                cout <<"Enter 1 to start a new game: "<<endl;
                int new_game;
                cin >> new_game;
                if (new_game == 1) {
                    start = high_resolution_clock::now();
                    steps=0;
                    break; 
                } else {
                    cout << "Goodbye!" << endl;
                    return 0; 
                }
            }
        }
    }

    return 0;
}
