#include <iostream>
#include <math.h>
#include <vector>
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

//Texture index
enum TextureIndex
{
    TEX_PLAYER = 0, TEX_BULLET, TEX_KADAL, TEX_NAGA
};


//Game class
class Game
{
    private:
    const int Windowwidth = 800;
    const int WindowHeight = 800;
    const int MAX_FPS = 60;

    bool GameOver = false;

    
    //Textures
    Texture2D tex_player, tex_bullet, tex_naga, tex_kadal;
    
    public:
    //Camera
    Camera2D camera = {{0, 0}, {0, 0}, 0, 1};

    void GameInit()
    {
        InitWindow(Windowwidth, WindowHeight, "Game");
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

    bool GetGameStatus()
    {
        return GameOver;
    }

    int GetWindowWidth()
    {
        return Windowwidth;
    }
    int GetWindowHeight()
    {
        return WindowHeight;
    }

    void ModGameStatus(bool status)
    {
        GameOver = status;
    }
};

//Game class declaration
Game game;

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
    bool HitByMelee = false;
    Vector2 MeleeAttackPoint;
    Vector2 MeleeAttackPointOri;
    float MeleeAttackSwingRad;
    float MeleeAttackSwingRadOri;

    bool FacingRight = true;

    //Frame & position related
    Vector2 position;
    Rectangle TextureFrame;
    Rectangle DestFrame;
    Rectangle Hitbox;
    Vector2 CenterPoint;

    Texture2D texture;
    
    public:
    //Constructor & destructor
    Entity(float health, float defense, float attack, float speed, Vector2 position, Texture2D texture)
    {
        this->health = health;
        this->MaxHealth = health;
        this->defense = defense;
        this->attack = attack;
        this->speed = speed;
        this->speedV = {this->speed, this->speed};
        this->position = position;
        this->texture = texture;
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
    
    void ModShotState(bool state)
    {
        if(state)
        {
            TrigShot = true;
        }
        else
        {
            TrigShot = false;
        }
    }
    void ModMeleeState(bool state)
    {
        if(state)
        {
            TrigMelee = true;
        }
        else
        {
            TrigMelee = false;
        }
    }
    void ModHitByMelee(bool state)
    {
        if(state)
        {
            HitByMelee = true;
        }
        else
        {
            HitByMelee = false;
        }
    }

    void TakeDamage(float damage)
    {
        if(IsAlive)
        {
            health -= float(damage-defense);
        }
    }

    //Movement related
    void MoveRight()
    {
        position.x += speedV.x/game.ScaleFPS();
        if(!FacingRight)
        {
            TextureFrame.width *= -1.0;
            FacingRight = true;
        }
    }
    void MoveLeft()
    {
        position.x -= speedV.x/game.ScaleFPS();
        if(FacingRight)
        {
            TextureFrame.width *= -1.0;
            FacingRight = false;
        }
    }
    void MoveUp()
    {
        position.y -= speedV.y/game.ScaleFPS();
    }
    void MoveDown()
    {
        position.y += speedV.y/game.ScaleFPS();
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
    void MeleeHit(Entity& target)
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

    int PlayerAmmo = 10;


    public:
    int const MAX_BULLET = 50;

    //Constructor & destructor
    Player(float health, float defense, float attack, float speed, Vector2 position, Texture2D texture):Entity(health, defense, attack, speed, position, texture)
    {
        //Stats
        this->health = health;
        this->MaxHealth = health;
        this->defense = defense;
        this->attack = attack;
        this->speed = speed;
        this->speedV = {this->speed, this->speed};
        this->position = position;
        this->texture = texture;
        this->ShieldHealth = 500;

        //Frame & Hitbox
        this->TextureFrame = {0, 0, 16, 16};
        this->DestFrame = {position.x, position.y, 50, 50};
        this->Hitbox = {this->DestFrame.x-this->DestFrame.width/2.0f, this->DestFrame.y-this->DestFrame.height/2.0f, this->DestFrame.width, this->DestFrame.height};
        this->CenterPoint = position;

        this->MeleeAttackPointOri = {CenterPoint.x + float((Hitbox.width/2.0f+30)*cos(MeleeAttackSwingRad)), CenterPoint.y - float((Hitbox.height/2.0f+30)*sin(MeleeAttackSwingRad))};
        this->MeleeAttackPoint = MeleeAttackPointOri;
        this->MeleeAttackSwingRadOri = float(1.0/4.0);
        this->MeleeAttackSwingRad = MeleeAttackSwingRadOri;
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
        if(IsKeyPressed(KEY_SPACE))
        {
            ModShotState(true);
        }
        else if(IsKeyReleased(KEY_SPACE))
        {
            ModShotState(false);
        }

        //Melee
        if(IsKeyPressed(KEY_V))
        {
            ModMeleeState(true);
        }
        else if(IsKeyReleased(KEY_V))
        {
            //ModMeleeState(false);
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
            TrigMelee = true;
        }
        else
        {
            TrigMelee = false;
            MeleeAttackSwingRad = MeleeAttackSwingRadOri;
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
        DrawCircle(MeleeAttackPoint.x, MeleeAttackPoint.y, 10, RED);
        DrawText(TextFormat("%f", FacingRight), 100, 0, 20, BLACK);
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

    public:
    EnemyMeleeBasic(Vector2 position):Entity(0, 0, 0, 0, position, texture)
    {
            this->health = 50;
            this->MaxHealth = health;
            this->defense = 5;
            this->attack = 10;
            this->position = position;
            this->speed = 5;
            this->speedV = {speed, speed};

            this->texture = game.AssignTexture(TEX_KADAL);

            this->TextureFrame = {0, 0, -128, 128};
            this->DestFrame = {position.x, position.y, 100, 100};
            this->Hitbox = {this->DestFrame.x-this->DestFrame.width/2.0f, this->DestFrame.y-this->DestFrame.height/2.0f, this->DestFrame.width, this->DestFrame.height-30};
            this->CenterPoint = position;
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

    void Draw() override
    {
        if(IsAlive)
        {
            DrawTexturePro(texture, TextureFrame, DestFrame, {DestFrame.width/2.0f, DestFrame.height/2.0f}, 0, WHITE);
            DrawText(TextFormat("%.1f/%.1f", health, MaxHealth), position.x-MeasureText(TextFormat("%.1f/%.1f", health, MaxHealth), 15)/2, position.y-30, 15, RED);
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
        this->speed = speed;
        this->speedV = {speed, speed};
        this->position = position;
        this->origin = position;
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

class ProtectBox
{
    private:
    Rectangle box;
    Vector2 position;

    public:
    ProtectBox(Vector2 position)
    {
        this->position = position;
        this->box = {position.x, position.y, 40, 40};
    }

    void Draw()
    {
        DrawRectangleRec(box, BLUE);
    }

    void DetectEnemy(Entity& enemy)
    {
        if(CheckCollisionRecs(box, enemy.GetHitbox()))
        {
            game.ModGameStatus(false);
        }
    }
};


//Menu, UI, HUD
class Button
{
    private:
    Rectangle ButtonBox;
    Vector2 CenterPoint;

};

class StatusBar
{

};

//Game related functions (nanti dibuat jadi class)
class Debug
{
    private:
    Player *player;
    EnemyMeleeBasic *enemy;
    vector<Bullet> bullet;

    public:
    Debug()
    {
        this->player = new Player(100, 10, 30, 5, {200, 400}, game.AssignTexture(TEX_PLAYER));
        this->enemy = new EnemyMeleeBasic({400, 400});

        for(int i = 0; i < player->MAX_BULLET; i++)
        {
            bullet.push_back(Bullet(5, player->GetCenterPoint(), game.AssignTexture(TEX_BULLET), *player));
        }
    }
    ~Debug()
    {

    }
    void Draw()
    {
        BeginDrawing();
        ClearBackground(WHITE);
        DrawFPS(0, 0);

        player->Draw();
        

        enemy->Draw();

        for(int i = 0; i < player->MAX_BULLET; i++)
        {
            bullet[i].Draw();
        }

        EndDrawing();
    }

    void Update()
    {
        if(!game.GetGameStatus())
        {
            player->Update();
            for(int i = 0; i < player->MAX_BULLET; i++)
            {
                bullet[i].Update();
            }
            player->MeleeHit(*enemy);
            enemy->Update();
            //Detect collision
            for(int i = 0; i < player->MAX_BULLET; i++)
            {
                bullet[i].DetectCollisionEntity(*enemy);
            }
        }   
    }
};


//Main
int main()
{
    //Game Init
    game.GameInit();
    game.TextureLoad();

    Debug debug;

    while(!WindowShouldClose())
    {
        debug.Update();
        debug.Draw();
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
            game.ModGameStatus(false);
        }        
    }*/
    
}
