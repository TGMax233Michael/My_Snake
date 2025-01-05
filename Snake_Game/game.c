#include "game.h"
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char Game_Difficulty[5][10] = {
    "Noob",
    "Easy",
    "Normal",
    "Hard",
    "Extreme",
};

char Map_Scale[3][10] = {
    "Small",
    "Normal",
    "Large"
};

// ========================================================================================================
HANDLE hdl;

// 游戏界面
double map_scale_list[] = {1, 1.2, 1.4};

// 蛇
snake_node *head; // 蛇的身体数组
snake_node *tail;
int size; // 蛇的大小

// 障碍物
obs_node *head_obs;

// 苹果
COORD apple;

// 玩家數據
char player_name[40];
data_node *data_head;
int score;
int len;
time_i start_time;
double map_size;
// ========================================================================================================

// 声明
void show_map();
void show_info(int score, int difficulty, int map_scale_);
void show_snake();
void init_snake();
void init_apple();
void init_obstacle();
void show_apple();
void refresh_snake(int direction);
void remove_object(COORD obj_pos);
void free_node();
void set_color(int color);
int get_state();
void save_score();
void read_score();
void append_score(int difficulty, int map_scale_);
void free_score();
void check_score();
void get_len();
void get_time();
void clear_keyboard_buffer();

// ========================================================================================================


// 顯示地圖
void show_map() {
    for (int i = 0; i <= (int) (map_size * HEIGHT) + 2; i++) {
        for (int j = 0; j <= (int) (map_size * WIDTH) + 2; j++) {
            if ((i == 0 || i == (int) (map_size * HEIGHT) + 2) || (j == 0 || j == (int) (map_size * WIDTH) + 2)) {
                set_color(LIGHT_YELLOW | HIGH);
                putchar('#');
                set_color(WHITE);
            } else putchar(' ');
        }
        putchar('\n');
    }
}

// 顯示分數
void show_info(int score, int difficulty, int map_scale_) {
    // 顯示玩家名稱
    COORD pos = {(int) (WIDTH * map_size) + 5, 0};
    SetConsoleCursorPosition(hdl, pos);
    printf("Player: ");
    set_color(YELLOW | HIGH);
    printf("%s", player_name);
    set_color(WHITE);


    // 顯示分數
    pos = (COORD){(int) (WIDTH * map_size) + 5, 1};
    SetConsoleCursorPosition(hdl, pos);
    printf("Score: ", score);
    set_color(LIGHT_BLUE | HIGH);
    printf("%d", score);
    set_color(WHITE);

    // 顯示游戲難度
    pos = (COORD){(int) (WIDTH * map_size) + 5, 2};
    SetConsoleCursorPosition(hdl, pos);
    printf("Difficulty: ");
    switch (difficulty) {
        case 1:
            set_color(LIGHT_GREEN | HIGH);
            break;
        case 2:
            set_color(BLUE | HIGH);
            break;
        case 3:
            set_color(YELLOW | HIGH);
            break;
        case 4:
            set_color(RED | HIGH);
            break;
        case 5:
            set_color(PURPLE | HIGH);
            break;
    }
    printf("%s", Game_Difficulty[difficulty-1]);
    set_color(WHITE);

    // 顯示地圖大小
    pos = (COORD){(int) (WIDTH * map_size) + 5, 3};
    SetConsoleCursorPosition(hdl, pos);
    printf("Map Scale: ");
    set_color(YELLOW);
    printf("%s", Map_Scale[map_scale_ - 1]);

    // 顯示當前時間
    set_color(LIGHT_BLUE | HIGH);
    pos = (COORD){(int) (WIDTH * map_size) + 5, 4};
    SetConsoleCursorPosition(hdl, pos);
    time_t cur_time = time(NULL);
    struct tm *cur_time_ = localtime(&cur_time);
    char buffer[25];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", cur_time_);
    printf("%s", buffer);
}

// 初始化苹果
void init_apple() {
    int flag = 1;

    while (flag) {
        flag = 0;
        apple.X = rand() % (int) (map_size * WIDTH) + 1;
        apple.Y = rand() % (int) (map_size * HEIGHT) + 1;

        snake_node *current = head;
        while (current) {
            if (current->body_node.X == apple.X && current->body_node.Y == apple.Y) {
                flag = 1;
                break;
            }
            current = current->next;
        }

        obs_node *current_ = head_obs;
        while (current_) {
            if (current_->obstacle_node.X == apple.X && current_->obstacle_node.Y == apple.Y) {
                flag = 1;
                break;
            }
            current_ = current_->next;
        }


    }
}

// 初始化蛇
void init_snake() {
    size = 4;
    snake_node *last = NULL;
    for (int i = 0; i < size; i++) {
        snake_node* new_node = (snake_node*)malloc(sizeof(snake_node));
        new_node->body_node = (COORD){(map_size * WIDTH / 2), (map_size * HEIGHT / 2) + i};
        new_node->next = NULL;

        if (head) {
            last->next = new_node;
            last = last->next;
        } else {
            head = new_node;
            last = head;
        }
    }

    tail = last;
}

// 初始化障碍物
void init_obstacle(int difficulty) {
    obs_node* last = NULL;

    int count = 0;

    switch (difficulty) {
        case 1:
            count = rand() % 4 + 4;
            break;
        case 2:
            count = rand() % 6 + 6;
            break;
        case 3:
            count = rand() % 8 + 8;
            break;
        case 4:
            count = rand() % 10 + 10;
            break;
        case 5:
            count = rand() % 12 + 12;
    }

    for (int i = 0; i < count; i++) {
        int flag = 1;
        int new_x, new_y;

        while (flag) {
            flag = 0;
            new_x = rand() % (int) (map_size * WIDTH) + 1;
            new_y = rand() % (int) (map_size * HEIGHT) + 1;

            snake_node *current = head;
            while (current) {
                if (current->body_node.X == new_x && current->body_node.Y == new_y) {
                    flag = 1;
                    break;
                }
                current = current->next;
            }
        }

        obs_node* new_node = (obs_node*)malloc(sizeof(obs_node));
        new_node->obstacle_node.X = new_x;
        new_node->obstacle_node.Y = new_y;
        new_node->next = NULL;

        if (head_obs == NULL) {
            head_obs = new_node;
            last = head_obs;
        }
        else {
            last->next = new_node;
            last = last->next;
        }

    }
}

// 顯示蛇
void show_snake(int color) {
    snake_node *current = head;
    int i = 0;

    while (current) {
        SetConsoleCursorPosition(hdl, current->body_node);
        if (i++ == 0) {
            set_color(LIGHT_GREEN | HIGH);
            putchar('O');
        } else {
            set_color(color | HIGH);
            putchar('o');
        }

        current = current->next;
    }

    set_color(WHITE);
}

// 顯示苹果
void show_apple() {
    SetConsoleCursorPosition(hdl, apple);
    set_color(RED | HIGH);
    putchar('*');
    set_color(WHITE);
}

// 显示障碍物
void show_obstacle() {
    obs_node* current = head_obs;
    while (current) {
        SetConsoleCursorPosition(hdl, current->obstacle_node);
        set_color(LIGHT_YELLOW | HIGH);
        putchar('#');
        set_color(WHITE);
        current = current->next;
    }
}

// 刷新蛇
void refresh_snake(int direction) {
    snake_node *current = head;
    snake_node *new_head = NULL;

    new_head = (snake_node *) malloc(sizeof(snake_node));
    new_head->body_node = head->body_node;
    new_head->next = head;


    switch (direction) {
        case 1:
            new_head->body_node.Y -= 1;
            break;
        case 2:
            new_head->body_node.Y += 1;
            break;
        case 3:
            new_head->body_node.X -= 1;
            break;
        case 4:
            new_head->body_node.X += 1;
            break;
    }

    head = new_head;

    while (current->next->next) current = current->next;
    free(tail);
    tail = current;
    tail->next = NULL;
}

// 移除对象
void remove_object(COORD obj_pos) {
    SetConsoleCursorPosition(hdl, obj_pos);
    putchar(' ');
}

// 獲取當前狀態
int get_state() {
    // 0 -> 游戏继续
    // 1 -> 游戏结束
    // 2 -> 吃到苹果
    snake_node *current = head->next;


    // 1.1 蛇是否碰到边界
    if (head->body_node.X == 0 || head->body_node.X == (int) (map_size * WIDTH) + 2 ||
        head->body_node.Y == 0 || head->body_node.Y == (int) (map_size * HEIGHT) + 2)
        return 1;

    // 1.2 蛇是否碰到自己
    for (int i = 1; i < size; i++) {
        if (head->body_node.X == current->body_node.X && head->body_node.Y == current->body_node.Y) {
            return 1;
        }
        current = current->next;
    }

    // 1.3 蛇是否碰到障碍物
    obs_node* current_ = head_obs;
    while (current_) {
        if (head->body_node.X == current_->obstacle_node.X && head->body_node.Y == current_->obstacle_node.Y) {
            return 1;
        }
        current_ = current_->next;
    }

    // 2 吃到苹果
    if (head->body_node.X == apple.X && head->body_node.Y == apple.Y) {
        return 2;
    }


    return 0;
}

// 蛇長度增加
void expand_snake() {
    snake_node *new_tail = (snake_node *) malloc(sizeof(snake_node));

    size += 1;

    new_tail->body_node = tail->body_node;
    new_tail->next = NULL;
    tail->next = new_tail;

    tail = new_tail;
}

// 釋放内存
void free_node() {
    snake_node *current = head;
    snake_node *last = NULL;

    while (current) {
        last = current;
        current = current->next;
        free(last);
    }

    obs_node *current_obs = head_obs;
    obs_node *last_obs = NULL;

    while (current_obs) {
        last_obs = current_obs;
        current_obs = current_obs->next;
        free(last_obs);
    }
}

// 保存數據
void save_score() {
    FILE *file = fopen("rank.txt", "w");

    data_node *current = data_head;
    while (current) {
        fprintf(file, "%s\n", current->name);
        fprintf(file, "%d\n", current->score);
        fprintf(file, "%d\n", current->difficulty);
        fprintf(file, "%d\n", current->map_scale);
        fprintf(file, "%d %d %d %d %d %d\n",
                current->time_info.year, current->time_info.month, current->time_info.day,
                current->time_info.hour, current->time_info.minute, current->time_info.second);

        current = current->next;
    }
    fclose(file);
}

// 讀取數據
void read_score() {
    FILE *file = fopen("rank.txt", "r");
    data_head = NULL;
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

        if (data_head == NULL) {
            data_head = new_node;
            last = new_node;
        } else {
            last->next = new_node;
            last = new_node;
        }
    }

    fclose(file);
}

// 添加數據
void append_score(int difficulty, int map_scale_) {
    data_node *new_node = (data_node *) malloc(sizeof(data_node));

    // 复制玩家名称
    if (strlen(player_name) == 0) strcpy(new_node->name, "player"); // 使用默认名称
    else strcpy(new_node->name, player_name);

    // 设置新节点的分数和难度
    new_node->score = score;
    new_node->difficulty = difficulty;
    new_node->map_scale = map_scale_;
    new_node->time_info = start_time;
    new_node->next = NULL;

    // 链表为空的情况
    if (data_head == NULL) {
        data_head = new_node;
        len++;
        return;
    }

    // 链表只有一个节点的情况
    if (data_head->next == NULL) {
        if (new_node->score > data_head->score) {
            new_node->next = data_head;
            data_head = new_node;
        } else {
            data_head->next = new_node;
        }
        len++;
        return;
    }

    // 链表有多于一个节点的情况
    data_node *current = data_head;
    data_node *last = NULL;

    while (current != NULL && current->score > new_node->score) {
        last = current;
        current = current->next;
    }

    if (last == NULL) {
        // 新节点分数最高，插入到头部
        new_node->next = data_head;
        data_head = new_node;
    } else {
        // 插入新节点到链表中
        new_node->next = last->next;
        last->next = new_node;
    }
    len++;

    // 如果链表长度超过20，删除尾部节点
    if (len > 20) {
        last = data_head;
        for (int i = 0; i < len - 2; i++) {
            last = last->next;
        }

        data_node *temp = last->next;
        last->next = NULL; // 防止内存泄漏
        free(temp);
        len--;
    }
}

// 釋放玩家數據
void free_score() {
    data_node *current = data_head;
    data_node *last = NULL;

    while (current) {
        last = current;
        current = current->next;
        free(last);
    }
}

// 獲取排行榜長度
void get_len() {
    data_node *current = data_head;
    len = 0;
    while (current) {
        current = current->next;
        len += 1;
    }
}

// 設置文本顔色
void set_color(int color) {
    SetConsoleTextAttribute(hdl, color);
}

// 獲取當前時間
void get_time() {
    time_t now = time(NULL);
    _time_ *tmp = localtime(&now);
    start_time.year = tmp->tm_year;
    start_time.month = tmp->tm_mon;
    start_time.day = tmp->tm_mday;
    start_time.hour = tmp->tm_hour;
    start_time.minute = tmp->tm_min;
    start_time.second = tmp->tm_sec;
}

// 清空緩衝區
void clear_keyboard_buffer() {
    while (_kbhit()) {
        _getch();
    }
}

// 游戲本體
void game(int difficulty, int map_scale_index, int color) {
    // 讀取玩家名字
    system("cls");
    system("mode con cols=30 lines=10");
    printf("Enter Your Name (len<=30)\n");
    printf(">> ");
    gets(player_name);

    // 設置游戲時窗口大小
    system("cls");
    if (map_scale_index == 1) system("mode con cols=70 lines=27");
    else if (map_scale_index == 2) system("mode con cols=80 lines=31");
    else system("mode con cols=90 lines=35");
    // 獲取玩家之前的數據
    data_head = NULL;
    read_score();
    get_len();
    get_time();

    srand(time(NULL));
    int direction = 1;
    int speed = (6 - difficulty) * 100 - 50;
    int game_state = 0;
    map_size = map_scale_list[map_scale_index - 1];
    score = 0;
    head = NULL;
    head_obs = NULL;

    COORD info_pos = {0,  (map_size * HEIGHT) + 4};

    // 初始化：光标
    hdl = GetStdHandle(STD_OUTPUT_HANDLE); // 获取终端句柄
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(hdl, &cursor_info);
    cursor_info.bVisible = FALSE;
    SetConsoleCursorInfo(hdl, &cursor_info);
    SetConsoleTitle("Game - Snake");


    // 绘制游戏界面
    init_snake();
    init_apple();
    init_obstacle(difficulty);
    show_map();
    show_snake(color);
    show_obstacle();

    SetConsoleCursorPosition(hdl, info_pos);
    set_color(YELLOW | HIGH);
    printf("Press UP DOWN LEFT RIGHT (or WASD) to move the snake\n");
    printf("The game will start in ");
    set_color(RED | HIGH);
    for (int i = 3; i >= 1; i--) {
        putchar('0' + i);
        Sleep(1000);
        SetConsoleCursorPosition(hdl, (COORD){23, (map_size * HEIGHT) + 5});
    }
    set_color(WHITE);
    system("cls");

    show_map();
    show_obstacle();
    show_apple();


    while (1) {
        show_snake(color);
        if (kbhit()) {
            if (GetAsyncKeyState(VK_UP) && direction != 2) direction = 1;
            else if (GetAsyncKeyState(VK_DOWN) && direction != 1) direction = 2;
            else if (GetAsyncKeyState(VK_LEFT) && direction != 4) direction = 3;
            else if (GetAsyncKeyState(VK_RIGHT) && direction != 3) direction = 4;

            else {
                char ch;
                ch = _getch();
                // 判斷方向(如果玩家鍵入方向與當前方向相反，則不做任何更改)
                if ((ch == 'w' || ch == 'W') && direction != 2) direction = 1;
                else if ((ch == 's' || ch == 'S') && direction != 1) direction = 2;
                else if ((ch == 'a' || ch == 'A') && direction != 4) direction = 3;
                else if ((ch == 'd' || ch == 'D') && direction != 3) direction = 4;
                clear_keyboard_buffer();
            }
        }

        remove_object(tail->body_node);
        refresh_snake(direction);

        show_snake(color);
        show_info(score, difficulty, map_scale_index);
        show_obstacle();

        // 判断游戏当前状态
        game_state = get_state();

        if (game_state == 1) {
            SetConsoleCursorPosition(hdl, head->body_node);
            set_color(RED);
            putchar('X');
            set_color(WHITE);
            SetConsoleCursorPosition(hdl, info_pos);
            printf("Game Over!!!\n"
                "Press any key to continue...");
            append_score(difficulty, map_scale_index);
            break;
        }

        if (game_state == 2) {
            score += 10;
            expand_snake();
            init_apple();
            show_apple();
        }

        Sleep(speed);
    }


    // 釋放内存以及存儲游戲數據
    free_node();
    save_score();
    free_score();

    // 防止玩家按的過快導致直接開了新的游戲
    Sleep(200);
    while (1) { if (kbhit()) break; }
    _getch();
}
