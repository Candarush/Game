#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>

// 1 - Регистрируется игрок на игру и вносит сумму в банк.
// 2 - В процессе кона в размещается база шириной W.

// Классы: Игрок, банк, игра, база, шар.

using namespace std;

double GetRand(double start, double end){
    if (end < start)
    {
        cerr<<"Ошибка в диапазоне случайных значений.";
        return 0;
    }
    srand(rand());
    return (start + (rand() % (int)(end-start)));
}

class Player
{
    int behavior; //0 - игрок, 1,2,3 - боты различной сложности
    int money;
    string name;
    
    public:
    Player(string iname, int imoney){
        name = iname;
        money = imoney;
        behavior = 0;
    }
    Player(string iname, int imoney, int ibehavior){
        name = iname;
        money = imoney;
        behavior = ibehavior;
    }
    int GetMoney(){
        return money;
    }
    string GetName(){
        return name;
    }
    int GetBehavior(){
        return behavior;
    }
    void AddMoney(int addmoney){
         money += addmoney;
    }
};

class Base
{
    protected:
    double x; // Координата цели.
    double w; // Ширина цели.
    
    public:
    void Randomize(){
        x = GetRand(0,100);
        w = GetRand(5,15);
    }
    void Show(){
        cout << "Координаты базы: "<< x << " Ширина: " << w << endl;
    }
    double GetX(){
        return x;
    }
    double GetW(){
        return w;
    }
};

class Bank
{
    protected:
        int money = 0;
    
    public:
    
        void AddMoney(int addmoney){
            money += addmoney;
        }

        int GetMoney(){
            return money;
        }

        void SetMoney(int imoney){
            money = imoney;
        }

        void Show(){
            cout << "В банке: "<< GetMoney()<<endl;
        }
};

class Game
{
    protected:
    bool isActive; // Идет ли игра?
    vector<Player> players; // Массив игроков.
    
    public:
    Base target; // Цель.
    Bank gameBank; // Банк.
    
    void NewPlayer(string name, int money)
    {
        players.push_back(Player(name, money));
    }
    
    void NewPlayer(string name, int money, int ibehavior)
    {
        players.push_back(Player(name, money, ibehavior));
    }
    
    void ShowBank()
    {
        cout << "Счет игроков:" << endl;
        for (int i = 0; i<players.size();i++)
        {
            cout << players[i].GetName() << " " << players[i].GetMoney() << endl;
        }
    }

    string GetName(int i)
    {
        return players[i].GetName();
    }
    
    double ThrowBall(double alpha, double speed){
        
        double landed = 2*speed*sin(2*alpha/57.2958)/9.8;
        cout << "Мяч упал: " <<landed << endl;
        return landed;
    }
    
    int NextTurn(int* cycle){
        
        double alpha, speed, landed = 0;
        
        cout << "Ход игрока: " << players[*cycle].GetName() << endl;
        
        switch (players[*cycle].GetBehavior())
        {
            case 1:
                speed = GetRand(0,1000);
                alpha = GetRand(1,89);
                break;
            case 2:
                speed = (target.GetX()+target.GetW()/2)*4.9+GetRand(-200,200);
                alpha = 45+GetRand(-20,20);
                break;
            case 3:
                speed = (target.GetX()+target.GetW()/2)*4.9+GetRand(-150,150);
                alpha = 45+GetRand(-15,15);
                break;
            default:
                cout << "Введите угол: ";
                cin >> alpha;
                alpha = (double)alpha;
                cout << "Введите скорость: ";
                cin >> speed;
                speed = (double)speed;
                break;
                
        }
        
        landed = ThrowBall(alpha, speed);
        if (landed > target.GetX() && landed < target.GetX() + target.GetW()) return *cycle+1;
            
        *cycle = (*cycle + 1) % players.size();
            
        return 0;
    }
    
    int GetPlayerCount(){
        return (int)players.size();
    }
    
    void SetBets(){
        
        string readline;
        int bet;
        
        for (int i = 0; i<players.size(); i++)
        {
            cout<<"Ставка игрока " << players[i].GetName() << " (на счету " << players[i].GetMoney()<< "): ";
            
            cin >> readline;
            bet = stoi(readline);
            
            if (bet <= players[i].GetMoney() && bet>0)
            {
                players[i].AddMoney(-bet);
                gameBank.AddMoney(bet);
            }
            else
            {
                i--;
            }
        }
    }
    
    void RegisterPlayers(){
        
        string cin_buffer;
        
        while (!isActive)
        {
            cout << "Добавить игрока? \"y\" для добавления игрока. \"b\" для добавления бота. "<<endl<<"Добавить игрока: ";
            cin >> cin_buffer;
            
            if (cin_buffer=="y"){
                string iname;
                int imoney;
                
                cout << "Введите логин: ";
                cin >> iname;
                
                cout << "Деньги на счете: ";
                cin >> cin_buffer;
                imoney = stoi(cin_buffer);
                
                NewPlayer(iname, imoney);
                
            }
            else if (cin_buffer=="b"){
                string difficulty_name;
                int difficulty;
                string iname;
                int imoney;
                cout<<"Выберите сложность бота:"
                        <<endl<<"\"1\"-Легкий."
                        <<endl<<"\"2\"-Средний."
                        <<endl<<"\"3\"-Сложный."
                        <<endl<<"Cложность бота: ";
                cin >> cin_buffer;
                difficulty = stoi(cin_buffer);
                switch (difficulty)
                {
                    case 1:
                        difficulty_name = "Easy";
                        break;
                    case 2:
                        difficulty_name = "Normal";
                        break;
                    case 3:
                        difficulty_name = "Hard";
                        break;
                    default:
                        difficulty_name = "Easy";
                        difficulty = 0;
                        break;
                }
                
                cout << "Введите имя бота: ";
                cin >> iname;
                
                cout << "Деньги на счете: ";
                cin >> cin_buffer;
                imoney = stoi(cin_buffer);
                
                NewPlayer("["+difficulty_name+" Bot] "+iname, imoney, difficulty);
            }
            else
            {
                isActive = true;
                ShowBank();
            }
            
        }
    }
    
    void Play()
    {
        string cin_buffer;
        target.Randomize();
        cout<< "Игра началась!"<<endl;
        while (1)
        {
            static int turncycle = 0;
            target.Show();
            int winner = NextTurn(&turncycle);
            if (winner)
            {
                cout << GetName(winner-1) << " выиграл " << gameBank.GetMoney() << endl;
                players[winner-1].AddMoney(gameBank.GetMoney());
                gameBank.SetMoney(0);
                ShowBank();
                
                cout << "Завершить игру? \"y\" - да." << endl;
                cin >> cin_buffer;
                if (cin_buffer == "y") break;
                SetBets();
                target.Randomize();
                ShowBank();
                turncycle = 0;
                cout<< "Игра началась!"<<endl;
            }
        }
        cout << "Игра завершена." << endl;
        ShowBank();
    }
    
};

int main ()
{
    Game MyGame;
    
    MyGame.RegisterPlayers();
    MyGame.SetBets();
    MyGame.Play();
    
}
