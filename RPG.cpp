#include <iostream>
#include <fstream>
#include <cstdlib>
#include <conio.h>
#include <chrono>
#include <thread>
#include <vector>
#include <ctime>

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

struct personajes {
    string nombre;
    string barraV;
    string barraM;
    int pociones = 0;
    int dinero = 0;
    int armas = 0;
    int danio = 0;
    int mana = 0;
    int vida = 0;
    int X = 0;
    int Y = 0;

    string barraVida(int V) {
        string barraV;
        for(int i = 0; i < V / 10; i++) {
            barraV += "♥";
        }
        for(int i = V / 10; i < 10; i++) {
            barraV += " ";
        }
        return barraV;
    }

    string barraMana(int M) {
        string barraM;
        for(int i = 0; i < M / 10; i++) {
            barraM += "★";
        }
        for(int i = M / 10; i < 10; i++) {
            barraM += " ";
        }
        return barraM;
    }
};


void batallaJugadorEnemigoflor(personajes &P, personajes &E1, int &batalla, int &an, const vector<string> &flor);
void batallaJugadorEnemigogumbat(personajes &P, personajes &E2, int &batalla, int &an, const vector<string> &gumbat);
void guardarPartida(const string mapa[7][7], ofstream &archivoP, const personajes &P, const personajes &E, const personajes &S, personajes &E1, personajes &E2);
bool leerPartida(string (&mapa)[7][7], ifstream &archivoP, personajes &P, personajes &E, personajes &S, personajes &E1, personajes &E2);
void comprar(int &batalla, const vector<string> &Rv, int an, personajes &P);
void inicializarPersonajes(personajes &P, personajes &E, personajes &S, personajes &E1, personajes &E2);
void batallaJugadorEnemigo(personajes &P, personajes &E, int &batalla, int &an, const vector<string> &Ue, const vector<string> &UE, const vector<string> &Eu);

const vector<string> flor = {
    "  ▓▓▓▓▓▓▓▓▓\n▓▓▓▓  ▓▓  ▓▓▓\n▓▓▓▓L ▓▓L ▓▓▓\n  ▓▓▓▓▓▓▓▓▓\n      ▓▓\n▓▓    ▓▓    ▓▓\n ▓▓▓▓ ▓▓ ▓▓▓▓\n  ▓▓▓ ▓▓ ▓▓▓\n    ▓▓▓▓▓▓\n"
};

const vector<string> gumbat = {
    "     ▓▓▓▓▓▓▓▓\n   ▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓  ◥▓▓▓▓◤  ▓▓\n▓▓▓▓   ▓▓▓▓   ▓▓▓▓\n▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n▓▓▓▓▓∆______∆▓▓▓▓▓\n  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n    ▓▓▓▓  ▓▓▓▓",
    "     ▓▓▓▓▓▓▓▓\n   ▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓  \\▓▓▓▓/  ▓▓\n▓▓▓▓   ▓▓▓▓   ▓▓▓▓\n▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n▓▓▓▓▓∆______∆▓▓▓▓▓\n  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n    ▓▓▓▓  ▓▓▓▓"
};

const vector<string> Vr = {
    "            ⛀     ⛁       ⛀     ⛃\n        ⛁      ⛃     ⛀      ⛁        ⛃\n       ▄▄▄▄                  ▄▄▄▄   \n       ██░░██\033[31m▒▒  ▒▒▒▒▒▒  ▒▒\033[0m██░░██      \n       ██░░░░\033[31m▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m░░░░██    ░░░░\n         ██░░\033[31m▒▒\033[0m██\033[31m▒▒▒▒▒▒\033[0m██\033[31m▒▒\033[0m░░██    ░░░░░░░░\n         \033[31m▒▒▒▒\033[0m██░░\033[31m▒▒\033[0m██\033[31m▒▒\033[0m░░██\033[31m\033[31m▒▒▒▒\033[0m    ░░░░░░\033[31m▒▒\033[0m\n         \033[31m▒▒\033[0m██\033[31m▒▒▒▒\033[0m██░░██\033[31m▒▒▒▒\033[0m██\033[31m▒▒\033[0m      ████\033[31m▒▒\033[0m\n         ░░██\033[31m▒▒\033[0m██████████\033[31m▒▒\033[0m██░░      \033[31m▒▒▒▒\033[0m██\n           ░░░░\033[31m▒▒\033[0m██████\033[31m▒▒\033[0m░░░░        \033[31m▒▒▒▒▒▒\033[0m\n       ░░░░  ░░░░░░░░░░░░░░  ░░░░  ████\033[31m▒▒▒▒\033[0m\n       ░░░░░░░░░░░░░░░░░░░░░░░░░░  \033[31m▒▒▒▒\033[0m██\n         ░░░░░░░░░░░░░░░░░░░░░░  ██\033[31m▒▒▒▒\033[0m\n           \033[31m▒▒\033[0m░░░░░░░░░░░░░░\033[31m▒▒\033[0m██\033[31m▒▒▒▒\033[0m██\033[31m▒▒\033[0m\n         \033[31m▒▒▒▒\033[0m░░░░░░░░░░░░░░\033[31m▒▒▒▒\033[0m██\033[31m▒▒\033[0m\n         \033[31m▒▒\033[0m░░░░░░░░░░░░░░░░░░\033[31m▒▒\033[0m\n       ░░░░░░░░░░░░░░░░░░░░░░░░░░\n       ░░░░░░░░          ░░░░░░░░\n"
};

const vector<string> Rv = {
    "\n\n       ▄▄▄▄                  ▄▄▄▄   \n       ██░░██\033[31m▒▒  ▒▒▒▒▒▒  ▒▒\033[0m██░░██      \n       ██░░░░\033[31m▒▒▒▒▒▒▒▒▒▒▒▒▒▒\033[0m░░░░██    ░░░░\n         ██░░\033[31m▒▒\033[0m██\033[31m▒▒▒▒▒▒\033[0m██\033[31m▒▒\033[0m░░██    ░░░░░░░░\n         \033[31m▒▒▒▒\033[0m██░░\033[31m▒▒\033[0m██\033[31m▒▒\033[0m░░██\033[31m\033[31m▒▒▒▒\033[0m    ░░░░░░\033[31m▒▒\033[0m\n         \033[31m▒▒\033[0m██\033[31m▒▒▒▒\033[0m██░░██\033[31m▒▒▒▒\033[0m██\033[31m▒▒\033[0m      ████\033[31m▒▒\033[0m\n         ░░██\033[31m▒▒\033[0m██████████\033[31m▒▒\033[0m██░░      \033[31m▒▒▒▒\033[0m██\n           ░░░░\033[31m▒▒\033[0m██████\033[31m▒▒\033[0m░░░░        \033[31m▒▒▒▒▒▒\033[0m\n       ░░░░  ░░░░░░░░░░░░░░  ░░░░  ████\033[31m▒▒▒▒\033[0m\n       ░░░░░░░░░░░░░░░░░░░░░░░░░░  \033[31m▒▒▒▒\033[0m██\n         ░░░░░░░░░░░░░░░░░░░░░░  ██\033[31m▒▒▒▒\033[0m\n           \033[31m▒▒\033[0m░░░░░░░░░░░░░░\033[31m▒▒\033[0m██\033[31m▒▒▒▒\033[0m██\033[31m▒▒\033[0m\n         \033[31m▒▒▒▒\033[0m░░░░░░░░░░░░░░\033[31m▒▒▒▒\033[0m██\033[31m▒▒\033[0m\n         \033[31m▒▒\033[0m░░░░░░░░░░░░░░░░░░\033[31m▒▒\033[0m\n       ░░░░░░░░░░░░░░░░░░░░░░░░░░\n       ░░░░░░░░          ░░░░░░░░\n"
};

const vector<string> Eu = {
    "        ▓▓▓▓▓\n      ▓▓▓▓▓▓▓▓▓▓\n   ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓░░▓░░▓░░▓▓▓▓\n     ▓▓░░▓░░▓░░▓▓\n    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n   ▓▓▓ ▓▓▓ ▓▓▓ ▓▓▓▓\n  ▓▓▓ ▓▓▓▓ ▓▓▓▓ ▓▓▓▓\n\n",
    "\n\n       ▓▓▓▓▓▓▓\n    ▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓░░▓░░▓░░▓▓▓▓\n     ▓▓░░▓░░▓░░▓▓\n ▓  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓  ▓\n▓▓▓▓▓▓▓ ▓▓▓ ▓▓▓ ▓▓▓▓▓▓▓\n  ▓▓▓ ▓▓▓▓ ▓▓▓▓ ▓▓▓▓\n",
    "\n\n       ▓▓▓▓▓▓▓\n    ▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓░░▓░░▓░░▓▓▓▓\n     ▓▓░░▓░░▓░░▓▓\n    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n▓▓ ▓▓▓ ▓▓▓ ▓▓▓ ▓▓▓▓ ▓▓\n  ▓▓▓ ▓▓▓▓ ▓▓▓▓ ▓▓▓▓\n",
    "\n\n       ▓▓▓▓▓▓▓\n    ▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓░░▓░░▓░░▓▓▓▓\n     ▓▓░░▓░░▓░░▓▓\n    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n   ▓▓▓ ▓▓▓ ▓▓▓ ▓▓▓▓\n  ▓▓▓ ▓▓▓▓ ▓▓▓▓ ▓▓▓▓\n",
    "\n\n      ▓▓▓▓▓▓▓▓▓\n    ▓▓▓▓▓▓▓▓▓▓▓▓▓\n   ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n   ▓▓▓▓░░▓░░▓░░▓▓▓\n     ▓▓░░▓░░▓░░▓▓\n    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n   ▓▓▓ ▓▓▓ ▓▓▓ ▓▓▓▓\n  ▓▓▓ ▓▓▓▓ ▓▓▓▓ ▓▓▓▓\n"
};

const vector<string> Ue = {
    "   ▓    ▓▓▓▓▓    ▓\n ▓    ▓▓▓▓▓▓▓▓▓▓  ▓\n   ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓░░\\░░/░░▓▓▓▓\n     ▓▓░░\\░░/░░▓▓\n    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n   ▓▓▓ ▓▓▓ ▓▓▓ ▓▓▓▓\n  ▓▓▓ ▓▓▓▓ ▓▓▓▓ ▓▓▓▓\n\n",
    "\n\n▓     ▓▓▓▓▓▓▓▓   ▓\n▓   ▓▓▓▓▓▓▓▓▓▓▓▓▓   ▓\n  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓░░\\░░/░░▓▓▓▓\n     ▓▓░░\\░░/░░▓▓\n ▓  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓  ▓\n▓▓▓▓▓▓▓ ▓▓▓ ▓▓▓ ▓▓▓▓▓▓▓\n  ▓▓▓ ▓▓▓▓ ▓▓▓▓ ▓▓▓▓\n",
    "\n\n  ▓   ▓▓▓▓▓▓▓▓   ▓\n▓   ▓▓▓▓▓▓▓▓▓▓▓▓▓  ▓\n  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓░░\\░░/░░▓▓▓▓\n     ▓▓░░\\░░/░░▓▓\n    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n▓▓ ▓▓▓ ▓▓▓ ▓▓▓ ▓▓▓▓ ▓▓\n  ▓▓▓ ▓▓▓▓ ▓▓▓▓ ▓▓▓▓\n",
    "\n\n░     ▓▓▓▓▓▓▓▓   ░\n░   ▓▓▓▓▓▓▓▓▓▓▓▓▓   ░\n  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓░░\\░░/░░▓▓▓▓\n     ▓▓░░\\░░/░░▓▓\n    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n   ▓▓▓ ▓▓▓ ▓▓▓ ▓▓▓▓\n  ▓▓▓ ▓▓▓▓ ▓▓▓▓ ▓▓▓▓\n",
    "\n\n ░    ▓▓▓▓▓▓▓▓▓   ░\n░   ▓▓▓▓▓▓▓▓▓▓▓▓▓   ░\n   ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n   ▓▓▓▓░░\\░░/░░▓▓▓\n     ▓▓░░\\░░/░░▓▓\n    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n   ▓▓▓ ▓▓▓ ▓▓▓ ▓▓▓▓\n  ▓▓▓ ▓▓▓▓ ▓▓▓▓ ▓▓▓▓\n"
};

const vector<string> UE = {
    "   ▓    ▓▓▓▓▓    ▓\n ▓    ▓▓▓▓▓▓▓▓▓▓  ▓\n   ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓░░/░░\\░░▓▓▓▓\n     ▓▓░░/░░\\░░▓▓\n    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n   ▓▓▓ ▓▓▓ ▓▓▓ ▓▓▓▓\n  ▓▓▓ ▓▓▓▓ ▓▓▓▓ ▓▓▓▓\n\n",
    "\n\n▓     ▓▓▓▓▓▓▓▓   ▓\n▓   ▓▓▓▓▓▓▓▓▓▓▓▓▓   ▓\n  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓░░/░░\\░░▓▓▓▓\n     ▓▓░░/░░\\░░▓▓\n ▓  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓  ▓\n▓▓▓▓▓▓▓ ▓▓▓ ▓▓▓ ▓▓▓▓▓▓▓\n  ▓▓▓ ▓▓▓▓ ▓▓▓▓ ▓▓▓▓\n",
    "\n\n  ▓   ▓▓▓▓▓▓▓▓   ▓\n▓   ▓▓▓▓▓▓▓▓▓▓▓▓▓  ▓\n  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓░░/░░\\░░▓▓▓▓\n     ▓▓░░/░░\\░░▓▓\n    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n▓▓ ▓▓▓ ▓▓▓ ▓▓▓ ▓▓▓▓ ▓▓\n  ▓▓▓ ▓▓▓▓ ▓▓▓▓ ▓▓▓▓\n",
    "\n\n░     ▓▓▓▓▓▓▓▓   ░\n░   ▓▓▓▓▓▓▓▓▓▓▓▓▓   ░\n  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n  ▓▓▓▓▓░░/░░\\░░▓▓▓▓\n     ▓▓░░/░░\\░░▓▓\n    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n   ▓▓▓ ▓▓▓ ▓▓▓ ▓▓▓▓\n  ▓▓▓ ▓▓▓▓ ▓▓▓▓ ▓▓▓▓\n",
    "\n\n ░    ▓▓▓▓▓▓▓▓▓   ░\n░   ▓▓▓▓▓▓▓▓▓▓▓▓▓   ░\n   ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n   ▓▓▓▓░░/░░\\░░▓▓▓\n     ▓▓░░/░░\\░░▓▓\n    ▓▓▓▓▓▓▓▓▓▓▓▓▓▓\n   ▓▓▓ ▓▓▓ ▓▓▓ ▓▓▓▓\n  ▓▓▓ ▓▓▓▓ ▓▓▓▓ ▓▓▓▓\n"
};

int main() {
    srand(time(0));

    string mapa[7][7];
    char mov;
    int an = 0;
    int batalla = 0;

    personajes P, E, E1, E2, S;
    inicializarPersonajes(P, E, S, E1, E2);

    char cargarPartida;
    cout << "¿Cargar partida? (s/n): ";
    cin >> cargarPartida;

    if(cargarPartida == 's') {
        ifstream archivoEntrada("Partida.txt");
        if(archivoEntrada.is_open()) {
            if(!leerPartida(mapa, archivoEntrada, P, E, S, E1, E2)) {
                cout << "Error al leer partida. Iniciando nueva partida..." << endl;
                sleep_for(seconds(2));
                inicializarPersonajes(P, E, S, E1, E2);
            }
            archivoEntrada.close();
        } else {
            cout << "No se encontró archivo de partida. Iniciando nueva partida..." << endl;
            sleep_for(seconds(2));
        }
    }

    do {
        an++;
        system("clear");

        if(batalla == 0) {
            for(int i = 0; i < 7; i++) {
                cout << "\t\t";
                for(int j = 0; j < 7; j++) {
                    mapa[i][j] = "⿴";
                    if(i == P.Y && j == P.X) mapa[i][j] = "◽";
                    if(i == S.Y && j == S.X) mapa[i][j] = "🎰";
                    if(i == E.Y && j == E.X && E.vida > 0) mapa[i][j] = "◾";
                    if(i == E1.Y && j == E1.X && E1.vida > 0) mapa[i][j] = "◾";
                    if(i == E2.Y && j == E2.X && E2.vida > 0) mapa[i][j] = "◾";
                    cout << mapa[i][j];
                }
                cout << endl;
            }
            cout << "\n\t\t     |W.|\n";
            cout << "\t\t|A.| |S.| |D.|\n";

            if(_kbhit()) {
                mov = _getch();
                int newX = P.X, newY = P.Y;

                switch(toupper(mov)) {
                case 'W':
                    newY--;
                    break;
                case 'A':
                    newX--;
                    break;
                case 'S':
                    newY++;
                    break;
                case 'D':
                    newX++;
                    break;
                case 'G':
                    ofstream archivoSalida("Partida.txt");
                    if(archivoSalida.is_open()) {
                        guardarPartida(mapa, archivoSalida, P, E, S, E1, E2);
                        cout << "Partida guardada exitosamente!" << endl;
                        sleep_for(seconds(1));
                    } else {
                        cout << "Error al guardar la partida" << endl;
                        sleep_for(seconds(1));
                    }
                    break;
                }

                if(newX >= 0 && newX < 7 && newY >= 0 && newY < 7) {
                    P.X = newX;
                    P.Y = newY;
                }
            }

            if(P.X == E.X && P.Y == E.Y && E.vida > 0) {
                batalla = 1;
            } else if(P.X == E1.X && P.Y == E1.Y && E1.vida > 0) {
                batalla = 4;
            } else if(P.X == E2.X && P.Y == E2.Y && E2.vida > 0) {
                batalla = 5;
            } else if(P.X == S.X && P.Y == S.Y) {
                batalla = 2;
            }

        } else if(batalla == 1) {
            batallaJugadorEnemigo(P, E, batalla, an, Ue, UE, Eu);
        } else if(batalla == 4) {
            batallaJugadorEnemigoflor(P, E1, batalla, an, flor);
        } else if(batalla == 5) {
            batallaJugadorEnemigogumbat(P, E2, batalla, an, gumbat);
        } else if(batalla == 2) {

            cout << "¿Desea comprar? (S/N): " << endl;

            if(_kbhit()) {
                switch(toupper(_getch())) {
                case 'S':
                    batalla = 3;
                    break;
                case 'N':
                    batalla = 0;
                    break;
                }
            }
        }
        else if(batalla >= 3) {
            comprar(batalla, Rv, an, P);
        }

        if(an % 10 == 0) {
            ofstream archivoSalida("Partida.txt");
            if(archivoSalida.is_open()) {
                guardarPartida(mapa, archivoSalida, P, E, S, E1, E2);
                archivoSalida.close();
            }
        }

        sleep_for(milliseconds(100));

    } while(P.vida > 0);

    cout << "¡Juego terminado!" << endl;
    return 0;
}

void inicializarPersonajes(personajes &P, personajes &E, personajes &S, personajes &E1, personajes &E2) {
    E.nombre = "Calamar Berserk";
    E.vida = 100;
    E.danio = 5 + rand() % 5;
    E.X = rand() % 5 + 1;
    E.Y = rand() % 5 + 1;

    E1.nombre = "Flor de fuego Berserk";
    E1.vida = 100;
    E1.danio = 5 + rand() % 5;
    E1.X = rand() % 5 + 1;
    E1.Y = rand() % 5 + 1;

    E2.nombre = "Gumbat Berserk";
    E2.vida = 100;
    E2.danio = 5 + rand() % 5;
    E2.X = rand() % 5 + 1;
    E2.Y = rand() % 5 + 1;

    P.vida = 100;
    P.mana = 100;
    P.pociones = 0;
    P.dinero = 0;
    P.armas = 0;
    P.danio = 7 + rand() % 5;
    P.X = rand() % 5 + 1;
    P.Y = rand() % 5 + 1;

    S.X = rand() % 5 + 1;
    S.Y = rand() % 5 + 1;
}

void batallaJugadorEnemigo(personajes &P, personajes &E, int &batalla, int &an, const vector<string> &Ue, const vector<string> &UE, const vector<string> &Eu) {
    E.barraV = E.barraVida(E.vida);
    P.barraV = P.barraVida(P.vida);
    P.barraM = P.barraMana(P.mana);

    int x, y = E.vida;

    if(E.vida <= 20) cout << "\033[1;31m" << Ue[an % Ue.size()] << "\033[0m" << endl;
    else if(E.vida <= 40) cout << "\033[31m" << Ue[an % Ue.size()] << "\033[0m" << endl;
    else if(y != x) cout << UE[an % UE.size()] << endl;
    else cout << "\033[1;37m" << Eu[an % Eu.size()] << "\033[0m" << endl;

    x = y;

    cout << E.nombre << endl;
    cout << E.barraV << " " << setfill('0') << setw(3) << E.vida << endl;
    cout << "\n\n\n";
    cout << P.barraM << endl;
    cout << P.barraV << " " << setfill('0') << setw(3) << P.vida << endl;
    cout << "1. Atacar⚔\t2. Pocion⚱︎ (" << P.pociones << ")\n"
         << "Dinero: " << P.dinero << "\033[33m⛁⛀\033[0m" << endl;

    if(_kbhit()) {
        char opcion = _getch();
        if(opcion == '1') {
            E.vida -= P.danio;
            if(E.vida > 0) {
                P.vida -= E.danio;
            }
        } else if(opcion == '2' && P.pociones > 0) {
            P.vida = min(100, P.vida + 30);
            P.pociones--;
        }
    }

    if(E.vida <= 0) {
        P.dinero += (rand() % 10) + 7;
        E.Y = -1;
        E.X = -1;
        batalla = 0;
    } else if(P.vida <= 0) {
        cout << "¡Has sido derrotado!" << endl;
        sleep_for(seconds(2));
        exit(0);
    }

    if(E.vida <= 20) sleep_for(milliseconds(50));
    else if(E.vida <= 40) sleep_for(milliseconds(100));
    else sleep_for(milliseconds(170));
}

void batallaJugadorEnemigoflor(personajes &P, personajes &E1, int &batalla, int &an, const vector<string> &flor) {
    E1.barraV = E1.barraVida(E1.vida);
    P.barraV = P.barraVida(P.vida);
    P.barraM = P.barraMana(P.mana);

    cout << flor[an % flor.size()] << endl;

    cout << E1.nombre << endl;
    cout << E1.barraV << " " << setfill('0') << setw(3) << E1.vida << endl;
    cout << "\n\n\n";
    cout << P.barraM << endl;
    cout << P.barraV << " " << setfill('0') << setw(3) << P.vida << endl;
    cout << "1. Atacar⚔\t2. Pocion⚱︎ (" << P.pociones << ")\n"
         << "Dinero: " << P.dinero << "\033[33m⛁⛀\033[0m" << endl;

    if(_kbhit()) {
        char opcion = _getch();
        if(opcion == '1') {
            E1.vida -= P.danio;
            if(E1.vida > 0) {
                P.vida -= E1.danio;
            }
        } else if(opcion == '2' && P.pociones > 0) {
            P.vida = min(100, P.vida + 30);
            P.pociones--;
        }
    }

    if(E1.vida <= 0) {
        P.dinero += (rand() % 10) + 7;
        E1.Y = -1;
        E1.X = -1;
        batalla = 0;
    } else if(P.vida <= 0) {
        cout << "¡Has sido derrotado!" << endl;
        sleep_for(seconds(2));
        exit(0);
    }

    sleep_for(milliseconds(170));
}

void batallaJugadorEnemigogumbat(personajes &P, personajes &E2, int &batalla, int &an, const vector<string> &gumbat) {
    E2.barraV = E2.barraVida(E2.vida);
    P.barraV = P.barraVida(P.vida);
    P.barraM = P.barraMana(P.mana);

    cout << gumbat[an % gumbat.size()] << endl;

    cout << E2.nombre << endl;
    cout << E2.barraV << " " << setfill('0') << setw(3) << E2.vida << endl;
    cout << "\n\n\n";
    cout << P.barraM << endl;
    cout << P.barraV << " " << setfill('0') << setw(3) << P.vida << endl;
    cout << "1. Atacar⚔\t2. Pocion⚱︎ (" << P.pociones << ")\n"
         << "Dinero: " << P.dinero << "\033[33m⛁⛀\033[0m" << endl;

    if(_kbhit()) {
        char opcion = _getch();
        if(opcion == '1') {
            E2.vida -= P.danio;
            if(E2.vida > 0) {
                P.vida -= E2.danio;
            }
        } else if(opcion == '2' && P.pociones > 0) {
            P.vida = min(100, P.vida + 30);
            P.pociones--;
        }
    }

    if(E2.vida <= 0) {
        P.dinero += (rand() % 10) + 7;
        E2.Y = -1;
        E2.X = -1;
        batalla = 0;
    } else if(P.vida <= 0) {
        cout << "¡Has sido derrotado!" << endl;
        sleep_for(seconds(2));
        exit(0);
    }

    sleep_for(milliseconds(170));
}

void comprar(int &batalla, const vector<string> &Rv, int an, personajes &P) {
    cout << Rv[an % Rv.size()] << endl;
    cout << "Productos:\n1. ⚔ Armas (43⛁⛀)\n2. ⚱︎ Pociones (23⛁⛀)\n3. ↦ Salir\n\n";
    cout << "Dinero: " << P.dinero << "⛁⛀\n";

    if(_kbhit()) {
        switch(_getch()) {
        case '1':
            cout << "¿Cuántas armas desea comprar? (Cada una cuesta 43⛁⛀): ";
            int cantArmas;
            cin >> cantArmas;
            if(cantArmas * 43 <= P.dinero) {
                P.armas += cantArmas;
                P.dinero -= cantArmas * 43;
                cout << "¡Compra exitosa!";
            } else {
                cout << "¡Dinero insuficiente!";
            }
            sleep_for(seconds(1));
            break;
        case '2':
            cout << "¿Cuántas pociones desea comprar? (Cada una cuesta 23⛁⛀): ";
            int cantPociones;
            cin >> cantPociones;
            if(cantPociones * 23 <= P.dinero) {
                P.pociones += cantPociones;
                P.dinero -= cantPociones * 23;
                cout << "¡Compra exitosa!";
            } else {
                cout << "¡Dinero insuficiente!";
            }
            sleep_for(seconds(1));
            break;
        case '3':
            batalla = 0;
            break;
        }
    }
}

void guardarPartida(const string mapa[7][7], ofstream &archivoP, const personajes &P, const personajes &E, const personajes &S, personajes &E1, personajes &E2) {

    for(int i = 0; i < 7; i++) {
        for(int j = 0; j < 7; j++) {
            archivoP << mapa[i][j] << " ";
        }
        archivoP << endl;
    }

    archivoP << P.X << " " << P.Y << " "
             << E.X << " " << E.Y << " "
             << E1.X << " " << E1.Y << " "
             << E2.X << " " << E2.Y << " "
             << S.X << " " << S.Y << " "
             << P.vida << " " << E.vida << " "
             << P.mana << " " << P.dinero << " "
             << P.pociones << " " << P.armas << " "
             << P.danio << endl;
}

bool leerPartida(string (&mapa)[7][7], ifstream &archivoP, personajes &P, personajes &E, personajes &S, personajes &E1, personajes &E2) {

    for(int i = 0; i < 7; i++) {
        for(int j = 0; j < 7; j++) {
            if(!(archivoP >> mapa[i][j])) {
                return false;
            }
        }
    }

    if(!(archivoP >> P.X >> P.Y
            >> E.X >> E.Y
            >> E1.X >> E1.Y
            >> E2.X >> E2.Y
            >> S.X >> S.Y
            >> P.vida >> E.vida
            >> P.mana >> P.dinero
            >> P.pociones >> P.armas
            >> P.danio)) {
        return false;
    }

    return true;
}