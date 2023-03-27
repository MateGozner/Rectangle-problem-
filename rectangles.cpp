#include <iostream>
#include <unordered_map>
#include <vector>
#include <set>
#include <fstream>
using namespace std;
/*
    This program reads a list of (x, y) points from a file and counts the number of
    rectangles that can be formed from these points. Each rectangle must have four
    distinct vertices with coordinates (x1, y1), (x1, y2), (x2, y1), and (x2, y2)
    where x1 < x2 and y1 < y2.
*/

/*
    Parses input file with given filename and returns a vector of pairs representing
    the points read from the file.Input format:(x1,y1), (x2,y2), ..., (xn,yn)
*/
vector<pair<int,int>> parse_input(string filename) {
    vector<pair<int,int>> points;
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error: failed to open input file " << filename << endl;
        return points;
    }
    if (infile.good()){
        string line;
        getline(infile, line);
         size_t pos = 0;
        while (pos < line.length()){
            // Find next pair of coordinates in the line
            size_t start = line.find('(', pos);
            size_t end = line.find(')', pos);
            if (start == string::npos || end == string::npos) {
                break;  // No more pairs found
            }
            // Extract x and y values from the pair
            string pair_str = line.substr(start+1, end-start-1);
            size_t comma = pair_str.find(',');
            if (comma == string::npos) {
                break;  // Invalid format
            }
            int x = stoi(pair_str.substr(0, comma));
            int y = stoi(pair_str.substr(comma+1));
            // Add pair to the list of points
            points.emplace_back(x, y);
            // Move on to the next pair in the line
            pos = end + 1;
        }
        infile.close();
    }
    infile.close();
    return points;
}

/*
    Counts the number of rectangles that can be formed from the given list of points
    using a set data structure to store the points.
    Returns the number of rectangles.
    The function uses a set data structure to store the points because sets have O(log n) insert and search operations, which are efficient for small input sizes.
    The function increments the count of rectangles by 1/2 to avoid counting the same rectangle twice since each rectangle can be formed using two pairs of points.
    The function uses a nested loop to iterate through all possible pairs of points, which takes O(n^2) time. However, the continue statement skips some iterations, which reduces the actual running time in practice.
*/

int count_rectangles2(const vector<pair<int, int>>& points) {
    int n = points.size();
    set<pair<int,int>> hash; // Ordered set of points
    int num_rectangle = 0;
    for (auto point:points){
        hash.insert(point); // Add point to set
    }
    for (int i = 0; i < n; i++){
        int x=points[i].first;
        int y=points[i].second;
        for (int j = i+1; j < n; j++){
            int xx=points[j].first;
            int yy=points[j].second;
            if (x == xx || y == yy) {
                continue; // Skip if points share same x or y value
            }
            if (hash.count({x,yy}) > 0 &&
                hash.count({xx,yy}) != 0) {
                num_rectangle++; // Found a rectangle
            }
        }
    }
    return num_rectangle/2; // divide the number of rectangles by 2 because each rectangle is counted twice
}
/*
    This function is similar in logic to the count_rectangles2 function using a set, but uses two unordered maps instead.
    Both implementations have a nested for loop that iterates through all pairs of points, which gives a time complexity of O(n^2).
    However, the time complexity of inserting and accessing elements in a set is O(log n) while it is O(1) for unordered maps.
    Therefore, the count_rectangles2 function may have a slower performance for large inputs due to the use of sets.
*/
int count_rectangles(const vector<pair<int, int>>& points) {
    int n = points.size();
    unordered_map<int, set<int>> xmap, ymap; // create unordered maps to store points by their x and y values
    int num_rectangle = 0; // initialize the number of rectangles found to 0
    for (int i = 0; i < n; i++){
        int x=points[i].first;
        int y=points[i].second;
        xmap[x].insert(y); // insert the y value of the point into the xmap under the key of its x value
        ymap[y].insert(x); // insert the x value of the point into the ymap under the key of its y value
    }
    for (int i = 0; i < n; i++){
        int x=points[i].first;
        int y=points[i].second;
        for (int j = i+1; j < n; j++){
            int xx=points[j].first;
            int yy=points[j].second;
            if (x == xx || y == yy) { // skip points with the same x or y value
                continue;
            }
            if (xmap[x].count(yy) > 0 && // check if there is a point with the same x value and yy y value
                ymap[yy].count(x) > 0) { // check if there is a point with the same y value and x xalue
                num_rectangle++; // increment the number of rectangles found
            }
        }
    }
    return num_rectangle/2; // divide the number of rectangles by 2 because each rectangle is counted twice
}

void write_output(string filename, int num_rectangles) {
    ofstream outfile(filename);
    if (outfile.good()){
        outfile << num_rectangles << endl; // Write number of rectangles to file
    }
    outfile.close();
}

int main()
{
    auto points=parse_input("input.txt");
    //cout<<count_rectangles2(points);
    auto num_rectangles=count_rectangles(points);
    write_output("output.txt",num_rectangles);
    return 0;
}
