#ifndef PIXEL_H
#define PIXEL_H

using namespace std;


/** 
 * @class Pixel pixel.h
 * \brief Image pixel.
 * \author {P. Even}
 */
class Pixel
{

public:

  /**
   * @fn Pixel ()
   * \brief Creates a pixel at origin.
   */
  Pixel ();

  /**
   * @fn Pixel (int x, int y)
   * \brief Creates a pixel using coordinates.
   * @param x abscissae.
   * @param y ordinate.
   */
  Pixel (int x, int y);

  /**
   * @fn Pixel (Pixel p)
   * \brief Creates a copy of a given pixel.
   * @param p original.
   */
  Pixel (const Pixel &p);

  /**
   * @fn int x ()
   * \brief Returns the pixel abscissae.
   */
  int x ();

  /**
   * @fn int y ()
   * \brief Returns the pixel ordinate.
   */
  int y ();

  /**
   * @fn int manhattan (Pixel p)
   * \brief Returns the manhattan distance to a other pixel.
   * @param p the distant pixel.
   */
  int manhattan (Pixel p);


  /**
   * @fn void translate (double distance, double cosa, double sina)
   * \brief Translates the pixel of a given distance in direction (cosa, sina).
   * @param dist distance.
   * @param dx direction absissae.
   * @param dy direction ordinate.
   */
  void translate (double dist, double dx, double dy);

  /**
   * @fn Pixel *drawing (Point p)
   * \brief Returns the segment from the point to point p.
   * @param p the remote point.
   * @param n size of the returned array.
   */
  Pixel *drawing (Pixel p, int *n);

  /**
   * @fn Pixel *pathTo (Point p)
   * \brief Returns the succession of local moves from the point to point p.
   * A local move is the vector from a point to one of his neighbours.
   * @param p the remote point.
   * @param n size of the returned array.
   */
  Pixel *pathTo (Pixel p, int *n);

  /**
   * @fn bool *stepsTo (Point p)
   * \brief Returns the location of the steps from the point to point p.
   * @param p the remote point.
   * @param n size of the returned array.
   */
  bool *stepsTo (Pixel p, int *n);


private:

  /** Pixel abscissae. */
  int xp;
  /** Pixel ordinate. */
  int yp;
};

#endif
