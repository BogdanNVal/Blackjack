#include "Lista.h"




Nod::Nod() {
	next = 0;
}
Nod::Nod(Carte info, Nod* next) {
	this->info = info;
	this->next = next;
}
Nod::~Nod() {}
Carte Nod::getInfo() {
	
	return info;
}
void Nod::setInfo(Carte info) {
	this->info = info;
}
Nod* Nod::getNext() {
	return next;
}
void Nod::setNext(Nod* next) {
	this->next = next;
}


/*clasa ListaStudenti*/
Lista::Lista() {
	primul = 0;
}

Lista::Lista(const Lista& list) {
	Nod* p;
	primul = 0;
	p = list.primul;
	while (p) {
		addElement(p->info);
		p = p->next;
	}
}

Lista::~Lista() {
	clear();
}
int Lista::isEmpty() {
	return primul == 0;
}
void Lista::clear() {
	Nod* p, * q;
	p = primul;
	while (p) {
		q = p;
		p = p->next;
		delete q;
	}
	primul = 0;
}
void Lista::addElement(Carte info) {
	Nod* p;
	p = new Nod;
	p->info = info;
	p->next = 0;
	if (isEmpty()) {
	 primul = p;
	}
	else {
		Nod* q = primul;
		while (q->next)
			q = q->next;
		q->next = p;
	}
}
Nod* Lista::getFirst() {
	return primul;
}




std::ostream& operator<<(std::ostream& out, const Lista& list) {
	Nod* p=list.primul;
	
	while (p) {
		
		out << p->getInfo();
		p = p->getNext();
	}
	return out;
}

Lista& Lista::operator=(const Lista& list) {
	clear();
	Nod* p = list.primul;
	while (p) {
		addElement(p->info);
		p = p->next;
	}
	return *this;
}

