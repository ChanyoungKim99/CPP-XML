#include <iostream>
#include <fstream>
#include <string>
#include <regex>			// C++11 정규 표현식을 사용하기 위한 헤더	<ex) 문자열로 정규표현식 생성>
#include <vector>

class Sprite
{
public:
	std::string n;
	int x;
	int y;
	int w;
	int h;
};

void LoadXML(const char* filename, std::vector<Sprite>& sprites)
{
	sprites.clear();

	std::ifstream file(filename, std::ifstream::binary);
	std::string line;
	std::regex pattern("\"([^\"]*)\"");		// 문자열로 정규표현식 생성
	// (x) = 괄호로 묶인 패턴
	// [^\"] = \"를 제외하고 찾음
	// x* = 0회 이상 반복
	// 즉, pattern = " ( "를 제외하고 찾는 행위를 0회 이상 반복 ) "


	while (!file.eof())
	{
		std::getline(file, line);
		auto result = line.find("<sprite");	// find()가 해당 문자열의 시작 위치를 반환
		if (result != std::string::npos)	// npos = -1의 값을 가지는 상수 = find() 함수 수행 시, 찾는 문자열이 없을 때 반환
		{				
			// sregex_iterator = 탐색한 결과를 여러 부분집합으로 저장해 관리하는 컨테이너
			std::sregex_iterator current(line.begin(), line.end(), pattern);	// [0]에 원본 문자열, [1]에 패턴에 맞는 문자열
			std::sregex_iterator end;	// 값 = end
			int index{ 0 };
			Sprite sprite;

			while (current != end)
			{
				std::string token = (*current)[1];
				switch (index)
				{
					case 0:
						// "n = 이름"
						sprite.n = token;
						break;
					case 1:
						// "x = 위치X"
						sprite.x = std::stoi(token);
						break;
					case 2:
						// "y = 위치Y"
						sprite.y = std::stoi(token);
						break;
					case 3:
						// "w = 너비"
						sprite.w = std::stoi(token);
						break;
					case 4:
						// "h = 높이"
						sprite.h = std::stoi(token);
						break;
				}

				index++;
				current++;
			}

			if (index > 4)
			{
				sprites.push_back(sprite);
			}
		}
	}
}

int main()
{
	std::vector<Sprite> mySprites;
	
	LoadXML("mydata.xml", mySprites);

	for (auto elem : mySprites)
	{
		std::cout <<
			elem.n << " : " <<
			elem.x << ", " << elem.y << ", " <<
			elem.w << ", " << elem.h << std::endl;
	}
}