// Kraig Walker
// Student No. 1000272
// Unicorns with Missiles Application
// C++ Header - main.h

#pragma once

// Include Files

#include <windows.h>
#include "resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"
#include "Background.h"

// Global Variables

HINSTANCE			_hInstance;
GameEngine*			_pGame;
HDC					_hOffscreenDC;
HBITMAP				_hOffscreenBitmap;
Bitmap*				_pAsteroidBitmap;
Bitmap*				_pGroundBitmap;
Bitmap*				_pTargetBitmap;
Bitmap*				_pUnicorn1Bitmap;
Bitmap*				_pUnicorn2Bitmap;
Bitmap*				_pUnicorn3Bitmap;
Bitmap*				_pUnicorn4Bitmap;
Bitmap*				_pMeteorBitmap;
Bitmap*				_pMissileBitmap;
Bitmap*				_pExplosionBitmap;
Bitmap*				_pGameOverBitmap;
Bitmap*				_pBackgroundBitmap;
Background*			_pBackground;
Sprite*				_pTargetSprite;
int					_iNumUnicorns, _iScore, _iDifficulty;
Bitmap*				_pSplashBitmap;
Bitmap*				_pSplashBackBitmap;
int					_iSplashTimer = 0;
Bitmap*				_pPausedBitmap;
Bitmap*				_pStartBitmap;


BOOL				_bStart = FALSE;
BOOL				_bSplash = TRUE;		// Initially display the Splash
BOOL				_bPause = FALSE;		// Pause the game loop
BOOL				_bGameOver = FALSE;				// Stop current game
BOOL				_bGameRunning = FALSE;
// Function Declarations

void NewGame();
void AddMeteor();