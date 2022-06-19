//
// Created by FNU Shariful on 4/13/22.
//

#ifndef YMONOTONETRIANGULATION_TEST_H
#define YMONOTONETRIANGULATION_TEST_H
//#EJ Added Required Header Files..

//#include <windows.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
//#include <GL/glut.h>
#include <algorithm>
#include <stack>
#include <CGAL/Simple_cartesian.h>

using namespace std;

//#EJ Created a Class Object Point..
class Point1{
public:
    float x,y;
};

//#EJ Created a Class Object Vertex..
class Vertex{
public:
    int index;
    Point1 P;
    int type;
};

/*Vectors for managing Polygon Vertices*/
vector <Vertex> poly;
vector <Vertex> eventQ;

//void init(){
//
//    glClearColor(0,0,0,0);
//    glMatrixMode(GL_PROJECTION);
//    gluOrtho2D(0,640,0,480);
//
//}
//
//void plotPt(Point pt){
//
//    glColor3f(1.0,0.0,0.0);
//    glPointSize(5);
//    glBegin(GL_POINTS);
//    glVertex2f(pt.x,pt.y);
//    glEnd();
//    glFlush();
//}
//
//void plotPoly(){
//
//    glColor3f(0,1,0);
//    glPointSize(5);
//    glBegin(GL_LINE_LOOP);
//    for(int i=0;i<poly.size();i++){
//        glVertex2f(poly[i].P.x,poly[i].P.y);
//    }
//    glEnd();
//    glFlush();
//}
//
//void plotLine(Point pt,Point pt1){
//
//    glColor3f(1,0,0);
//    glLineWidth(2);
//    glBegin(GL_LINES);
//    glVertex2f(pt.x,pt.y);
//    glVertex2f(pt1.x,pt1.y);
//    glEnd();
//    glFlush();
//
//}

void drawPolygonUsingQT(const std::vector<Vertex> &polygonVertices,std::vector<pair<unsigned ,unsigned >> &diagonals,const bool labels) {

    const double pointSize = 4; // adjust this value to change the size of the points
    /***************************************************/
    QPicture pi;
    QPainter canvas(&pi);
    canvas.setRenderHint(QPainter::Antialiasing);
    //canvas.setFont(QFont("Times", 12));
    // DO NOT TOUCH THESE Qt STATEMENTS!!!
    /***************************************************/

    canvas.setBrush(Qt::lightGray);

    std::vector<QPointF> verticesForQTPolygon;
    verticesForQTPolygon.reserve(polygonVertices.size());
    for( auto p : polygonVertices )
        verticesForQTPolygon.emplace_back( QPointF(p.P.x,p.P.y ) );

    canvas.drawPolygon(&verticesForQTPolygon[0], (int)verticesForQTPolygon.size());

    for ( auto e : diagonals ) {
        cout<<e.first<<" "<<e.second<<endl;
        QPointF endPointA(poly[e.first].P.x,poly[e.first].P.y),
                endPointB(poly[e.second].P.x,poly[e.second].P.y);
        canvas.drawLine( endPointA, endPointB );
    }

    unsigned id = 0;
    for ( auto p : polygonVertices ) {

//        if( vertexColors[id] == Qt::red) {
//            canvas.setBrush(Qt::red); canvas.setPen(Qt::red);
//        }
//        else if( vertexColors[id] == Qt::darkGreen) {
//            canvas.setBrush(Qt::darkGreen); canvas.setPen(Qt::darkGreen);
//        }
//        else {
//            canvas.setBrush(Qt::blue); canvas.setPen(Qt::blue);
//        }

        canvas.drawEllipse(QPointF(p.P.x, p.P.y), pointSize, pointSize);
        if(labels) {
            canvas.setBrush(Qt::black);
            canvas.setPen(Qt::black);
            QPointF textPos(p.P.x + 4.0, p.P.y + 4.0);
            canvas.drawText(textPos, QString(std::to_string(id).c_str()));
//            canvas.drawText(textPos, QString(std::to_string(p.y()).c_str()));
        }
        id++;
    }

    /*********************************************/
    canvas.end();
    auto l = new QLabel;
    l->setStyleSheet("QLabel { background-color : white; color : black; }");
    l->setPicture(pi);
    l->setWindowTitle("Polygon Triangulation");
    l->show();
    // l->showMaximized();
    QApplication::exec();
    // DO NOT TOUCH THESE Qt STATEMENTS!!!
    /***************************************************/
}

float areaofTriangle(Point1 a,Point1 b,Point1 c){
    return((b.x-a.x)*(c.y-a.y)-(c.x-a.x)*(b.y-a.y));
}

bool Left(Point1 a,Point1 b,Point1 c){
    return areaofTriangle(a,b,c) > 0;
}

bool LeftOn(Point1 a,Point1 b,Point1 c){
    return areaofTriangle(a,b,c) >= 0;
}

bool Collinear(Point1 a,Point1 b,Point1 c){
    return areaofTriangle(a,b,c) == 0;
}

bool Xor(bool x,bool y){
    return !x ^ !y;
}

bool Intersection(Point1 a,Point1 b,Point1 c,Point1 d){
    if(Collinear(a,b,c)||Collinear(a,b,d)||Collinear(c,d,a)||Collinear(c,d,b))
        return false;
    return Xor(Left(a,b,c),Left(a,b,d)) && Xor(Left(c,d,a),Left(c,d,b));

}

bool Between(Point1 a,Point1 b,Point1 c){
    if(!Collinear(a,b,c))
        return false;
    if(a.x!=b.x){
        return(((a.x<=c.x)&&(c.x<=b.x))||((a.x>=c.x)&&(c.x>=b.x)));
    }
    else
        return(((a.y<=c.y)&&(c.y<=b.y))||((a.y>=c.y)&&(c.y>=b.y)));
}

bool Intersect(Point1 a,Point1 b,Point1 c,Point1 d){
    if(Intersection(a,b,c,d)){
        return true;
    }
    else if(Between(a,b,c)||Between(a,b,d)||Between(c,d,a)||Between(c,d,b))
        return true;
    else{
        return false;
    }
}

bool isDiagonal(Vertex a,Vertex b){
    for(int i=0;i<poly.size()-1;i++){
        if((poly[i].index!=a.index)&& (poly[i+1].index!=a.index)&&(poly[i].index!=b.index)&& (poly[i+1].index!=b.index)&&Intersect(a.P,b.P,poly[i].P,poly[i+1].P)){
            return false;
        }
    }
    if((poly[poly.size()-1].index!=a.index)&& (poly[0].index!=a.index)&&(poly[poly.size()-1].index!=b.index)&& (poly[0].index!=b.index)&&Intersect(a.P,b.P,poly[poly.size()-1].P,poly[0].P)){
        return false;
    }
    return true;
}

bool InCone(Vertex a,Vertex b){
    if(LeftOn(a.P,poly[a.index].P,poly[a.index-2].P)){
        return(Left(a.P,b.P,poly[a.index-2].P) && Left(b.P,a.P,poly[a.index].P));
    }
    return(!(Left(a.P,b.P,poly[a.index].P) && Left(b.P,a.P,poly[a.index-2].P)));
}

bool Diagonal(Vertex a, Vertex b){
    return(InCone(a,b) && InCone(b,a) && isDiagonal(a,b));
}

bool comp(Vertex v1,Vertex v2){
    if(v1.P.y==v2.P.y)
        return v1.P.x<v2.P.x;
    else
        return v1.P.y>v2.P.y;
}

bool compx(pair<int,int> a,pair<int,int> b){
    return(a.second < b.second);
}

bool right(Vertex cur,Vertex prev){
    if(cur.P.y < prev.P.y)
        return true;
    else if(cur.P.y == prev.P.y){
        if(cur.P.x < prev.P.x)
            return false;
    }
    return false;
}


void Triangulate(){
    vector<Point1> polygonVertices;
    vector<pair<unsigned ,unsigned >> diagonals;
    polygonVertices.push_back({391, 374});
    polygonVertices.push_back({240, 431});
    polygonVertices.push_back({252, 340});
    polygonVertices.push_back({374, 320});
//    polygon.push_back(Point(289, 214));
//    polygon.push_back(Point(134, 390));
//    polygon.push_back(Point( 68, 186));
    polygonVertices.push_back({154, 259});
    polygonVertices.push_back({161, 107});
    polygonVertices.push_back({435, 90});
    polygonVertices.push_back({208, 148});
    polygonVertices.push_back({295, 160});
    polygonVertices.push_back({421, 212});
    polygonVertices.push_back({441, 303});


    for (unsigned i = 0; i <polygonVertices.size(); i++) {
        Vertex v;
        v.P = polygonVertices.at(i);
        v.index = i;
        poly.emplace_back(v);
//        if( i >= 1 and i<= 5 ) leftChain.emplace_back(v);
//        else rightChain.emplace_back(v);
    }

//    sort(eventQ.begin(), eventQ.end(), [](const auto &v1, const auto &v2) {
//        if (v1.P.y == v2.P.y)
//            return v1.P.x < v2.P.x;
//        else
//            return v1.P.y > v2.P.y;
//    });

    for( int i=0; i<poly.size()-1; i++ )
    {
        cout<<"is Diagonal "<<poly.at(i).index<<" "<<poly.at(i+1).index<<" "<<isDiagonal(poly.at(4),poly.at(8))<<endl;
    }

    int n = poly.size();
    eventQ=poly;
    sort(eventQ.begin(),eventQ.end(),comp);
    cout<<"Event queue:\n";
    cout<<"Xcor Ycor index\n";
    for(int i=0;i<n;i++){
        cout<<eventQ[i].P.x<<" "<<eventQ[i].P.y<<" "<<eventQ[i].index<<"\n";
    }
    int frontQ;
    stack <int> s;
    cout<<"\nPush segment"<<eventQ[0].index;
    cout<<"\nPush segment"<<eventQ[1].index;
    s.push(eventQ[0].index);
    s.push(eventQ[1].index);
    frontQ=2;
    while(frontQ!=n){
        cout<<"front "<<frontQ<<endl;
        int val=right(poly[s.top()-1],poly[s.top()-2])^ right(poly[eventQ[frontQ].index-1],poly[eventQ[frontQ].index-2]);
        cout<<"val "<<endl;
        if(val==1){
            while(!s.empty()){
                int ind=s.top();
                int qind=eventQ[frontQ].index,prev1,next1;
                if(qind==1) prev1=poly.size();
                else prev1=qind-1;
                if(qind==poly.size())next1=1;
                else next1=qind+1;
                if(ind!=next1 && ind!=prev1 && isDiagonal(eventQ[s.top()],eventQ[frontQ])){
                    cout<<"\nDifferent chain: Insert diagonal between "<<s.top()<<" and "<<eventQ[frontQ].index;
                    diagonals.push_back(make_pair(s.top(),eventQ[frontQ].index));
//                    plotLine(poly[s.top()-1].P,poly[eventQ[frontQ].index-1].P);
//                    poly.push_back(eventQ[s.top()]);
//                    poly.push_back(eventQ[frontQ]);
                }
                cout<<"\nPop out segment"<<s.top();
                s.pop();
            }
            cout<<"\nPush segment"<<eventQ[frontQ-1].index;
            s.push(eventQ[frontQ-1].index);
            cout<<"\nPush segment"<<eventQ[frontQ].index;
            s.push(eventQ[frontQ].index);
        }
        else{
            int ch=1;
            int pind;
            while(ch==1){
                int ind=s.top();
                int c=eventQ[frontQ].index;
                cout<<"Diagonal "<<Diagonal(poly[ind-1],poly[c-1])<<endl;
                if(Diagonal(poly[ind-1],poly[c-1])){
                    int qind=eventQ[frontQ].index,prev1,next1;
                    if(qind==1) prev1=poly.size();
                    else prev1=qind-1;
                    if(qind==poly.size())next1=1;
                    else next1=qind+1;
                    if(ind!=next1 && ind!=prev1){
                        cout<<"\nSame chain: Insert diagonal between "<<s.top()<<" and "<<eventQ[frontQ].index;
                        diagonals.push_back(make_pair(s.top(),eventQ[frontQ].index));
//                        plotLine(poly[s.top()-1].P,poly[eventQ[frontQ].index-1].P);
//                        poly.push_back(eventQ[s.top()]);
//                        poly.push_back(eventQ[frontQ]);
                        pind=s.top();
                        cout<<"\nPop out segment"<<pind;
                        s.pop();
                    }
                }
                else{

                    pind=s.top();
                    s.pop();
                    ch=0;
                }
            }
            cout<<"\nPush segment"<<pind;
            s.push(pind);
            s.push(eventQ[frontQ].index);
        }
        frontQ++;

    }
    cout<<"\nPolygon is triangulated"<<endl;
    drawPolygonUsingQT(poly,diagonals,true);
}

//static void display(void){
//    glFlush();
//}

//int winHeight=480;
//void mousePlot(int button,int state,int x,int y){
//    static int countv=0;
//    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
//        y=winHeight-y;
//        Point pt;
//        pt.x=x;
//        pt.y=y;
//        plotPt(pt);
//        Vertex v;
//        v.P=pt;
//        v.index=countv+1;
//        poly.push_back(v);
//        countv++;
//    }
//    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
//        plotPoly();
//        Triangulate();
//    }
//}

//int main(int argc,char **argv){
////    glutInit(&argc,argv);
////    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
////    glutInitWindowPosition(0,0);
////    glutInitWindowSize(640,480);
////    glutCreateWindow("Triangulation");
////    init();
////    glutDisplayFunc(display);
////    glutMouseFunc(mousePlot);
////    glutMainLoop();
//
//}

#endif //YMONOTONETRIANGULATION_TEST_H
