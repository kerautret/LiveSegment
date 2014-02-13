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

int Pixel::manhattan (Pixel p)
{
  return (((p.xp > xp) ? p.xp - xp : xp - p.xp)
          + ((p.yp > yp) ? p.yp - yp : yp - p.yp));
}

void Pixel::translate (double dist, double dx, double dy)
{
  xp += dist * dx;
  yp += dist * dy;
}

Pixel *Pixel::drawing (Pixel p, int *n)
{
  int x1, y1, x2, y2;
  if (xp > p.xp)
  {
    x1 = p.xp;
    x2 = xp;
    y1 = p.yp;
    y2 = yp;
  }
  else
  {
    x1 = xp;
    x2 = p.xp;
    y1 = yp;
    y2 = p.yp;
  }
  int dx = x2 - x1;
  int dy = y2 - y1;
  int e, i = 0;
  Pixel *pts;

  if (dy > 0)
  {
    // Octant 1
    if (dx >= dy)
    {
      *n = dx + 1;
      pts = new Pixel[dx + 1];
      e = dx;
      dx *= 2;
      dy *= 2;
      while (x1 < x2)
      {
        pts[i++] = Pixel (x1, y1);
        x1 ++;
        e -= dy;
        if (e < 0)
        {
          y1 ++;
          e += dx;
        }
      }
      pts[i] = Pixel (x1, y1);
    }

    // Octant 2
    else
    {
      *n = dy + 1;
      pts = new Pixel[dy + 1];
      e = dy;
      dx *= 2;
      dy *= 2;
      while (y1 < y2)
      {
        pts[i++] = Pixel (x1, y1);
        y1 ++;
        e -= dx;
        if (e < 0)
        {
          x1 ++;
          e += dy;
        }
      }
      pts[i] = Pixel (x1, y1);
    }
  }

  else
  {
    // Octant 8
    if (dx >= -dy)
    {
      *n = 1 + dx;
      pts = new Pixel[dx + 1];
      e = dx;
      dx *= 2;
      dy *= 2;
      while (x1 < x2)
      {
        pts[i++] = Pixel (x1, y1);
        x1 ++;
        e += dy;
        if (e < 0)
        {
          y1 --;
          e += dx;
        }
      }
      pts[i] = Pixel (x1, y1);
    }

    // Octant 7
    else
    {
      *n = 1 - dy;
      pts = new Pixel[1 - dy];
      e = dy;
      dx *= 2;
      dy *= 2;
      while (y1 > y2)
      {
        pts[i++] = Pixel (x1, y1);
        y1 --;
        e += dx;
        if (e > 0)
        {
          x1 ++;
          e += dy;
        }
      }
      pts[i] = Pixel (x1, y1);
    }
  }
  return (pts);
}

Pixel *Pixel::pathTo (Pixel p, int *n)
{
  int x1, y1, x2, y2, delta;
  if (xp > p.xp)
  {
    x1 = p.xp;
    x2 = xp;
    y1 = p.yp;
    y2 = yp;
    delta = -1;
  }
  else
  {
    x1 = xp;
    x2 = p.xp;
    y1 = yp;
    y2 = p.yp;
    delta = 1;
  }
  int dx = x2 - x1;
  int dy = y2 - y1;
  int e, i = 0;
  Pixel *pts;

  if (dy > 0)
  {
    // Octant 1
    if (dx >= dy)
    {
      *n = dx;
      pts = new Pixel[dx];
      e = dx;
      dx *= 2;
      dy *= 2;
      while (x1 < x2)
      {
        x1 ++;
        e -= dy;
        if (e < 0)
        {
          y1 ++;
          e += dx;
          pts[i++] = Pixel (delta, delta);
        }
        else pts[i++] = Pixel (delta, 0);
      }
    }

    // Octant 2
    else
    {
      *n = dy;
      pts = new Pixel[dy];
      e = dy;
      dx *= 2;
      dy *= 2;
      while (y1 < y2)
      {
        y1 ++;
        e -= dx;
        if (e < 0)
        {
          x1 ++;
          e += dy;
          pts[i++] = Pixel (delta, delta);
        }
        else pts[i++] = Pixel (0, delta);
      }
    }
  }

  else
  {
    // Octant 8
    if (dx >= -dy)
    {
      *n = dx;
      pts = new Pixel[dx];
      e = dx;
      dx *= 2;
      dy *= 2;
      while (x1 < x2)
      {
        x1 ++;
        e += dy;
        if (e < 0)
        {
          y1 --;
          e += dx;
          pts[i++] = Pixel (delta, -delta);
        }
        else pts[i++] = Pixel (delta, 0);
      }
    }
    // Octant 7
    else
    {
      *n = dy;
      pts = new Pixel[-dy];
      e = dy;
      dx *= 2;
      dy *= 2;
      while (y1 > y2)
      {
        y1 --;
        e += dx;
        if (e > 0)
        {
          x1 ++;
          e += dy;
          pts[i++] = Pixel (delta, -delta);
        }
        else pts[i++] = Pixel (0, -delta);
      }
    }
  }
  return (pts);
}

bool *Pixel::stepsTo (Pixel p, int *n)
{
  int x2 = (xp > p.xp) ? xp - p.xp : p.xp - xp;
  int y2 = (yp > p.yp) ? yp - p.yp : p.yp - yp;
  int dx = x2, dy = y2;
  if (y2 > x2)
  {
    dx = y2;
    dy = x2;
    x2 = y2;
  }
  int e, x = 0, i = 0;
  *n = x2;
  bool *paliers = new bool[x2];

  e = dx;
  dx *= 2;
  dy *= 2;

  while (x < x2)
  {
    x ++;
    e -= dy;
    if (e < 0)
    {
      e += dx;
      paliers[i++] = true;
    }
    else paliers[i++] = false;
  }
  return (paliers); 
}
