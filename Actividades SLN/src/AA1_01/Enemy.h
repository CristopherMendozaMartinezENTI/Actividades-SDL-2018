#pragma once
#include "../../dep/inc/XML/rapidxml.hpp"
#include "../../dep/inc/XML/rapidxml_iterators.hpp"
#include "../../dep/inc/XML/rapidxml_print.hpp"
#include "../../dep/inc/XML/rapidxml_utils.hpp"
#include "../../res/file/config.xml"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

class Enemy
{
public:
	struct Weapon {
		std::string weaponName;
		std::string weaponType;
		int range;
	};
	std::string enemyName;
	int vitality;
	int damage;
	int defense;
	int atackFrequency;
	int exp;
	std::vector<Weapon> weaponList;
};

