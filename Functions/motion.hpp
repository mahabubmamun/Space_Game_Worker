#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <bits/stdc++.h>

#define Height 600
#define Width 1000
#define Gravity_Value 10
#define delay 10 // Time in milliseconds

#define up_value -15
#define down_value 15
#define right_value 15
#define left_value -15

const int shot_limit = 5;
const int asteroid_limit = 50;

using namespace std;
// srand(time(0));

class spaceship_class
{
public:
    bool on;
    int x;
    int y;
    int dimension = 50;
    int lives = 3;
    int points = 0;
    int point_val = 10;

    int spawn_X = 475;
    int spawn_Y = 600;

    void border_limits()
    {
        if (x <= 50)
            x = 50;

        else if (x >= Height - 100)
        {
            x = Height - 100;
        }

        if (y <= 50)
            y = 50;

        else if (y >= Height - 50)
        {
            y = Height - 50;
        }
    }

    void motion_right()
    {
        x += right_value;
    }

    void motion_left()
    {
        x += left_value;
    }

    void motion_up()
    {
        y += up_value;
    }

    void motion_down()
    {
        y += down_value;
    }

    void declare()
    {
        on = true;
        x = spawn_X;
        y = spawn_Y;
        points = 0;
        lives = 3;
    }

    void respawn()
    {
        on = true;
        x = spawn_X;
        y = spawn_Y;
    }

    void IncreasePoint()
    {
        points += point_val;
    }

    bool CheckLives()
    {
        if (lives >= 1)
            return 0;

        return 1;
    }
};

class bullet
{

public:
    bool on;
    int x;
    int y;
    int speed = 20;
    int height = 20;
    int width = 30;

    void update()
    {
        x += speed;
    }

    void dec()
    {
        on = false;
    }

    void increae_speed()
    {
        speed += 10;
    }

    bool turn_on()
    {
        if (on == false)
        {
            on = true;
            return true;
        }

        return false;
    }
};

class asteroid
{
public:
    bool on;
    bool explode;
    int x;
    int y;
    int length = 100;
    int width = 100;
    int speed = 5;
    int probability = 2500; // the more the less likely

    void declare()
    {
        on = false;
        explode = false;
    }

    bool probability_asteroid()
    {
        if ((rand() % probability) == 0)
            return true;

        return false;
    }

    void create_asteroid()
    {
        x = 1000;
        y = (rand() % 600);
    }

    void update()
    {
        x -= speed;
    }

    void turn_of()
    {
        on = false;
        explode = true;
    }

    bool turn_on()
    {
        if (on == false)
        {
            on = true;
            return true;
        }

        return false;
    }

    bool collide(class bullet &a)
    {
        int x1 = x + width;
        int x2 = x;
        int y1 = y + length;
        int y2 = y;

        // int offsetL = length / 2;
        // int offsetW = width / 2;

        if ((a.x <= x1 && a.x >= x2) && (a.y <= y1 && a.y >= y2) && on == true)
        {
            a.on = false;
            on = false;
            explode = true;

            return 1;
        }

        return 0;
    }

    void collide_spaceship(class spaceship_class &a)
    {
        int x1 = x + width / 2;
        int x2 = x - width / 2;
        int y1 = y + length / 2;
        int y2 = y - length / 2;

        if ((a.x <= x1 && a.x >= x2) && (a.y <= y1 && a.y >= y2) && on == true)
        {
            a.lives = a.lives - 1;
            a.on = false;
        }
    }
};
