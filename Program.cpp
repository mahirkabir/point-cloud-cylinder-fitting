#include<stdio.h>
#include<limits.h>
#include<math.h>
#include<iostream>
#include<vector>
#include<sstream>
#include<fstream>

using namespace std;

#define PI 2 * acos(0.0)

struct node {
    double x;
    double y;
    double z;
    int red;
    int green;
    int blue;

    node() {
        x = y = z = 0.0;
        red = green = blue = 0;
    }

    node(double _x, double _y, double _z) {
        x = _x;
        y = _y;
        z = _z;
        red = green = blue = 0;
    }

    node(double _x, double _y, double _z, int _red, int _green, int _blue) {
        x = _x;
        y = _y;
        z = _z;
        red = _red;
        green = _green;
        blue = _blue;
    }

    string print() {
        char buffer[50];
        sprintf (buffer, "(%f %f %f)", this->x, this->y, this->z);
        return buffer;
    }
};

struct cylinder {
    node center;
    double height;
    double diameter;
    node surfaceCenter;

    cylinder(node _center, double _height, double _diameter, node _anotherPointOnCenterSurface) {
        center = _center;
        height = _height;
        diameter  = _diameter;
        surfaceCenter = _anotherPointOnCenterSurface;
    }
};

vector<node> nodes;

vector<double> Split(string str) {
    vector<double> ret;

    istringstream ss(str);
    string token;

    while(ss >> token) {
        double val;
        sscanf(token.c_str(), "%lf", &val);
        ret.push_back(val);
    }

    return ret;
}

int doubleToInt(double d) {
    return (int)d;
}

node GetNode(string str) {
    int n = str.length();

    vector<double> props = Split(str);

    return (props.size() == 6)
        ? node(props[0], props[1], props[2], doubleToInt(props[3]), doubleToInt(props[4]), doubleToInt(props[5]))
        : node(0.0, 0.0, 0.0, 0, 0, 0);
}

// Idea: First create a bounding box then wrap up that box within a cylinder
cylinder GetBoundingCylinder(vector<node> nodes) {
    node cornerMax(INT_MIN, INT_MIN, INT_MIN);
    node cornerMin(INT_MAX, INT_MAX, INT_MAX);

    int n = nodes.size();

    for(int i = 0; i < n; ++i) {
        cornerMax = node(max(cornerMax.x, nodes[i].x), max(cornerMax.y, nodes[i].y), max(cornerMax.z, nodes[i].z));
        cornerMin = node(min(cornerMin.x, nodes[i].x), min(cornerMin.y, nodes[i].y), min(cornerMin.z, nodes[i].z));
    }

    //Corner points of bounding box
    node a (cornerMin.x, cornerMin.y, cornerMin.z);
    node b (cornerMin.x, cornerMin.y, cornerMax.z);
    node c (cornerMax.x, cornerMin.y, cornerMax.z);
    node d (cornerMax.x, cornerMin.y, cornerMin.z);
    node e (cornerMax.x, cornerMax.y, cornerMax.z);
    node f (cornerMin.x, cornerMax.y, cornerMax.z);
    node g (cornerMin.x, cornerMax.y, cornerMin.z);
    node h (cornerMax.x, cornerMax.y, cornerMin.z);

    // Cylinder axis line going through centerPointOfCylinder and centerOfAPlane point
    node centerPointOfCylinder ((cornerMax.x + cornerMin.x) / 2, (cornerMax.y + cornerMin.y) / 2, (cornerMax.z + cornerMin.z) / 2);
    node centerPointOfAPlane ((e.x + g.x) / 2, (e.y + g.y) / 2, (e.z + g.z) / 2);

    //node centerPointOfAPlane2 ((a.x + c.x) / 2, (a.y + c.y) / 2, (a.z + c.z) / 2);
    double diameter = max(sqrt((a.x - c.x)*(a.x - c.x) + (a.y - c.y)*(a.y - c.y) + (a.z - c.z)*(a.z - c.z)),
                          sqrt((b.x - d.x)*(b.x - d.x) + (b.y - d.y)*(b.y - d.y) + (b.z - d.z)*(b.z - d.z))); // take max diagonals form plane

    double height = fabs(cornerMax.y - cornerMin.y);

    return cylinder(centerPointOfCylinder, height, diameter, centerPointOfAPlane);
}

int main() {
    ifstream fin("cmesh0.ply");
    ofstream fout("output.txt");

    string str;

    bool header = true;

    while(getline(fin, str)) {
        if(header) {
            if(str == "end_header") {
                header = false;
            }

        } else {
            nodes.push_back(GetNode(str));
        }
    }

    int n = nodes.size();

    cylinder ans = GetBoundingCylinder(nodes);

    fout << "Cylinder center: " << ans.center.print() << endl;
    fout << "Cylinder height: " << ans.height << endl;
    fout << "Center of a surface: " << ans.surfaceCenter.print() << endl;
}
