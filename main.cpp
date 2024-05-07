#include <iostream>
#include <math.h>
#include <vector>
#include "raylib.h"

using namespace std;

const int WindowWidth = 800;
const int WindowHeight = 800;

//Class
class Entity
{
    protected:
    float health;
    float deffense;
    float attack;
    float speed;            //scalar speed
    Vector2 speedV;         //vector speed
    bool IsAlive = true;

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
    ~Entity();

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

    //Reset state function
    void ResetState()
    {
        IsAlive = true;
    }

    virtual void Update() = 0;
    virtual void Draw() = 0;

};

class Player:public Entity
{
    private:
    int AttackMode;
    float ShieldHealth;
    int const PLAYER_MAX_BULLET = 50;
    int PlayerAmmo = 10;

    vector<Bullet> bullet;


    public:
    //Constructor & destructor
    Player(float health, float deffense, float attack, float speed, Vector2 position, Rectangle frame, Texture2D Texture):Entity(health, deffense,attack,speed, position, frame, texture)
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

        for(int i = 0; i < PLAYER_MAX_BULLET; i++)
        {
            bullet.push_back(Bullet(5, GetPos()));
        }
    };


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

    public:
    Projectile(float speed, Vector2 position)
    {
        this->speed = speed;
        this->speedV = {speed, speed};
        this->position = position;
    }
    ~Projectile();

    float GetSpeed()
    {
        return speed;
    }
    Vector2 GetSpeedV()
    {
        return speedV;
    }

    virtual void Update() = 0;
    virtual void 
};

class Bullet:public Projectile
{
    private:
    Texture2D texture;
    Rectangle frame;

    public:
    Bullet(float speed, Vector2 position):Projectile(speed, position)
    {
        this->speed = speed;
        this->speedV = {speed, speed};
        this->position = position;
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


//Main
int main()
{
    InitWindow(WindowWidth, WindowHeight, "Test");

    //Main Game Loop
    while(!WindowShouldClose())
    {

    }
}