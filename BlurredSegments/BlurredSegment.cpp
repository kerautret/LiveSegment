#include <cstdlib>
#include "BlurredSegment.h"
#include "color.h"
#include "point2d.h"
#include "util.h"
#include "pointset.h"








BlurredSegment::BlurredSegment(Point2D point1, Point2D point2, Point2D point3,
			       int maxThickness){
  convexhull = new ConvexHull(point1, point2, point3);
  this->maxThickness=maxThickness;
  vectorPixel.push_back(point1);
  vectorPixel.push_back(point2);
  vectorPixel.push_back(point3);
  this->point1=point1;
  this->point2=point2;
  this->point3=point3;
  this->nbPoint=3;
}


BlurredSegment::~BlurredSegment(){
  delete convexhull;
  
  
}







QPoint
BlurredSegment::getCenter(){
  return QPoint(point1.getX(),point1.getY());
}




bool
BlurredSegment::addPoint(Point2D p, Direction d){
  Node *bot, *top, *pbot, *ptop, *ptrStart;
  AntiPodalH preAPH;
  AntiPodalV preAPV;
  NodePtr preConvexHull;
  
  
  preConvexHull=convexhull->getFrontAndBack();
  preAPH=convexhull->getAPH();
  preAPV=convexhull->getAPV();
  
  convexhull->addPoint(p, d, ptrStart, bot, top, pbot, ptop);
  
  if(getThickness()>getMaxThickness()){
    // traitement particulier pour le retour en arrière 
    if(nbPoint==3){
      delete convexhull;
      this->convexhull = new ConvexHull(point1, point2, point3);
      return false;
    }else{
      //cerr<< "avant jol" << endl;
      convexhull->jolSetPreviousConvexHull(bot, top, pbot, ptop); 
      //cerr << " après retour arrière"<< endl;
      convexhull->setAPH(preAPH);
      convexhull->setAPV(preAPV);
      
      return false;
    }
  }else{
    nbPoint++;
  }
  
  if(d==RIGHT){
    lastRightPointAdded = p;
  }else{
    lastLeftPointAdded = p;
  }
  vectorPixel.push_back(p);
  //  cerr << " après retour arrière"<< endl;
  return true;

}




ConvexHull *
BlurredSegment::getConvexHull(){
  return convexhull;
 
}


vector<Point2D>
BlurredSegment::getListePixel(){
  return vectorPixel;
}


vector<QPoint>
BlurredSegment::getListeQPoint(){
  vector<QPoint> vectResult;
  vector<Point2D>::iterator iter = vectorPixel.begin();
  while( iter !=vectorPixel.end()){
    Point2D p = *iter;
    vectResult.push_back(QPoint(p.getX(),p.getY()));
    iter++;
  }  
  return vectResult;
}


 
double 
BlurredSegment::getThickness(){
  return convexhull->getOptimalEpais(); 
}





double 
BlurredSegment::getMaxThickness(){
  return maxThickness; 
}




double 
BlurredSegment::getDirection(){
  double apvh = (double)convexhull->getAPV().height();
  double aphl = (double)convexhull->getAPH().large();
  double apslope;
  if ( apvh < aphl ) 
    {
      // Epaisseur vertical plus petite => droite "horizontale"
      apslope = (double) convexhull->getAPV().getEdge().slope();//apvSlope;
    }
  else
    {
      // Epaisseur horizontale plus petite => droite "verticale"
      apslope = (double) convexhull->getAPH().getEdge().slope();
      
    }
  return apslope;
}






vector<QPoint> 
BlurredSegment::getPrintedPoints(){
  Point2D d = lastLeftPointAdded;
  Point2D f = lastRightPointAdded;
  vector<QPoint> vectResult;
  
  
  double apvh = (double)convexhull->getAPV().height();
  double aphl = (double)convexhull->getAPH().large();
  
  
  vector<Point2D>::iterator iterPoints;
  iterPoints = vectorPixel.begin();
  
  int Ymin=0, Ymax=0, Xmax=0, Xmin=0;
  bool first = true;
  
  while(iterPoints!=vectorPixel.end()){
    Point2D ps = *iterPoints;   
    if(first){
      first= false;
      Ymax=ps.getY();
      Ymin=ps.getY();
      Xmax=ps.getX();
      Xmin=ps.getX();
	
    }else{
      if(Ymin>ps.getY())
	Ymin=ps.getY();
      if(Ymax<ps.getY())
	Ymax=ps.getY();
      if(Xmin>ps.getX())
	Xmin=ps.getX();
      if(Xmax<ps.getX())
	Xmax=ps.getX();
    }
    iterPoints++;
  }

  // P5-------- P6//
  // P3-------- P4//
  

  if ( apvh < aphl ){
    Line lVerti=  convexhull->getAPV().getEdge();
    Point2DinCV p =  convexhull->getAPV().getVertex();
    Point2D p1 = lVerti.getBegin();
    Point2D p2 = lVerti.getEnd();
        
    double a = tan(getDirection());
    double b = -(double)(p1.getX())*a +p1.getY();
    double p3 [2] = {Xmax, a*(double)(Xmax)+b};
    double p4 [2] = {Xmin, a*(double)(Xmin)+b};
    double b2 = -(double)p.getX()*a +(double)p.getY();
    double p5[2] = {Xmin, a*(double)(Xmin)+b2};
    double p6[2] = {Xmax, a*(double)(Xmax)+b2};
    
    
    vector<QPoint> droiteSup = PointSet::tracerSegment(QPoint(p3[0],p3[1]),
								      QPoint(p4[0],p4[1]));
    
    vector<QPoint> droiteInf = PointSet::tracerSegment(QPoint(p5[0],p5[1]),
								      QPoint(p6[0],p6[1]));
    
    PointSet::fusionVector(vectResult, droiteSup);
    PointSet::fusionVector(vectResult, droiteInf);
    
    
//     cout << "\n" << styleLineXFIG      
// 	 << "\n" << (int) p4[0]*RESOLUTION << " " << (int) p4[1]*RESOLUTION << " " 
// 	 <<(int) p3[0]*RESOLUTION << " " <<(int) p3[1]*RESOLUTION << " " 
// 	 <<(int) p6[0]*RESOLUTION <<  " " << (int)p6[1]*RESOLUTION << " " 
// 	 <<(int) p5[0]*RESOLUTION << " " << (int)p5[1]*RESOLUTION << " "
// 	 <<(int) p4[0]*RESOLUTION << " " << (int) p4[1]*RESOLUTION << endl;
    
    
  }else{
      Line lVerti=  convexhull->getAPH().getEdge();
      Point2DinCV p =  convexhull->getAPH().getVertex();
      Point2D p1 = lVerti.getBegin();
      Point2D p2 = lVerti.getEnd();
      

      double a = tan(getDirection());
      double b = -(double)(p1.getX())*a +p1.getY();
      double p3 [2] = {(((double)Ymax)-b)/a,Ymax};
      double p4 [2] = {(((double)Ymin)-b)/a,Ymin};
      double b2 = -(double)p.getX()*a +(double)p.getY();
      double p5[2] = {(((double)Ymin)-b2)/a, Ymin};
      double p6[2] = {(((double)Ymax)-b2)/a, Ymax};
      

      if((p2.getX() - p1.getX())==0){
	p3[0]  = p2.getX();
	p3[1]  = Ymax;
	p4[0]  = p2.getX();
	p4[1]  = Ymin;
	p5[0] = p.getX();
	p5[1] = Ymin;
	p6[0] = p.getX();
	p6[1] = Ymax;
      }
      

      vector<QPoint> droiteSup = PointSet::tracerSegment(QPoint(p3[0],p3[1]),
									QPoint(p4[0],p4[1]));
      
      vector<QPoint> droiteInf = PointSet::tracerSegment(QPoint(p5[0],p5[1]),
									QPoint(p6[0],p6[1]));
      
      PointSet::fusionVector(vectResult, droiteSup);
      PointSet::fusionVector(vectResult, droiteInf);
      



//       cout << "\n" << styleLineXFIG  
// 	   << "\n" << (int) p4[0]*RESOLUTION << " " << (int)p4[1]*RESOLUTION << " "
// 	   << (int)p3[0]*RESOLUTION << " " << (int)p3[1]*RESOLUTION << " "
// 	   << (int)p6[0]*RESOLUTION << " " << (int)p6[1]*RESOLUTION << " "
// 	   << (int)p5[0]*RESOLUTION << " " << (int)p5[1]*RESOLUTION << " " 
// 	   << (int) p4[0]*RESOLUTION << " " << (int) p4[1]*RESOLUTION <<endl;
//   }

      
      
  }

  
  return vectResult;
}

// ------------------------------------------------------
// Test de la fonction de construction de segments flous.

// int main(int argc, char *argv[])
// {
  
  

//   ImaGene::Arguments args;  
//   args.addOption("-e", "-e epaisseur", "1");
//   args.addBooleanOption("-affichePixels", "-affichePixels");
//   args.addBooleanOption("-afficheEnvloppeConvexe", "-afficheEnvloppeConvexe");
  
  
//   if ( ! args.readArguments( argc, argv )){
//     cerr << args.usage( "BlurredSegment", 
// 			"Options: ",
// 			"-affichePixels -afficheEnvloppeConvexe -e " )      
// 	 << endl;
//   }
  
  
  
  
//   Point2D p1(0,0);
//   Point2D p2(-1,2);
//   Point2D p3(3,0);
  
  
  
//   int epaisseur = args.getOption("-e")->getIntValue(0);
//   BlurredSegment blurSeg(p1,p2,p3, epaisseur);
  
//   blurSeg.addPoint(Point2D(1,-3), RIGHT);
//   blurSeg.addPoint(Point2D(3,2), RIGHT);
//   blurSeg.addPoint(Point2D(6,-1), RIGHT);
//   blurSeg.addPoint(Point2D(8,1), RIGHT);
  
  
  
  
//   int epaisseur = args.getOption("-e")->getIntValue(0);
//   BlurredSegment blurSeg(p1,p2,p3, epaisseur);
  
//   blurSeg.addPoint(Point2D(1,-3), RIGHT);
//   blurSeg.addPoint(Point2D(3,2), RIGHT);
//   blurSeg.addPoint(Point2D(6,-1), RIGHT);
//   blurSeg.addPoint(Point2D(8,1), RIGHT);
  
  
  
//   if(args.check("-affichePixels")){
//     vector<Point2D> vectPixel = blurSeg.getListePixel();
//     vector<Point2D>::iterator iter = vectPixel.begin();
//     while( iter !=vectPixel.end()){
//       Point2D p = *iter;
//       cout <<"\\pixel{(" <<  p.getX() << "," << p.getY() <<")}"<< endl;
//       iter++;
//     }
//   }
  
  
//   if(args.check("-afficheEnvloppeConvexe")){
//     blurSeg.getConvexHull()->afficheConvex(Color());
//   }

  
  
//   return EXIT_SUCCESS;  
// }



