//
// Created by HONOR on 25-1-1.
//

#ifndef GAME_H
#define GAME_H

#include <windows.h>
#include <time.h>
#define RED 4
#define YELLOW 6
#define BLUE 9
#define GREEN 2
#define WHITE 7
#define DARK_GRAY 8
#define LIGHT_YELLOW 14
#define LIGHT_GREEN 10
#define LIGHT_BLUE 11
#define PURPLE 13
#define HIGH FOREGROUND_INTENSITY

#define _time_ struct tm


#define HEIGHT 20
#define WIDTH 40

typedef struct node{
    COORD body_node;
    struct node* next;
}snake_node;

typedef struct _time {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
}time_i;

typedef struct node1 {
    char name[40];
    int score;
    int difficulty;
    int map_scale;
    time_i time_info;
    struct node1* next;
}data_node;


void game(int difficulty, int map_scale_index, int color);

#endif //GAME_H
