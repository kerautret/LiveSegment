#ifndef DIRECTIONALSCAN_H
#define DIRECTIONALSCAN_H

#include <cstdlib>
#include <vector>
#include "pixel.h"
using namespace std;



/** 
 * @class DirectionalScan directionalscan.h
 * \brief Scan strip composed of parallel scan lines.
 * \author {B. Kerautret}
 */
class DirectionalScan
{

public:
  
  /**
   * @fn DirectionalScan(Pixel p1, Pixel p2)
   * \brief Creates a scan strip from two control points.
   * Creates a scan strip from two control points.
   * The scan strip is composed of parallel scan lines, the first one being
   *   defined by control points p1 and p2.
   * @param p1 start control point
   * @param p2 end control point
   */
  DirectionalScan (Pixel p1, Pixel p2);

  /**
   * @fn DirectionalScan(Pixel p1, Pixel p2, double directionAngle)
   * \brief Creates a scan strip from two control points and a angular threshold
   * Creates a scan strip from two control points and an angular threshold.
   * The scan strip is composed of parallel scan lines, the first one being
   *   defined by control points p1 and p2.
   * QUESTION : ou l'angle est-il utilise ?
   * @param p1 start control point
   * @param p2 end control point
   * @param directionAngle angular threshold value
   */
  DirectionalScan (Pixel p1, Pixel p2, double directionAngle);

  /**
   * @fn DirectionalScan(Pixel p1, Pixel p2, Pixel v)
   * \brief Creates a scan strip from two control points and a vector
   * The scan strip is composed of parallel scan lines, the first one being
   *   defined by control points p1 and p2.
   * @param p1 : first control point.
   * @param p2 : second control point.
   * @param v : lead vector of the scan strip.
   */
  DirectionalScan (Pixel p1, Pixel p2, Pixel v);

  /**
   * @fn DirectionalScan()
   * \brief Creates a "default" scan strip.
   * BUG : Obligatoire car Paint comporte un attribut DirectionalScan
   *       initialise avec ce constructeur
   */
  DirectionalScan ();

  /**
   * @fn ~DirectionalScan()
   * \brief Deletes the scan strip.
   * BUG : Desallocations a verifier
   */
  ~DirectionalScan ()
  {
    // delete tabDeplacements;
  };


  /**
   * @fn  retOctantDroiteD1();
   * \brief Computes the octant of the scan strip direction
   * @return the octant
   */
  int retOctantDroiteD1 ();


  /**
   * @fn  computeAllScans(int xmin, int ymin, int xmax, int ymax);
   * \brief determines scan lines on both sides.
   * determines scan lines on both sides where points are included
   * within the intervals xmin, ymin, xmax, ymax
   */
  void computeAllScans (int xmin, int ymin, int xmax, int ymax);


  /**
   * @fn  computeAllScans(int xmin, int ymin, int xmax, int ymax, int direction);
   * \brief Determines scan lines on both sides.
   * Determines scans on both sides where points are included
   * within the intervals xmin, ymin, xmax, ymax
   */
  void computeScans (int xmin, int ymin, int xmax, int ymax, int direction);
  

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
   * \brief Tests if the scan exists.
   * Tests if the scan exists (works with an absolute index value).
   **/
  bool isScanDefined (int num, int direction);
  

  /**
   * @fn getScanOrientedIndex(int num, int direction)
   * \brief returns * the vector<Pixel> associated to the "num" "direction" scan if
   * the index is negative, return the scan in the opposite direction
   * with the bas(index) position.  @return the vector<Pixel>
   * associated to a scan
   */
  vector<Pixel> getScanOrientedIndex (int num, int direction);


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


  /**
   * @fn isInImageBounds(Pixel &p)
   * \brief  Checks if the point belongs to the scan area.
   */
  bool isInImageBounds (Pixel &p);


  /**
   * @fn isInImageBounds(vector<Pixel> &vectP)
   * \brief  Checks if the points all belong to the scan area.
   */
  bool isInImageBounds (vector<Pixel> &vectP);
  
  /** Scan area used to compute the scans (all or part of the image) */
  int xmin, xmax, ymin, ymax;




protected:
  /** Left scan lines */
  vector <vector<Pixel> > vectLeftScan;
  /** Right scan lines */
  vector <vector<Pixel> > vectRightScan;

  /** Left scan iterating position */
  int leftPos;
  /** Right scan iterating position */
  int rightPos;


  /** Computes the offset vectors of the segment joining p1 to p2 */
  void computeMotif (Pixel p1, Pixel p2);

  /** Reverse a reversed segment to v */
  vector<Pixel> inverseVector (vector<Pixel> &v);
  
  /** Returns the offset vectors the inverse segment to tabDepl */
  Pixel *inverseSensMotif (Pixel *tabDepl, int taille);
  
  /** Returns the scan line */
  vector<Pixel> getVectPoints ();
  /** Checks if point p belongs to the scan area. */
  bool isBetweenLines (Pixel p); 
  
  /** Checks if point p is on rhe right side of the initial scan line. */
  bool isRight (Pixel p);  
  /** Returns the parameters of the scan strip border passing through P1. */
  int *retCoeffDroitesD1 ();
  /** Returns the parameters of the scan strip border passing through P2. */
  int *retCoeffDroitesD2 ();
  
  /** Returns an offset vector orthogonal to the scan line. */
  Pixel *retTabDeplacements (int direction);
  /** Returns the length of the scan line - 1. */
  int retTailleMotif ();


  /** Control points. */
  Pixel p1, p2;
  /** Pixels of the scan line. */
  vector<Pixel> vectPoints;
  /** Offset vectors of the scan line. */
  Pixel  *tabDeplacements;

  /** Scan strip border passing through P1 */
  int a1, b1, mu1;
  /** Scan strip border passing through P2 */
  int a2, b2, mu2;

  /** Scan strip octant number (from 1 to 8) */
  int directionOctant;
  
  /** Adds a pixel to the scan line. */
  void addPixel (int x, int y);

  /** Computes the scan strip borders based on the control points. */
  void calcDroites ();

  /** Computes the scan strip borders based on a direction angle. */
  void calcDroites (double directionAngle);


  /** Starts an iterator and returns the central scan */
  vector<Pixel> start ();

  /** Checks if there are other scans on left side */
  bool leftOn ();

  /** Returns the next left scan */
  vector<Pixel> left ();

  /** Checks if there are other scans on right side */
  bool rightOn ();

  /** Returns the next right scan */
  vector<Pixel> right ();
};

#endif

