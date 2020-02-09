
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
#include "local/Objects.h"

#define FRAME 60.0

enum GameState{
	INTRO,
	PLAYING,
	VICTORY,
	GAMEOVER
};

void timer(gf::Clock clock, float seconds){
	clock.restart();
	gf::Time actualTime = gf::seconds(0);
	gf::Time timeToWait = gf::seconds(seconds);
	while(actualTime<timeToWait){
		actualTime = clock.getElapsedTime();
	}
}

int main() {
	
	//Mettre le b2body dans la classe square
	//SetOrigin (rectangleshape, setanchor)
	
	static constexpr gf::Vector2u ScreenSize(1024, 768);
	static constexpr gf::Vector2f ViewSizeIntro(2048, 1024); 
	static constexpr gf::Vector2f ViewSizeJeu(420, 420); 
	static constexpr gf::Vector2f ViewSizeIntroTitre(1024, 512);
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

	gf::ExtendView mainView(ViewCenter, ViewSizeIntro);
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

	KGB::gMessageManager().registerHandler<KGB::Clef>([&state](gf::Id type, gf::Message *msg) {
		assert(type == KGB::Clef::type);
		gf::unused(type, msg);
		gf::Log::debug("Je winnzzzz");
		return gf::MessageStatus::Die;
  	});

	KGB::gMessageManager().registerHandler<KGB::Victory>([&state](gf::Id type, gf::Message *msg) {
		assert(type == KGB::Victory::type);
		gf::unused(type, msg);
		state = GameState::VICTORY;
		gf::Log::debug("Je winnzzzz");
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
	KGB::Objects objs;
  	
	KGB::MapGraphicsData data(layers,objs);
  	KGB::Map map( data);

	mainEntities.addEntity(map);
	mainEntities.addEntity(objs);

	static constexpr gf::Vector2u initialPosition(32*51,32*4);
	KGB::BabyHero bebeHero(initialPosition);
	mainEntities.addEntity(bebeHero);


	static constexpr gf::Vector2u posEnemy1(32*71, 32*5);
	static constexpr gf::Vector2u posEnemy2(32*54, 32*12);
	static constexpr gf::Vector2u posEnemy3(32*45, 32*9);
	static constexpr gf::Vector2u posEnemy4(32*71, 32*22);
	static constexpr gf::Vector2u posEnemy5(32*48, 32*15.45);

	KGB::Enemy Vilain(posEnemy1, KGB::Enemy::PathType::Round, gf::Orientation::South, KGB::Enemy::Status::Walking, 380.0);
	KGB::Enemy Vilain2(posEnemy2, KGB::Enemy::PathType::VerticalLine, gf::Orientation::South, KGB::Enemy::Status::Walking, 700.0);
	KGB::Enemy Vilain3(posEnemy3, KGB::Enemy::PathType::HorizontalLine, gf::Orientation::South, KGB::Enemy::Status::Walking, 500.0);
	KGB::Enemy Vilain4(posEnemy4, KGB::Enemy::PathType::Static, gf::Orientation::West, KGB::Enemy::Status::Waiting, 0.0);
	KGB::Enemy Vilain5(posEnemy5, KGB::Enemy::PathType::Static, gf::Orientation::East, KGB::Enemy::Status::Waiting, 0.0);

	mainEntities.addEntity(Vilain);
	mainEntities.addEntity(Vilain2);
	mainEntities.addEntity(Vilain3);
	mainEntities.addEntity(Vilain4);
	mainEntities.addEntity(Vilain5);
	
	
	static constexpr gf::Vector2u posBonus1(33*51,32*4);
	static constexpr gf::Vector2u posBonus2(34*51,32*4);
	static constexpr gf::Vector2u posBonus3(35*51,32*4);
	
	
	KGB::Bonus Couche1(posBonus1, KGB::DataType::Bonus_Type::STUNNING_DIAPERS);
	KGB::Bonus Couche2(posBonus2, KGB::DataType::Bonus_Type::STUNNING_DIAPERS);
	KGB::Bonus Couche3(posBonus3, KGB::DataType::Bonus_Type::STUNNING_DIAPERS);

	mainEntities.addEntity(Couche1);
	mainEntities.addEntity(Couche2);
	mainEntities.addEntity(Couche3);

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

	gf::Action invisibleDiapers("Invisible Diapers");
	invisibleDiapers.addScancodeKeyControl(gf::Scancode::X);
	invisibleDiapers.setContinuous();
	actions.addAction(invisibleDiapers);

	//Physics
	KGB::Physics physics(objs,layers,bebeHero, Vilain, Vilain2, Vilain3, Vilain4, Vilain5, Couche1, Couche2, Couche3);

	// game loop
	gf::Clock clock;
	renderer.clear(gf::Color::Black);

	KGB::PhysicsDebugger debug(physics.getWorld());
	mainEntities.addEntity(debug);

	static constexpr float Vitesse = 10.0f;
	gf::Vector2d velocity(0,0);

	bool debugPhysics = false;

	//Initialisation pour intro
	static constexpr gf::Vector2u introPos(32*51,32*32);
	mainView.setCenter(introPos);
	renderer.setView(mainView);

	static constexpr gf::Vector2u textPosition(32*25, 32*24);
	static constexpr gf::Vector2f textBasIntroPosition(32*51, 32*52);
	static constexpr gf::Vector2u textKBGPosition(32*41, 32*33);

	static constexpr gf::Vector2f backgroundPosition(32*19, 32*16);

	gf::Font font("../data/KGB/Pokemon_Classic.ttf");

	size_t intro = 0;
	size_t nbCharToSelect = 0;
	bool spaceisActiveOneTime = true;

	gf::Text textBasIntro("Appuyer sur espace pour voir la suite et sur p pour passer directement au jeu", font);
	textBasIntro.setCharacterSize(30);
	textBasIntro.setColor(gf::Color::Red);
	textBasIntro.setPosition(textBasIntroPosition);
	textBasIntro.setParagraphWidth(1000.0f);
	textBasIntro.setAlignment(gf::Alignment::Center);
	textBasIntro.setAnchor(gf::Anchor::Center);

	gf::Texture background("../data/KGB/Image/intro1.png");
	gf::Sprite backgroundSprite(background);
	backgroundSprite.setPosition(backgroundPosition);
	
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
				nbCharToSelect = 0;
				if(spaceisActiveOneTime){
					intro++;
					spaceisActiveOneTime = false;
				}
			}

			if(skip.isActive()){
				intro = 4;
			}
			
			std::string CompleteString;
			std::string currentString;	

			gf::Text textIntro;			
			textIntro.setPosition(textPosition);
			textIntro.setFont(font);
			textIntro.setParagraphWidth(1700.0f);
			textIntro.setAlignment(gf::Alignment::Center);
			textIntro.setAnchor(gf::Anchor::Center);
			textIntro.setCharacterSize(30);
			textIntro.setColor(gf::Color::Black);

			if(intro == 0){
				CompleteString  = "Franchement je perds mon temps a surveiller ces mioches...\nT'as vu les nouvelles reformes pour les enfants etrangers ?";
				currentString = CompleteString.substr(0, nbCharToSelect);
			}else if(intro == 1){
				CompleteString  = "Ouais faut cramer ceux qui naissent chauves, qui n'ont pas les yeux verts et qui ont un poids inferieur a 3,75kg.";
				currentString = CompleteString.substr(0, nbCharToSelect);
			}else if(intro == 2){
				CompleteString  = "Mmmhhh... De ce que je comprends, je vais devoir partir au plus vite...";
				currentString = CompleteString.substr(0, nbCharToSelect);
			}else if(intro == 3){
				mainView.setSize(ViewSizeIntroTitre);
				renderer.setView(mainView);

				if(nbCharToSelect >= 2){
					timer(clock, 1.0);
				}
				
				CompleteString  = "K.G.B.";
				currentString = CompleteString.substr(0, nbCharToSelect);

				if(nbCharToSelect < CompleteString.length()-1){
					nbCharToSelect++;
				}

				textIntro.setColor(gf::Color::Red);
				textIntro.setAlignment(gf::Alignment::Left);
				textIntro.setCharacterSize(140);
				textIntro.setPosition(textKBGPosition);
			}
			
			textIntro.setString(currentString);

			
			if(intro < 3){
				renderer.clear(gf::Color::Black);
				renderer.draw(backgroundSprite);
			}else{
				renderer.clear(gf::Color::White);
			}
			renderer.draw(textIntro);
			renderer.draw(textBasIntro);
			renderer.display();
			timer(clock, 0.04);
			introAction.reset();
			
			if(!space.isActive()){
				spaceisActiveOneTime = true;
			}
			if(nbCharToSelect < CompleteString.length()){
				nbCharToSelect++;
			}

			if(intro == 3 && currentString == "K.G.B."){
				timer(clock, 2);
				mainView.setSize(ViewSizeJeu);
				state = GameState::PLAYING;
			}
			if(intro > 3){
				mainView.setSize(ViewSizeJeu);
				state = GameState::PLAYING;
			}
		}

		
		//Début du jeu
		if(state == GameState::PLAYING){
			gf::Event event;
			while (window.pollEvent(event)) {
				actions.processEvent(event);
				views.processEvent(event);
			}

			if(invisibleDiapers.isActive()){ 
	
				bebeHero.setInvisible(3*FRAME);

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
			
			gf::Time time = clock.restart();
			mainEntities.update(time);
			physics.update();

			mainView.setCenter(bebeHero.getPosition());
			renderer.clear(gf::Color::White);
			renderer.setView(mainView);


			mainEntities.render(renderer);
			bebeHero.render(renderer);
			Vilain.render(renderer);
			Vilain2.render(renderer);
			Vilain3.render(renderer);
			Vilain4.render(renderer);
			Vilain5.render(renderer);
			Couche1.render(renderer);
			Couche2.render(renderer);
			Couche3.render(renderer);
			
			objs.render(renderer);
			if(debugPhysics){
				debug.render(renderer);
			}
			renderer.setView(hudView);
			renderer.display();
			actions.reset();
		}

		if(state == GameState::GAMEOVER){
			mainView.setCenter(introPos);
			renderer.setView(mainView);
			renderer.clear();
			gf::Text textGameOver("T'es mort kek", font);
			textGameOver.setCharacterSize(20);
			textGameOver.setColor(gf::Color::Black);
			textGameOver.setPosition(introPos);
			textGameOver.setParagraphWidth(1000.0f);
			textGameOver.setAlignment(gf::Alignment::Center);
			textGameOver.setAnchor(gf::Anchor::Center);
			renderer.draw(textGameOver);
			renderer.display();
			timer(clock, 3.0);
			break;
		}else if(state == GameState::VICTORY){
			mainView.setCenter(introPos);
			renderer.setView(mainView);
			renderer.clear();
			gf::Text textVictoire("Free Jacob", font);
			textVictoire.setCharacterSize(20);
			textVictoire.setColor(gf::Color::Black);
			textVictoire.setPosition(introPos);
			textVictoire.setParagraphWidth(1000.0f);
			textVictoire.setAlignment(gf::Alignment::Center);
			textVictoire.setAnchor(gf::Anchor::Center);
			renderer.draw(textVictoire);
			renderer.display();
			timer(clock, 3.0);
			break;
		}

	}
	
	return 0;
}
