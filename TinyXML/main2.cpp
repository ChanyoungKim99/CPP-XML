#include "Monster.h"
#include "tinyxml2.h"

using namespace tinyxml2;

bool SaveToXML(const char* filename, const std::vector<Monster>& monsters)
{
	XMLDocument doc;

	XMLDeclaration* decl = doc.NewDeclaration(
		R"(xml version="1.0" encoding="EUC-KR")"		// UTF-8�� �⺻������, �츮�� ����ϴ� ���ڿ� �ý����� �����ڵ� ���ڵ� ���x
	);													// ���ô��� ���������� std::string�� ����� ���������� �ѱ� ǥ���� ����

	doc.LinkEndChild(decl);					// ����� decl�� doc�� ����

	XMLElement* pRootNode = doc.NewElement("monsters");

	for (auto monster : monsters)
	{
		auto pMonsterNode = doc.NewElement("monster");
		pMonsterNode->SetAttribute("name", monster.GetName().c_str());

		auto pStatusNode = doc.NewElement("status");
		Status status = monster.GetStatus();
		pStatusNode->SetAttribute("level", status.mLevel);
		pStatusNode->SetAttribute("hp", status.mHP);
		pStatusNode->SetAttribute("mp", status.mMP);
		pMonsterNode->LinkEndChild(pStatusNode);		// pStatusNode�� pMonsterNode�� ����

		auto pItemsNode = doc.NewElement("items");
		for (auto item : monster.GetDropItems())
		{
			auto pItemNode = doc.NewElement("item");
			pItemNode->SetAttribute("name", item.mName.c_str());
			pItemNode->SetAttribute("gold", item.mGold);
			pItemsNode->LinkEndChild(pItemNode);		// pItemNode�� pItemsNode�� ����
		}
		pMonsterNode->LinkEndChild(pItemsNode);			// pItemsNode�� pMonsterNode�� ����
		pRootNode->LinkEndChild(pMonsterNode);			// pMonsterNode�� pRootNode�� ����
	}

	doc.LinkEndChild(pRootNode);			// pRootNode�� doc�� ����
	return (doc.SaveFile(filename) == XML_SUCCESS);
}

bool LoadFromXML(const char* filename, std::vector<Monster>& monsters)
{
	XMLDocument doc;
	if (doc.LoadFile(filename) != XML_SUCCESS)
	{
		return false;
	}

	auto pRootNode = doc.FirstChildElement("monsters");			// Document���� tag�� "monsters"�� ���� ù��° �ڽ� ���Ҹ� ã�ƿ�

	for (auto pMonsterNode = pRootNode->FirstChildElement();    // pRootNode�� ù��° �ڽ� ��带 ������ (�ʱ�ȭ��)
		pMonsterNode;											// ���� ��尡 nullptr���� Ȯ��			 (���ǽ�)
		pMonsterNode = pMonsterNode->NextSiblingElement())		// ���� ���̳��� �̵�					 (������)
	{
		Monster monster;

		monster.SetName(pMonsterNode->Attribute("name"));		// Setter�� ����

		auto pStatusNode = pMonsterNode->FirstChildElement("status");
		Status status;
		status.mLevel = pStatusNode->IntAttribute("level");
		status.mHP = pStatusNode->IntAttribute("hp");
		status.mMP = pStatusNode->IntAttribute("mp");	
		monster.SetStatus(status);								// Setter�� ����

		auto pItemsNode = pMonsterNode->FirstChildElement("items");
		for (auto pItemNode = pItemsNode->FirstChildElement();
			pItemNode;
			pItemNode = pItemNode->NextSiblingElement())
		{
			Item item;
			item.mName = pItemNode->Attribute("name");
			item.mGold = pItemNode->IntAttribute("gold");
			monster.AddDropItem(item);
		}
		monsters.push_back(monster);
	}
	return true;
}

int main()
{
	std::vector<Monster> monsters;
	Monster monster;
	monster.SetName("������");
	monster.SetStatus(Status{ 1,1,1 });
	monster.AddDropItem(Item{ "������ ����", 1 });
	monsters.push_back(monster);
	monster.GetDropItems().clear();
	monster.SetName("�����ΰ�");
	monster.SetStatus(Status{ 5,5,5 });
	monster.AddDropItem(Item{ "����", 2 });
	monster.AddDropItem(Item{ "���밡��", 5 });
	monsters.push_back(monster);
	monster.GetDropItems().clear();
	monster.SetName("�Ǹ�");
	monster.SetStatus(Status{ 10,10,10 });
	monster.AddDropItem(Item{ "����", 10 });
	monster.AddDropItem(Item{ "����", 5 });
	monsters.push_back(monster);
	SaveToXML("Data/monsters.xml", monsters);
	monsters.clear();
	LoadFromXML("Data/monsters.xml", monsters);
}