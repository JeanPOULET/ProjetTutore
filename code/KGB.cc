
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

#include <SFML/Audio.hpp>

#include "config.h"
#include "local/BabyHero.h"
#include "local/Messages.h"
#include "local/ResourceManager.h"
#include "local/Singletons.h"
#include "local/Map.h"
#include "local/Enemy.h"
#include "local/Physics.h"
#include "local/BackgroundMusic.h"

#define FRAME 80.0


int main() {
	
	//Mettre le b2body dans la classe square
	//SetOrigin (rectangleshape, setanchor)
	
	static constexpr gf::Vector2u ScreenSize(1024, 768);
	static constexpr gf::Vector2f ViewSize(2048, 1024); 
	static constexpr gf::Vector2f ViewSize2(240, 240); 
  	static constexpr gf::Vector2f ViewCenter(0, 0); 
	// initialization
	gf::Window window("K.G.B.", ScreenSize);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(FRAME);
	
	gf::Event::MouseButtonEvent button;
	
	gf::RenderWindow renderer(window);
	
	
	gf::SingletonStorage<gf::MessageManager> storageForMessageManager(KGB::gMessageManager);
	gf::SingletonStorage<KGB::ResourceManager> storageForResourceManager(KGB::gResourceManager);
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

	//Sound

	KGB::BackgroundMusic music;

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
	static constexpr gf::Vector2u pos5(32*48, 32*15.45);

	KGB::Enemy Vilain(pos1, KGB::Enemy::PathType::Round, gf::Orientation::South, KGB::Enemy::Status::Walking, 200.0);
	KGB::Enemy Vilain2(pos2, KGB::Enemy::PathType::VerticalLine, gf::Orientation::South, KGB::Enemy::Status::Walking, 700.0);
	KGB::Enemy Vilain3(pos3, KGB::Enemy::PathType::HorizontalLine, gf::Orientation::South, KGB::Enemy::Status::Walking, 500.0);
	KGB::Enemy Vilain4(pos4, KGB::Enemy::PathType::Static, gf::Orientation::West, KGB::Enemy::Status::Waiting, 0.0);
	KGB::Enemy Vilain5(pos5, KGB::Enemy::PathType::Static, gf::Orientation::East, KGB::Enemy::Status::Waiting, 0.0);

	mainEntities.addEntity(Vilain);
	mainEntities.addEntity(Vilain2);
	mainEntities.addEntity(Vilain3);
	mainEntities.addEntity(Vilain4);
	mainEntities.addEntity(Vilain5);

	// controls
	
	gf::ActionContainer introAction;

	gf::Action space("Next");
	space.addScancodeKeyControl(gf::Scancode::Space);
	space.setContinuous();
	introAction.addAction(space);

	gf::Action skip("Skip");
	skip.addScancodeKeyControl(gf::Scancode::P);
	skip.setContinuous();
	introAction.addAction(skip);


	gf::ActionContainer actions;

	gf::Action closeWindowAction("Close window");
	closeWindowAction.addCloseControl();
	closeWindowAction.addKeycodeKeyControl(gf::Keycode::Escape);
	closeWindowAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Back);
	actions.addAction(closeWindowAction);
	introAction.addAction(closeWindowAction);

	gf::Action debugPhysicsAction("Debug Physics");
	debugPhysicsAction.addScancodeKeyControl(gf::Scancode::F10);
	actions.addAction(debugPhysicsAction);

	gf::Action toogleMuteAction("Toggle Mute");
	toogleMuteAction.addKeycodeKeyControl(gf::Keycode::M);
	actions.addAction(toogleMuteAction);

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
	KGB::Physics physics(layers,bebeHero, Vilain, Vilain2, Vilain3, Vilain4, Vilain5);

	// game loop
	gf::Clock clock;
	renderer.clear(gf::Color::White);

	KGB::PhysicsDebugger debug(physics.getWorld());
	mainEntities.addEntity(debug);
	static constexpr float Vitesse = 10.0f;
	gf::Vector2d velocity(0,0);
	
	size_t intro = 0;
	bool spaceisActiveOneTime = true;
	bool debugPhysics = false;
	while(window.isOpen() && intro <= 3){
		
		gf::Event event;
		while (window.pollEvent(event)) {
			introAction.processEvent(event);
			views.processEvent(event);
		}

		if(closeWindowAction.isActive()){
			window.close();
		}

		if(space.isActive()){
			renderer.clear();
			if(spaceisActiveOneTime){
				intro++;
				spaceisActiveOneTime = false;
			}
		}

		if(skip.isActive()){
			intro = 4;
		}

		mainView.setCenter(bebeHero.getPosition());
		renderer.setView(mainView);
		gf::Font font("../data/KGB/Xolonium-Regular.ttf");
		gf::Text text("Hello", font);
		text.setOutlineColor(gf::Color::Black);
		text.setOutlineThickness(2.0f);
		text.setPosition(initialPosition);
		text.setParagraphWidth(1000.0f);
		text.setAlignment(gf::Alignment::Center);
		text.setAnchor(gf::Anchor::Center);

		gf::Text text2("Appuyer sur espace pour voir la suite et sur p pour passer directement au jeu", font);
		text2.setCharacterSize(30);
		text2.setColor(gf::Color::Black);
		text2.setPosition(gf::Vector2f(32*51, 32*20));
		text2.setParagraphWidth(1000.0f);
		text2.setAlignment(gf::Alignment::Center);
		text2.setAnchor(gf::Anchor::Center);
		if(intro == 0){
			text.setCharacterSize(30);
			text.setColor(gf::Color::Red);			
		}else if(intro == 1){
			text.setString("This is");
			text.setCharacterSize(160);
			text.setColor(gf::Color::Blue);
		}else if(intro == 2){
			text.setString("Une intro");
			text.setCharacterSize(60);
			text.setColor(gf::Color::Black);
		}else if(intro == 3){
			text.setString("De petit joueur");
			text.setCharacterSize(40);
			text.setColor(gf::Color::Green);
		}
		renderer.draw(text);
		renderer.draw(text2);
		// draw everything
		renderer.display();
		introAction.reset();
		if(!space.isActive()){
			spaceisActiveOneTime = true;
		}
	}
	mainView.setSize(ViewSize2);
	while (window.isOpen() && intro > 3) {
	
		// 1. input
		gf::Event event;
		while (window.pollEvent(event)) {
			actions.processEvent(event);
			views.processEvent(event);
		}

		if(closeWindowAction.isActive()){
			window.close();
		}

		if (debugPhysicsAction.isActive()) {
			debugPhysics = !debugPhysics;
			debug.setDebug(debugPhysics);
			
    	}
		if (toogleMuteAction.isActive()) {
			music.toggleMute();
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
		Vilain5.render(renderer);
		
		if(debugPhysics){
			debug.render(renderer);
		}
		renderer.setView(hudView);
		// draw everything
		renderer.display();
		actions.reset();
	}
	
	return 0;
}
