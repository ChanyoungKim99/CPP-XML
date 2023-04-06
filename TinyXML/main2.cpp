#include "Monster.h"
#include "tinyxml2.h"

using namespace tinyxml2;

bool SaveToXML(const char* filename, const std::vector<Monster>& monsters)
{
	XMLDocument doc;

	XMLDeclaration* decl = doc.NewDeclaration(
		R"(xml version="1.0" encoding="EUC-KR")"		// UTF-8이 기본이지만, 우리가 사용하는 문자열 시스템은 유니코드 인코딩 사용x
	);													// 구시대의 유물이지만 std::string을 사용한 예제에서는 한글 표현이 쉽다

	doc.LinkEndChild(decl);					// 선언부 decl를 doc에 붙임

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
		pMonsterNode->LinkEndChild(pStatusNode);		// pStatusNode를 pMonsterNode에 연결

		auto pItemsNode = doc.NewElement("items");
		for (auto item : monster.GetDropItems())
		{
			auto pItemNode = doc.NewElement("item");
			pItemNode->SetAttribute("name", item.mName.c_str());
			pItemNode->SetAttribute("gold", item.mGold);
			pItemsNode->LinkEndChild(pItemNode);		// pItemNode를 pItemsNode에 연결
		}
		pMonsterNode->LinkEndChild(pItemsNode);			// pItemsNode를 pMonsterNode에 연결
		pRootNode->LinkEndChild(pMonsterNode);			// pMonsterNode를 pRootNode에 연결
	}

	doc.LinkEndChild(pRootNode);			// pRootNode를 doc에 붙임
	return (doc.SaveFile(filename) == XML_SUCCESS);
}

bool LoadFromXML(const char* filename, std::vector<Monster>& monsters)
{
	XMLDocument doc;
	if (doc.LoadFile(filename) != XML_SUCCESS)
	{
		return false;
	}

	auto pRootNode = doc.FirstChildElement("monsters");			// Document에서 tag가 "monsters"인 가장 첫번째 자식 원소를 찾아옴

	for (auto pMonsterNode = pRootNode->FirstChildElement();    // pRootNode의 첫번째 자식 노드를 가져옴 (초기화식)
		pMonsterNode;											// 현재 노드가 nullptr인지 확인			 (조건식)
		pMonsterNode = pMonsterNode->NextSiblingElement())		// 다음 사촌노드로 이동					 (증감식)
	{
		Monster monster;

		monster.SetName(pMonsterNode->Attribute("name"));		// Setter로 설정

		auto pStatusNode = pMonsterNode->FirstChildElement("status");
		Status status;
		status.mLevel = pStatusNode->IntAttribute("level");
		status.mHP = pStatusNode->IntAttribute("hp");
		status.mMP = pStatusNode->IntAttribute("mp");	
		monster.SetStatus(status);								// Setter로 설정

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
	monster.SetName("슬라임");
	monster.SetStatus(Status{ 1,1,1 });
	monster.AddDropItem(Item{ "끈적한 젤리", 1 });
	monsters.push_back(monster);
	monster.GetDropItems().clear();
	monster.SetName("늑대인간");
	monster.SetStatus(Status{ 5,5,5 });
	monster.AddDropItem(Item{ "발톱", 2 });
	monster.AddDropItem(Item{ "늑대가죽", 5 });
	monsters.push_back(monster);
	monster.GetDropItems().clear();
	monster.SetName("악마");
	monster.SetStatus(Status{ 10,10,10 });
	monster.AddDropItem(Item{ "날개", 10 });
	monster.AddDropItem(Item{ "손톱", 5 });
	monsters.push_back(monster);
	SaveToXML("Data/monsters.xml", monsters);
	monsters.clear();
	LoadFromXML("Data/monsters.xml", monsters);
}