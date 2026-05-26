#ifndef PRIORITY_QUEUE_ARRAY_HPP
#define PRIORITY_QUEUE_ARRAY_HPP

#include <iostream>
#include <vector>
#include <stdexcept>

class PriorityQueue_Array
{
private:
    struct Element
    {
        int wartosc;
        int priorytet;
    };

    std::vector<Element> dane;

public:
    void push(int wartosc, int priorytet)
    {
        Element nowy_element{wartosc, priorytet};

        auto aktualny = dane.begin();

        while(aktualny != dane.end() && aktualny->priorytet <= priorytet)
        {
            aktualny++;
        }

        dane.insert(aktualny, nowy_element);
    }

    int pop()
    {
        if(dane.empty())
        {
            throw std::runtime_error("Kolejka jest pusta!");
        }

        int wartosc = dane.front().wartosc;
        dane.erase(dane.begin());

        return wartosc;
    }

    int peek() const
    {
        if(dane.empty())
        {
            throw std::runtime_error("Kolejka jest pusta!");
        }

        return dane.front().wartosc;
    }

    void changePriority(int wartosc, int nowy_priorytet)
    {
        for(auto aktualny = dane.begin(); aktualny != dane.end(); aktualny++)
        {
            if(aktualny->wartosc == wartosc)
            {
                dane.erase(aktualny);
                push(wartosc, nowy_priorytet);

                return;
            }
        }
    }

    int size() const
    {
        return dane.size();
    }

    void print() const
    {
        std::cout << "Kolejka [wartosc(priorytet)]: ";

        for(const auto& element : dane)
        {
            std::cout << element.wartosc << "(" << element.priorytet << ") -> ";
        }

        std::cout << "NULL\n";
    }
};

#endif
