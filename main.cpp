#include <iostream>
#include <math.h>
#include <vector>
#include "raylib.h"

using namespace std;


//Class

//Game
class Game
{
    private:
    const int Windowwidth = 800;
    const int WindowHeight = 800;
    
    //Textures
    

    public:
    //Textures
    Texture2D player_tex, bullet_tex;

    void GameInit()
    {
        InitWindow(Windowwidth, WindowHeight, "Game");
        SetTargetFPS(60);
        TextureLoad();
    }

    void TextureLoad()
    {
        bullet_tex = LoadTexture("assets/dummy_bulletpng.png");
        player_tex = LoadTexture("assets/test_box_player.png");
    }


};



class Entity
{
    protected:
    //stats related
    float health;
    float deffense;
    float attack;
    float speed;            //scalar speed
    Vector2 speedV;         //vector speed
    bool IsAlive = true;

    //attack related
    bool TrigShot = false;

    //
    Vector2 position;
    Rectangle frame;
    Texture2D texture;
    
    public:
    //Constructor & destructor
    Entity(float health, float deffense, float attack, float speed, Vector2 position, Rectangle frame, Texture2D texture)
    {
        this->health = health;
        this->deffense = deffense;
        this->attack = attack;
        this->speed = speed;
        this->speedV = {this->speed, this->speed};
        this->position = position;
        this->frame = frame;
        this->texture = texture;
    }
    ~Entity()
    {

    }

    //Functions
    //Property access functions
    float GetHealth()
    {
        return health;
    }
    float GetDeff()
    {
        return deffense;
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

    Vector2 GetPos()
    {
        return position;
    }

    bool GetShotState()
    {
        return TrigShot;
    }

    //Property modification functions
    float AddHealth(float val)
    {
        return health + val;
    }
    float AddDeffense(float val)
    {
        return deffense + val;
    }
    float AddSpeed(float val)
    {
        return speed + val;
    }
    float AddAttack(float val)
    {
        return attack + val;
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

    //Reset state function
    void ResetState()
    {
        IsAlive = true;
    }

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
    Player(float health, float deffense, float attack, float speed, Vector2 position, Rectangle frame, Texture2D texture):Entity(health, deffense, attack, speed, position, frame, texture)
    {
        this->health = health;
        this->deffense = deffense;
        this->attack = attack;
        this->speed = speed;
        this->speedV = {this->speed, this->speed};
        this->position = position;
        this->frame = frame;
        this->texture = texture;
        this->ShieldHealth = 500;

    };

    void DetectInput()
    {
        //Shot
        if(IsKeyPressed(KEY_SPACE))
        {
            TrigShot = true;
        }
        if(IsKeyReleased(KEY_SPACE))
        {
            TrigShot = false;
        }
    }

    void Update() override
    {
        DetectInput();
    }

    void Draw() override
    {
        DrawTexture(texture, position.x, position.y, WHITE);
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

class Enemy:public Entity
{
    private:
    int AbilityMode;
    int EnemyType;

    public:

};

//Attack type class
//Projectile
class Projectile
{
    protected:
    float speed;
    bool IsActive = false;
    Vector2 speedV;
    Vector2 position;
    Vector2 origin;

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
    Rectangle frame;
    float TravelDist = 0;
    float const MAX_TRAVEL_DISTANCE = 200;

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
    }
    void Update() override
    {
        //Shot trigger
        if(!IsActive)
        {
            ShotTrigger(entity->GetShotState());
        }
        //update position
        if(IsActive)
        {
            position.x += speedV.x/(float(GetFPS())/60.0);
            TravelDist += speedV.x/(float(GetFPS())/60.0);
        }

        //update origin
        origin = entity->GetPos();

        //max travel
        if(TravelDist > MAX_TRAVEL_DISTANCE)
        {
            IsActive = false;
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
        }
    }

    void Draw() override
    {
        if(IsActive)
        {
            DrawTexture(texture, position.x, position.y, WHITE);
        }
    }
};

//Enumerator
//Entity type
//Enemy
enum EnemyType
{
    ENEMY_MELEE = 1, ENEMY_RANGED, ENEMY_REVIVE, ENEMY_BOSS
};
//Ally
enum AllyType
{
    ALLY_MELEE = 1, ALLY_RANGED, ALLY_SUPPORT, ALLY_BLOCKER
};

//Stats type
enum Attribute
{
    ATTRIBUTE_HEALTH = 0, ATTRIBUTE_DEFF, ATTRIBUTE_ATK, ATTRIBUTE_SPD, 
};




//Game related functions (nanti dibuat jadi class)



//Main
int main()
{
    //Game Init
    Game game;
    game.GameInit();
    game.TextureLoad();
    //Class init
    Player player(100, 10, 10, 5, {200, 400}, {200, 400, 20, 20}, game.player_tex);
    vector<Bullet> bullet;
    for(int i = 0; i < player.MAX_BULLET; i++)
    {
        bullet.push_back(Bullet(5, player.GetPos(), game.bullet_tex, player));
    }
    
    //Main Game Loop
    while(!WindowShouldClose())
    {
        //Draw
        BeginDrawing();
        ClearBackground(WHITE);
        DrawFPS(0, 0);
        player.Draw();
        for(int i = 0; i < player.MAX_BULLET; i++)
        {
            bullet[i].Draw();
        }


        EndDrawing();

        //Update
        player.Update();
        for(int i = 0; i < player.MAX_BULLET; i++)
        {
            bullet[i].Update();
        }


    }
}
