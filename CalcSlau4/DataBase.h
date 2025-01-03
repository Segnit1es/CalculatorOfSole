#pragma once

#include "Slau.h"
int CheckNode(const vector<Node>& list, Node& node);
void UpdateNode(Node node);
vector<Node> LoadDB(); 
void SaveDB(vector<Node> list); 
void Sort(vector<Node>& list); 
Node LoadNode(int index); 
void SaveNode(Node node); 
void PrintDB(const vector<Node>& list); 
void DeleteNodeFromDB(int index);
void CreateDB();