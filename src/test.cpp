#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include "rtree.h"

int main(){
    
    //rectangle rect;
    //a page pointer point to the root
   
    RTree r;
r.insert("R1",50,40,55,45);
    r.insert("R2",55,20,75,25);
    r.insert("R3",20,50,40,60);
    r.insert("R4",30,75,40,85);
   r.insert("R5",50,40,55,45);





    r.display();

cout<<"contains:\n";
r.contains(20,19,57,88);
cout<<"nearest:\n";
r.nearest_neighbor(31,31);

     

    return 0;
}
