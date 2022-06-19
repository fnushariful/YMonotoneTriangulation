//
// Created by FNU Shariful on 4/13/22.
//



#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <stack>
#include "CGAL/Triangle_2.h"

#include <CGAL/Simple_cartesian.h>

using namespace std;

typedef CGAL::Simple_cartesian<double> K;
typedef K::Point_2 Point;
typedef std::pair<unsigned,unsigned> Edge;
typedef K::Segment_2 Segment_2;

class Vertex{
public:
    unsigned index;
    Point P;
};

vector<Vertex> poly,eventQ,leftChain,rightChain;

void drawPolygonUsingQT(const std::vector<Vertex> &polygonVertices,std::vector<Edge> &diagonals,const bool labels) {

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
        verticesForQTPolygon.emplace_back( QPointF(p.P.x(),p.P.y() ) );

    canvas.drawPolygon(&verticesForQTPolygon[0], (int)verticesForQTPolygon.size());

    for ( Edge e : diagonals ) {
        cout<<e.first<<" "<<e.second<<endl;
        QPointF endPointA(poly.at(e.first).P.x(),poly[e.first].P.y() ),
                endPointB(polygonVertices[e.second].P.x(),polygonVertices[e.second].P.y() );
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

        canvas.drawEllipse(QPointF(p.P.x(), p.P.y()), pointSize, pointSize);
        if(labels) {
            canvas.setBrush(Qt::black);
            canvas.setPen(Qt::black);
            QPointF textPos(p.P.x() + 4.0, p.P.y() + 4.0);
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


bool searchLeftOrRight(double stackIndex,double eventIndex)
{
    cout<<stackIndex<<" "<<eventIndex<<endl;
    bool leftChainStackIndex = false,leftChainEventIndex = false,rightChainStackIndex = false,rightChainEventIndex = false;
    for( auto x: leftChain )
    {
        if( x.index == stackIndex ) leftChainStackIndex = true;
        else if( x.index == eventIndex ) leftChainEventIndex = true;
    }

    for( auto x: rightChain )
    {
        if( x.index == stackIndex ) rightChainStackIndex = true;
        else if( x.index == eventIndex ) rightChainEventIndex = true;
    }

    if( leftChainStackIndex == rightChainEventIndex || leftChainEventIndex == rightChainStackIndex ) return true;
    return false;
}

bool isPolygonIntersect(Point b, Point c, const std::vector<Point> &verticesOfP )
{
    Segment_2 seg(b, c);

    for( unsigned i=0; i<verticesOfP.size()-1; i++ )
    {
        if( b == verticesOfP.at(i) || b == verticesOfP.at(i+1) || c == verticesOfP.at(i) || c == verticesOfP.at(i+1) )  continue;
        else {
            Segment_2 segOfPoly(verticesOfP.at(i), verticesOfP.at(i + 1));
            const auto result = intersection(seg, segOfPoly);
            if (result) return true;
        }
    }
    return false;
}

bool isDiagonalIntersect(Point b,Point c, std::vector<Vertex>&poly, std::vector<Edge> &diagonals)
{
   Segment_2 seg(b,c);
   for( unsigned i=0; i<poly.size()-1; i++ )
   {
       Segment_2 segOfPoly(poly.at(i).P,poly.at(i+1).P);
       const auto result = intersection(seg,segOfPoly);
       if( result ) return true;
   }
   return false;
}

void TriangulateYMonotoneModified(const std::vector<Point> &polygonVertices,std::vector<Edge> &diagonals) {

    for (unsigned i = 0; i <polygonVertices.size(); i++) {
        Vertex v;
        v.P = polygonVertices.at(i);
        v.index = i;
        poly.emplace_back(v);
        if( i >= 1 and i<= 5 ) leftChain.emplace_back(v);
        else rightChain.emplace_back(v);

    }

    eventQ = poly;
    int sizeOfPolygon = eventQ.size();

    sort(eventQ.begin(), eventQ.end(), [](const auto &v1, const auto &v2) {
        if (v1.P.y() == v2.P.y())
            return v1.P.x() > v2.P.x();
        else
            return v1.P.y() < v2.P.y();
    });

    cout<<"Event queue:\n";
    cout<<"Xcor Ycor index\n";
    for(int i=0;i<sizeOfPolygon;i++){
        cout<<eventQ[i].P.x()<<" "<<eventQ[i].P.y()<<" "<<eventQ[i].index<<"\n";
    }

    int frontQ = 0;
    stack<unsigned> polygonStack;

    cout << "\nPush segment" << eventQ[0].index;
    cout << "\nPush segment" << eventQ[1].index;
    polygonStack.push(eventQ[0].index);
    polygonStack.push(eventQ[1].index);
    frontQ = 2;

    for( unsigned i =2; i<polygonVertices.size()-1; i++ ){
        cout<<"In while"<<endl;
        int val = searchLeftOrRight(polygonStack.top(),eventQ[i].index);
        cout<<"val "<<val<<endl;
//        int val = 1;
        if (val == 1) {
            while (!polygonStack.empty()) {
                unsigned vertexStackTop = polygonStack.top();
                if(!polygonStack.empty())
                polygonStack.pop();
//                if(!isPolygonIntersect(eventQ.at(i).P,eventQ.at(vertexStackTop).P,polygonVertices))
                {
                    cout << "\nDifferent chain: Insert diagonal between " << vertexStackTop << " and "
                         << eventQ[i].index;
                    diagonals.emplace_back(make_pair(vertexStackTop, eventQ[i].index));
//                    poly.emplace_back(eventQ.at(i));
//                    poly.emplace_back(eventQ.at(vertexStackTop));
                    cout << "\nPop out segment" << vertexStackTop;
//                    polygonStack.pop();
                }
            }
            if(!polygonStack.empty()) polygonStack.pop();
            cout << "\nPush segment" << eventQ[i - 1].index;
            polygonStack.push(eventQ[i - 1].index);
            cout << "\nPush segment" << eventQ[i].index;
            polygonStack.push(eventQ[i].index);
        }
        else
        {
            unsigned vertexStackTop = polygonStack.top();
            if(!polygonStack.empty())
            polygonStack.pop();
//            while(!isDiagonalIntersect(eventQ.at(polygonStack.top()).P,eventQ.at(i).P,poly,diagonals))
            while(CGAL::orientation(eventQ.at(i).P,eventQ.at(polygonStack.top()).P,eventQ.at(vertexStackTop).P) == CGAL::LEFT_TURN )
            {
                cout<<"\nSame chain: Insert diagonal between "<<polygonStack.top()<<" and "<<eventQ[i].index<<endl;
                vertexStackTop = polygonStack.top();
                diagonals.emplace_back(make_pair(vertexStackTop, eventQ[i].index));
//                poly.emplace_back(eventQ.at(i));
//                poly.emplace_back(eventQ.at(vertexStackTop));
                polygonStack.pop();
            }
            polygonStack.push(vertexStackTop);
            polygonStack.push(eventQ.at(i).index);
        }
    }
//    while(!polygonStack.empty())
//    {
//        diagonals.emplace_back(make_pair(eventQ.at(eventQ.size()-1).index,polygonStack.top()));
//        cout << "\nDifferent chain: Insert diagonal between " << eventQ.at(eventQ.size()-1).index << " and "
//             << polygonStack.top();
//        polygonStack.pop();
//    }
    cout<<"\nPolygon is triangulated"<<polygonStack.size();
    drawPolygonUsingQT(poly,diagonals,true);
}



