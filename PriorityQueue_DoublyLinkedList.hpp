#ifndef PRIORITY_QUEUE_DOUBLY_LINKED_LIST_HPP
#define PRIORITY_QUEUE_DOUBLY_LINKED_LIST_HPP

#include <iostream>
#include <stdexcept>

class PriorityQueue_DoublyLinkedList
{
private:
    struct Node
    {
        int wartosc;
        int priorytet;
        Node* nastepny;
        Node* poprzedni;

        Node(int wartosc, int priorytet)
            : wartosc(wartosc),
              priorytet(priorytet),
              nastepny(nullptr),
              poprzedni(nullptr)
        {
        }
    };

    Node* glowa;
    int licznik;

public:
    PriorityQueue_DoublyLinkedList()
        : glowa(nullptr),
          licznik(0)
    {
    }

    ~PriorityQueue_DoublyLinkedList()
    {
        while(glowa != nullptr)
        {
            Node* do_usuniecia = glowa;
            glowa = glowa->nastepny;
            delete do_usuniecia;
        }
    }

    void push(int wartosc, int priorytet)
    {
        Node* nowy_wezel = new Node(wartosc, priorytet);

        if(glowa == nullptr || priorytet < glowa->priorytet)
        {
            nowy_wezel->nastepny = glowa;

            if(glowa != nullptr)
            {
                glowa->poprzedni = nowy_wezel;
            }

            glowa = nowy_wezel;
        }
        else
        {
            Node* aktualny = glowa;

            while(aktualny->nastepny != nullptr && aktualny->nastepny->priorytet <= priorytet)
            {
                aktualny = aktualny->nastepny;
            }

            nowy_wezel->nastepny = aktualny->nastepny;
            nowy_wezel->poprzedni = aktualny;

            if(aktualny->nastepny != nullptr)
            {
                aktualny->nastepny->poprzedni = nowy_wezel;
            }

            aktualny->nastepny = nowy_wezel;
        }

        licznik++;
    }

    int pop()
    {
        if(glowa == nullptr)
        {
            throw std::runtime_error("Kolejka jest pusta!");
        }

        int wartosc = glowa->wartosc;
        Node* do_usuniecia = glowa;

        glowa = glowa->nastepny;

        if(glowa != nullptr)
        {
            glowa->poprzedni = nullptr;
        }

        delete do_usuniecia;
        licznik--;

        return wartosc;
    }

    int peek() const
    {
        if(glowa == nullptr)
        {
            throw std::runtime_error("Kolejka jest pusta!");
        }

        return glowa->wartosc;
    }

    void changePriority(int wartosc, int nowy_priorytet)
    {
        Node* aktualny = glowa;

        while(aktualny != nullptr)
        {
            if(aktualny->wartosc == wartosc)
            {
                if(aktualny->poprzedni != nullptr)
                {
                    aktualny->poprzedni->nastepny = aktualny->nastepny;
                }
                else
                {
                    glowa = aktualny->nastepny;
                }

                if(aktualny->nastepny != nullptr)
                {
                    aktualny->nastepny->poprzedni = aktualny->poprzedni;
                }

                delete aktualny;
                licznik--;

                push(wartosc, nowy_priorytet);

                return;
            }

            aktualny = aktualny->nastepny;
        }
    }

    int size() const
    {
        return licznik;
    }

    void print() const
    {
        Node* aktualny = glowa;

        std::cout << "Kolejka [wartosc(priorytet)]: ";

        while(aktualny != nullptr)
        {
            std::cout << aktualny->wartosc << "(" << aktualny->priorytet << ") <-> ";
            aktualny = aktualny->nastepny;
        }

        std::cout << "NULL\n";
    }
};

#endif
