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
   * @fn void translate (double distance, double cosa, double sina)
   * \brief Translates the pixel of a given distance in direction (cosa, sina).
   * @param dist distance.
   * @param dx direction absissae.
   * @param dy direction ordinate.
   */
  void translate (double dist, double dx, double dy);


private:

  /** Pixel abscissae. */
  int xp;
  /** Pixel ordinate. */
  int yp;
};

#endif
