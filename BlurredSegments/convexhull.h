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
#ifndef CONVEXHULL_H
#define CONVEXHULL_H


#include <sys/types.h> 
#include "antipodalh.h"
#include "antipodalv.h"
#include "circulator.h"
#include "algorithm.h"

using namespace std;

class ConvexHull{

protected:
  NodePtr convexhull;
  NodePtr previous_convexhull;
  AntiPodalH aph;
  AntiPodalV apv;
  vector<Node*> all_nodes;
public:
  ConvexHull();
  ConvexHull(Point2D p1, Point2D p2, Point2D p3);

  void memorize( Node* n );
  
  void addPoint(Point2D point, Direction d, Node* &ptr, Node* &bot, Node* &top, Node* &pbot, Node* &ptop);
  void addPoint(Point2D point, Direction d);
  //NodePtr getConvexHull();
  NodePtr getFrontAndBack();

  Node* getLeft();
  Node* getRight();		
  const AntiPodalH & getAPH() const;
  const AntiPodalV & getAPV() const;
  void setAPH(const AntiPodalH & aph){this->aph=aph;}
  void setAPV(const AntiPodalV & apv){this->apv=apv;}
  void setLeft(Node* ptr);
  void setRight(Node* ptr);
  void setPreviousConvexHull(Direction dr, 
			     Node* &ptr,Node* &bot, Node* &top, 
			     Node* &pbot, Node* &ptop);
  bool isHeightLine() const;
  double getOptimalEpais() const;
  void afficheConvex(Graphic graphic, Color color);
  void afficheConvex(Color color) const;
  
  void jolSetPreviousConvexHull(Node* &bot, Node* &top, Node* &pbot, Node* &ptop);

  /**
   * @return the node that is on bottom of Melkman's deque. 
   * @see getLeft
   */
  inline Node* getBack() const
    {	
      return convexhull.leftPtr;
    }
  
  /**
   * @return the node that is on top of Melkman's deque. 
   * @see getRight
   */
  inline Node* getFront() const
    {
      return convexhull.rightPtr;
    }
  
  /**
   * @param the node that is now on bottom of Melkman's deque. 
   * @see setLeft
   */
  inline void setBack(Node* ptr)
    {	
      convexhull.leftPtr=ptr;
    }
  
  /**
   * @param the node that is now on top of Melkman's deque. 
   * @see setRight
   */
  inline void setFront(Node* ptr)
    {
      convexhull.rightPtr=ptr;
    }


  
  ~ConvexHull();
  private: 
   ConvexHull( const ConvexHull & other ); 
   ConvexHull& operator=( const ConvexHull & other ); 
};

#endif
