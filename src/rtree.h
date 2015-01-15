#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <utility>
#include <stack>
using namespace std;

struct Page;
struct Entry;
typedef struct Rectangle
{
	int x1,y1,x2,y2;
	string label;
	Rectangle(string lab, int x_1, int y_1, int x_2, int y_2){ // rectangle constructor
        label = lab;
        x1 = x_1;
        y1 = y_1;
        x2 = x_2;
        y2 = y_2;
	};
	Rectangle(){ label = ""; };
}Rectangle;

typedef struct Entry // 1 data entry in a page
{
	Rectangle rect; // rectangle
	Page *ptr; // pointer to subtree page
	Entry(){ rect.label = "";};
}Entry;

typedef struct Page
{
	Entry entries[4]; // the subtrees of the intermediate node
	bool isLeaf; // is the page a leaf or not?
	Page(){ isLeaf = false;};
}Page;

//Page newPage = new Page;
//Page leaf = new Page;//create a leaf page


typedef struct Pair{
    Page *vP;
    int index;
}Pair;



class RTree{
    
public:
    
   
    Page* root;//pointer to the root of the tree
    bool TreeisEmpty;
    
    
    //Pair visited;
    stack<Pair> mystack;
    
     std::ostringstream mbrstr;
    
   

    RTree();
    //simply inserts into your RTree page.
    void insert(string label, int x1, int y1, int x2, int y2 );
    
    // returns the nearest rectangle to the given point (x1, y1).
    void nearest_neighbor(int x1, int y1);
    
    // returns all of the rectangles contained within the given rectangle (x1, y1, x2, y2)
    void contains(int x1, int y1, int x2, int y2);
    
    //compute the MBR of a page
    Rectangle computeMBR1(Entry *ent);
    //compute the MBR of two rectangle
    Rectangle computeMBR2(Rectangle A, Rectangle B);
    
    int computeArea(int x1, int y1, int x2, int y2);
    //find the area dfference between R1 and R2
    int Difference(Rectangle R1, Rectangle R2);
    //display the tree
    void display();
    //insert
    int doInsert(Page *p, Rectangle Rect, Page *ptr);

    Rectangle* linearSort(Rectangle *rectangles);
    
    //check if the page is full
    bool isFull(Page *p);
    
    //split if a page is full
    void Split(Page *p, Rectangle newNode);
  
   
    //RTree &operator= (const RTree&);
  
    
};


//simply inserts into your RTree page.
//void insert(string label, float x1, float y1, float x2, float y2 );

// returns the nearest rectangle to the given point (x1, y1).
//void nearest_neighbor(float x1, float y1);

// returns all of the rectangles contained within the given rectangle (x1, y1, x2, y2)
//void contains(float x1, float y1, float x2, float y2);
//void Initial();

//void display();//display the entries in the rtree

// compute the mbr of the set of rectangles
//rectangle computeMBR(rectangle *rectangles);
