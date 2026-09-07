#include <catch2/catch_test_macros.hpp>

#include "Carte.h"
#include "Reguli.h"

TEST_CASE("valoareBlackjack: as = 1, figuri = 10, restul nominal", "[reguli]")
{
	REQUIRE(valoareBlackjack(Carte(1, 0)) == 1);   // As
	REQUIRE(valoareBlackjack(Carte(7, 0)) == 7);
	REQUIRE(valoareBlackjack(Carte(11, 0)) == 10); // J
	REQUIRE(valoareBlackjack(Carte(13, 0)) == 10); // K
}

TEST_CASE("dealerTrebuieSaTraga: trage sub 17, sta de la 17", "[reguli]")
{
	REQUIRE(dealerTrebuieSaTraga(16) == true);
	REQUIRE(dealerTrebuieSaTraga(7) == true);
	REQUIRE(dealerTrebuieSaTraga(17) == false);
	REQUIRE(dealerTrebuieSaTraga(21) == false);
}

TEST_CASE("determinaRezultat acopera toate cazurile", "[reguli]")
{
	REQUIRE(determinaRezultat(22, 18) == Rezultat::JucatorBust);
	REQUIRE(determinaRezultat(20, 23) == Rezultat::DealerBust);
	REQUIRE(determinaRezultat(18, 20) == Rezultat::DealerCastiga);
	REQUIRE(determinaRezultat(20, 18) == Rezultat::JucatorCastiga);
	REQUIRE(determinaRezultat(19, 19) == Rezultat::Egalitate);
}

TEST_CASE("Bust-ul jucatorului are prioritate fata de bust-ul dealerului", "[reguli]")
{
	// Daca ambii au depasit 21, jucatorul pierde (a "spart" primul).
	REQUIRE(determinaRezultat(22, 25) == Rezultat::JucatorBust);
}
