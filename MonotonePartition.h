//
// Created by FNU Shariful on 4/14/22.
//

#ifndef YMONOTONETRIANGULATION_MONOTONEPARTITION_H
#define YMONOTONETRIANGULATION_MONOTONEPARTITION_H


#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>

using namespace std;


typedef std::pair<int,int> Edge;
//typedef CGAL::Simple_cartesian<double> K;
//typedef K::Point_2 Point;

class Point1{
public:
    double x,y;
};


class Vertex{
public:
    int index;
    Point1 P;
    int type;
};

vector <Vertex> poly;
vector <Vertex> eventQ;
vector <pair <int,int> > Tree;
vector <int> helper(500);

vector <pair <int,int> > ans;


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
        verticesForQTPolygon.emplace_back( QPointF(p.P.x,p.P.y ) );

    canvas.drawPolygon(&verticesForQTPolygon[0], (int)verticesForQTPolygon.size());

    for ( Edge e : diagonals ) {
        cout<<e.first<<" "<<e.second<<endl;
        QPointF endPointA(poly.at(e.first).P.x,poly[e.first].P.y ),
                endPointB(polygonVertices[e.second].P.x,polygonVertices[e.second].P.y );
        canvas.drawLine( endPointA, endPointB );
    }

    int id = 0;
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
    l->showNormal();
//    l->hasScaledContents();
    QApplication::exec();
    // DO NOT TOUCH THESE Qt STATEMENTS!!!
    /***************************************************/
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

void handleStartVertex(Vertex cur){
    cout<<"\nSTART:"<<cur.index<<"\n\t";
//    cout<<Tree.size()<<endl;
//    for( auto x: Tree ) cout<<x.first<<" "<<x.second<<endl;
    cout<<" "<<cur.index-1<<endl;
    Tree.emplace_back(make_pair(cur.index,cur.P.x));
    cout<<" "<<cur.index-1<<endl;
    helper[cur.index-1]=cur.index;
    cout<<"Edge"<<cur.index<<" is inserted and helper("<<cur.index<<")="<<cur.index<<"\n";
}

void handleEndVertex(Vertex cur){
    cout<<"\nEND:"<<cur.index<<"\n\t";
    int prev=cur.index-2;
    if(cur.index-2==-1)
        prev=poly.size()-1;
    if(poly[helper[prev]-1].type==3){
        cout<<"Insert Diagonal between "<<cur.index<<" and "<<helper[prev]<<"\n";
//        plotLine(cur.P,poly[helper[prev]-1].P);
        ans.push_back(make_pair(cur.index,helper[prev]));
    }
    for(int i=0;i<Tree.size();i++){
        if(Tree[i].first==helper[prev]){
            cout<<"Delete"<<Tree[i].first<<"from the tree\n\t";
            Tree[i].first=-1;
            Tree.erase(Tree.begin()+i);
            break;
        }
    }
}

void handleMergeVertex(Vertex cur){
    cout<<"\nMERGE:"<<cur.index<<"\n\t";
    int prev=cur.index-2;
    if(cur.index-2==-1)
        prev=poly.size()-1;
    cout<<"Prev node:"<<prev+1<<"\n\t";
    if(poly[helper[prev]-1].type==3){
        cout<<"is a merge vertex.\n\tInsert Diagonal between "<<cur.index<<" and "<<helper[prev]<<"\n\t";
//        plotLine(cur.P,poly[helper[prev]-1].P);
        ans.push_back(make_pair(cur.index,helper[prev]));
    }
    for(int i=0;i<Tree.size();i++){
        if(Tree[i].first==helper[prev]){
            Tree[i].first=-1;
            Tree.erase(Tree.begin()+i);
            cout<<"Edge"<<prev+1<<"deleted from the tree"<<"\n\t";
            break;
        }
    }
    if(Tree.size()!=0){
        cout<<"Left neighbor:";
        Vertex left;
        if(Tree.size()==1){
            left=poly[Tree[0].first-1];
        }
        for(int i=0;i<Tree.size();i++){
            //cout<<"Iterating node:"<<Tree[i].second<<" "<<Tree[i].first;
            left=poly[Tree[i].first-1];
            if(Tree[i].second > cur.P.x){
                left=poly[Tree[i-1].first-1];
                //cout<<Tree[i-1].first;
                break;
            }
            left=poly[Tree[i].first-1];
            cout<<left.index<<"\n\t";
        }
        //cout<<":::";
        if(poly[helper[left.index-1]-1].type==3){
            cout<<"is a Merge vertex.\n\t Insert Diagonal between "<<cur.index<<" and "<<helper[left.index-1]<<"\n\t";
//            plotLine(cur.P,poly[helper[left.index-1]-1].P);
            ans.push_back(make_pair(cur.index,helper[left.index-1]));
        }
        helper[left.index-1]=cur.index;
        cout<<"Set helper("<<left.index<<")="<<cur.index<<"\n";
    }
}

void handleSplitVertex(Vertex cur){
    Vertex left;
    cout<<"\nSPLIT:"<<cur.index<<"\n\t";
    if(Tree.size()!=0){
        if(Tree.size()==1){
            left=poly[Tree[0].first-1];
        }
        for(int i=0;i<Tree.size();i++){
            if(Tree[i].second> cur.P.x){
                left=poly[Tree[i-1].first-1];
                break;
            }
            left=poly[Tree[i].first-1];
        }
        cout<<"Left neighbor:"<<left.index<<"\n\t";
        cout<<"Insert Diagonal between "<<cur.index<<" and "<<helper[left.index-1]<<"\n\t";
        ans.push_back(make_pair(cur.index,helper[left.index-1]));
//        plotLine(cur.P,poly[helper[left.index-1]-1].P);
        helper[left.index-1]=cur.index;
        cout<<"Set helper("<<left.index<<")="<<cur.index<<"\n\t";
    }
    Tree.push_back(make_pair(cur.index,cur.P.x));
    cout<<"Insert"<<cur.index<<" into the tree ans set helper("<<cur.index<<")="<<cur.index<<"\n";
    helper[cur.index-1]=cur.index;
}

bool right(Vertex cur,Vertex prev){
    if(cur.P.y < prev.P.y)
        return true;
    else if(cur.P.y==prev.P.y){
        if(cur.P.x < prev.P.x)
            return false;
    }
    return false;
}

void handleRegularVertex(Vertex cur){
    int prev=cur.index-2;
    cout<<"\nREGULAR:"<<cur.index<<"\n\t";
    if(right(cur,poly[cur.index-2])){
        cout<<"if(right) case:\n\t";
        if(cur.index-2==-1)
            prev=poly.size()-1;
        if(poly[helper[prev]-1].type==3){
            cout<<"Insert Diagonal between "<<cur.index<<" and "<<helper[prev]<<"\n\t";
//            plotLine(cur.P,poly[helper[prev]-1].P);
            ans.push_back(make_pair(cur.index,helper[prev]));
        }
        for(int i=0;i<Tree.size();i++){
            if(Tree[i].first==helper[prev]){
                cout<<"Delete"<<Tree[i].first<<"from the tree\n\t";
                Tree[i].first=-1;
                Tree.erase(Tree.begin()+i);
                break;
            }

        }
        Tree.push_back(make_pair(cur.index,cur.P.x));
        helper[cur.index-1]=cur.index;
        cout<<"Insert"<<cur.index<<" into the tree ans set helper("<<cur.index<<")="<<cur.index<<"\n";
    }
    else{
        cout<<"Else case:\n\t";
        Vertex left;
        if(Tree.size()!=0){
            if(Tree.size()==1){
                left=poly[Tree[0].first-1];
            }
            cout<<Tree.size()<<endl;
            for(int i=0;i<Tree.size();i++){
                if(Tree[i].second> cur.P.x){
                    left=poly[Tree[i-1].first-1];
                    break;
                }
                left=poly[Tree[i].first-1];
            }
            cout<<"Left neighbor:"<<left.index<<"\n\t";
            if(poly[helper[left.index-1]-1].type==3){
                cout<<"Insert Diagonal between "<<cur.index<<" and "<<helper[left.index-1]<<"\n\t";
//                plotLine(cur.P,poly[helper[left.index-1]-1].P);
                ans.push_back(make_pair(cur.index,helper[left.index-1]));
            }
            helper[left.index-1]=cur.index;
            cout<<"Set helper("<<left.index<<")="<<cur.index<<"\n\t";
        }
    }
}

bool angle(Point1 a, Point1 b,Point1 c){
    int area=(b.x-a.x)*(c.y-a.y)-(c.x-a.x)*(b.y-a.y);
    return area>=0;
}

void identifyVertexType(){
    for(int i=0;i<poly.size();i++){
        int prev=i-1,next=i+1;
        if(i-1 ==-1)
            prev=poly.size()-1;
        if(i==poly.size()-1)
            next=0;
        if((poly[prev].P.y < poly[i].P.y) && (poly[i].P.y > poly[next].P.y)){
            if(angle(poly[i].P,poly[next].P,poly[prev].P)){
                poly[i].type=1;
            }
            else{
                poly[i].type=4;
            }
        }
        else if((poly[prev].P.y > poly[i].P.y) && (poly[i].P.y < poly[next].P.y)){
            if(angle(poly[i].P,poly[next].P,poly[prev].P)){
                poly[i].type=5;
            }
            else{
                poly[i].type=3;
            }
        }
        else{
            poly[i].type=2;
        }
    }
}

void monotonePartition(std::vector<Point> &polygonVertice){
    vector<Point1> polygonVertices;
    for( auto xx: polygonVertice ) polygonVertices.push_back({xx.x(),xx.y()});
    vector<Edge> diagonals;
//    polygonVertices.push_back({391, 374});
//    polygonVertices.push_back({240, 431});
//    polygonVertices.push_back({252, 340});
//    polygonVertices.push_back({374, 320});
//    polygonVertices.push_back({289, 214});
//    polygonVertices.push_back({134, 390});
//    polygonVertices.push_back({ 68, 186});
//    polygonVertices.push_back({154, 259});
//    polygonVertices.push_back({161, 107});
//    polygonVertices.push_back({435, 90});
//    polygonVertices.push_back({208, 148});
//    polygonVertices.push_back({295, 160});
//    polygonVertices.push_back({421, 212});
//    polygonVertices.push_back({441, 303});


    for (int i = 0; i <polygonVertices.size(); i++) {
        Vertex v;
        v.P = polygonVertices.at(i);
        v.index = i;
        poly.emplace_back(v);
//        if( i >= 1 and i<= 5 ) leftChain.emplace_back(v);
//        else rightChain.emplace_back(v);
    }
    int n=poly.size();
    identifyVertexType();
    eventQ=poly;
    sort(eventQ.begin(),eventQ.end(),comp);
    cout<<"Event queue:\n";
    cout<<"Xcor Ycor index type\n";
    for(int i=0;i<n;i++){
        cout<<eventQ[i].P.x<<" "<<eventQ[i].P.y<<" "<<eventQ[i].index<<"   "<<eventQ[i].type<<"\n\t";
    }
    int frontQ=0;
    while(frontQ!=n){
        Vertex current=eventQ[frontQ];
        if(current.type==1){
            //cout<<"start";
            handleStartVertex(current);
        }
        else if(current.type==2){
            //cout<<"reg";
            handleRegularVertex(current);
        }
        else if(current.type==3){
            //cout<<"merge";
            handleMergeVertex(current);
        }

        else if(current.type==4){
            //cout<<"split";
            handleSplitVertex(current);
        }
        else if(current.type==5){
            //cout<<"end";
            handleEndVertex(current);
        }
        sort(Tree.begin(),Tree.end(),compx);
        cout<<"\nTree structure at iteration "<<frontQ+1<<":\n\t";
        for(int i=0;i<Tree.size();i++){
            cout<<Tree[i].first<<" ";
        }
        frontQ++;
    }
    cout<<"\nEnd of algorithm. Partitioned into monotone pieces\n\n";
    cout<<"No of diagonals inserted:"<<ans.size();
    cout<<"\n The diagonals are inserted between:\n";
    for(int i=0;i<ans.size();i++){
        cout<<ans[i].first<<"\t"<<ans[i].second<<"\n";
        diagonals.emplace_back(make_pair(ans[i].first,ans[i].second));
    }
    drawPolygonUsingQT(poly,diagonals,true);
}


#endif //YMONOTONETRIANGULATION_MONOTONEPARTITION_H
