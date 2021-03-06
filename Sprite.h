// Kraig Walker
// Student No. 1000272
// Sprite Object
// C++ Header - Sprite.h

#pragma once

// Include Files
#include <windows.h>
#include "Bitmap.h"

// Custom Data Types
typedef WORD        SPRITEACTION;
const SPRITEACTION  SA_NONE   = 0x0000L,
                    SA_KILL   = 0x0001L;

// Sprites have a state that tells them how to 
// react when they go out of the window bounds

typedef WORD        BOUNDSACTION;
const BOUNDSACTION  BA_STOP   = 0,			// Stop moving
                    BA_WRAP   = 1,			// appear on the other side of screen
                    BA_BOUNCE = 2,			// Bounce off of the edge of the screen
                    BA_DIE    = 3;			// Kill the sprite

// Sprite Class
class Sprite
{
protected:
  // Member Variables
  Bitmap*       m_pBitmap;							// Image displayed by the sprite
  int           m_iNumFrames, m_iCurFrame;			// Animation counters
  int           m_iFrameDelay, m_iFrameTrigger;		//
  RECT          m_rcPosition,						// Holds the area of the screen that the sprite appears on.
                m_rcCollision;						// Hold's a slightly smaller area than m_rcPosition, used for Collsion detection
  POINT         m_ptVelocity;						// holds x & y velocity (for missile & meteor sprites)
  int           m_iZOrder;							// Order that the sprite is drawn on the screen
  RECT          m_rcBounds;							// Area used to detect when the sprite is going off screen.
  BOUNDSACTION  m_baBoundsAction;					// Behaviour of the sprite when it collides with screen edges
  BOOL          m_bHidden;							// Hide the sprite from view
  BOOL			m_bDying;							// Determine whether a sprite has been flagged as dying
  BOOL			m_bOneCycle;						// Animate once through frame cycles then die

  // Helper Methods
  void          UpdateFrame();
  virtual void  CalcCollisionRect();

public:
  // Constructor(s)/Destructor
  Sprite(Bitmap* pBitmap);
  Sprite(Bitmap* pBitmap, RECT& rcBounds,
    BOUNDSACTION baBoundsAction = BA_STOP);
  Sprite(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder,
    RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP);
  virtual ~Sprite();

  // General Methods
  virtual SPRITEACTION  Update();
  void                  Draw(HDC hDC);
  BOOL                  IsPointInside(int x, int y);
  BOOL                  TestCollision(Sprite* pTestSprite);

  // Accessor Methods
  Bitmap* GetBitmap()               { return m_pBitmap; };
  void    SetNumFrames(int iNumFrames, BOOL bOneCycle = FALSE);
  void    SetFrameDelay(int iFrameDelay) { m_iFrameDelay = iFrameDelay; };
  RECT&   GetPosition()             { return m_rcPosition; };
  void    SetPosition(int x, int y);
  void    SetPosition(POINT ptPosition);
  void    SetPosition(RECT& rcPosition);
  void    OffsetPosition(int x, int y);
  RECT&   GetCollision()            { return m_rcCollision; };
  POINT   GetVelocity()             { return m_ptVelocity; };
  void    SetVelocity(int x, int y);
  void    SetVelocity(POINT ptVelocity);
  BOOL    GetZOrder()               { return m_iZOrder; };
  void    SetZOrder(int iZOrder)    { m_iZOrder = iZOrder; };
  void    SetBounds(RECT& rcBounds) { CopyRect(&m_rcBounds, &rcBounds); };
  void    SetBoundsAction(BOUNDSACTION ba) { m_baBoundsAction = ba; };
  BOOL    IsHidden()                { return m_bHidden; };
  void    SetHidden(BOOL bHidden)   { m_bHidden = bHidden; };
  int     GetWidth()
    { return (m_pBitmap->GetWidth() / m_iNumFrames); };
  int     GetHeight()               { return m_pBitmap->GetHeight(); };
  void	  Kill()					{ m_bDying = TRUE; };

};

// Sprite Inline Helper Methods
inline void Sprite::UpdateFrame()
{
  if ((m_iFrameDelay >= 0) && (--m_iFrameTrigger <= 0))
  {
    // Reset the frame trigger;
    m_iFrameTrigger = m_iFrameDelay;

    // Increment the frame
    if (++m_iCurFrame >= m_iNumFrames)
	{
		if(m_bOneCycle)
			m_bDying = TRUE;
		else
        m_iCurFrame = 0;
	}
  }
}

///<summary>
/// The collision rectangle is a 6th smaller than the Position rectangle
///</summary>
inline void Sprite::CalcCollisionRect()
{
  int iXShrink = (m_rcPosition.left - m_rcPosition.right) / 12;
  int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / 12;
  CopyRect(&m_rcCollision, &m_rcPosition);
  InflateRect(&m_rcCollision, iXShrink, iYShrink);
}


// Sprite Inline General Methods

///<summary>
/// returns true if the sprite being tested is 
/// colliding with the sprite carrying out the test.
///</summary>
inline BOOL Sprite::TestCollision(Sprite* pTestSprite)
{
  RECT& rcTest = pTestSprite->GetCollision();
  return m_rcCollision.left <= rcTest.right &&
         rcTest.left <= m_rcCollision.right &&
         m_rcCollision.top <= rcTest.bottom &&
         rcTest.top <= m_rcCollision.bottom;
}

inline BOOL Sprite::IsPointInside(int x, int y)
{
  POINT ptPoint;
  ptPoint.x = x;
  ptPoint.y = y;
  return PtInRect(&m_rcPosition, ptPoint);
}

// Sprite Inline Accessor Methods
inline void Sprite::SetNumFrames(int iNumFrames, BOOL bOneCycle)
{
  // Set the number of animation frames
  m_iNumFrames = iNumFrames;
  m_bOneCycle = bOneCycle;		// if bOneCycle = true; Play through once, then die (for explosion sprites).

  // Recalculate the position
  RECT rect = GetPosition();
  rect.right = rect.left + ((rect.right - rect.left) / iNumFrames);
  SetPosition(rect);
}

///<summary>
/// It's always tough when you have limited options,
/// fortunately, there are three different ways of setting the sprite's
/// position.
///</summary>

inline void Sprite::SetPosition(int x, int y)
{
  OffsetRect(&m_rcPosition, x - m_rcPosition.left, y - m_rcPosition.top);
  CalcCollisionRect();
}

inline void Sprite::SetPosition(POINT ptPosition)
{
  OffsetRect(&m_rcPosition, ptPosition.x - m_rcPosition.left,
    ptPosition.y - m_rcPosition.top);
  CalcCollisionRect();
}

inline void Sprite::SetPosition(RECT& rcPosition)
{
  CopyRect(&m_rcPosition, &rcPosition);
  CalcCollisionRect();
}

inline void Sprite::OffsetPosition(int x, int y)
{
  OffsetRect(&m_rcPosition, x, y);
  CalcCollisionRect();
}

inline void Sprite::SetVelocity(int x, int y)
{
  m_ptVelocity.x = x;
  m_ptVelocity.y = y;
}

inline void Sprite::SetVelocity(POINT ptVelocity)
{
  m_ptVelocity.x = ptVelocity.x;
  m_ptVelocity.y = ptVelocity.y;
}