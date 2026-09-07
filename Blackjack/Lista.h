#pragma once
#include "Carte.h"


class Nod {
	Carte info;
	Nod* next;
public:
	Nod();
	Nod(Carte info, Nod* next = 0);
	~Nod();
	Carte getInfo();
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
	Lista();
	Lista(const Lista&);
	~Lista();
	int isEmpty();
	void clear();
	void addElement(Carte info);
	Nod* getFirst();

	friend std::ostream& operator<<(std::ostream&, const Lista&);
	Lista& operator=(const Lista& list);

	friend class Carte;
};
