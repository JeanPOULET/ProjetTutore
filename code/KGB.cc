
#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>

#include <gf/EntityContainer.h>
#include <gf/Event.h>
#include <gf/Log.h>
#include <gf/RenderWindow.h>
#include <gf/RenderTarget.h>
#include <gf/Gamepad.h>
#include <gf/Text.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Unused.h>
#include <gf/Window.h>
#include <gf/Sprite.h>
#include <gf/Tmx.h>


#include <iostream>
#include <cassert>
#include <vector>

//#include <SFML/Audio.hpp>

#include "config.h"
#include "local/Square.h"
#include "local/BabyHero.h"
#include "local/Messages.h"
#include "local/Singletons.h"
#include "local/Map.h"
#include "local/Enemy.h"
#include "local/Debug.h"
#include "local/Physics.h"

#define FRAME 80.0


int main() {
	
	//Mettre le b2body dans la classe square
	//SetOrigin (rectangleshape, setanchor)
	
	static constexpr gf::Vector2u ScreenSize(1024, 768);
	static constexpr gf::Vector2f ViewSize(200, 200); 
  	static constexpr gf::Vector2f ViewCenter(0, 0); 
	// initialization
	gf::Window window("K.G.B.", ScreenSize);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(FRAME);
	gf::RenderWindow renderer(window);

	gf::SingletonStorage<gf::MessageManager> storageForMessageManager(KGB::gMessageManager);
	gf::SingletonStorage<gf::ResourceManager> storageForResourceManager(KGB::gResourceManager);
	KGB::gResourceManager().addSearchDir(KGB_DATA_DIR);

	// views
	gf::ViewContainer views;

	gf::ExtendView mainView(ViewCenter, ViewSize);
	views.addView(mainView);

	gf::ScreenView hudView;
	views.addView(hudView);

	views.setInitialScreenSize(ScreenSize);

	// entity
	gf::EntityContainer mainEntities;

	static constexpr gf::Vector2u center(ScreenSize/2);
	KGB::BabyHero bebeHero(center);
	mainEntities.addEntity(bebeHero);


	static constexpr gf::Vector2u zero(0, 0);
	static constexpr gf::Vector2u cinquante(50, 50);
	static constexpr gf::Vector2u troisCent(300, 300);
	static constexpr gf::Vector2u cinqCent(500, 500);

	KGB::Enemy Vilain(zero, KGB::Enemy::PathType::Round, gf::Orientation::South);
	KGB::Enemy Vilain2(troisCent, KGB::Enemy::PathType::VerticalLine, gf::Orientation::South);
	KGB::Enemy Vilain3(cinqCent, KGB::Enemy::PathType::HorizontalLine, gf::Orientation::South);
	KGB::Enemy Vilain4(cinquante);
	mainEntities.addEntity(Vilain);
	mainEntities.addEntity(Vilain2);
	mainEntities.addEntity(Vilain3);
	mainEntities.addEntity(Vilain4);

	std::vector<KGB::Enemy> vilains;
	vilains.push_back(Vilain);
	vilains.push_back(Vilain2);
	//map

	gf::TmxLayers layers;
  	if (!layers.loadFromFile(KGB::gResourceManager().getAbsolutePath("map/garderie.tmx"))) {
    	gf::Log::error("Impossible de charger la carte !\n");
    	return EXIT_FAILURE;
  	}
	KGB::MapGraphicsData data(layers);
  	KGB::Map map( data);

	mainEntities.addEntity(map);

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

	//Physics
	KGB::Physics physics(bebeHero, Vilain, Vilain2, Vilain3, Vilain4);

	// game loop
	gf::Clock clock;
	renderer.clear(gf::Color::White);

	
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
			if(velocity.x > -50){
				velocity.x -= Vitesse;
			}
			bebeHero.updateOrientation(3);
		}else if(rightAction.isActive()){
			if(velocity.x < 50){
				velocity.x += Vitesse;
			}
			bebeHero.updateOrientation(2);
		}else{
			velocity.x = 0;
		}

		if(downAction.isActive()){
			if(velocity.y < 50){
				velocity.y += Vitesse;
			}
			bebeHero.updateOrientation(0);
		}else if(upAction.isActive()){
			if(velocity.y > -50){
				velocity.y -= Vitesse;
			}
			bebeHero.updateOrientation(1);
		}else{
			velocity.y = 0;
			
		}
		
		bebeHero.setVelocity(velocity);

		// 2. update
		
		gf::Time time = clock.restart();
		bebeHero.update(time);
		Vilain.update(time);
		Vilain2.update(time);
		Vilain3.update(time);
		Vilain4.update(time);
		physics.update();

		// 3. draw

		mainView.setCenter(bebeHero.getPosition());
		renderer.clear();
		renderer.setView(mainView);
		map.render(renderer);

		bebeHero.render(renderer);
		Vilain.render(renderer);
		Vilain2.render(renderer);
		Vilain3.render(renderer);
		Vilain4.render(renderer); 

		renderer.setView(hudView);
		// draw everything
		renderer.display();
		actions.reset();
	}
	return 0;
}
