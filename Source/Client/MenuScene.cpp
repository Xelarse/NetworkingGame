#include "MenuScene.h"

MenuScene::MenuScene(ASGE::Renderer * renderer, ASGE::Input * input)
{
	main_inputs = input;

	init(renderer, main_inputs);
}

MenuScene::~MenuScene()
{
	main_inputs->unregisterCallback(click_handler_id);
}

void MenuScene::init(ASGE::Renderer * renderer, ASGE::Input* inputs)
{

	click_handler_id = this->inputs->addCallbackFnc(ASGE::EventType::E_MOUSE_CLICK,
		&MyNetGame::mouseClickHandler, this);

	menu_background = renderer->createUniqueSprite();
	menu_background->loadTexture("..\\..\\Resources\\Backgrounds\\MainMenu.png");
	menu_background->xPos(0);
	menu_background->yPos(0);

	start_button = renderer->createUniqueSprite();
	start_button->loadTexture("..\\..\\Resources\\Buttons\\Start.png");
	start_button->xPos(390);
	start_button->yPos(300);

	exit_button = renderer->createUniqueSprite();
	exit_button->loadTexture("..\\..\\Resources\\Buttons\\Exit.png");
	exit_button->xPos(390);
	exit_button->yPos(450);
}

void MenuScene::update(const ASGE::GameTime& ms)
{
	
}

void MenuScene::render(ASGE::Renderer * renderer)
{
}

void MenuScene::clickHandler()
{

}
