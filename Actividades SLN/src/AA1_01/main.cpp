#include "Enemy.h"


void enemyListReader(std::vector<Enemy> &enemyVec)
{
	rapidxml::xml_document<> doc;
	std::ifstream file("../ ../res/file/config.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);

	int i = 0;
	rapidxml::xml_node<> *pRoot = doc.first_node()->first_node();
	while (pRoot = nullptr)
	{
		Enemy newEnemy;
		newEnemy.enemyName = std::stoi(pRoot->first_node("name")->first_attribute()->value());
		newEnemy.vitality = std::stoi(pRoot->first_node("vitality")->first_attribute()->value());
		newEnemy.damage = std::stoi(pRoot->first_node("damage")->first_attribute()->value());
		newEnemy.defense = std::stoi(pRoot->first_node("defense")->first_attribute()->value());
		newEnemy.atackFrequency = std::stoi(pRoot->first_node("attackfrequency")->first_attribute()->value());
		newEnemy.exp = std::stoi(pRoot->first_node("exp")->first_attribute()->value());
		enemyVec.push_back(newEnemy);
		pRoot = pRoot->next_sibling();
	}
}

int main()
{
	std::vector<Enemy> enemyList;
	enemyListReader(enemyList);
	return 0;
}

