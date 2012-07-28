/***************************************************************************
 *   Copyright (C) 2007 by Thanh-Phuong Nguyen                             *
 *   nguyentp@loria.fr                                              	   *                                                                       *
 *   This program is in the framework of project GEODIB, supported by ANR  *
 *   Its object aim at curvature and its applications for noisy discrete   *
 *   object   	                                                           *
 *   									   *
 *   No part of this program is provided in any method without author's    *
 *   permission                                                            * 
 ***************************************************************************/
 
#ifndef SEGMENT_H
#define SEGMENT_H

#include <iostream>
#include "convexhull.h"


class Segment
{
protected:
  long begin;
  long end;
  bool isMaxRight;
  ConvexHull* convexhull;
  


public:
	Segment();
	Segment(long b, long e);
	~Segment();
/*
	bool getMaxRight();
	void setMaxRight(bool b);
*/
  long getBegin() const;
  long getEnd() const;
  void setBegin(long b);
  void setEnd(long e);	
  void setConvexHull( ConvexHull* ch);
  const ConvexHull & getConvexHull() const;
		
 private:
  Segment( const Segment & other);
  Segment& operator=( const Segment & other);
//  	friend ostream& operator<<(ostream& os, Segment & segment);	
};
#endif
