#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Partition_traits_2.h>
#include <CGAL/partition_2.h>
#include <cassert>
#include <list>

#include <vector>
#include <unordered_set>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/random_convex_set_2.h>
#include <CGAL/Timer.h>
#include <CGAL/random_polygon_2.h>
#include <CGAL/Polygon_2.h>

#include <QtWidgets>
#include <QApplication>
#include <QLabel>
#include <QString>
#include <QTranslator>

//#include "TriangulateYMonotone.h"
//#include "Test.h"
//#include "TriangulateYMonotoneModified.h"
//#include "MonotonePartition.h"
#include "MonotonePartitionModified.h"

typedef CGAL::Simple_cartesian<double> K;
typedef K::Point_2 Point;
typedef std::list<Point>                         Container;
typedef CGAL::Polygon_2<K, Container>              Polygon;
//typedef std::pair<int,int> Edge1;  /// for "TriangulateYMonotoneModified.h"
//typedef std::pair<unsigned ,unsigned > Edge;
typedef std::pair<Vertex ,Vertex > Edge;

//typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Partition_traits_2<K>                         Traits;
//typedef Traits::Point_2                                     Point_2;
typedef Traits::Polygon_2                                   Polygon_2;
typedef std::list<Polygon_2>                                Polygon_list;


//void drawPolygonUsingQT(const std::vector<Point> &polygonVertices,
//                        const std::vector<QColor> &vertexColors,
//                        const std::vector<Edge> &diagonals, const bool labels) {
//    assert( !polygonVertices.empty() );
void drawPolygonUsingQT(const std::vector<Point> &polygonVertices,const std::vector<Edge> &diagonals,const bool labels) {

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
    for( Point p : polygonVertices )
        verticesForQTPolygon.emplace_back( QPointF(p.x(),p.y() ) );

    canvas.drawPolygon(&verticesForQTPolygon[0], (int)verticesForQTPolygon.size());

//    for ( Edge e : diagonals ) {
//        QPointF endPointA(polygonVertices[e.first].x(),polygonVertices[e.first].y() ),
//                endPointB(polygonVertices[e.second].x(),polygonVertices[e.second].y() );
//        canvas.drawLine( endPointA, endPointB );
//    }


    unsigned id = 1;
    for ( Point p : polygonVertices ) {

//        if( vertexColors[id] == Qt::red) {
//            canvas.setBrush(Qt::red); canvas.setPen(Qt::red);
//        }
//        else if( vertexColors[id] == Qt::darkGreen) {
//            canvas.setBrush(Qt::darkGreen); canvas.setPen(Qt::darkGreen);
//        }
//        else {
//            canvas.setBrush(Qt::blue); canvas.setPen(Qt::blue);
//        }

        canvas.drawEllipse(QPointF(p.x(), p.y()), pointSize, pointSize);
        if(labels) {
            canvas.setBrush(Qt::black);
            canvas.setPen(Qt::black);
            QPointF textPos(p.x() + 4.0, p.y() + 4.0);
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


void make_polygon(Polygon_2& polygon)
{
    polygon.push_back(Point(391, 374));
    polygon.push_back(Point(240, 431));
    polygon.push_back(Point(252, 340));
    polygon.push_back(Point(374, 320));
//    polygon.push_back(Point(289, 214));
//    polygon.push_back(Point(134, 390));
//    polygon.push_back(Point( 68, 186));
    polygon.push_back(Point(154, 259));
    polygon.push_back(Point(161, 107));
    polygon.push_back(Point(435, 90));
    polygon.push_back(Point(208, 148));
    polygon.push_back(Point(295, 160));
    polygon.push_back(Point(421, 212));
    polygon.push_back(Point(441, 303));

//     polygon.push_back(Point(0+100,0+100));
//     polygon.push_back(Point(2+100,0+100));
//     polygon.push_back(Point(2+100,2+100));
//     polygon.push_back(Point(1+100,1+100));
//     polygon.push_back(Point(0+100,2+100));


//       polygon.push_back(Point(6.3,150.5));
//    polygon.push_back(Point(-34.08,134.58));
//    polygon.push_back(Point(64.31,71.45));
//    polygon.push_back(Point(60.33,24.81));
//    polygon.push_back(Point(150.19,33.91));
//    polygon.push_back(Point(50.09,115.24));
//    polygon.push_back(Point(81.37,162.44));
//    polygon.push_back(Point(50,200));

/// for y monotone conversion

//    polygon.push_back(Point(1288.84, 1090.99));
//    polygon.push_back(Point(1127.68, 894.32));
//    polygon.push_back(Point(854.53, 1025.44));
//    polygon.push_back(Point(750.73, 951.6));
//    polygon.push_back(Point(909.16, 847.89));
//    polygon.push_back(Point(881.84, 686));
//    polygon.push_back(Point(669.06, 767.04));
//    polygon.push_back(Point(807.27, 508.63));
//    polygon.push_back(Point(1041.64, 391.45));
//    polygon.push_back(Point(1269.99, 535.58));
//    polygon.push_back(Point(1567.45, 346.38));
//    polygon.push_back(Point(1465.29,667.88));
//    polygon.push_back(Point(1741.72, 517.65));
//    polygon.push_back(Point(1739.54, 703.12));
//    polygon.push_back(Point(1482.78, 757.75));


    /// example from book
//    polygon.push_back(Point(1004.3,674.38));
//    polygon.push_back(Point(855.29,592.61));
//    polygon.push_back(Point(811.67,825.21));
//    polygon.push_back(Point(651.75,756.16));
//    polygon.push_back(Point(522.74,839.75));
//    polygon.push_back(Point(384.63,716.18));
//    polygon.push_back(Point(475.49,623.5));
//    polygon.push_back(Point(426.43,523.55));
//    polygon.push_back(Point(324.67,590.70));
//    polygon.push_back(Point(250.16,387.26));
//    polygon.push_back(Point(384.67,296.4));
//    polygon.push_back(Point(622.69,380));
//    polygon.push_back(Point(858.92,172.84));
//    polygon.push_back(Point(768.06,450.87));
//    polygon.push_back(Point(958.87,396.35));


}

Polygon generatePolygon(const unsigned n) {
    Polygon P;
    typedef CGAL::Random_points_in_square_2<Point> pg;
    CGAL::random_polygon_2(n, std::back_inserter(P), pg(300));
    return P;
}

void getPolygonVertices(const Polygon &P, std::vector<Point> &vertices){
    for (auto vi = P.vertices_begin(); vi != P.vertices_end(); ++vi)
        vertices.emplace_back(*vi);
}

int main(int argc, char *argv[] )
{
    QApplication a(argc, argv);

    Polygon_2 polygon;

    Polygon_list partition_polys;
//    make_polygon(polygon);
//    int x,y,z;
//    for( int i=0; i<8; i++ )
//    {
//        cin>>x>>y;
//        polygon.push_back(Point(x,y));
//    }

    unsigned n = 10;
    polygon = generatePolygon(n);

//    CGAL::y_monotone_partition_2(polygon.vertices_begin(),
//                                 polygon.vertices_end(),
//                                 std::back_inserter(partition_polys));


    std::vector<Point> polygonVertices;
    getPolygonVertices(polygon, polygonVertices);

//    std::sort(polygonVertices.begin(), polygonVertices.end(), [](const auto &v1, const auto &v2) {
//        if (v1.y() == v2.y())
//            return v1.x() < v2.x();
//        else
//            return v1.y() > v2.y();
//    });

//    Point topVertex = polygonVertices.at(0);
//    Point bottonVertex = polygonVertices.at(polygonVertices.size()-1);

//    std::sort(polygonVertices.begin(),polygonVertices.end(),[](const auto &v1, const auto &v2)
//              {
//                  if( v1.x() == v2.x() ) return v1.y() < v2.y();
//                  else
//                      return v1.x() < v2.x();
//              }
//    );



//    std::list<Polygon_2>::const_iterator   poly_it;
//    for (poly_it = partition_polys.begin(); poly_it != partition_polys.end(); poly_it++)
//    {
//        assert(CGAL::is_y_monotone_2((*poly_it).vertices_begin(),
//                                     (*poly_it).vertices_end()));
//    }

    std::vector<Edge> diagonals;
//    TriangulateYMonotoneModified(polygonVertices,diagonals);
//    TriangulateYMonotone(polygonVertices,diagonals);
//    drawPolygonUsingQT(polygonVertices,diagonals,true);
    monotonePartition(polygonVertices);

//    assert(CGAL::partition_is_valid_2(polygon.vertices_begin(),
//                                      polygon.vertices_end(),
//                                      partition_polys.begin(),
//                                      partition_polys.end()));
    return 0;
}