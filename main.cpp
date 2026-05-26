#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <utility>
#include <iomanip>
#include <string>

#include "PriorityQueue_SinglyLinkedList.hpp"
#include "PriorityQueue_DoublyLinkedList.hpp"
#include "PriorityQueue_Array.hpp"

using namespace std;
using namespace chrono;

const int liczba_prob = 7;
const int liczba_powtorzen = 1000000;
const int min_rozmiar = 5000;
const int max_rozmiar = 50000;
const int krok_rozmiaru = 5000;
const int maks_priorytet = 100;

volatile long long kontrola_wyniku = 0;

void zapisz_naglowek(ofstream& plik, const string& jednostka)
{
    plik << "n;Singly(" << jednostka << ");Doubly(" << jednostka << ");Array(" << jednostka << ")\n";
}

void zapisz_wynik_mikrosekundy(
    ofstream& plik,
    int rozmiar,
    long long czas_lista_jednokierunkowa,
    long long czas_lista_dwukierunkowa,
    long long czas_tablica
)
{
    plik << rozmiar << ";"
         << czas_lista_jednokierunkowa << ";"
         << czas_lista_dwukierunkowa << ";"
         << czas_tablica << "\n";
}

void zapisz_wynik_nanosekundy(
    ofstream& plik,
    int rozmiar,
    double czas_lista_jednokierunkowa,
    double czas_lista_dwukierunkowa,
    double czas_tablica
)
{
    plik << fixed << setprecision(3);

    plik << rozmiar << ";"
         << czas_lista_jednokierunkowa << ";"
         << czas_lista_dwukierunkowa << ";"
         << czas_tablica << "\n";
}

long long policz_mediane(vector<long long> wyniki)
{
    sort(wyniki.begin(), wyniki.end());

    return wyniki[wyniki.size() / 2];
}

double policz_mediane_double(vector<double> wyniki)
{
    sort(wyniki.begin(), wyniki.end());

    return wyniki[wyniki.size() / 2];
}

vector<pair<int, int>> wygeneruj_dane_losowe(int rozmiar)
{
    vector<pair<int, int>> dane;
    dane.reserve(rozmiar);

    for(int i = 0; i < rozmiar; i++)
    {
        dane.push_back({rand(), rand() % maks_priorytet});
    }

    return dane;
}

vector<pair<int, int>> wygeneruj_dane_z_id(int rozmiar)
{
    vector<pair<int, int>> dane;
    dane.reserve(rozmiar);

    for(int i = 0; i < rozmiar; i++)
    {
        dane.push_back({i, rand() % maks_priorytet});
    }

    return dane;
}

vector<int> wygeneruj_nowe_priorytety(int rozmiar)
{
    vector<int> priorytety;
    priorytety.reserve(rozmiar);

    for(int i = 0; i < rozmiar; i++)
    {
        priorytety.push_back(rand() % maks_priorytet);
    }

    return priorytety;
}

template <typename Kolejka>
void wypelnij_kolejke(Kolejka& kolejka, const vector<pair<int, int>>& dane)
{
    for(const auto& element : dane)
    {
        kolejka.push(element.first, element.second);
    }
}

template <typename Kolejka>
long long zmierz_dodawanie(const vector<pair<int, int>>& dane)
{
    Kolejka kolejka;

    auto start = high_resolution_clock::now();

    wypelnij_kolejke(kolejka, dane);

    auto koniec = high_resolution_clock::now();

    return duration_cast<microseconds>(koniec - start).count();
}

template <typename Kolejka>
long long zmierz_usuwanie(const vector<pair<int, int>>& dane)
{
    Kolejka kolejka;
    wypelnij_kolejke(kolejka, dane);

    auto start = high_resolution_clock::now();

    for(int i = 0; i < static_cast<int>(dane.size()); i++)
    {
        kolejka.pop();
    }

    auto koniec = high_resolution_clock::now();

    return duration_cast<microseconds>(koniec - start).count();
}

template <typename Kolejka>
long long zmierz_zmiane_priorytetu(
    const vector<pair<int, int>>& dane,
    const vector<int>& nowe_priorytety
)
{
    Kolejka kolejka;
    wypelnij_kolejke(kolejka, dane);

    auto start = high_resolution_clock::now();

    for(int i = 0; i < static_cast<int>(dane.size()); i++)
    {
        kolejka.changePriority(dane[i].first, nowe_priorytety[i]);
    }

    auto koniec = high_resolution_clock::now();

    return duration_cast<microseconds>(koniec - start).count();
}

template <typename Kolejka>
double zmierz_peek(Kolejka& kolejka)
{
    auto start = high_resolution_clock::now();

    for(int i = 0; i < liczba_powtorzen; i++)
    {
        kontrola_wyniku += kolejka.peek();
    }

    auto koniec = high_resolution_clock::now();

    return static_cast<double>(duration_cast<nanoseconds>(koniec - start).count()) / liczba_powtorzen;
}

template <typename Kolejka>
double zmierz_size(Kolejka& kolejka)
{
    auto start = high_resolution_clock::now();

    for(int i = 0; i < liczba_powtorzen; i++)
    {
        kontrola_wyniku += kolejka.size();
    }

    auto koniec = high_resolution_clock::now();

    return static_cast<double>(duration_cast<nanoseconds>(koniec - start).count()) / liczba_powtorzen;
}

int main()
{
    srand(time(nullptr));

    ofstream plik_dodawanie("push.csv");
    ofstream plik_usuwanie("pop.csv");
    ofstream plik_zmiana_priorytetu("changePriority.csv");
    ofstream plik_peek("peek.csv");
    ofstream plik_size("size.csv");

    zapisz_naglowek(plik_dodawanie, "us");
    zapisz_naglowek(plik_usuwanie, "us");
    zapisz_naglowek(plik_zmiana_priorytetu, "us");
    zapisz_naglowek(plik_peek, "ns");
    zapisz_naglowek(plik_size, "ns");

    for(int rozmiar = min_rozmiar; rozmiar <= max_rozmiar; rozmiar += krok_rozmiaru)
    {
        vector<long long> wyniki_dodawanie_lista_jednokierunkowa;
        vector<long long> wyniki_dodawanie_lista_dwukierunkowa;
        vector<long long> wyniki_dodawanie_tablica;

        vector<long long> wyniki_usuwanie_lista_jednokierunkowa;
        vector<long long> wyniki_usuwanie_lista_dwukierunkowa;
        vector<long long> wyniki_usuwanie_tablica;

        vector<long long> wyniki_zmiana_lista_jednokierunkowa;
        vector<long long> wyniki_zmiana_lista_dwukierunkowa;
        vector<long long> wyniki_zmiana_tablica;

        vector<double> wyniki_peek_lista_jednokierunkowa;
        vector<double> wyniki_peek_lista_dwukierunkowa;
        vector<double> wyniki_peek_tablica;

        vector<double> wyniki_size_lista_jednokierunkowa;
        vector<double> wyniki_size_lista_dwukierunkowa;
        vector<double> wyniki_size_tablica;

        for(int proba = 0; proba < liczba_prob; proba++)
        {
            vector<pair<int, int>> dane_losowe = wygeneruj_dane_losowe(rozmiar);

            wyniki_dodawanie_lista_jednokierunkowa.push_back(
                zmierz_dodawanie<PriorityQueue_SinglyLinkedList>(dane_losowe)
            );

            wyniki_dodawanie_lista_dwukierunkowa.push_back(
                zmierz_dodawanie<PriorityQueue_DoublyLinkedList>(dane_losowe)
            );

            wyniki_dodawanie_tablica.push_back(
                zmierz_dodawanie<PriorityQueue_Array>(dane_losowe)
            );

            wyniki_usuwanie_lista_jednokierunkowa.push_back(
                zmierz_usuwanie<PriorityQueue_SinglyLinkedList>(dane_losowe)
            );

            wyniki_usuwanie_lista_dwukierunkowa.push_back(
                zmierz_usuwanie<PriorityQueue_DoublyLinkedList>(dane_losowe)
            );

            wyniki_usuwanie_tablica.push_back(
                zmierz_usuwanie<PriorityQueue_Array>(dane_losowe)
            );

            vector<pair<int, int>> dane_z_id = wygeneruj_dane_z_id(rozmiar);
            vector<int> nowe_priorytety = wygeneruj_nowe_priorytety(rozmiar);

            wyniki_zmiana_lista_jednokierunkowa.push_back(
                zmierz_zmiane_priorytetu<PriorityQueue_SinglyLinkedList>(dane_z_id, nowe_priorytety)
            );

            wyniki_zmiana_lista_dwukierunkowa.push_back(
                zmierz_zmiane_priorytetu<PriorityQueue_DoublyLinkedList>(dane_z_id, nowe_priorytety)
            );

            wyniki_zmiana_tablica.push_back(
                zmierz_zmiane_priorytetu<PriorityQueue_Array>(dane_z_id, nowe_priorytety)
            );

            PriorityQueue_SinglyLinkedList lista_jednokierunkowa;
            PriorityQueue_DoublyLinkedList lista_dwukierunkowa;
            PriorityQueue_Array tablica;

            wypelnij_kolejke(lista_jednokierunkowa, dane_z_id);
            wypelnij_kolejke(lista_dwukierunkowa, dane_z_id);
            wypelnij_kolejke(tablica, dane_z_id);

            wyniki_peek_lista_jednokierunkowa.push_back(zmierz_peek(lista_jednokierunkowa));
            wyniki_peek_lista_dwukierunkowa.push_back(zmierz_peek(lista_dwukierunkowa));
            wyniki_peek_tablica.push_back(zmierz_peek(tablica));

            wyniki_size_lista_jednokierunkowa.push_back(zmierz_size(lista_jednokierunkowa));
            wyniki_size_lista_dwukierunkowa.push_back(zmierz_size(lista_dwukierunkowa));
            wyniki_size_tablica.push_back(zmierz_size(tablica));
        }

        zapisz_wynik_mikrosekundy(
            plik_dodawanie,
            rozmiar,
            policz_mediane(wyniki_dodawanie_lista_jednokierunkowa),
            policz_mediane(wyniki_dodawanie_lista_dwukierunkowa),
            policz_mediane(wyniki_dodawanie_tablica)
        );

        zapisz_wynik_mikrosekundy(
            plik_usuwanie,
            rozmiar,
            policz_mediane(wyniki_usuwanie_lista_jednokierunkowa),
            policz_mediane(wyniki_usuwanie_lista_dwukierunkowa),
            policz_mediane(wyniki_usuwanie_tablica)
        );

        zapisz_wynik_mikrosekundy(
            plik_zmiana_priorytetu,
            rozmiar,
            policz_mediane(wyniki_zmiana_lista_jednokierunkowa),
            policz_mediane(wyniki_zmiana_lista_dwukierunkowa),
            policz_mediane(wyniki_zmiana_tablica)
        );

        zapisz_wynik_nanosekundy(
            plik_peek,
            rozmiar,
            policz_mediane_double(wyniki_peek_lista_jednokierunkowa),
            policz_mediane_double(wyniki_peek_lista_dwukierunkowa),
            policz_mediane_double(wyniki_peek_tablica)
        );

        zapisz_wynik_nanosekundy(
            plik_size,
            rozmiar,
            policz_mediane_double(wyniki_size_lista_jednokierunkowa),
            policz_mediane_double(wyniki_size_lista_dwukierunkowa),
            policz_mediane_double(wyniki_size_tablica)
        );

        cout << "Zakonczono test dla n = " << rozmiar << endl;
    }

    plik_dodawanie.close();
    plik_usuwanie.close();
    plik_zmiana_priorytetu.close();
    plik_peek.close();
    plik_size.close();

    cout << "Wszystkie testy zakonczone!" << endl;

    return 0;
}