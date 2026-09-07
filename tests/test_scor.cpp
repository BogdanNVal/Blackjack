#include <catch2/catch_test_macros.hpp>

#include "Carte.h"
#include "Lista.h"
#include "Reguli.h"

// Helper: construieste o mana din valori de carti (simbolul nu conteaza la scor).
static Lista mana(std::initializer_list<int> valori)
{
	Lista l;
	for (int v : valori)
		l.addElement(Carte(v, 0));
	return l;
}

TEST_CASE("Blackjack natural: as + figura = 21", "[scor]")
{
	Lista l = mana({ 1, 13 }); // A + K
	REQUIRE(calculeazaScor(l) == 21);
}

TEST_CASE("Doi asi = 12 (unul 11, unul 1)", "[scor]")
{
	Lista l = mana({ 1, 1 });
	REQUIRE(calculeazaScor(l) == 12);
}

TEST_CASE("As folosit ca 11 cand incape", "[scor]")
{
	Lista l = mana({ 1, 1, 9 }); // 11 + 1 + 9 = 21
	REQUIRE(calculeazaScor(l) == 21);
}

TEST_CASE("As retrogradat la 1 ca sa evite bust", "[scor]")
{
	Lista l = mana({ 1, 9, 13 }); // A(1) + 9 + K(10) = 20
	REQUIRE(calculeazaScor(l) == 20);
}

TEST_CASE("Figurile valoreaza 10", "[scor]")
{
	Lista l = mana({ 11, 12, 13 }); // J + Q + K = 30
	REQUIRE(calculeazaScor(l) == 30);
}

TEST_CASE("Carti numerice se aduna normal", "[scor]")
{
	Lista l = mana({ 5, 6 });
	REQUIRE(calculeazaScor(l) == 11);
}

TEST_CASE("esteManaSoft detecteaza asul numarat ca 11", "[scor]")
{
	Lista soft = mana({ 1, 6 }); // A,6 = soft 17
	Lista hard = mana({ 10, 6 }); // 16 hard
	Lista degradat = mana({ 1, 9, 13 }); // asul devine 1 -> hard 20

	REQUIRE(esteManaSoft(soft) == true);
	REQUIRE(esteManaSoft(hard) == false);
	REQUIRE(esteManaSoft(degradat) == false);
}
