// Pong.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"

using namespace std;

Color Green = Color{ 38, 185, 154, 255 };
Color Dark_Green = Color{ 20,160,133,255 };
Color Light_Green = Color{ 129,204,184,255 };
Color Yellow = Color{ 243,213,81,255 };

int player_score = 0;
int cpu_score = 0;

class Ball {
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw();
    void Update();
    void ResetBall();
};

void Ball::Draw() {
    DrawCircle(x, y, radius, Yellow);
}

void Ball::Update(){
    x += speed_x;
    y += speed_y;

    if (y + radius >= GetScreenHeight() || y - radius <= 0) {
        speed_y *= -1;
    }
    if (x + radius >= GetScreenWidth()) {
        cpu_score++;
        ResetBall();
    } 
    if(x - radius <= 0) {
        player_score++;
        ResetBall();
    }
}

void Ball::ResetBall() {
    x = GetScreenWidth() / 2;
    y = GetScreenHeight() / 2;

    int speed_choices[2] = { -1,1 };
    speed_x *= speed_choices[GetRandomValue(0, 1)];
    speed_y *= speed_choices[GetRandomValue(0, 1)];
}

class Paddle {
protected:
    void LimitMovement();

public:
    float x, y;
    float width, height;
    int speed;

    void Draw();
    void Update();
};

void Paddle::Draw() {
    //DrawRectangle(x, y, width, height, WHITE);
    DrawRectangleRounded(Rectangle{ x,y,width,height }, 0.8, 0, WHITE);
}

void Paddle::Update() {
    if (IsKeyDown(KEY_UP)) {
        y = y - speed;
    }
    if (IsKeyDown(KEY_DOWN)) {
        y = y + speed;
    }

    LimitMovement();
}

void Paddle::LimitMovement() {
    if (y <= 0) {
        y = 0;
    }
    if (y + height >= GetScreenHeight()) {
        y = GetScreenHeight() - height;
    }
}

class CPUPaddle :public Paddle {
public:
    void Update(int ball_y);
};

void CPUPaddle::Update(int ball_y) {
    if (y + height / 2 > ball_y) {
        y = y - speed;
    }
    if (y + height / 2 <= ball_y) {
        y = y + speed;
    }

    LimitMovement();
}

Ball ball;
Paddle player;
CPUPaddle cpu;

int main()
{
    cout << "Starting the game" << endl;
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Pong Game");
    SetTargetFPS(60);

    //Ball
    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    //Player
    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    //CPU
    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height;
    cpu.speed = 6;

    while (!WindowShouldClose()) {
        BeginDrawing();

        //Updating
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        //Check collision
        if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ player.x,player.y,player.width,player.height })) {
            ball.speed_x *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ ball.x,ball.y }, ball.radius, Rectangle{ cpu.x,cpu.y,cpu.width,cpu.height })) {
            ball.speed_x *= -1;
        }

        //Drawing
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
        DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
        ball.Draw();
        cpu.Draw();
        player.Draw();
        DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
