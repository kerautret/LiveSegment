/***************************************************************************
 *   Copyright (C) 2007 by Thanh-Phuong Nguyen                             *
 *   nguyentp@loria.fr                                              	   *   *                  	                                                   *
 *   This program is in the framework of project GEODIB, supported by ANR  *
 *   Its object aim at curvature and its applications for noisy discrete   *
 *   object   	                                                           *
 *   									   *
 *   No part of this program is provided in any method without author's    *
 *   permission                                                            * 
 ***************************************************************************/
#include "circulator.h"
#include "convexhull.h"

/*
Creer un nouvel noeud qui est montre par un pointeur
*/
Node* createNode(Point2D point)
{
	Node* node;

	node=new Node;
	node->content=point;
	node->left=NULL;
	node->right=NULL;

	return node;
}

/**
 * JOL
 *
 * Not nice but better than before. Allows garbage collecting.
 */
Node* createNode(Point2D point, ConvexHull & cv )
{
	Node* node;

	node=new Node;
	node->content=point;
	node->left=NULL;
	node->right=NULL;

	cv.memorize( node );

	return node;
}

/*
Initialisation un circulator de 3 noeuds
*/
NodePtr initCirculator(ConvexHull & cv, Point2D p1, Point2D p2, Point2D p3, Node* &ptr1, Node* &ptr2, Node* &ptr3)
{
	//Declarer les varibales
	Node *node1, *node2, *node3;
	NodePtr nodePtr;
	
	//Initialiser les donnees
	node1=createNode( p1, cv );
	node2=createNode( p2, cv );
	node3=createNode( p3, cv );	
	
	
	if(isLeft(p1,p2,p3)>0)
	  {
		//Creer les associations
		node1->right=node2; node2->left=node1;
		node2->right=node3; node3->left=node2;
		node3->right=node1; node1->left=node3;
	}
	if(isLeft(p1,p2,p3)<0)
	{
		//Creer les associations
		node1->right=node3; node3->left=node1;
		node3->right=node2; node2->left=node3;
		node2->right=node1; node1->left=node2;

	}
	if(isLeft(p1,p2,p3)==0)
	  {
		cout<<"Error in initialization of convex hull"<<endl;
		exit(1);
	}

	

	//Retour le resultat
	ptr1=node1;
	ptr2=node2;
	ptr3=node3;	
	
	nodePtr.leftPtr=node1;
	nodePtr.rightPtr=node3;
		
	return nodePtr;
}

/*
Retourner le noeud suivant 
*/
Node* getNext(Node* ptrNode)
{
	return ptrNode->right;
}







/*
Retourner le noeud precedent 
*/
Node* getLast(Node* ptrNode)
{
	return ptrNode->left;
}





void afficheCirculator(Node * ptrNode, Graphic graphic, Color color)
{
  Point2D point;
  Node* ptr=ptrNode;
  
  do{
    point = getContent(*ptr);
    point.affichePixel(graphic, color);
    ptr=getNext(ptr);
  }
  while(ptr!=ptrNode);
}







/*
Imprimer le content du circulator
*/
void printCirculator(Node* ptrNode)
{
	Node* ptr;
	ptr=ptrNode;
	do{
	  cout<<"Node x="<<getContent(*ptr).getX()<<" y="<<getContent( *ptr).getY()<<endl;
	  ptr=getNext(ptr);
	}
	while(ptr!=ptrNode);
}

void printCirculator(Node* ptrNode,long limit)
{
	Node* ptr;
	long id=0;
	ptr=ptrNode;
	do{
		cout<<"Node x="<<getContent(*ptr).getX()<<" y="<<getContent( *ptr).getY()<<endl;
		ptr=getNext(ptr);
	}
	while((ptr!=ptrNode)&&(++id<limit));	
}

/*
Retourner le contenu d'un noeud
*/
Point2D getContent(Node node)
{
	Point2D point=node.content;
	return point;
}

/*
Retourner le pointeur a gauche
*/
Node* getLeftPtr(NodePtr ptr)
{
	return ptr.leftPtr;
}

/*
Retourner le pointeur a droit
*/
Node* getRightPtr(NodePtr ptr)
{
	return ptr.rightPtr;
}





/*
Installisation du l'algorithme de Melkman
*/

// JOL.
// ALL REMOVED : BAD VERSION
// Node* insertMelkman(Node* &ptr, Node* &bot, Node* &top, Node* &pbot, Node* &ptop, Point2D point)
// {
  
//   Node* ptrResult;
//   bot=ptr;
//   pbot=getLast(ptr);
//   top=ptr;
//   ptop=getNext(ptr);
  
//   ptrResult=createNode(point);
  
//   // test si le point est à l'intérieur de l'env convexe
//   if((isLeft(getContent(*bot),getContent(*getNext( bot)),point)>=0)&&
//      (isLeft(getContent(*getLast(top)), getContent(*top),point)>=0))
//     {
//       delete ptrResult; // JOL
//       return ptr;	
//     }
  
  
  
//   while(isLeft(getContent(*bot),getContent(*getNext( bot)),point)<=0)
//     {
//       //delete pbot;       // JOL
//       pbot=bot;
//       bot=getNext(bot);
//     }
  
//   while(isLeft(getContent(*getLast(top)),getContent(*top),point)<=0)
//     {
//       //delete ptop;      // JOL
//       ptop=top;
//       top=getLast(top);
//     }
  
//   bot->left=ptrResult;
//   ptrResult->right=bot;
//   ptrResult->left=top;
//   top->right=ptrResult;
  
  
//   return ptrResult;
// }

// /**
//  * 
//  * @param ptr 
//  * @param point 
//  * @return 
//  */
// Node* insertMelkman(Node* ptr, Point2D point)
// {	

// 	Node* ptrResult;
// 	Node *bot, *top;
// 	ptrResult=createNode(point);
// 	bot=ptr;top=ptr;

// 	if((isLeft(getContent(*bot),getContent(*getNext( bot)),point)>=0)&&
// 	   (isLeft(getContent(*getLast(top)),getContent(*top),point)>=0))
// 	  {
// 	    return ptr;
// 	  }
// 	while(isLeft(getContent(*bot),getContent(*getNext( bot)),point)<=0)
// 	  {
// 	    bot=getNext(bot);
// 	  }
// 	while(isLeft(getContent(*getLast(top)),getContent(*top),point)<=0)
// 	  {
// 	    top=getLast(top);
// 	  }

// 	bot->left=ptrResult;
// 	ptrResult->right=bot;
// 	ptrResult->left=top;
// 	top->right=ptrResult;
// 	return ptrResult;
// }


/*
Imprimer les informations d'un noeud
*/
void printNode(Node* ptr)
{
	cout<<"Node info: x="<<ptr->content.getX()<<" y="<<ptr->content.getY()<<endl;
}




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
			    Point2D point )
{
  // last==left==CW
  // next==right=CCW
  
  // test if the new point is inside the convex hull.
    if( ( isLeft( getContent( *front ),
		  getContent( *getCCW( front ) ),
		  point )
	  >=0 )
	&&
	( isLeft( getContent(*getCW( front ) ), 
		  getContent( *front ),
		  point )
	  >=0) )
      return false; // nothing to do.
  
  // Otherwise, creates a new node for the added point
  Node* ptrResult = createNode( point, cv );
  // then updates the CCW part at front.
  bool update_back = false;
  bot = front;
  pbot = getCW( front ); // last==left==CW
  
  while( isLeft( point,
		 getContent( *bot ),
		 getContent( *getCCW( bot ) ) )
	 <= 0 )
    {
      // Rk. should delete pbot;       // JOL
      // advances front
      if ( bot == back ) update_back = true;
      pbot = bot;
      bot = getCCW( bot );
    }
  // updates back if necessary
  if ( update_back )
    back = bot;

  update_back = false;
  // Now updates the CW part at front.
  top = front;
  ptop = getCCW( front ); // next==right=CCW
  while ( isLeft( point, 
		  getContent( *top ),
		  getContent( *getCW( top ) ) )
	  >= 0 )
    {
      // Rk. should delete ptop;      // JOL
      if ( top == back ) update_back = true;
      ptop = top;
      top = getCW( top );
    }

  // updates front links
  setCW( bot, ptrResult );
  setCCW( ptrResult, bot );
  setCCW( top, ptrResult );
  setCW( ptrResult, top );

  // update front
  front = ptrResult;

  // updates back if necessary
  if ( update_back )
    back = top;

  return true;
}

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
			   Point2D point )
{
  // last==left==CW
  // next==right=CCW
  
  // test if the new point is inside the convex hull.
    if( ( isLeft( getContent( *back ),
		  getContent( *getCCW( back ) ),
		  point )
	  >=0 )
	&&
	( isLeft( getContent(*getCW( back ) ), 
		  getContent( *back ),
		  point )
	  >=0) )
      return false; // nothing to do.
  
  // Otherwise, creates a new node for the added point
  Node* ptrResult = createNode( point, cv );

  // then updates the CW part at back
  bool update_front = false;
  top = back;
  ptop = getCCW( back ); // next==right=CCW
  
  while( isLeft( point,
		 getContent( *top ),
		 getContent( *getCW( top ) ) )
	 >= 0 )
    {
      // Rk. should delete ptop;       // JOL
      // advances front
      if ( top == front ) update_front = true;
      ptop = top;
      top = getCW( top );
    }
  // updates front if necessary
  if ( update_front )
    front = top;

  update_front = false;
  // Now updates the CCW part at back
  bot = back;
  pbot = getCW( back ); // last==left==CW
  while ( isLeft( point, 
		  getContent( *bot ),
		  getContent( *getCCW( bot ) ) )
	  <= 0 )
    {
      // Rk. should delete pbot;      // JOL
      if ( bot == front ) update_front = true;
      pbot = bot;
      bot = getCCW( bot );
    }

  // updates back links
  setCCW( top, ptrResult );
  setCW( ptrResult, top );
  setCW( bot, ptrResult );
  setCCW( ptrResult, bot );

  // update back
  back = ptrResult;

  // updates front if necessary
  if ( update_front )
    front = bot;

  return true;
}

