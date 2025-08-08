#include<iostream>
#include<raylib.h>

using namespace std;

const int screen_height = 800;
const int screen_width = 1280;

int player_points = 0;
int enemy_points = 0;

Color red = Color {136, 8, 8, 255};
Color burntSienna = Color {233, 116, 81, 255};
Color lightRed = Color {170, 74, 68, 255};

class Ball {
public:
    float position_x, position_y;
    float speed_x, speed_y;
    int radius;

    void Draw () {
        DrawCircle (position_x, position_y, radius, WHITE);
    }
    void UpdatePosition () {
         position_x += speed_x;
         position_y += speed_y;

    }
    void CheckPosition () {
        if (position_y + radius >= screen_height || position_y - radius <= 0) speed_y *= -1;
        UpdatePosition ();
        
        if (position_x + radius >= screen_width) {
            enemy_points ++;
            ResetBall ();
        }
        if (position_x - radius <= 0) {
            player_points ++;
            ResetBall ();
        }
    }
    void ResetBall () {
        position_x = screen_width / 2;
        position_y = screen_height / 2;

        int directions [2] = {1, -1};
        speed_x *= directions [GetRandomValue (0, 1)];
        speed_y *= directions [GetRandomValue (0, 1)];
    }
};

class Racket {
public:
    float position_x, position_y;
    float width, height;
    int speed;  

    void Draw () {
        DrawRectangle (position_x, position_y, width, height, WHITE);
    }
    void Update () {
        if (IsKeyDown (KEY_S)) position_y += speed;
        if (IsKeyDown (KEY_W)) position_y -= speed;

        if (position_y < 0) position_y = 0;
        if (position_y + height > GetScreenHeight()) position_y = GetScreenHeight() - height;
    }
};

class EnemyRacket: public Racket {
public:
    void AiBrain (int ball_position) {
        if (ball_position > position_y - height / 2 ) position_y += speed;
        if (ball_position < position_y - height / 2) position_y -= speed;

        if (position_y < 0) position_y = 0;
        if (position_y + height > GetScreenHeight()) position_y = GetScreenHeight() - height;
        
    }

};

Ball ball;
Racket player;
EnemyRacket enemy;

int main () 
{
    InitWindow (screen_width, screen_height, "!P-O-N-G!");
    SetTargetFPS (144);

    const int racket_height = 120;
    const int racket_width = 25;

    ball.radius = 20;
    ball.position_x = screen_width / 2;
    ball.position_y = screen_height / 2;
    ball.speed_x = 5;
    ball.speed_y = 5;

    player.width = 25;
    player.height = 120;
    player.position_x = screen_width - player.width;
    player.position_y = screen_height / 2 - player.height / 2;
    player.speed = 8;

    enemy.width = 25;
    enemy.height = 120;
    enemy.position_x = 0;
    enemy.position_y = screen_height / 2 - enemy.height / 2;
    enemy.speed = 8;

    while (WindowShouldClose() == false) {
        BeginDrawing ();
        

        ClearBackground (burntSienna);
        DrawLine (screen_width / 2, 0, screen_width / 2, screen_height, red);
        DrawCircle (screen_width / 2, screen_height / 2, 200, lightRed);
       
        ball.CheckPosition ();
        ball.Draw ();
        
        player.Update ();
        player.Draw ();

        enemy.AiBrain (ball.position_y);
        enemy.Draw ();

        if (CheckCollisionCircleRec (Vector2 {ball.position_x, ball.position_y}, ball.radius,
            Rectangle {player.position_x, player.position_y, player.width, player.height})) 
                ball.speed_x *= -1;
            
        if (CheckCollisionCircleRec (Vector2 {ball.position_x, ball.position_y}, ball.radius,
            Rectangle {enemy.position_x, enemy.position_y, enemy.width, enemy.height})) 
                ball.speed_x *= -1;
        
        DrawText (TextFormat ("%i", enemy_points), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText (TextFormat ("%i", player_points), 3 * screen_width / 4 - 20, 20, 80, WHITE);

        
        EndDrawing ();
    }

    CloseWindow ();
    return 0;
}