
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
#include <gf/Log.h>

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
	static constexpr gf::Vector2f ViewSize(2048, 1024); 
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

	//map

	gf::TmxLayers layers;
  	if (!layers.loadFromFile(KGB::gResourceManager().getAbsolutePath("map/garderie.tmx"))) {
    	gf::Log::error("Impossible de charger la carte !\n");
    	return EXIT_FAILURE;
  	}
	KGB::MapGraphicsData data(layers);
  	KGB::Map map( data);

	mainEntities.addEntity(map);


	static constexpr gf::Vector2u initialPosition(32*51,32*4);
	KGB::BabyHero bebeHero(initialPosition);
	mainEntities.addEntity(bebeHero);


	static constexpr gf::Vector2u pos1(32*73, 32*4);
	static constexpr gf::Vector2u pos2(32*54, 32*12);
	static constexpr gf::Vector2u pos3(32*45, 32*9);
	static constexpr gf::Vector2u pos4(32*71, 32*22);

	KGB::Enemy Vilain(pos1, KGB::Enemy::PathType::Round, gf::Orientation::South, KGB::Enemy::Status::Walking, 200.0);
	KGB::Enemy Vilain2(pos2, KGB::Enemy::PathType::VerticalLine, gf::Orientation::South, KGB::Enemy::Status::Walking, 700.0);
	KGB::Enemy Vilain3(pos3, KGB::Enemy::PathType::HorizontalLine, gf::Orientation::South, KGB::Enemy::Status::Walking, 500.0);
	KGB::Enemy Vilain4(pos4, KGB::Enemy::PathType::Static, gf::Orientation::East, KGB::Enemy::Status::Waiting, 0.0);
	mainEntities.addEntity(Vilain);
	mainEntities.addEntity(Vilain2);
	mainEntities.addEntity(Vilain3);
	mainEntities.addEntity(Vilain4);

	std::vector<KGB::Enemy> vilains;
	vilains.push_back(Vilain);
	vilains.push_back(Vilain2);


	// controls
	
	gf::ActionContainer actions;

	gf::Action closeWindowAction("Close window");
	closeWindowAction.addCloseControl();
	closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
	closeWindowAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Back);
	actions.addAction(closeWindowAction);

	gf::Action debugPhysicsAction("Debug Physics");
	debugPhysicsAction.addScancodeKeyControl(gf::Scancode::F10);
	debugPhysicsAction.setContinuous();
	actions.addAction(debugPhysicsAction);

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
	KGB::Physics physics(layers,bebeHero, Vilain, Vilain2, Vilain3, Vilain4);

	// game loop
	gf::Clock clock;
	renderer.clear(gf::Color::White);

	
	static constexpr float Vitesse = 10.0f;
	gf::Vector2d velocity(0,0);
	
	/*KGB::Debug debug();
  	mainEntities.addEntity(debug);*/

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

		if(leftAction.isActive() && !downAction.isActive() && !upAction.isActive()){
			if(velocity.x > -50){
				velocity.x -= Vitesse;
			}
			bebeHero.updateOrientation(3);
		}else if(rightAction.isActive()&& !downAction.isActive() && !upAction.isActive()){
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
		mainEntities.update(time);
		physics.update();

		// 3. draw
		//gf::Log::info("Position x : %lf\t Position y :%lf\n",bebeHero.getPosition().u,bebeHero.getPosition().v);
		mainView.setCenter(bebeHero.getPosition());
		renderer.clear();
		renderer.setView(mainView);

		mainEntities.render(renderer);
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
