#include <catch2/catch_test_macros.hpp>

#include "Carte.h"
#include "Lista.h"

TEST_CASE("Lista operator= handles self-assignment", "[lista]")
{
	Lista l;
	l.addElement(Carte(5, 0));
	l.addElement(Carte(10, 1));

	l = l;

	REQUIRE_FALSE(l.isEmpty());
	REQUIRE(l.getFirst()->getInfo().getValoare() == 5);
	REQUIRE(l.getFirst()->getNext()->getInfo().getValoare() == 10);
	REQUIRE(l.getFirst()->getNext()->getNext() == nullptr);
}

TEST_CASE("Lista operator= copies independently", "[lista]")
{
	Lista a;
	a.addElement(Carte(7, 2));

	Lista b;
	b = a;
	a.clear();

	REQUIRE(a.isEmpty());
	REQUIRE_FALSE(b.isEmpty());
	REQUIRE(b.getFirst()->getInfo().getValoare() == 7);
}
