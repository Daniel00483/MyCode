#include<iostream>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctime>
#include<windows.h>
using namespace std;
int T[13][22], color[8] = { 0,6,5,3,14,4,1,9 },score;
class Tetris {
public:
    int type;
    pair<int, int>center, block[4];
    Tetris(int type) {
        switch (type) {
        case 1://Z型
            center = { 6,2 }, block[0] = { 0,0 }, block[1] = { -1,-1 }, block[2] = { 0,-1 }, block[3] = { 1,0 };
            break;
        case 2://S型
            center = { 6,2 }, block[0] = { 0,0 }, block[1] = { -1,0 }, block[2] = { 0,-1 }, block[3] = { 1,-1 };
            break;
        case 3://I型
            center = { 6,1 }, block[0] = { 0,0 }, block[1] = { -1,0 }, block[2] = { 1,0 }, block[3] = { 2,0 };
            break;
        case 4://T型
            center = { 6,2 }, block[0] = { 0,0 }, block[1] = { -1,0 }, block[2] = { 0,-1 }, block[3] = { 1,0 };
            break;
        case 5://O型
            center = { 6,2 }, block[0] = { 0,0 }, block[1] = { 0,-1 }, block[2] = { 1,-1 }, block[3] = { 1,0 };
            break;
        case 6://J型
            center = { 6,1 }, block[0] = { -1,0 }, block[1] = { -1,1 }, block[2] = { 0,1 }, block[3] = { 1,1 };
            break;
        case 7://L型
            center = { 6,1 }, block[0] = { -1,1 }, block[1] = { 0,1 }, block[2] = { 1,0 }, block[3] = { 1,1 };
            break;
        }
    }
    void setcolor(int color) {
        HANDLE hConsole;
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }
    bool out_of_bound(int MoveType, int direction) {
        bool out = false;
        if (MoveType == 0) {//movetype=0橫移,movetype=1掉落,movetype=2旋轉
            for (int i = 0; i < 4; i++) {
                int new_x = center.first + block[i].first + direction;
                if (new_x <= 0 || new_x > 12 || T[new_x][center.second + block[i].second] != 0)out = true;
            }
        }
        else if (MoveType == 1) {
            for (int i = 0; i < 4; i++) {
                int new_y = center.second + block[i].second + 1;
                if (new_y > 21 || T[center.first + block[i].first][new_y] != 0)out = true;
            }
        }
        else if (MoveType == 2) {
            for (int i = 0; i < 4; i++) {
                int new_x = center.first + block[i].second * (-direction), new_y = center.second + block[i].first * direction;
                if (new_x <= 0 || new_x > 12 || new_y <= 0 || new_y > 21 || T[new_x][new_y] != 0) {
                    out = true;
                }
            }
        }
        return out;
    }
    void update_on_T(int x, int type) {
        for (int i = 0; i < 4; i++) {
            T[center.first + block[i].first][center.second + block[i].second] += x * type;
        }
    }
    void rotate_block(int direction) {//順時針為1 逆時針為-1
        update_on_T(-1, type);
        if (!out_of_bound(2, direction)) {
            for (int i = 0; i < 4; i++) {
                swap(block[i].first, block[i].second);
                block[i].first *= -direction, block[i].second *= direction;
            }
        }
        update_on_T(1, type);
    }
    void move_block(int direction) {//右為1 左為-1
        update_on_T(-1, type);
        if (!out_of_bound(0, direction)) {
            center.first += direction;
        }
        update_on_T(1, type);
    }
    bool fall() {
        bool out = true;
        update_on_T(-1, type);
        if (!out_of_bound(1, 1)) {
            out = false;
            center.second++;
        }
        update_on_T(1, type);
        return out;
    }
    void eliminate(int y) {
        for (int x = 1; x <= 12; x++) {
            T[x][y] = 0;
        }
    }
    void show(bool game_over) {
        setcolor(8);
        cout << "  □□□□□□□□□□□□" << "\n";
        for (int y = 1; y <= 21; y++) {
            cout << "｜";
            for (int x = 1; x <= 12; x++) {
                if (T[x][y] != 0) {
                    setcolor(T[x][y]);
                    cout << "■";
                }
                else cout << "　";
            }
            setcolor(8);
            cout << "｜";
            if (y == 10) {
                setcolor(10);
                cout << "           score: " << score;
            }
            else if (y == 8 && game_over) {
                setcolor(3);
                cout << "    ******GAME OVER!!!******";
            }
            setcolor(8);
            cout << "\n";
        }
        setcolor(8);
        cout << "  □□□□□□□□□□□□" << "\n";
    }
    void putdown(int start_y) {
        for (int x = 1; x <= 12; x++) {
            T[x][start_y] = 0;
        }
        for (int y = start_y; y >= 2; y--) {
            for (int x = 1; x <= 12; x++) {
                T[x][y] = T[x][y - 1];
            }
        }
    }
    void eliminate() {
        int eliminate_line=0;
        for (int y = 21; y >= 2; y--) {
            for (int x = 1; x <= 12; x++) {
                if (T[x][y] == 0)break;
                else if (x == 12 && T[x][y] != 0) {
                    putdown(y);
                    y++,eliminate_line++;
                }
            }
        }
        score += 100 * (1 << eliminate_line-1);
    }
    bool gameover() {
        bool over = false;
        for (int i = 0; i < 4; i++) {
            if (T[center.first + block[i].first][center.second + block[i].second] != 0)over = true;
        }
        if (over)center.second--;
        return over;
    }
    void gameover_show() {
        for (int i = 0;i < 4; i++) {
            if (center.second + block[i].second >= 1 && T[center.first + block[i].first][center.second + block[i].second] == 0)T[center.first + block[i].first][center.second + block[i].second] = type;
        }
        show(true);
    }
};
int main() {
    int fall_time = -150, move_time = -100;
    while (true) {
        srand(time(NULL));
        int typ = rand() % 7 + 1;
        Tetris tetris(typ);
        tetris.type = typ;
        if (tetris.gameover()) {
            Sleep(200);
            system("cls");
            tetris.gameover_show();
            break;
        }
        tetris.update_on_T(1, typ);
        system("cls");
        tetris.show(false);
        bool on_bottom = false;
        while (!on_bottom) {
            if (clock() - fall_time >= 300) {
                on_bottom = tetris.fall();
                system("cls");
                tetris.show(false);
                fall_time = clock();
            }
            if (_kbhit()) {
                int key = _getch();
                if (key == 32) {
                    if (clock() - fall_time >= 50) {
                        on_bottom = tetris.fall();
                        system("cls");
                        tetris.show(false);
                        fall_time = clock();
                    }
                }
                else if (clock() - move_time >= 100) {
                    if (key == 224) {
                        key = _getch();
                        if (key == 75)tetris.move_block(-1);
                        else if (key == 77)tetris.move_block(1);
                        else if (key == 72)tetris.rotate_block(1);
                    }
                    else if (key == 88 || key == 120)tetris.rotate_block(-1);
                    else if (key == 67 || key == 99)tetris.rotate_block(1);
                    else return 0;
                    system("cls");
                    tetris.show(false);
                    move_time = clock();
                }
            }
            if (on_bottom) {
                tetris.eliminate();
                Sleep(100);
                system("cls");
                    tetris.show(false);
            }
        }
    }
    return 0;
}
