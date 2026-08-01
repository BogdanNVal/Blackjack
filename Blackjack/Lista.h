#pragma once
#include "Carte.h"


class Nod {
	Carte info; //informatia
	Nod* next;//legatura catre urmatorul nod din lista
public:
	Nod();//Constructor Implicit
	Nod(Carte info, Nod* next = 0);//Constructor de initializare
	~Nod();
	Carte getInfo();//
	void setInfo(Carte info);
	Nod* getNext();
	void setNext(Nod* next);

	friend class Lista;
	friend class Carte;

};

class Lista{
protected:
	Nod* primul;
public:
	Lista(); //constructor implicit
	Lista(const Lista&);
	~Lista(); //destructor
	int isEmpty();
	void clear();
	void addElement(Carte info);//adauga un nod la sfarsitul listei
	Nod* getFirst();//intoarce primul elemnt din lista

	friend std::ostream& operator<<(std::ostream&, const Lista&);
	Lista& operator=(const Lista& list);

	friend class Carte;
};
