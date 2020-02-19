
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
#include "local/Object.h"
#include "local/Dialogs.h"


#define FRAME 60.0

enum GameState{
	INTRO,
	PLAYING,
	VICTORY,
	GAMEOVER
};

void timer(gf::Clock clockss, float seconds){
	gf::Clock clock;

	gf::Time actualTime = gf::seconds(0);
	gf::Time timeToWait = gf::seconds(seconds);
	while(actualTime<timeToWait){
		actualTime = clock.getElapsedTime();
	}
}

int main() {
	
	
	static constexpr gf::Vector2u ScreenSize(1024, 768);
	static constexpr gf::Vector2f ViewSizeIntro(2048, 1024); 
	static constexpr gf::Vector2f ViewSizeJeu(600, 600); 
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
	gf::SingletonStorage<gf::Random> storageForRandom(KGB::gRandom);
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
		gf::Log::debug("J'ai perdu");
		return gf::MessageStatus::Die;
  	});

	KGB::gMessageManager().registerHandler<KGB::Clef>([&state](gf::Id type, gf::Message *msg) {
		assert(type == KGB::Clef::type);
		gf::unused(type, msg);
		gf::Log::debug("Clef recupérée");
		return gf::MessageStatus::Keep;
  	});

	KGB::gMessageManager().registerHandler<KGB::Victory>([&state](gf::Id type, gf::Message *msg) {
		assert(type == KGB::Victory::type);
		gf::unused(type, msg);
		state = GameState::VICTORY;
		gf::Log::debug("J'ai gagné");
		return gf::MessageStatus::Die;
  	});

	//Sound

	KGB::BackgroundMusic music;
	KGB::Dialogs dialogs;

	//map

	gf::TmxLayers layers;
  	if (!layers.loadFromFile(KGB::gResourceManager().getAbsolutePath("map/garderie.tmx"))) {
    	gf::Log::error("Impossible de charger la carte !\n");
    	return EXIT_FAILURE;
  	}
	std::vector<KGB::Object> objs;
  	
	KGB::MapGraphicsData data(layers,objs);
  	KGB::Map map(data);

	mainEntities.addEntity(map);
	for(auto ob : objs){
		mainEntities.addEntity(ob);
	}

	static constexpr gf::Vector2u initialPosition(32*51,32*4);
	KGB::BabyHero bebeHero(initialPosition);
	mainEntities.addEntity(bebeHero);


	static constexpr gf::Vector2u posEnemy1(32*71, 32*5);
	static constexpr gf::Vector2u posEnemy2(32*54, 32*12);
	static constexpr gf::Vector2u posEnemy3(32*45, 32*9);
	static constexpr gf::Vector2u posEnemy4(32*70.5, 32*21);
	static constexpr gf::Vector2u posEnemy5(32*48, 32*15.45);
	static constexpr gf::Vector2u posEnemy6(32*29.5, 32*28);
	static constexpr gf::Vector2u posEnemy7(32*26, 32*35);
	static constexpr gf::Vector2u posEnemy8(32*86.5, 32*29);
	static constexpr gf::Vector2u posEnemy9(32*24, 32*43);
	static constexpr gf::Vector2u posEnemy10(32*23, 32*50);
	static constexpr gf::Vector2u posEnemy11(32*44.5, 32*49);
	static constexpr gf::Vector2u posEnemy12(32*51.5, 32*49);
	static constexpr gf::Vector2u posEnemy13(32*70, 32*49);
	static constexpr gf::Vector2u posEnemy14(32*30, 32*73);
	static constexpr gf::Vector2u posEnemy15(32*54.5, 32*50);
	static constexpr gf::Vector2u posEnemy16(32*55, 32*81);

	std::vector<gf::Orientation> noRotation;
	std::vector<gf::Orientation> rotaEnemy4;
	rotaEnemy4.push_back(gf::Orientation::East);
	rotaEnemy4.push_back(gf::Orientation::South);
	rotaEnemy4.push_back(gf::Orientation::West);

	std::vector<gf::Orientation> rotaEnemy6;
	rotaEnemy6.push_back(gf::Orientation::East);
	rotaEnemy6.push_back(gf::Orientation::South);
	rotaEnemy6.push_back(gf::Orientation::North);

	std::vector<gf::Orientation> rotaEnemy8;
	rotaEnemy8.push_back(gf::Orientation::West);
	rotaEnemy8.push_back(gf::Orientation::North);

	//salle de classe
	KGB::Enemy Vilain(posEnemy1, KGB::Enemy::PathType::Round, gf::Orientation::South, KGB::Enemy::Status::Walking, 380.0, 50.0, noRotation);
	// couloir au dessus fontaine
	KGB::Enemy Vilain2(posEnemy2, KGB::Enemy::PathType::VerticalLine, gf::Orientation::South, KGB::Enemy::Status::Walking, 700.0, 70.0, noRotation);
	//couloir sous spawn
	KGB::Enemy Vilain3(posEnemy3, KGB::Enemy::PathType::HorizontalLine, gf::Orientation::South, KGB::Enemy::Status::Walking, 500.0, 30.0, noRotation);
	//croisement couloir biblio
	KGB::Enemy Vilain4(posEnemy4, KGB::Enemy::PathType::Static, gf::Orientation::West, KGB::Enemy::Status::Waiting, 0.0, 0.0, rotaEnemy4);
	//salle des thunes
	KGB::Enemy Vilain5(posEnemy5, KGB::Enemy::PathType::Static, gf::Orientation::East, KGB::Enemy::Status::Waiting, 0.0, 0.0, noRotation);
	//piece au dessus jardin
	KGB::Enemy Vilain6(posEnemy6, KGB::Enemy::PathType::Static, gf::Orientation::East, KGB::Enemy::Status::Waiting, 0.0, 0.0, rotaEnemy6);
	//jardin
	KGB::Enemy Vilain7(posEnemy7, KGB::Enemy::PathType::HorizontalLine, gf::Orientation::East, KGB::Enemy::Status::Walking, 600.0, 60.0, noRotation);
	//biblio
	KGB::Enemy Vilain8(posEnemy8, KGB::Enemy::PathType::Static, gf::Orientation::North, KGB::Enemy::Status::Waiting, 0.0, 0.0, rotaEnemy8);
	//couloir chambre -- jardin
	KGB::Enemy Vilain9(posEnemy9, KGB::Enemy::PathType::HorizontalLine, gf::Orientation::East, KGB::Enemy::Status::Walking, 1400.0, 85.0, noRotation);
	//couloir bas gauche
	KGB::Enemy Vilain10(posEnemy10, KGB::Enemy::PathType::VerticalLine, gf::Orientation::South, KGB::Enemy::Status::Walking, 950.0, 70.0, noRotation);
	//chambre
	KGB::Enemy Vilain11(posEnemy11, KGB::Enemy::PathType::Static, gf::Orientation::East, KGB::Enemy::Status::Waiting, 0.0, 0.0, noRotation);
	KGB::Enemy Vilain12(posEnemy12, KGB::Enemy::PathType::Static, gf::Orientation::West, KGB::Enemy::Status::Waiting, 0.0, 0.0, noRotation);
	//cuisine
	KGB::Enemy Vilain13(posEnemy13, KGB::Enemy::PathType::Static, gf::Orientation::West, KGB::Enemy::Status::Waiting, 0.0, 0.0, noRotation);
	//salle de jeu
	KGB::Enemy Vilain14(posEnemy14, KGB::Enemy::PathType::HorizontalLine, gf::Orientation::East, KGB::Enemy::Status::Walking, 700.0, 50.0, noRotation);
	//couloir salle de jeu --  batiment H
	KGB::Enemy Vilain15(posEnemy15, KGB::Enemy::PathType::VerticalLine, gf::Orientation::South, KGB::Enemy::Status::Walking, 560.0, 65.0, noRotation);
	//couloir bas droite
	KGB::Enemy Vilain16(posEnemy16, KGB::Enemy::PathType::HorizontalLine, gf::Orientation::East, KGB::Enemy::Status::Walking, 560.0, 65.0, noRotation);
	//salle de classe bis
	KGB::Enemy Vilain17(posEnemy1, KGB::Enemy::PathType::Round, gf::Orientation::North, KGB::Enemy::Status::Walking, 380.0, 50.0, noRotation);

	std::vector<KGB::Enemy> vilains = {Vilain, Vilain2, Vilain3, Vilain4, Vilain5, Vilain6, Vilain7, Vilain8, Vilain9, Vilain10, Vilain11, Vilain12, Vilain13, Vilain14, Vilain15, Vilain16, Vilain17};
	for(std::size_t i = 0; i < vilains.size(); i++){
		mainEntities.addEntity(vilains[i]);
	}
	
	static constexpr gf::Vector2u posBonus1(33*51,32*4);
	static constexpr gf::Vector2u posBonus2(34*51,32*4);
	static constexpr gf::Vector2u posBonus3(35*51,32*4);
	
	
	KGB::Bonus Couche1(posBonus1, KGB::DataType::Bonus_Type::STUNNING_DIAPERS);
	KGB::Bonus Couche2(posBonus2, KGB::DataType::Bonus_Type::INVISIBLE_DIAPERS);
	KGB::Bonus Couche3(posBonus3, KGB::DataType::Bonus_Type::SPEED_DIAPERS);

	std::vector<KGB::Bonus> bonus = {Couche1, Couche2, Couche3};
	
	for(std::size_t i = 0; i < bonus.size(); i++){
		mainEntities.addEntity(bonus[i]);	
	}

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
	
	gf::Action speedDiapers("Speed Diapers");
	speedDiapers.addScancodeKeyControl(gf::Scancode::C);
	speedDiapers.setContinuous();
	actions.addAction(speedDiapers);

	gf::Action stunningDiapers("Stunning Diapers");
	stunningDiapers.addScancodeKeyControl(gf::Scancode::V);
	stunningDiapers.setContinuous();
	actions.addAction(stunningDiapers);

	//Physics
	KGB::Physics physics(objs,layers,bebeHero, vilains, bonus);


	// game loop
	gf::Clock clock;
	renderer.clear(gf::Color::Black);

	KGB::PhysicsDebugger debug(physics.getWorld());
	mainEntities.addEntity(debug);

	static constexpr float Vitesse = 10.0f;
	static constexpr float VitesseMax = 50.0f;
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
				dialogs.muteAll();
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
				dialogs.playFranchement();
			}else if(intro == 1){
				CompleteString  = "Ouais faut cramer ceux qui naissent chauves, qui n'ont pas les yeux verts et qui ont un poids inferieur a 3,75kg.";
				currentString = CompleteString.substr(0, nbCharToSelect);
				dialogs.playCramer();
				
			}else if(intro == 2){
				CompleteString  = "Mmmhhh... De ce que je comprends, je vais devoir partir au plus vite...";
				currentString = CompleteString.substr(0, nbCharToSelect);
				
			}else if(intro == 3){
				dialogs.muteAll();
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
			
			if(invisibleDiapers.isActive()){ 
	
				bebeHero.setInvisible(3*FRAME);

			}

			if(speedDiapers.isActive()){ 
	
				bebeHero.setSpeed(3*FRAME);

			}
			
			if(leftAction.isActive() && !downAction.isActive() && !upAction.isActive()){
				if(bebeHero.getSpeedActive()){
					if(velocity.x > -VitesseMax*2){
						velocity.x -= Vitesse;
					}
				}else{
					if(velocity.x > -VitesseMax){
						velocity.x -= Vitesse;
					}else{
						velocity.x = -VitesseMax;	
					}
				}
				bebeHero.updateOrientation(3);
			}else if(rightAction.isActive()&& !downAction.isActive() && !upAction.isActive()){
				
				if(bebeHero.getSpeedActive()){
					if(velocity.x < VitesseMax*2){
						velocity.x += Vitesse;
					}
				}else{
					if(velocity.x < VitesseMax){
						velocity.x += Vitesse;
					}else{
						velocity.x = VitesseMax;	
					}
				}
				bebeHero.updateOrientation(2);
			}else{
				velocity.x = 0;
			}

			if(downAction.isActive()){
				if(bebeHero.getSpeedActive()){
					if(velocity.y < VitesseMax*2){
						velocity.y += Vitesse;
					}
				}else{
					if(velocity.y < VitesseMax){
						velocity.y += Vitesse;
					}else{
						velocity.y = VitesseMax;	
					}
				}
				bebeHero.updateOrientation(0);
			}else if(upAction.isActive()){
				if(bebeHero.getSpeedActive()){
					if(velocity.y > -VitesseMax*2){
						velocity.y -= Vitesse;
					}
				}else{
					if(velocity.y > -VitesseMax){
						velocity.y -= Vitesse;
					}else{
						velocity.y = -VitesseMax;	
					}
				}
				bebeHero.updateOrientation(1);
			}else{
				velocity.y = 0;
				
			}
			
			bebeHero.setVelocity(velocity);
			
			/*if(stunningDiapers.isActive() && bebeHero.getNbStunning() > 0){	
				
				bebeHero.setStun();
				KGB::Projectile stun(bebeHero.getPosition(), DataType::Projectile_Type::STUNNING_DIAPERS, velocity);
				physics.addProjectile(stun);
			
			}*/
			
			gf::Time time = clock.restart();
			mainEntities.update(time);
			physics.update();

			mainView.setCenter(bebeHero.getPosition());
			renderer.clear(gf::Color::White);
			renderer.setView(mainView);

			mainEntities.render(renderer);
			bebeHero.render(renderer);

			for(size_t i = 0; i < vilains.size(); i++){
				vilains[i].render(renderer);
			}
			
			for(size_t i = 0; i < bonus.size(); i++){
				bonus[i].render(renderer);
			}
			
			for(auto& obj :objs){
				obj.render(renderer);
			}

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
			gf::Text textGameOver("Tu t'es fait choppé !!", font);
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
			gf::Text textVictoire("Bravo ! Le petit Jacob est libre", font);
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
