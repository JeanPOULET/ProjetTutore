#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Window.h>
#include <gf/Action.h>
#include <gf/EntityContainer.h>
#include "local/Square.h"

int main() {
	static constexpr gf::Vector2u ScreenSize(1024, 768);
	// initialization
	gf::Window window("K.G.B.", ScreenSize);
	gf::RenderWindow renderer(window);

	// entity
	gf::EntityContainer mainEntities;

	KGB::Square carrinou(ScreenSize / 2, 80.0f, gf::Color::Black);
	mainEntities.addEntity(carrinou);

	// game loop
	gf::Clock clock;
	renderer.clear(gf::Color::White);

	static constexpr float Vitesse = 160.0f;
	gf::Vector2d velocity(0,0);

	// controls

	gf::ActionContainer actions;

	gf::Action closeWindowAction("Close window");
	closeWindowAction.addCloseControl();
	closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
	closeWindowAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Back);
	actions.addAction(closeWindowAction);

	gf::Action leftAction("Left");
	leftAction.addScancodeKeyControl(gf::Scancode::Q);
	leftAction.addScancodeKeyControl(gf::Scancode::Left);
	leftAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::RightX, gf::GamepadAxisDirection::Negative);
	leftAction.setContinuous();
	actions.addAction(leftAction);

	gf::Action rightAction("Right");
	rightAction.addScancodeKeyControl(gf::Scancode::D);
	rightAction.addScancodeKeyControl(gf::Scancode::Right);
	rightAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::RightX, gf::GamepadAxisDirection::Positive);
	rightAction.setContinuous();
	actions.addAction(rightAction);

	gf::Action upAction("Up");
	upAction.addScancodeKeyControl(gf::Scancode::Z);
	upAction.addScancodeKeyControl(gf::Scancode::Up);
	upAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Negative);
	upAction.setContinuous();
	actions.addAction(upAction);

	gf::Action downAction("Down");
	downAction.addScancodeKeyControl(gf::Scancode::S);
	downAction.addScancodeKeyControl(gf::Scancode::Down);
	downAction.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Positive);
	downAction.setContinuous();
	actions.addAction(downAction);

<<<<<<< HEAD

||||||| merged common ancestors
	//entities


=======
>>>>>>> dd9c9c3ba9441472537cf62a718fe78f57e9bf90
	while (window.isOpen()) {
		// 1. input
		gf::Event event;
		while (window.pollEvent(event)) {
			actions.processEvent(event);
		}

		if(closeWindowAction.isActive()){
			window.close();
		}

		if(leftAction.isActive()){
			velocity.x -= Vitesse;
		}else if(rightAction.isActive()){
			velocity.x += Vitesse;
		}else{
			velocity.x = 0;
		}

		if(downAction.isActive()){
			velocity.y += Vitesse;
		}else if(upAction.isActive()){
			velocity.y -= Vitesse;
		}else{
			velocity.y = 0;
		}

		// 2. update
		carrinou.setVelocity(velocity);
		float dt = clock.restart().asSeconds();
		carrinou.update(dt);

		// 3. draw
		renderer.clear();
		carrinou.render(renderer);
		// draw everything
		renderer.display();
		actions.reset();
	}
	return 0;
}