#ifndef DIRECTIONALSCAN_H
#define DIRECTIONALSCAN_H

#include <cstdlib>
#include <vector>
#include "pixel.h"
using namespace std;



/** 
 * @class DirectionalScan directionalscan.h
 * \brief Scan strip composed of parallel scan lines.
 * \author {B. Kerautret and P. Even}
 */
class DirectionalScan
{

public:
  
  /**
   * @fn DirectionalScan(Pixel p1, Pixel p2, int xmin, int xmax, int ymin, int ymax)
   * \brief Creates a scan strip from two control points.
   * Creates a scan strip from two control points.
   * The scan strip is composed of parallel scan lines, the first one being
   *   defined by control points p1 and p2.
   * @param p1 start control point
   * @param p2 end control point
   * @param xmin left border of the scan area
   * @param xmax right border of the scan area
   * @param ymin low border of the scan area
   * @param ymax up border of the scan area
   */
  DirectionalScan (Pixel p1, Pixel p2,
                   int xmin, int xmax, int ymin, int ymax);

  /**
   * @fn DirectionalScan(Pixel p1, Pixel p2, double directionAngle)
   * \brief Creates a scan strip from two control points and a angular threshold
   * Creates a scan strip from two control points and direction v1 -> v2.
   * The scan strip is composed of parallel scan lines, centered on the middle
   *   of (p1,p2) and aligned on (v1,v2).
   * @param p1 start control point
   * @param p2 end control point
   * @param v1 direction start point
   * @param v2 direction end point
   * @param xmin left border of the scan area
   * @param xmax right border of the scan area
   * @param ymin low border of the scan area
   * @param ymax up border of the scan area
   */
  DirectionalScan (Pixel p1, Pixel p2, Pixel v1, Pixel v2,
                   int xmin, int xmax, int ymin, int ymax);

  /**
   * @fn ~DirectionalScan()
   * \brief Deletes the scan strip.
   */
  ~DirectionalScan ()
  {
  };


  /**
   * @fn getLeftScan(int num)
   * \brief Returns the vector<Pixel> associated to the "num" left scan.
   * @return  the vector<Pixel> associated to a scan
   */
  vector<Pixel> getLeftScan (int num);


  /**
   * @fn getRightScan(int num)
   * \brief Returns the vector<Pixel> associated to the "num" right  scan.
   * @return  the vector<Pixel> associated to a scan
   */
  vector<Pixel> getRightScan (int num);


  /**
   * @fn getLeftScan(int num)
   * \brief Returns the vector<Pixel> associated to the "num" "direction" scan.
   * @return  the vector<Pixel> associated to a scan
   */
  vector<Pixel> getScan (int num, int direction);


  /**
   * @fn getNbLeftScan()
   * \brief returns the total number of scan lines computed on left side.
   * @return returns the total number of scans computed on left side.
   */
  int getNbScan (int Direction);


  /**
   * @fn getNbLeftScan()
   * \brief returns the total number of scans computed in the left direction.
   * @return returns the total number of scans computed in the left direction.
   */
  int getNbLeftScan ();


  /**
   * @fn getNbLeftScan()
   * \brief returns the total number of scans computed in the right direction.
   * @return returns the total number of scans computed in the right direction.
   */
  int getNbRightScan ();



protected:

  /** Scan area used to compute the scans (all or part of the image) */
  int xmin, xmax, ymin, ymax;

  /** Left scan lines (including the central scan) */
  vector <vector<Pixel> > lscan;
  /** Right scan lines */
  vector <vector<Pixel> > rscan;

  /** Minimal length of a scan */
  static const int SMIN = 8;


  /**
   * @fn void scan(int a, int b, int mu1, int mu2, int nbs, boolean *steps, int cx, int cy, int mw)
   * \brief computes the scan strips.
   * @param (a,b,mu2) equation of the upper support lines
   * @param nbs size of the steps position array
   * @param steps steps position array
   * @param (cx,cy) start position of the central scan
   */
  void scan (int a, int b, int mu2,
             int nbs, bool *steps, int cx, int cy);

};

#endif
