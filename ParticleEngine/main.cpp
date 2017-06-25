#include <SFML/Graphics.hpp>
#include "ParticleEngine.h"
#include "Config.hpp"

void main()
{
	sf::ContextSettings settings;
	settings.majorVersion = 4;
	settings.minorVersion = 4;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(Config::width, Config::height), "Particle Engine", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(Config::useVsync);
	window.setFramerateLimit(60u);

	sf::Clock frameTimer;
	float fpsDisplayDelay = 0.0f;
	ParticleEngine engine;

	while (window.isOpen())
	{
		//GPU Kick
		window.clear();
		engine.Render(window);

		float deltaTime = frameTimer.restart().asSeconds(); 
		fpsDisplayDelay += deltaTime;
		deltaTime = std::min(deltaTime, 0.1f);

		//FPS Counter
		if (fpsDisplayDelay > 0.5f)
		{
			int fps = static_cast<int>(1.0f / deltaTime);
			window.setTitle("Particle Engine FPS: " + std::to_string(fps));
			fpsDisplayDelay = 0.0f;
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			//Window Closed
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			//Button Input
			if(event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Key::Escape)
				{
					window.close();
				}
			}
		}

		engine.Update(deltaTime);


		//Display
		window.display();
	}
}
