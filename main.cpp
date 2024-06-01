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
    TEX_PLAYER = 0, TEX_BULLET, TEX_KADAL, TEX_NAGA
};


//Game class
class Game
{
    private:
    const int WindowWidth = 1920;
    const int WindowHeight = 1080;
    const int MAX_FPS = 60;

    bool GameOver = false;
    bool ResetStage = false;

    bool InMainMenu = true;
    bool InDebug = false;
    bool InStageSelect = false;
    bool InStageOne = false;
    bool InStageTwo = false;

    bool exit = false;

    double TimeElapsed;
    double ReferenceTime = GetTime();
    double CurrentTime;

    
    //Textures
    Texture2D tex_player, tex_bullet, tex_naga, tex_kadal;
    
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
        tex_player = LoadTexture("assets/test_box_player.png");
        tex_kadal = LoadTexture("assets/kadal.png");
        tex_naga = LoadTexture("assets/naga.png");
    }
    void TextureUnload()
    {
        UnloadTexture(tex_bullet);
        UnloadTexture(tex_player);
        UnloadTexture(tex_kadal);
        UnloadTexture(tex_naga);
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
        case 2:
            return tex_kadal;
            break;
        case 3:
            return tex_naga;
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
Game game;

//Menu, UI, HUD
class Button
{
    protected:
    Rectangle ButtonBox;
    Vector2 CenterPoint;
    char *text;
    Texture2D texture;
    Rectangle TextureFrame;
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
        DrawText(text, CenterPoint.x-MeasureText(text, FontSize)/2.0f, CenterPoint.y, FontSize, BLACK);
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
        
        this->ButtonBox = {CenterPoint.x-MeasureText(text, FontSize)/2.0f, CenterPoint.y, float(MeasureText(text, FontSize)), float(FontSize)};
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
        this->ButtonBox = {CenterPoint.x-MeasureText(text, FontSize)/2.0f, CenterPoint.y, float(MeasureText(text, FontSize)), float(FontSize)};
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
        switch(stage)
        {
        case 1:
            this->text = new char[8]{"Level 1"};
            break;
        case 2:
            this->text = new char[8]{"Level 2"};
            break;
        case 3:
            this->text = new char[8]{"Level 3"};
            break;
        case 4:
            this->text = new char[8]{"Level 4"};
            break;
        case 5:
            this->text = new char[8]{"Level 5"};
            break;
        default:
            break;
        }
        this->ButtonBox = {CenterPoint.x-MeasureText(text, FontSize)/2.0f, CenterPoint.y, float(MeasureText(text, FontSize)), float(FontSize)};
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
        this->ButtonBox = {CenterPoint.x-MeasureText(text, FontSize)/2.0f, CenterPoint.y, float(MeasureText(text, FontSize)), float(FontSize)};
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
        this->ButtonBox = {CenterPoint.x-MeasureText(text, FontSize)/2.0f, CenterPoint.y, float(MeasureText(text, FontSize)), float(FontSize)};
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
        this->ButtonBox = {CenterPoint.x-MeasureText(text, FontSize)/2.0f, CenterPoint.y, float(MeasureText(text, FontSize)), float(FontSize)};
    }
    void Update() override
    {
        if(CheckCollisionPointRec(GetMousePosition(), ButtonBox))
        {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                game.ModResetStage(true);
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
    
    Vector2 CenterPoint = {game.GetWindowWidth(), game.GetWindowHeight()};
    Rectangle MainPanel;
    Rectangle PanelBorder;


    public:
    GameOverPanel()
    {
        PanelBorder = {CenterPoint.x, CenterPoint.y, };
        MainPanel = {CenterPoint.x, CenterPoint.y, };
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

    public:
    StatusBar(Vector2 position, float width, float height, Color color, float MaxStatsValue, float StatsValue)
    {
        this->CenterPoint = position;
        this->bar = {CenterPoint.x-float((bar.width/BarPercentage)), CenterPoint.y - float(bar.height), width*BarPercentage, height};
        this->width = width;
        this->color = color;
        this->MaxStatsValue = MaxStatsValue;
        this->StatsValue = StatsValue;
    }
    void Update(Vector2 position, float MaxStatsValue, float StatsValue)
    {
        CenterPoint = position;
        BarPercentage = StatsValue / MaxStatsValue;
        bar.x = CenterPoint.x-25;
        bar.y = CenterPoint.y-bar.height-30;
        bar.width = width*BarPercentage;

        //cout<<CenterPoint->x<<" "<<CenterPoint->y<<endl;
    }
     void Draw()
     {
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
    float MeleeAttackSwingRad;
    float MeleeAttackSwingRadOri;

    float AtkCooldown;
    double CurrentCooldown;
    double CooldownCounter;

    bool FacingRight = true;

    bool MovementAllowed = true;

    //Frame & position related
    Vector2 position;
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
        return IsAlive;
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
    }

    Vector2 GetPos()
    {
        return position;
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
            position.x += speedV.x/game.ScaleFPS();
        }
        if(!FacingRight)
        {
            TextureFrame.width *= -1.0;
            FacingRight = true;
        }
    }
    void MoveLeft()
    {
        if(MovementAllowed)
        {
            position.x -= speedV.x/game.ScaleFPS();
        }
        if(FacingRight)
        {
            TextureFrame.width *= -1.0;
            FacingRight = false;
        }
    }
    void MoveUp()
    {
        if(MovementAllowed)
        {
            position.y -= speedV.y/game.ScaleFPS();
        }
    }
    void MoveDown()
    {
        if(MovementAllowed)
        {
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

    int MAX_AMMO = 10;
    int ammo = 10;


    public:
    int const MAX_BULLET = 50;

    //Constructor & destructor
    Player(float health, float defense, float attack, float speed, Vector2 position):Entity(health, defense, attack, speed, position)
    {
        //Stats
        this->ShieldHealth = 500;

        this->texture = game.AssignTexture(TEX_PLAYER);

        //Frame & Hitbox
        this->TextureFrame = {0, 0, 16, 16};
        this->DestFrame = {position.x, position.y, 50, 50};
        this->Hitbox = {this->DestFrame.x-this->DestFrame.width/2.0f, this->DestFrame.y-this->DestFrame.height/2.0f, this->DestFrame.width, this->DestFrame.height};
        this->CenterPoint = position;

        this->MeleeAttackPointOri = {CenterPoint.x + float((Hitbox.width/2.0f+30)*cos(MeleeAttackSwingRad)), CenterPoint.y - float((Hitbox.height/2.0f+30)*sin(MeleeAttackSwingRad))};
        this->MeleeAttackPoint = MeleeAttackPointOri;
        this->MeleeAttackSwingRadOri = float(1.0/4.0);
        this->MeleeAttackSwingRad = MeleeAttackSwingRadOri;
        this->AtkCooldown = 0.4;
    };

    void DetectInput()
    {
        //Movement
        if(IsKeyDown(KEY_W))
        {
            MoveUp();
        }
        if(IsKeyDown(KEY_S))
        {
            MoveDown();
        }
        if(IsKeyDown(KEY_D))
        {
            MoveRight();
        }
        if(IsKeyDown(KEY_A))
        {
            MoveLeft();
        }

        //Shot
        if(IsKeyPressed(KEY_SPACE) && ammo > 0)
        {
            ModShotState(true);
            ammo--;
        }
        else if(IsKeyReleased(KEY_SPACE))
        {
            ModShotState(false);
        }

        //Reload
        if(IsKeyPressed(KEY_R))
        {
            ammo = MAX_AMMO;
        }

        //Melee
        if(IsKeyPressed(KEY_V) && TrigAttack)
        {
            ModMeleeState(true);
        }

    }

    //Update related
    void Update() override
    {
        DetectInput();
        UpdateDestFrame();
        UpdateHitbox();
        UpdateMelee();
    }

    void UpdateMelee() override
    {
        if(TrigMelee && (MeleeAttackSwingRad >= -(1.0/4.0)))
        {
            MeleeAttackSwingRad -= 0.1/game.ScaleFPS();
            CooldownCounter = game.GetTimer();
            TrigAttack = false;
            TrigMelee = true;
        }
        else
        {
            TrigMelee = false;
            MeleeAttackSwingRad = MeleeAttackSwingRadOri;
            AttackCD();
        }
        if(FacingRight)
        {
            MeleeAttackPointOri = {CenterPoint.x + float((Hitbox.width/2.0f+30)*cos(MeleeAttackSwingRad)), CenterPoint.y - float((Hitbox.height/2.0f+30)*sin(MeleeAttackSwingRad))};
        }
        else
        {
            MeleeAttackPointOri = {CenterPoint.x - float((Hitbox.width/2.0f+30)*cos(MeleeAttackSwingRad)), CenterPoint.y - float((Hitbox.height/2.0f+30)*sin(MeleeAttackSwingRad))};
        }
        
        MeleeAttackPoint = MeleeAttackPointOri;
    }

    //Draw related
    void Draw() override
    {
        //DrawTexturePro(texture, frame, Hitbox, {Hitbox.width/2.0f, Hitbox.height/2.0f}, 0, WHITE);
        DrawTexturePro(texture, TextureFrame, DestFrame, {DestFrame.width/2.0f, DestFrame.height/2.0f}, 0, WHITE);
        if(TrigMelee)
        {
            DrawCircle(MeleeAttackPoint.x, MeleeAttackPoint.y, 10, RED);
        }
        //DrawText(TextFormat("%d/%d", ammo, MAX_AMMO), 100, 0, 20, BLACK);
    }
    void DrawUI()
    {
        DrawText(TextFormat("%d/%d", ammo, MAX_AMMO), 100, 0, 20, BLACK);
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
    
    StatusBar *HealthBar;

    public:
    EnemyMeleeBasic(Vector2 position):Entity(0, 0, 0, 0, position)
    {
            this->health = 50;
            this->MaxHealth = health;
            this->defense = 5;
            this->attack = 20;
            this->position = position;
            this->speed = 4;
            this->speedV = {speed, speed};

            this->texture = game.AssignTexture(TEX_KADAL);

            this->TextureFrame = {12, 47, 78, 35};
            this->DestFrame = {position.x, position.y, 78, 35};
            this->Hitbox = {this->DestFrame.x-this->DestFrame.width/2.0f, this->DestFrame.y-this->DestFrame.height/2.0f, this->DestFrame.width, this->DestFrame.height};
            this->CenterPoint = position;

            this->MeleeAttackPointOri = {CenterPoint.x + 45, CenterPoint.y - 10};
            this->MeleeAttackPoint = MeleeAttackPointOri;
            this->AtkCooldown = 1;

            HealthBar = new StatusBar(this->position, 50, 10, RED, this->MaxHealth, this->health);
    }

    void Update() override
    {
        if(health <= 0)
        {
            IsAlive = false;
        }
        if(IsAlive)
        {
            UpdateHitbox();
            UpdateMelee();
            HealthBar->Update(position, MaxHealth, health);
        }
    }

    void UpdateHitbox()
    {
        DestFrame.x = position.x;
        DestFrame.y = position.y;
        CenterPoint = position;
        Hitbox.x = DestFrame.x - DestFrame.width/2.0f;
        Hitbox.y = DestFrame.y - DestFrame.height/2.0f;
    }

    void UpdateMelee() override
    {
        if(TrigMelee && IsAlive && (MeleeAttackPoint.y < MeleeAttackPointOri.y+20))
        {
            MeleeAttackPoint.y = MeleeAttackPoint.y + 0.5/game.ScaleFPS();
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
            MeleeAttackPointOri = {CenterPoint.x + 45, CenterPoint.y - 10};
        }
        else
        {
            MeleeAttackPointOri = {CenterPoint.x - 50, CenterPoint.y - 10};
        }
    }

    void DetectMeleeRange(Entity& target)
    {
        if(TrigAttack && (CheckCollisionPointRec({CenterPoint.x - 50, CenterPoint.y}, target.GetHitbox()) || CheckCollisionPointRec({CenterPoint.x + 45, CenterPoint.y}, target.GetHitbox())))
        {
            TrigMelee = true;
            MovementAllowed = false;
        }
    }

    void MeleeHit(Entity& target) override
    {
        if(CheckCollisionPointRec(MeleeAttackPoint, target.GetHitbox()) && TrigMelee && target.GetLiveStatus() && !IsMeleeHitSuccess() && IsAlive && MeleeAttackPoint.y > MeleeAttackPointOri.y+10)
        {
            target.TakeDamage(attack);
            ModMeleeHitSuccess(true);
        }
        else if(MeleeAttackPoint.y == MeleeAttackPointOri.y)
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

            DrawRectangleRec({MeleeAttackPoint.x, MeleeAttackPoint.y, 5, 5}, RED);
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
    Color color;

    public:
    Wall(Vector2 position, Size size, int orientation)
    {
        this->position = position;
        this->CenterPoint = position;
        this->size = size;

        this->Hitbox = {CenterPoint.x - size.width/2.0f, CenterPoint.y - size.height/2.0f, size.width, size.height};
        this->orientation = orientation;
    }
    Wall(Vector2 position, Size size, int orientation, Color color)
    {
        this->position = position;
        this->CenterPoint = position;
        this->size = size;

        this->Hitbox = {CenterPoint.x - size.width/2.0f, CenterPoint.y - size.height/2.0f, size.width, size.height};
        this->orientation = orientation;
        
        this->color = color;
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
    }
    
};
class Tile
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
    Rectangle tile;
    Color color;

    public:
    Tile(Vector2 position, Size size)
    {
        this->position = position;
        this->CenterPoint = position;
        this->size = size;

        this->tile = {CenterPoint.x - size.width/2.0f, CenterPoint.y - size.height/2.0f, size.width, size.height};
    }
    Tile(Vector2 position, Size size, Color color)
    {
        this->position = position;
        this->CenterPoint = position;
        this->size = size;

        this->tile = {CenterPoint.x - size.width/2.0f, CenterPoint.y - size.height/2.0f, size.width, size.height};

        this->color = color;
    }

    void Draw()
    {
        DrawRectangleRec(tile, color);
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
    Texture2D texture;
    float TravelDist = 0;
    float const MAX_TRAVEL_DISTANCE = 2000;

    Entity *entity;

    public:
    Bullet(float speed, Vector2 position, Texture2D texture, Entity& entity):Projectile(speed, position)
    {
        this->texture = texture;
        this->entity = &entity;

        //Frame & hitbox
        this->TextureFrame = {0, 0, 16, 16};
        this->DestFrame = {position.x, position.y, 20, 20};
        this->Hitbox = {this->DestFrame.x-this->DestFrame.width/2.0f, this->DestFrame.y-this->DestFrame.height/2.0f, this->DestFrame.width, this->DestFrame.height};
        this->CenterPoint = position;
    }
    void Update() override
    {
        //Shot trigger
        if(!IsActive)
        {
            position = origin;
            ShotTrigger(entity->GetShotState());
        }
        //update position
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
            if(entity->IsFacingRight() && TextureFrame.width < 0)
            {
                TextureFrame.width *= -1;
            }
            if(!entity->IsFacingRight() && TextureFrame.width > 0)
            {
                TextureFrame.width *= -1;
            }

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
    Rectangle box;
    Vector2 position;

    public:
    ProtectBox(Vector2 position)
    {
        this->position = position;
        this->box = {position.x - 40, position.y - 40, 80, 80};
    }

    void Draw()
    {
        DrawRectangleRec(box, BLUE);
    }

    void DetectEnemy(Entity& enemy)
    {
        if(CheckCollisionRecs(box, enemy.GetHitbox()))
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
        return box;
    }
};


//Game related functions (nanti dibuat jadi class)
class Debug
{
    private:
    Player *player;
    EnemyMeleeBasic *enemy;
    EnemyMeleeBasic *enemy1;
    vector<Bullet> bullet;
    vector<EnemyMeleeBasic> test;

    Wall *wall;

    public:
    Debug()
    {
        game.StartTimer();
        
        this->player = new Player(100, 10, 30, 5, {200, 400});
        this->enemy = new EnemyMeleeBasic({800, 200});
        this->enemy1 = new EnemyMeleeBasic({800, 400});
        enemy1->MoveLeft();
        enemy1->Update();
        this->wall = new Wall({600, 200}, {300, 60}, TOP, GREEN);

        for(int i = 0; i < player->MAX_BULLET; i++)
        {
            bullet.push_back(Bullet(5, player->GetCenterPoint(), game.AssignTexture(TEX_BULLET), *player));
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
        delete enemy;
        bullet.clear();
    }
    void Draw()
    {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawFPS(0, 0);

        player->Draw();
        
        wall->Draw();

        enemy->Draw();
        enemy1->Draw();

        for(int i = 0; i < player->MAX_BULLET; i++)
        {
            bullet[i].Draw();
        }
        for(int i = 0; i < 3; i++)
        {
            test[i].Draw();
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

    public:
    MainMenu()
    {
        DebugButton = new ButtonDebug({game.GetWindowWidth()/2.0f, 500}, BLACK);
        PlayButton = new ButtonPlay({game.GetWindowWidth()/2.0f, 300}, BLACK);
        ExitButton = new ButtonExit({game.GetWindowWidth()/2.0f, 700}, BLACK);
        cout<<"Main Menu Created"<<endl;
    }
    ~MainMenu()
    {
        delete DebugButton;
        delete PlayButton;
        delete ExitButton;
        cout<<"Main Manu Destroyed"<<endl;
    }
    void Update()
    {
        DebugButton->Update();
        PlayButton->Update();
        ExitButton->Update();
    }
    void Draw()
    {
        BeginDrawing();

        ClearBackground(WHITE);
        DebugButton->Draw();
        PlayButton->Draw();
        ExitButton->Draw();

        EndDrawing();
    }
};

class StageSelectMenu
{
    private:
    vector<ButtonStageSelect> Button_StageSelect;
    int TOTAL_STAGE = 5;
    public:
    StageSelectMenu()
    {
        for(int i = 0; i < TOTAL_STAGE; i++)
        {
            Button_StageSelect.push_back(ButtonStageSelect({400, float(200+100*i)}, BLACK, i+1));
        }
        cout<<"Stage Select Menu Created"<<endl;
    }
    ~StageSelectMenu()
    {
        Button_StageSelect.clear();
        cout<<"Stage Select Menu Destroyed"<<endl;
    }

    void Update()
    {
        for(int i = 0; i < TOTAL_STAGE; i++)
        {
            Button_StageSelect[i].Update();
        }
        if(IsKeyPressed(KEY_ESCAPE))
        {
            game.ModExit(true);
        }
    }

    void Draw()
    {
        BeginDrawing();
        ClearBackground(WHITE);
        for(int i = 0; i < TOTAL_STAGE; i++)
        {
            Button_StageSelect[i].Draw();
        }
        EndDrawing();
    }
};

class StageOne
{
    private:
    const int KADAL_MID_COUNT = 20;
    const int KADAL_TOP_COUNT = 13;
    const int KADAL_BOTTOM_COUNT = 18;

    int wave = 1;
    bool SpawnWave = true;
    bool WaveCleared = false;
    int WaveKillCounter = 0;

    Player *player;
    vector<Bullet> PlayerBullet;
    vector<EnemyMeleeBasic> kadal_top;
    vector<EnemyMeleeBasic> kadal_mid;
    vector<EnemyMeleeBasic> kadal_bottom;
    ProtectBox *protect_box;
    vector<Wall> wall;
    vector<Tile> tile;

    public:
    StageOne()
    {
        game.StartTimer();

        //Player
        player = new Player(100, 10, 20, 5, {300, 500});
        for(int i = 0; i < player->MAX_BULLET; i++)
        {
            PlayerBullet.push_back(Bullet(15, player->GetCenterPoint(), game.AssignTexture(TEX_BULLET), *player));
        }
        
        //Protect box
        protect_box = new ProtectBox({100, 500});

        //Enemy
        for(int i = 0; i < KADAL_MID_COUNT ;i++)
        {
            kadal_mid.push_back(EnemyMeleeBasic({2000, 2000}));
        }
        for(int i = 0; i < KADAL_TOP_COUNT ;i++)
        {
            kadal_top.push_back(EnemyMeleeBasic({2000, 2000}));
        }
        for(int i = 0; i < KADAL_BOTTOM_COUNT ;i++)
        {
            kadal_bottom.push_back(EnemyMeleeBasic({2000, 2000}));
        }
        
        //Wall
        wall.push_back(Wall({240, 130},{480, 260}, TOP, GRAY));
        wall.push_back(Wall({648+1278/2.0f, 130},{1278, 260}, TOP, GRAY));
        wall.push_back(Wall({1114/2.0f, 805+(1080-805)/2.0f},{1114, 1080-805}, TOP, GRAY));
        wall.push_back(Wall({1362+(1920-1362)/2.0f, 805+(1080-805)/2.0f},{1920-1362, 1080-805}, TOP, GRAY));

        cout<<"Stage One Created"<<endl;
    }
    ~StageOne()
    {
        delete player;
        delete protect_box;
        wall.clear();
        PlayerBullet.clear();
        cout<<"Stage One Destroyed"<<endl;
    }
    void Update()
    {
        if(!game.IsGameOver())
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
                    SetRandomSeed(time(0));
                    for(int i = 0; i < 7; i++)
                    {
                        kadal_mid[i].SetPos({float(1930+GetRandomValue(1, 2000)), float(GetRandomValue(300, 700))});
                    }
                    for(int i = 0; i < 5; i++)
                    {
                        kadal_bottom[i].SetPos({float(GetRandomValue(1167, 1362)), float(1100+GetRandomValue(1, 1000))});
                    }
                }
                //KADAL MID
                for(int i = 0; i < 7; i++)
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
                        WaveKillCounter++;
                    }
                }
                //KADAL BOTOM
                for(int i = 0; i < 5; i++)
                {
                    kadal_bottom[i].MoveUp();
                    kadal_bottom[i].Update();
                }
                /*if(WaveKillCounter == 7)
                {
                    wave++;
                }*/
                
                break;
            default:
                break;
            }

            //Protectbox update
            for(int i = 0; i < KADAL_MID_COUNT; i++)
            {
                protect_box->DetectEnemy(kadal_mid[i]);
            }

            //Action Update

            //Collision Update
            for(int i = 0; i < player->MAX_BULLET; i++)
            {
                //PlayerBullet[i].DetectCollisionEntity(kadal_top[0]);
                for(int j = 0; j < 4; j++)
                {
                    PlayerBullet[i].DetectCollisionWall(wall[j]);
                }
                
                for(int j = 0; j < KADAL_MID_COUNT; j++)
                {
                    PlayerBullet[i].DetectCollisionEntity(kadal_mid[j]);
                }
            }

            
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
        player->Draw();
        for(int i = 0; i < 4; i++)
        {
            wall[i].Draw();
        }
        protect_box->Draw();

        for(int i = 0; i < player->MAX_BULLET; i++)
        {
            PlayerBullet[i].Draw();
        }

        player->DrawUI();
        DrawText(TextFormat("%.2f, %.2f", player->GetPos().x, player->GetPos().y), 500, 0, 50, BLACK);

        switch(wave)
        {
        case 1:
            for(int i = 0; i < 7; i++)
            {
                kadal_mid[i].Draw();
            }
            for(int i = 0; i < 5; i++)
            {
                kadal_bottom[i].Draw();
            }
            break;
            
        default:
            break;
        }

        DrawFPS(0, 0);

        if(game.IsGameOver())
        {
            DrawText("GAME OVER", game.GetWindowWidth()/2.0f, game.GetWindowHeight()/2.0f, 70, RED);
        }

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
    StageSelectMenu *stage_select_menu;

    StageOne *stage_one;

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

            if(!game.IsInStageOne() || game.IsExit())
            {
                stage_one->~StageOne();
                CreateObject = true;
                break;
            }
        }
    }

    game.TextureUnload();
    CloseWindow();

        /*
        ///////////////////////
        //MANUAL RESET, DEBUG PURPOSE ONLY//
        //////////////////////
        if(IsKeyPressed(KEY_TAB))
        {
            player.ResetState();
            enemy.ResetState();
            for(int i = 0; i < 10; i++)
            {
                kadal[i].ResetState();
            }
            game.ModGameOver(false);
        }        
    }*/
    
}
