#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>

// 1 - Регистрируется игрок на игру и вносит сумму в банк.
// 2 - В процессе кона в следующее место размещается база шириной W.
// 3 - Игрок бросает биту под углом Alpha и скоростью V. Масса и сила притяжения - константы. Если попал в базу - выиграл.

// Классы: Игрок, банк, игра, база, шар.

using namespace std;

class Game
{
    class Player
    {
        int money;
        public:
        string name;
        Player(string iname, int imoney){
            name = iname;
            money = imoney;
        }
        int GetMoney()
        {
            return money;
        }
        void ChangeMoney(int addmoney)
        {
             money += addmoney;
        }
    };
    
    class Base
    {
        public:
        double x;
        double w;
        
    public:
        void Randomize(){
            srand((unsigned int)time(NULL));
            x = rand() % 100;
            w = 5 + rand() % 5;
            cout << "Координаты базы: "<< x << " Ширина: " << w << endl;
        }
        void Show(){
            cout << "Координаты базы: "<< x << " Ширина: " << w << endl;
        }
    };
    
    class Bank
    {
        int money;
        
        public:
        
        Bank(){
            money = 0;
        }
        
        void ChangeMoney(int addmoney)
        {
            money += addmoney;
        }
        
        int GetMoney()
        {
            return money;
        }
        
        void SetMoney(int in)
        {
            money = in;
        }
    };
    
    
    public:
    
    Game(){
        target.Randomize();
    }
    
    vector<Player> players;
    
    Base target;
    Bank gameBank;
    
    void NewPlayer(string name, int money)
    {
        players.push_back(Player(name, money));
    }
    void ShowPlayers()
    {
        cout << "Счет игроков:" << endl;
        for (int i = 0; i<players.size();i++)
        {
            cout << players[i].name << " " << players[i].GetMoney() << endl;
        }
    }

    string GetName(int i)
    {
        return players[i].name;
    }
    
    double ThrowBall(double alpha, double speed){
        
        double landed = 2*speed*sin(2*alpha)/9.8;
        
        cout << "Мяч упал: " <<landed << endl;
        return landed;
    }

    int NextTurn(int* cycle){
        
        double alpha;
        double speed;
        
        cout << "Ход игрока: " << players[*cycle].name << endl;
        cout << "Введите угол: ";
        cin >> alpha;
        alpha = (double)alpha;
        cout << "Введите скорость: ";
        cin >> speed;
        speed = (double)speed;
        double landed = ThrowBall(alpha, speed);
        if (landed > target.x && landed < target.x + target.w) return *cycle+1;
            
        *cycle = (*cycle + 1) % players.size();
            
        return 0;
    }
    
    void SetBets(){
        
        string readline;
        int bet;
        
        for (int i = 0; i<players.size(); i++)
        {
            cout<<"Ставка игрока " << players[i].name << " (на счету " << players[i].GetMoney()<< "):"<< endl;
            
            cin >> readline;
            bet = stoi(readline);
            
            if (bet <= players[i].GetMoney() && bet>0)
            {
                players[i].ChangeMoney(-bet);
                gameBank.ChangeMoney(bet);
            }
            else
            {
                i--;
            }
            
        }
        
    }

};

using namespace std;

int main ()
{
    Game MyGame;
    
    string readline = "y";
    bool GameIsOn = false;
    
    while (!GameIsOn)
    {
        
        if (readline=="y")
        {
            string iname;
            int imoney;
            
            cout << "Login: ";
            cin >> iname;
            
            
            cout << "Money: ";
            cin >> readline;
            imoney = stoi(readline);
            
            MyGame.NewPlayer(iname, imoney);
            
            
            cout << "Добавить игрока? \"y\" - да." << endl;
            cin >> readline;
            
        }
        else
        {
            GameIsOn = true;
            MyGame.ShowPlayers();
        }
        
    }
    
    MyGame.SetBets();
    cout << "В банке: "<< MyGame.gameBank.GetMoney()<<endl;
    cout<< "Игра началась!"<<endl;
    
    while (1)
    {
        
        int turncycle = 0;
        MyGame.target.Show();
        int winner = MyGame.NextTurn(&turncycle);
        if (winner)
        {
            cout << MyGame.GetName(winner-1) << " выиграл " << MyGame.gameBank.GetMoney() << endl;
            MyGame.players[winner-1].ChangeMoney(MyGame.gameBank.GetMoney());
            MyGame.gameBank.SetMoney(0);
            MyGame.ShowPlayers();
            
            cout << "Завершить игру? \"y\" - да." << endl;
            cin >> readline;
            if (readline == "y") break;
            MyGame.SetBets();
            MyGame.target.Randomize();
            cout << "В банке: "<< MyGame.gameBank.GetMoney()<<endl;
            cout<< "Игра началась!"<<endl;
        }
    }
    cout << "Игра завершена." << endl;
    MyGame.ShowPlayers();
    
}

