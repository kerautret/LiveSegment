/***************************************************************************
 *   Copyright (C) 2007 by Thanh-Phuong Nguyen                             *
 *   nguyentp@loria.fr                                              	   *                                                       *	   								   *
 *   This program is in the framework of project GEODIB, supported by ANR  *
 *   Its object aim at curvature and its applications for noisy discrete   *
 *   object   	                                                           *
 *   									   *
 *   No part of this program is provided in any method without author's    *
 *   permission                                                            * 
 ***************************************************************************/
#include "convexhull.h"

ConvexHull::ConvexHull()
{
}




ConvexHull::ConvexHull(Point2D p1, Point2D p2, Point2D p3)
{	
	Point2DinCV _p1, _p2, _p3;
	Node *ptr1, *ptr2, *ptr3;
		
	//Construire l'enveloppe convexe
	convexhull=initCirculator(*this, p1,p2,p3,ptr1,ptr2,ptr3);	
	
	//Construire des paires antipodals
	_p1=Point2DinCV(p1);
	_p2=Point2DinCV(p2);
	_p3=Point2DinCV(p3);
	_p1.setPos(ptr1);
	_p2.setPos(ptr2);
	_p3.setPos(ptr3);
	aph=AntiPodalH(_p1,_p2,_p3);
	apv=AntiPodalV(_p1,_p2,_p3);

}

// NodePtr ConvexHull::getConvexHull()
// {
// 	Node* left=new Node;
// 	Node* right=new Node;
// 	left = getLeft();
// 	right = getRight();
// 	NodePtr cv;
// 	cv.leftPtr=left;
// 	cv.rightPtr=right;
// 	return cv;
// }

void
ConvexHull::memorize( Node* n )
{
  all_nodes.push_back( n );
}


NodePtr
ConvexHull::getFrontAndBack()
{
  return convexhull;
}



/**
 * Deermination de nouvelle enveloppe convexe en appliquant l'algorithme de Melkman 
 * @param point 
 * @param d 
 * @param ptr 
 * @param bot 
 * @param top 
 * @param pbot 
 * @param ptop 
 */
void ConvexHull::addPoint(Point2D point, Direction d, Node* &ptr, Node* &bot, Node* &top, Node* &pbot, Node* &ptop)
{
  
  bool antiPcorrectV =true;
  bool antiPcorrectH =true;

  
//   cout << " Rajout du point P:"<< point.getX() << " " << point.getY() <<endl;
//   cout << "direction " << d << " left=" << LEFT;
  
  Node *ptr1;

  Node* front = getFront();
  Node* back = getBack();

  previous_convexhull.leftPtr = convexhull.leftPtr;
  previous_convexhull.rightPtr = convexhull.rightPtr;

  bool modified = false;
  if(d==LEFT)
    {
      ptr = back;
      modified = jolInsertMelkmanBack( *this, front, back, 
				       bot, top, pbot, ptop, point );
      if ( modified ) ptr1 = back;
    }
  else
    {
      ptr = front;
      modified = jolInsertMelkmanFront( *this, front, back,
					bot, top, pbot, ptop, point );
      if ( modified ) ptr1 = front;
    }
  setFront( front );
  setBack( back );

	
  if ( !modified ) return;

//   cout << endl << "ADDPOINT " << point.getX()<< "," << point.getY() << endl;
//   afficheConvex(Color(120,100,10));	
  
  
//   cout << "\\croixx{(" << point.getX()+.1<< "," << point.getY()+.1 << ")}{red}" <<endl;
//   cout << endl;

//   cout << "\\croixx{(" << getContent(*getRight()).getX()<< "," << getContent(*getRight()).getY() << ")}{blue}" <<endl;
//   cout << "\\croixx{(" << getContent(*getLeft()).getX()<< "," << getContent(*getLeft()).getY() << ")}{darkgreen}" <<endl;

  
  
  
  AntiPodalH preAPH=getAPH();
  AntiPodalH antipodalH=determineAPH(ptr1,preAPH,point);
  if(!antipodalH.isCorrect()) 
    {
      //afficheConvex();
      antiPcorrectH =false;
      cout<<"AntipodalH determination is error at "<<antipodalH;
      cout<<"added point: "<<point;
      cout<<"previous APH "<<preAPH;
      Point2D p0 = antipodalH.getVertex();
      Point2D p1 = antipodalH.getEdge().getBegin();
      Point2D p2 = antipodalH.getEdge().getEnd();
	    
      //cout << "\\psline[linewidth=0.05, linecolor=green](" << p1.getX() << "," << p1.getY()<<")";
      //cout << "(" << p2.getX()<< "," << p2.getY() << ")"<< endl;
      //cout << "\\croix{(" << p0.getX()<< "," << p0.getY() << ")}{green}" <<endl;
	    
      p0 = preAPH.getVertex();
      p1 = preAPH.getEdge().getBegin();
      p2 = preAPH.getEdge().getEnd();
	    
      //cout << "\\psline[linewidth=0.05, linecolor=yellow](" << p1.getX() << "," << p1.getY()<<")";
      //cout << "(" << p2.getX()<< "," << p2.getY() << ")"<< endl;
      //cout << "\\croix{(" << p0.getX()<< "," << p0.getY() << ")}{yellow}" <<endl;
	     
	    
      exit(1);
    }
  setAPH(antipodalH);
	
  AntiPodalV preAPV=getAPV();
  AntiPodalV antipodalV=determineAPV(ptr1,preAPV,point);
  if(!antipodalV.isCorrect())
    {
      afficheConvex( Color( 200, 0, 200 ) );
      antiPcorrectV =false;
      cout<<endl<<"AntipodalV determination is error at "<<antipodalV;	
      cout<<"added point: "<<point;
      cout<<"previous APV"<<preAPV;	
	  
      Point2D p0 = antipodalV.getVertex();
      Point2D p1 = antipodalV.getEdge().getBegin();
      Point2D p2 = antipodalV.getEdge().getEnd();
	  
      cout << "\\psline[linewidth=0.2, linecolor=red](" << p1.getX() << "," << p1.getY()<<")";
      cout << "(" << p2.getX()<< "," << p2.getY() << ")"<< endl;
      cout << "\\croix{(" << p0.getX()<< "," << p0.getY() << ")}{red}" <<endl;
	  
	  
      p0 = preAPV.getVertex();
      p1 = preAPV.getEdge().getBegin();
      p2 = preAPV.getEdge().getEnd();
	  
      cout << "\\psline[linewidth=0.2, linecolor=blue](" << p1.getX() << "," << p1.getY()<<")";
      cout << "(" << p2.getX()<< "," << p2.getY() << ")"<< endl;
      cout << "\\croix{(" << p0.getX()<< "," << p0.getY() << ")}{blue}" <<endl;
      cout << "\\croix{(" << point.getX()<< "," << point.getY() << ")}{blue}" <<endl;
	  
      exit(1);
    }
  setAPV(antipodalV);
	
  if (!antiPcorrectV && ! antiPcorrectH){

    cout << "aucun antipodal bon!!!"<< endl;
    exit(1);
  }

//   AntiPodalV avv=getAPV();
//   cout<<"New antipodalV: "<<avv;
//   cout<<"Height "<<avv.height()<<endl;	
//   AntiPodalH ahh=getAPH();
//   cout<<"New antipodalH:"<<ahh;
//   cout<<"Large "<<ahh.large()<<endl;
	
  //cout <<" après rajout" << endl;


}






// /**
//  * Deermination de nouvelle enveloppe convexe en appliquant l'algorithme de Melkman 
//  * @param point 
//  * @param d 
//  */
// void ConvexHull::addPoint(Point2D point, Direction d)
// {
//   Node *ptr, *ptr1;	

	
// 	if(d==LEFT)
// 	{
// 		ptr=getLeft(); 		
// 		ptr1=insertMelkman(ptr,point);
// 		setLeft(ptr1);
// 	}
// 	else 
// 	{
// 		ptr=getRight();
// 		ptr1=insertMelkman(ptr,point);		
// 		setRight(ptr1);
// 	}
	
//    	#ifdef DEBUG
// 	cout<<"==========Added point============"<<endl;
// 	cout<<point;
// 	cout<<"New convex hull"<<endl;
// 	printCirculator(ptr1);
//     	#endif	 	
	
// 	AntiPodalH preAPH=getAPH();

// 	AntiPodalH antipodalH=determineAPH(ptr1,preAPH,point);

// 	if(!antipodalH.isCorrect()) 
// 	  {
// 	    cout<<"AntipodalH determination is error at "<<antipodalH;
// 	    cout<<"previous APH "<<preAPH;
// 	    exit(1);
// 	  }
// 	setAPH(antipodalH);
	
	
// 	AntiPodalV preAPV=getAPV();
// 	AntiPodalV antipodalV=determineAPV(ptr1,preAPV,point);
// 	if(!antipodalV.isCorrect())
// 	{
// 		cout<<endl<<"AntipodalV determination is error at "<<antipodalV;	
// 		cout<<"previous APV"<<preAPV;	
// 		exit(1);
// 	}
// 	setAPV(antipodalV);
	
//         #ifdef DEBUG	
// 	AntiPodalV avv=getAPV();
//  	cout<<"New antipodalV: "<<avv;
//  	cout<<"Height "<<avv.height()<<endl;
//   	AntiPodalH ahh=getAPH();
//   	cout<<"New antipodalH:"<<ahh;
//   	cout<<"Large "<<ahh.large()<<endl;
//     	#endif
// }

/**
 * Construire l'enveloppe convexe precedente
 * @param dr 
 * @param ptr 
 * @param bot 
 * @param top 
 * @param pbot 
 * @param ptop 
 */
void ConvexHull::setPreviousConvexHull(Direction dr, Node* &ptr,Node* &bot, Node* &top, Node* &pbot, Node* &ptop)
{	


  pbot->right=bot;
  bot->left=pbot;
  ptop->left=top;
  top->right=ptop;	
  if(dr==RIGHT) setRight(ptr);
  else setLeft(ptr);
}

void 
ConvexHull::jolSetPreviousConvexHull( Node* &bot, Node* &top, 
				      Node* &pbot, Node* &ptop)
{
  //setCCW( pbot, bot );
  setCW( bot, pbot );
  //setCW( ptop, top );
  setCCW( top, ptop );
  convexhull.leftPtr = previous_convexhull.leftPtr;
  convexhull.rightPtr = previous_convexhull.rightPtr;
}



Node* ConvexHull::getLeft()
{	
	return convexhull.leftPtr;
}

Node* ConvexHull::getRight()
{
	return convexhull.rightPtr;
}

void ConvexHull::setLeft(Node* ptr)
{	
	convexhull.leftPtr=ptr;
}

void ConvexHull::setRight(Node* ptr)
{
	convexhull.rightPtr=ptr;
}
const AntiPodalH & ConvexHull::getAPH() const
{
	return this->aph; 
}
const AntiPodalV & ConvexHull::getAPV() const
{
	return this->apv;
}
bool ConvexHull::isHeightLine() const
{
	AntiPodalH ah = getAPH();
	AntiPodalV av = getAPV();
	double height=av.height();
	double large=ah.large();
	if(height<large) return true; else return false;
}

double ConvexHull::getOptimalEpais() const
{
	AntiPodalH ah = getAPH();
	AntiPodalV av = getAPV();
	double height=av.height();
	double large=ah.large();
	if(height<large) return height; else return large;
}






void ConvexHull::afficheConvex(Color color) const
{

  cout << "\\newrgbcolor{cv}{" << color.getR()/255.0 << " " <<  color.getG()/255.0  << " " <<  color.getB()/255.0  << "} " <<endl;

  Point2D point;
  Node* ptr = convexhull.rightPtr;
  do{
    point = getContent(*ptr);
    cout << "\\croix{(" << point.getX()<< "," << point.getY() << ")}{cv}" <<endl;
    cout << "\\psline[linewidth=0.05, linecolor=cv, arrowsize=0.5]{->}(" << point.getX() << "," << point.getY()<<")";
    cout << "(" <<  getContent(*getCCW(ptr)).getX() << "," <<  getContent(*getCCW(ptr)).getY()<<")";
    ptr=getCCW(ptr);
  }
  while(ptr != convexhull.rightPtr);

  ptr = convexhull.leftPtr;
  do{
    point = getContent(*ptr);
    cout << "\\psline[linewidth=0.05, linecolor=cv, arrowsize=0.5]{->}(" << point.getX() << "," << point.getY()<<")";
    cout << "(" <<  getContent(*getCW(ptr)).getX() << "," <<  getContent(*getCW(ptr)).getY()<<")";
    ptr=getCW(ptr);
  }
  while(ptr != convexhull.leftPtr);
  
  cout << endl;
  
  cout << "\\croixx{(" << getContent(*getFront()).getX()<< "," << getContent(*getFront()).getY() << ")}{blue}" <<endl;
  cout << "\\croixx{(" << getContent(*getBack()).getX()<< "," << getContent(*getBack()).getY() << ")}{darkgreen}" <<endl;



//   Color color3(10, 200,20);
//   Color color4(200, 00,20);
  Line l = getAPH().getEdge();
  Line l2 = getAPV().getEdge();
  
  Point2D deb = getContent(* l.getBegin().getPos());
  Point2D fin = getContent(* l.getEnd().getPos());
  
  Point2D deb2 = getContent(* l2.getBegin().getPos());
  Point2D fin2 = getContent(* l2.getEnd().getPos());
  
  
  double apvh = getAPV().height();
  double aphl = getAPH().large();
  
  if ( apvh < aphl ) 
    {
      // Epaisseur vertical plus petite => droite "horizontale"
      cout << "\\psline[linewidth=0.05, linecolor=green, arrowsize=0.5]{->}(" << deb2.getX() << "," << deb2.getY()<<")(" << fin2.getX() << "," << fin2.getY()<<")" <<endl;;
      
    }else
    
    {
   cout << "\\psline[linewidth=0.05, linecolor=yellow, arrowsize=0.5]{->}(" << deb.getX() << "," << deb.getY()<<")(" << fin.getX() << "," << fin.getY()<<")" <<endl;;
    }
  
   //   deb.afficheLine(graphic, color3, fin);
   //deb2.afficheLine(graphic, color4, fin2);
//    deb.affichePixelPoint(graphic, 0,255,0);
//     fin.affichePixelPoint(graphic, 0,100,0);
//   deb2.affichePixelPoint(graphic, 255,0, 0);
  



}







void ConvexHull::afficheConvex(Graphic graphic, Color color)
{
  
  Point2D point;
  Node* ptr = convexhull.rightPtr;
  do{
    point = getContent(*ptr);
    point.affichePixelPoint(graphic, color);
    point.afficheLine(graphic, color, getContent(*getNext(ptr)));
    ptr=getNext(ptr);
  }
  while(ptr != convexhull.rightPtr);
  
  Color color3(10, 200,20);
  Color color4(200, 00,20);
  Line l = getAPH().getEdge();
  Line l2 = getAPV().getEdge();
  
  Point2D deb = getContent(* l.getBegin().getPos());
  Point2D fin = getContent(* l.getEnd().getPos());
  
  Point2D deb2 = getContent(* l2.getBegin().getPos());
  Point2D fin2 = getContent(* l2.getEnd().getPos());
  
  
  
  deb.afficheLine(graphic, color3, fin);
  deb2.afficheLine(graphic, color4, fin2);
  deb.affichePixelPoint(graphic, 0,255,0);
  fin.affichePixelPoint(graphic, 0,100,0);
  deb2.affichePixelPoint(graphic, 255,0, 0);
  
  

}





ConvexHull::~ConvexHull()
{
  for ( uint i = 0; i < all_nodes.size(); ++ i )
    delete all_nodes[ i ];
}


