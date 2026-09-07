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
#include <limits>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

using std::cout;
using std::cin;
using std::endl;
using std::setw;
using std::string;

static const int LATIME_MANA = 45;
static const int PAUZA_CARTE_MS = 600;

static void asteapta(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

static void stergeEcran()
{
#ifdef _WIN32
	system("cls");
#else
	cout << "\033[2J\033[H";
	cout.flush();
#endif
}

// Discard the leftover newline (and anything else) after `cin >> ...`
// so a following getline actually waits for the user.
static void ignoraRestulLiniei()
{
	if (!cin)
		return;
	cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}

// Wait for Enter; return immediately on EOF so scripted runs do not hang.
static void asteaptaEnter()
{
	cout << "Apasa Enter pentru a continua...";
	cout.flush();
	cin.clear();
	string linie;
	std::getline(cin, linie);
}

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

static void afiseazaColorat(const string& text, int cod)
{
	culoare(cod);
	cout << text;
	culoare(CULOARE_IMPLICITA);
}

static void afiseazaCarteColorata(Carte c)
{
	bool rosu = (c.getSimbol() == 1 || c.getSimbol() == 3);
	culoare(rosu ? CULOARE_ROSU : CULOARE_IMPLICITA);
	cout << c;
	culoare(CULOARE_IMPLICITA);
}

static void afiseazaManaColorata(Lista& carti)
{
	Nod* p = carti.getFirst();
	while (p)
	{
		afiseazaCarteColorata(p->getInfo());
		p = p->getNext();
	}
}

static string cartiToString(Lista& carti)
{
	std::ostringstream oss;
	oss << carti;
	return oss.str();
}

static int numarCarti(Lista& carti)
{
	int n = 0;
	for (Nod* p = carti.getFirst(); p; p = p->getNext())
		n++;
	return n;
}

static int scorCartePartiala(Carte c)
{
	int v = c.getValoare();
	if (v == 1) return 11;
	if (v > 10) return 10;
	return v;
}

// Count Unicode characters, not bytes — suit symbols are multi-byte UTF-8.
static int lungimeUtf8(const string& s)
{
	int n = 0;
	for (unsigned char c : s)
		if ((c & 0xC0) != 0x80)
			n++;
	return n;
}

// Option 3 (Double) only when allowed. On EOF return Stand so redirected input ends cleanly.
static int citesteAlegere(bool permiteDouble)
{
	int nr;
	while (cin >> nr)
	{
		ignoraRestulLiniei();
		if (nr == 1 || nr == 2 || (nr == 3 && permiteDouble))
			return nr;
		if (permiteDouble)
			cout << "Optiune invalida. [1] Hit, [2] Stand, [3] Double: ";
		else
			cout << "Optiune invalida. [1] Hit, [2] Stand: ";
	}
	return 2;
}

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
		afiseazaMana("Dealer: ", "", []() {}, "-");
	}
	else if (!p->getNext())
	{
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
		ignoraRestulLiniei();
	} while (alegere != 1 && alegere != 2 && alegere != 3);

	stergeEcran();

	if (alegere == 1)
	{
		char nume[50];
		cout << "Nume jucator nou: ";
		if (!(cin >> setw(50) >> nume)) return;
		ignoraRestulLiniei();

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
		ignoraRestulLiniei();

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
		ignoraRestulLiniei();
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
	ignoraRestulLiniei();
	while (s <= 0 || s > jucator.getBanii())
	{
		cout << "Suma invalida. Alege alta suma: ";
		if (!(cin >> s)) return;
		ignoraRestulLiniei();
	}
	jucator.Bet(s);
	stergeEcran();

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

	// Natural blackjack resolves immediately (player natural pays 3:2; both = push).
	bool jucatorNatural = esteBlackjackNatural(jucator.getCarti());
	bool dealerNatural = esteBlackjackNatural(dealer.getCarti());
	if (jucatorNatural || dealerNatural)
	{
		stergeEcran();
		afiseazaStare(jucator, true, dealer);
		if (jucatorNatural && dealerNatural)
		{
			afiseazaColorat("Ambii aveti Blackjack. Push!\n", CULOARE_GALBEN);
			draw(jucator);
		}
		else if (jucatorNatural)
		{
			afiseazaColorat("Blackjack! Platit 3:2!\n", CULOARE_VERDE);
			winBlackjack(jucator);
		}
		else
		{
			afiseazaColorat("Dealerul are Blackjack. Ati pierdut.\n", CULOARE_ROSU);
		}
		return;
	}

	verificare(jucator, pachet, dealer);
}

void runda(Jucator& jucator, Pachet& pachet, Jucator& dealer)
{
	stergeEcran();
	afiseazaStare(jucator, false, dealer);

	Carte cartaDealer = dealer.getCarti().getFirst()->getInfo();
	bool poateDubla = (numarCarti(jucator.getCarti()) == 2) &&
		(jucator.getSuma_pariata() <= jucator.getBanii());
	Actiune sugestie = sfatBaza(jucator.getScor(),
		esteManaSoft(jucator.getCarti()), cartaDealer, poateDubla);
	cout << "Sugestie (strategie de baza): " << numeActiune(sugestie) << "\n";

	if (poateDubla)
		cout << "[1] Hit, [2] Stand, [3] Double\n ";
	else
		cout << "[1] Hit, [2] Stand\n ";
	int a = citesteAlegere(poateDubla);
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
		runda_dealer(jucator, pachet, dealer);
	}
	else if (jucator.getScor() == 21)
	{
		afiseazaColorat("21!\n", CULOARE_VERDE);
		runda_dealer(jucator, pachet, dealer);
	}
	else
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

// Natural blackjack pays 3:2 (stake back + 1.5x, floored for odd bets).
void winBlackjack(Jucator& jucator)
{
	int miza = jucator.getSuma_pariata();
	jucator.setBanii(jucator.getBanii() + miza + (miza * 3) / 2);
}

void draw(Jucator& jucator)
{
	jucator.setBanii(jucator.getBanii() + jucator.getSuma_pariata());
}
