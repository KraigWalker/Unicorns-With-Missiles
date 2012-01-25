// Unicorns with Missile Launcers!
// A Micro-game by Kraig Walker
// Student No. 1000272
// Source - main.cpp 

// Include Files

#include "main.h"

// Game Engine Functions

///<summary>
/// Initialize Game Engine & hide system cursor (we've got our own)
///</summary>
BOOL GameInitialize(HINSTANCE hInstance)
{
	// Hide the Windows Mouse Cursor
	ShowCursor(FALSE);
  // Create the GameEngine
  _pGame = new GameEngine(hInstance, TEXT("Unicorns With Missiles!"),
    TEXT("Unicorns With Missiles!"), IDI_ICON, IDI_ICON, 600, 450);
  if (_pGame == NULL)
    return FALSE;

  // Set the frame rate
  _pGame->SetFrameRate(30);

  // Store the instance handle
  _hInstance = hInstance;

  return TRUE;
}

///<summary>
// GameStart() doesn't start a new game, is 
// used to initialize our bitmaps & drawing surfaces.
// Once this is done, NewGame() takes the reigns.
///</summary>
void GameStart(HWND hWindow)
{
	// Seed the random number generator
	srand(GetTickCount());

	// Create the offscreen device context and bitmap
	_hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
	_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow), 
	_pGame->GetWidth(), _pGame->GetHeight());
	SelectObject(_hOffscreenDC, _hOffscreenBitmap);

	// Create and load the bitmaps
	HDC hDC = GetDC(hWindow);
	_pBackgroundBitmap = new Bitmap(hDC, IDB_BACKGROUND, _hInstance);
	_pGroundBitmap = new Bitmap(hDC, IDB_GROUND, _hInstance);
	_pTargetBitmap = new Bitmap(hDC, IDB_TARGET, _hInstance);
	_pUnicorn1Bitmap = new Bitmap(hDC, IDB_UNICORNS1, _hInstance);
	_pUnicorn2Bitmap = new Bitmap(hDC, IDB_UNICORNS2, _hInstance);
	_pUnicorn3Bitmap = new Bitmap(hDC, IDB_UNICORNS3, _hInstance);
	_pUnicorn4Bitmap = new Bitmap(hDC, IDB_UNICORNS4, _hInstance);
	_pMeteorBitmap = new Bitmap(hDC, IDB_METEOR, _hInstance);
	_pMissileBitmap = new Bitmap(hDC, IDB_MISSILE, _hInstance);
	_pExplosionBitmap = new Bitmap(hDC, IDB_EXPLOSION, _hInstance);
	_pGameOverBitmap = new Bitmap(hDC, IDB_GAMEOVER, _hInstance);
	_pPausedBitmap = new Bitmap(hDC, IDB_PAUSED, _hInstance);
	_pStartBitmap = new Bitmap(hDC, IDB_START, _hInstance);


	// Create the Splash Screen background:
	// "Why have the same boring old splash screen when you can have
	// FOUR splash screens!?"
	int iRand = rand()%4;
	switch(iRand)
	{
	case 0: 
		  {
			  _pSplashBitmap = new Bitmap(hDC, IDB_SPLASH1, _hInstance);
			  _pSplashBackBitmap = new Bitmap(hDC, 600, 450, RGB(71,131,136));
			  break;
		  }
	  case 1:
		  {
			  _pSplashBitmap = new Bitmap(hDC, IDB_SPLASH4, _hInstance);
			  _pSplashBackBitmap = new Bitmap(hDC, 600, 450, RGB(240,240,216));
			  break;
		  }
	  case 2:
		  {
			  _pSplashBitmap = new Bitmap(hDC, IDB_SPLASH3, _hInstance);
			  _pSplashBackBitmap = new Bitmap(hDC, 600, 450, RGB(171,230,67));
			  break;
		  }
	  case 3:
		  {
			  _pSplashBitmap = new Bitmap(hDC, IDB_SPLASH2, _hInstance);
			  _pSplashBackBitmap = new Bitmap(hDC, 600, 450, RGB(255,87,129));
			  break;
		  }
	  }

	// Create the Background
	_pBackground = new Background(_pBackgroundBitmap);

	// Start the game
	NewGame();
}

///<summary>
/// Safely cleans up all of our game variables
///</summary>
void GameEnd()
{
  // Cleanup the offscreen device context and bitmap
  DeleteObject(_hOffscreenBitmap);
  DeleteDC(_hOffscreenDC);  

  // Cleanup the asteroid bitmap
  delete _pGroundBitmap;
  delete _pTargetBitmap;
  delete _pUnicorn1Bitmap;
  delete _pUnicorn2Bitmap;
  delete _pUnicorn3Bitmap;
  delete _pUnicorn4Bitmap;
  delete _pMeteorBitmap;
  delete _pMissileBitmap;
  delete _pExplosionBitmap;
  delete _pGameOverBitmap;
  delete _pPausedBitmap;
  delete _pStartBitmap;

  // Cleanup the background
  delete _pBackground;

  // Cleanup the sprites
  _pGame->CleanupSprites();

  // Cleanup the game engine
  delete _pGame;
}

///<summary>
/// Reacts to when the program is made the current active window
///</summary>
void GameActivate(HWND hWindow)
{
	// This was useful in debugging, to mess with pausing the game
	// but doesn't have a use in the game anymore.
}

///<summary>
// Reacts to when the program is made the current active window
// If the game is being played, automatically pause it for the player
///</summary>
void GameDeactivate(HWND hWindow)
{
}

///<summary>
/// Draws game images to the window on WM_PAINT message
///</summary>
void GamePaint(HDC hDC)
{
    // Draw the background
    _pBackground->Draw(hDC);

    // Draw the ground bitmap

	_pGroundBitmap->Draw(hDC, 0, 398, TRUE);
	
	// Draw Splash Screen
    if(_bSplash)
    {
		// Draw the splash screen Image
		_pSplashBackBitmap->Draw(hDC, 0, 0);
		_pSplashBitmap->Draw(hDC, 50, 25);
	}
	// Draw Start Screen
	if(_bStart)
	{
		_pStartBitmap->Draw(hDC, 0, 0);
	}

	// Draw Game
	if(_bGameRunning)
	{
		// Draw the sprites
		_pGame->DrawSprites(hDC);
		
		//Draw the score
		TCHAR szText[64];
		RECT rect = { 275, 0, 325, 50 };
		wsprintfA((LPSTR)szText, "%d", _iScore);			//Use wsprintfW to get cool Tiwanese characters instead of your score!
		SetBkMode(hDC, TRANSPARENT);
		SetTextColor(hDC, RGB(255, 255, 255));
		DrawTextA(hDC, (LPCSTR)szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		// The Pause Screen & Game Over screen draw over the game, so they remain in the "if" depth   

		// Draw the game over message, if necessary
		if (_bGameOver)
		{
			_pGameOverBitmap->Draw(hDC, 170, 150, TRUE);
		}
		// Draw the paused screen message
		if(_bPause)
		{
			_pPausedBitmap->Draw(hDC, 0, 0, TRUE);	
		}
	}
}

///<summary>
/// This function controls the flow of your typical game cycle
/// on screen items are updated, added and removed, as well as
/// "GamePaint();-ing" to our back buffer and flipping it to 
/// the front window.
///</summary>
void GameCycle()
{
	if(_bSplash)
	{
		// Carry out Splash Screen Logic
		if(_bSplash && _iSplashTimer == 60)
		{
				// end Splash Screen
			_bSplash = FALSE;
			_bStart = TRUE;
		}
		else
		{
			_iSplashTimer++;
		}
	}
	if(_bStart)
	{
		_bGameRunning = FALSE;
	}
	if(_bGameRunning && !_bPause && !_bGameOver)
	{
		// Randomly add meteors
		if((rand()%_iDifficulty) == 0)
		{
			AddMeteor();
		}

		// Update the background
		_pBackground->Update();
			
		// Update the sprites
		_pGame->UpdateSprites();
	}	
			// Obtain a device context for repainting the game
		HWND  hWindow = _pGame->GetWindow();
		HDC   hDC = GetDC(hWindow);

		// Paint the game to the offscreen device context
		GamePaint(_hOffscreenDC);

		// Blit the offscreen bitmap to the game screen
		BitBlt(hDC, 0, 0, _pGame->GetWidth(), _pGame->GetHeight(),
		_hOffscreenDC, 0, 0, SRCCOPY);

		// Cleanup
		ReleaseDC(hWindow, hDC);
}

///<summary>
/// Responds to keyboard input from the user;
/// This game isn't that big on keyboard use,
/// but you can pause the game and shut it down
/// by just using the keyboard.
///</summary>
void HandleKeys()
{
	// If user presses the escape key
	if(GetAsyncKeyState(VK_ESCAPE) < 0)
	{
		if (_bGameRunning && !_bPause && !_bGameOver)	// If game is playing
		{
			// Pause the game
			_bPause = TRUE;
		}
	}
	// If the user presses Return
	if(GetAsyncKeyState(VK_RETURN) < 0)
	{
		if(_bPause)									// If game is paused
		{
			_bPause = FALSE;	// Resume Game
		}
		else if(_bSplash)							// If displaying splash screen
		{
			// Start a new game without the splash screen
			_bSplash = FALSE;
			_bStart = TRUE;		// Switch to Start Screen
		}
		else if(_bGameOver)							// If displaying game over screen
		{
			// Start a new game
			_bStart = TRUE;
		}
	}
	if(GetAsyncKeyState(VK_SPACE) < 0)
	{
		if(_bStart || _bGameOver)		// Allow space to be used on Game Over and Start Screen
		{
			_bStart = FALSE;
			_bGameRunning = TRUE;
			NewGame();
		}

		if(_bPause && !_bSplash && !_bGameOver)
		{
			//  Quit the program
			  PostQuitMessage(0);
		}
		else if(_bSplash)
		{
			_bStart = TRUE;
			
		}
	}
}

///<summary>
/// Respond to user mouse clicks
///</summary>
void MouseButtonDown(int x, int y, BOOL bLeft)
{
	if(!_bPause)					// I don't want you to fire missiles when the game is paused! Cheater!
	{
		if(!_bGameOver && bLeft)	// While game is playing
		{
			// Create a new missile sprite and set it's position
			RECT	rcBounds = { 0, 0, 600, 450 };
			int		iXPos = (x < 300) ? 144 : 449;
			Sprite* pSprite = new Sprite(_pMissileBitmap, rcBounds, BA_DIE);
			pSprite->SetPosition(iXPos, 365);

			// Calculate the velocity so that it is aimed at the target reticle
			int iXVel; 
			int iYVel = -6;
			y = min(y, 300);
			iXVel = (iYVel * ((iXPos + 8) - x)) / (365 -y);
			pSprite->SetVelocity(iXVel, iYVel);

			///<summary>
			/// I spent what felt like forever working on rotating these
			/// missile sprites. But it's just not practicle.
			/// PlgBlt() is a GDI function that maps pixels onto a non right angled rectangle
			/// (parallelogram to you and I) - this function is slow, complex to work with,
			/// and I actually took a performance hit, nor does Microsoft recommend using it.
			/// GDI+ solves thsi problem, but that's beyond the scope of the brief.
			/// instead, we'll have to settle for side-shuffling missiles...
			///</summary>

			// Add the missile Sprite
			_pGame->AddSprite(pSprite);

			// Update the Score
			///<summary>
			/// I found you could rapidly spam missiles into an endless flood of meteor-exploding
			/// madness. You can still do this, but you're score is going to suffer.
			///</summary>
			_iScore = max(--_iScore, 0);
		}
		else if(_bGameOver && !bLeft)
		NewGame();						// Start a new game if you're at the game over screen
	}
}

///<summary>
/// I put this hear with the intension of listening for when the mouse button
/// was lifted up, as more physical solution to missile spamming.
/// The player wouldn't be able to fire another missile till they lifted up the mouse
/// and waited a few ticks for their cannons to "reload" or whatever.
/// It was a low priority with respect to all the other stuff that needs
/// to be created for a working game engine. Another time perhaps?
///</summary>
void MouseButtonUp(int x, int y, BOOL bLeft)
{

}


///<summary>
/// Keeps track of the mouse position and updates our target reticle
/// accordingly
///</summary>
void MouseMove(int x, int y)
{
	// Target sprite position = the same position as where the (invisible) cursor is
	_pTargetSprite->SetPosition(x - (_pTargetSprite->GetWidth() / 2), y - (_pTargetSprite->GetHeight() / 2));
}

///<summary>
/// I dug into a bit of a black hole when trying to handle 
/// collisions with sprites. Originally, I had created
/// a sleuth of classes that inherited Sprite, and they
/// all had their own similar collision detection functions.
/// This got a bit complicated, but you can already tell what
/// type of world object a sprite is by getting it's bitmap
/// </summary>
BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
	// See if a missile and a meteor have collided
	if ((pSpriteHitter->GetBitmap() == _pMissileBitmap &&
		pSpriteHittee->GetBitmap() == _pMeteorBitmap) ||
		(pSpriteHitter->GetBitmap() == _pMeteorBitmap &&
		pSpriteHittee->GetBitmap() == _pMissileBitmap))
	{
		// Kill both sprites
		pSpriteHitter->Kill();
		pSpriteHittee->Kill();

		// Update the score
		_iScore += 6;
		_iDifficulty = max(50 - (_iScore / 10), 5);			// The higher you're score, the harder it gets.
	}

	// See if a meteor has collided with a Unicorn
	if((pSpriteHitter->GetBitmap() == _pMeteorBitmap &&		// This is probably an example of where using OOP
		pSpriteHittee->GetBitmap() == _pUnicorn1Bitmap)		// comes in handy.
		||													//
		(pSpriteHitter->GetBitmap() == _pMeteorBitmap &&	// Because there are four different Unicorn sprites,
		pSpriteHittee->GetBitmap() == _pUnicorn2Bitmap)		// all of this needs to be looked over.
		||
		(pSpriteHitter->GetBitmap() == _pMeteorBitmap &&
		pSpriteHittee->GetBitmap() == _pUnicorn3Bitmap)
		||
		(pSpriteHitter->GetBitmap() == _pMeteorBitmap &&
		pSpriteHittee->GetBitmap() == _pUnicorn4Bitmap))
	{
		// Kill both sprites
		pSpriteHitter->Kill();
		pSpriteHittee->Kill();

		// See if game is over
		if (--_iNumUnicorns == 0)							// If all Unicorns are obliterated, it's Game Over...
			_bGameOver = TRUE;
	}

  return FALSE;
}

///<summary>
/// SpriteDying only has an effect on sprites that have a Meteor Bitmap.
/// The goal of this function is to make a meteor explode when it collides
/// with the ground or a well-placed missile.
///</summary>
void SpriteDying(Sprite* pSprite)
{
	// See if a meteor sprite is dying
	if(pSprite->GetBitmap() == _pMeteorBitmap)
	{
		// Create an explosion sprite at the meteor's position
		RECT rcBounds = { 0, 0, 600, 450 };
		RECT rcPos = pSprite->GetPosition();
		Sprite* pSprite = new Sprite(_pExplosionBitmap, rcBounds);
		pSprite->SetNumFrames(12, TRUE);
		pSprite->SetPosition(rcPos.left, rcPos.top);
		_pGame->AddSprite(pSprite);
	}
}

///<summary>
/// Sets up all of our unicorns, and the target reticule sprite,
/// ready to begin the game.
///</summary>
void NewGame()
{
	// Clear the sprites
	_pGame->CleanupSprites();

	// Create the target sprite
	RECT rcBounds = { 0, 0, 600, 450 };
	_pTargetSprite  = new Sprite(_pTargetBitmap, rcBounds, BA_STOP);
	_pTargetSprite->SetZOrder(10);
	_pGame->AddSprite(_pTargetSprite);

	//Create the unicorns
	Sprite* pSprite;
	pSprite = new Sprite(_pUnicorn1Bitmap, rcBounds);
	pSprite ->SetPosition(2, 370);
	_pGame->AddSprite(pSprite);
	pSprite = new Sprite(_pUnicorn2Bitmap, rcBounds);
	pSprite ->SetPosition(186, 370);
	_pGame->AddSprite(pSprite);
	pSprite = new Sprite(_pUnicorn3Bitmap, rcBounds);
	pSprite ->SetPosition(302, 370);
	_pGame->AddSprite(pSprite);
	pSprite = new Sprite(_pUnicorn4Bitmap, rcBounds);
	pSprite ->SetPosition(490, 370);
	_pGame->AddSprite(pSprite);

	// Initialize the game variables
	_iScore = 0;
	_iNumUnicorns = 4;
	_iDifficulty = 50;
	_bGameOver = FALSE;
}

///<summary>
/// This function automatically generates a new meteor sprite when required.
/// "pSprite" is our sort of template sprite. Anything we define in this one,
/// will be realized in the game when we AddSprite() it into the Sprite vector
///</summary>
void AddMeteor()
{
	// Create a new meteor sprite and set it's position
	RECT	rcBounds = { 0, 0, 600, 390 };
	int		iXPos = rand() %600;
	Sprite*	pSprite = new Sprite(_pMeteorBitmap, rcBounds, BA_DIE);
	pSprite->SetNumFrames(14);					
	pSprite->SetPosition(iXPos, 0);

	// Calculate the velocity so that it is aimed at one of the Unicorns
	int iXVel;
	int iYVel = (rand() % 4) +3;
	switch(rand() %4)
	{
	case 0:
		iXVel = (iYVel * (56 - (iXPos + 50))) / 400;
		break;
	case 1:
		iXVel = (iYVel * (240 - (iXPos + 50))) / 400;
		break;
	case 2:
		iXVel = (iYVel * (360 - (iXPos + 50))) / 400;
	case 3:
		iXVel = (iYVel * (546 - (iXPos + 50))) / 400;
		break;
	}
	pSprite->SetVelocity(iXVel, iYVel);

	// Add the meteor sprite
	_pGame->AddSprite(pSprite);
}