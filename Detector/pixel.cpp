#include "pixel.h"


Pixel::Pixel ()
{
  xp = 0;
  yp = 0;
}

Pixel::Pixel (int x, int y)
{
  xp = x;
  yp = y;
}

Pixel::Pixel (const Pixel &p)
{
  xp = p.xp;
  yp = p.yp;
}

int Pixel::x ()
{
  return (xp);
}

int Pixel::y ()
{
  return (yp);
}

void Pixel::translate (double dist, double dx, double dy)
{
  xp += dist * dx;
  yp += dist * dy;
}
