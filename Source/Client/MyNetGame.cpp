#include <Engine\InputEvents.h>

#include "MyNetGame.h"
#include "GameFont.h"

namespace {
	const float BACKGROUND_LAYER = 0.0f;
	const float FOREGROUND_LAYER = 1.0f;
};

MyNetGame::~MyNetGame()
{
	//this->inputs->unregisterCallback(key_handler_id);
	//this->inputs->unregisterCallback(click_handler_id);
	LoadedGameFont::loaded_fonts.clear();
	network.deinitialize();
	audio_engine->stopAllSounds();
}

bool MyNetGame::init()
{
	game_width = 1280;
	game_height = 720;

	if (!initAPI(ASGE::Renderer::WindowMode::WINDOWED))
	{
		return false;
	}

	renderer->setWindowTitle("The Gathering Storm");
	renderer->setClearColour(ASGE::COLOURS::CORNFLOWERBLUE);
	renderer->setSpriteMode(ASGE::SpriteSortMode::BACK_TO_FRONT);
	toggleFPS();

	this->inputs->use_threads = true;
	
	key_handler_id = this->inputs->addCallbackFnc(
		ASGE::EventType::E_KEY, &MyNetGame::keyHandler, this);

	click_handler_id = this->inputs->addCallbackFnc(ASGE::EventType::E_MOUSE_CLICK, 
		&MyNetGame::mouseClickHandler, this);

	auto font_idx = renderer->loadFont(".\\Resources\\Fonts\\Zorque.ttf", 60);

	LoadedGameFont::loaded_fonts.reserve(5);
	LoadedGameFont::loaded_fonts.push_back(
		LoadedGameFont(font_idx, "zorque", 60));

	network.initialize();
	th = std::thread(&ClientComponent::consumeEvents, &network);
	th.detach();
	

	//initilising audio engine
	initAudioEngine();


	////Loading in units and initialising one, Change the number index in unit types to determine which unit is loaded from the json file
	UnitType::load();
	gunner_enemy.reset(UnitType::unit_types[1].createUnit(renderer.get()));

	return true;
}

void MyNetGame::update(const ASGE::GameTime& ms)
{

}

void MyNetGame::render(const ASGE::GameTime& ms)
{
	renderer->setFont(LoadedGameFont::loaded_fonts[0].id);

	if (network.isConnected())
	{
		renderer->renderText("CONNECTED", 250, 100, ASGE::COLOURS::WHITE);
	}

	else if (network.isConnecting())
	{
		renderer->renderText("CONNECTING", 250, 100, ASGE::COLOURS::WHITE);
	}

	else
	{
		renderer->renderText("DISCONNECTED", 250, 100, ASGE::COLOURS::WHITE);
	}

	renderer->renderSprite(*gunner_enemy->getObjectSprite());
}

void MyNetGame::keyHandler(const ASGE::SharedEventData data)
{
	//const ASGE::KeyEvent* key_event =
	//	static_cast<const ASGE::KeyEvent*>(data.get());

	//auto key = key_event->key;
	//auto action = key_event->action;

	//if (key == ASGE::KEYS::KEY_ESCAPE)
	//{
	//	signalExit();
	//}
}

void MyNetGame::mouseClickHandler(const ASGE::SharedEventData data)
{
	//const ASGE::ClickEvent* click_event = 
	//	static_cast<const ASGE::ClickEvent*>(data.get());

	//auto button = click_event->button;
	//auto action = click_event->action;

	//// this will change slightly on the next update to the project where mouse click co - ords will be with the click event itself.
}

bool MyNetGame::initAudioEngine()
{
	using namespace irrklang;
	audio_engine.reset(createIrrKlangDevice());
	if (!audio_engine)
	{
		// error starting audio engine
		return false;
	}
	return true;
}
