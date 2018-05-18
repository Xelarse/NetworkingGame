#pragma once
#include <Engine\OGLGame.h>
#include <Engine\InputEvents.h>
#include <Engine\Sprite.h>
#include "SceneManager.h"
#include "Constants.h"

class Scene
{
public:

	// JH: WHere's the virtual destructor, you could be leaking memory on destruction?
	virtual void init(ASGE::Renderer* renderer, ASGE::Input* input, SceneManager* host) = 0;
	virtual void update(const ASGE::GameTime& ms) = 0;
	virtual void render(ASGE::Renderer* renderer) = 0;

	bool lastScene() { return last_scene; };
	void lastScene(bool rhs) { last_scene = rhs; };

	void clickHandlerReset() { main_inputs->unregisterCallback(click_handler_id); };
	void keyHandlerReset() { main_inputs->unregisterCallback(key_handler_id); };
	void mouseHandlerReset() { main_inputs->unregisterCallback(mouse_handler_id); };

protected:

	// JH - never been a fan of the scene having a circular dependency to the manager.
	// Either the manager manages it or it doesn't. 
	SceneManager* host_manager = nullptr;


	ASGE::Input* main_inputs = nullptr;
	ASGE::Renderer* main_renderer = nullptr;

	std::atomic<bool> last_scene = false;

	int click_handler_id = -1;
	int key_handler_id = -1;
	int mouse_handler_id = -1;
};
