#include <cstdlib>
#include <iostream>
#include "directionalscan.h"
#include "util.h"  // Direction



DirectionalScan::DirectionalScan (Pixel p1, Pixel p2,
                                  int xmin, int xmax, int ymin, int ymax)
{
  this->xmin = xmin;
  this->xmax = xmax;
  this->ymin = ymin;
  this->ymax = ymax;

  // Enforces P1 to be lower than p2
  if (p1.y () > p2.y ())
  {
    Pixel tmp = p1;
    p1 = p2;
    p2 = tmp;
  }
    
  // Computes the steps position array
  int nbs = 0;
  bool *steps = p1.stepsTo (p2, &nbs);

  // Start position of the central scan
  int cx = p1.x ();
  int cy = p1.y ();

  // Equation of the strip support lines : ax - by = mu
  int a = p1.x () - p2.x ();
  int b = p2.y () - p1.y ();
  int mu2 = a * p2.x () - b * p2.y ();
  // int mw = nbs - 1;   // unused

  // Builds the scans
  scan (a, b, mu2, nbs, steps, cx, cy);
  delete steps;
}



DirectionalScan::DirectionalScan (Pixel p1, Pixel p2, Pixel v1, Pixel v2,
                                  int xmin, int xmax, int ymin, int ymax)
{
  this->xmin = xmin;
  this->xmax = xmax;
  this->ymin = ymin;
  this->ymax = ymax;

  // Get the scan strip center
  int cx = (p1.x () + p2.x ()) / 2;
  int cy = (p1.y () + p2.y ()) / 2;
    
  // Gets the steps position array
  int nbs = 0;
  bool *steps = v1.stepsTo (v2, &nbs);

  // Equation of the straigth line passing through the center : ax - by = mu
  int a = v2.y () - v1.y ();
  int b = v2.x () - v1.x ();
  if (b < 0)
  {
    a = -a;
    b = -b;
  }
  int mu1 = a * cx - b * cy;

  // Enforces P1 at the right side of the straight line (C,V)
  if (a * p1.x () - b * p1.y () < mu1)
  {
    Pixel tmp = p1;
    p1 = p2;
    p2 = tmp;
  }

  // Equation of the support lines of the scan strip
  mu1 = a * p1.x () - b * p1.y ();
  int mu2 = a * p2.x () - b * p2.y ();

  // Looking for the central scan start position
  int mw = 0;
  bool *st = steps + nbs;
  if (b * b < a * a)  // rather vertical strip
  {
    if (a > 0)     // octant 2
    {
      do
      {
        if (--st < steps) st = steps + nbs - 1;
        if (*st) cy --;
        cx ++;
        mw++;
      }
      while (a * cx - b * cy < mu1);
    }
    else           // octant 7
    {
      do
      {
        if (--st < steps) st = steps + nbs - 1;
        if (*st) cy --;
        cx --;
        mw++;
      }
      while (a * cx - b * cy < mu1);
    }
  }
  else         // rather horizontal strip
  {
    if (a > 0)     // octant 1
    {
      do
      {
        if (--st < steps) st = steps + nbs - 1;
        if (*st) cx ++;
        cy --;
        mw++;
      }
      while (a * cx - b * cy < mu1);
    }
    else           // octant 8
    {
      do
      {
        if (--st < steps) st = steps + nbs - 1;
        if (*st) cx --;
        cy --;
        mw++;
      }
      while (a * cx - b * cy < mu1);
    }
  }
  // mw = 2 * mw - 1;  // cause of variable scan lengths, unused

  // Builds the scans
  scan (a, b, mu2, nbs, steps, cx, cy);
  delete steps;
}



vector<Pixel> DirectionalScan::getScan (int num, int direction)
{
  return (direction == RIGHT) ? rscan.at (num) : lscan.at (num);
}



vector<Pixel> DirectionalScan::getLeftScan (int num)
{
  return lscan.at (num);
}


vector<Pixel> DirectionalScan::getRightScan (int num)
{
  return rscan.at (num);
}


int DirectionalScan::getNbScan (int direction)
{
  return (direction == RIGHT) ? rscan.size () : lscan.size ();
}


int DirectionalScan::getNbLeftScan ()
{
  return lscan.size ();
}


int DirectionalScan::getNbRightScan ()
{
  return rscan.size ();
}



void DirectionalScan::scan (int a, int b, int mu2,
                            int nbs, bool *steps, int cx, int cy)
{
  int x, y;                 // Current position coordinates
  int sx, sy;               // Current scan start position
  bool *lst; // idir        // Current step in strip direction
  int ndir;                 // Nombre de scans
  bool *mst; // ipts        // Start step in scan direction
  bool *nst; // jpts        // Current step in scan direction
  int npts;                 // Longueur d'un scan
  bool *fs = steps + nbs;   // End of the steps position array
  bool transition;


  if (b * b < a * a)  // vertical strip
  {
    if (a > 0)     // octant 2
    {
      // Fills in the central scan
      sx = cx;
      sy = cy;
      x = sx;
      y = sy;
      nst = steps;
      vector<Pixel> sc;
      for (int i = 1; a * x - b * y >= mu2; i++)
      {
        if (y >= ymin && y < ymax && x >= xmin && x < xmax)
          sc.push_back (Pixel (x, y));
        if (*nst) y++;
        x--;
        if (++nst >= fs) nst = steps;
      }
      lscan.push_back (sc);

      // Fills in the left scans
      sx = cx;
      sy = cy;
      lst = steps;
      ndir = 0; // numero de bande
      transition = false;
        
      npts = 1;
      mst = steps;
      while (npts != 0)
      {
        // Preparation of the next scan
        if (transition)
        {
          sy --;
          transition = false;
        }
        else
        {
          if (--lst < steps) lst = fs - 1;
          sy --;
          if (*lst)
          {
            sx --;
            if (*mst)
            {
              sy ++;
              transition = true;
            }
            if (++mst >= fs) mst = steps;
          }
        }

        // Computes the next scan
        x = sx;
        y = sy;
        nst = mst;
        npts = 0;
        vector<Pixel> pix;
        for (int i = 1; a * x - b * y >= mu2; i++)
        {
          if (y >= ymin && y < ymax && x >= xmin && x < xmax)
          {
            pix.push_back (Pixel (x, y));
            npts ++;
          }
          if (*nst) y++;
          x--;
          if (++nst >= fs) nst = steps;
        }
        if (npts > SMIN) lscan.push_back (pix);
        ndir++;
      }

      // Fills in the right scans
      sx = cx;
      sy = cy;
      lst = steps;
      ndir = 0; // numero de bande
      transition = false;

      npts = 1;
      mst = steps;
      while (npts != 0)
      {
        // Preparation of the next scan
        if (transition)
        {
          sx ++;
          if (--mst < steps) mst = fs - 1;
          transition = false;
        }
        else
        {
          sy ++;
          if (*lst)
          {
            if (--mst < steps) mst = fs - 1;
            if (*mst)
            {
              if (++mst >= fs) mst = steps;
              transition = true;
            }
            else sx ++;
          }
          if (++lst >= fs) lst = steps;
        }

        // Computes the next scan
        x = sx;
        y = sy;
        nst = mst;
        npts = 0;
        vector<Pixel> pix;
        for (int i = 1; a * x - b * y >= mu2; i++)
        {
          if (y >= ymin && y < ymax && x >= xmin && x < xmax)
          {
            pix.push_back (Pixel (x, y));
            npts ++;
          }
          if (*nst) y++;
          x--;
          if (++nst >= fs) nst = steps;
        }
        if (npts > SMIN) rscan.push_back (pix);
        ndir++;
      }
    }

    else           // octant 7
    {
      // Fills in the central scan
      sx = cx;
      sy = cy;
      x = sx;
      y = sy;
      nst = steps;
      npts = 0;
      vector<Pixel> sc;
      for (int i = 1; a * x - b * y >= mu2; i++)
      {
        if (y >= ymin && y < ymax && x >= xmin && x < xmax)
        {
          sc.push_back (Pixel (x, y));
          npts ++;
        }
        if (*nst) y++;
        x++;
        if (++nst >= fs) nst = steps;
      }
      lscan.push_back (sc);

      // Fills in the left scans
      sx = cx;
      sy = cy;
      lst = steps;
      ndir = 0; // numero de bande
      transition = false;

      npts = 1;
      mst = steps;
      while (npts != 0)
      {
        // Prepares the next scan
        if (transition)
        {
          sx --;
          if (--mst < steps) mst = fs - 1;
          transition = false;
        }
        else
        {
          if (--lst < steps) lst = fs - 1;
          sy ++;
          if (*lst)
          {
            if (--mst < steps) mst = fs - 1;
            if (*mst)
            {
              if (++mst >= fs) mst = steps;
              transition = true;
            }
            else sx --;
          }
        }

        // Computes the next scan
        x = sx;
        y = sy;
        nst = mst;
        npts = 0;
        vector<Pixel> pix;
        for (int i = 1; a * x - b * y >= mu2; i++)
        {
          if (y >= ymin && y < ymax && x >= xmin && x < xmax)
          {
            pix.push_back (Pixel (x, y));
            npts ++;
          }
          if (*nst) y++;
          x++;
          if (++nst >= fs) nst = steps;
        }
        if (npts > SMIN) lscan.push_back (pix);
        ndir++;
      }

      // Fills in the right scans
      sx = cx;
      sy = cy;
      lst = steps;
      ndir = 0; // numero de bande
      transition = false;

      npts = 1;
      mst = steps;
      while (npts != 0)
      {
        // Prepares the next scan
        if (transition)
        {
          sy --;
          transition = false;
        }
        else
        {
          sy --;
          if (*lst)
          {
            sx ++;
            if (*mst)
            {
              sy ++;
              transition = true;
            }
            if (++mst >= fs) mst = steps;
          }
          if (++lst >= fs) lst = steps;
        }

        // Computes the next scan
        x = sx;
        y = sy;
        nst = mst;
        npts = 0;
        vector<Pixel> pix;
        for (int i = 1; a * x - b * y >= mu2; i++)
        {
          if (y >= ymin && y < ymax && x >= xmin && x < xmax)
          {
            pix.push_back (Pixel (x, y));
            npts ++;
          }
          if (*nst) y++;
          x++;
          if (++nst == fs) nst = steps;
        }
        if (npts > SMIN) rscan.push_back (pix);
        ndir++;
      }
    }
  }

  else  // horizontal strip
  {
    if (a > 0)     // octant 1
    {
      // Fills in the central scan
      sx = cx;
      sy = cy;
      x = sx;
      y = sy;
      nst = steps;
      npts = 0;
      vector<Pixel> sc;
      for (int i = 1; a * x - b * y >= mu2; i++)
      {
        if (y >= ymin && y < ymax && x >= xmin && x < xmax)
        {
          sc.push_back (Pixel (x, y));
          npts ++;
        }
        if (*nst) x--;
        y++;
        if (++nst >= fs) nst = steps;
      }
      lscan.push_back (sc);

      // Fills in the left scans
      sx = cx;
      sy = cy;
      lst = steps;
      ndir = 0; // numero de bande
      transition = false;
        
      npts = 1;
      mst = steps;
      while (npts != 0)
      {
        // Prepares the next scan
        if (transition)
        {
          sy --;
          if (--mst < 0) mst = fs - 1;
          transition = false;
        }
        else
        {
          if (--lst < steps) lst = fs - 1;
          sx --;
          if (*lst)
          {
            sy --;
            if (--mst < steps) mst = fs - 1;
            if (*mst)
            {
              if (++mst >= fs) mst = steps;
              sy ++;
              transition = true;
            }
          }
        }

        // Computes the next scan
        x = sx;
        y = sy;
        nst = mst;
        npts = 0;
        vector<Pixel> pix;
        for (int i = 1; a * x - b * y >= mu2; i++)
        {
          if (y >= ymin && y < ymax && x >= xmin && x < xmax)
          {
            pix.push_back (Pixel (x, y));
            npts ++;
          }
          if (*nst) x--;
          y++;
          if (++nst >= fs) nst = steps;
        }
        if (npts > SMIN) lscan.push_back (pix);
        ndir++;
      }

      // Fills in the right scan
      sx = cx;
      sy = cy;
      lst = steps;
      ndir = 0; // numero de bande
      transition = false;

      npts = 1;
      mst = steps;
      while (npts != 0)
      {
        // Prepares the next scan
        if (transition)
        {
          sx ++;
          transition = false;
        }
        else
        {
          sx ++;
          if (*lst)
          {
            if (*mst)
            {
              sx --;
              transition = true;
            }
            sy ++;
            if (++mst >= fs) mst = steps;
          }
          if (++lst >= fs) lst = steps;
        }

        // Computes the next scan
        x = sx;
        y = sy;
        nst = mst;
        npts = 0;
        vector<Pixel> pix;
        for (int i = 1; a * x - b * y >= mu2; i++)
        {
          if (y >= ymin && y < ymax && x >= xmin && x < xmax)
          {
            pix.push_back (Pixel (x, y));
            npts ++;
          }
          if (*nst) x--;
          y++;
          if (++nst >= fs) nst = steps;
        }
        if (npts > SMIN) rscan.push_back (pix);
        ndir++;
      }
    }

    else        // octant 8
    {
      // Fills in the central scan
      sx = cx;
      sy = cy;
      x = sx;
      y = sy;
      nst = steps;
      npts = 0;
      vector<Pixel> sc;
      for (int i = 1; a * x - b * y >= mu2; i++)
      {
        if (y >= ymin && y < ymax && x >= xmin && x < xmax)
          sc.push_back (Pixel (x, y));
        if (*nst) x++;
        y++;
        if (++nst >= fs) nst = steps;
      }
      lscan.push_back (sc);

      // Fills in the left scans
      sx = cx;
      sy = cy;
      lst = steps;
      ndir = 0; // numero de bande
      transition = false;
      
      npts = 1;
      mst = steps;
      while (npts != 0)
      {
        // Prepares the next scan
        if (transition)
        {
          sx --;
          transition = false;
        }
        else
        {
          if (--lst < steps) lst = fs - 1;
          sx --;
          if (*lst)
          {
            sy ++;
            if (*mst)
            {
              sx ++;
              transition = true;
            }
            if (++mst >= fs) mst = steps;
          }
        }

        // Computes the next scan
        x = sx;
        y = sy;
        nst = mst;
        npts = 0;
        vector<Pixel> pix;
        for (int i = 1; a * x - b * y >= mu2; i++)
        {
          if (y >= ymin && y < ymax && x >= xmin && x < xmax)
          {
            pix.push_back (Pixel (x, y));
            npts ++;
          }
          if (*nst) x++;
          y++;
          if (++nst >= fs) nst = steps;
        }
        if (npts > SMIN) lscan.push_back (pix);
        ndir++;
      }

      // Fills in the right scans
      sx = cx;
      sy = cy;
      lst = steps;
      ndir = 0; // numero de bande
      transition = false;

      npts = 1;
      mst = steps;
      while (npts != 0)
      {
        // Prepares the next scan
        if (transition)
        {
          sy --;
          if (--mst < steps) mst = fs - 1;
          transition = false;
        }
        else
        {
          sx ++;
          if (*lst)
          {
            if (--mst < steps) mst = fs - 1;
            if (*mst)
            {
              if (++mst >= fs) mst = steps;
              transition = true;
            }
            else sy --;
          }
          if (++lst >= fs) lst = steps;
        }

        // Computes the next scan
        x = sx;
        y = sy;
        nst = mst;
        npts = 0;
        vector<Pixel> pix;
        for (int i = 1; a * x - b * y >= mu2; i++)
        {
          if (y >= ymin && y < ymax && x >= xmin && x < xmax)
          {
            pix.push_back (Pixel (x, y));
            npts ++;
          }
          if (*nst) x++;
          y++;
          if (++nst >= fs) nst = steps;
        }
        if (npts > SMIN) rscan.push_back (pix);
        ndir++;
      }
    }
  }
}
