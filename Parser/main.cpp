#include <iostream>
#include <fstream>
#include <string>
#include <regex>			// C++11 ���� ǥ������ ����ϱ� ���� ���	<ex) ���ڿ��� ����ǥ���� ����>
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
	std::regex pattern("\"([^\"]*)\"");		// ���ڿ��� ����ǥ���� ����
	// (x) = ��ȣ�� ���� ����
	// [^\"] = \"�� �����ϰ� ã��
	// x* = 0ȸ �̻� �ݺ�
	// ��, pattern = " ( "�� �����ϰ� ã�� ������ 0ȸ �̻� �ݺ� ) "


	while (!file.eof())
	{
		std::getline(file, line);
		auto result = line.find("<sprite");	// find()�� �ش� ���ڿ��� ���� ��ġ�� ��ȯ
		if (result != std::string::npos)	// npos = -1�� ���� ������ ��� = find() �Լ� ���� ��, ã�� ���ڿ��� ���� �� ��ȯ
		{				
			// sregex_iterator = Ž���� ����� ���� �κ��������� ������ �����ϴ� �����̳�
			std::sregex_iterator current(line.begin(), line.end(), pattern);	// [0]�� ���� ���ڿ�, [1]�� ���Ͽ� �´� ���ڿ�
			std::sregex_iterator end;	// �� = end
			int index{ 0 };
			Sprite sprite;

			while (current != end)
			{
				std::string token = (*current)[1];
				switch (index)
				{
					case 0:
						// "n = �̸�"
						sprite.n = token;
						break;
					case 1:
						// "x = ��ġX"
						sprite.x = std::stoi(token);
						break;
					case 2:
						// "y = ��ġY"
						sprite.y = std::stoi(token);
						break;
					case 3:
						// "w = �ʺ�"
						sprite.w = std::stoi(token);
						break;
					case 4:
						// "h = ����"
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