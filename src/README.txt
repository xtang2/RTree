                        CMPS 181- Project 03 README-Detail

# RTree
An RTree made for CMPS 181 (Fall 2014) at University of California, Santa Cruz.
In this RTree, in which the leaf level pages contain entries of the form < key, rid >, where the key is a rectangle and rid is the rectangle's label.  
The label represents a unique identifier of the data object in a database.  We create an RTree by inserting a series of rectangles, in the order received. 
We then need to answer a few queries:  
(1) nearest-neighbor queries:  given a point, you should return the closest rectangle.  
(2) range queries:  given a range (i.e., a rectangle) return all objects contained within this range. 

WE define the following 3 interfaces, and assume all of the points are integers and the label will be no more than 8 characters.
insert(label, x1, y1, x2, y2):   simply inserts into  RTree.
nearest_neighbor(x1, y1):  returns the nearest rectangle to the given point (x1, y1).
contains(x1, y1, x2, y2):  returns all of the rectangles contained within the given rectangle (x1, y1, x2, y2).

Each page, whether a leaf page or internal page, contain a minimum of 2 entries (m=2) and maximum of 4 entries (M=4).  
A page should split upon trying to insert a 5th entry

When splitting, we use a "linear" split function for simplicity.  A sort function which sorts all of the rectangles by their minimum x values. 
After the split, in the parent page we update the MBR pointing to the old leaf, and create an entry for the new MBR for the new leaf.  MBR contain all of the rectangles in its subtree. Splits may propagate up the tree, and splits should be performed the same. 

We also need a function

computeMBR( rectangles)
Where the input is a set of rectangles, and the output is the MBR of the input rectangles.  
The MBR should be computed as (minx, miny, maxx, maxy) of the input rectangles.

The nearest neighbor query should return the nearest rectangle to the given point, 
and the range query should return all rectangles contained within the range (MBR) given. 
