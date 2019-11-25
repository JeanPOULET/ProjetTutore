#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Window.h>
int main() {
	static constexpr gf::Vector2u ScreenSize(1024, 768);
	// initialization
	gf::Window window("K.G.B.", ScreenSize);
	gf::RenderWindow renderer(window);
	// game loop
	gf::Clock clock;
	renderer.clear(gf::Color::White);
	while (window.isOpen()) {
		// 1. input
		gf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				case gf::EventType::Closed:
				window.close();
				break;
			case gf::EventType::KeyPressed:
				break;
			default:
				break;
			}
		}
		// 2. update
		float dt = clock.restart().asSeconds();
		// 3. draw
		renderer.clear();
		// draw everything
		renderer.display();
		}
	return 0;
}