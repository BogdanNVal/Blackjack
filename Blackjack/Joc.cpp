#include "Joc.h"
#include "Salvare.h"
#include "Reguli.h"
#include "Strategie.h"
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <functional>

#ifdef _WIN32
#include <windows.h>
#endif

using std::cout;
using std::cin;
using std::endl;
using std::setw;
using std::string;

static const int LATIME_MANA = 45;
static const int PAUZA_CARTE_MS = 600;   // pauza intre cartile date, pt. efect de "animatie"

// Opreste executia pentru un scurt timp, ca sa dea senzatia ca dealerul
// (sau jucatorul) trage cartile una cate una, nu toate deodata.
static void asteapta(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// --- Consola portabila ----------------------------------------------------
// Sterge ecranul: pe Windows prin `cls`, pe restul sistemelor prin secventa
// ANSI (evita system() si mesajele "CLS: not found" de pe Linux/macOS).
static void stergeEcran()
{
#ifdef _WIN32
	system("cls");
#else
	cout << "\033[2J\033[H";
	cout.flush();
#endif
}

// Asteapta ca utilizatorul sa apese Enter. Consuma linia curenta ramasa in
// buffer; la EOF (input redirectat/terminat) se intoarce imediat, deci nu
// blocheaza rularile scriptate.
static void asteaptaEnter()
{
	cout << "Apasa Enter pentru a continua...";
	string linie;
	std::getline(cin, linie);
}
// --------------------------------------------------------------------------

// --- Culori consola (Windows) ---------------------------------------------
// Pe alte sisteme (ex. testare pe Linux), functiile nu fac nimic vizibil,
// dar codul tot compileaza si ruleaza normal.
#ifdef _WIN32
static const int CULOARE_IMPLICITA = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
static const int CULOARE_ROSU = FOREGROUND_RED | FOREGROUND_INTENSITY;
static const int CULOARE_VERDE = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
static const int CULOARE_GALBEN = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
static const int CULOARE_GRI = FOREGROUND_INTENSITY;
#else
static const int CULOARE_IMPLICITA = 0, CULOARE_ROSU = 0, CULOARE_VERDE = 0, CULOARE_GALBEN = 0, CULOARE_GRI = 0;
#endif

static void culoare(int cod)
{
#ifdef _WIN32
	static HANDLE hConsola = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsola, (WORD)cod);
#else
	(void)cod;
#endif
}

// Afiseaza un mesaj intr-o culoare, apoi revine la culoarea implicita.
static void afiseazaColorat(const string& text, int cod)
{
	culoare(cod);
	cout << text;
	culoare(CULOARE_IMPLICITA);
}

// O carte rosie (Inima/Diamant) se afiseaza cu rosu, restul cu culoarea implicita.
static void afiseazaCarteColorata(Carte c)
{
	bool rosu = (c.getSimbol() == 1 || c.getSimbol() == 3);
	culoare(rosu ? CULOARE_ROSU : CULOARE_IMPLICITA);
	cout << c;
	culoare(CULOARE_IMPLICITA);
}

// Afiseaza toate cartile dintr-o mana, fiecare in culoarea ei.
static void afiseazaManaColorata(Lista& carti)
{
	Nod* p = carti.getFirst();
	while (p)
	{
		afiseazaCarteColorata(p->getInfo());
		p = p->getNext();
	}
}
// ----------------------------------------------------------------------------

// Reda o mana (Lista de carti) ca string, ca sa poata fi aliniata pe ecran.
static string cartiToString(Lista& carti)
{
	std::ostringstream oss;
	oss << carti;
	return oss.str();
}

// Numarul de carti dintr-o mana.
static int numarCarti(Lista& carti)
{
	int n = 0;
	for (Nod* p = carti.getFirst(); p; p = p->getNext())
		n++;
	return n;
}

// Valoarea de Blackjack a unei singure carti vizibile (asul conteaza 11
// cand e singura carte cunoscuta, fara riscul de a depasi 21).
static int scorCartePartiala(Carte c)
{
	int v = c.getValoare();
	if (v == 1) return 11;
	if (v > 10) return 10;
	return v;
}

// Numara caracterele dintr-un string UTF-8 (nu octetii), ca sa ramana
// corecta alinierea pe coloane cand textul contine simboluri Unicode
// (trefla/inima/pica/romb), care ocupa mai multi octeti dar un singur
// caracter afisat pe ecran.
static int lungimeUtf8(const string& s)
{
	int n = 0;
	for (unsigned char c : s)
		if ((c & 0xC0) != 0x80) // sare peste octetii de continuare UTF-8
			n++;
	return n;
}

// Citeste alegerea jucatorului (1/2/3). La input invalid re-intreaba; la EOF
// intoarce 2 (Stand), ca jocul sa se termine curat in loc sa intre in bucla
// infinita cand input-ul redirectat s-a terminat.
static int citesteAlegere()
{
	int nr;
	while (cin >> nr)
	{
		if (nr == 1 || nr == 2 || nr == 3)
			return nr;
		cout << "Optiune invalida. [1] Hit, [2] Stand, [3] Double: ";
	}
	return 2;
}

// Afiseaza o mana (eticheta + carti colorate) cu scorul aliniat in partea
// dreapta. 'textPtruLungime' e versiunea simpla (necolorata) a cartilor,
// folosita doar ca sa se calculeze corect padding-ul; 'printCarti' e ce
// se afiseaza efectiv (poate fi colorat).
static void afiseazaMana(const string& eticheta, const string& textPtruLungime,
	const std::function<void()>& printCarti, const string& scor)
{
	int lungime = lungimeUtf8(eticheta) + lungimeUtf8(textPtruLungime);

	cout << eticheta;
	printCarti();

	int padding = LATIME_MANA - lungime;
	cout << string(padding > 0 ? padding : 1, ' ');
	cout << "Scor: " << scor << endl;
}

// Afiseaza banii jucatorului si ambele maini (jucator + dealer).
// Daca dealerVizibilComplet e false, a doua carte a dealerului ramane
// ascunsa si se arata doar scorul partial calculat din cartea vizibila.
// Functioneaza corect si in timpul animatiei de impartire, cand dealerul
// inca nu are nicio carte sau are doar prima.
static void afiseazaStare(Jucator& jucator, bool dealerVizibilComplet, Jucator& dealer)
{
	cout << "Banii: " << jucator.getBanii() << endl << endl;

	afiseazaMana(string(jucator.getNume()) + ": ", cartiToString(jucator.getCarti()),
		[&]() { afiseazaManaColorata(jucator.getCarti()); },
		jucator.getCarti().getFirst() ? std::to_string(jucator.getScor()) : string("-"));

	Nod* p = dealer.getCarti().getFirst();

	if (dealerVizibilComplet)
	{
		afiseazaMana("Dealer: ", cartiToString(dealer.getCarti()),
			[&]() { afiseazaManaColorata(dealer.getCarti()); },
			std::to_string(dealer.getScor()));
	}
	else if (!p)
	{
		// dealerul inca nu a primit nicio carte (animatie de impartire in curs)
		afiseazaMana("Dealer: ", "", []() {}, "-");
	}
	else if (!p->getNext())
	{
		// dealerul are doar prima carte (vizibila); a doua nu a fost inca data
		std::ostringstream carti;
		carti << p->getInfo();
		Carte prima = p->getInfo();
		afiseazaMana("Dealer: ", carti.str(),
			[=]() { afiseazaCarteColorata(prima); },
			std::to_string(scorCartePartiala(prima)) + "+?");
	}
	else
	{
		std::ostringstream carti;
		carti << p->getInfo() << "?? ";
		Carte prima = p->getInfo();
		afiseazaMana("Dealer: ", carti.str(),
			[=]() { afiseazaCarteColorata(prima); afiseazaColorat("?? ", CULOARE_GRI); },
			std::to_string(scorCartePartiala(prima)) + "+?");
	}

	cout << endl;
}


void meniuPrincipal()
{
	int alegere;
	do
	{
		cout << "=== BLACKJACK ===\n";
		cout << "[1] Jucator nou\n";
		cout << "[2] Continua cu un jucator existent\n";
		cout << "[3] Iesire\n";
		cout << "Alegere: ";
		if (!(cin >> alegere)) return;
	} while (alegere != 1 && alegere != 2 && alegere != 3);

	stergeEcran();

	if (alegere == 1)
	{
		char nume[50];
		cout << "Nume jucator nou: ";
		if (!(cin >> setw(50) >> nume)) return;

		Jucator jucator(nume);
		stergeEcran();
		ruleazaJoc(jucator);
	}
	else if (alegere == 2)
	{
		if (!existaJucatoriSalvati())
		{
			cout << "Nu exista niciun jucator salvat inca.\n\n";
			asteaptaEnter();
			stergeEcran();
			meniuPrincipal();
			return;
		}

		afiseazaJucatoriSalvati();
		char nume[50];
		cout << "\nNume jucator: ";
		if (!(cin >> setw(50) >> nume)) return;

		Jucator jucator;
		if (incarcaJucator(nume, jucator))
		{
			stergeEcran();
			ruleazaJoc(jucator);
		}
		else
		{
			cout << "\nNu am gasit un jucator cu acest nume.\n\n";
			asteaptaEnter();
			stergeEcran();
			meniuPrincipal();
		}
	}
	// alegere == 3: iesire, main() se termina
}

void ruleazaJoc(Jucator& jucator)
{
	Pachet pachet;
	Jucator dealer("Dealer", 0);

	bool continua = true;

	while (continua)
	{
		if (jucator.getBanii() <= 0)
		{
			cout << "Nu mai aveti bani. Jocul se incheie pentru " << jucator.getNume() << ".\n";
			break;
		}

		jucator.getCarti().clear();
		dealer.getCarti().clear();
		jucator.setScor(0);
		dealer.setScor(0);

		start(jucator, pachet, dealer);

		cout << "\nBanii tai: " << jucator.getBanii() << "\n";
		cout << "Mai joci o runda? [1] Da, [2] Nu: ";
		int r;
		if (!(cin >> r)) break;
		continua = (r == 1);
		stergeEcran();
	}

	salveazaJucator(jucator);
	cout << "Progresul lui " << jucator.getNume() << " a fost salvat (Banii: " << jucator.getBanii() << ").\n";
	cout << "La revedere!\n";
}

void start(Jucator& jucator, Pachet& pachet, Jucator& dealer)
{
	pachet.creeaza();
	pachet.amesteca();
	cout << jucator;
	cout << endl; cout << endl;
	cout << "Ce suma doriti sa pariati: ";
	int s;
	if (!(cin >> s)) return;
	while (s <= 0 || s > jucator.getBanii())
	{
		cout << "Suma invalida. Alege alta suma: ";
		if (!(cin >> s)) return;
	}
	jucator.Bet(s);
	stergeEcran();

	// Se trag cartile pe rand (jucator, dealer, jucator, dealer), cu o mica
	// pauza dupa fiecare, ca sa para ca sunt date una cate una, nu instant.
	jucator.primeste(pachet.trage());
	stergeEcran();
	afiseazaStare(jucator, false, dealer);
	asteapta(PAUZA_CARTE_MS);

	dealer.primeste(pachet.trage());
	stergeEcran();
	afiseazaStare(jucator, false, dealer);
	asteapta(PAUZA_CARTE_MS);

	jucator.primeste(pachet.trage());
	stergeEcran();
	afiseazaStare(jucator, false, dealer);
	asteapta(PAUZA_CARTE_MS);

	dealer.primeste(pachet.trage());
	stergeEcran();
	afiseazaStare(jucator, false, dealer);

	asteaptaEnter();

	verificare(jucator, pachet, dealer);
}

void runda(Jucator& jucator, Pachet& pachet, Jucator& dealer)
{
	stergeEcran();
	afiseazaStare(jucator, false, dealer);

	// Sfat de strategie de baza, calculat din cartea vizibila a dealerului.
	Carte cartaDealer = dealer.getCarti().getFirst()->getInfo();
	bool poateDubla = (numarCarti(jucator.getCarti()) == 2) &&
		(jucator.getSuma_pariata() <= jucator.getBanii());
	Actiune sugestie = sfatBaza(jucator.getScor(),
		esteManaSoft(jucator.getCarti()), cartaDealer, poateDubla);
	cout << "Sugestie (strategie de baza): " << numeActiune(sugestie) << "\n";

	cout << "[1] Hit, [2] Stand, [3] Double\n ";
	int a = citesteAlegere();
	alegere(jucator, a, pachet, dealer);
}

void runda_dealer(Jucator& jucator, Pachet& pachet, Jucator& dealer)
{
	if (jucator.getScor() > 21)
	{
		stergeEcran();
		afiseazaStare(jucator, true, dealer);
		afiseazaColorat("Ati pierdut (Bust)\n", CULOARE_ROSU);
		return;
	}

	// Dealerul isi arata mai intai cartea ascunsa, cu o pauza scurta
	stergeEcran();
	afiseazaStare(jucator, true, dealer);
	asteapta(PAUZA_CARTE_MS);

	while (dealerTrebuieSaTraga(dealer.getScor()))
	{
		dealer.primeste(pachet.trage());

		stergeEcran();
		afiseazaStare(jucator, true, dealer);
		asteapta(PAUZA_CARTE_MS);
	}

	switch (determinaRezultat(jucator.getScor(), dealer.getScor()))
	{
	case Rezultat::DealerBust:
		afiseazaColorat("Dealer Bust! Ati castigat!\n", CULOARE_VERDE);
		win(jucator);
		break;
	case Rezultat::JucatorCastiga:
		afiseazaColorat("Ati castigat!\n", CULOARE_VERDE);
		win(jucator);
		break;
	case Rezultat::DealerCastiga:
		afiseazaColorat("Dealer castiga.\n", CULOARE_ROSU);
		break;
	case Rezultat::Egalitate:
		afiseazaColorat("Push! (egalitate)\n", CULOARE_GALBEN);
		draw(jucator);
		break;
	case Rezultat::JucatorBust:
		afiseazaColorat("Ati pierdut (Bust)\n", CULOARE_ROSU);
		break;
	}
}


void verificare(Jucator& jucator, Pachet& pachet, Jucator& dealer)
{
	if (jucator.getScor() > 21)
	{
		afiseazaColorat("Bust\n", CULOARE_ROSU);
		runda_dealer(jucator, pachet, dealer);
	}
	if (jucator.getScor() == 21)
	{
		afiseazaColorat("Blackjack\n", CULOARE_VERDE);
		runda_dealer(jucator, pachet, dealer);
	}
	if (jucator.getScor() < 21)
	{
		runda(jucator, pachet, dealer);
	}
}


void alegere(Jucator& jucator, int nr, Pachet& pachet, Jucator& dealer)
{
	if (nr == 1)
	{
		jucator.primeste(pachet.trage());
		cout << "Trageti o carte...\n";
		asteapta(PAUZA_CARTE_MS);
		verificare(jucator, pachet, dealer);
	}
	else if (nr == 2)
	{
		runda_dealer(jucator, pachet, dealer);
	}
	else if (nr == 3)
	{
		if (jucator.dubleazaMiza())
		{
			jucator.primeste(pachet.trage());
			cout << "Trageti o carte...\n";
			asteapta(PAUZA_CARTE_MS);
			runda_dealer(jucator, pachet, dealer);
		}
		else
		{
			cout << "Nu aveti suficienti bani pentru Double.\n";
			asteapta(PAUZA_CARTE_MS);
			runda(jucator, pachet, dealer);
		}
	}
}


void win(Jucator& jucator)
{
	jucator.setBanii(jucator.getBanii() + 2 * jucator.getSuma_pariata());
}

void draw(Jucator& jucator)
{
	jucator.setBanii(jucator.getBanii() + jucator.getSuma_pariata());
}
