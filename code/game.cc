
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
#include <gf/Sprite.h>

#include <iostream>
#include <cassert>

//#include <SFML/Audio.hpp>


#include "local/Square.h"
#include "local/Messages.h"
#include "local/Singletons.h"

#include <Box2D/Box2D.h>

#define PHYSICSCALE 0.02f
#define FRAME 80

b2Vec2 fromVec(gf::Vector2f vec) {
      return { vec.x * PHYSICSCALE, vec.y * PHYSICSCALE };
}

gf::Vector2f toVec(b2Vec2 vec) {
      return { vec.x / PHYSICSCALE, vec.y / PHYSICSCALE };
}

int main() {
	
	static constexpr gf::Vector2u ScreenSize(1024, 768);
	static constexpr gf::Vector2f ViewSize(1024, 768); 
  	static constexpr gf::Vector2f ViewCenter(512, 384); 
	// initialization
	gf::Window window("K.G.B.", ScreenSize);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(FRAME);
	gf::RenderWindow renderer(window);

	gf::SingletonStorage<gf::MessageManager> storageForMessageManager(KGB::gMessageManager);


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

	// music 
	/*
	sf::Music music;
	if (!music.openFromFile("../data/Music/theme.ogg")){
    	return -1;
	}
	music.play();*/

	// background

	/*
	gf::Texture texture("../data/Image/maternel.png");
	gf::Sprite coca;
	coca.setTexture(texture);
	coca.setTextureRect(gf::RectF::fromPositionSize({0.1f,0.1f},{0.5f,0.3f}));
	coca.setColor({ 1.0f, 1.0f, 1.0f, 0.8f});
	coca.setPosition({100.0f,25.0f});
	renderer.draw(coca);*/
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
	
	
	//================TEST Box2D================
	
	//World Setting
	b2Vec2 gravity(0, 0);
	
	b2World* m_world = new b2World(gravity);
	
	float32 timeStep = 1/FRAME;      //the length of time passed to simulate (seconds)
  	int32 velocityIterations = 8;   //how strongly to correct velocity
  	int32 positionIterations = 3;   //how strongly to correct position
	
	m_world->Step(timeStep, velocityIterations, positionIterations);

	
	
	//KGB Setting
    static constexpr gf::Vector2f initialPosition(1024 / 2, 768 / 2);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = fromVec(initialPosition);
    auto m_body = m_world->CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(50.0f*PHYSICSCALE, 50.0f*PHYSICSCALE);

    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.shape = &shape;

    m_body->CreateFixture(&fixtureDef);
	
	
	//New Cube
	
	KGB::Square carrini(ScreenSize / 4, 50.0f, gf::Color::Black);
	mainEntities.addEntity(carrini);
	
	static constexpr gf::Vector2f initialPositionCarrini(1024 / 4, 768 / 4);

    b2BodyDef bodyDefCarrini;
    bodyDefCarrini.type = b2_staticBody;
    bodyDefCarrini.position = fromVec(initialPositionCarrini);
    auto carrini_body = m_world->CreateBody(&bodyDefCarrini);

    b2PolygonShape shapeCarrini;
    shapeCarrini.SetAsBox(50.0f*PHYSICSCALE, 50.0f*PHYSICSCALE);

    carrini_body->CreateFixture(&fixtureDef);
	
	
	//================END of TEST Box2D================

	
	static constexpr float Vitesse = 10.0f;
	gf::Vector2d velocity(0,0);
	
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
		carrinou.setVelocity(velocity);
		// 2. update

		
		float dt = clock.restart().asSeconds();
		carrinou.update(dt);
		
		m_body->SetTransform(fromVec(carrinou.getPosition()), 0.0f);
		m_body->SetLinearVelocity(fromVec(velocity));
		
		m_world->Step(timeStep, velocityIterations, positionIterations);
		
		carrinou.setPosition(toVec(m_body->GetPosition()));
		
		// 3. draw
		renderer.clear();
		renderer.setView(mainView);
		carrinou.render(renderer);
		carrini.render(renderer);

		renderer.setView(hudView);
		// draw everything
		renderer.display();
		actions.reset();
	}
	return 0;
}
