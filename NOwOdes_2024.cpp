//version : 2024.1014.0704

#pragma once

#include <iostream>
#include <iomanip>

#include <cstdlib>

#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <cstring>
#include <cctype>

#include <cmath>
#include <math.h>

#include <sys/types.h>
#include <unistd.h>

#include "FOwOFunctions_2024.cpp"

using namespace std;

template <typename TYPE>
class NOwOdes_Node
{
    public:

    TYPE content;
    vector<NOwOdes_Node<TYPE>*> pointers; //a vector of pointers to other nodes
    int Flag;

    NOwOdes_Node(TYPE InContent)
    {
        content = InContent;
        pointers = {};
        Flag = 0;
    }
};

template <typename TYPE>
class NOwOdes_List
{
    public:

    NOwOdes_Node<TYPE>* rootPointerL;
    NOwOdes_Node<TYPE>* rootPointerR;

    int size;

    TYPE popNullValue;

    FOwO fowo;

    //here we will
    //let NOwOdes_node.pointers[0] left
    //let NOwOdes_node.pointers[1] right
    //let rootL -> min = 0 = 0 = 0 = ... = 0 = 0 = 0 = max <- rootR


    NOwOdes_List()
    {
        rootPointerL = nullptr;
        rootPointerR = nullptr;
        size = 0;
    }



    void PrintNodesH(string InString)
    {
        NOwOdes_Node<TYPE>* Arrow = rootPointerL;
        int Counter = 0;

        cout << InString << " , len : " << size << " , content : rootL -> ";
        while(Arrow != nullptr)
        {
            cout << "[" << Counter << "]" << Arrow->content;
            Arrow = Arrow->pointers[1];
            if(Arrow != nullptr)
            {
                cout << " = ";
            }
            Counter++;

        }

        cout << " <- rootR" << endl;
    }

    void PrintNodesV(string InString)
    {
        NOwOdes_Node<TYPE>* Arrow = rootPointerL;
        int Counter = 0;

        cout << InString << endl;
        while(Arrow != nullptr)
        {
            cout << "[" << Counter << "] : " << Arrow->content;
            if(Counter == 0){cout << "<- RootL";}
            else if(Counter == size - 1){cout << "<- RootR";}
            cout << endl;

            Counter++;
            Arrow = Arrow->pointers[1];
        }
    }



    NOwOdes_Node<TYPE>* push(TYPE InContent, char InDirection)
    {
        NOwOdes_Node<TYPE>* NewArrow = new NOwOdes_Node<TYPE>(InContent);//create a new node out of nowhere
        NewArrow->pointers = {nullptr,nullptr};

        if (rootPointerL == nullptr && rootPointerR == nullptr)
        {
            rootPointerL = NewArrow;
            rootPointerR = NewArrow;
            size++;
        }
        else
        {
            if (InDirection == 'l')
            {
                //add it to the left side
                rootPointerL->pointers[0] = NewArrow;
                NewArrow->pointers[1] = rootPointerL;
                rootPointerL = NewArrow;
                size++;
            }
            else if (InDirection == 'r')
            {
                //add it to the right
                rootPointerR->pointers[1] = NewArrow;
                NewArrow->pointers[0] = rootPointerR;
                rootPointerR = NewArrow;
                size++;
            }
        }

        return NewArrow;
        
    }

    void push(NOwOdes_List<TYPE> &InList, char InStartReadDirection, char InPushDirection)
    {
        if(InStartReadDirection == 'l')//left to right
        {
            NOwOdes_Node<TYPE>* Arrow = InList.rootPointerL;

            while(Arrow != nullptr)
            {
                push(Arrow->content,InPushDirection);
                Arrow = Arrow->pointers[1];
            }
        }
        else if (InStartReadDirection == 'r')//right to left
        {
            NOwOdes_Node<TYPE>* Arrow = InList.rootPointerR;

            while(Arrow != nullptr)
            {
                push(Arrow->content,InPushDirection);
                Arrow = Arrow->pointers[0];
            }
        }
    }

    void insert_byOrder (TYPE inContent)
    {
        int Box_Int;
        float Box_Float;
        long Box_Long;

        vector<string> TypeNameList = {fowo.DatatypeOfVar(Box_Int) , fowo.DatatypeOfVar(Box_Float) , fowo.DatatypeOfVar(Box_Long)};
        string TypeName_inContent = fowo.DatatypeOfVar(inContent);

        if ( fowo.vectOwOr.FindObjPos(TypeNameList, TypeName_inContent, 'r') > -1 )
        {
            //this structure stores numerical values that can be compared

            if (rootPointerL == nullptr && rootPointerR == nullptr) //there are no nodes yet
            {
                push(inContent,'l');
            }
            else if (rootPointerL == rootPointerR) //there is one node
            {
                if (inContent <= rootPointerL->content)
                {
                    push(inContent,'l');
                }
                else
                {
                    push(inContent,'r');
                }
            }
            else //there are multiple nodes
            {
                NOwOdes_Node<TYPE>* ArrowN = new NOwOdes_Node<TYPE>(inContent);//create a new node out of nowhere
                ArrowN->pointers = {nullptr,nullptr};

                NOwOdes_Node<TYPE>* ArrowL = rootPointerL;
                NOwOdes_Node<TYPE>* ArrowR = rootPointerR;

                bool JobDone = false;

                
                if(inContent <= ArrowL->content)//if inCOntent is the new min
                {
                    push(inContent,'l');
                }
                else if (ArrowR->content <= inContent)//if inCOntent is the new max
                {
                    push(inContent,'r');
                }
                else//it should be somewhere in between
                {
                    while(JobDone == false)
                    {
                        //for ArrowL
                        if (ArrowL->content < inContent)
                        {
                            ArrowL = ArrowL->pointers[1];
                        }
                        else if (inContent <= ArrowL->content)//the previous place where ArrowL was, and the current node tat ArrowL is at now, in between those 2 nodes, the inContent should go in there
                        {
                            ArrowL = ArrowL->pointers[0]; //first go back
                            ArrowR = ArrowL->pointers[1]; //let ArrowR stand on the right
                            ArrowL->pointers[1] = ArrowN;
                            ArrowR->pointers[0] = ArrowN;
                            ArrowN->pointers[0] = ArrowL;
                            ArrowN->pointers[1] = ArrowR;
                            size++;

                            JobDone = true;
                        }

                        //for ArrowR
                        if(JobDone == false)
                        {
                        if (inContent < ArrowR->content)
                        {
                            ArrowR = ArrowR->pointers[0];
                        }
                        else if (ArrowR->content <= inContent)
                        {
                            ArrowR = ArrowR->pointers[1]; //first go back
                            ArrowL = ArrowR->pointers[0]; //then let ArrowL be on the left side
                            ArrowL->pointers[1] = ArrowN;
                            ArrowR->pointers[0] = ArrowN;
                            ArrowN->pointers[0] = ArrowL;
                            ArrowN->pointers[1] = ArrowR;
                            size++;

                            JobDone = true;
                        }
                        }
                    }
                }
                

                
                
            }


        }
    }



    TYPE remove_byPointer(NOwOdes_Node<TYPE>* TargetArrow)
    {
        TYPE returnResult;

        if (rootPointerL != rootPointerR && rootPointerL != nullptr && rootPointerR != nullptr)
        {
            //if there are more than one nodes
            if(TargetArrow->pointers[0] != nullptr)//there is something on the left
            {
                TargetArrow->pointers[0]->pointers[1] = TargetArrow->pointers[1];
            }
            else
            {
                //there is nothing on the left, meaning, it is a left most node
                TargetArrow->pointers[1]->pointers[0] = nullptr;
                rootPointerL = rootPointerL->pointers[1];
            }

            if(TargetArrow->pointers[1] != nullptr)//there is something on the right
            {
                TargetArrow->pointers[1]->pointers[0] = TargetArrow->pointers[0];
            }
            else
            {
                //there is nothing on the right, it is a right most node
                TargetArrow->pointers[0]->pointers[1] = nullptr;
                rootPointerR = rootPointerR->pointers[0];
            }
        }
        else if (rootPointerL == rootPointerR && rootPointerL != nullptr)
        {
            //if there is only one node
            rootPointerL = nullptr;
            rootPointerR = nullptr;
        }
        else if (rootPointerL == nullptr && rootPointerR == nullptr)
        {
            //no nodes
            cout << fowo.cOwOut.ConsoleQuick("error","no more nodes to remove") << endl;
            returnResult = popNullValue;
        }

        if(TargetArrow != nullptr)
        {
            returnResult = TargetArrow->content;
            delete TargetArrow;
            size--;
        }

        return returnResult;
    }

    TYPE pop(char InDirection)
    {
        TYPE returnResult;

        if(InDirection == 'l')
        {
            returnResult = remove(rootPointerL);
        }
        else if (InDirection == 'r')
        {
            returnResult = remove(rootPointerR);
        }

        return returnResult;
    }

    void remove_byContent(TYPE InTargetedContent, int InRemoveMaxCount)
    {
        //removes the nodes which contains the targeted content
        //every iteration it removes a node, counter++
        //if counter reaches max, or arrow reaches nullptr, done

        NOwOdes_Node<TYPE>* Arrow = rootPointerL;
        NOwOdes_Node<TYPE>* Arrow2 = Arrow;
        int Counter = 0;

        while(Arrow != nullptr && Counter < InRemoveMaxCount)
        {
            if(Arrow->content == InTargetedContent)
            {
                Arrow2 = Arrow;
                Arrow = Arrow->pointers[1];
                remove_byPointer(Arrow2);
                Counter++;
                Arrow2 = nullptr;
            }
            else
            {
                Arrow = Arrow->pointers[1];
            }
        }

        return;
    }



    NOwOdes_Node<TYPE>* search_byIndex(int InIndex)
    {
        //if CutIndex is closer to rootPointerL, then start from L
        //if CutIndex is closer to rootPointerR, then start from R

        int IndexCurrent;
        NOwOdes_Node<TYPE>* Arrow = nullptr;

        if(InIndex >= size)
        {
            cout << fowo.cOwOut.ConsoleQuick("error","cut index out of range") << endl;
        }
        else
        {
            if(InIndex < size/2)
            {
                Arrow = rootPointerL;
                IndexCurrent = 0;
                if(rootPointerL == nullptr)
                {
                    return nullptr;
                }
                else
                {
                    while(Arrow != nullptr && IndexCurrent < InIndex)
                    {
                        Arrow = Arrow->pointers[1];
                        IndexCurrent++;
                    }

                    return Arrow;
                }
            }
            else
            {
                Arrow = rootPointerR;
                IndexCurrent = size-1;
                if(rootPointerR == nullptr)
                {
                    return nullptr;
                }
                else
                {
                    while(Arrow != nullptr && IndexCurrent > InIndex)
                    {
                        Arrow = Arrow->pointers[0];
                        IndexCurrent--;
                    }

                    return Arrow;
                }
            }
        }

        return Arrow;
    }

    TYPE content_byIndex(int InIndex, TYPE returnThisIfNull)
    {
        NOwOdes_Node<TYPE>* Arrow = search_byIndex(InIndex);
        if(Arrow == nullptr)
        {
            return returnThisIfNull;
        }
        else
        {
            return Arrow->content;
        }

        return returnThisIfNull;
    }

    void cutFlip(int InCutIndex)
    {
        if(InCutIndex >= size-1)
        {
            cout << fowo.cOwOut.ConsoleQuick("error","cut index out of range") << endl;
        }
        else
        {
            //                     cut index
            //                     |
            //                     v
            // L->(0)=(1)=(2)=...=(k)==//==(k+1)=...=(s-3)=(s-2)=(s-1)<-R
            //     ^               ^         ^                     ^
            //    oldL          newR         newL               oldR

            NOwOdes_Node<TYPE>* NewRootR = search_byIndex(InCutIndex);
            NOwOdes_Node<TYPE>* NewRootL = NewRootR->pointers[1];

            rootPointerL->pointers[0] = rootPointerR; //to deal with oldL
            rootPointerR->pointers[1] = rootPointerL; //to deal with oldR
            NewRootL->pointers[0] = nullptr; //to deal with newL
            NewRootR->pointers[1] = nullptr; //to deal with newR

            //update root pointers
            rootPointerL = NewRootL;
            rootPointerR = NewRootR;
        }
    }



};

template <typename TYPE>
class NOwOdes_Tree
{
    public :

    NOwOdes_Node<TYPE>* rootPointer;
    NOwOdes_Node<TYPE>* CursorPointer;

    int size;

    TYPE popNullValue;

    FOwO fowo;

    //here we will
    //let NOwOdes_node.pointers[0] = parent
    //let NOwOdes_node.pointers[x>0] = children 

    NOwOdes_Tree()
    {
        rootPointer = nullptr;
        CursorPointer = nullptr;
        size = 0;
    }

    bool Cursor_GoUp()
    {
        //if managed to successfully go Up, return true, else false

        if (rootPointer == CursorPointer)
        {
            return false ;
        }
        else if (CursorPointer->pointers.size() == 0)
        {
            return false;
        }
        else
        {
            CursorPointer = CursorPointer->pointers[0];
            return true;
        }

        return false;
    }

    bool Cursor_GoToChildren (int ChildrenIndex)
    {
        //children index starts from 0
        //but pointers index starts from 0 too
        //so ChildIndex = PointersIndex - 1

        //pointers : 0 1 2 3 4 5 , size = 6
        //children :   0 1 2 3 4
        //asked : 5 , result : reject
        //asked : 2 , result : goto pointers[3]

        if (CursorPointer->pointers.size() == 0)
        {
            //this node has no children
            return false;
        }
        else if (CursorPointer->pointers.size() - 2 < ChildrenIndex ) //pointrs.size()-2 will be the index of the last child
        {
            //out of range
            return false;
        }
        else
        {
            CursorPointer = CursorPointer->pointers[ChildrenIndex + 1];
            return true;
        }

        return false;
    }

    void Cursor_AddChildren (int InIndex, TYPE InContent)
    {
        //InIndex over here will be the child index instead of the pointers index

        NOwOdes_Node<TYPE>* NewArrow = new NOwOdes_Node<TYPE>(InContent);//create a new node out of nowhere
        NewArrow->pointers.push_back(CursorPointer); //the parent of this node is the same as the cursor

        vector< NOwOdes_Node<TYPE>* > pointerTempList = {};

    }

};