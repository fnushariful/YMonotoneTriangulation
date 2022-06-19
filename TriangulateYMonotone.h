//
// Created by FNU Shariful on 4/11/22.
//

#ifndef YMONOTONETRIANGULATION_TRIANGULATEYMONOTONE_H
#define YMONOTONETRIANGULATION_TRIANGULATEYMONOTONE_H

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

class Vertex{
public:
    int index;
    Point P;
    int type;
};

vector<Vertex> poly;
vector<Vertex> eventQ;

float areaofTriangle(Point a,Point b,Point c){
    return((b.x()-a.x())*(c.y()-a.y())-(c.x()-a.x())*(b.y()-a.y()));
}

bool Left(Point a,Point b,Point c){
    return areaofTriangle(a,b,c) > 0;
}

bool LeftOn(Point a,Point b,Point c){
    return areaofTriangle(a,b,c) >= 0;
}

bool InCone(Vertex a,Vertex b){
    if(LeftOn(a.P,poly[a.index].P,poly[a.index-2].P)){
        return(Left(a.P,b.P,poly[a.index-2].P) && Left(b.P,a.P,poly[a.index].P));
    }
    return(!(Left(a.P,b.P,poly[a.index].P) && Left(b.P,a.P,poly[a.index-2].P)));
}

bool Collinear(Point a,Point b,Point c){
    return areaofTriangle(a,b,c) == 0;
}

bool Xor(bool x,bool y){
    return !x ^ !y;
}

bool Intersection(Point a,Point b,Point c,Point d){
    if(Collinear(a,b,c)||Collinear(a,b,d)||Collinear(c,d,a)||Collinear(c,d,b))
        return false;
    return Xor(Left(a,b,c),Left(a,b,d)) && Xor(Left(c,d,a),Left(c,d,b));

}

bool Between(Point a,Point b,Point c){
    if(!Collinear(a,b,c))
        return false;
    if(a.x()!=b.x()){
        return(((a.x()<=c.x())&&(c.x()<=b.x()))||((a.x()>=c.x())&&(c.x()>=b.x())));
    }
    else
        return(((a.y()<=c.y())&&(c.y()<=b.y()))||((a.y()>=c.y())&&(c.y()>=b.y())));
}

bool Intersect(Point a,Point b,Point c,Point d){
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
    for(int i=0;i<poly.size();i++){
//        cout<<a.index<<" "<<b.index<<" "<<poly[i].index<<" "<<poly[i+1].index<<endl;
        if((poly[i].index!=a.index)&& (poly[i+1].index!=a.index)&&(poly[i].index!=b.index)&& (poly[i+1].index!=b.index)&&Intersect(a.P,b.P,poly[i].P,poly[i+1].P)){
            return false;
        }
    }
    if((poly[poly.size()-1].index!=a.index)&& (poly[0].index!=a.index)&&(poly[poly.size()-1].index!=b.index)&& (poly[0].index!=b.index)&&Intersect(a.P,b.P,poly[poly.size()-1].P,poly[0].P)){
        return false;
    }
    return true;
}

bool Diagonal(Vertex a, Vertex b){
    return(InCone(a,b) && InCone(b,a) && isDiagonal(a,b));
}

bool right(Vertex cur,Vertex prev){
    cout<<"In right" <<endl;
    if(cur.P.y() < prev.P.y())
        return true;
    else if(cur.P.y() == prev.P.y()){
        if(cur.P.x() < prev.P.x())
            return false;
    }
    return false;
}
vector<Vertex> leftChain,rightChain;

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

void TriangulateYMonotone(const std::vector<Point> &polygonVertices,std::vector<Edge> &diagonals) {

    for (unsigned i = 0; i <polygonVertices.size(); i++) {
        Vertex v;
        v.P = polygonVertices.at(i);
        v.index = i;
        poly.emplace_back(v);
        if( i >= 1 and i<= 5 ) leftChain.emplace_back(v);
        else rightChain.emplace_back(v);
    }

    for( int i=0; i<poly.size()-1; i++ )
    {
        cout<<"is Diagonal "<<poly.at(i).index<<" "<<poly.at(i+1).index<<" "<<isDiagonal(poly.at(i),poly.at(i+1))<<endl;
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
    stack<unsigned> s;

    cout << "\nPush segment" << eventQ[0].index;
    cout << "\nPush segment" << eventQ[1].index;
    s.push(eventQ[0].index);
    s.push(eventQ[1].index);
    frontQ = 2;

    while (frontQ != sizeOfPolygon) {
        cout<<"In while"<<endl;
//        int val = right(poly[s.top() - 1], poly[s.top() - 2])^
//                  right(poly[eventQ[frontQ].index - 1], poly[eventQ[frontQ].index - 2]);
//        int val;

        int val = searchLeftOrRight(s.top(),eventQ[frontQ].index);
        cout<<"val "<<val<<endl;
//        int val = 1;
        if (val == 1) {
            while (!s.empty()) {
                int ind = s.top();
                int qind = eventQ[frontQ].index, prev1, next1;
                if (qind == 0) prev1 = poly.size();
                else prev1 = qind - 1;
                if (qind == poly.size()) next1 = 0;
                else next1 = qind + 1;
//                if (ind != next1 && ind != prev1)
                {
                    cout << "\nDifferent chain: Insert diagonal between " << s.top() << " and " << eventQ[frontQ].index;
                    diagonals.emplace_back(make_pair(s.top(),eventQ[frontQ].index));
                }
                cout << "\nPop out segment" << s.top();
                s.pop();
            }
            cout << "\nPush segment" << eventQ[frontQ - 1].index;
            s.push(eventQ[frontQ - 1].index);
            cout << "\nPush segment" << eventQ[frontQ].index;
            s.push(eventQ[frontQ].index);
        }
        else
        {
            int ch=1;
            int pind;
            while(ch==1){
                int ind=s.top();
                int c=eventQ[frontQ].index;
                if(Diagonal(poly[ind-1],poly[c-1]))
//                if( CGAL::orientation(eventQ[frontQ].P,poly[c-1].P,poly[ind-1].P) == CGAL::LEFT_TURN )
                {
                    cout<<"Left turn checking"<<endl;
                    int qind=eventQ[frontQ].index,prev1,next1;
                    if(qind==0) prev1=poly.size();
                    else prev1=qind-1;
                    if(qind==poly.size())next1=0;
                    else next1=qind+1;
//                    if(ind!=next1 && ind!=prev1)
                    {
                        cout<<"\nSame chain: Insert diagonal between "<<s.top()<<" and "<<eventQ[frontQ].index;
                        diagonals.emplace_back(make_pair(s.top(),eventQ[frontQ].index));
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
    cout<<"\nPolygon is triangulated";
}
#endif //YMONOTONETRIANGULATION_TRIANGULATEYMONOTONE_H
