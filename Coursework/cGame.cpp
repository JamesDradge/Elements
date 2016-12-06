/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	score = 0;


	// Store the textures
	textureName = { "enemyFire", "enemyWater", "enemyEarth", "enemyAir", "bulletFire", "bulletWater", "bulletEarth", "bulletAir", "thePlayer","theBackground", "theTitle", "theEnd" };
	texturesToUse = { "Images\\enemyFire.png", "Images\\enemyWater.png", "Images\\enemyEarth.png", "Images\\enemyAir.png", "Images\\bulletFire.png", "Images\\bulletWater.png", "Images\\bulletEarth.png", "Images\\bulletAir.png", "Images\\newPlayer.png", "Images\\newGameboard.png", "Images\\newTitle.png", "Images\\endNew.png" };
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	// Store the textures for the buttons
	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn", "settings_btn" };
	btnTexturesToUse = { "Images/exitNew.png", "Images/Buttons/button_instructions.png", "Images/Buttons/button_load.png", "Images/retryNew.png", "Images/startNew.png", "Images/Buttons/button_save.png", "Images/Buttons/button_settings.png" };
	btnPos = { { 400, 375 }, { 400, 300 }, { 400, 300 }, { 500, 500 }, { 400, 300 }, { 740, 500 }, { 400, 300 } };
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theGameState = MENU;
	theBtnType = EXIT;

	// Create textures for Game Dialogue (text)
	fontList = { "viner"};
	fontsToUse = { "Fonts/Viner Hand ITC.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}
	gameTextNames = { "Title," "Score" };
	gameTextList = { "Elements", "Score" };
	theTextureMgr->addTexture("Title", theFontMgr->getFont("viner")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 0, 0, 0, 255 }, { 0, 0, 0, 0 }));


	theTextureMgr->addTexture("Score", theFontMgr->getFont("viner")->createTextTexture(theRenderer, gameTextList[1], SOLID, { 0, 0, 0, 255 }, { 0, 0, 0, 0 }));
	



	// Load game sounds
	soundList = { "theme", "shot", "hit" };
	soundTypes = { MUSIC, SFX, SFX };
	soundsToUse = { "Audio/backgroundMusic.wav", "Audio/shotFired.wav", "Audio/enemyHit.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	spriteMenuBkgd.setSpritePos({ 0, 0 });
	spriteMenuBkgd.setTexture(theTextureMgr->getTexture("theTitle"));
	spriteMenuBkgd.setSpriteDimensions(theTextureMgr->getTexture("theTitle")->getTWidth(), theTextureMgr->getTexture("theTitle")->getTHeight());

	spriteEndBkgd.setSpritePos({ 0, 0 });
	spriteEndBkgd.setTexture(theTextureMgr->getTexture("theEnd"));
	spriteEndBkgd.setSpriteDimensions(theTextureMgr->getTexture("theEnd")->getTWidth(), theTextureMgr->getTexture("theEnd")->getTHeight());

	//Sets the players spawn
	thePlayer.setSpritePos({ 0, 344 });
	thePlayer.setSpriteRotAngle(thePlayer.getSpriteRotAngle() + 90);
	thePlayer.setTexture(theTextureMgr->getTexture("thePlayer"));
	thePlayer.setSpriteDimensions(theTextureMgr->getTexture("thePlayer")->getTWidth(), theTextureMgr->getTexture("thePlayer")->getTHeight());
	thePlayer.setPlayerVelocity({ 0, 0 });

	
	
	// Spawns Fire enemies
	for (int enemF = 0; enemF < 5; enemF++)
	{
		fireEnemys.push_back(new cEnemy);
		fireEnemys[enemF]->setSpritePos({ 400 * (rand() % 5 + 1), 140 });
		fireEnemys[enemF]->setSpriteTranslation({ -25, 0 });
		fireEnemys[enemF]->setTexture(theTextureMgr->getTexture("enemyFire"));
		fireEnemys[enemF]->setSpriteDimensions(theTextureMgr->getTexture("enemyFire")->getTWidth(), theTextureMgr->getTexture("enemyFire")->getTHeight());
		fireEnemys[enemF]->setEnemyVelocity({ 3.0f, 3.0f });
		fireEnemys[enemF]->setActive(true);
		

	}

	// Spawns Water enemies
	for (int enemW = 0; enemW < 5; enemW++)
	{
		waterEnemys.push_back(new cEnemy);
		waterEnemys[enemW]->setSpritePos({ 350 * (rand() % 6 + 1), 340 });
		waterEnemys[enemW]->setSpriteTranslation({ -25, 0 });
		waterEnemys[enemW]->setTexture(theTextureMgr->getTexture("enemyWater"));
		waterEnemys[enemW]->setSpriteDimensions(theTextureMgr->getTexture("enemyWater")->getTWidth(), theTextureMgr->getTexture("enemyWater")->getTHeight());
		waterEnemys[enemW]->setEnemyVelocity({ 3.0f, 3.0f });
		waterEnemys[enemW]->setActive(true);

	}

	// Spawns Earth enemies
	for (int enemE = 0; enemE < 5; enemE++)
	{
		earthEnemys.push_back(new cEnemy);
		earthEnemys[enemE]->setSpritePos({ 420 * (rand() % 2 + 1), 540 });
		earthEnemys[enemE]->setSpriteTranslation({ -25, 0 });
		earthEnemys[enemE]->setTexture(theTextureMgr->getTexture("enemyEarth"));
		earthEnemys[enemE]->setSpriteDimensions(theTextureMgr->getTexture("enemyEarth")->getTWidth(), theTextureMgr->getTexture("enemyEarth")->getTHeight());
		earthEnemys[enemE]->setEnemyVelocity({ 3.0f, 3.0f });
		earthEnemys[enemE]->setActive(true);
	}

	// Spawns Air enemies
	for (int enemA = 0; enemA < 5; enemA++)
	{
		airEnemys.push_back(new cEnemy);
		airEnemys[enemA]->setSpritePos({ 700 * (rand() % 2 + 1), 700 });
		airEnemys[enemA]->setSpriteTranslation({ -25, 0 });
		airEnemys[enemA]->setTexture(theTextureMgr->getTexture("enemyAir"));
		airEnemys[enemA]->setSpriteDimensions(theTextureMgr->getTexture("enemyAir")->getTWidth(), theTextureMgr->getTexture("enemyAir")->getTHeight());
		airEnemys[enemA]->setEnemyVelocity({ 3.0f, 3.0f });
		airEnemys[enemA]->setActive(true);
	}


}


void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	 loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}



void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{
	// Check Button clicked and change state
	if (theGameState == MENU || theGameState == END)
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
	
	}
	else if (theGameState == END)
	{
		theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
	}
	else
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, MENU, theAreaClicked);
	}
	theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);


}

void cGame::update(double deltaTime)
{
	switch (theGameState)
	{
		case PLAYING:
		{
	// Update the visibility and position of each fireEnemy
	vector<cEnemy*>::iterator fireEnemyIterator = fireEnemys.begin();
	while (fireEnemyIterator != fireEnemys.end())
	{
		if ((*fireEnemyIterator)->isActive() == false)
		{
			fireEnemyIterator = fireEnemys.erase(fireEnemyIterator);
		}
		else
		{
			(*fireEnemyIterator)->update(deltaTime);
			++fireEnemyIterator;
		}
	}
	// Update the visibility and position of each waterEnemy
	vector<cEnemy*>::iterator waterEnemyIterator = waterEnemys.begin();
	while (waterEnemyIterator != waterEnemys.end())
	{
		if ((*waterEnemyIterator)->isActive() == false)
		{
			waterEnemyIterator = waterEnemys.erase(waterEnemyIterator);
		}
		else
		{
			(*waterEnemyIterator)->update(deltaTime);
			++waterEnemyIterator;
		}
	}
		// Update the visibility and position of each earthEnemy
	vector<cEnemy*>::iterator earthEnemyIterator = earthEnemys.begin();
	while (earthEnemyIterator != earthEnemys.end())
	{
		if ((*earthEnemyIterator)->isActive() == false)
		{
			earthEnemyIterator = earthEnemys.erase(earthEnemyIterator);
		}
		else
		{
			(*earthEnemyIterator)->update(deltaTime);
			++earthEnemyIterator;
		}
	}
	// Update the visibility and position of each airEnemy
	vector<cEnemy*>::iterator airEnemyIterator = airEnemys.begin();
	while (airEnemyIterator != airEnemys.end())
	{
		if ((*airEnemyIterator)->isActive() == false)
		{
			airEnemyIterator = airEnemys.erase(airEnemyIterator);
		}
		else
		{
			(*airEnemyIterator)->update(deltaTime);
			++airEnemyIterator;
		}
	}

	// Update the visibility and position of each bulletFire
	vector<cBullets*>::iterator bulletFireIterartor = fireBullets.begin();
	while (bulletFireIterartor != fireBullets.end())
	{
		if ((*bulletFireIterartor)->isActive() == false)
		{
			bulletFireIterartor = fireBullets.erase(bulletFireIterartor);
		}
		else
		{
			(*bulletFireIterartor)->update(deltaTime);
			++bulletFireIterartor;
		}
	}

		// Update the visibility and position of each bulletWater
	vector<cBullets*>::iterator bulletWaterIterartor = waterBullets.begin();
	while (bulletWaterIterartor != waterBullets.end())
	{
		if ((*bulletWaterIterartor)->isActive() == false)
		{
			bulletWaterIterartor = waterBullets.erase(bulletWaterIterartor);
		}
		else
		{
			(*bulletWaterIterartor)->update(deltaTime);
			++bulletWaterIterartor;
		}
	}
		// Update the visibility and position of each bulletEarth
		vector<cBullets*>::iterator bulletEarthIterartor = earthBullets.begin();
		while (bulletEarthIterartor != earthBullets.end())
		{
			if ((*bulletEarthIterartor)->isActive() == false)
			{
				bulletEarthIterartor = earthBullets.erase(bulletEarthIterartor);
			}
			else
			{
				(*bulletEarthIterartor)->update(deltaTime);
				++bulletEarthIterartor;
			}
		}

			// Update the visibility and position of each bulletAir
			vector<cBullets*>::iterator bulletAirIterartor = airBullets.begin();
			while (bulletAirIterartor != airBullets.end())
			{
				if ((*bulletAirIterartor)->isActive() == false)
				{
					bulletAirIterartor = airBullets.erase(bulletAirIterartor);
				}
				else
				{
					(*bulletAirIterartor)->update(deltaTime);
					++bulletAirIterartor;
				}
			}

		}
		/*
		==============================================================
		| Check for collisions
		==============================================================
		*/
		for (vector<cBullets*>::iterator bulletFireIterartor = fireBullets.begin(); bulletFireIterartor != fireBullets.end(); ++bulletFireIterartor)
		{
			//(*bulletFireIterartor)->update(deltaTime);
			for (vector<cEnemy*>::iterator fireEnemyIterator = fireEnemys.begin(); fireEnemyIterator != fireEnemys.end(); ++fireEnemyIterator)
			{
				if ((*fireEnemyIterator)->collidedWith(&(*fireEnemyIterator)->getBoundingRect(), &(*bulletFireIterartor)->getBoundingRect()))
				{
					// if a collision set the bulletFire and enemy to false
					(*fireEnemyIterator)->setActive(false);
					(*bulletFireIterartor)->setActive(false);
					theSoundMgr->getSnd("hit")->play(0);
					score += 100;
					strScore = gameTextList[1] + to_string(score);
					theTextureMgr->deleteTexture("Score");
					cout << "Player score: " << strScore << endl;
					
				}

			}
			
			
		}
		for (vector<cBullets*>::iterator bulletWaterIterartor = waterBullets.begin(); bulletWaterIterartor != waterBullets.end(); ++bulletWaterIterartor)
		{
			//(*bulletWaterIterartor)->update(deltaTime);
			for (vector<cEnemy*>::iterator waterEnemyIterator = waterEnemys.begin(); waterEnemyIterator != waterEnemys.end(); ++waterEnemyIterator)
			{
				if ((*waterEnemyIterator)->collidedWith(&(*waterEnemyIterator)->getBoundingRect(), &(*bulletWaterIterartor)->getBoundingRect()))
				{
					// if a collision set the bulletWaterIterartor and enemy to false
					(*waterEnemyIterator)->setActive(false);
					(*bulletWaterIterartor)->setActive(false);
					theSoundMgr->getSnd("hit")->play(0);
					score += 100;
					strScore = gameTextList[1] + to_string(score);
					theTextureMgr->deleteTexture("Score");
					cout << "Player score: " << strScore << endl;
				}
			}
		}
		for (vector<cBullets*>::iterator bulletEarthIterartor = earthBullets.begin(); bulletEarthIterartor != earthBullets.end(); ++bulletEarthIterartor)
		{
			//(*bulletEarthIterartor)->update(deltaTime);
			for (vector<cEnemy*>::iterator earthEnemyIterator = earthEnemys.begin(); earthEnemyIterator != earthEnemys.end(); ++earthEnemyIterator)
			{
				if ((*earthEnemyIterator)->collidedWith(&(*earthEnemyIterator)->getBoundingRect(), &(*bulletEarthIterartor)->getBoundingRect()))
				{
					// if a collision set the bulletEarthIterartor and enemy to false
					(*earthEnemyIterator)->setActive(false);
					(*bulletEarthIterartor)->setActive(false);
					theSoundMgr->getSnd("hit")->play(0);
					score += 100;
					strScore = gameTextList[1] + to_string(score);
					theTextureMgr->deleteTexture("Score");
					cout << "Player score: " << strScore << endl;
				}
			}
		}
		for (vector<cBullets*>::iterator bulletAirIterartor = airBullets.begin(); bulletAirIterartor != airBullets.end(); ++bulletAirIterartor)
		{
			//(*bulletAirIterartor)->update(deltaTime);
			for (vector<cEnemy*>::iterator airEnemyIterator = airEnemys.begin(); airEnemyIterator != airEnemys.end(); ++airEnemyIterator)
			{
				if ((*airEnemyIterator)->collidedWith(&(*airEnemyIterator)->getBoundingRect(), &(*bulletAirIterartor)->getBoundingRect()))
				{
					// if a collision set the bulletAirIterartor and enemy to false
					(*airEnemyIterator)->setActive(false);
					(*bulletAirIterartor)->setActive(false);
					theSoundMgr->getSnd("hit")->play(0);
					score += 100;
					strScore = gameTextList[1] + to_string(score);
					theTextureMgr->deleteTexture("Score");
					cout << "Player score: " << strScore << endl;
				}
			}
		}
		if (score > 1500)
		{
			theGameState = END;
		}


	}

	

	// Update the Players position
	thePlayer.update(deltaTime);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	switch (theGameState)
	{
	case MENU:
	{
		spriteMenuBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		// Render Button
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 400, 375 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
		
	}
	break;
	case PLAYING:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
	

		// Render the Title
		cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
		SDL_Rect pos = { 150, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//Render a score
		theTextureMgr->addTexture("Score", theFontMgr->getFont("viner")->createTextTexture(theRenderer, strScore.c_str(), SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));
		tempTextTexture = theTextureMgr->getTexture("Score");
		pos = { 700, 22, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// render the player
		thePlayer.render(theRenderer, &thePlayer.getSpriteDimensions(), &thePlayer.getSpritePos(), thePlayer.getSpriteRotAngle(), &thePlayer.getSpriteCentre(), thePlayer.getSpriteScale());
		///SDL_RenderPresent(theRenderer);
		



		//SDL_RenderClear(theRenderer);
		//spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render each fireEnemy in the vector array
		for (int draw = 0; draw < fireEnemys.size(); draw++)
		{
			fireEnemys[draw]->render(theRenderer, &fireEnemys[draw]->getSpriteDimensions(), &fireEnemys[draw]->getSpritePos(), fireEnemys[draw]->getSpriteRotAngle(), &fireEnemys[draw]->getSpriteCentre(), fireEnemys[draw]->getSpriteScale());
		}
		// Render each waterEnemy in the vector array
		for (int draw = 0; draw < waterEnemys.size(); draw++)
		{
			waterEnemys[draw]->render(theRenderer, &waterEnemys[draw]->getSpriteDimensions(), &waterEnemys[draw]->getSpritePos(), waterEnemys[draw]->getSpriteRotAngle(), &waterEnemys[draw]->getSpriteCentre(), waterEnemys[draw]->getSpriteScale());
		}
		// Render each earthEnemy in the vector array
		for (int draw = 0; draw < earthEnemys.size(); draw++)
		{
			earthEnemys[draw]->render(theRenderer, &earthEnemys[draw]->getSpriteDimensions(), &earthEnemys[draw]->getSpritePos(), earthEnemys[draw]->getSpriteRotAngle(), &earthEnemys[draw]->getSpriteCentre(), earthEnemys[draw]->getSpriteScale());
		}
		// Render each airEnemy in the vector array
		for (int draw = 0; draw < airEnemys.size(); draw++)
		{
			airEnemys[draw]->render(theRenderer, &airEnemys[draw]->getSpriteDimensions(), &airEnemys[draw]->getSpritePos(), airEnemys[draw]->getSpriteRotAngle(), &airEnemys[draw]->getSpriteCentre(), airEnemys[draw]->getSpriteScale());
		}


		// Render each fire bullet in the vector array
		for (int draw = 0; draw < fireBullets.size(); draw++)
		{
			fireBullets[draw]->render(theRenderer, &fireBullets[draw]->getSpriteDimensions(), &fireBullets[draw]->getSpritePos(), fireBullets[draw]->getSpriteRotAngle(), &fireBullets[draw]->getSpriteCentre(), fireBullets[draw]->getSpriteScale());
		}
		// Render each water bullet in the vector array
		for (int draw = 0; draw < waterBullets.size(); draw++)
		{
			waterBullets[draw]->render(theRenderer, &waterBullets[draw]->getSpriteDimensions(), &waterBullets[draw]->getSpritePos(), waterBullets[draw]->getSpriteRotAngle(), &waterBullets[draw]->getSpriteCentre(), waterBullets[draw]->getSpriteScale());
		}
		// Render each earth bullet in the vector array
		for (int draw = 0; draw < earthBullets.size(); draw++)
		{
			earthBullets[draw]->render(theRenderer, &earthBullets[draw]->getSpriteDimensions(), &earthBullets[draw]->getSpritePos(), earthBullets[draw]->getSpriteRotAngle(), &earthBullets[draw]->getSpriteCentre(), earthBullets[draw]->getSpriteScale());
		}
		// Render each air bullet in the vector array
		for (int draw = 0; draw < airBullets.size(); draw++)
		{
			airBullets[draw]->render(theRenderer, &airBullets[draw]->getSpriteDimensions(), &airBullets[draw]->getSpritePos(), airBullets[draw]->getSpriteRotAngle(), &airBullets[draw]->getSpriteCentre(), airBullets[draw]->getSpriteScale());
		}
		
		
	}
	break;
	case END:
	{
		score = 0;
		
		

		spriteEndBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("Title");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 500, 500 });
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 500, 575 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
	}

	break;
	case QUIT:
	{
		loop = false;
	}
	break;
	default:
		break;
	}
	SDL_RenderPresent(theRenderer);
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				theAreaClicked = { event.motion.x, event.motion.y };
			}
			break;
			case SDL_BUTTON_RIGHT:
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
			}
			break;
			case SDL_BUTTON_RIGHT:
				break;
			default:
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				theLoop = false;
				break;
			case SDLK_DOWN:
			{
				if (thePlayer.getSpritePos().y < 670)
				{
					thePlayer.setSpriteTranslation({ 0, 30 });
				}
			}
			break;

			case SDLK_UP:
			{
				if (thePlayer.getSpritePos().y > 90)
				{
					thePlayer.setSpriteTranslation({ 0, -30 });
				}
			}
			break;
			case SDLK_RIGHT:
			{
				if (thePlayer.getSpritePos().x < 0)
				{
					thePlayer.setSpriteTranslation({ 10, 0 });
					
				}
			}
			break;

			case SDLK_LEFT:
			{
				if (thePlayer.getSpritePos().x > 0)
				{
					thePlayer.setSpriteTranslation({ -10, 0 });
					
				}
			}
			break;
			case SDLK_SPACE:
			{
				

	
			}
				

					break;

			case SDLK_1:
			{

				
				fireBullets.push_back(new cBullets);
				int fnumBullets = fireBullets.size() - 1;
				fireBullets[fnumBullets]->setSpritePos({ thePlayer.getBoundingRect().x + thePlayer.getSpriteCentre().x, thePlayer.getBoundingRect().y + thePlayer.getSpriteCentre().y });
				fireBullets[fnumBullets]->setSpriteTranslation({ 2.0f, 2.0f });
				fireBullets[fnumBullets]->setTexture(theTextureMgr->getTexture("bulletFire"));
				fireBullets[fnumBullets]->setSpriteDimensions(theTextureMgr->getTexture("bulletFire")->getTWidth(), theTextureMgr->getTexture("bulletFire")->getTHeight());
				fireBullets[fnumBullets]->setBulletVelocity({ 2.0f, 2.0f });
				fireBullets[fnumBullets]->setSpriteRotAngle(thePlayer.getSpriteRotAngle());
				fireBullets[fnumBullets]->setActive(true);
				cout << "Bullet added to Vector at position - x: " << thePlayer.getBoundingRect().x << " y: " << thePlayer.getBoundingRect().y << endl;
				
			}
				theSoundMgr->getSnd("shot")->play(0);

				
				break;

				case SDLK_2:
				{
					waterBullets.push_back(new cBullets);
					int wnumBullets = waterBullets.size() - 1;
					waterBullets[wnumBullets]->setSpritePos({ thePlayer.getBoundingRect().x + thePlayer.getSpriteCentre().x, thePlayer.getBoundingRect().y + thePlayer.getSpriteCentre().y });
					waterBullets[wnumBullets]->setSpriteTranslation({ 2.0f, 2.0f });
					waterBullets[wnumBullets]->setTexture(theTextureMgr->getTexture("bulletWater"));
					waterBullets[wnumBullets]->setSpriteDimensions(theTextureMgr->getTexture("bulletWater")->getTWidth(), theTextureMgr->getTexture("bulletWater")->getTHeight());
					waterBullets[wnumBullets]->setBulletVelocity({ 2.0f, 2.0f });
					waterBullets[wnumBullets]->setSpriteRotAngle(thePlayer.getSpriteRotAngle());
					waterBullets[wnumBullets]->setActive(true);
					cout << "Bullet added to Vector at position - x: " << thePlayer.getBoundingRect().x << " y: " << thePlayer.getBoundingRect().y << endl;
				
				theSoundMgr->getSnd("shot")->play(0);

				}
				break;

				case SDLK_3:
				{
					earthBullets.push_back(new cBullets);
					int enumBullets = earthBullets.size() - 1;
					earthBullets[enumBullets]->setSpritePos({ thePlayer.getBoundingRect().x + thePlayer.getSpriteCentre().x, thePlayer.getBoundingRect().y + thePlayer.getSpriteCentre().y });
					earthBullets[enumBullets]->setSpriteTranslation({ 2.0f, 2.0f });
					earthBullets[enumBullets]->setTexture(theTextureMgr->getTexture("bulletEarth"));
					earthBullets[enumBullets]->setSpriteDimensions(theTextureMgr->getTexture("bulletEarth")->getTWidth(), theTextureMgr->getTexture("bulletEarth")->getTHeight());
					earthBullets[enumBullets]->setBulletVelocity({ 2.0f, 2.0f });
					earthBullets[enumBullets]->setSpriteRotAngle(thePlayer.getSpriteRotAngle());
					earthBullets[enumBullets]->setActive(true);
					cout << "Bullet added to Vector at position - x: " << thePlayer.getBoundingRect().x << " y: " << thePlayer.getBoundingRect().y << endl;

				}
				theSoundMgr->getSnd("shot")->play(0);

				
				break;

				case SDLK_4:
				{
					airBullets.push_back(new cBullets);
					int anumBullets = airBullets.size() - 1;
					airBullets[anumBullets]->setSpritePos({ thePlayer.getBoundingRect().x + thePlayer.getSpriteCentre().x, thePlayer.getBoundingRect().y + thePlayer.getSpriteCentre().y });
					airBullets[anumBullets]->setSpriteTranslation({ 2.0f, 2.0f });
					airBullets[anumBullets]->setTexture(theTextureMgr->getTexture("bulletAir"));
					airBullets[anumBullets]->setSpriteDimensions(theTextureMgr->getTexture("bulletAir")->getTWidth(), theTextureMgr->getTexture("bulletAir")->getTHeight());
					airBullets[anumBullets]->setBulletVelocity({ 2.0f, 2.0f });
					airBullets[anumBullets]->setSpriteRotAngle(thePlayer.getSpriteRotAngle());
					airBullets[anumBullets]->setActive(true);
					cout << "Bullet added to Vector at position - x: " << thePlayer.getBoundingRect().x << " y: " << thePlayer.getBoundingRect().y << endl;

				}
				theSoundMgr->getSnd("shot")->play(0);

				
				break;

				default:
					break;
				}

			default:
				break;
		}

	}
	return theLoop;
}


double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

