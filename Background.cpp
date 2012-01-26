 // Kraig Walker
// Background Object
// Source - Background.cpp

// Include Files

#include "Background.h"

// Background Constructor(s)/Destructor

///<summary>
/// There are two types of Backgrounds:
/// 1. Create a flat colour background from scratch
/// 2. use a bitmap object.
///</summary>
Background::Background(int iWidth, int iHeight, COLORREF crColor)
{
  // Initialize the member variables
  m_iWidth = iWidth;
  m_iHeight = iHeight;
  m_crColor = crColor;
  m_pBitmap = NULL;
}

Background::Background(Bitmap* pBitmap)
{
  // Initialize the member variables
  m_crColor = 0;
  m_pBitmap = pBitmap;
  m_iWidth = pBitmap->GetWidth();
  m_iHeight = pBitmap->GetHeight();
}

Background::~Background()
{
}


// Background General Methods

void Background::Update()
{
  // Do nothing since the basic background is static
}

void Background::Draw(HDC hDC)
{
  // Draw the background
  if (m_pBitmap != NULL)			// if an image background
    m_pBitmap->Draw(hDC, 0, 0);
  else								// if a flat colour background
  {
    RECT    rect = { 0, 0, m_iWidth, m_iHeight };
    HBRUSH  hBrush = CreateSolidBrush(m_crColor);
    FillRect(hDC, &rect, hBrush);
    DeleteObject(hBrush);
  }
}
