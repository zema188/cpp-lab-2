#include <SFML/Graphics.hpp>
#include <iostream>
#include "RClass.h"
#include "imgui-SFML.h"
#include "imgui.h"

float RAnd(float w1, float w2) { return w1 + w2 + std::sqrt((w1 * w1 + w2 * w2) - 2 * w1 * w2); }

float ROr(float w1, float w2) { return w1 + w2 - std::sqrt((w1 * w1 + w2 * w2) - 2 * w1 * w2); }

std::vector<RClass *> shapes;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "lab-2");
	window.setFramerateLimit(60);
	if (!ImGui::SFML::Init(window))
	{
		std::cout << "ImGui initialization failed";
		return -1;
	}

	auto shapeSize = sf::Vector2u{window.getSize().x / 2, window.getSize().y / 2};

	RClass shapeNX;
	shapeNX.Create(shapeSize, 0);
	shapeNX.setPosition(100, 50);
	shapes.push_back(&shapeNX);

	RClass shapeNY;
	shapeNY.Create(shapeSize, 1);
	shapeNY.setPosition(200, 150);
	shapes.push_back(&shapeNY);

	RClass shapeNZ;
	shapeNZ.Create(shapeSize, 2);
	shapeNZ.setPosition(50, 250);
	shapes.push_back(&shapeNZ);

	RClass shapeNW;
	shapeNW.Create(shapeSize, 3);
	shapeNW.setPosition(300, 300);
	shapes.push_back(&shapeNW);

	std::function<float(const sf::Vector2f &)> shapeFunction[5];

	shapeFunction[0] = [](const sf::Vector2f &point) -> float { return std::sin(point.x) + std::cos(point.y); };
	shapeFunction[1] = [](const sf::Vector2f &point) -> float { return std::cos(point.x) * std::sin(point.y); };
	shapeFunction[2] = [](const sf::Vector2f &point) -> float { return std::cos(point.x + point.y); };
	shapeFunction[3] = [](const sf::Vector2f &point) -> float { return point.x * point.x + point.y * point.y - 200; };
	shapeFunction[4] = [](const sf::Vector2f &point) -> float { return std::sin(point.x) * std::cos(point.y); };

	std::function<float(const sf::Vector2f &)> complexFunction = [&shapeFunction](const sf::Vector2f &point) -> float
	{
		return RAnd(RAnd(ROr(RAnd(shapeFunction[0](point), shapeFunction[1](point)), shapeFunction[2](point)),
						 shapeFunction[3](point)),
					ROr(shapeFunction[4](point), shapeFunction[0](point)));
	};

	sf::FloatRect subSpace(-10.f, -10.f, 20.f, 20.f);

	static ImVec4 firstColor(0, 0, 0, 1);
	static ImVec4 secondColor(1, 1, 1, 1);

	for (RClass *shape : shapes)
	{
		shape->DrawRFunc(complexFunction, subSpace);
	}

	sf::Clock deltaClock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Controls");

		if (ImGui::ColorEdit3("First color", &firstColor.x))
		{
		}
		if (ImGui::ColorEdit3("Second color", &secondColor.x))
		{
		}

		if (ImGui::Button("Update"))
		{
			auto sfFirstColor =
				sf::Color(static_cast<sf::Uint8>(firstColor.x * 255), static_cast<sf::Uint8>(firstColor.y * 255),
						  static_cast<sf::Uint8>(firstColor.z * 255), static_cast<sf::Uint8>(firstColor.w * 255));

			auto sfSecondColor =
				sf::Color(static_cast<sf::Uint8>(secondColor.x * 255), static_cast<sf::Uint8>(secondColor.y * 255),
						  static_cast<sf::Uint8>(secondColor.z * 255), static_cast<sf::Uint8>(secondColor.w * 255));

			for (RClass *shape : shapes)
			{
				shape->UpdatePalette(sfFirstColor, sfSecondColor);
			}
		}

		ImGui::Text("Press 'A' to save nx,ny,nz,nw images");

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			shapeNX.SaveImageToFile("images/nx.png");
			shapeNY.SaveImageToFile("images/ny.png");
			shapeNZ.SaveImageToFile("images/nz.png");
			shapeNW.SaveImageToFile("images/nw.png");
		}

		ImGui::End();

		window.clear();

		window.draw(shapeNX);
		window.draw(shapeNY);
		window.draw(shapeNZ);
		window.draw(shapeNW);

		ImGui::SFML::Render(window);

		window.display();
	}

	ImGui::SFML::Shutdown();

	return 0;
}
