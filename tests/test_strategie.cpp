#include <catch2/catch_test_macros.hpp>

#include <string>

#include "Carte.h"
#include "Strategie.h"

static Carte dealer(int valoare) { return Carte(valoare, 0); }

TEST_CASE("Hard 11 dubleaza contra oricarei carti mici", "[strategie]")
{
	REQUIRE(sfatBaza(11, false, dealer(6), true) == Actiune::Double);
	REQUIRE(sfatBaza(11, false, dealer(6), false) == Actiune::Hit);
	REQUIRE(sfatBaza(11, false, dealer(1), true) == Actiune::Hit);
}

TEST_CASE("Hard 12 sta doar contra 4-6", "[strategie]")
{
	REQUIRE(sfatBaza(12, false, dealer(6), true) == Actiune::Stand);
	REQUIRE(sfatBaza(12, false, dealer(3), true) == Actiune::Hit);
}

TEST_CASE("Hard 16 contra 10 cere carte", "[strategie]")
{
	REQUIRE(sfatBaza(16, false, dealer(13), true) == Actiune::Hit); // K = 10
	REQUIRE(sfatBaza(16, false, dealer(6), true) == Actiune::Stand);
}

TEST_CASE("Hard 20 sta mereu", "[strategie]")
{
	REQUIRE(sfatBaza(20, false, dealer(10), true) == Actiune::Stand);
}

TEST_CASE("Hard 10 dubleaza contra 2-9", "[strategie]")
{
	REQUIRE(sfatBaza(10, false, dealer(9), true) == Actiune::Double);
	REQUIRE(sfatBaza(10, false, dealer(10), true) == Actiune::Hit);
}

TEST_CASE("Soft 18 (A,7): stand/double/hit dupa upcard", "[strategie]")
{
	REQUIRE(sfatBaza(18, true, dealer(2), true) == Actiune::Stand);
	REQUIRE(sfatBaza(18, true, dealer(6), true) == Actiune::Double);
	REQUIRE(sfatBaza(18, true, dealer(9), true) == Actiune::Hit);
}

TEST_CASE("Soft 20 sta", "[strategie]")
{
	REQUIRE(sfatBaza(20, true, dealer(6), true) == Actiune::Stand);
}

TEST_CASE("numeActiune returneaza etichete lizibile", "[strategie]")
{
	REQUIRE(std::string(numeActiune(Actiune::Hit)) == "Hit");
	REQUIRE(std::string(numeActiune(Actiune::Stand)) == "Stand");
	REQUIRE(std::string(numeActiune(Actiune::Double)) == "Double");
}
