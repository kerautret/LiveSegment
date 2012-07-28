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
#ifndef CIRCULATOR_H
#define CIRCULATOR_H

//#include "point2d.h"
#include <iostream>
#include "util.h"

using namespace std;

class ConvexHull;

void afficheCirculator(Node * ptrNode, Graphic graphic, Color color);

// JOL
// Node* createNode(Point2D point);

Node* createNodeInCv(Point2D point, ConvexHull & cv );
NodePtr initCirculator(ConvexHull & cv, Point2D p1, Point2D p2, Point2D p3, Node* &ptr1, Node* &ptr2, Node* &ptr3);
Node* getNext(Node* ptrNode);
Node* getLast(Node* ptrNode);
void printCirculator(Node* ptrNode);
void printCirculator(Node* ptrNode,long limit);
Point2D getContent(Node node);
Node* getLeftPtr(NodePtr ptr);
Node* getRightPtr(NodePtr ptr);

/* Node* insertMelkman(Node* &ptr, Node* &bot, Node* &top, Node* &pbot, Node* &ptop, Point2D point); */
/* Node* insertMelkman(Node* ptr, Point2D point); */

void printNode(Node* ptr);


/**
 * JOL
 *
 * Melkman algorithm for incrementally computing a convex hull.
 * 
 * @param front (InOut) points on the front part of the polygonal curve.
 * @param back (InOut) points on the back part of the polygonal curve
 * @param bot (for undoing)
 * @param top (for undoing)
 * @param pbot (for undoing)
 * @param ptop (for undoing)
 * @param point the point, added to the front, which may update the convex hull. 
 * @return 'true' if the convex hull has been modified, false otherwise.
 */
bool jolInsertMelkmanFront( ConvexHull & cv,
			    Node* & front, 
			    Node* & back,
			    Node* &bot, Node* &top, 
			    Node* &pbot, Node* &ptop, 
			    Point2D point );


/**
 * JOL
 *
 * Melkman algorithm for incrementally computing a convex hull.
 * 
 * @param front (InOut) points on the front part of the polygonal curve.
 * @param back (InOut) points on the back part of the polygonal curve
 * @param bot (for undoing)
 * @param top (for undoing)
 * @param pbot (for undoing)
 * @param ptop (for undoing)
 * @param point the point, added to the back, which may update the convex hull. 
 * @return 'true' if the convex hull has been modified, false otherwise.
 */
bool jolInsertMelkmanBack( ConvexHull & cv,
			   Node* & front, 
			   Node* & back,
			   Node* &bot, Node* &top, 
			   Node* &pbot, Node* &ptop, 
			   Point2D point );


/**
 * @param ptrNode any node
 * @return its CCW neighbor (or next or right).
 * @see getNext
 */
inline 
Node* getCCW(Node* ptrNode)
{
  return ptrNode->right;
}

/**
 * @param ptrNode any node
 * @return its CW neighbor (or last or left).
 * @see getLast
 */
inline 
Node* getCW(Node* ptrNode)
{
  return ptrNode->left;
}


/**
 * @param ptrNode any node
 * @param ccw_n its new CCW neighbor (or next or right).
 */
inline 
void setCCW(Node* ptrNode, Node* ccw_n)
{
  ptrNode->right = ccw_n;
}

/**
 * @param ptrNode any node
 * @return ccw_n its new CW neighbor (or last or left).
 */
inline 
void setCW(Node* ptrNode, Node* cw_n)
{
  ptrNode->left = cw_n;
}


/**
 * @param ptrNode a node coding front and back
 * @return the back node (or leftptr).
 * @see getLeftPtr
 */
inline 
Node* getBack(NodePtr ptr) 
{
  return ptr.leftPtr;
}

/**
 * @param ptrNode a node coding front and back
 * @return the front node (or rightptr).
 * @see getRightPtr
 */
inline 
Node* getFront(NodePtr ptr) 
{
  return ptr.rightPtr;
}




#endif
