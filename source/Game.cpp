/*
 * Rhythm Run for Nintendo 3DS
 * Lauren Kelly, 2020
 */

#include "debug.hpp"

#include "Game.hpp"
#include "Label.hpp"
#include "Entity.hpp"

#include <3ds.h>
#include <memory>

#include <citro2d.h>

Game::Game()
:upperScreen(GFX_TOP, GFX_LEFT), lowerScreen(GFX_BOTTOM, GFX_LEFT)
{
	eprintf("Init Labels\n");
	const char *hello1 = "Hello world!";
	myLabel = std::make_shared<Label>(hello1, 24.0f, 24.0f, 0, 0.5f, 0.5f, 0);
	const char *hello2 = "Hello world, again!";
	myLabel2 = std::make_shared<Label>(hello2, 24.0f, 24.0f, 0, 0.5f, 0.5f, 0);

	eprintf("Init sprite\n");
	C2D_SpriteSheet spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!spriteSheet) {
		eprintf("failed to load sprite sheet!!\n");
		// justSpin();
		svcBreak(USERBREAK_PANIC);
	}

	mySprite = std::make_shared<Entity>(5, 5, // position
			spriteSheet, 1, // which sprite sheet and index
			0.5f, 0.5f, // center
			0.5f, 0.5f, // scaling
			0 // angle
	);
	mySprite2 = std::make_shared<Entity>(64, 64, // position
			spriteSheet, 1, // which sprite sheet and index
			0.5f, 0.5f, // center
			0.5f, 0.5f, // scaling
			0 // angle
	);

	eprintf("Done\n");
};

void Game::update() {
	// ((void)0);
	hidScanInput();

	if(hidKeysHeld() & KEY_DUP && mySprite->getY() > 0) { // move sprite up
		eprintf("UP\n");
		mySprite->move(0, -2.5f); // move up 2.5px
	};
	if(hidKeysHeld() & KEY_DRIGHT && mySprite->getX() < 400) { // move sprite right
		eprintf("RIGHT\n");
		mySprite->move(2.5f, 0); // move right 2.5px
	};
	if(hidKeysHeld() & KEY_DDOWN && mySprite->getY() < 240) { // move sprite down
		eprintf("DOWN\n");
		mySprite->move(0, 2.5f); // move down 2.5px
	};
	if(hidKeysHeld() & KEY_DLEFT && mySprite->getX() > 0) { // move sprite left
		eprintf("LEFT\n");
		mySprite->move(-2.5f, 0); // move left 2.5px
	};

	// scale

	if(hidKeysHeld() & KEY_CPAD_UP) { // scale sprite Y larger
		eprintf("UP\n");
		mySprite->setScale(mySprite->getScaleX(), mySprite->getScaleY() * 1.1f); // scale Y 1.1x
		// mySprite->scale(1, 1.1f); // scale Y 1.1x
	};
	if(hidKeysHeld() & KEY_CPAD_RIGHT) { // scale sprite X larger
		eprintf("RIGHT\n");
		mySprite->setScale(mySprite->getScaleX() * 1.1f, mySprite->getScaleY()); // scale X 1.1x
		// mySprite->scale(1.1f, 1); // scale X 1.1x
	};
	if(hidKeysHeld() & KEY_CPAD_DOWN && mySprite->getScaleY() > 0) { // scale sprite Y smaller
		eprintf("DOWN\n");
		mySprite->setScale(mySprite->getScaleX(), mySprite->getScaleY() / 1.1f); // scale Y 1/1.1x
		// mySprite->scale(1, 1/1.1f); // scale Y 1/1.1x
	};
	if(hidKeysHeld() & KEY_CPAD_LEFT && mySprite->getScaleX() > 0) { // scale sprite X smaller
		eprintf("LEFT\n");
		mySprite->setScale(mySprite->getScaleX() / 1.1f, mySprite->getScaleY()); // scale X 1/1.1x
		// mySprite->scale(1 / 1.1f, 1); // scale X 1/1.1x
	};

	eprintf("update end (%x)\n", hidKeysHeld());
};

void Game::lateUpdate() { ((void)0); };

void Game::drawUpper() {
	// eprintf("BeginDrawU\n");
	upperScreen.beginDraw();

	float dX = 0.5f;
	float dY = 0.5f;

	float pX = myLabel->getX();
	float pY = myLabel->getY();

	if(pX >= 400 || pX <= 0) dX = -dX;
	if(pY >= 240 || pY <= 0) dY = -dY;

	myLabel->setPosition(pX + dX, pY + dY);

	upperScreen.draw(myLabel);

	upperScreen.draw(mySprite);

	upperScreen.endDraw();
	// eprintf("End\n");
};

void Game::drawLower() {
	lowerScreen.beginDraw();

	float dX = 0.5f;
	float dY = 0.5f;

	float pX = myLabel2->getX();
	float pY = myLabel2->getY();

	if(pX <= 0 || pX >= 320) dX = -dX;
	if(pY <= 0 || pY >= 240) dY = -dY;

	myLabel2->setPosition(pX + dX, pY + dY);

	lowerScreen.draw(myLabel2);
	lowerScreen.draw(mySprite2);

	lowerScreen.endDraw();
};