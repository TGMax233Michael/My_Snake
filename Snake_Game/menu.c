#include "menu.h"
#include "game.h"
#include <windows.h>
#include <conio.h>
#include <stdio.h>

char Menu_Text[4][20] = {
    "Start Game",
    "Game Setting",
    "Rank",
    "Quit"
};

char Game_Difficulty_Setting[6][10] = {
    "Noob",
    "Easy",
    "Normal",
    "Hard",
    "Extreme",
    "Back"
};

char Map_Scale_Setting[4][10] = {
    "Small",
    "Normal",
    "Large",
    "Back"
};

char Game_Setting[4][12] = {
    "Difficulty",
    "Map Scale",
    "Snake Color",
    "Back"
};

char Color_Setting[6][10] = {
    "White",
    "Red",
    "Yellow",
    "Blue",
    "Green",
    "Back"
};

char Rank[3][6] = {
    "Month",
    "Year",
    "Back"
};

int difficulty;
int map_scale;
int color = WHITE;
data_node *_head = NULL;
HANDLE hdl_menu;
// ##########################################################################################################
// 声明
void display_main_menu(int pos);

void display_difficulty_menu(int pos);

void display_setting_menu(int pos);

int move_ptr(int pos, int max_index, char ch);

void choose_main(int pos, int difficulty, int map_scale);

void choose_setting(int pos);

void read_data();

void set_text_color(int color);

void display_rank(int command);

int menu_get_time(int command);

void display_current_game_settings();

void display_rank_info_special();
// ##########################################################################################################

// 显示主菜单
void display_main_menu(int pos) {
    printf("=====MAIN MENU=====");
    printf("\n");
    set_text_color(YELLOW);
    for (int i = 0; i < 4; i++) {
        if (i != pos - 1) {
            printf("  %s\n", Menu_Text[i]);
        } else {
            set_text_color(WHITE);
            printf(">>");
            if (i == 3) set_text_color(RED | DARK_GRAY << 4 | HIGH);
            else set_text_color(GREEN | DARK_GRAY << 4 | HIGH);
            printf("%s\n", Menu_Text[i]);
            set_text_color(YELLOW);
        }
    }
    set_text_color(WHITE);
    printf("===================");
    printf("\n");

    set_text_color(WHITE);
    printf("Press Up and Down (or W and S) to move the cursor\n"
        "Press Enter to confirm\n");

    display_current_game_settings();
}

// 顯示设置菜單
void display_setting_menu(int pos) {
    printf("====SETTINGS====");
    printf("\n");

    set_text_color(YELLOW);
    for (int i = 0; i < 4; i++) {
        if (i != pos - 1) printf("  %s\n", Game_Setting[i]);
        else {
            set_text_color(WHITE);
            printf(">>");
            set_text_color(GREEN | DARK_GRAY << 4 | HIGH);
            printf("%s\n", Game_Setting[i]);
            set_text_color(YELLOW);
        }
    }

    set_text_color(WHITE);
    printf("================");
    printf("\n");
    printf("Press Up and Down (or W and S) to move the cursor\n"
        "Press Enter to confirm\n");

    display_current_game_settings();
}

// 显示困难选择菜单
void display_difficulty_menu(int pos) {
    printf("==DIFFICULTY==");
    printf("\n");

    for (int i = 0; i < 6; i++) {
        switch (i+1) {
            case 1:
                set_text_color(LIGHT_GREEN | HIGH);
                break;
            case 2:
                set_text_color(BLUE | HIGH);
                break;
            case 3:
                set_text_color(YELLOW | HIGH);
                break;
            case 4:
                set_text_color(RED | HIGH);
                break;
            case 5:
                set_text_color(PURPLE | HIGH);
                break;
            default:
                set_text_color(WHITE | HIGH);
                break;
        }

        if (i != pos - 1) printf("  %s\n", Game_Difficulty_Setting[i]);
        else {
            set_text_color(WHITE);
            printf(">>");
            switch (i+1) {
                case 1:
                    set_text_color(LIGHT_GREEN | DARK_GRAY << 4 | HIGH);
                    break;
                case 2:
                    set_text_color(BLUE | DARK_GRAY << 4 | HIGH);
                    break;
                case 3:
                    set_text_color(YELLOW | DARK_GRAY << 4 | HIGH);
                    break;
                case 4:
                    set_text_color(RED | DARK_GRAY << 4 | HIGH);
                    break;
                case 5:
                    set_text_color(PURPLE | DARK_GRAY << 4 | HIGH);
                    break;
                default:
                    set_text_color(GREEN | DARK_GRAY << 4);
                    break;
            }
            printf("%s\n", Game_Difficulty_Setting[i]);
            set_text_color(YELLOW);
        }
    }

    set_text_color(WHITE);
    printf("=============");
    printf("\n");
    printf("Press Up and Down (or W and S) to move the cursor\n"
        "Press Enter to confirm\n");

    display_current_game_settings();
}

// 显示地图尺寸菜单
void display_map_scale_menu(int pos) {
    printf("==MAP SCALE==");
    printf("\n");
    set_text_color(YELLOW);
    for (int i = 0; i < 4; i++) {
        if (i != pos - 1) printf("  %s\n", Map_Scale_Setting[i]);
        else {
            set_text_color(WHITE);
            printf(">>");
            set_text_color(GREEN | DARK_GRAY << 4 | HIGH);
            printf("%s\n", Map_Scale_Setting[i]);
            set_text_color(YELLOW);
        }
    }
    set_text_color(WHITE);
    printf("=============");
    printf("\n");
    printf("Press Up and Down (or W and S) to move the cursor\n"
        "Press Enter to confirm\n");

    display_current_game_settings();
}

// 显示蛇的演示模型
void display_snake_model(int color) {
    SetConsoleCursorPosition(hdl_menu, (COORD){13, 1});
    printf("|EXAMPLE|");
    for (int i = 0; i < 4; i++) {
        SetConsoleCursorPosition(hdl_menu, (COORD){17, 2+i});
        if (i == 0) {
            set_text_color(GREEN);
            putchar('O');
        } else {
            set_text_color(color);
            putchar('o');
        }
    }

    set_text_color(WHITE);
    SetConsoleCursorPosition(hdl_menu, (COORD){0, 0});
}

// 显示颜色选择菜单
void display_color_menu(int pos) {
    printf("===COLOR===");
    printf("\n");

    for (int i = 0; i < 6; i++) {
        switch (i + 1) {
            case 2:
                set_text_color(RED);
                break;
            case 3:
                set_text_color(YELLOW);
                break;
            case 4:
                set_text_color(BLUE);
                break;
            case 5:
                set_text_color(GREEN);
                break;
            default:
                set_text_color(WHITE);
                break;
        }

        if (i != pos - 1) printf("  %s\n", Color_Setting[i]);
        else {
            printf(">>");
            if (i == 5) set_text_color(GREEN | DARK_GRAY << 4 | HIGH);
            else set_text_color(DARK_GRAY << 4);
            printf("%s\n", Color_Setting[i]);
        }
    }

    set_text_color(WHITE);
    printf("===========");
    printf("\n");
    printf("Press Up and Down (or W and S) to move the cursor\n"
        "Press Enter to confirm\n");

    display_current_game_settings();
}

// 显示排行榜类别显示菜单
void display_rank_menu(int pos) {
    printf("====RANK====");
    printf("\n");
    set_text_color(YELLOW);
    for (int i = 0; i < 3; i++) {
        if (i != pos - 1) printf("  %s\n", Rank[i]);
        else {
            set_text_color(WHITE);
            printf(">>");
            set_text_color(GREEN | DARK_GRAY << 4 | HIGH);
            printf("%s\n", Rank[i]);
            set_text_color(YELLOW);
        }
    }
    set_text_color(WHITE);
    printf("============");
    printf("\n");
    printf("Press Up and Down (or W and S) to move the cursor\n"
        "Press Enter to confirm\n");
}

// 显示当前游戏设置
void display_current_game_settings() {
    SetConsoleCursorPosition(hdl_menu, (COORD){24, 1});
    printf("CURRENT GAME SETTINGS");

    SetConsoleCursorPosition(hdl_menu, (COORD){24, 2});
    printf("Difficulty: ");
    switch (difficulty) {
        case 1:
            set_text_color(LIGHT_GREEN | HIGH);
            break;
        case 2:
            set_text_color(BLUE | HIGH);
            break;
        case 3:
            set_text_color(YELLOW | HIGH);
            break;
        case 4:
            set_text_color(RED | HIGH);
            break;
        case 5:
            set_text_color(PURPLE | HIGH);
            break;
    }
    printf("%s", Game_Difficulty_Setting[difficulty-1]);


    set_text_color(WHITE);
    SetConsoleCursorPosition(hdl_menu, (COORD){24, 3});
    printf("Map Scale: ");
    set_text_color(YELLOW | HIGH);
    printf("%s", Map_Scale_Setting[map_scale-1]);

    set_text_color(WHITE);
    SetConsoleCursorPosition(hdl_menu, (COORD){24, 4});
    printf("Snake Color: ");
    set_text_color(color);
    switch (color) {
        case WHITE:
            printf("White");
            break;
        case YELLOW:
            printf("Yellow");
            break;
        case GREEN:
            printf("Green");
            break;
        case RED:
            printf("Red");
            break;
        case BLUE:
            printf("BLUE");
            break;
    }
    set_text_color(WHITE);

    SetConsoleCursorPosition(hdl_menu, (COORD){0, 0});
}

// 显示有关排行榜的相关信息
void display_rank_info_special() {
    SetConsoleCursorPosition(hdl_menu, (COORD){36, 1});
    printf("DISPLAY ORDER");

    SetConsoleCursorPosition(hdl_menu, (COORD){36, 2});
    printf("1. ");
    set_text_color(LIGHT_BLUE | HIGH);
    printf("Score");
    set_text_color(WHITE);
    SetConsoleCursorPosition(hdl_menu, (COORD){36, 3});
    printf("2. ");
    set_text_color(LIGHT_YELLOW | HIGH);
    printf("Difficulty");
    set_text_color("WHITE");

    SetConsoleCursorPosition(hdl_menu, (COORD){0, 1});
}

// 光标移动
int move_ptr(int pos, int max_index, char ch) {
    if (ch == 'w' || ch == 'W' || ch == 72) {
        if (pos > 1) return pos - 1;
        return 1;
    }

    if (ch == 's' || ch == 'S' || ch == 80) {
        if (pos < max_index) return pos + 1;
        return max_index;
    }
    return pos;
}

// 主菜单选项
void choose_main(int pos, int difficulty, int map_scale) {
    switch (pos) {
        case 1:
            game(difficulty, map_scale, color);

        // 重置难度
            difficulty = 3;
            map_scale = 1;
            break;
        case 2:
            // 显示窗口
            system("cls");
            SetConsoleTitle("Settings");
            int setting_pos = 1;
            display_setting_menu(setting_pos);
            while (1) {
                if (kbhit()) {
                    char ch = _getch();
                    if (ch == 'w' || ch == 's' || ch == 'W' || ch == 'S' || ch == 72 || ch == 80) {
                        setting_pos = move_ptr(setting_pos, 4, ch);
                        system("cls");
                        display_setting_menu(setting_pos);
                    }

                    if (ch == '\r') {
                        if (setting_pos == 4) {
                            system("cls");
                            break;
                        }

                        choose_setting(setting_pos);
                        display_setting_menu(setting_pos);
                    }
                }
            }
            break;

        case 3:
            system("cls");
            SetConsoleTitle("Rank");
            system("mode con cols=50 lines=10");
            read_data();
            int rank_pos = 1;
            display_rank_menu(rank_pos);
            while (1) {
                if (kbhit()) {
                    char ch = _getch();
                    if (ch == 'w' || ch == 's' || ch == 'W' || ch == 'S' || ch == 72 || ch == 80) {
                        rank_pos = move_ptr(rank_pos, 3, ch);
                        system("cls");
                        display_rank_menu(rank_pos);
                    }
                    if (ch == '\r') {
                        if (rank_pos <= 2) {
                            system("mode con cols=50 lines=33");
                            display_rank(rank_pos - 1);
                            set_text_color(GREEN | DARK_GRAY << 4 | HIGH);
                            printf(">>Back\n");
                            while (1) if (kbhit()) {
                                char ch = _getch();
                                if (ch == '\r') break;
                            }
                            set_text_color(WHITE);
                            system("cls");
                            system("mode con cols=50 lines=10");
                            display_rank_menu(rank_pos);
                        }

                        else break;
                    }
                }
            }
        break;

        case 4:
            exit(0);
    }
}

// 设置菜单选项
void choose_setting(int pos) {
    switch (pos) {
        case 1:
            // 选择难度
            system("cls");
            system("mode con cols=50 lines=12");
            SetConsoleTitle("Settings - Difficulty");
            int difficulty_pos = 1;
            display_difficulty_menu(difficulty_pos);
            while (1) {
                if (kbhit()) {
                    char ch = ' ';
                    ch = _getch();
                    if (ch == 'w' || ch == 's' || ch == 'W' || ch == 'S' || ch == 72 || ch == 80) {
                        difficulty_pos = move_ptr(difficulty_pos, 6, ch);
                        system("cls");
                        display_difficulty_menu(difficulty_pos);
                    }

                    if (ch == '\r') {
                        if (difficulty_pos < 6) difficulty = difficulty_pos;
                        system("cls");
                        system("mode con cols=50 lines=10");
                        break;
                    }
                }
            }
            break;

        case 2:
            // 选择地图大小
            system("cls");
            SetConsoleTitle("Settings - Map Scale");
            int map_scale_pos = 1;
            display_map_scale_menu(map_scale_pos);
            while (1) {
                if (kbhit()) {
                    char ch = _getch();
                    if (ch == 'w' || ch == 's' || ch == 'W' || ch == 'S' || ch == 72 || ch == 80) {
                        map_scale_pos = move_ptr(map_scale_pos, 4, ch);
                        system("cls");
                        display_map_scale_menu(map_scale_pos);
                    }

                    if (ch == '\r') {
                        if (map_scale_pos < 4) map_scale = map_scale_pos;
                        system("cls");
                        break;
                    }
                }
            }
            break;

        case 3:
            system("cls");
            system("mode con cols=60 lines=12");
            SetConsoleTitle("Settings - Snake Color");
            int color_pos = 1;
            display_color_menu(color_pos);
            display_snake_model(WHITE);
            while (1) {
                if (kbhit()) {
                    char ch = _getch();
                    if (ch == 'w' || ch == 's' || ch == 'W' || ch == 'S' || ch == 72 || ch == 80) {
                        color_pos = move_ptr(color_pos, 6, ch);
                        system("cls");
                        switch (color_pos) {
                            case 1:
                                display_snake_model(WHITE);
                                break;
                            case 2:
                                display_snake_model(RED);
                                break;
                            case 3:
                                display_snake_model(YELLOW);
                                break;
                            case 4:
                                display_snake_model(BLUE);
                                break;
                            case 5:
                                display_snake_model(GREEN);
                                break;
                        }

                        display_color_menu(color_pos);
                    }

                    if (ch == '\r') {
                        switch (color_pos) {
                            case 1:
                                color = WHITE;
                                break;
                            case 2:
                                color = RED;
                                break;
                            case 3:
                                color = YELLOW;
                                break;
                            case 4:
                                color = BLUE;
                                break;
                            case 5:
                                color = GREEN;
                                break;
                        }
                        system("cls");
                        system("mode con cols=50 lines=10");
                        break;
                    }
                }
            }

            break;
    }
}

// 读取之前的信息
void read_data() {
    FILE *file = fopen("rank.txt", "r");
    _head = NULL;
    data_node *last = NULL;
    char tmp_name[40];

    while (1) {
        if (fgets(tmp_name, 40, file) == NULL) break;

        data_node *new_node = (data_node *) malloc(sizeof(data_node));
        tmp_name[strlen(tmp_name) - 1] = '\0';
        strcpy(new_node->name, tmp_name);
        fscanf(file, "%d\n", &new_node->score);
        fscanf(file, "%d\n", &new_node->difficulty);
        fscanf(file, "%d\n", &new_node->map_scale);
        fscanf(file, "%d %d %d %d %d %d\n",
                &new_node->time_info.year, &new_node->time_info.month, &new_node->time_info.day,
                &new_node->time_info.hour, &new_node->time_info.minute, &new_node->time_info.second);

        new_node->next = NULL;

        if (_head == NULL) {
            _head = new_node;
            last = new_node;
        } else {
            last->next = new_node;
            last = new_node;
        }
    }

    fclose(file);
}

// 显示排行榜
void display_rank(int command) {
    int condition;
    if (command == 0) {
        printf("==========MONTH=RANK==========\n");
        condition = menu_get_time(0);
    }
    else if (command == 1) {
        printf("==========YEAR==RANK==========\n");
        condition = menu_get_time(1);
    }
    else return;

    int i = 0;
    data_node *current = _head;


    if (_head == NULL) {
        printf("No Data!\n");
        printf("==============================\n");
    }

    display_rank_info_special();

    while (i < 5 && current != NULL) {
        // 如果不是今年的, 則直接跳過
        if (command == 0 && current->time_info.year * 100 + current->time_info.month != condition ||
            command == 1 && current->time_info.year != condition) {
            current = current->next;
            continue;
        }

        set_text_color(WHITE);
        printf("Player Name: ");
        set_text_color(YELLOW | HIGH);
        printf("%s\n", current->name);

        set_text_color(WHITE);
        printf("Score: ");
        set_text_color(LIGHT_BLUE | HIGH);
        printf("%d\n", current->score);

        set_text_color(WHITE);
        printf("Difficulty: ");
        switch (current->difficulty) {
            case 1:
                set_text_color(LIGHT_GREEN | HIGH);
                break;
            case 2:
                set_text_color(BLUE | HIGH);
                break;
            case 3:
                set_text_color(YELLOW | HIGH);
                break;
            case 4:
                set_text_color(RED | HIGH);
                break;
            case 5:
                set_text_color(PURPLE | HIGH);
                break;
        }

        printf("%s\n", Game_Difficulty_Setting[current->difficulty - 1]);
        set_text_color(WHITE);
        printf("Map Scale: ");
        set_text_color(LIGHT_GREEN | HIGH);
        printf("%s\n", Map_Scale_Setting[current->map_scale - 1]);

        set_text_color(WHITE);
        printf("Play Time: ");
        set_text_color(LIGHT_BLUE | HIGH);
        printf("%.4d-%.2d-%.2d %.2d:%.2d:%.2d\n",
                current->time_info.year+1900, current->time_info.month+1,
                current->time_info.day, current->time_info.hour,
                current->time_info.minute, current->time_info.second);
        set_text_color(WHITE);

        printf("==============================\n");

        current = current->next;
        i++;
    }

}

// 设置文本颜色
void set_text_color(int color) {
    SetConsoleTextAttribute(hdl_menu, color);
}

// 获取时间当前年/月
int menu_get_time(int command) {
    time_t now = time(NULL);
    _time_ *tmp = localtime(&now);
    if (command == 0) return (tmp->tm_year) *100 + tmp->tm_mon;
    if (command == 1) return tmp->tm_year;

}

// 控制台
void Controller() {
    int position = 1;
    difficulty = 3;
    map_scale = 1;
    system("mode con cols=50 lines=10");
    SetConsoleTitle("Main Menu - Snake Game ver alpha 0.0.2");
    hdl_menu = GetStdHandle(STD_OUTPUT_HANDLE); // 获取终端句柄

    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(hdl_menu, &cursor_info);
    cursor_info.bVisible = FALSE;
    SetConsoleCursorInfo(hdl_menu, &cursor_info);
    display_main_menu(position);


    while (1) {
        // 主菜单
        Sleep(10);
        if (kbhit()) {
            char ch = _getch();
            fflush(stdin);
            if (ch == 'w' || ch == 's' || ch == 'W' || ch == 'S' || ch == 72 || ch == 80) {
                position = move_ptr(position, 4, ch);
                system("cls");
                display_main_menu(position);
            }

            // 回车
            else if (ch == '\r') {
                choose_main(position, difficulty, map_scale);
                position = 1;

                // 重置窗口
                system("cls");
                SetConsoleTitle("Main Menu");
                system("mode con cols=50 lines=10");
                display_main_menu(position);
            }
        }
    }
}
