#pragma once
#include <Engine\OGLGame.h>
#include <Engine\Sprite.h>
#include <Client\ClientNetworking.h>
#include <irrKlang.h>

#include "Unit.h"
#include "UnitType.h"
#include "SceneManager.h"
#include "MenuScene.h"
#include "Scene.h"

namespace ASGE {
	struct GameTime;
}

namespace irrklang {
	class ISoundEngine;
}


/**
*  MyNetGame is the main entrypoint into the game.
*  It is based on the ASGE framework and makes use of
*  the OGLGame class. This allows the rendering and
*  input initialisation to be handled easily and without
*  fuss. The game logic responsible for updating and
*  rendering the game starts here.
*/
class MyNetGame 
	: public ASGE::OGLGame
{
public:
	
	/**
	*  Default constructor for game.
	*/
	MyNetGame() = default;
	
	/**
	*  Destructor for game.
	*  Make sure to clean up any dynamically allocated
	*  memory inside the game's destructor. For example
	*  game fonts need to be deallocated.
	*/ 
	~MyNetGame();

	/**
	*  The initialisation of the game.
	*  Both the game's initialisation and the API setup
	*  should be performed inside this function. Once the
	*  API is up and running the input system can register
	*  callback functions when certain events happen.
	*/ 
	virtual bool init() override;
	
private:
	/**
	*  The simulation for the game.
	*  The objects in the game need to updated or simulated
	*  over time. Use this function to decouple the render
	*  phases from the simulation.
	*  @param us The ms time between frames and running time
	*  @see GameTime
	*/
	virtual void update(const ASGE::GameTime& ms) override;
	
	/**
	*  The rendering of the game.
	*  This function gets called after the game world has
	*  been updated. Ensure all your objects are in a sane
	*  simulated state before calling this function.
	*  @param us The delta time between frames and running time
	*  @see GameTime
	*/
	virtual void render(const ASGE::GameTime& ms) override;

	//Our stuff under the goes here under the boilerplate

private:
	std::thread th;           /**< Network Event Thread. 
							       Consumes incoming network events. */
	ClientComponent network;  /**< Network Component. 
							       The networking component used for clients. */

	//Our stuff goes here under the boilerplate

	bool initAudioEngine();

	std::unique_ptr<SceneManager> scene_manager;


	std::unique_ptr<Unit> gunner_enemy = nullptr;

	std::unique_ptr<irrklang::ISoundEngine> audio_engine = nullptr;

	std::atomic<bool> exit_game = false;
};

