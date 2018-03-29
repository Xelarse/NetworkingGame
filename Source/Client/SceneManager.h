#pragma once
#include <Engine\OGLGame.h>
#include <Engine\InputEvents.h>
#include <queue>

class Scene;

class SceneManager

{

	struct input_info
	{
		ASGE::ClickEvent* click_event = nullptr;
		ASGE::KeyEvent* key_event = nullptr;
	};


public:

	void init();
	void update();

	void addInputToQueue(input_info info);
	void addScene(Scene* scene);

private:
	
	std::vector<Scene> game_scenes;

	std::queue<input_info> input_queue;
	std::mutex input_queue_mtx;
};
