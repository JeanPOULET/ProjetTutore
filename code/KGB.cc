
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

enum GameState{
	INTRO,
	PLAYING,
	VICTORY,
	GAMEOVER
};

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


	//Messages handlers

	GameState state = GameState::INTRO;

	KGB::gMessageManager().registerHandler<KGB::GameOver>([&state](gf::Id type, gf::Message *msg) {
		assert(type == KGB::GameOver::type);
		gf::unused(type, msg);
		state = GameState::GAMEOVER;
		gf::Log::debug("Je me meursssss");
		return gf::MessageStatus::Die;
  	});

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

	gf::Vector2u introPos(32*51,32*32);
	mainView.setCenter(introPos);
	renderer.setView(mainView);

	//Initialisation pour intro
	size_t intro = 0;
	size_t i = 0;
	size_t varX = 22;
	size_t varY = 20;
	size_t drawRect = 0;
	bool spaceisActiveOneTime = true;

	gf::RectangleShape rectangle({ 32*60, 32*8 });
	rectangle.setPosition(gf::Vector2f(32*51, 32*21));
	rectangle.setOutlineColor(gf::Color::Black);
	rectangle.setOutlineThickness(5);
	rectangle.setAnchor(gf::Anchor::Center);

	gf::Font font("../data/KGB/Pokemon_Classic.ttf");

	gf::Text text2("Appuyer sur espace pour voir la suite et sur p pour passer directement au jeu", font);
	text2.setCharacterSize(30);
	text2.setColor(gf::Color::Black);
	text2.setPosition(gf::Vector2f(32*51, 32*50));
	text2.setParagraphWidth(1000.0f);
	text2.setAlignment(gf::Alignment::Center);
	text2.setAnchor(gf::Anchor::Center);

	//Mode débug actif ou non
	bool debugPhysics = false;

	while (window.isOpen()) {
		if(state == GameState::INTRO){
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
				i = 0;
				varX = 22;
				varY = 20;
				drawRect = 0;
				if(spaceisActiveOneTime){
					intro++;
					spaceisActiveOneTime = false;
				}
			}

			if(skip.isActive()){
				intro = 4;
			}
			
			std::string str;
			std::string str2;	

			gf::Text text;

			gf::Vector2u textPosition(32*varX,32*(varY));

			text.setFont(font);
			text.setPosition(textPosition);
			text.setAlignment(gf::Alignment::Center);
			text.setAnchor(gf::Anchor::BottomRight);
			text.setCharacterSize(30);
			text.setColor(gf::Color::Black);

			varX++;
			if(i == 57){
				varY+=2;
				varX = 22;
			}
			if(intro == 0){
				str  = "Franchement je perds mon temps a surveiller ces mioches...T'as vu les nouvelles reformes pour les enfants etrangers ?";
				str2 = str.substr(i, 1);
				
			}else if(intro == 1){
				str  = "Ouais faut cramer ceux qui naissent chauves, qui n'ont pasles yeux verts et qui ont un poids inferieur a 3,75kg.";
				str2 = str.substr(i, 1);
			}else if(intro == 2){
				str  = "Mmmhhh... De ce que je comprends, je vais devoir partir auplus vite...";
				str2 = str.substr(i, 1);
			}else if(intro == 3){
				if(i == 0){
					varX = 40;
				}else{
					sleep(1);
				}
				gf::Vector2f ViewSize3(1024, 512); 
				mainView.setSize(ViewSize3);
				renderer.setView(mainView);
				text.setColor(gf::Color::Red);
				str  = "K.G.B.";
				str2 = str.substr(i, 2);
				if(i < str.length()-1){
					i++;
				}
				text.setCharacterSize(140);
				text.setPosition(gf::Vector2u (32*varX,32*34));
				varX += 10;
			}
			
			text.setString(str2);

			if(drawRect == 0 && intro != 3){
				renderer.draw(rectangle);
				drawRect++;
			}
			renderer.draw(text);
			renderer.draw(text2);
			// draw everything
			renderer.display();
			sleep(0.7);
			introAction.reset();
			
			if(!space.isActive()){
				spaceisActiveOneTime = true;
			}
			if(i < str.length()){
				i++;
			}

			if(intro == 3 && str2 == "B."){
				sleep(2);
				mainView.setSize(ViewSize2);
				state = GameState::PLAYING;
			}
			if(intro > 3){
				mainView.setSize(ViewSize2);
				state = GameState::PLAYING;
			}
		}

		

		if(state == GameState::PLAYING){
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

		if(state == GameState::GAMEOVER){
			gf::Vector2u introPos(32*51,32*32);
			mainView.setCenter(introPos);
			renderer.setView(mainView);
			renderer.clear();
			gf::Text text3("T'es mort kek", font);
			text3.setCharacterSize(20);
			text3.setColor(gf::Color::Black);
			text3.setPosition(gf::Vector2f(32*51, 32*32));
			text3.setParagraphWidth(1000.0f);
			text3.setAlignment(gf::Alignment::Center);
			text3.setAnchor(gf::Anchor::Center);
			renderer.draw(text3);
			renderer.display();
			sleep(3);
			break;
		}else if(state == GameState::VICTORY){
			
		}

	}
	
	return 0;
}
