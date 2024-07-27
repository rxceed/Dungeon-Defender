#include <iostream>
#include <math.h>
#include <vector>
#include <string.h>
#include <time.h>
#include "raylib.h"

using namespace std;

//Enumerator
//Entity type
//Enemy
enum EnemyType
{
    ENEMY_MELEE = 0, ENEMY_RANGED, ENEMY_REVIVE, ENEMY_BOSS
};
//Ally
enum AllyType
{
    ALLY_MELEE = 0, ALLY_RANGED, ALLY_SUPPORT, ALLY_BLOCKER
};

//Stats type
enum Attribute
{
    ATTRIBUTE_HEALTH = 0, ATTRIBUTE_DEF, ATTRIBUTE_ATK, ATTRIBUTE_SPD, 
};

//Facing orientation
enum Orientation
{
    TOP = 0, RIGHT, BOTTOM, LEFT
};

//Texture index
enum TextureIndex
{
    TEX_PLAYER = 0, TEX_BULLET, TEX_KADAL, TEX_GOBLIN, TEX_SWORDS, TEX_RANGED, TEX_WALL, TEX_STATUE, TEX_TILE, TEX_GUI_BUTTON, TEX_GUI_TEXT,
    TEX_MAGIC_ARROW, TEX_MAIN_MENU_BG, TEX_STAGE_SELECT_BG, TEX_HELP_MENU_BG
};


//Game class
class Core
{
    private:
    const int WindowWidth = 1920;
    const int WindowHeight = 1080;
    const int MAX_FPS = 60;

    bool GameOver = false;
    bool ResetStage = false;
    bool pause = false;
    bool StageCleared = false;

    bool InMainMenu = true;
    bool InDebug = false;
    bool InStageSelect = false;
    bool InHelp = false;
    bool InStageOne = false;
    bool InStageTwo = false;
    bool InStageThree = false;
    bool InStageFour = false;
    bool InStageFive = false;

    bool exit = false;

    double TimeElapsed;
    double ReferenceTime = GetTime();
    double CurrentTime;

    
    //Textures
    Texture2D tex_player, tex_bullet, tex_goblin, tex_swords, tex_ranged, tex_wall, tex_statue, tex_tile, tex_gui_button, tex_gui_text,
    tex_main_menu_bg, tex_stage_select_bg, tex_help_menu_bg;
    Texture2D tex_magic_arrow[30];
    Texture2D tex_kadal[13];
    public:
    //Camera
    Camera2D camera = {{0, 0}, {0, 0}, 0, 1};

    void GameInit()
    {
        InitWindow(WindowWidth, WindowHeight, "Game");
        ToggleFullscreen();
        SetTargetFPS(60);
        TextureLoad();
    }

    void TextureLoad()
    {
        tex_bullet = LoadTexture("assets/dummy_bulletpng.png");
        tex_player = LoadTexture("assets/adventurer-v1.5-Sheet.png");
        tex_goblin = LoadTexture("assets/goblin.png");
        tex_swords = LoadTexture("assets/sword.png");
        tex_ranged = LoadTexture("assets/ranged.png");
        tex_wall  = LoadTexture("assets/TX Tileset Wall_1.png");
        tex_statue = LoadTexture("assets/objective_statue.png");
        tex_tile = LoadTexture("assets/TX Tileset Grass.png");
        tex_gui_button = LoadTexture("assets/GUI Button.png");
        tex_gui_text = LoadTexture("assets/GUI_Text.png");
        tex_main_menu_bg = LoadTexture("assets/main_menu_bg.png");
        tex_stage_select_bg = LoadTexture("assets/Background.png");
        tex_help_menu_bg = LoadTexture("assets/help_menu.png");

        for(int i = 0; i < 30; i++)
        {
            char path[100] = "assets/magic_arrow/1_";
            if(i < 10)
            {
                char index[2] = {char(48+i)};
                strcat(path, index);
            }
            else if(i < 20)
            {
                char index[3] = {char(49), char(48+i-10)};
                strcat(path, index);
            }
            else if(i < 30)
            {
                char index[3] = {char(50), char(48+i-20)};
                strcat(path, index);
            }
            strcat(path, ".png");
            tex_magic_arrow[i] = LoadTexture(path);
        }
        for(int i = 0; i < 13; i++)
        {
            char path[100] = "assets/kadal/";
            if(i < 3)
            {
                strcat(path, "Idle");
                char index[2] = {char(49+i)};
                strcat(path, index);
            }
            else if(i < 7)
            {
                strcat(path, "Walk");
                char index[3] = {char(49+i-3)};
                strcat(path, index);
            }
            else if(i < 10)
            {
                strcat(path, "Attack");
                char index[3] = {char(49+i-7)};
                strcat(path, index);
                
            }
            else if(i < 13)
            {
                strcat(path, "Fire_Attack");
                char index[3] = {char(49+i-10)};
                strcat(path, index);
            }
            strcat(path, ".png");
            tex_kadal[i] = LoadTexture(path);
        }
    }
    void TextureUnload()
    {
        UnloadTexture(tex_bullet);
        UnloadTexture(tex_player);
        UnloadTexture(tex_goblin);
        UnloadTexture(tex_swords);
        UnloadTexture(tex_wall);
        UnloadTexture(tex_statue);
        UnloadTexture(tex_tile);
        UnloadTexture(tex_gui_button);
        UnloadTexture(tex_gui_text);
        UnloadTexture(tex_main_menu_bg);
        UnloadTexture(tex_stage_select_bg);
        UnloadTexture(tex_help_menu_bg);
        for(int i = 0; i < 30; i++)
        {
            UnloadTexture(tex_magic_arrow[i]);
        }
        for(int i = 0; i < 13; i++)
        {
            UnloadTexture(tex_kadal[i]);
        }
    }

    Texture2D AssignTexture(int index)
    {
        switch(index)
        {
        case 0:
            return tex_player;
            break;
        case 1:
            return tex_bullet;
            break;
        case 3:
            return tex_goblin;
            break;
        case 4:
            return tex_swords;
            break;
        case 5:
            return tex_ranged;
            break;
        case 6:
            return tex_wall;
            break;
        case 7:
            return tex_statue;
            break;
        case 8:
            return tex_tile;
            break;
        case 9:
            return tex_gui_button;
            break;
        case 10:
            return tex_gui_text;
            break;
        case 12:
            return tex_main_menu_bg;
            break;
        case 13:
            return tex_stage_select_bg;
            break;
        case 14:
            return tex_help_menu_bg;
            break;
        default:
            break;
        }
    }
    Texture2D AssignTexture(int index, int n)
    {
        switch(index)
        {
        case 2:
            return tex_kadal[n];
            break;
        case 11:
            return tex_magic_arrow[n];
            break;
        default:
            break;
        }
    }

    float ScaleFPS()
    {
        return float(GetFPS())/float(MAX_FPS);
    }

    void StartTimer()
    {
        ReferenceTime = GetTime();
    }
    double GetTimer()
    {
        CurrentTime = GetTime();
        TimeElapsed = CurrentTime - ReferenceTime;
        return TimeElapsed;
    }

    bool IsGameOver()
    {
        return GameOver;
    }
    void ModGameOver(bool status)
    {
        GameOver = status;
    }

    bool IsResetStage()
    {
        return ResetStage;
    }
    void ModResetStage(bool status)
    {
        ResetStage = status;
    }

    bool IsPaused()
    {
        return pause;
    }
    void ModPause(bool status)
    {
        pause = status;
    }

    bool IsStageCleared()
    {
        return StageCleared;
    }
    void ModStageCleared(bool status)
    {
        StageCleared = status;
    }

    bool IsInMainMenu()
    {
        return InMainMenu;
    }
    void ModInMainMenu(bool status)
    {
        InMainMenu = status;
    }

    bool IsInDebug()
    {
        return InDebug;
    }
    void ModInDebug(bool status)
    {
        InDebug = status;
    }

    bool IsInHelp()
    {
        return InHelp;
    }
    void ModInHelp(bool status)
    {
        InHelp = status;
    }

    bool IsInStageSelect()
    {
        return InStageSelect;
    }
    void ModInStageSelect(bool status)
    {
        InStageSelect = status;
    }

    bool IsInStageOne()
    {
        return InStageOne;
    }
    void ModInStageOne(bool status)
    {
        InStageOne = status;
    }

    bool IsInStageTwo()
    {
        return InStageTwo;
    }
    void ModInStageTwo(bool status)
    {
        InStageTwo = status;
    }

    bool IsInStageThree()
    {
        return InStageThree;
    }
    void ModInStageThree(bool status)
    {
        InStageThree = status;
    }

    bool IsInStageFour()
    {
        return InStageFour;
    }
    void ModInStageFour(bool status)
    {
        InStageFour = status;
    }

    bool IsInStageFive()
    {
        return InStageFive;
    }
    void ModInStageFive(bool status)
    {
        InStageFive = status;
    }

    bool IsExit()
    {
        return exit;
    }
    void ModExit(bool status)
    {
        exit = status;
    }

    int GetWindowWidth()
    {
        return WindowWidth;
    }
    int GetWindowHeight()
    {
        return WindowHeight;
    }    
};

//Game class declaration
Core game;

//Menu, UI, HUD
class Button
{
    protected:
    Rectangle ButtonBox;
    Vector2 CenterPoint;
    char *text;
    Texture2D texture;
    Rectangle TextureFrame;
    Rectangle DestFrame;
    int FontSize;
    Color TextColor;
    
    public:
    Button(Vector2 position, Color TextColor)
    {
        this->CenterPoint = position;
        this->ButtonBox = {CenterPoint.x, CenterPoint.y, 0, 0};
        this->TextColor = TextColor;
    }
    virtual void Update() = 0;
    virtual void Draw()
    {
        //DrawText(text, CenterPoint.x-MeasureText(text, FontSize)/2.0f, CenterPoint.y, FontSize, BLACK);
        DrawTexturePro(texture, TextureFrame, DestFrame, {DestFrame.width/2.0f, DestFrame.height/2.0f}, 0, WHITE);
        //DrawRectangleRec(ButtonBox, BLACK);
    }
};

class ButtonDebug:public Button
{
    public:
    ButtonDebug(Vector2 position, Color TextColor):Button(position, TextColor)
    {
        this->text = new char[6]{"Debug"};
        this->FontSize = 40;

        this->texture = game.AssignTexture(TEX_GUI_BUTTON);
        this->TextureFrame = {7, 78, 69, 21};
        this->DestFrame = {CenterPoint.x, CenterPoint.y, TextureFrame.width*3, TextureFrame.height*3};
        
        this->ButtonBox = {CenterPoint.x-DestFrame.width/2.0f, CenterPoint.y-DestFrame.height/2.0f, DestFrame.width, DestFrame.height};
    }
    void Update() override
    {
        if(CheckCollisionPointRec(GetMousePosition(), ButtonBox))
        {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                game.ModInMainMenu(false);
                game.ModInDebug(true);
            }
        }
    }

};

class ButtonPlay:public Button
{
    public:
    ButtonPlay(Vector2 position, Color TextColor):Button(position, TextColor)
    {
        this->text = new char[5]{"Play"};
        this->FontSize = 40;

        this->texture = game.AssignTexture(TEX_GUI_BUTTON);
        this->TextureFrame = {7, 14, 69, 21};
        this->DestFrame = {CenterPoint.x, CenterPoint.y, TextureFrame.width*3, TextureFrame.height*3};
        
        this->ButtonBox = {CenterPoint.x-DestFrame.width/2.0f, CenterPoint.y-DestFrame.height/2.0f, DestFrame.width, DestFrame.height};
    }
    void Update() override
    {
        if(CheckCollisionPointRec(GetMousePosition(), ButtonBox))
        {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                game.ModInMainMenu(false);
                game.ModInStageSelect(true);
            }
        }
    }
};

class ButtonStageSelect:public Button
{
    private:
    int stage;

    public:
    ButtonStageSelect(Vector2 position, Color TextColor, int stage):Button(position, TextColor)
    {
        FontSize = 40;
        this->stage = stage;
        this->texture = game.AssignTexture(TEX_GUI_BUTTON);
        switch(stage)
        {
        case 1:
            this->text = new char[8]{"Level 1"};
            this->TextureFrame = {7, 202, 69, 21};
            this->DestFrame = {CenterPoint.x, CenterPoint.y, TextureFrame.width*3, TextureFrame.height*3};
            break;
        case 2:
            this->text = new char[8]{"Level 2"};
            this->TextureFrame = {7, 231, 69, 21};
            this->DestFrame = {CenterPoint.x, CenterPoint.y, TextureFrame.width*3, TextureFrame.height*3};
            break;
        case 3:
            this->text = new char[8]{"Level 3"};
            this->TextureFrame = {7, 262, 69, 21};
            this->DestFrame = {CenterPoint.x, CenterPoint.y, TextureFrame.width*3, TextureFrame.height*3};
            break;
        case 4:
            this->text = new char[8]{"Level 4"};
            this->TextureFrame = {106, 202, 69, 21};
            this->DestFrame = {CenterPoint.x, CenterPoint.y, TextureFrame.width*3, TextureFrame.height*3};
            break;
        case 5:
            this->text = new char[8]{"Level 5"};
            this->TextureFrame = {106, 231, 69, 21};
            this->DestFrame = {CenterPoint.x, CenterPoint.y, TextureFrame.width*3, TextureFrame.height*3};
            break;
        default:
            break;
        }
        this->ButtonBox = {CenterPoint.x-DestFrame.width/2.0f, CenterPoint.y-DestFrame.height/2.0f, DestFrame.width, DestFrame.height};
    }
    void Update() override
    {
        if(CheckCollisionPointRec(GetMousePosition(), ButtonBox))
        {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                game.ModInStageSelect(false);
                switch(stage)
                {
                case 1:
                    game.ModInStageOne(true);
                    break;
                case 2:
                    game.ModInStageTwo(true);
                    break;
                default:
                    break;
                }
            }
        }
    }
};

class ButtonMainMenu:public Button
{
    public:
    ButtonMainMenu(Vector2 position, Color TextColor):Button(position, TextColor)
    {
        this->text = new char[10]{"Main Menu"};
        this->FontSize = 40;
        this->texture = game.AssignTexture(TEX_GUI_BUTTON);
        this->TextureFrame = {106, 14, 69, 21};
        this->DestFrame = {CenterPoint.x, CenterPoint.y, TextureFrame.width*3, TextureFrame.height*3};
        
        this->ButtonBox = {CenterPoint.x-DestFrame.width/2.0f, CenterPoint.y-DestFrame.height/2.0f, DestFrame.width, DestFrame.height};
    }
    void Update() override
    {
        if(CheckCollisionPointRec(GetMousePosition(), ButtonBox))
        {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                game.ModInMainMenu(true);
                game.ModInStageSelect(false);
                game.ModInStageOne(false);

                game.ModGameOver(false);
            }
        }
    }
};

class ButtonExit:public Button
{
    public:
    ButtonExit(Vector2 position, Color TextColor):Button(position, TextColor)
    {
        this->text = new char[5]{"Exit"};
        this->FontSize = 40;
        this->texture = game.AssignTexture(TEX_GUI_BUTTON);
        this->TextureFrame = {7, 45, 69, 21};
        this->DestFrame = {CenterPoint.x, CenterPoint.y, TextureFrame.width*3, TextureFrame.height*3};
        
        this->ButtonBox = {CenterPoint.x-DestFrame.width/2.0f, CenterPoint.y-DestFrame.height/2.0f, DestFrame.width, DestFrame.height};
    }
    void Update() override
    {
        if(CheckCollisionPointRec(GetMousePosition(), ButtonBox))
        {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                game.ModExit(true);
            }
        }
    }
};

class ButtonRetry:public Button
{
    public:
    ButtonRetry(Vector2 position, Color TextColor):Button(position, TextColor)
    {
        this->text = new char[6]{"Retry"};
        this->FontSize = 40;
        this->texture = game.AssignTexture(TEX_GUI_BUTTON);
        this->TextureFrame = {106, 78, 69, 21};
        this->DestFrame = {CenterPoint.x, CenterPoint.y, TextureFrame.width*3, TextureFrame.height*3};
        
        this->ButtonBox = {CenterPoint.x-DestFrame.width/2.0f, CenterPoint.y-DestFrame.height/2.0f, DestFrame.width, DestFrame.height};
    }
    void Update() override
    {
        if(CheckCollisionPointRec(GetMousePosition(), ButtonBox))
        { 
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                game.ModResetStage(true);
                game.ModGameOver(false);
            }
        }
    }
};

class ButtonResume:public Button
{
    public:
    ButtonResume(Vector2 position, Color TextColor):Button(position, TextColor)
    {
        this->text = new char[7]{"Resume"};
        this->texture = game.AssignTexture(TEX_GUI_BUTTON);
        this->TextureFrame = {106, 113, 69, 21};
        this->DestFrame = {CenterPoint.x, CenterPoint.y, TextureFrame.width*3, TextureFrame.height*3};
        
        this->ButtonBox = {CenterPoint.x-DestFrame.width/2.0f, CenterPoint.y-DestFrame.height/2.0f, DestFrame.width, DestFrame.height};
    }
    void Update() override
    {
        if(CheckCollisionPointRec(GetMousePosition(), ButtonBox))
        {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                game.ModPause(false);
            }
        }
    }
};

class ButtonNextLevel:public Button
{
    public:
    ButtonNextLevel(Vector2 position, Color TextColor):Button(position, TextColor)
    {
        this->text = new char[11]{"Next Level"};
        this->texture = game.AssignTexture(TEX_GUI_BUTTON);
        this->TextureFrame = {106, 45, 69, 21};
        this->DestFrame = {CenterPoint.x, CenterPoint.y, TextureFrame.width*3, TextureFrame.height*3};
        
        this->ButtonBox = {CenterPoint.x-DestFrame.width/2.0f, CenterPoint.y-DestFrame.height/2.0f, DestFrame.width, DestFrame.height};
    }
    void Update() override
    {
        if(CheckCollisionPointRec(GetMousePosition(), ButtonBox))
        {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if(game.IsInStageOne())
                {
                    game.ModInStageOne(false);
                    game.ModInStageTwo(true);
                }
                else if(game.IsInStageTwo())
                {
                    game.ModInStageTwo(false);
                    game.ModInStageThree(true);
                }
                else if(game.IsInStageThree())
                {
                    game.ModInStageThree(false);
                    game.ModInStageFour(true);
                }
                else if(game.IsInStageFive())
                {
                    game.ModInStageFour(false);
                    game.ModInStageFive(true);
                }
            }
        }
    }
};

class ButtonBack:public Button
{
    public:
    ButtonBack(Vector2 position, Color TextColor):Button(position, TextColor)
    {
        this->texture = game.AssignTexture(TEX_GUI_BUTTON);
        this->TextureFrame = {106, 262, 69, 21};
        this->DestFrame = {CenterPoint.x, CenterPoint.y, TextureFrame.width*3, TextureFrame.height*3};
        this->ButtonBox = {CenterPoint.x-DestFrame.width/2.0f, CenterPoint.y-DestFrame.height/2.0f, DestFrame.width, DestFrame.height};
    }
    void Update() override
    {
        if(CheckCollisionPointRec(GetMousePosition(), ButtonBox))
        {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if(game.IsInStageSelect())
                {
                    game.ModInStageSelect(false);
                    game.ModInMainMenu(true);
                }
                else if(game.IsInHelp())
                {
                    game.ModInHelp(false);
                    game.ModInMainMenu(true);
                }
            }
        }
    }
};

class ButtonHelp:public Button
{
    public:
    ButtonHelp(Vector2 position, Color TextColor):Button(position, TextColor)
    {
        this->texture = game.AssignTexture(TEX_GUI_BUTTON);
        this->TextureFrame = {7, 113, 69, 21};
        this->DestFrame = {CenterPoint.x, CenterPoint.y, TextureFrame.width*3, TextureFrame.height*3};
        this->ButtonBox = {CenterPoint.x-DestFrame.width/2.0f, CenterPoint.y-DestFrame.height/2.0f, DestFrame.width, DestFrame.height};
    }
    void Update() override
    {
        if(CheckCollisionPointRec(GetMousePosition(), ButtonBox))
        {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                game.ModInMainMenu(false);
                game.ModInHelp(true);
            }
        }
    }
};

class GameOverPanel
{
    private:
    ButtonExit *ExitButton;
    ButtonRetry *RetryButton;
    ButtonMainMenu *MainMenuButton;

    Vector2 CenterPoint = {float(game.GetWindowWidth()/2.0f), float(game.GetWindowHeight()/2.0f)};

    Texture2D texture = game.AssignTexture(TEX_GUI_BUTTON);
    Rectangle TextureFrame;
    Rectangle DestFrame;

    Texture2D texture_text = game.AssignTexture(TEX_GUI_TEXT);
    Rectangle TextTextureFrame;
    Rectangle TextDestFrame;

    public:
    GameOverPanel()
    {
        TextureFrame = {7, 143, 80, 50};
        DestFrame = {CenterPoint.x, CenterPoint.y, TextureFrame.width*10, TextureFrame.height*10};
        TextTextureFrame = {71, 83, 711, 130};
        TextDestFrame = {CenterPoint.x, CenterPoint.y-100, TextTextureFrame.width*0.8f, TextTextureFrame.height*0.8f};
        ExitButton = new ButtonExit({CenterPoint.x+250, CenterPoint.y+100}, BLACK);
        RetryButton = new ButtonRetry({CenterPoint.x, CenterPoint.y+100}, BLACK);
        MainMenuButton = new ButtonMainMenu({CenterPoint.x-250, CenterPoint.y+100}, BLACK);
    }
    void Update()
    {
        if(game.IsGameOver())
        {
            ExitButton->Update();
            RetryButton->Update();
            MainMenuButton->Update();
        }
    }
    void Draw()
    {
        if(game.IsGameOver())
        {
            DrawTexturePro(texture, TextureFrame, DestFrame, {DestFrame.width/2.0f, DestFrame.height/2.0f}, 0, WHITE);
            DrawTexturePro(texture_text, TextTextureFrame, TextDestFrame, {TextDestFrame.width/2.0f, TextDestFrame.height/2.0f}, 0, WHITE);
            ExitButton->Draw();
            RetryButton->Draw();
            MainMenuButton->Draw();
            
        }
    }
};

class PausePanel
{    
    private:
    ButtonExit *ExitButton;
    ButtonMainMenu *MainMenuButton;
    ButtonResume *ResumeButton;
    
    Vector2 CenterPoint = {float(game.GetWindowWidth()/2.0f), float(game.GetWindowHeight()/2.0f)};


    Texture2D texture = game.AssignTexture(TEX_GUI_BUTTON);
    Rectangle TextureFrame;
    Rectangle DestFrame;

    Texture2D texture_text = game.AssignTexture(TEX_GUI_TEXT);
    Rectangle TextTextureFrame;
    Rectangle TextDestFrame;

    public:
    PausePanel()
    {
        TextureFrame = {7, 143, 80, 50};
        DestFrame = {CenterPoint.x, CenterPoint.y, TextureFrame.width*10, TextureFrame.height*10};
        TextTextureFrame = {66, 448, 397, 113};
        TextDestFrame = {CenterPoint.x, CenterPoint.y-100, TextTextureFrame.width*0.8f, TextTextureFrame.height*0.8f};
        ExitButton = new ButtonExit({CenterPoint.x+250, CenterPoint.y+100}, BLACK);
        ResumeButton = new ButtonResume({CenterPoint.x, CenterPoint.y+100}, BLACK);
        MainMenuButton = new ButtonMainMenu({CenterPoint.x-250, CenterPoint.y+100}, BLACK);
    }
    void Update()
    {
        if(game.IsPaused())
        {
            ExitButton->Update();
            ResumeButton->Update();
            MainMenuButton->Update();
        }
    }
    void Draw()
    {
        if(game.IsPaused())
        {
            DrawTexturePro(texture, TextureFrame, DestFrame, {DestFrame.width/2.0f, DestFrame.height/2.0f}, 0, WHITE);
            //DrawText(TextPause->text, TextPause->x-MeasureText(TextPause->text, TextPause->FontSize)/2.0f, TextPause->y, TextPause->FontSize, TextPause->TextColor);
            DrawTexturePro(texture_text, TextTextureFrame, TextDestFrame, {TextDestFrame.width/2.0f, TextDestFrame.height/2.0f}, 0, WHITE);
            ExitButton->Draw();
            ResumeButton->Draw();
            MainMenuButton->Draw();
        }
    }
};

class LevelClearPanel
{    
    private:
    ButtonExit *ExitButton;
    ButtonMainMenu *MainMenuButton;
    ButtonNextLevel *NextButton;
    
    Vector2 CenterPoint = {float(game.GetWindowWidth()/2.0f), float(game.GetWindowHeight()/2.0f)};

    Texture2D texture = game.AssignTexture(TEX_GUI_BUTTON);
    Rectangle TextureFrame;
    Rectangle DestFrame;

    Texture2D texture_text = game.AssignTexture(TEX_GUI_TEXT);
    Rectangle TextTextureFrame;
    Rectangle TextDestFrame;

    public:
    LevelClearPanel()
    {
        TextureFrame = {7, 143, 80, 50};
        DestFrame = {CenterPoint.x, CenterPoint.y, TextureFrame.width*10, TextureFrame.height*10};
        TextTextureFrame = {74, 262, 931, 102};
        TextDestFrame = {CenterPoint.x, CenterPoint.y-100, TextTextureFrame.width*0.8f, TextTextureFrame.height*0.8f};
        ExitButton = new ButtonExit({CenterPoint.x+250, CenterPoint.y+100}, BLACK);
        NextButton = new ButtonNextLevel({CenterPoint.x, CenterPoint.y+100}, BLACK);
        MainMenuButton = new ButtonMainMenu({CenterPoint.x-250, CenterPoint.y+100}, BLACK);
    }
    void Update()
    {
        if(game.IsStageCleared())
        {
            ExitButton->Update();
            NextButton->Update();
            MainMenuButton->Update();
        }
    }
    void Draw()
    {
        if(game.IsStageCleared())
        {
            DrawTexturePro(texture, TextureFrame, DestFrame, {DestFrame.width/2.0f, DestFrame.height/2.0f}, 0, WHITE);
            DrawTexturePro(texture_text, TextTextureFrame, TextDestFrame, {TextDestFrame.width/2.0f, TextDestFrame.height/2.0f}, 0, WHITE);
            ExitButton->Draw();
            NextButton->Draw();
            MainMenuButton->Draw();
        }
    }
};

class StatusBar
{
    private:
    Rectangle bar;
    float BarPercentage = 1;
    float MaxStatsValue;
    float StatsValue;
    float width;
    Vector2 CenterPoint;
    Color color;

    Rectangle border;

    public:
    StatusBar(Vector2 position, float width, float height, Color color, float MaxStatsValue, float StatsValue)
    {
        this->CenterPoint = position;
        this->bar = {CenterPoint.x-float((bar.width/BarPercentage)/2.0f), CenterPoint.y - float(bar.height)/2.0f, width*BarPercentage, height};
        this->border = {CenterPoint.x-float(border.width)/2.0f, CenterPoint.y - float(border.height)/2.0f, width+20, height+20};
        this->width = width;
        this->color = color;
        this->MaxStatsValue = MaxStatsValue;
        this->StatsValue = StatsValue;
    }
    void Update(Vector2 position, float MaxStatsValue, float StatsValue)
    {
        CenterPoint = position;
        BarPercentage = StatsValue / MaxStatsValue;
        bar.x = CenterPoint.x-width/2.0f;
        bar.y = CenterPoint.y-bar.height/2.0f-30;
        bar.width = width*BarPercentage;
        border.x = CenterPoint.x-border.width/2.0f;
        border.y = CenterPoint.y-border.height/2.0f-30;

        //cout<<CenterPoint->x<<" "<<CenterPoint->y<<endl;
    }
     void Draw()
     {
        DrawRectangleRec(bar, color);
     }
     void Draw(Color BorderColor)
     { 
        DrawRectangleRec(border, BorderColor);
        DrawRectangleRec(bar, color);
     }
};

//Class
class Entity
{
    protected:
    //stats related
    float health;
    float MaxHealth;
    float defense;
    float attack;
    float speed;            //scalar speed
    Vector2 speedV;         //vector speed
    bool IsAlive = true;

    //action related
    bool TrigShot = false;
    bool TrigMelee = false;
    bool TrigAttack = true;
    bool HitByMelee = false;
    bool MeleeHitSuccess = false;
    Vector2 MeleeAttackPoint;
    Vector2 MeleeAttackPointOri;
    Rectangle MeleeAttackBox;
    float MeleeAttackSwingRad;
    float MeleeAttackSwingRadOri;

    float AtkCooldown;
    double CurrentCooldown;
    double CooldownCounter;
    float AtkSpeed;

    bool FacingRight = true;

    bool MovementAllowed = true;

    //Frame & position related
    Vector2 position;
    Vector2 PreviousPosition;
    Rectangle TextureFrame;
    Rectangle DestFrame;
    Rectangle Hitbox;
    Vector2 CenterPoint;

    Texture2D texture;
    
    public:
    //Constructor & destructor
    Entity(float health, float defense, float attack, float speed, Vector2 position)
    {
        this->health = health;
        this->MaxHealth = health;
        this->defense = defense;
        this->attack = attack;
        this->speed = speed;
        this->speedV = {this->speed, this->speed};
        this->position = position;
        this->PreviousPosition = position;
    }
    ~Entity()
    {

    }

    //Functions
    //Property access functions
    //Stats related
    float GetHealth()
    {
        return health;
    }
    float GetMaxHealth()
    {
        return MaxHealth;
    }
    float GetDef()
    {
        return defense;
    }
    float GetSpeed()
    {
        return speed;
    }
    float GetAtk()
    {
        return attack;
    }

    Vector2 GetSpeedV()
    {
        return speedV;
    }

    bool GetLiveStatus()
    {
        if(health <= 0)
        {
            return false;
        }
        else
        {
            return true;
        }
        
    }

    //stats modification
    float AddHealth(float val)
    {
        return health + val;
    }
    float AddDefense(float val)
    {
        return defense + val;
    }
    float AddSpeed(float val)
    {
        return speed + val;
    }
    float AddAttack(float val)
    {
        return attack + val;
    }

    //Position related
    void SetPos(Vector2 position)
    {
        this->position = position;
        this->PreviousPosition = position;
    }

    Vector2 GetPos()
    {
        return position;
    }
    Vector2 GetPrevPos()
    {
        return PreviousPosition;
    }
    Vector2 GetCenterPoint()
    {
        return CenterPoint;
    }
    bool IsFacingRight()
    {
        return FacingRight;
    }

    //Hitbox and frame related
    Rectangle GetHitbox()
    {
        return Hitbox;
    }

    //Action related
    bool GetShotState()
    {
        return TrigShot;
    }

    bool GetMeleeState()
    {
        return TrigMelee;
    }
    bool IsHitByMelee()
    {
        return HitByMelee;
    }
    bool IsMeleeHitSuccess()
    {
        return MeleeHitSuccess;
    }
    
    void ModShotState(bool state)
    {
        TrigShot = state;
    }
    void ModMeleeState(bool state)
    {
        TrigMelee = state;
    }
    void ModHitByMelee(bool state)
    {
        HitByMelee = state;
    }
    void ModMeleeHitSuccess(bool state)

    {
        MeleeHitSuccess = state;
    }

    void TakeDamage(float damage)
    {
        if(IsAlive)
        {
            health -= float(damage-defense);
        }
    }

    virtual void MeleeHit(Entity& target)
    {
        if(CheckCollisionPointRec(MeleeAttackPoint, target.GetHitbox()) && TrigMelee && target.GetLiveStatus() && !target.IsHitByMelee())
        {
            target.TakeDamage(attack);
            target.ModHitByMelee(true);
        }
        if(MeleeAttackSwingRad == MeleeAttackSwingRadOri)
        {
            target.ModHitByMelee(false);
        }
    }

    //Movement related
    void MoveRight()
    {
        if(MovementAllowed)
        {
            PreviousPosition = position;
            position.x += speedV.x/game.ScaleFPS();
            if(!FacingRight)
            {
                FacingRight = true;
            }
        }
        
    }
    void MoveLeft()
    {
        if(MovementAllowed)
        {
            PreviousPosition = position;
            position.x -= speedV.x/game.ScaleFPS();
            if(FacingRight)
            {
                FacingRight = false;
            }
        }
        
    }
    void MoveUp()
    {
        if(MovementAllowed)
        {
            PreviousPosition = position;
            position.y -= speedV.y/game.ScaleFPS();
        }
    }
    void MoveDown()
    {
        if(MovementAllowed)
        {
            PreviousPosition = position;
            position.y += speedV.y/game.ScaleFPS();
        }
    }

    bool IsMovementAllowed()
    {
        return MovementAllowed;
    }
    void ModMovementAllowed(bool status)
    {
        MovementAllowed = status;
    }

    //Reset state function
    void ResetState()
    {
        IsAlive = true;
        health = MaxHealth;
    }

    //Update related
    void UpdateDestFrame()
    {
        //Update position
        DestFrame.x = position.x;
        DestFrame.y = position.y;
        //Update origin point
        CenterPoint.x = position.x;
        CenterPoint.y = position.y;
    }
    void UpdateHitbox()
    {
        //Update Hitbox position
        Hitbox.x = DestFrame.x - DestFrame.width/2.0f;
        Hitbox.y = DestFrame.y - DestFrame.height/2.0f;
    }
    virtual void UpdateMelee()
    {
        
    }

    virtual void AttackCD()
    {
        if(game.GetTimer() - CooldownCounter >= AtkCooldown)
        {
            TrigAttack = true;
        }
    }

    //Update & draw
    virtual void Update()
    {

    };
    virtual void Draw()
    {

    };
};

class Player:public Entity
{
    private:
    int AttackMode;
    float ShieldHealth;

    int Ammo;

    StatusBar *HealthBar;
    StatusBar *HealthBarBackground;
    Vector2 HealthBarPosition;

    Texture2D SwordTexture = game.AssignTexture(TEX_SWORDS);
    Rectangle SwordTextureFrame = {32, 16, 16, 16};
    Rectangle SwordDestFrame;
    float SwordRotationDeg;

    bool PlayerIsMoving = false;
    bool PlayerIsMelee = false;
    bool PlayerIsCasting = false;
    bool LockDirection = false;

    bool AnimationShotMagic = false;

    float AnimationIdleStage = 15;
    float AnimationMoveStage = 0;
    float AnimationMagicStage = 0;
    float AnimationMeleeStage = 0;

    public:
    int const MAX_BULLET = 50;

    //Constructor & destructor 100, 10, 20, 5, {280, 500}
    Player(Vector2 position):Entity(100, 10, 25, 5, position)
    {
        //Stats
        this->ShieldHealth = 500;

        this->texture = game.AssignTexture(TEX_PLAYER);

        //Frame & Hitbox
        this->TextureFrame = {14, 7, 19, 29};
        this->DestFrame = {position.x, position.y, 19*3, 29*3};
        this->Hitbox = {this->DestFrame.x-this->DestFrame.width/2.0f, this->DestFrame.y-this->DestFrame.height/2.0f, this->DestFrame.width, this->DestFrame.height};
        this->CenterPoint = position;

        this->SwordDestFrame = {DestFrame.x + 40, DestFrame.y, 32, 32};
        this->SwordRotationDeg = 0;

        this->MeleeAttackPointOri = {CenterPoint.x + float((Hitbox.width/2.0f+30)*cos(MeleeAttackSwingRad)), CenterPoint.y - float((Hitbox.height/2.0f+30)*sin(MeleeAttackSwingRad))};
        this->MeleeAttackPoint = MeleeAttackPointOri;
        this->MeleeAttackSwingRadOri = float(1.0/4.0);
        this->MeleeAttackSwingRad = MeleeAttackSwingRadOri;
        this->MeleeAttackBox = {CenterPoint.x, CenterPoint.y-DestFrame.height/2.0f, 75, DestFrame.height};
        this->AtkCooldown = 0.2;

        this->Ammo = 10;

        //HUD
        HealthBarPosition = {225, 60};
        HealthBar = new StatusBar(HealthBarPosition, 400, 20, RED, MaxHealth, health);
        HealthBarBackground = new StatusBar(HealthBarPosition, 400, 20, BLACK, 10, 10);
    };

    void DetectInput()
    {
        //Movement
        if(IsKeyDown(KEY_W))
        {
            MoveUp();
            PlayerIsMoving = true;
        }
        if(IsKeyDown(KEY_S))
        {
            MoveDown();
            PlayerIsMoving = true;
        }
        if(IsKeyDown(KEY_D))
        {
            if(IsKeyDown(KEY_LEFT_SHIFT) && IsFacingRight())
            {
                LockDirection = false;
            }
            else if(IsKeyDown(KEY_LEFT_SHIFT) && !IsFacingRight())
            {
                LockDirection = true;
            }
            MoveRight();
            if(LockDirection)
            {
                FacingRight = false;
            }
            PlayerIsMoving = true;
        }
        if(IsKeyDown(KEY_A))
        {
            if(IsKeyDown(KEY_LEFT_SHIFT) && IsFacingRight())
            {
                LockDirection = true;
            }
            else if(IsKeyDown(KEY_LEFT_SHIFT) && !IsFacingRight())
            {
                LockDirection = false;
            }
            MoveLeft();
            if(LockDirection)
            {
                FacingRight = true;
            }
            PlayerIsMoving = true;
        }
        if(!IsKeyDown(KEY_A) && !IsKeyDown(KEY_W) && !IsKeyDown(KEY_S) && !IsKeyDown(KEY_D))
        {
            PlayerIsMoving = false;
        }

        //Shot
        if(IsKeyPressed(KEY_SPACE) && Ammo > 0 && TrigAttack)
        {
            //ModShotState(true);
            PlayerIsCasting = true;
            Ammo--;
        }
        else if(IsKeyReleased(KEY_SPACE))
        {
            ModShotState(false);
        }

        //Melee
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && TrigAttack)
        {
            ModMeleeState(true);
            PlayerIsMelee = true;
        }
    }

    //Get & mod stats
    int GetAmmoLoaded()
    {
        return Ammo;
    }
    void AddAmmo(int amount)
    {
        Ammo += amount;
    }

    //Update related
    void Update() override
    {
        DetectInput();
        UpdateDestFrame();
        UpdateHitbox();
        UpdateTextureFrame();
        UpdateMelee();
        HealthBar->Update(HealthBarPosition, MaxHealth, health);
        HealthBarBackground->Update(HealthBarPosition, 100, 100);
        if(AnimationShotMagic)
        {
            ModShotState(true);
            AnimationShotMagic = false;
        }
        
    }

    void UpdateTextureFrame()
    {
        if(!PlayerIsMoving && !PlayerIsCasting && !PlayerIsMelee)
        {
            AnimationMoveStage = 0;
            AnimationMagicStage = 0;
            AnimationMeleeStage = 0;
            if(AnimationIdleStage < 15)
            {
                TextureFrame = {14, 7, 19, 29};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = TextureFrame.height*3;
                AnimationIdleStage += 1/game.ScaleFPS();
            }
            else if(AnimationIdleStage < 30)
            {
                TextureFrame = {66, 6, 16, 30};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = TextureFrame.height*3;
                AnimationIdleStage += 1/game.ScaleFPS();
            }
            else if(AnimationIdleStage < 45)
            {
                TextureFrame = {115, 6, 19, 30};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = TextureFrame.height*3;
                AnimationIdleStage += 1/game.ScaleFPS();
            }
            else if(AnimationIdleStage < 60)
            {
                TextureFrame = {163, 7, 20, 29};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = TextureFrame.height*3;
                AnimationIdleStage += 1/game.ScaleFPS();
                if(AnimationIdleStage >= 60)
                {
                    AnimationIdleStage = 0;
                }
            }
        }
        else if(PlayerIsMoving && !PlayerIsCasting && !PlayerIsMelee)
        {
            AnimationIdleStage = 15;
            AnimationMagicStage = 0;
            AnimationMeleeStage = 0;
            if(AnimationMoveStage < 10)
            {
                TextureFrame = {67, 45, 20, 28};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = TextureFrame.height*3;
                AnimationMoveStage += 1/game.ScaleFPS();
            }
            else if(AnimationMoveStage < 20)
            {
                TextureFrame = {116, 46, 20, 27};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = TextureFrame.height*3;
                AnimationMoveStage += 1/game.ScaleFPS();
            }
            else if(AnimationMoveStage < 30)
            {
                TextureFrame = {166, 48, 20, 25};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = TextureFrame.height*3;
                AnimationMoveStage += 1/game.ScaleFPS();
            }
            else if(AnimationMoveStage < 40)
            {
                TextureFrame = {217, 45, 23, 28};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = TextureFrame.height*3;
                AnimationMoveStage += 1/game.ScaleFPS();
            }
            else if(AnimationMoveStage < 50)
            {
                TextureFrame = {266, 46, 20, 27};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = TextureFrame.height*3;
                AnimationMoveStage += 1/game.ScaleFPS();
            }
            else if(AnimationMoveStage < 60)
            {
                TextureFrame = {316, 48, 20, 28};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = TextureFrame.height*3;
                AnimationMoveStage += 1/game.ScaleFPS();
                if(AnimationMoveStage >= 60)
                {
                    AnimationMoveStage = 0;
                }
            } 
        }
        else if(PlayerIsCasting && !PlayerIsMelee)
        {
            ModMovementAllowed(false);
            AnimationMoveStage = 0;
            AnimationIdleStage = 15;
            AnimationMeleeStage = 0;
            if(AnimationMagicStage < 7.5)
            {
                TextureFrame = {65, 454, 21, 26};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = (TextureFrame.height+4)*3;
                AnimationMagicStage += 1/game.ScaleFPS();
            }
            else if(AnimationMagicStage < 15)
            {
                TextureFrame = {115, 455, 18, 25};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = (TextureFrame.height+4)*3;
                AnimationMagicStage += 1/game.ScaleFPS();
            }
            else if(AnimationMagicStage < 22.5)
            {
                TextureFrame = {166, 455, 17, 25};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = (TextureFrame.height+4)*3;
                AnimationMagicStage += 1/game.ScaleFPS();
            }
            else if(AnimationMagicStage < 30)
            {
                TextureFrame = {215, 456, 27, 24};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = (TextureFrame.height+4)*3;
                AnimationMagicStage += 1/game.ScaleFPS();
                if(AnimationMagicStage >= 30)
                {
                    AnimationShotMagic = true;
                }
            }
            else if(AnimationMagicStage < 50)
            {
                AnimationMagicStage += 1/game.ScaleFPS();
                if(AnimationMagicStage >= 50)
                {
                    AnimationMagicStage = 0;
                    PlayerIsCasting = false;
                    ModMovementAllowed(true);
                }
            }
        }
        else if(PlayerIsMelee && !PlayerIsCasting)
        {
            ModMovementAllowed(false);
            AnimationIdleStage = 15;
            AnimationMagicStage = 0;
            AnimationMoveStage = 0;
            if(AnimationMeleeStage < 8)
            {
                TextureFrame = {13, 268, 20, 27};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = (TextureFrame.height+3)*3;
                AnimationMeleeStage += 1/game.ScaleFPS();
            }
            else if(AnimationMeleeStage < 16)
            {
                TextureFrame = {60, 266, 37, 29};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = (TextureFrame.height+1)*3;
                AnimationMeleeStage += 1/game.ScaleFPS();
            }
            else if(AnimationMeleeStage < 24)
            {
                TextureFrame = {102, 274, 32, 21};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = (TextureFrame.height+9)*3;
                AnimationMeleeStage += 1/game.ScaleFPS();
            }
            else if(AnimationMeleeStage < 32)
            {
                TextureFrame = {50, 312, 34, 20};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = (TextureFrame.height+10)*3;
                AnimationMeleeStage += 1/game.ScaleFPS();
            }
            else if(AnimationMeleeStage < 40)
            {
                TextureFrame = {100, 312, 34, 20};
                DestFrame.width = TextureFrame.width*3;
                DestFrame.height = (TextureFrame.height+10)*3;
                AnimationMeleeStage += 1/game.ScaleFPS();
            }
            else if(AnimationMeleeStage < 45)
            {
                AnimationMeleeStage += 1/game.ScaleFPS();
                if(AnimationMeleeStage >= 45)
                {
                    ModMovementAllowed(true);
                    AnimationMeleeStage = 0;
                    PlayerIsMelee = false;
                }
            }
        }

        if((IsFacingRight() && TextureFrame.width < 0) || (!IsFacingRight() && TextureFrame.width > 0))
        {
            TextureFrame.width *= -1;
        }
    }
    void UpdateMelee() override
    {
        if(TrigMelee && (AnimationMeleeStage < 32) && PlayerIsMelee && !PlayerIsCasting)
        {
            //MeleeAttackSwingRad >= -(1.0/4.0))
            MeleeAttackSwingRad -= 0.1/game.ScaleFPS();
            if(IsFacingRight())
            {
                MeleeAttackBox = {CenterPoint.x, CenterPoint.y-DestFrame.height/2.0f, 75, DestFrame.height};
            }
            else
            {
                MeleeAttackBox = {CenterPoint.x-75, CenterPoint.y-DestFrame.height/2.0f, 75, DestFrame.height};
            }
            
            CooldownCounter = game.GetTimer();
            TrigAttack = false;
            TrigMelee = true;
        }
        else
        {
            TrigMelee = false;
            if(TrigAttack)
            {
                SwordRotationDeg = 0;
            }
            MeleeAttackSwingRad = MeleeAttackSwingRadOri;

            AttackCD();
        }
        if(FacingRight)
        {
            MeleeAttackPointOri = {CenterPoint.x + float((Hitbox.width/2.0f+30)*cos(MeleeAttackSwingRad)), CenterPoint.y - float((Hitbox.height/2.0f+30)*sin(MeleeAttackSwingRad))};
            MeleeAttackBox = {CenterPoint.x, CenterPoint.y-DestFrame.height/2.0f, 75, DestFrame.height};
        }
        else
        {
            MeleeAttackPointOri = {CenterPoint.x - float((Hitbox.width/2.0f+30)*cos(MeleeAttackSwingRad)), CenterPoint.y - float((Hitbox.height/2.0f+30)*sin(MeleeAttackSwingRad))};
            MeleeAttackBox = {CenterPoint.x-75, CenterPoint.y-DestFrame.height/2.0f, 75, DestFrame.height};
        }
        
        MeleeAttackPoint = MeleeAttackPointOri;
    }
    void MeleeHit(Entity& target) override
    {
        if(CheckCollisionRecs(MeleeAttackBox, target.GetHitbox()) && TrigMelee && target.GetLiveStatus() && !target.IsHitByMelee())
        {
            target.TakeDamage(attack*1.5);
            if(FacingRight)
            {
                target.SetPos({target.GetPos().x+50, target.GetPos().y});
            }
            else
            {
                target.SetPos({target.GetPos().x-50, target.GetPos().y});
            }
            target.ModHitByMelee(true);
        }
        if(AnimationMeleeStage == 0)
        {
            target.ModHitByMelee(false);
        }
    }

    //Draw related
    void Draw() override
    {
        DrawTexturePro(texture, TextureFrame, DestFrame, {DestFrame.width/2.0f, DestFrame.height/2.0f}, 0, WHITE);
        
    }
    void DrawUI()
    {
        DrawText(TextFormat("x%d", Ammo), 125, 60, 40, WHITE);
        DrawTexturePro(game.AssignTexture(TEX_MAGIC_ARROW, 0), {0, 0, 100, 100}, {40, 50, 100*1.5, 100*1.5}, {50, 50}, 0, WHITE);
        HealthBar->Draw(MAROON);
        HealthBarBackground->Draw();
        HealthBar->Draw();
        

        /*if(FacingRight)
        {
            DrawText("right", 200, 50, 30, RED);
        }
        else
        {
            DrawText("left", 200, 50, 30, RED);
        }
        DrawText(TextFormat("%f", AnimationIdleStage), 300, 50, 30, BLUE);*/
        

    }
};

class Ally:public Entity
{
    private:
    int AllyType;
    int AbilityMode;
    int AbilityPoint;
    int HireCost;

    public:

};

class EnemyMeleeBasic:public Entity
{
    private:
    int AbilityMode;
    int EnemyType;

    bool IsMoving = false;
    bool IsAttacking = false;

    float AnimationIdleStage = 15;
    float AnimationMovingStage = 0;
    float AnimationAttackingStage = 0;
    
    StatusBar *HealthBar;

    public:
    EnemyMeleeBasic(Vector2 position):Entity(50, 0, 20, 3, position)
    {

            this->texture = game.AssignTexture(TEX_GOBLIN);

            this->TextureFrame = {56, 192, 33, 36};
            this->DestFrame = {position.x, position.y, TextureFrame.width*2, TextureFrame.height*2};
            this->Hitbox = {this->DestFrame.x-this->DestFrame.width/2.0f, this->DestFrame.y-this->DestFrame.height/2.0f, this->DestFrame.width, this->DestFrame.height};
            this->CenterPoint = position;
            this->PreviousPosition = position;

            this->MeleeAttackPointOri = {CenterPoint.x + 45, CenterPoint.y - 10};
            this->MeleeAttackPoint = MeleeAttackPointOri;
            this->MeleeAttackBox = {CenterPoint.x, CenterPoint.y-Hitbox.height/2.0f, 50, Hitbox.height};
            this->AtkCooldown = 0.5;
            this->AtkSpeed = 0.7;

            HealthBar = new StatusBar({this->CenterPoint.x, this->CenterPoint.y-10}, 50, 10, RED, this->MaxHealth, this->health);
    }

    void Update() override
    {
        if(health <= 0)
        {
            IsAlive = false;
        }
        if(IsAlive)
        {
            UpdateDestFrame();
            UpdateHitbox();
            UpdateTextureFrame();
            UpdateMelee();
            HealthBar->Update({position.x, position.y-10}, MaxHealth, health);
            if(PreviousPosition.x == position.x && PreviousPosition.y == position.y)
            {
                IsMoving = false;
            }
            else
            {
                IsMoving = true;
            }
            PreviousPosition = position;
        }
    }
    void UpdateTextureFrame()
    {
        if(!IsMoving && !IsAttacking)
        {
            AnimationMovingStage = 0;
            AnimationAttackingStage = 0;
            if(AnimationIdleStage < 15)
            {
                TextureFrame = {56, 192, 33, 36};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationIdleStage += 1/game.ScaleFPS();
            }
            else if(AnimationIdleStage < 30)
            {
                TextureFrame = {206, 193, 33, 35};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationIdleStage += 1/game.ScaleFPS();
            }
            else if(AnimationIdleStage < 45)
            {
                TextureFrame = {356, 194, 33, 34};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationIdleStage += 1/game.ScaleFPS();
            }
            else if(AnimationIdleStage < 60)
            {
                TextureFrame = {506, 196, 33, 32};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationIdleStage += 1/game.ScaleFPS();
                if(AnimationIdleStage >= 60)
                {
                    AnimationIdleStage = 0;
                }
            }
        }
        else if(IsMoving && !IsAttacking)
        {
            AnimationIdleStage = 15;
            AnimationAttackingStage = 0;
            if(AnimationMovingStage < 7.5)
            {
                TextureFrame = {56, 306, 35, 36};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationMovingStage += 1/game.ScaleFPS();
            }
            else if(AnimationMovingStage < 15)
            {
                TextureFrame = {209, 307, 27, 37};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationMovingStage += 1/game.ScaleFPS();
            }
            else if(AnimationMovingStage < 22.5)
            {
                TextureFrame = {366, 307, 20, 37};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationMovingStage += 1/game.ScaleFPS();
            }
            else if(AnimationMovingStage < 30)
            {
                TextureFrame = {514, 308, 26, 36};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationMovingStage += 1/game.ScaleFPS();
            }
            else if(AnimationMovingStage < 37.5)
            {
                TextureFrame = {661, 306, 33, 35};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationMovingStage += 1/game.ScaleFPS();
            }
            else if(AnimationMovingStage < 45)
            {
                TextureFrame = {817, 307, 25, 37};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationMovingStage += 1/game.ScaleFPS();
            }
            else if(AnimationMovingStage < 52.5)
            {
                TextureFrame = {969, 307, 23, 37};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationMovingStage += 1/game.ScaleFPS();
            }
            else if(AnimationMovingStage < 60)
            {
                TextureFrame = {1112, 308, 26, 36};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationMovingStage += 1/game.ScaleFPS();
                if(AnimationMovingStage >= 60)
                {
                    AnimationMovingStage = 0;
                }
            }
        }
        else if(IsAttacking)
        {
            AnimationIdleStage = 15;
            AnimationMovingStage = 0;
            if(AnimationAttackingStage < 7.5*AtkSpeed)
            {
                TextureFrame = {56, 412, 33, 36};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationAttackingStage += 1/game.ScaleFPS();
            }
            else if(AnimationAttackingStage < 15*AtkSpeed)
            {
                TextureFrame = {208, 412, 28, 36};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationAttackingStage += 1/game.ScaleFPS();
            }
            else if(AnimationAttackingStage < 22.5*AtkSpeed)
            {
                TextureFrame = {358, 414, 29, 34};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationAttackingStage += 1/game.ScaleFPS();
            }
            else if(AnimationAttackingStage < 30*AtkSpeed)
            {
                TextureFrame = {508, 414, 29, 34};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationAttackingStage += 1/game.ScaleFPS();
            }
            else if(AnimationAttackingStage < 37.5*AtkSpeed)
            {
                TextureFrame = {658, 414, 29, 34};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationAttackingStage += 1/game.ScaleFPS();
            }
            else if(AnimationAttackingStage < 45*AtkSpeed)
            {
                TextureFrame = {808, 413, 31, 35};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationAttackingStage += 1/game.ScaleFPS();
            }
            else if(AnimationAttackingStage < 52.5*AtkSpeed)
            {
                TextureFrame = {929, 403, 88, 45};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationAttackingStage += 1/game.ScaleFPS();
            }
            else if(AnimationAttackingStage < 60*AtkSpeed)
            {
                TextureFrame = {1079, 403, 85, 45};
                DestFrame.width = TextureFrame.width*2;
                DestFrame.height = TextureFrame.height*2;
                AnimationAttackingStage += 1/game.ScaleFPS();
                if(AnimationAttackingStage >= 60*AtkSpeed)
                {
                    AnimationAttackingStage = 0;
                    IsAttacking = false;
                }
            }
        }

        if((IsFacingRight() && TextureFrame.width < 0) || (!IsFacingRight() && TextureFrame.width > 0))
        {
            TextureFrame.width *= -1;
        }
    }

    void UpdateMelee() override
    {
        if(TrigMelee && IsAlive && AnimationAttackingStage < 52.5*AtkSpeed)
        {
            CooldownCounter = game.GetTimer();
            TrigAttack = false;
        }
        else
        {
            TrigMelee = false;
            MeleeAttackPoint = MeleeAttackPointOri;
            MovementAllowed = true;
            AttackCD();
        }
        if(FacingRight)
        {
            MeleeAttackBox = {position.x, position.y-Hitbox.height/2.0f, 50, Hitbox.height};
        }
        else
        {
            MeleeAttackBox = {position.x-50, position.y-Hitbox.height/2.0f, 50, Hitbox.height};
        }
    }

    void DetectMeleeRange(Entity& target)
    {
        if((CheckCollisionPointRec({CenterPoint.x - 50, CenterPoint.y}, target.GetHitbox()) || CheckCollisionPointRec({CenterPoint.x + 45, CenterPoint.y}, target.GetHitbox())))
        {
            if(!CheckCollisionPointRec({CenterPoint.x - 50, CenterPoint.y}, target.GetHitbox()) && !IsFacingRight())
            {
                FacingRight = true;
            }
            if(TrigAttack)
            {
                TrigMelee = true;
                IsAttacking = true;
            }
            MovementAllowed = false;
        }
        
    }

    void MeleeHit(Entity& target) override
    {
        if(CheckCollisionRecs(MeleeAttackBox, target.GetHitbox()) && TrigMelee && target.GetLiveStatus() && !IsMeleeHitSuccess() && IsAlive && (AnimationAttackingStage < 52.5*AtkSpeed && AnimationAttackingStage > 37.5*AtkSpeed))
        {
            target.TakeDamage(attack);
            ModMeleeHitSuccess(true);
        }
        else if(AnimationAttackingStage >= 52.5*AtkSpeed)
        {
            ModMeleeHitSuccess(false);
        }
    }

    void Draw() override
    {
        if(IsAlive)
        {
            DrawTexturePro(texture, TextureFrame, DestFrame, {DestFrame.width/2.0f, DestFrame.height/2.0f}, 0, WHITE);
            HealthBar->Draw();
        }
    }

};

class EnemyRangedBasic:public Entity
{
    private:
    bool ShotDelay = true;
    bool AllowShot = false;

    bool IsMoving = false;
    bool IsAttacking = false;

    float AnimationIdleStage = 15;
    float AnimationMovingStage = 0;
    float AnimationAttackingStage = 0;

    StatusBar *HealthBar;

    public:
    EnemyRangedBasic(Vector2 position):Entity(0, 0, 0, 0, position)
    {
            this->health = 30;
            this->MaxHealth = health;
            this->defense = 10;
            this->attack = 30;
            this->position = position;
            this->speed = 3;
            this->speedV = {speed, speed};

            this->texture = game.AssignTexture(TEX_KADAL, 0);

            this->TextureFrame = {0, 0, 128, 128};
            this->DestFrame = {position.x, position.y, TextureFrame.width*1.5f, TextureFrame.height*1.5f};
            this->Hitbox = {this->DestFrame.x-this->DestFrame.width/2.0f, this->DestFrame.y-this->DestFrame.height/2.0f, this->DestFrame.width-50, this->DestFrame.height-35};
            this->CenterPoint = position;

            this->AtkCooldown = 3;
            this->AtkSpeed = 1;
            this->CooldownCounter = game.GetTimer();

            HealthBar = new StatusBar({this->CenterPoint.x-20, this->CenterPoint.y}, 50, 10, RED, this->MaxHealth, this->health);
    }

    void Update() override
    {
        if(health <= 0)
        {
            IsAlive = false;
        }
        if(IsAlive)
        {
            UpdateDestFrame();
            UpdateHitbox();
            UpdateTextureFrame();
            //UpdateShot();
            HealthBar->Update({position.x-20, position.y}, MaxHealth, health);
        }
        if(PreviousPosition.x == position.x && PreviousPosition.y == position.y)
        {
            IsMoving = false;
        }
        else
        {
            IsMoving = true;
        }
    }

    void UpdateTextureFrame()
    {
        if(!IsMoving && !IsAttacking)
        {
            AnimationMovingStage = 0;
            AnimationAttackingStage = 0;
            if(AnimationIdleStage < 15)
            {
                texture = game.AssignTexture(TEX_KADAL, 0);
                AnimationIdleStage += 1/game.ScaleFPS();
            }
            else if(AnimationIdleStage < 30)
            {
                texture = game.AssignTexture(TEX_KADAL, 1);
                AnimationIdleStage += 1/game.ScaleFPS();
            }
            else if(AnimationIdleStage < 45)
            {
                texture = game.AssignTexture(TEX_KADAL, 2);
                AnimationIdleStage += 1/game.ScaleFPS();
                if(AnimationIdleStage >= 45)
                {
                    AnimationIdleStage = 0;
                }
            }
        }
        else if(IsMoving && !IsAttacking)
        {
            AnimationIdleStage = 15;
            AnimationAttackingStage = 0;
            if(AnimationMovingStage < 15)
            {
                texture = game.AssignTexture(TEX_KADAL, 3);
                AnimationMovingStage += 1/game.ScaleFPS();
            }
            else if(AnimationMovingStage < 30)
            {
                texture = game.AssignTexture(TEX_KADAL, 4);
                AnimationMovingStage += 1/game.ScaleFPS();
            }
            else if(AnimationMovingStage < 45)
            {
                texture = game.AssignTexture(TEX_KADAL, 5);
                AnimationMovingStage += 1/game.ScaleFPS();
            }
            else if(AnimationMovingStage < 60)
            {
                texture = game.AssignTexture(TEX_KADAL, 6);
                AnimationMovingStage += 1/game.ScaleFPS();
                if(AnimationMovingStage >= 60)
                {
                    AnimationMovingStage = 0;
                }
            }
        }
        else if(IsAttacking)
        {
            AnimationIdleStage = 15;
            AnimationMovingStage = 0;
            if(AnimationAttackingStage < 20*AtkSpeed)
            {
                texture = game.AssignTexture(TEX_KADAL, 7);
                AnimationAttackingStage += 1/game.ScaleFPS();
            }
            else if(AnimationAttackingStage < 40*AtkSpeed)
            {
                texture = game.AssignTexture(TEX_KADAL, 8);
                AnimationAttackingStage += 1/game.ScaleFPS();
            }
            else if(AnimationAttackingStage < 60*AtkSpeed)
            {
                texture = game.AssignTexture(TEX_KADAL, 9);
                AnimationAttackingStage += 1/game.ScaleFPS();
                if(AnimationAttackingStage >= 60*AtkSpeed)
                {
                    AnimationAttackingStage = 0;
                    IsAttacking = false;
                }
            }
        }

        if((IsFacingRight() && TextureFrame.width < 0) || (!IsFacingRight() && TextureFrame.width > 0))
        {
            TextureFrame.width *= -1;
        }
    }

    void ModAllowShot(bool status)
    {
        AllowShot = status;
    }

    void UpdateShot()
    {
        if(ShotDelay)
        {
            ShotDelay = false;
            CooldownCounter = game.GetTimer();
            ModMovementAllowed(false);
        }
        else
        {
            if(game.GetTimer() - CooldownCounter >= 0.5 && TrigAttack && AllowShot)
            {
                //ModShotState(true);
                IsAttacking = true;
                
                TrigAttack = false;
                //ShotDelay = true;
            }
            else if(AnimationAttackingStage >= 59)
            {
                ModShotState(true);
            }
        }
        if(position.x > 1920 && position.x < 0 && position.y > 1080 && position.y < 0)
        {
            AllowShot = false;
        }
        else
        {
            AllowShot = true;
        }
        if(!TrigAttack)
        {
            if(game.GetTimer() - CooldownCounter >= 1 && !IsAttacking)
            {
                ModMovementAllowed(true);
                ModShotState(false);
            }
            AttackCD();
            if(TrigAttack)
            {
                ShotDelay = true;
            }
        }
    }

    void Draw() override
    {
        if(IsAlive)
        {
            DrawTexturePro(texture, TextureFrame, DestFrame, {DestFrame.width/2.0f, DestFrame.height/2.0f}, 0, WHITE);
            HealthBar->Draw();
        }
    }
};

//Wall, obstacle, etc.
class Wall
{
    private:
    Vector2 position;
    Vector2 CenterPoint;
    struct Size
    {
        float width;
        float height;
    };
    Size size;
    Rectangle Hitbox;
    int orientation;

    Texture2D texture;
    Color color;

    Rectangle TextureFrameCornerTop, TextureFrameCornerMid, TextureFrameCornerBottom, TextureFrameCornerTopR, TextureFrameCornerMidR, TextureFrameCornerBottomR, TextureFrameTop, TextureFrameMid, TextureFrameBottom;

    public:
    Wall(Vector2 position, Size size, int orientation)
    {
        this->position = position;
        this->CenterPoint = position;
        this->size = size;

        this->Hitbox = {CenterPoint.x - size.width/2.0f, CenterPoint.y - size.height/2.0f, size.width, size.height};
        this->orientation = orientation;

        this->TextureFrameCornerTop = {236, 360, 46, 55};
        this->TextureFrameCornerMid = {236, 427, 46, 26};
        this->TextureFrameCornerBottom = {236, 467, 46, 26};
        this->TextureFrameCornerTopR = {236, 360, -46, 55};
        this->TextureFrameCornerMidR = {236, 427, -46, 26};
        this->TextureFrameCornerBottomR = {236, 467, -46, 26};
        this->TextureFrameTop = {409, 360, 63, 55};
        this->TextureFrameMid = {409, 427, 63, 26};
        this->TextureFrameBottom = {409, 467, 63, 26};

        this->texture = game.AssignTexture(TEX_WALL);
    }
    Wall(Vector2 position, Size size, int orientation, Color color)
    {
        this->position = position;
        this->CenterPoint = position;
        this->size = size;

        this->Hitbox = {CenterPoint.x - size.width/2.0f, CenterPoint.y - size.height/2.0f, size.width, size.height};
        this->orientation = orientation;

        this->TextureFrameCornerTop = {236, 360, 46, 55};
        this->TextureFrameCornerMid = {236, 427, 46, 26};
        this->TextureFrameCornerBottom = {236, 467, 46, 26};
        this->TextureFrameCornerTopR = {236, 360, -46, 55};
        this->TextureFrameCornerMidR = {236, 427, -46, 26};
        this->TextureFrameCornerBottomR = {236, 467, -46, 26};
        this->TextureFrameTop = {346, 360, 63, 55};
        this->TextureFrameMid = {346, 427, 63, 26};
        this->TextureFrameBottom = {346, 467, 63, 26};
        
        this->color = color;
        this->texture = game.AssignTexture(TEX_WALL);
    }

    Rectangle GetHitbox()
    {
        return Hitbox;
    }

    void Update(Entity& entity)
    {
        DetectCollision(entity);
    }

    void DetectCollision(Entity& entity)
    {
        if(CheckCollisionRecs(Hitbox, entity.GetHitbox()))
        {
            if((CenterPoint.x - entity.GetCenterPoint().x) > 0 && (entity.GetCenterPoint().y > (CenterPoint.y - Hitbox.height/2.0f) && entity.GetCenterPoint().y < (CenterPoint.y + Hitbox.height/2.0f)))
            {
                entity.SetPos({(CenterPoint.x - Hitbox.width/2.0f - entity.GetHitbox().width/2.0f), entity.GetPos().y});
            }
            else if((CenterPoint.x - entity.GetCenterPoint().x) < 0 && (entity.GetCenterPoint().y > (CenterPoint.y - Hitbox.height/2.0f) && entity.GetCenterPoint().y < (CenterPoint.y + Hitbox.height/2.0f)))
            {
                entity.SetPos({(CenterPoint.x + Hitbox.width/2.0f + entity.GetHitbox().width/2.0f), entity.GetPos().y});
            }
            else if((CenterPoint.y - entity.GetCenterPoint().y) > 0)
            {
                entity.SetPos({entity.GetPos().x, CenterPoint.y - Hitbox.height/2.0f - entity.GetHitbox().height/2.0f});
            }
            else if((CenterPoint.y - entity.GetCenterPoint().y) < 0)
            {
                entity.SetPos({entity.GetPos().x, CenterPoint.y + Hitbox.height/2.0f + entity.GetHitbox().height/2.0f});
            }
        }
    }

    void Draw()
    {
        DrawRectangleRec(Hitbox, color);
        
        int WidthIteration = abs((size.width-92)/TextureFrameMid.width);
        int HeightIteration = abs((size.height-81)/TextureFrameMid.height);
        float WidthScaling = WidthIteration/float((size.width-92)/TextureFrameMid.width);
        float HeightScaling = HeightIteration/float((size.height-81)/TextureFrameMid.height);

        WidthIteration++;
        HeightIteration++;

        if(orientation == TOP)
        {
            DrawTexturePro(texture, TextureFrameCornerTop, {CenterPoint.x, CenterPoint.y, 46/WidthScaling, 55/HeightScaling}, {Hitbox.width/2.0f, Hitbox.height/2.0f}, 0 , WHITE);
            DrawTexturePro(texture, TextureFrameCornerBottom, {CenterPoint.x, CenterPoint.y, 46/WidthScaling, 26/HeightScaling}, {Hitbox.width/2.0f, Hitbox.height/2.0f-(26*(HeightIteration+2))}, 0 , WHITE);
            for(int i = 0; i < WidthIteration; i++)
            {
                DrawTexturePro(texture, TextureFrameTop, {CenterPoint.x, CenterPoint.y, 63/WidthScaling, 55/HeightScaling}, {Hitbox.width/2.0f-(46+(63*i)), Hitbox.height/2.0f}, 0 , WHITE);
                for(int j = 0; j < HeightIteration; j++)
                {
                    DrawTexturePro(texture, TextureFrameMid, {CenterPoint.x, CenterPoint.y, 63/WidthScaling, 26/HeightScaling}, {Hitbox.width/2.0f-(46+(63*i)), Hitbox.height/2.0f-(55+(26*j))}, 0 , WHITE);
                    DrawTexturePro(texture, TextureFrameCornerMid, {CenterPoint.x, CenterPoint.y, 46/WidthScaling, 26/HeightScaling}, {Hitbox.width/2.0f, Hitbox.height/2.0f-(55+(26*j))}, 0 , WHITE);
                    DrawTexturePro(texture, TextureFrameCornerMidR, {CenterPoint.x, CenterPoint.y, 46/WidthScaling, 26/HeightScaling}, {Hitbox.width/2.0f-(63*WidthIteration-1), Hitbox.height/2.0f-(55+(26*j))}, 0 , WHITE);
                }
                DrawTexturePro(texture, TextureFrameBottom, {CenterPoint.x, CenterPoint.y, 63/WidthScaling, 26/HeightScaling}, {Hitbox.width/2.0f-(46+(63*i)), Hitbox.height/2.0f-(26*(HeightIteration+2))}, 0 , WHITE);
            }
            DrawTexturePro(texture, TextureFrameCornerTopR, {CenterPoint.x, CenterPoint.y, 46/WidthScaling, 55/HeightScaling}, {Hitbox.width/2.0f-(63*(WidthIteration)), Hitbox.height/2.0f}, 0 , WHITE);
            DrawTexturePro(texture, TextureFrameCornerBottomR, {CenterPoint.x, CenterPoint.y, 46/WidthScaling, 26/HeightScaling}, {Hitbox.width/2.0f-(63*WidthIteration), Hitbox.height/2.0f-(26*(HeightIteration+2))}, 0 , WHITE);
        }
        else if(orientation == BOTTOM)
        {
            DrawTexturePro(texture, TextureFrameCornerTop, {CenterPoint.x, CenterPoint.y, 46/WidthScaling, 55/HeightScaling}, {Hitbox.width/2.0f, Hitbox.height/2.0f}, 180 , WHITE);
            DrawTexturePro(texture, TextureFrameCornerBottom, {CenterPoint.x, CenterPoint.y, 46/WidthScaling, 26/HeightScaling}, {Hitbox.width/2.0f, Hitbox.height/2.0f-(26*(HeightIteration+2))}, 180 , WHITE);
            for(int i = 0; i < WidthIteration; i++)
            {
                DrawTexturePro(texture, TextureFrameTop, {CenterPoint.x, CenterPoint.y, 63/WidthScaling, 55/HeightScaling}, {Hitbox.width/2.0f-(46+(63*i)), Hitbox.height/2.0f}, 180 , WHITE);
                for(int j = 0; j < HeightIteration; j++)
                {
                    DrawTexturePro(texture, TextureFrameMid, {CenterPoint.x, CenterPoint.y, 63/WidthScaling, 26/HeightScaling}, {Hitbox.width/2.0f-(46+(63*i)), Hitbox.height/2.0f-(55+(26*j))}, 180 , WHITE);
                    DrawTexturePro(texture, TextureFrameCornerMid, {CenterPoint.x, CenterPoint.y, 46/WidthScaling, 26/HeightScaling}, {Hitbox.width/2.0f, Hitbox.height/2.0f-(55+(26*j))}, 180 , WHITE);
                    DrawTexturePro(texture, TextureFrameCornerMidR, {CenterPoint.x, CenterPoint.y, 46/WidthScaling, 26/HeightScaling}, {Hitbox.width/2.0f-(63*WidthIteration-1), Hitbox.height/2.0f-(55+(26*j))}, 180 , WHITE);
                }
                DrawTexturePro(texture, TextureFrameBottom, {CenterPoint.x, CenterPoint.y, 63/WidthScaling, 26/HeightScaling}, {Hitbox.width/2.0f-(46+(63*i)), Hitbox.height/2.0f-(26*(HeightIteration+2))}, 180 , WHITE);
            }
            DrawTexturePro(texture, TextureFrameCornerTopR, {CenterPoint.x, CenterPoint.y, 46/WidthScaling, 55/HeightScaling}, {Hitbox.width/2.0f-(63*(WidthIteration)), Hitbox.height/2.0f}, 180 , WHITE);
            DrawTexturePro(texture, TextureFrameCornerBottomR, {CenterPoint.x, CenterPoint.y, 46/WidthScaling, 26/HeightScaling}, {Hitbox.width/2.0f-(63*WidthIteration), Hitbox.height/2.0f-(26*(HeightIteration+2))}, 180 , WHITE);
        }
    }
    
};
class Tile
{
    protected:
    Vector2 position;
    Vector2 CenterPoint;
    struct Size
    {
        float width;
        float height;
    };
    Size size;
    Rectangle tile;
    Color color;

    Rectangle TextureFrame;
    Rectangle DestFrame;
    Texture2D texture;

    public:
    Tile(Vector2 position, float width, float height)
    {
        this->position = position;
        this->CenterPoint = position;
        this->size = {width, height};
        this->TextureFrame = {33, 128, 30, 31};
        this->DestFrame = {CenterPoint.x, CenterPoint.y, size.width, size.height+1};

        this->texture = game.AssignTexture(TEX_TILE);

        this->tile = {CenterPoint.x - size.width/2.0f, CenterPoint.y - size.height/2.0f, size.width, size.height};
    }

    void Draw()
    {
        DrawRectangleRec(tile, GRAY);
        DrawTexturePro(texture, TextureFrame, DestFrame, {DestFrame.width/2.0f, DestFrame.height/2.0f}, 0, WHITE);
    }

};

class DeployTile:public Tile
{
    private:
    Player *player;

    public:
    DeployTile(Vector2 position, float width, float height, Player& player):Tile(position, width, height)
    {
        this->player = &player;
    }

    void Update()
    {
        if(CheckCollisionRecs(player->GetHitbox(), tile))
        {
            
        }
    }
};
//Attack type class
//Projectile
class Projectile
{
    protected:
    float speed;
    bool IsActive = false;
    bool CommitMovementRight = false;
    Vector2 speedV;

    //position & frame
    Vector2 position;
    Vector2 origin;
    Vector2 CenterPoint;
    Rectangle TextureFrame;
    Rectangle DestFrame;
    Rectangle Hitbox;

    Texture2D texture;

    public:
    Projectile(float speed, Vector2 position)
    {
        this->speed = speed;
        this->speedV = {speed, speed};
        this->position = position;
        this->origin = position;
    }
    ~Projectile()
    {

    }

    float GetSpeed()
    {
        return speed;
    }
    Vector2 GetSpeedV()
    {
        return speedV;
    }

    virtual void Update() = 0;
    virtual void Draw() = 0;
};

class Bullet:public Projectile
{
    private:
    float TravelDist = 0;
    float const MAX_TRAVEL_DISTANCE = 2000;

    float AnimationStage = 0;

    Entity *entity;

    public:
    Bullet(float speed, Vector2 position, Entity& entity):Projectile(speed, position)
    {
        this->texture = game.AssignTexture(TEX_MAGIC_ARROW, 0);
        this->entity = &entity;

        //Frame & hitbox
        this->TextureFrame = {0, 0, 100, 100};
        this->DestFrame = {position.x, position.y, TextureFrame.width, TextureFrame.height};
        this->Hitbox = {this->DestFrame.x-this->DestFrame.width/2.0f, this->DestFrame.y-this->DestFrame.height/2.0f, this->DestFrame.width-50, this->DestFrame.height-70};
        this->CenterPoint = position;


    }
    void Update() override
    {
        //Shot trigger
        if(!IsActive)
        {
            position = origin;
            ShotTrigger(entity->GetShotState());
            AnimationStage = 0;
            texture = game.AssignTexture(TEX_MAGIC_ARROW, 0);
        }
        //update position & texture
        if(IsActive)
        {
            //Update position
            if(CommitMovementRight)
            {
                position.x += speedV.x/game.ScaleFPS();
                TravelDist += speedV.x/game.ScaleFPS();
            }
            else
            {
                position.x -= speedV.x/game.ScaleFPS();
                TravelDist -= speedV.x/game.ScaleFPS();
            }

            //update texture frame
            if(entity->IsFacingRight() && TextureFrame.width < 0 && CommitMovementRight)
            {
                TextureFrame.width *= -1;
            }
            if(!entity->IsFacingRight() && TextureFrame.width > 0 && !CommitMovementRight)
            {
                TextureFrame.width *= -1;
            }

            //update texture
            UpdateTexture();

            //Update dest frame
            DestFrame.x = position.x;
            DestFrame.y = position.y;
            //update center point
            CenterPoint.x = position.x;
            CenterPoint.y = position.y;
            //update hitbox
            Hitbox.x = DestFrame.x - DestFrame.width/2.0f;
            Hitbox.y = DestFrame.y - DestFrame.height/2.0f;
        }

        //update origin
        origin = entity->GetCenterPoint();

        //max travel
        if(TravelDist > MAX_TRAVEL_DISTANCE && entity->IsFacingRight())
        {
            IsActive = false;
            CommitMovementRight = false;
            position.x = origin.x;
            position.y = origin.y;
            TravelDist = 0;
        }
        else if(TravelDist < -1*(MAX_TRAVEL_DISTANCE) && !entity->IsFacingRight())
        {
            IsActive = false;
            CommitMovementRight = false;
            position.x = origin.x;
            position.y = origin.y;
            TravelDist = 0;
        }
    }

    void UpdateTexture()
    {
        if(AnimationStage < 30)
        {
            texture = game.AssignTexture(TEX_MAGIC_ARROW, AnimationStage);
            AnimationStage += 1/game.ScaleFPS();
        }
        else
        {
            AnimationStage = 0;
        }
        
    }

    void ShotTrigger(bool trigger)
    {
        if(trigger)
        {
            IsActive = true;
            entity->ModShotState(false);
            if(entity->IsFacingRight())
            {
                CommitMovementRight = true;
            }
        }
    }

    void DetectCollisionEntity(Entity& target)
    {
        if(IsActive)
        {
            if(CheckCollisionRecs(Hitbox, target.GetHitbox()) && target.GetLiveStatus())
            {
                IsActive = false;
                TravelDist = 0;
                DealDamage(target);
            }
        }
    }
    void DetectCollisionWall(Wall& wall)
    {
        if(IsActive)
        {
            if(CheckCollisionRecs(Hitbox, wall.GetHitbox()))
            {
                IsActive = false;
                TravelDist = 0;
            }
        }
    }

    void DealDamage(Entity& target)
    {
        target.TakeDamage(this->entity->GetAtk());
    }

    void Draw() override
    {
        if(IsActive)
        {
            DrawTexturePro(texture, TextureFrame, DestFrame, {DestFrame.width/2.0f, DestFrame.height/2.0f}, 0, WHITE);
        }
    }
};

class BulletEnemy:public Projectile
{
    private:
    float TravelDist = 0;
    float const MAX_TRAVEL_DISTANCE = 2000;

    float AnimationStage = 0;

    Entity *entity;

    public:
    BulletEnemy(float speed, Vector2 position, Entity& entity):Projectile(speed, position)
    {
        this->texture = game.AssignTexture(TEX_KADAL, 10);
        this->entity = &entity;

        //Frame & hitbox
        this->TextureFrame = {0, 0, 64, 64};
        this->DestFrame = {position.x, position.y, TextureFrame.width*2, TextureFrame.height*2};
        this->Hitbox = {this->DestFrame.x-this->DestFrame.width/2.0f, this->DestFrame.y-this->DestFrame.height/2.0f, this->DestFrame.width-20, this->DestFrame.height-10};
        this->CenterPoint = position;


    }
    void Update() override
    {
        //Shot trigger
        if(!IsActive)
        {
            position = origin;
            ShotTrigger(entity->GetShotState());
            AnimationStage = 0;
            texture = game.AssignTexture(TEX_KADAL, 0);
        }
        //update position & texture
        if(IsActive)
        {
            //Update position
            if(CommitMovementRight)
            {
                position.x += speedV.x/game.ScaleFPS();
                TravelDist += speedV.x/game.ScaleFPS();
            }
            else
            {
                position.x -= speedV.x/game.ScaleFPS();
                TravelDist -= speedV.x/game.ScaleFPS();
            }

            //update texture frame
            if(entity->IsFacingRight() && TextureFrame.width < 0 && CommitMovementRight)
            {
                TextureFrame.width *= -1;
            }
            if(!entity->IsFacingRight() && TextureFrame.width > 0 && !CommitMovementRight)
            {
                TextureFrame.width *= -1;
            }

            //update texture
            UpdateTexture();

            //Update dest frame
            DestFrame.x = position.x;
            DestFrame.y = position.y;
            //update center point
            CenterPoint.x = position.x;
            CenterPoint.y = position.y;
            //update hitbox
            Hitbox.x = DestFrame.x - DestFrame.width/2.0f;
            Hitbox.y = DestFrame.y - DestFrame.height/2.0f;
        }

        //update origin
        origin = entity->GetCenterPoint();

        //max travel
        if(TravelDist > MAX_TRAVEL_DISTANCE && entity->IsFacingRight())
        {
            IsActive = false;
            CommitMovementRight = false;
            position.x = origin.x;
            position.y = origin.y;
            TravelDist = 0;
        }
        else if(TravelDist < -1*(MAX_TRAVEL_DISTANCE) && !entity->IsFacingRight())
        {
            IsActive = false;
            CommitMovementRight = false;
            position.x = origin.x;
            position.y = origin.y;
            TravelDist = 0;
        }
    }

    void UpdateTexture()
    {
        if(AnimationStage < 10)
        {
            texture = game.AssignTexture(TEX_KADAL, 10);
            AnimationStage += 1/game.ScaleFPS();
        }
        else if(AnimationStage < 20)
        {
            texture = game.AssignTexture(TEX_KADAL, 11);
            AnimationStage += 1/game.ScaleFPS();
        }
        else if(AnimationStage < 30)
        {
            texture = game.AssignTexture(TEX_KADAL, 12);
            AnimationStage += 1/game.ScaleFPS();
        }
        else
        {
            AnimationStage = 0;
        }
        
    }

    void ShotTrigger(bool trigger)
    {
        if(trigger)
        {
            IsActive = true;
            entity->ModShotState(false);
            if(entity->IsFacingRight())
            {
                CommitMovementRight = true;
            }
        }
    }

    void DetectCollisionEntity(Entity& target)
    {
        if(IsActive)
        {
            if(CheckCollisionRecs(Hitbox, target.GetHitbox()) && target.GetLiveStatus())
            {
                IsActive = false;
                TravelDist = 0;
                DealDamage(target);
            }
        }
    }
    void DetectCollisionWall(Wall& wall)
    {
        if(IsActive)
        {
            if(CheckCollisionRecs(Hitbox, wall.GetHitbox()))
            {
                IsActive = false;
                TravelDist = 0;
            }
        }
    }

    void DealDamage(Entity& target)
    {
        target.TakeDamage(this->entity->GetAtk());
    }

    void Draw() override
    {
        if(IsActive)
        {
            DrawTexturePro(texture, TextureFrame, DestFrame, {DestFrame.width/2.0f, DestFrame.height/2.0f}, 0, WHITE);
        }
    }
};

//Protect box
class ProtectBox
{
    private:
    Rectangle Hitbox;
    Vector2 position;

    Texture2D texture;
    Rectangle TextureFrame;
    Rectangle DestFrame;

    public:
    ProtectBox(Vector2 position)
    {
        this->position = position;
        this->TextureFrame = {12, 7, 99, 97};
        this->DestFrame = {position.x, position.y, TextureFrame.width*1.8f, TextureFrame.height*1.8f};
        this->Hitbox = {DestFrame.x-DestFrame.width/2.0f, DestFrame.y-DestFrame.height/2.0f, DestFrame.width, DestFrame.height};
        this->texture = game.AssignTexture(TEX_STATUE);
    }

    void Draw()
    {
        DrawTexturePro(texture, TextureFrame, DestFrame, {DestFrame.width/2.0f, DestFrame.height/2.0f}, 0, WHITE);
    }

    void DetectEnemy(Entity& enemy)
    {
        if(CheckCollisionRecs({position.x-(DestFrame.width-100)/2.0f, position.y-(DestFrame.height-80)/2.0f, DestFrame.width-100, DestFrame.height-80}, enemy.GetHitbox()))
        {
            game.ModGameOver(true);
        }
    }

    Vector2 GetPos()
    {
        return position;
    }
    Rectangle GetBox()
    {
        return Hitbox;
    }
};


//Game related functions (nanti dibuat jadi class)
class Debug
{
    private:
    Player *player;
    EnemyMeleeBasic *enemy;
    EnemyMeleeBasic *enemy1;
    EnemyRangedBasic *ranged;
    vector<Bullet> bullet;
    vector<BulletEnemy> EnemyBullet;
    vector<EnemyMeleeBasic> test;

    Tile *tile;

    Wall *wall;

    public:
    Debug()
    {
        game.StartTimer();
        
        this->player = new Player({200, 400});
        this->enemy = new EnemyMeleeBasic({800, 200});
        this->enemy1 = new EnemyMeleeBasic({800, 400});
        this->ranged = new EnemyRangedBasic({800, 700});
        this->wall = new Wall({600, 200}, {300, 300}, BOTTOM, GREEN);

        this->tile = new Tile({1000, 200}, 95, 95);

        for(int i = 0; i < player->MAX_BULLET; i++)
        {
            bullet.push_back(Bullet(5, player->GetCenterPoint(), *player));
        }
        for(int i = 0; i < 100; i++)
        {
            EnemyBullet.push_back(BulletEnemy(5, ranged->GetCenterPoint(), *ranged));
        }
        for(int i = 0; i < 3; i++)
        {
            test.push_back(EnemyMeleeBasic({600, 200}));
            test.push_back(EnemyMeleeBasic({800, 400}));
            test.push_back(EnemyMeleeBasic({900, 600}));
        }
    }
    ~Debug()
    {
        delete player;
        delete ranged;
        delete enemy;
        bullet.clear();
    }
    void Draw()
    {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawFPS(0, 0);

        player->Draw();
        player->DrawUI();
        
        wall->Draw();

        tile->Draw();

        enemy->Draw();
        enemy1->Draw();

        ranged->Draw();

        for(int i = 0; i < player->MAX_BULLET; i++)
        {
            bullet[i].Draw();
        }
        for(int i = 0; i < 3; i++)
        {
            test[i].Draw();
        }
        for(int i = 0; i < 100; i++)
        {
            EnemyBullet[i].Draw();
        }

        DrawText(TextFormat("%.2f/%.2f", player->GetHealth(), player->GetMaxHealth()), 600, 0, 20, RED);
        DrawText(TextFormat("%.2f", game.GetTimer()), 400, 0, 20, BLACK);

        EndDrawing();
    }

    void Update()
    {
        if(!game.IsGameOver())
        {
            player->Update();
            for(int i = 0; i < player->MAX_BULLET; i++)
            {
                bullet[i].Update();
            }
            player->MeleeHit(*enemy);

            enemy->Update();
            //enemy->MoveLeft();
            enemy->DetectMeleeRange(*player);
            enemy->MeleeHit(*player);

            enemy1->Update();
            enemy1->DetectMeleeRange(*player);
            enemy1->MeleeHit(*player);

            ranged->MoveRight();
            ranged->Update();
            for(int i = 0; i < 100; i++)
            {
                EnemyBullet[i].Update();
            }
            for(int i = 0; i < 100; i++)
            {
                EnemyBullet[i].DetectCollisionEntity(*player);
            }
            for(int i = 0; i < 3; i++)
            {
                test[i].Update();
                test[i].MoveLeft();
            }


            //Detect collision
            for(int i = 0; i < player->MAX_BULLET; i++)
            {
                bullet[i].DetectCollisionEntity(*enemy);
            }

            wall->Update(*player);

            //manual exit
            if(IsKeyPressed(KEY_ESCAPE))
            {
                game.ModExit(true);
            }
        }   
    }
};

class MainMenu
{
    private:
    ButtonDebug *DebugButton;
    ButtonPlay *PlayButton;
    ButtonExit *ExitButton;
    ButtonHelp *HelpButton;

    Texture2D BGTexture = game.AssignTexture(TEX_MAIN_MENU_BG);
    Texture2D TitleText = game.AssignTexture(TEX_GUI_TEXT);

    public:
    MainMenu()
    {
        DebugButton = new ButtonDebug({game.GetWindowWidth()/2.0f, 750}, BLACK);
        PlayButton = new ButtonPlay({game.GetWindowWidth()/2.0f, 550}, BLACK);
        ExitButton = new ButtonExit({game.GetWindowWidth()/2.0f, 850}, BLACK);
        HelpButton = new ButtonHelp({game.GetWindowWidth()/2.0f, 650}, BLACK);
        cout<<"Main Menu Created"<<endl;
    }
    ~MainMenu()
    {
        delete DebugButton;
        delete PlayButton;
        delete ExitButton;
        delete HelpButton;
        cout<<"Main Manu Destroyed"<<endl;
    }
    void Update()
    {
        DebugButton->Update();
        PlayButton->Update();
        ExitButton->Update();
        HelpButton->Update();
    }
    void Draw()
    {
        BeginDrawing();

        ClearBackground(WHITE);
        DrawTexturePro(BGTexture, {0, 0, 768, 512}, {0, 0, 1920, 1080}, {0,0}, 0 ,WHITE);
        DrawTexturePro(TitleText, {75, 636, 871, 235}, {game.GetWindowWidth()/2.0f+125, 350, 871*0.7, 512*0.7}, {871/2.0f, 512/2.0f}, 0, WHITE);
        DebugButton->Draw();
        PlayButton->Draw();
        ExitButton->Draw();
        HelpButton->Draw();
        EndDrawing();
    }
};

class HelpMenu
{
    private:
    ButtonBack *button_back;
    Texture2D texture = game.AssignTexture(TEX_HELP_MENU_BG);

    public:
    HelpMenu()
    {
        button_back = new ButtonBack({150, 75}, BLACK);
        cout<<"Help menu created"<<endl;
    }
    ~HelpMenu()
    {
        delete button_back;
    }

    void Update()
    {
        button_back->Update();
        if(IsKeyPressed(KEY_ESCAPE))
        {
            game.ModExit(true);
        }
    }
    void Draw()
    {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawTexturePro(texture, {0, 0, 1920, 1080}, {0, 0, 1920, 1080}, {0,0}, 0, WHITE);
        button_back->Draw();
        EndDrawing();
    }
};

class StageSelectMenu
{
    private:
    vector<ButtonStageSelect> Button_StageSelect;
    ButtonBack *Button_Back;
    int TOTAL_STAGE = 5;
    Texture2D BGTexture = game.AssignTexture(TEX_STAGE_SELECT_BG);
    public:
    StageSelectMenu()
    {
        for(int i = 0; i < 2; i++)
        {
            Button_StageSelect.push_back(ButtonStageSelect({game.GetWindowWidth()/2.0f, float(250+150*i)}, BLACK, i+1));
        }
        Button_Back = new ButtonBack({150, 100}, BLACK);
        cout<<"Stage Select Menu Created"<<endl;
    }
    ~StageSelectMenu()
    {
        Button_StageSelect.clear();
        delete Button_Back;
        cout<<"Stage Select Menu Destroyed"<<endl;
    }

    void Update()
    {
        for(int i = 0; i < 2; i++)
        {
            Button_StageSelect[i].Update();
        }
        Button_Back->Update();
        if(IsKeyPressed(KEY_ESCAPE))
        {
            game.ModExit(true);
        }
    }

    void Draw()
    {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawTexturePro(BGTexture, {0, 0, 1920, 1080}, {0,0,1920,1080}, {0,0}, 0, WHITE);
        for(int i = 0; i < 2; i++)
        {
            Button_StageSelect[i].Draw();
        }
        Button_Back->Draw();
        EndDrawing();
    }
};

class StageOne
{
    private:
    const int GOBLIN_MID_COUNT = 10;
    const int GOBLIN_TOP_COUNT = 5;
    const int GOBLIN_BOTTOM_COUNT = 7;

    int wave = 1;
    bool SpawnWave = true;
    bool WaveCleared = false;
    int WaveKillCounter = 0;

    vector<int> dead_goblin_mid;
    vector<int> dead_goblin_top;
    vector<int> dead_goblin_bottom;

    Player *player;
    vector<Bullet> PlayerBullet;
    vector<EnemyMeleeBasic> goblin_top;
    vector<EnemyMeleeBasic> goblin_mid;
    vector<EnemyMeleeBasic> goblin_bottom;
    ProtectBox *protect_box;
    vector<Wall> wall;
    vector<Tile> tile;

    GameOverPanel *Panel_GameOver;
    PausePanel *Panel_Pause;
    LevelClearPanel *Panel_Cleared;

    public:
    StageOne()
    {
        game.ModStageCleared(false);
        game.StartTimer();
        SetRandomSeed(time(0));
        //Player
        player = new Player({280, 500});
        for(int i = 0; i < player->MAX_BULLET; i++)
        {
            PlayerBullet.push_back(Bullet(15, player->GetCenterPoint(), *player));
        }
        
        //Protect box
        protect_box = new ProtectBox({100, 500});

        //Enemy
        for(int i = 0; i < GOBLIN_MID_COUNT ;i++)
        {
            goblin_mid.push_back(EnemyMeleeBasic({2000, 2000}));
        }
        for(int i = 0; i < GOBLIN_TOP_COUNT ;i++)
        {
            goblin_top.push_back(EnemyMeleeBasic({2000, 2000}));
        }
        for(int i = 0; i < GOBLIN_BOTTOM_COUNT ;i++)
        {
            goblin_bottom.push_back(EnemyMeleeBasic({2000, 2000}));
        }
        //Initial dead count
        for(int i = 0; i < GOBLIN_MID_COUNT ;i++)
        {
            dead_goblin_mid.push_back(-1);
        }
        for(int i = 0; i < GOBLIN_TOP_COUNT ;i++)
        {
            dead_goblin_top.push_back(-1);
        }
        for(int i = 0; i < GOBLIN_BOTTOM_COUNT ;i++)
        {
            dead_goblin_bottom.push_back(-1);
        }
        
        //Wall
        wall.push_back(Wall({240, 170},{480, 340}, BOTTOM, GRAY));
        wall.push_back(Wall({648+(1298/2.0f), 170}, {1298, 340}, BOTTOM, GRAY));
        wall.push_back(Wall({1114/2.0f, 755+(1080-755)/2.0f},{1114, 1080-755}, TOP, GRAY));
        wall.push_back(Wall({1362+(1920-1362)/2.0f, 775+(1080-775)/2.0f},{1920-1362, 1080-775}, TOP, GRAY));

        //Tile
        for(int i = 0; i < 33; i++)
        {
            for(int j = 0; j < 19; j++)
            {
                tile.push_back(Tile({float(60*i), float(60*j)}, float(60), float(60)));
            }
        }

        //Game Over Panel
        Panel_GameOver = new GameOverPanel();
        //Pause Panel
        Panel_Pause = new PausePanel();
        //Stage Cleared panel
        Panel_Cleared = new LevelClearPanel();

        cout<<"Stage One Created"<<endl;
    }
    ~StageOne()
    {
        delete player;
        delete protect_box;
        delete Panel_GameOver;
        delete Panel_Pause;
        delete Panel_Cleared;
        wall.clear();
        PlayerBullet.clear();
        tile.clear();

        dead_goblin_bottom.clear();
        dead_goblin_mid.clear();
        dead_goblin_top.clear();
        goblin_bottom.clear();
        goblin_mid.clear();
        goblin_top.clear();

        game.ModGameOver(false);
        game.ModPause(false);
        cout<<"Stage One Destroyed"<<endl;
    }
    void Update()
    {
        if(!game.IsGameOver() && !game.IsPaused())
        {
            //General Update
            //Player
            player->Update();

            for(int i = 0; i < player->MAX_BULLET; i++)
            {
                PlayerBullet[i].Update();
            }

            for(int i = 0; i < 4; i++)
            {
                wall[i].Update(*player);
            }

            //Enemy Update
            switch(wave)
            {
            //Wave 1
            case 1:
                if(SpawnWave)
                {
                    SpawnWave = false;
                    for(int i = 0; i < 4; i++)
                    {
                        goblin_mid[i].SetPos({float(1930+GetRandomValue(1, 2000)), float(GetRandomValue(350, 700))});
                    }
                    for(int i = 0; i < 3; i++)
                    {
                        goblin_bottom[i].SetPos({float(GetRandomValue(1167, 1362)), float(1100+GetRandomValue(1, 1000))});
                    }
                }
                //GOBLIN MID
                for(int i = 0; i < 4; i++)
                {
                    if(goblin_mid[i].GetPos().x <= protect_box->GetPos().x+200)
                    {
                        if(goblin_mid[i].GetPos().y < protect_box->GetPos().y-20)
                        {
                            goblin_mid[i].MoveDown();
                        }
                        else if(goblin_mid[i].GetPos().y > protect_box->GetPos().y+20)
                        {
                            goblin_mid[i].MoveUp();
                        }
                    }
                    goblin_mid[i].MoveLeft();
                    goblin_mid[i].Update();
                    if(!goblin_mid[i].GetLiveStatus())
                    {
                        if(dead_goblin_mid[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_goblin_mid[i] = i;
                        }
                    }
                }
                //GOBLIN BOTOM
                for(int i = 0; i < 3; i++)
                {
                    if(goblin_bottom[i].GetPos().y >= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)) && goblin_bottom[i].GetPos().x > 1167)
                    {
                        goblin_bottom[i].MoveUp();
                        if(goblin_bottom[i].GetPos().y <= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)))
                        {
                            goblin_bottom[i].MoveLeft();
                        }
                    }
                    else
                    {
                        goblin_bottom[i].MoveLeft();
                    }
                    goblin_bottom[i].Update();
                    if(!goblin_bottom[i].GetLiveStatus())
                    {
                        if(dead_goblin_bottom[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_goblin_bottom[i] = i;
                        }
                    }
                }
                if(WaveKillCounter == 7)
                {
                    wave++;
                    WaveKillCounter = 0;
                    SpawnWave = true;
                }
                break;
            
            case 2:
                if(SpawnWave)
                {
                    SpawnWave = false;
                    for(int i = 4; i < 7; i++)
                    {
                        goblin_mid[i].SetPos({float(1930+GetRandomValue(1, 2000)), float(GetRandomValue(350, 700))});
                    }
                    for(int i = 3; i < 5; i++)
                    {
                        goblin_bottom[i].SetPos({float(GetRandomValue(1167, 1362)), float(1100+GetRandomValue(1, 1000))});
                    }
                    for(int i = 0; i < 2; i++)
                    {
                        goblin_top[i].SetPos({float(GetRandomValue(505, 630)), float(GetRandomValue(-10, -1000))});
                    }
                }
                //GOBLIN MID                
                for(int i = 4; i < 7; i++)
                {
                    if(goblin_mid[i].GetPos().x <= protect_box->GetPos().x+200)
                    {
                        if(goblin_mid[i].GetPos().y < protect_box->GetPos().y-20)
                        {
                            goblin_mid[i].MoveDown();
                        }
                        else if(goblin_mid[i].GetPos().y > protect_box->GetPos().y+20)
                        {
                            goblin_mid[i].MoveUp();
                        }
                    }
                    goblin_mid[i].MoveLeft();
                    goblin_mid[i].Update();
                    if(!goblin_mid[i].GetLiveStatus())
                    {
                        if(dead_goblin_mid[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_goblin_mid[i] = i;
                        }
                    }
                }
                //GOBLIN BOTOM
                for(int i = 3; i < 5; i++)
                {
                    if(goblin_bottom[i].GetPos().y >= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)) && goblin_bottom[i].GetPos().x > 1167)
                    {
                        goblin_bottom[i].MoveUp();
                        if(goblin_bottom[i].GetPos().y <= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)))
                        {
                            goblin_bottom[i].MoveLeft();
                        }
                    }
                    else
                    {
                        goblin_bottom[i].MoveLeft();
                    }
                    goblin_bottom[i].Update();
                    if(!goblin_bottom[i].GetLiveStatus())
                    {
                        if(dead_goblin_bottom[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_goblin_bottom[i] = i;
                        }
                    }
                }
                ///GOBLIN TOP
                for(int i = 0; i < 2; i++)
                {
                    if(goblin_top[i].GetPos().y <= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)) && goblin_top[i].GetPos().x > 300)
                    {
                        goblin_top[i].MoveDown();
                        if(goblin_top[i].GetPos().y >= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)))
                        {
                            goblin_top[i].MoveLeft();
                        }
                    }
                    else
                    {
                        goblin_top[i].MoveLeft();
                    }
                    goblin_top[i].Update();
                    if(!goblin_top[i].GetLiveStatus())
                    {
                        if(dead_goblin_top[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_goblin_top[i] = i;
                        }
                    }
                }
                if(WaveKillCounter == 7)
                {
                    WaveKillCounter = 0;
                    wave++;
                    SpawnWave = true;
                }
                break;
            case 3:
                if(SpawnWave)
                {
                    //10 7 5 total wave ini: 8
                    SpawnWave = false;
                    for(int i = 7; i < 10; i++)
                    {
                        goblin_mid[i].SetPos({float(1930+GetRandomValue(1, 2000)), float(GetRandomValue(350, 700))});
                    }
                    for(int i = 5; i < 7; i++)
                    {
                        goblin_bottom[i].SetPos({float(GetRandomValue(1167, 1362)), float(1100+GetRandomValue(1, 1000))});
                    }
                    for(int i = 2; i < 5; i++)
                    {
                        goblin_top[i].SetPos({float(GetRandomValue(505, 630)), float(GetRandomValue(-10, -1000))});
                    }

                    //buat solve glitch/bug player direction
                    //CARI TAU LAGI PENYEBABNYA KENAPA, INI FIX SEMENTARA
                    /*for(int i = 5; i < 7; i++)
                    {
                        goblin_bottom[i].SetPos({float(GetRandomValue(1167, 1362)), float(1100+GetRandomValue(1, 1000))});
                    }*/
                }
                //GOBLIN MID
                for(int i = 7; i < 10; i++)
                {
                    if(goblin_mid[i].GetPos().x <= protect_box->GetPos().x+200)
                    {
                        if(goblin_mid[i].GetPos().y < protect_box->GetPos().y-20)
                        {
                            goblin_mid[i].MoveDown();
                        }
                        else if(goblin_mid[i].GetPos().y > protect_box->GetPos().y+20)
                        {
                            goblin_mid[i].MoveUp();
                        }
                    }
                    goblin_mid[i].MoveLeft();
                    goblin_mid[i].Update();
                    if(!goblin_mid[i].GetLiveStatus())
                    {
                        if(dead_goblin_mid[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_goblin_mid[i] = i;
                        }
                    }
                }
                //GOBLIN BOTTOM
                for(int i = 5; i < 7; i++)
                {
                    if(goblin_bottom[i].GetPos().y >= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)) && goblin_bottom[i].GetPos().x > 1167)
                    {
                        goblin_bottom[i].MoveUp();
                        if(goblin_bottom[i].GetPos().y <= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)))
                        {
                            goblin_bottom[i].MoveLeft();
                        }
                    }
                    else
                    {
                        goblin_bottom[i].MoveLeft();
                    }
                    goblin_bottom[i].Update();
                    if(!goblin_bottom[i].GetLiveStatus())
                    {
                        if(dead_goblin_bottom[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_goblin_bottom[i] = i;
                        }
                    }
                }
                //GOBLIN TOP
                for(int i = 2; i < 5; i++)
                {
                    if(goblin_top[i].GetPos().y <= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)) && goblin_top[i].GetPos().x > 300)
                    {
                        goblin_top[i].MoveDown();
                        if(goblin_top[i].GetPos().y >= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)))
                        {
                            goblin_top[i].MoveLeft();
                        }
                    }
                    else
                    {
                        goblin_top[i].MoveLeft();
                    }
                    goblin_top[i].Update();
                    if(!goblin_top[i].GetLiveStatus())
                    {
                        if(dead_goblin_top[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_goblin_top[i] = i;
                        }
                    }
                }
                if(WaveKillCounter == 8)
                {
                    wave++;
                    WaveKillCounter = 0;
                    SpawnWave = true;
                }
                break;
            case 4:
                game.ModStageCleared(true);
            default:
                break;
            }

            //Protectbox update
            for(int i = 0; i < GOBLIN_MID_COUNT; i++)
            {
                protect_box->DetectEnemy(goblin_mid[i]);
            }
            for(int i = 0; i < GOBLIN_BOTTOM_COUNT; i++)
            {
                protect_box->DetectEnemy(goblin_bottom[i]);
            }
            for(int i = 0; i < GOBLIN_TOP_COUNT; i++)
            {
                protect_box->DetectEnemy(goblin_top[i]);
            }
            //Player death game over
            if(!player->GetLiveStatus())
            {
                game.ModGameOver(true);
            }

            //Action Update
            //Player Melee
            for(int i = 0; i < GOBLIN_MID_COUNT; i++)
            {
                player->MeleeHit(goblin_mid[i]);
            }
            for(int i = 0; i < GOBLIN_BOTTOM_COUNT; i++)
            {
                player->MeleeHit(goblin_bottom[i]);
            }
            for(int i = 0; i < GOBLIN_TOP_COUNT; i++)
            {
                player->MeleeHit(goblin_top[i]);
            }
            //Enemy Melee
            for(int i = 0; i < GOBLIN_MID_COUNT; i++)
            {
                goblin_mid[i].DetectMeleeRange(*player);
                goblin_mid[i].MeleeHit(*player);
            }
            for(int i = 0; i < GOBLIN_BOTTOM_COUNT; i++)
            {
                goblin_bottom[i].DetectMeleeRange(*player);
                goblin_bottom[i].MeleeHit(*player);
            }
            for(int i = 0; i < GOBLIN_TOP_COUNT; i++)

            {
                goblin_top[i].DetectMeleeRange(*player);
                goblin_top[i].MeleeHit(*player);
            }

            //Collision Update
            for(int i = 0; i < player->MAX_BULLET; i++)
            {
                //PlayerBullet[i].DetectCollisionEntity(goblin_top[0]);
                for(int j = 0; j < 4; j++)
                {
                    PlayerBullet[i].DetectCollisionWall(wall[j]);
                }
                
                for(int j = 0; j < GOBLIN_MID_COUNT; j++)
                {
                    PlayerBullet[i].DetectCollisionEntity(goblin_mid[j]);
                }
                for(int j = 0; j < GOBLIN_BOTTOM_COUNT; j++)
                {
                    PlayerBullet[i].DetectCollisionEntity(goblin_bottom[j]);
                }
                for(int j = 0; j < GOBLIN_TOP_COUNT; j++)
                {
                    PlayerBullet[i].DetectCollisionEntity(goblin_top[j]);
                }
            }
        }
        //Game over panel update
        Panel_GameOver->Update();
        //Pause panel update
        Panel_Pause->Update();
        //Level Cleared panel
        Panel_Cleared->Update();
        //Pause
        if(IsKeyPressed(KEY_P))
        {
            game.ModPause(true);
        }
        //Manual exit
        if(IsKeyPressed(KEY_ESCAPE))
        {
            game.ModExit(true);
        }
    }
    void Draw()
    {
        BeginDrawing();

        ClearBackground(WHITE);
        for(int i = 0; i < 627; i++)
        {
            tile[i].Draw();
        }
        for(int i = 0; i < 4; i++)
        {
            wall[i].Draw();
        }
        protect_box->Draw();
        player->Draw();
        for(int i = 0; i < player->MAX_BULLET; i++)
        {
            PlayerBullet[i].Draw();
        }

        player->DrawUI();
        //DrawText(TextFormat("%.2f, %.2f", player->GetPos().x, player->GetPos().y), 500, 0, 50, BLACK);
        DrawText(TextFormat("%d", wave), 900, 0, 20, BLACK);
        //DrawText(TextFormat("%d", WaveKillCounter), 950, 0, 20, BLACK);

        switch(wave)
        {
        case 1:
            for(int i = 0; i < 4; i++)
            {
                goblin_mid[i].Draw();
            }
            for(int i = 0; i < 3; i++)
            {
                goblin_bottom[i].Draw();
            }
            break;
         case 2:
            for(int i = 4; i < 7; i++)
            {
                goblin_mid[i].Draw();
            }
            for(int i = 3; i < 5; i++)
            {
                goblin_bottom[i].Draw();
            }
            for(int i = 0; i < 2; i++)
            {
                goblin_top[i].Draw();
            }
            break;
        case 3:
            for(int i = 7; i < 10; i++)
            {
                goblin_mid[i].Draw();
            }
            for(int i = 5; i < 7; i++)
            {
                goblin_bottom[i].Draw();
            }
            for(int i = 2; i < 5; i++)
            {
                goblin_top[i].Draw();
            }
            break;
        default:
            break;
        }
        Panel_GameOver->Draw();
        Panel_Pause->Draw();
        Panel_Cleared->Draw();

        EndDrawing();
    }
};

class StageTwo
{
    private:
    const int GOBLIN_MID_COUNT = 5;
    const int GOBLIN_TOP_COUNT = 3;
    const int GOBLIN_BOTTOM_COUNT = 4;

    const int KADAL_MID_COUNT = 5;
    const int KADAL_TOP_COUNT = 3;
    const int KADAL_BOTTOM_COUNT = 4;


    int wave = 1;
    bool SpawnWave = true;
    bool WaveCleared = false;
    int WaveKillCounter = 0;

    vector<int> dead_goblin_mid;
    vector<int> dead_goblin_top;
    vector<int> dead_goblin_bottom;

    vector<int> dead_kadal_mid;
    vector<int> dead_kadal_top;
    vector<int> dead_kadal_bottom;

    Player *player;
    vector<Bullet> PlayerBullet;
    vector<vector<BulletEnemy>> EnemyBullet;
    vector<EnemyMeleeBasic> goblin_top;
    vector<EnemyMeleeBasic> goblin_mid;
    vector<EnemyMeleeBasic> goblin_bottom;
    vector<EnemyRangedBasic> kadal_top;
    vector<EnemyRangedBasic> kadal_mid;
    vector<EnemyRangedBasic> kadal_bottom;
    ProtectBox *protect_box;
    vector<Wall> wall;
    vector<Tile> tile;

    GameOverPanel *Panel_GameOver;
    PausePanel *Panel_Pause;
    LevelClearPanel *Panel_Cleared;

    public:
    StageTwo()
    {
        game.ModStageCleared(false);
        game.StartTimer();
        SetRandomSeed(time(0));
        //Player
        player = new Player({280, 500});
        for(int i = 0; i < player->MAX_BULLET; i++)
        {
            PlayerBullet.push_back(Bullet(15, player->GetCenterPoint(), *player));
        }
        
        //Protect box
        protect_box = new ProtectBox({100, 500});

        //Enemy
        for(int i = 0; i < GOBLIN_MID_COUNT ;i++)
        {
            goblin_mid.push_back(EnemyMeleeBasic({2000, 2000}));
        }
        for(int i = 0; i < GOBLIN_TOP_COUNT ;i++)
        {
            goblin_top.push_back(EnemyMeleeBasic({2000, 2000}));
        }
        for(int i = 0; i < GOBLIN_BOTTOM_COUNT ;i++)
        {
            goblin_bottom.push_back(EnemyMeleeBasic({2000, 2000}));
        }
        for(int i = 0; i < KADAL_MID_COUNT ;i++)
        {
            kadal_mid.push_back(EnemyRangedBasic({2000, 2000}));
        }
        for(int i = 0; i < KADAL_TOP_COUNT ;i++)
        {
            kadal_top.push_back(EnemyRangedBasic({2000, 2000}));
        }
        for(int i = 0; i < KADAL_BOTTOM_COUNT ;i++)
        {
            kadal_bottom.push_back(EnemyRangedBasic({2000, 2000}));
        }

        /*for(int i = 0; i < 12; i++)
        {
            vector<BulletEnemy> v1;
            for(int j = 0; j < 50; j++)
            {
                v1.push_back(BulletEnemy(15, player->GetCenterPoint(), *kadal_));
            }
            EnemyBullet.push_back(v1);
        }*/
        //Initial dead count
        for(int i = 0; i < GOBLIN_MID_COUNT ;i++)
        {
            dead_goblin_mid.push_back(-1);
        }
        for(int i = 0; i < GOBLIN_TOP_COUNT ;i++)
        {
            dead_goblin_top.push_back(-1);
        }
        for(int i = 0; i < GOBLIN_BOTTOM_COUNT ;i++)
        {
            dead_goblin_bottom.push_back(-1);
        }
        for(int i = 0; i < KADAL_MID_COUNT ;i++)
        {
            dead_kadal_mid.push_back(-1);
        }
        for(int i = 0; i < KADAL_TOP_COUNT ;i++)
        {
            dead_kadal_top.push_back(-1);
        }
        for(int i = 0; i < KADAL_BOTTOM_COUNT ;i++)
        {
            dead_kadal_bottom.push_back(-1);
        }
        
        //Wall
        wall.push_back(Wall({240, 170},{480, 340}, BOTTOM, GRAY));
        wall.push_back(Wall({648+(1298/2.0f), 170}, {1298, 340}, BOTTOM, GRAY));
        wall.push_back(Wall({1114/2.0f, 755+(1080-755)/2.0f},{1114, 1080-755}, TOP, GRAY));
        wall.push_back(Wall({1362+(1920-1362)/2.0f, 775+(1080-775)/2.0f},{1920-1362, 1080-775}, TOP, GRAY));

        //Tile
        for(int i = 0; i < 33; i++)
        {
            for(int j = 0; j < 19; j++)
            {
                tile.push_back(Tile({float(60*i), float(60*j)}, float(60), float(60)));
            }
        }

        //Game Over Panel
        Panel_GameOver = new GameOverPanel();
        //Pause Panel
        Panel_Pause = new PausePanel();
        //Stage Cleared panel
        Panel_Cleared = new LevelClearPanel();

        cout<<"Stage Two Created"<<endl;
    }
    ~StageTwo()
    {
        delete player;
        delete protect_box;
        delete Panel_GameOver;
        delete Panel_Pause;
        delete Panel_Cleared;
        wall.clear();
        PlayerBullet.clear();
        tile.clear();

        dead_goblin_bottom.clear();
        dead_goblin_mid.clear();
        dead_goblin_top.clear();
        goblin_bottom.clear();
        goblin_mid.clear();
        goblin_top.clear();

        dead_kadal_bottom.clear();
        dead_kadal_mid.clear();
        dead_kadal_top.clear();
        kadal_bottom.clear();
        kadal_mid.clear();
        kadal_top.clear();

        game.ModGameOver(false);
        game.ModPause(false);
        cout<<"Stage One Destroyed"<<endl;
    }
    void Update()
    {
        if(!game.IsGameOver() && !game.IsPaused())
        {
            //General Update
            //Player
            player->Update();

            for(int i = 0; i < player->MAX_BULLET; i++)
            {
                PlayerBullet[i].Update();
            }

            for(int i = 0; i < 4; i++)
            {
                wall[i].Update(*player);
            }

            //Enemy Update
            switch(wave)
            {
            //Wave 1
            case 1:
                if(SpawnWave)
                {
                    SpawnWave = false;
                    for(int i = 0; i < 2; i++)
                    {
                        goblin_mid[i].SetPos({float(1930+GetRandomValue(1, 2000)), float(GetRandomValue(350, 700))});
                    }
                    for(int i = 0; i < 2; i++)
                    {
                        kadal_mid[i].SetPos({float(1930+GetRandomValue(1, 2000)), float(GetRandomValue(350, 700))});
                    }
                    for(int i = 0; i < 2; i++)
                    {
                        goblin_bottom[i].SetPos({float(GetRandomValue(1167, 1362)), float(1100+GetRandomValue(1, 1000))});
                    }
                    for(int i = 0; i < 2; i++)
                    {
                        kadal_bottom[i].SetPos({float(GetRandomValue(1167, 1362)), float(1100+GetRandomValue(1, 1000))});
                    }
                }
                //GOBLIN MID
                for(int i = 0; i < 2; i++)
                {
                    if(goblin_mid[i].GetPos().x <= protect_box->GetPos().x+200)
                    {
                        if(goblin_mid[i].GetPos().y < protect_box->GetPos().y-20)
                        {
                            goblin_mid[i].MoveDown();
                        }
                        else if(goblin_mid[i].GetPos().y > protect_box->GetPos().y+20)
                        {
                            goblin_mid[i].MoveUp();
                        }
                    }
                    goblin_mid[i].MoveLeft();
                    goblin_mid[i].Update();
                    if(!goblin_mid[i].GetLiveStatus())
                    {
                        if(dead_goblin_mid[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_goblin_mid[i] = i;
                        }
                    }
                }
                //KADAL MID
                for(int i = 0; i < 2; i++)
                {
                    if(kadal_mid[i].GetPos().x <= protect_box->GetPos().x+200)
                    {
                        if(kadal_mid[i].GetPos().y < protect_box->GetPos().y-20)
                        {
                            kadal_mid[i].MoveDown();
                        }
                        else if(kadal_mid[i].GetPos().y > protect_box->GetPos().y+20)
                        {
                            kadal_mid[i].MoveUp();
                        }
                    }
                    kadal_mid[i].MoveLeft();
                    kadal_mid[i].Update();
                    if(!kadal_mid[i].GetLiveStatus())
                    {
                        if(dead_kadal_mid[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_kadal_mid[i] = i;
                        }
                    }
                }
                //GOBLIN BOTOM
                for(int i = 0; i < 1; i++)
                {
                    if(goblin_bottom[i].GetPos().y >= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)) && goblin_bottom[i].GetPos().x > 1167)
                    {
                        goblin_bottom[i].MoveUp();
                        if(goblin_bottom[i].GetPos().y <= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)))
                        {
                            goblin_bottom[i].MoveLeft();
                        }
                    }
                    else
                    {
                        goblin_bottom[i].MoveLeft();
                    }
                    goblin_bottom[i].Update();
                    if(!goblin_bottom[i].GetLiveStatus())
                    {
                        if(dead_goblin_bottom[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_goblin_bottom[i] = i;
                        }
                    }
                }
                //KADAL BOTTOM
                for(int i = 0; i < 2; i++)
                {
                    if(kadal_bottom[i].GetPos().y >= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)) && kadal_bottom[i].GetPos().x > 1167)
                    {
                        kadal_bottom[i].MoveUp();
                        if(kadal_bottom[i].GetPos().y <= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)))
                        {
                            kadal_bottom[i].MoveLeft();
                        }
                    }
                    else
                    {
                        kadal_bottom[i].MoveLeft();
                    }
                    kadal_bottom[i].Update();
                    if(!kadal_bottom[i].GetLiveStatus())
                    {
                        if(dead_kadal_bottom[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_kadal_bottom[i] = i;
                        }
                    }
                }
                if(WaveKillCounter == 8)
                {
                    wave++;
                    WaveKillCounter = 0;
                    SpawnWave = true;
                }
                break;
            
            case 2:
                if(SpawnWave)
                {
                    SpawnWave = false;
                    for(int i = 2; i < 3; i++)
                    {
                        goblin_mid[i].SetPos({float(1930+GetRandomValue(1, 2000)), float(GetRandomValue(350, 700))});
                    }
                    for(int i = 2; i < 3; i++)
                    {
                        kadal_mid[i].SetPos({float(1930+GetRandomValue(1, 2000)), float(GetRandomValue(350, 700))});
                    }
                    for(int i = 2; i < 3; i++)
                    {
                        goblin_bottom[i].SetPos({float(GetRandomValue(1167, 1362)), float(1100+GetRandomValue(1, 1000))});
                    }
                    for(int i = 2; i < 3; i++)
                    {
                        kadal_bottom[i].SetPos({float(GetRandomValue(1167, 1362)), float(1100+GetRandomValue(1, 1000))});
                    }
                    for(int i = 0; i < 2; i++)
                    {
                        goblin_top[i].SetPos({float(GetRandomValue(505, 630)), float(GetRandomValue(-10, -1000))});
                    }
                    for(int i = 0; i < 2; i++)
                    {
                        kadal_top[i].SetPos({float(GetRandomValue(505, 630)), float(GetRandomValue(-10, -1000))});
                    }
                }
                //GOBLIN MID                
                for(int i = 2; i < 3; i++)
                {
                    if(goblin_mid[i].GetPos().x <= protect_box->GetPos().x+200)
                    {
                        if(goblin_mid[i].GetPos().y < protect_box->GetPos().y-20)
                        {
                            goblin_mid[i].MoveDown();
                        }
                        else if(goblin_mid[i].GetPos().y > protect_box->GetPos().y+20)
                        {
                            goblin_mid[i].MoveUp();
                        }
                    }
                    goblin_mid[i].MoveLeft();
                    goblin_mid[i].Update();
                    if(!goblin_mid[i].GetLiveStatus())
                    {
                        if(dead_goblin_mid[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_goblin_mid[i] = i;
                        }
                    }
                }
                //KADAL MID
                for(int i = 2; i < 3; i++)
                {
                    if(kadal_mid[i].GetPos().x <= protect_box->GetPos().x+200)
                    {
                        if(kadal_mid[i].GetPos().y < protect_box->GetPos().y-20)
                        {
                            kadal_mid[i].MoveDown();
                        }
                        else if(kadal_mid[i].GetPos().y > protect_box->GetPos().y+20)
                        {
                            kadal_mid[i].MoveUp();
                        }
                    }
                    kadal_mid[i].MoveLeft();
                    kadal_mid[i].Update();
                    if(!kadal_mid[i].GetLiveStatus())
                    {
                        if(dead_kadal_mid[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_kadal_mid[i] = i;
                        }
                    }
                }
                //GOBLIN BOTOM
                for(int i = 2; i < 3; i++)
                {
                    if(goblin_bottom[i].GetPos().y >= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)) && goblin_bottom[i].GetPos().x > 1167)
                    {
                        goblin_bottom[i].MoveUp();
                        if(goblin_bottom[i].GetPos().y <= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)))
                        {
                            goblin_bottom[i].MoveLeft();
                        }
                    }
                    else
                    {
                        goblin_bottom[i].MoveLeft();
                    }
                    goblin_bottom[i].Update();
                    if(!goblin_bottom[i].GetLiveStatus())
                    {
                        if(dead_goblin_bottom[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_goblin_bottom[i] = i;
                        }
                    }
                }
                //KADAL BOTTOM
                for(int i = 2; i < 3; i++)
                {
                    if(kadal_bottom[i].GetPos().y >= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)) && kadal_bottom[i].GetPos().x > 1167)
                    {
                        kadal_bottom[i].MoveUp();
                        if(kadal_bottom[i].GetPos().y <= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)))
                        {
                            kadal_bottom[i].MoveLeft();
                        }
                    }
                    else
                    {
                        kadal_bottom[i].MoveLeft();
                    }
                    kadal_bottom[i].Update();
                    if(!kadal_bottom[i].GetLiveStatus())
                    {
                        if(dead_kadal_bottom[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_kadal_bottom[i] = i;
                        }
                    }
                }
                ///GOBLIN TOP
                for(int i = 0; i < 2; i++)
                {
                    if(goblin_top[i].GetPos().y <= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)) && goblin_top[i].GetPos().x > 300)
                    {
                        goblin_top[i].MoveDown();
                        if(goblin_top[i].GetPos().y >= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)))
                        {
                            goblin_top[i].MoveLeft();
                        }
                    }
                    else
                    {
                        goblin_top[i].MoveLeft();
                    }
                    goblin_top[i].Update();
                    if(!goblin_top[i].GetLiveStatus())
                    {
                        if(dead_goblin_top[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_goblin_top[i] = i;
                        }
                    }
                }
                //KADAL TOP
                for(int i = 0; i < 2; i++)
                {
                    if(kadal_top[i].GetPos().y <= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)) && kadal_top[i].GetPos().x > 300)
                    {
                        kadal_top[i].MoveDown();
                        if(kadal_top[i].GetPos().y >= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)))
                        {
                            kadal_top[i].MoveLeft();
                        }
                    }
                    else
                    {
                        kadal_top[i].MoveLeft();
                    }
                    kadal_top[i].Update();
                    if(!kadal_top[i].GetLiveStatus())
                    {
                        if(dead_kadal_top[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_kadal_top[i] = i;
                        }
                    }
                }
                if(WaveKillCounter == 8)
                {
                    WaveKillCounter = 0;
                    wave++;
                    SpawnWave = true;
                }
                break;
            case 3:
                if(SpawnWave)
                {
                    //10 7 5 total wave ini: 8
                    SpawnWave = false;
                    for(int i = 3; i < 5; i++)
                    {
                        goblin_mid[i].SetPos({float(1930+GetRandomValue(1, 2000)), float(GetRandomValue(350, 700))});
                    }
                    for(int i = 3; i < 5; i++)
                    {
                        kadal_mid[i].SetPos({float(1930+GetRandomValue(1, 2000)), float(GetRandomValue(350, 700))});
                    }
                    for(int i = 3; i < 4; i++)
                    {
                        goblin_bottom[i].SetPos({float(GetRandomValue(1167, 1362)), float(1100+GetRandomValue(1, 1000))});
                    }
                    for(int i = 3; i < 4; i++)
                    {
                        kadal_bottom[i].SetPos({float(GetRandomValue(1167, 1362)), float(1100+GetRandomValue(1, 1000))});
                    }
                    for(int i = 2; i < 3; i++)
                    {
                        goblin_top[i].SetPos({float(GetRandomValue(505, 630)), float(GetRandomValue(-10, -1000))});
                    }
                    for(int i = 2; i < 3; i++)
                    {
                        kadal_top[i].SetPos({float(GetRandomValue(505, 630)), float(GetRandomValue(-10, -1000))});
                    }
                    /*for(int i = 5; i < 7; i++)
                    {
                        goblin_bottom[i].SetPos({float(GetRandomValue(1167, 1362)), float(1100+GetRandomValue(1, 1000))});
                    }*/
                }
                //GOBLIN MID
                for(int i = 3; i < 5; i++)
                {
                    if(goblin_mid[i].GetPos().x <= protect_box->GetPos().x+200)
                    {
                        if(goblin_mid[i].GetPos().y < protect_box->GetPos().y-20)
                        {
                            goblin_mid[i].MoveDown();
                        }
                        else if(goblin_mid[i].GetPos().y > protect_box->GetPos().y+20)
                        {
                            goblin_mid[i].MoveUp();
                        }
                    }
                    goblin_mid[i].MoveLeft();
                    goblin_mid[i].Update();
                    if(!goblin_mid[i].GetLiveStatus())
                    {
                        if(dead_goblin_mid[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_goblin_mid[i] = i;
                        }
                    }
                }
                //KADAL MID
                for(int i = 3; i < 5; i++)
                {
                    if(kadal_mid[i].GetPos().x <= protect_box->GetPos().x+200)
                    {
                        if(kadal_mid[i].GetPos().y < protect_box->GetPos().y-20)
                        {
                            kadal_mid[i].MoveDown();
                        }
                        else if(kadal_mid[i].GetPos().y > protect_box->GetPos().y+20)
                        {
                            kadal_mid[i].MoveUp();
                        }
                    }
                    kadal_mid[i].MoveLeft();
                    kadal_mid[i].Update();
                    if(!kadal_mid[i].GetLiveStatus())
                    {
                        if(dead_kadal_mid[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_kadal_mid[i] = i;
                        }
                    }
                }
                //GOBLIN BOTTOM
                for(int i = 3; i < 4; i++)
                {
                    if(goblin_bottom[i].GetPos().y >= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)) && goblin_bottom[i].GetPos().x > 1167)
                    {
                        goblin_bottom[i].MoveUp();
                        if(goblin_bottom[i].GetPos().y <= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)))
                        {
                            goblin_bottom[i].MoveLeft();
                        }
                    }
                    else
                    {
                        goblin_bottom[i].MoveLeft();
                    }
                    goblin_bottom[i].Update();
                    if(!goblin_bottom[i].GetLiveStatus())
                    {
                        if(dead_goblin_bottom[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_goblin_bottom[i] = i;
                        }
                    }
                }
                //KADAL BOTTOM
                for(int i = 3; i < 4; i++)
                {
                    if(kadal_bottom[i].GetPos().y >= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)) && kadal_bottom[i].GetPos().x > 1167)
                    {
                        kadal_bottom[i].MoveUp();
                        if(kadal_bottom[i].GetPos().y <= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)))
                        {
                            kadal_bottom[i].MoveLeft();
                        }
                    }
                    else
                    {
                        kadal_bottom[i].MoveLeft();
                    }
                    kadal_bottom[i].Update();
                    if(!kadal_bottom[i].GetLiveStatus())
                    {
                        if(dead_kadal_bottom[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_kadal_bottom[i] = i;
                        }
                    }
                }
                //GOBLIN TOP
                for(int i = 2; i < 3; i++)
                {
                    if(goblin_top[i].GetPos().y <= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)) && goblin_top[i].GetPos().x > 300)
                    {
                        goblin_top[i].MoveDown();
                        if(goblin_top[i].GetPos().y >= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)))
                        {
                            goblin_top[i].MoveLeft();
                        }
                    }
                    else
                    {
                        goblin_top[i].MoveLeft();
                    }
                    goblin_top[i].Update();
                    if(!goblin_top[i].GetLiveStatus())
                    {
                        if(dead_goblin_top[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_goblin_top[i] = i;
                        }
                    }
                }
                //KADAL TOP
                for(int i = 2; i < 3; i++)
                {
                    if(kadal_top[i].GetPos().y <= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)) && kadal_top[i].GetPos().x > 300)
                    {
                        kadal_top[i].MoveDown();
                        if(kadal_top[i].GetPos().y >= float(GetRandomValue(protect_box->GetPos().y-20, protect_box->GetPos().y+20)))
                        {
                            kadal_top[i].MoveLeft();
                        }
                    }
                    else
                    {
                        kadal_top[i].MoveLeft();
                    }
                    kadal_top[i].Update();
                    if(!kadal_top[i].GetLiveStatus())
                    {
                        if(dead_kadal_top[i] != i)
                        {
                            WaveKillCounter++;
                            if(GetRandomValue(1, 100) > 60)
                            {
                                player->AddAmmo(1);
                            }
                            dead_kadal_top[i] = i;
                        }
                    }
                }
                if(WaveKillCounter == 8)
                {
                    wave++;
                    WaveKillCounter = 0;
                    SpawnWave = true;
                }
                break;
            case 4:
                game.ModStageCleared(true);
            default:
                break;
            }

            //Protectbox update
            for(int i = 0; i < GOBLIN_MID_COUNT; i++)
            {
                protect_box->DetectEnemy(goblin_mid[i]);
            }
            for(int i = 0; i < GOBLIN_BOTTOM_COUNT; i++)
            {
                protect_box->DetectEnemy(goblin_bottom[i]);
            }
            for(int i = 0; i < GOBLIN_TOP_COUNT; i++)
            {
                protect_box->DetectEnemy(goblin_top[i]);
            }
            for(int i = 0; i < KADAL_MID_COUNT; i++)
            {
                protect_box->DetectEnemy(kadal_mid[i]);
            }
            for(int i = 0; i < KADAL_BOTTOM_COUNT; i++)
            {
                protect_box->DetectEnemy(kadal_bottom[i]);
            }
            for(int i = 0; i < KADAL_TOP_COUNT; i++)
            {
                protect_box->DetectEnemy(kadal_top[i]);
            }
            //Player death game over
            if(!player->GetLiveStatus())
            {
                game.ModGameOver(true);
            }

            //Action Update
            //Player Melee
            for(int i = 0; i < GOBLIN_MID_COUNT; i++)
            {
                player->MeleeHit(goblin_mid[i]);
            }
            for(int i = 0; i < GOBLIN_BOTTOM_COUNT; i++)
            {
                player->MeleeHit(goblin_bottom[i]);
            }
            for(int i = 0; i < GOBLIN_TOP_COUNT; i++)
            {
                player->MeleeHit(goblin_top[i]);
            }
            for(int i = 0; i < KADAL_MID_COUNT; i++)
            {
                player->MeleeHit(kadal_mid[i]);
            }
            for(int i = 0; i < KADAL_BOTTOM_COUNT; i++)
            {
                player->MeleeHit(kadal_bottom[i]);
            }
            for(int i = 0; i < GOBLIN_TOP_COUNT; i++)
            {
                player->MeleeHit(kadal_top[i]);
            }
            //Enemy Melee
            for(int i = 0; i < GOBLIN_MID_COUNT; i++)
            {
                goblin_mid[i].DetectMeleeRange(*player);
                goblin_mid[i].MeleeHit(*player);
            }
            for(int i = 0; i < GOBLIN_BOTTOM_COUNT; i++)
            {
                goblin_bottom[i].DetectMeleeRange(*player);
                goblin_bottom[i].MeleeHit(*player);
            }
            for(int i = 0; i < GOBLIN_TOP_COUNT; i++)
            {
                goblin_top[i].DetectMeleeRange(*player);
                goblin_top[i].MeleeHit(*player);
            }

            //Collision Update
            for(int i = 0; i < player->MAX_BULLET; i++)
            {
                //PlayerBullet[i].DetectCollisionEntity(goblin_top[0]);
                for(int j = 0; j < 4; j++)
                {
                    PlayerBullet[i].DetectCollisionWall(wall[j]);
                }
                
                for(int j = 0; j < GOBLIN_MID_COUNT; j++)
                {
                    PlayerBullet[i].DetectCollisionEntity(goblin_mid[j]);
                    PlayerBullet[i].DetectCollisionEntity(kadal_mid[j]);
                }
                for(int j = 0; j < GOBLIN_BOTTOM_COUNT; j++)
                {
                    PlayerBullet[i].DetectCollisionEntity(goblin_bottom[j]);
                    PlayerBullet[i].DetectCollisionEntity(kadal_bottom[j]);
                }
                for(int j = 0; j < GOBLIN_TOP_COUNT; j++)
                {
                    PlayerBullet[i].DetectCollisionEntity(goblin_top[j]);
                    PlayerBullet[i].DetectCollisionEntity(kadal_top[j]);
                }
            }
        }
        //Game over panel update
        Panel_GameOver->Update();
        //Pause panel update
        Panel_Pause->Update();
        //Level Cleared panel
        Panel_Cleared->Update();
        //Pause
        if(IsKeyPressed(KEY_P))
        {
            game.ModPause(true);
        }
        //Manual exit
        if(IsKeyPressed(KEY_ESCAPE))
        {
            game.ModExit(true);
        }
    }

    void Draw()
    {
        BeginDrawing();

        ClearBackground(WHITE);
        for(int i = 0; i < 627; i++)
        {
            tile[i].Draw();
        }
        for(int i = 0; i < 4; i++)
        {
            wall[i].Draw();
        }
        protect_box->Draw();
        player->Draw();
        for(int i = 0; i < player->MAX_BULLET; i++)
        {
            PlayerBullet[i].Draw();
        }

        player->DrawUI();
        //DrawText(TextFormat("%.2f, %.2f", player->GetPos().x, player->GetPos().y), 500, 0, 50, BLACK);
        DrawText(TextFormat("%d", wave), 900, 0, 20, BLACK);
        //DrawText(TextFormat("%d", WaveKillCounter), 950, 0, 20, BLACK);

        switch(wave)
        {
        case 1:
            for(int i = 0; i < 2; i++)
            {
                goblin_mid[i].Draw();
                kadal_mid[i].Draw();
            }
            for(int i = 0; i < 2; i++)
            {
                goblin_bottom[i].Draw();
                kadal_bottom[i].Draw();
            }
            break;
         case 2:
            for(int i = 2; i < 3; i++)
            {
                goblin_mid[i].Draw();
                kadal_mid[i].Draw();
            }
            for(int i = 2; i < 3; i++)
            {
                goblin_bottom[i].Draw();
                kadal_bottom[i].Draw();
            }
            for(int i = 0; i < 2; i++)
            {
                goblin_top[i].Draw();
                kadal_top[i].Draw();
            }
            break;
        case 3:
            for(int i = 3; i < 5; i++)
            {
                goblin_mid[i].Draw();
                kadal_mid[i].Draw();
            }
            for(int i = 3; i < 4; i++)
            {
                goblin_bottom[i].Draw();
                kadal_bottom[i].Draw();
            }
            for(int i = 2; i < 1; i++)
            {
                goblin_top[i].Draw();
                kadal_top[i].Draw();
            }
            break;
        default:
            break;
        }
        Panel_GameOver->Draw();
        Panel_Pause->Draw();
        Panel_Cleared->Draw();
        EndDrawing();
    }
};

//Main
int main()
{
    //Game Init
    game.GameInit();
    game.TextureLoad();

    bool CreateObject = true;

    Debug *debug;
    MainMenu *main_menu;
    HelpMenu *help_menu;
    StageSelectMenu *stage_select_menu;

    StageOne *stage_one;
    StageTwo *stage_two;

    while(!game.IsExit())
    {
        while(game.IsInMainMenu())
        {
            if(CreateObject)
            {
                main_menu = new MainMenu;
                CreateObject = false;
            }
            
            main_menu->Update();
            main_menu->Draw();

            if(!game.IsInMainMenu() || game.IsExit())
            {
                main_menu->~MainMenu();
                CreateObject = true;
                break;
            }
        }
        while(game.IsInHelp())
        {
            if(CreateObject)
            {
                help_menu = new HelpMenu;
                CreateObject = false;
            }
            
            help_menu->Update();
            help_menu->Draw();

            if(!game.IsInHelp() || game.IsExit())
            {
                help_menu->~HelpMenu();
                CreateObject = true;
                break;
            }
        }
        while(game.IsInDebug())
        {
            if(CreateObject)
            {
                debug = new Debug;
                CreateObject = false;
            }

            debug->Update();
            debug->Draw();

            if(!game.IsInDebug() || game.IsExit())
            {
                debug->~Debug();
                CreateObject = true;
                break;
            }
        }
        while(game.IsInStageSelect())
        {
            if(CreateObject)
            {
                stage_select_menu = new StageSelectMenu;
                CreateObject = false;
            }

            stage_select_menu->Update();
            stage_select_menu->Draw();

            if(!game.IsInStageSelect() || game.IsExit())
            {
                stage_select_menu->~StageSelectMenu();
                CreateObject = true;
                break;
            }
        }

        while(game.IsInStageOne())
        {
            if(CreateObject)
            {
                stage_one = new StageOne;
                CreateObject = false;
            }

            stage_one->Update();
            stage_one->Draw();

            if(game.IsResetStage())
            {
                stage_one->~StageOne();
                CreateObject = true;
                game.ModResetStage(false);
            }

            if(!game.IsInStageOne() || game.IsExit())
            {
                stage_one->~StageOne();
                CreateObject = true;
                break;
            }
        }
        while(game.IsInStageTwo())
        {
            if(CreateObject)
            {
                stage_two = new StageTwo;
                CreateObject = false;
            }

            stage_two->Update();
            stage_two->Draw();

            if(game.IsResetStage())
            {
                stage_two->~StageTwo();
                CreateObject = true;
                game.ModResetStage(false);
            }

            if(!game.IsInStageTwo() || game.IsExit())
            {
                stage_two->~StageTwo();
                CreateObject = true;
                break;
            }
        }
    }

    game.TextureUnload();
    CloseWindow();
}