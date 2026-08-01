#pragma once
#include "Jucator.h"

// Salveaza/actualizeaza jucatorul (nume + banii) in fisierul de salvare.
void salveazaJucator(Jucator& jucator);

// Incarca un jucator existent dupa nume. Intoarce true si populeaza
// 'jucator' daca a fost gasit, altfel intoarce false.
bool incarcaJucator(const char* nume, Jucator& jucator);

// Afiseaza pe ecran lista jucatorilor salvati si banii lor.
void afiseazaJucatoriSalvati();

// Intoarce true daca exista cel putin un jucator salvat.
bool existaJucatoriSalvati();
