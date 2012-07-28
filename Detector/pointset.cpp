#include "pointset.h"
#include <math.h>


vector<QPoint> PointSet::tracerSegment (QPoint p1, QPoint p2)
{
  int dx, dy, e;
  int x1, x2, y1, y2;

  vector<QPoint> vectPoints;
  x1 = p1.x ();
  x2 = p2.x ();
  y1 = p1.y ();
  y2 = p2.y ();

  dx = x2 - x1;
  dy = y2 - y1;

  // 1er quadrant
  if (dx > 0 && dy > 0)
  {
    // 1er octant
    if (dx >= dy)
    {
      e = dx;
      dx = dx * 2;
      dy = dy * 2;
      while (x1 != x2)
      {
        vectPoints.push_back (QPoint (x1, y1));
        x1 = x1 + 1;
        e = e - dy;
        if (e < 0)
        {
          y1 = y1 + 1;
          e = e + dx;
        }
      }
      vectPoints.push_back (QPoint (x1, y1));
    }
    // 2eme octant
    else if (dx < dy)
    {
      e = dy;
      dx = dx * 2;
      dy = dy * 2;
      while (y1 != y2)
      {
        vectPoints.push_back (QPoint (x1, y1));
        y1 = y1 + 1;
        e = e - dx;
        if (e < 0)
        {
          x1 = x1 + 1;
          e = e + dy;
        }
      }
      vectPoints.push_back (QPoint (x1, y1));
    }
  }

  // 4er quadrant
  else if (dx > 0 && dy < 0)
  {
    // 8ieme octant
    if (dx >= -dy)
    {
      e = dx;
      dx = dx * 2;
      dy = dy * 2;
      while (x1 != x2)
      {
        vectPoints.push_back (QPoint (x1, y1));
        x1 = x1 + 1;
        e = e + dy;
        if (e < 0)
        {
          y1 = y1 - 1;
          e = e + dx;
        }
      }
      vectPoints.push_back (QPoint (x1, y1));
    }
    else
    {
      // 7ieme octant
      e = dy;
      dx = dx * 2;
      dy = dy * 2;
      while (y1 != y2)
      {
        vectPoints.push_back (QPoint (x1, y1));
        y1 = y1 - 1;
        e = e + dx;
        if (e > 0)
        {
          x1 = x1 + 1;
          e = e + dy;
        }
      }
      vectPoints.push_back (QPoint (x1, y1));
    }
  }
  else if (dy == 0 && dx > 0)
  {
    while (x1 != x2)
    {
      vectPoints.push_back (QPoint (x1, y1));
      x1 = x1 + 1;
    }
    vectPoints.push_back (QPoint (x1, y1));
  }

  // 2ieme quadrant
  if (dx < 0 && dy > 0)
  {
    // 4ieme octant
    if (-dx >= dy)
    {
      e = dx;
      dx = dx * 2;
      dy = dy * 2;
      while (x1 != x2)
      {
	vectPoints.push_back (QPoint (x1, y1));
	x1 = x1 - 1;
	e = e + dy;
	if (e >= 0)
        {
	  y1 = y1 + 1;
	  e = e + dx;
	}
      }
      vectPoints.push_back (QPoint (x1, y1));
    }
    // 3ieme octant
    else if (-dx < dy)
    {
      e = dy;
      dx = dx * 2;
      dy = dy * 2;
      while (y1 != y2)
      {
        vectPoints.push_back (QPoint (x1, y1));
        y1 = y1 + 1;
        e = e + dx;
        if (e <= 0)
        {
          x1 = x1 - 1;
          e = e + dy;
        }
      }
      vectPoints.push_back (QPoint (x1, y1));
    }
  }

  // 3er quadrant
  else if (dx < 0 && dy < 0)
  {
    // 5ieme octant
    if (dx <= dy)
    {
      e = dx;
      dx = dx * 2;
      dy = dy * 2;
      while (x1 != x2)
      {
        vectPoints.push_back (QPoint (x1, y1));
        x1 = x1 - 1;
        e = e - dy;
        if (e >= 0)
        {
          y1 = y1 - 1;
          e = e + dx;
        }
      }
      vectPoints.push_back (QPoint (x1, y1));
    }
    else
    {
      // 6ieme octant
      e = dy;
      dx = dx * 2;
      dy = dy * 2;
      while (y1 != y2)
      {
        vectPoints.push_back (QPoint (x1, y1));
        y1 = y1 - 1;
        e = e - dx;
        if (e >= 0)
        {
          x1 = x1 - 1;
          e = e + dy;
        }
      }
      vectPoints.push_back (QPoint (x1, y1));
    }
  }
  else if (dy == 0 && dx < 0)
  {
    while (x1 != x2)
    {
      vectPoints.push_back(QPoint(x1,y1));
      x1 = x1 - 1;
    }
    vectPoints.push_back (QPoint (x1, y1));
  }
  else if (dx == 0 && dy > 0)
  {
    while (y1 != y2)
    {
      vectPoints.push_back (QPoint (x1, y1));
      y1 = y1 + 1;
    }
    vectPoints.push_back (QPoint (x1, y1));
  }
  else if (dx == 0 && dy < 0)
  {
    while (y1 != y2)
    {
      vectPoints.push_back (QPoint (x1, y1));
      y1 = y1 - 1;
    }
    vectPoints.push_back (QPoint (x1, y1));
  }
  return vectPoints;
}


vector<QPoint> PointSet::tracerSegment (QPoint p1, QPoint p2, int n)
{
  vector<QPoint> pts;

  int x1 = p1.x ();
  int x2 = p2.x ();
  int y1 = p1.y ();
  int y2 = p2.y ();
  int dx = x2 - x1;
  int dy = y2 - y1;
  int e;

  double foot = dx * dy / (double) (dx * dx + dy * dy);
  if (foot < 0.) foot = -foot;
  int floors, steps = (int) (n * foot + (n < 0 ? -0.5 : 0.5));

  if (dx > 0 && dy > 0)   // Quadrant 1
  {
    if (dx >= dy) // Octant 1
    {
      e = dx;
      dx = dx * 2;
      dy = dy * 2;
      if (n < 0) floors = (steps * dy - e) / dx;
      else floors = (e - 1 + steps * dy) / dx;
      e += floors * dx - steps * dy;
      x1 += steps;
      y1 -= n - floors;
      x2 += steps;
      // y2 -= n - floors;   // inutile
      while (x1 < x2)
      {
        pts.push_back (QPoint (x1, y1));
        x1 ++;
        e = e - dy;
        if (e < 0)
        {
          y1 ++;
          e = e + dx;
        }
      }
      pts.push_back (QPoint (x1, y1));
    }
    else if (dx < dy) // Octant 2
    {
      e = dy;
      dx = dx * 2;
      dy = dy * 2;
      if (n < 0) floors = (1 - e + steps * dx) / dy;
      else floors = (e + steps * dx) / dy;
      e -= floors * dy - steps * dx;
      x1 += n - floors;
      y1 -= steps;
      // x2 += n - floors;  // inutile
      y2 -= steps;  // inutile
      while (y1 < y2)
      {
        pts.push_back (QPoint (x1, y1));
        y1 ++;
        e -= dx;
        if (e < 0)
        {
          x1 ++;
          e += dy;
        }
      }
      pts.push_back (QPoint (x1, y1));
    }
  }

  else if (dx > 0 && dy < 0) // Quadrant 4
  {
    if (dx >= -dy) // Octant 8
    {
      e = dx;
      dx = dx * 2;
      dy = dy * 2;
      if (n < 0) floors = (e - 1 + steps * dy) / dx;
      else floors = (steps * dy - e) / dx;   // < 0
      e += floors * dx - steps * dy;
      x1 -= steps;
      y1 -= n + floors;
      x2 -= steps;
      // y2 -= n + floors;  // inutile
      while (x1 < x2)
      {
        pts.push_back (QPoint (x1, y1));
        x1 ++;
        e += dy;
        if (e < 0)
        {
          y1 --;
          e += dx;
        }
      }
      pts.push_back (QPoint (x1, y1));
    }
    else // Octant 7
    {
      e = dy;
      dx = dx * 2;
      dy = dy * 2;
      if (n < 0) floors = - (e + steps * dx) / dy;
      else floors = (e + 1 - steps * dx) / dy;
      e += floors * dy + steps * dx;
      x1 -= n - floors;
      y1 -= steps;
      // x2 -= n - floors;  // inutile
      y2 -= steps;
      while (y1 > y2)
      {
        pts.push_back (QPoint (x1, y1));
        y1 --;
        e += dx;
        if (e > 0)
        {
          x1 ++;
          e += dy;
        }
      }
      pts.push_back (QPoint (x1, y1));
    }
  }
  else if (dy == 0 && dx > 0)
    while (x1 <= x2) pts.push_back (QPoint (x1++, y1 - n));

  if (dx < 0 && dy > 0)    // Quadrant 2
  {
    if (-dx >= dy) // Octant 4
    {
      e = dx;
      dx = dx * 2;
      dy = dy * 2;
      if (n < 0) floors = - (steps * dy + e) / dx;
      else floors = (e + 1 - steps * dy) / dx;  // > 0
      e -= steps * dy + floors * dx;
      x1 += steps;
      y1 += n - floors;
      x2 += steps;
      // y2 += n - floors;   // inutile
      while (x1 > x2)
      {
        pts.push_back (QPoint (x1, y1));
        x1 --;
        e += dy;
        if (e >= 0)
        {
          y1 ++;
          e += dx;
        }
      }
      pts.push_back (QPoint (x1, y1));
    }
    else if (-dx < dy) // Octant 3
    {
      e = dy;
      dx = dx * 2;
      dy = dy * 2;
      if (n < 0) floors = (1 - e - steps * dx) / dy;
      else floors = (e - steps * dx) / dy;
      e += floors * dy + steps * dx;
      x1 += n - floors;
      y1 += steps;
      // x2 += n - floors;   // inutile
      y2 += steps;
      while (y1 < y2)
      {
        pts.push_back (QPoint (x1, y1));
        y1 ++;
        e += dx;
        if (e <= 0)
        {
          x1 --;
          e += dy;
        }
      }
      pts.push_back (QPoint (x1, y1));
    }
  }

  else if (dx < 0 && dy < 0)    // Quadrant 3
  {
    if (dx <= dy) // Octant 5
    {
      e = dx;
      dx = dx * 2;
      dy = dy * 2;
      if (n < 0) floors = (steps * dy - e - 1) / dx;
      else floors = (steps * dy + e) / dx;
      e += floors * dx - steps * dy;
      x1 -= steps;
      y1 += n - floors;
      x2 -= steps;
      // y2 += n - floors;  // inutile
      while (x1 > x2)
      {
        pts.push_back (QPoint (x1, y1));
        x1 --;
        e -= dy;
        if (e >= 0)
        {
          y1 --;
          e += dx;
        }
      }
      pts.push_back (QPoint (x1, y1));
    }
    else // Octant 6
    {
      e = dy;
      dx = dx * 2;
      dy = dy * 2;
      if (n < 0) floors = (e - steps * dx) / dy;  // > 0
      else floors = - (steps * dx + e + 1) / dy;
      e += floors * dy + steps * dx;
      x1 -= n + floors;
      y1 += steps;
      // x2 -= n - floors;  // inutile
      y2 += steps;
      while (y1 > y2)
      {
        pts.push_back (QPoint (x1, y1));
        y1 --;
        e -= dx;
        if (e >= 0)
        {
          x1 --;
          e += dy;
        }
      }
      pts.push_back (QPoint (x1, y1));
    }
  }
  else if (dy == 0 && dx < 0)
    while (x1 >= x2) pts.push_back (QPoint (x1--, y1 + n));
  else if (dx == 0 && dy > 0)
    while (y1 <= y2) pts.push_back (QPoint (x1 + n, y1++));
  else if (dx == 0 && dy < 0)
    while(y1 >= y2) pts.push_back (QPoint (x1 - n, y1--));
  return pts;
}


void PointSet::fusionVector (vector<QPoint> &v1, vector<QPoint> &v2)
{
  vector<QPoint>::iterator it = v2.begin ();
  while (it != v2.end ())
  {
    v1.push_back (*it);
    it ++;
  }
}
