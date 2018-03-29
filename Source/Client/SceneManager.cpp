#include "SceneManager.h"
#include "Scene.h"

void SceneManager::init()
{
}

void SceneManager::update()
{
}

void SceneManager::addInputToQueue(input_info info)
{
	std::lock_guard<std::mutex> lock(input_queue_mtx);
	input_queue.push(std::move(info));
}

void SceneManager::addScene(Scene* scene)
{

}
