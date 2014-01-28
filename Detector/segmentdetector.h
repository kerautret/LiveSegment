#ifndef SEGMENTDETECTOR_H
#define SEGMENTDETECTOR_H

#include <vector>
#include "blurredsegment.h"
#include "imagedata.h"
#include "pixel.h"

using namespace std;


/** 
 * @class SegmentDetector segmentdetector.h
 * \brief Blurred segment detector in grey level images.
 * \author {B. Kerautret and P. Even}
 */
class SegmentDetector
{

public:
  
  /**
   * \brief Creates a segment detector.
   * \author {P. Even}
   */
  SegmentDetector ();

  /**
   * \brief Deletes the segment detector.
   * \author {P. Even}
   */
  ~SegmentDetector ();

  /**
   * \brief  Runs a blurred segment detection.
   * \author {B. Kerautret and P. Even}
   * Step 1: For each scan line, one candidate is selected
   *         based on the gradient norm only (no direction).
   * Step 2: For each scan line, local candidates are detected
   *         on top of gradient ridges with closest direction,
   *         The first candidates that prolongates the segment is retained.
   * Step 3: id. step 2.
   */
  BlurredSegment *detect (Pixel p1, Pixel p2);

  /**
   * \brief Tries to build a blurred segment from candidates.
   * \author {B. Kerautret and P. Even}
   * Returns true if succeeded.
   */
  bool buildSegment (vector<Pixel> leftV, vector<Pixel> rightV);

  /**
   * \brief Tries to build a blurred segment from candidates.
   * \author {B. Kerautret and P. Even}
   * Returns true if succeeded.
   */
  bool buildSegment (vector< vector<Pixel> > leftW,
                     vector< vector<Pixel> > rightW);

  /**
   * \brief Sets the gradient min threshold.
   * \author {P. Even}
   */
  void setGradientThreshold (double value);

  /**
   * \brief Sets the blurred segment maximal width.
   * \author {P. Even}
   */
  void setSegmentMaxWidth (double value);

  /**
   * \brief Sets the accepted lacks of added candidates.
   * \author {P. Even}
   */
  void setAcceptedLacks (int number);

  /**
   * \brief Sets the number of detection steps (for settings).
   * \author {P. Even}
   */
  void setNumberOfSteps (int number);

  /**
   * \brief Sets the image data.
   * \author {P. Even}
   */
  void setImageData (ImageData *data);


private :

  // Segment detection default parameters. */
  static const double DEFAULT_GRADIENT_THRESHOLD = 0.0;
  static const double DEFAULT_DIRECTION_CLOSENESS = 0.9;
  static const double DEFAULT_MAX_WIDTH = 5;
  static const double DEFAULT_ACCEPTED_LACKS = 5;
  static const double DEFAULT_NUMBER_OF_STEPS = 3;

  /** Blurred segment availability. */
  bool isDetected;
  /** Last detected blurred segment. */
  BlurredSegment *segment;
  /** Gradient min threshold. */
  double gradientThreshold;
  /** Direction closeness test value (min cosine). */
  double directionCloseness;
  /** Blurred segment max width. */
  double maxWidth;
  /** Accepted number of successive lacks. */
  int acceptedLacks;
  /** Number of iteration steps (for debug). */
  int numberOfSteps;
  /** Processed image data. */
  ImageData *imageData;

};
#endif
