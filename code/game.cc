
#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>

#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Gamepad.h>
#include <gf/Text.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Unused.h>
#include <gf/Window.h>

#include <iostream>
#include <cassert>

#include "local/Square.h"
#include "local/Messages.h"
#include "local/Singletons.h"

int main() {
	static constexpr gf::Vector2u ScreenSize(1024, 768);
	static constexpr gf::Vector2f ViewSize(800.0f, 800.0f); 
  	static constexpr gf::Vector2f ViewCenter(0, 0); 
	// initialization
	gf::Window window("K.G.B.", ScreenSize);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(80);
	gf::RenderWindow renderer(window);


	// views
	gf::ViewContainer views;
	gf::ExtendView mainView(ViewCenter, ViewSize);
	views.addView(mainView);

	gf::ScreenView hudView;
	views.addView(hudView);

	views.setInitialScreenSize(ScreenSize);

	KGB::gMessageManager().registerHandler<KGB::SquareState>([&mainView](gf::Id type, gf::Message *msg){
		assert(type == KGB::SquareState::type);
		gf::unused(type);
		auto state = static_cast<KGB::SquareState*>(msg);
		mainView.setCenter(state->position);
		return gf::MessageStatus::Keep;
	});


	// entity
	gf::EntityContainer mainEntities;

	KGB::Square carrinou(ScreenSize / 2, 50.0f, gf::Color::Black);
	mainEntities.addEntity(carrinou);

<<<<<<< HEAD
=======
	// game loop
	gf::Clock clock;
	renderer.clear(gf::Color::White);

	static constexpr float Vitesse = 10.0f;
	gf::Vector2d velocity(0,0);

>>>>>>> 76752da0653708d3cf1185d2ce87f7726578511e
	// controls

	gf::ActionContainer actions;

	gf::Action closeWindowAction("Close window");
	closeWindowAction.addCloseControl();
	closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
	closeWindowAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Back);
	actions.addAction(closeWindowAction);

	gf::Action leftAction("Left");
	leftAction.addScancodeKeyControl(gf::Scancode::A);
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
	upAction.addScancodeKeyControl(gf::Scancode::W);
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

	// game loop
	gf::Clock clock;
	renderer.clear(gf::Color::White);

	static constexpr float Vitesse = 10.0f;
	gf::Vector2d velocity(0,0);

	home::gMessageManager().registerHandler<home::HeroPosition>([&mainView](gf::Id type, gf::Message *msg) {
		assert(type == home::HeroPosition::type);
		auto hero = static_cast<home::HeroPosition*>(msg);
		mainView.setCenter(hero->position);
		return gf::MessageStatus::Keep;
	});
	while (window.isOpen()) {
		// 1. input
		gf::Event event;
		while (window.pollEvent(event)) {
			actions.processEvent(event);
			views.processEvent(event);
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
		renderer.setView(mainView);
		carrinou.render(renderer);

		renderer.setView(hudView);
		// draw everything
		renderer.display();
		actions.reset();
	}
	return 0;
}