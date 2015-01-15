#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include "rtree.h"
#include <math.h>

#include <queue>



// compute the mbr of the set of rectangles
Rectangle RTree::computeMBR1(Entry *ent){
    Rectangle r;
    
    int cmbr[4]={ent->ptr->entries[0].rect.x1,ent->ptr->entries[0].rect.y1,0,0};
    
    for(int i=0;i<4;i++)
    {
        if ( ent->ptr->entries[i].rect.x1 < cmbr[0] && ent->ptr->entries[i].rect.x1 !=0) cmbr[0] = ent->ptr->entries[i].rect.x1;
        if ( ent->ptr->entries[i].rect.y1 < cmbr[1] && ent->ptr->entries[i].rect.y1 !=0) cmbr[1] = ent->ptr->entries[i].rect.y1;
        if ( ent->ptr->entries[i].rect.x2 > cmbr[2] ) cmbr[2] = ent->ptr->entries[i].rect.x2;
        if ( ent->ptr->entries[i].rect.y2 > cmbr[3] ) cmbr[3] = ent->ptr->entries[i].rect.y2;
    }
    
    r.x1 = cmbr[0];
    r.y1 = cmbr[1];
    r.x2 = cmbr[2];
    r.y2 = cmbr[3];
    r.label = ent->ptr->entries[0].rect.label;
    
    return r;
}

//compute the MBR of two rectangle
Rectangle RTree::computeMBR2(Rectangle A, Rectangle B){
    Rectangle r;
    
    r.x1 = A.x1;
    r.y1 = A.y1;
    r.x2 = A.x2;
    r.y2 = A.y2;
    
    if(A.x1>B.x1){ r.x1=B.x1;}
    if(A.x2<B.x2){ r.x2=B.x2;}
    if(A.y1>B.y1){ r.y1=B.y1;}
    if(A.y2<B.y2){ r.y2=B.y2;}
    
    return r;
}

//initialize the rtree

RTree::RTree(){
    
    root = new Page;
    root->entries[0].ptr=new Page;//point to a leaf page
    root->entries[0].ptr->isLeaf = true;
    bool TreeisEmpty = true;
    
}




//insert
void RTree::insert(string label, int x1, int y1, int x2, int y2){
    
    Rectangle R = Rectangle(label,x1,y1,x2,y2);//create the rectangle
    cout<<"insert "<<R.label<<" \n";
    if(TreeisEmpty){//insert the first node
        root->entries[0].rect = R;//MBR=R1
        root->entries[0].rect.label = mbrstr.str();
        TreeisEmpty = false;
    }
    
    //*p=value pointed to by p, or value stored in the adrress of P
    //root point to the address of the root page
    
    Page *p;
    p = root;//a new pointer that initially point to the root
    
    int Diff[4];
    //check if the page is leaf
    
    while(p->isLeaf==false){
        cout<<"is not leaf"<< " \n";
        //if page is not leaf
        
        
        int Diff[4] = { 0 };
        int stop;
        Rectangle nRect, oRect;
        //check MBR of all entries
        for(int i =0; i<4; i++){
            
            //compute the MBR with the new rectangle
            //compute the increase in area if we were gonna insert it here
            //area of newMbr - area of oldMbr
            if (!p->entries[i].rect.label.empty()){
                nRect = computeMBR2(p->entries[i].rect, R);
                oRect = p->entries[i].rect;
                Diff[i]=Difference(nRect,oRect);
                cout<<"Diff["<<i<<"] is "<<Diff[i]<<"\n";
                stop = i;
            }
        }
        
        //find the one that will increase the least in area
        int curr = Diff[0];
        int index = 0;
        
        for(int i=0;i<=stop;i++){
            if(Diff[i]<curr){
                cout<<"Diff["<<i<<"] is selected!  \n";
                curr = Diff[i];
                
                index = i;
                cout<< index << " \n";
            }
        }
        
        Pair visited;
        visited.vP = p;
        visited.index=index;
        cout<<"we are going down from the "<<index << "entry"<<" \n";
        
        mystack.push(visited);
        //travel down the tree
        p=p->entries[index].ptr;
        //keep going down untill we find leave page
        
    }
    
    
    cout<<"is leaf"<< " \n";
    //p is pointing to the leaf node now
    doInsert(p, R, NULL);
    
}


//insert the rectangle into the page pointer
int RTree::doInsert(Page *p, Rectangle Rect, Page *ptr){
    
    Rectangle r;
    if(!isFull(p)){
        cout<<"this page is not full"<< " \n";
        
        for (int j=0;j<4;j++){
            if (p->entries[j].rect.label.empty()){
                // = Rect;
                p->entries[j].rect = Rect;
                p->entries[j].ptr = ptr;
                mbrstr.str("");mbrstr.clear();
                mbrstr<<"MBR"<<j+1;
                
                if(p->isLeaf==false)
                    p->entries[j].rect.label = mbrstr.str();
                
                cout<<"inserted "<<p->entries[j].rect.label<<" \n";
                
                break;
                
            }
        }
        
        
        ///while there is more level
        //pop the parent page and the entry
        if(!mystack.empty()){
            cout<<"stack is not empty"<< " \n";
            int entryIndex = mystack.top().index;
            cout<< "entryIndex: "<< entryIndex << " \n";
            //compute the new mbr
            r = computeMBR1(&mystack.top().vP->entries[entryIndex]);
            //recompte MBR of the parent
            mystack.top().vP->entries[entryIndex].rect=r;
            
            mbrstr.str("");mbrstr.clear();
            mbrstr<<"MBR"<<entryIndex+1;
            mystack.top().vP->entries[entryIndex].rect.label=mbrstr.str();
            mystack.pop();
            
        }
        
    }else{ // if stack is not empty
        cout<<"this page is full"<< " \n";
        Split(p,Rect);
    }
}


//check if the page is full
bool RTree::isFull(Page *p){
    for (int i=0;i<4;i++){
        if ( p->entries[i].rect.label.empty()){
            return false;
        }
        
    }
    return true;
    
}

void RTree::Split(Page *p, Rectangle newNode){
    
    
    cout<<"start split "<< " \n";
    //generate new page;
    
    Page* pageN = new Page;
    pageN->isLeaf = true;
    
    Rectangle rect[5];
    
    for (int i=0;i<4;i++){
        rect[i]=p->entries[i].rect;
    }
    rect[4]=newNode;
    // cout<<"end of array "<< " \n"; // debug statement
    
    //sort the rectangles
    Rectangle *sorted;
    sorted = linearSort(rect);
    
    //clear the old page
    for (int i=0;i<4;i++){
        p->entries[i].rect.label= "";
    }
    
    //redistribute
    p->entries[0].rect=sorted[0];
    p->entries[1].rect=sorted[1];
    pageN->entries[0].rect=sorted[2];
    pageN->entries[1].rect=sorted[3];
    pageN->entries[2].rect=sorted[4];
    
    int entryIndex = mystack.top().index;
    //compute the new MBR for the old page and update the parent
    mystack.top().vP->entries[entryIndex].rect= computeMBR1(&mystack.top().vP->entries[entryIndex]);
    //this only update one upper level
    //don't pop page,
    //need to wait for the newMBR to be inserted into the page
    //then update the MBRs for the upper levels if necessary
    
    Rectangle tmp;
    tmp = computeMBR2(sorted[2], computeMBR2(sorted[3], sorted[4]));
    tmp.label = mbrstr.str();
    
    cout<<"computeMBR for new Page = "<<tmp.label<< " \n";
    
    //insert into the new page
    if(!isFull(mystack.top().vP)){//if the MBR page is not full
        
        doInsert(mystack.top().vP,tmp,pageN);
        
        
    }else{//split the mbr page
        cout<<"the mbr page is full \n";
        Page* newRoot = new Page;//create a root page
        
        newRoot->entries[0].ptr=mystack.top().vP;//connect it with the old page
        newRoot->entries[0].rect=computeMBR1(&newRoot->entries[0]);
        newRoot->entries[0].rect.label= "MBR5";
        
        mystack.pop();
        
        root = newRoot;//root is now point to the new root/
        
        Pair visited;
        visited.vP = newRoot;
        visited.index=0;
        //insert the new page so when it split it split the mbr page now
        mystack.push(visited);
        doInsert(newRoot,tmp,pageN);
        
    }
    
    
}


//find the increase in area
int RTree::Difference(Rectangle newMBR, Rectangle oldMBR){
    int newArea = computeArea(newMBR.x1, newMBR.y1, newMBR.x2, newMBR.y2);
    int oldArea = computeArea(oldMBR.x1, oldMBR.y1, oldMBR.x2, oldMBR.y2);
    return newArea-oldArea;
}

//compute the area of the given points
int RTree::computeArea(int x1, int y1, int x2, int y2){
    return (x2-x1)*(y2-y1);
}

// simple linear sorting algorithm
Rectangle* RTree::linearSort(Rectangle *rectangles){
    Rectangle temp;
    cout<<"start sort "<< " \n";
    for(int i=0;i<5;i++){
        for(int j=i+1;j<5;j++){
            if (rectangles[i].x1 >= rectangles[j].x1){
                temp = rectangles[i];
                rectangles[i] = rectangles[j];
                rectangles[j] = temp;
            }
        }
    }
    return rectangles;
}



// function to display
void RTree::display()
{
    
    queue<Page*> pgs;
    Page *temproot = root;
    pgs.push(temproot);
    
    while(!pgs.empty())
    {
        
        pgs.pop();
        
        for (int i=0;i<4;i++)
        {
            if (temproot->entries[i].rect.label != "" )
            {
                cout<<"MBR : "<<temproot->entries[i].rect.label<<" LOC : ";
                cout<<temproot->entries[i].rect.x1<<" ";
                cout<<temproot->entries[i].rect.y1<<" ";
                cout<<temproot->entries[i].rect.x2<<" ";
                cout<<temproot->entries[i].rect.y2<<" ";
                cout<<"LEAF : "<<temproot->isLeaf<<"\n";
                if (temproot->entries[i].ptr->isLeaf==false) pgs.push(temproot->entries[i].ptr);
                for (int j=0;j<4;j++)
                {
                    if ( temproot->entries[i].ptr->entries[j].rect.label != "" /*&& temproot->entries[i].ptr->isLeaf == true*/)
                    {
                        
                        
                        
                        
                        cout<<"LABEL : "<<temproot->entries[i].ptr->entries[j].rect.label;
                        cout<<" LOC: "<<temproot->entries[i].ptr->entries[j].rect.x1;
                        cout<<" "<<temproot->entries[i].ptr->entries[j].rect.y1;
                        cout<<" "<<temproot->entries[i].ptr->entries[j].rect.x2;
                        cout<<" "<<temproot->entries[i].ptr->entries[j].rect.y2<<" ";
                        cout<<"LEAF : "<<temproot->entries[i].ptr->isLeaf<<"\n";
                    }
                }
                
            }
        }
        if (!pgs.empty()) temproot = pgs.front();
        
    }
    
}


void RTree::contains(int x1, int y1, int x2, int y2)
{
    for (int i=0;i<4;i++)
    {
        if (!root->entries[i].rect.label.empty() ){
            
            for (int j=0;j<4;j++)
            {// is the rectangle within the given coordinate boundaries?
                if (
                    !root->entries[i].ptr->entries[j].rect.label.empty() &&
                    x1 <= root->entries[i].ptr->entries[j].rect.x1 &&
                    y1 <= root->entries[i].ptr->entries[j].rect.y1 &&
                    x2 >= root->entries[i].ptr->entries[j].rect.x2 &&
                    y2 >= root->entries[i].ptr->entries[j].rect.y2
                    )
                {
                    cout<<"LABEL : "<<root->entries[i].ptr->entries[j].rect.label;
                    cout<<" LOC: "<<root->entries[i].ptr->entries[j].rect.x1;
                    cout<<" "<<root->entries[i].ptr->entries[j].rect.y1;
                    cout<<" "<<root->entries[i].ptr->entries[j].rect.x2;
                    cout<<" "<<root->entries[i].ptr->entries[j].rect.y2<<"\n";
                }
            }
            
        }
    }
    
    
}


// returns the nearest rectangle to the given point (x1, y1).
void RTree::nearest_neighbor(int x, int y)
{
    
    int dist;
    bool first=true; // To start distance comparion from first
    int d[3]={0,0,0}; // Keep track of distance and index
    
    for (int i=0;i<4;i++)
    {
        if (root->entries[i].rect.label != "" ){
            
            for (int j=0;j<4;j++)
            {// check if not empty label
                if ( root->entries[i].ptr->entries[j].rect.label != "" )
                {// cases as mentioned in lab report for different corners around the rectangle
                    
                    if ( x <= root->entries[i].ptr->entries[j].rect.x1 &&
                        y >= root->entries[i].ptr->entries[j].rect.y2 )
                        dist = sqrt(pow(x - root->entries[i].ptr->entries[j].rect.x1,2)+pow(y - root->entries[i].ptr->entries[j].rect.y2,2));
                    else if ( x >= root->entries[i].ptr->entries[j].rect.x2 &&
                             y >= root->entries[i].ptr->entries[j].rect.y2 )
                        dist = sqrt(pow(x - root->entries[i].ptr->entries[j].rect.x2,2)+pow(y - root->entries[i].ptr->entries[j].rect.y2,2));
                    else if ( x >= root->entries[i].ptr->entries[j].rect.x2 &&
                             y <= root->entries[i].ptr->entries[j].rect.y1 )
                        dist = sqrt(pow(x - root->entries[i].ptr->entries[j].rect.x2,2)+pow(y - root->entries[i].ptr->entries[j].rect.y1,2));
                    else if ( x <= root->entries[i].ptr->entries[j].rect.x1 &&
                             y <= root->entries[i].ptr->entries[j].rect.y1 )
                        dist = sqrt(pow(x - root->entries[i].ptr->entries[j].rect.x1,2)+pow(y - root->entries[i].ptr->entries[j].rect.y1,2));
                    else if ( y > root->entries[i].ptr->entries[j].rect.y2 ) dist = y - root->entries[i].ptr->entries[j].rect.y2;
                    else if ( x > root->entries[i].ptr->entries[j].rect.x2 ) dist = x - root->entries[i].ptr->entries[j].rect.x2;
                    else if ( y < root->entries[i].ptr->entries[j].rect.y1 ) dist = root->entries[i].ptr->entries[j].rect.y1 - y;
                    else if ( x < root->entries[i].ptr->entries[j].rect.x1 ) dist = root->entries[i].ptr->entries[j].rect.x1 - x;
                    
                    
                    if (first) { d[0]=i;d[1]=j;d[2]=dist; first=false;}
                    else if (dist < d[2]) { d[0]=i;d[1]=j;d[2]=dist; }
                    
                }
            }
            
        }
    }
    
    if (!first) 
    {// debugging and test purposes
        
        cout<<"LABEL : "<<root->entries[d[0]].ptr->entries[d[1]].rect.label;
        cout<<" LOC: "<<root->entries[d[0]].ptr->entries[d[1]].rect.x1;
        cout<<" "<<root->entries[d[0]].ptr->entries[d[1]].rect.y1;
        cout<<" "<<root->entries[d[0]].ptr->entries[d[1]].rect.x2;
        cout<<" "<<root->entries[d[0]].ptr->entries[d[1]].rect.y2;
        cout<<" DIST : "<<d[2]<<"\n";
        
    }
    
    
}
