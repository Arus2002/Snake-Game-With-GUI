#include <iostream>
#include <deque>
#include <raylib.h>

struct Window {
    Color medium_turquoise = {93 , 55 , 66 , 0};
    Color darkColor = {43, 51, 24, 255};
    const int width = 750;
    const int height = 750;
    const int cellSize = 30;
    const int cellCount = 25;
    const int offset = 75;
};


int count;
int score = 0;
double UpdateTime = 0;
double TimeInFPS = 0.2;

bool SameCoordinatesForCross(std::pair<int , int> coordinates1, std::pair<int , int> coordinates2 ,std::deque<std::pair<int , int>> deque) {
    for(size_t i = 0 ; i < deque.size() ; ++i) {
        if(deque[i] == coordinates1 && coordinates1 == coordinates2) {
            return true;
        }
    }
    return false;
}

bool SameCoordinates(std::pair<int , int> coordinates, std::deque<std::pair<int , int>> deque) {
    for (size_t i = 0; i < deque.size(); i++) {
        if (deque[i] == coordinates) {
            return true;
        }
    }
    return false;
}

bool event(double time) {
    double CurrentTime = GetTime();
    if (CurrentTime - UpdateTime >= time) {
        UpdateTime = CurrentTime;
        return true;
    }
    return false;
}

class Snake {
public:
    std::deque<std::pair<int , int>> snakeBody = {{12 , 12} , {11 , 12} , {10 , 12} , {9 , 12}};
    std::pair<int , int> direction = {1 , 0}; //skzbum gnuma aj minchev urish uxxutyun chtan
    bool addSegment = false;
    bool subSegment = false;

    
    Texture2D texture1;
    Texture2D texture2;
    Snake() {
        Image image1 = LoadImage("SnakePhotos/head.png");
        texture1 = LoadTextureFromImage(image1);
        UnloadImage(image1);
        Image image2 = LoadImage("SnakePhotos/snake.png");
        texture2 = LoadTextureFromImage(image2);
        UnloadImage(image2);
    }
    ~Snake() {
        UnloadTexture(texture1);
        UnloadTexture(texture2);
    }

    void Draw() {
        Window window;
        double x1 = snakeBody[0].first;
        double y1 = snakeBody[0].second;
        DrawTexture(texture1 ,window.offset + x1 * window.cellSize , window.offset + y1 * window.cellSize , WHITE);
        for (size_t i = 1; i < snakeBody.size(); i++) {
            float x = snakeBody[i].first;
            float y = snakeBody[i].second;
            DrawTexture(texture2 ,window.offset + x * window.cellSize , window.offset + y * window.cellSize , WHITE);
        }
    }

    void UpdatedByApple() {
        int x = snakeBody[0].first + direction.first;
        int y = snakeBody[0].second + direction.second;
        std::pair<int , int> addCoordinate = {x , y};
        if (addSegment == true) { 
            snakeBody.push_front(addCoordinate);
            addSegment = false;
        }
        else {
            snakeBody.push_front(addCoordinate);
            snakeBody.pop_back();
        }
        
    }

    void UpdatedByCross() {
        if(subSegment == true && addSegment == false) {
            snakeBody.pop_back();
            subSegment = false;
        }
    }

    void StartGame() {
        snakeBody = {{12 , 12} , {11 , 12} , {10 , 12} , {9 , 12}};
        direction = {1 , 0};
    }
};


class Apple {

public:
    Window window;
    std::pair<int , int> positionForApple;
    Texture2D texture3;

    Apple(std::deque<std::pair<int , int>> snakeBody) {
        Image image3 = LoadImage("SnakePhotos/apple.png");
        texture3 = LoadTextureFromImage(image3);
        UnloadImage(image3);
        positionForApple = GenerateRandomPos(snakeBody);
    }

    ~Apple() {
        UnloadTexture(texture3);
    }

    void Draw() {
        DrawTexture(texture3, window.offset + positionForApple.first * window.cellSize, window.offset + positionForApple.second * window.cellSize, WHITE);
    }

    std::pair<int , int> GenerateRandomCell() {
        int x = GetRandomValue(2, window.cellCount - 3);
        int y = GetRandomValue(2, window.cellCount - 3);
        std::pair<int , int> p = {x , y};
        return p;
    }
    std::pair<int , int> GenerateRandomPos(std::deque<std::pair<int , int>> snakeBody) {
        std::pair<int , int> position = GenerateRandomCell();
        while (SameCoordinates(position, snakeBody)) {
            position = GenerateRandomCell();
        }
        return position;
    }
};

class Cross {
public:
    std::deque<std::pair<int , int>> snakeBody;
    Apple apple = Apple(snakeBody);
    Window window;
    std::pair<int , int> positionForCross;
    Texture2D texture6;

    Cross(std::deque<std::pair<int , int>> snakeBody) {
        Image image6 = LoadImage("SnakePhotos/cross.png");
        texture6 = LoadTextureFromImage(image6);
        UnloadImage(image6);
        positionForCross = GenerateRandomPos(snakeBody , apple.positionForApple); 
    }

    ~Cross() {
        UnloadTexture(texture6);
    }
    void Draw() {
        DrawTexture(texture6, window.offset + positionForCross.first * window.cellSize, window.offset + positionForCross.second * window.cellSize, WHITE);
    }
    
    std::pair<int , int> GenerateRandomCell() {
        int x = GetRandomValue(2, window.cellCount - 3);
        int y = GetRandomValue(2, window.cellCount - 3);
        std::pair<int , int> p = {x , y};
        return p;
    }
    std::pair<int , int> GenerateRandomPos(std::deque<std::pair<int , int>> snakeBody , std::pair<int , int> ApplePosition) {
        std::pair<int , int> position = GenerateRandomCell();
        while (SameCoordinatesForCross(position , ApplePosition, snakeBody)) {
            position = GenerateRandomCell();
        }
        return position;
    }
};

class Wall {
public:
Window window;
Texture2D texture4;
Texture2D texture5;

    Wall() {
        Image image2 = LoadImage("SnakePhotos/wall.png");
        texture4 = LoadTextureFromImage(image2);
        UnloadImage(image2);
        Image image3 = LoadImage("SnakePhotos/none.png");
        texture5 = LoadTextureFromImage(image3);
        UnloadImage(image3);
    }

    ~Wall() {
        UnloadTexture(texture4);
        UnloadTexture(texture5);
    }


    void Draw() {

        for (int i = 0 ; i <= window.cellCount - 1 ; ++i) {
            for (int j = 0 ; j <= window.cellCount - 1; ++j) {
                if (i == 0 || j == 0 || i == window.cellCount - 1 || j == window.cellCount - 1) {
                    DrawTexture(texture4 , window.offset + i * window.cellSize , window.offset + j * window.cellSize , WHITE);
                }
                else {
                    DrawTexture(texture5 , window.offset + i * window.cellSize , window.offset + j * window.cellSize , WHITE);
                }
            }
        }
    }

};

class MyGameSound {
public:
    Sound eatSound;
    Sound gameOverSound;
    MyGameSound() {
        InitAudioDevice();
        eatSound = LoadSound("Sounds/eat.mp3");
        gameOverSound = LoadSound("Sounds/gameOverSound.mp3");
    }

    ~MyGameSound() {
        UnloadSound(eatSound);
        UnloadSound(gameOverSound);
        CloseAudioDevice();
    }
};

class Logic {
public:
    Window window;
    Wall wall = Wall();
    Snake snake = Snake();
    Apple apple = Apple(snake.snakeBody);
    Cross cross = Cross(snake.snakeBody);
    MyGameSound sound = MyGameSound();
    bool IsRunning = true;

    void Draw() {
        apple.Draw();
        if(score >= 5 && score % 5 == 0) {
            cross.Draw();
        }
        snake.Draw();
        wall.Draw();
    }

    void EatTheApple() {
        if (snake.snakeBody[0].first == apple.positionForApple.first && snake.snakeBody[0].second == apple.positionForApple.second) { //hamematum e hamapatasxanabar x y coordinatnery
            TimeInFPS -= 0.001;
            apple.positionForApple = apple.GenerateRandomPos(snake.snakeBody);
            snake.addSegment = true;
            count++;
            score++;
            PlaySound(sound.eatSound);
        }
    }

    void EatTheCross() {
        if (snake.snakeBody[0].first == cross.positionForCross.first && snake.snakeBody[0].second == cross.positionForCross.second) {
           if (score % 5 == 0 && score >= 5) {
                TimeInFPS -= 0.001;
                cross.positionForCross = cross.GenerateRandomPos(snake.snakeBody , apple.positionForApple);
                snake.subSegment = true;
                count--;
                score--;
                PlaySound(sound.eatSound);
           }
       }
    }

    void TouchTheWall() {
        if (snake.snakeBody[0].first == 0) {
            snake.snakeBody[0].first = window.cellCount - 2;
        }
        else if (snake.snakeBody[0].first == window.cellCount - 1) {
            snake.snakeBody[0].first = 1;
        }
        if (snake.snakeBody[0].second == 0) {
            snake.snakeBody[0].second = window.cellCount - 2;
        }
        else if (snake.snakeBody[0].second == window.cellCount - 1) {
            snake.snakeBody[0].second = 1;
        }
    }

    void GameOver() {
        snake.StartGame();
        apple.positionForApple = apple.GenerateRandomPos(snake.snakeBody);
        cross.positionForCross = cross.GenerateRandomPos(snake.snakeBody , apple.positionForApple);
        IsRunning = false;
        score = 0;
        TimeInFPS = 0.2;
        PlaySound(sound.gameOverSound);
        
    }

    bool TouchTheTail() {
        std::deque<std::pair<int , int>> BodyWithoutHead = snake.snakeBody;
        BodyWithoutHead.pop_front();
        if (SameCoordinates(snake.snakeBody[0], BodyWithoutHead)) {
            return true;
        }
        return false;

    }
};




int main() {
    Window window;
    InitWindow(2 * window.offset + window.width, 2 * window.offset + window.height, "Snake Game!");
    Logic game;
    SetTargetFPS(60);
    std::string GameOver = "";
    bool HandlePause = false;
    while (!WindowShouldClose()) {
        if(game.TouchTheTail()) {
            GameOver = "Game Over!";
        }
        BeginDrawing();

        if (event(TimeInFPS)) {
            if (game.IsRunning) {
                game.snake.UpdatedByApple();
                game.snake.UpdatedByCross();
                game.EatTheApple();
                game.EatTheCross();
                game.TouchTheWall();
                game.TouchTheTail();
            }
        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.second != 1) {
            game.snake.direction = {0, -1};
            game.IsRunning = true;
            HandlePause = false;
        }
        else if (IsKeyPressed(KEY_DOWN) && game.snake.direction.second != -1) {
            game.snake.direction = {0, 1};
            game.IsRunning = true;
            HandlePause = false;
        }
        else if (IsKeyPressed(KEY_LEFT) && game.snake.direction.first != 1) {
            game.snake.direction = {-1, 0};
            game.IsRunning = true;
            HandlePause = false;
        }
        else if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.first != -1) {
            game.snake.direction = {1, 0};
            game.IsRunning = true;
            HandlePause = false;
        }
        if(IsKeyPressed(KEY_SPACE)) {
            game.IsRunning = true;
            HandlePause = false;
            GameOver = "";
            count = 0;
        }
        if(IsKeyPressed(KEY_P)) {
            game.IsRunning = false;
            HandlePause = true;
        }
        if(HandlePause) {
            DrawText("Game Is Paused!" , window.offset + (window.width / 2) - (2 * window.offset) + 1 , (window.width / 2) - (2 * window.offset) + 1 , 40 , window.darkColor);
            DrawText("Click 'SPACE' to continue." , window.offset + (window.width / 2) - (2 * window.offset) + 31 , (window.width / 2) - (2 * window.offset) + 35 , 20 , window.darkColor);
        }
        if(GameOver.size() != 0 || count < 0) {
            DrawText("Game Over!" , window.offset + (window.width / 2) - (2 * window.offset) + 31 , (window.width / 2) - (2 * window.offset) + 1 , 40 , window.darkColor);
            DrawText("Click 'SPACE' to try again." , window.offset + (window.width / 2) - (2 * window.offset) + 31 , (window.width / 2) - (2 * window.offset) + 35 , 20 , window.darkColor);
            DrawText(TextFormat("%i", count) , window.offset + (window.width / 2) - (2 * window.offset) + 31 ,  window.offset + (window.width / 2) - (2 * window.offset) + 41 , 40 , window.darkColor);
            game.GameOver();
        }
        ClearBackground(window.medium_turquoise);
        DrawText("Snake Game!" , window.offset + (window.width / 2) - (2 * window.offset) + 31 , 10 , 40 , window.darkColor);
        DrawText(TextFormat("%i", score), window.offset - 5, window.offset + window.cellSize * window.cellCount + 10, 40, window.darkColor);
        game.Draw();
        EndDrawing();
    }
    
    CloseWindow();
    
    
}