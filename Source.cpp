// Source.cpp : Intimate Algorithm (Finding the same attributes among a group of people)
// A project for Algorithmics Course, University Of Tartu
// Mohammad Anagreh 

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <omp.h>
#include <sstream>  
#include <stdio.h>
#include<algorithm>


using namespace std;

#define Graph_Size 10  //  Number of Vertices (Number of People)
#define Attributes_Size 5  //  Number of atributes in both arrays Like and Dislike
#define Constant 1.0   //  The total number of attributes is 1  


float like_convergence = 0.0;   // A counter to count the number of converging attributes (Positive)
float Dislike_convergence = 0.0;// A counter to count the number of converging attributes (Negative) 
float Like_spacing = 0.0;     // A counter to count the number of incompatible attributes (like ==> Dislike)
float Dislike_spacing = 0.0;     // A counter to count the number of incompatible attributes (Dislike ==> like)
float spacing = 0.0;      // A counter to calculate uncomputable attributes (for both cases)
float convergence = 0.0;  // A counter to calculate convergence attributes (for both cases)
float intimate = 0.0;     // A counter to calculate the difference between convergence and spacing

// The main structure of the vertex in the graph
struct NodeData
{
	int ver_num ;
	string ver_name;
	string Like[Attributes_Size];  
	string Dislike[Attributes_Size]; 
};


// The structure of the relationship of the vertices 
struct cluster
{
	int ver_num;
	int num1;
	string ver_name1;
	int num2;
	string ver_name2;
	float convergence;
	float spacing;
	float intimate;
};


// The structure of the closest vertex
struct closest
{
	int ver_num;
	string ver_name;
	float friend_num[Graph_Size];
	string friend_name[Graph_Size];
	float  intimate[Graph_Size ];
	
};

// Prototypes of the functions
NodeData *ReadFile(); 
void WriteFile();



int main()
{

	//system("COLOR FC");
	
	double start = omp_get_wtime();
	cluster clusterN[Graph_Size*(Graph_Size - 1) / 2]; // The graph size of the complete graph which is n(n-1)/2 
	closest closestN[Graph_Size];
	NodeData *NodeV = ReadFile(); // To read the data of the graph

	/*
	int j = 0;
	for (int i = 0; i < Graph_Size; i++)
	{
		cout << NodeV[i].ver_num << " ==> " << NodeV[i].ver_name << endl;
		for (int j = 0; j < Attributes_Size; j++)
		{
			cout << "Like " << NodeV[i].Like[j] << endl;
		}
		for (j = 0; j < Attributes_Size; j++)
		{
			cout << "Dislike " << NodeV[i].Dislike[j] << endl;
		}
	
		cout << "-------------------------------" << endl;
	}
	*/

	//A counter to provide the number of the vertices
	int counter = 0; 

	//A counter to index the size of the clauster graph
	int count = 0;  


	//To provide the number of attributes for each vertex in the grapg G
	float Attributes_constant;
	Attributes_constant = (float) Constant / Attributes_Size;
	cout << endl;
	cout <<"Attributes Constant is : "<< Attributes_constant << endl;


	// The main processes is to find the attributes matching for the whole vertices in graph G
	// Loops i and j are to route all the vertices in the given complete graph
	// Loops k and l are to check the convergence and spacing of the attributes
	// like_convergence is a counter to count the number of identical attributes among vertices (Likes)
	// Dislike_convergence is a counter to count the number of identical attributes among vertices( DisLikes)
	// Convergence is a counter to find the convergence among vertices in both cases (Like + Dislike)
	// Like_spacing is a counter to find the spacing from vertex i to k
	// Dislike_spacing is a counter to find the spacing from vertex k to i 
	// Spacing  is the highest spacing eathier Like_spacing or Dislike_spacing
	// Intimate is the difference between the convergence and spacing  

	for (int i = 0; i < Graph_Size; i++)
	{
		for (int j = i+1; j < Graph_Size; j++)
		{
			for (int k = 0; k < Attributes_Size; k++)
			{
				for (int l = 0; l <Attributes_Size; l++)
				{
					if (NodeV[i].Like[k] == NodeV[j].Like[l])
						like_convergence = like_convergence + Attributes_constant;
					if (NodeV[i].Dislike[k] == NodeV[j].Dislike[l])
						Dislike_convergence = Dislike_convergence + Attributes_constant;
					if (NodeV[i].Like[k] == NodeV[j].Dislike[l])
						Like_spacing = Like_spacing + Attributes_constant;
					if (NodeV[i].Dislike[k] == NodeV[j].Like[l])
						Dislike_spacing = Dislike_spacing + Attributes_constant;
				}
			}
			convergence = like_convergence + Dislike_convergence;
			if (Like_spacing >= Dislike_spacing)
				spacing = Like_spacing;
			else
				spacing = Dislike_spacing;
		
				intimate = convergence - spacing;


		//	cout << count << " " << NodeV[i].ver_name << "  and  " << NodeV[j].ver_name << "    ==>" << " The Convergence is " << convergence << ",   Spacing is " << spacing <<"     Intimate = "<< intimate<<  endl;
		//	count = count + 1;
			
 
		//  Finding the total relationships of the givin graph 	 
			clusterN[count].ver_num = counter;

			clusterN[count].num1 = NodeV[i].ver_num;
			clusterN[count].ver_name1 = NodeV[i].ver_name;

			clusterN[count].num2 = NodeV[j].ver_num;
			clusterN[count].ver_name2 = NodeV[j].ver_name;

			clusterN[count].convergence = convergence;
			clusterN[count].spacing = spacing;
			clusterN[count].intimate = intimate;


			count = count + 1;
			counter = counter + 1;

        // Reset all locations in the memory before a new iteration starts
			convergence = 0.0;
			spacing = 0.0;
			Like_spacing = 0.0;
			Dislike_spacing = 0.0;
			like_convergence = 0.0;
			Dislike_convergence = 0.0;
			intimate = 0.0;
		}
	}




	int x = 0, y = 0;
	int con = 0;

	// from the cluster graph, find the closest vertex for each vertex in the cluster graph 
	// based on intimate flag

	for (int i = 0; i < Graph_Size*(Graph_Size - 1) / 2; i++)
	{

			   x =  clusterN[i].num1;
			   y =  clusterN[i].num2;
			   char* str = &clusterN[i].ver_name2[0];
			   char* str1 = &clusterN[i].ver_name2[0];

			   //cout << "--------------------------" << endl;
			   //cout << x << " " << y << endl;
			   closestN[x].ver_num = (float) x;
			   closestN[x].ver_name = clusterN[i].ver_name1;

			   closestN[x].friend_num[y] = (float)  y;
			   closestN[x].friend_name[y] = str;
			   //cout << "str = " << str << endl;
			   closestN[x].intimate[y] = clusterN[i].intimate;
		       // cout << closestN[x].ver_num <<"  "<<closestN[x].friend_num[y]<<"  Intimate is:"<< closestN[x].intimate[y]<< endl;
		       // cout << closestN[x].friend_name[y] << "  " << closestN[x].ver_name << "  " << endl;
			   // cout << y << " " << x;
			   closestN[y].ver_num = (float) y;
			   closestN[y].ver_name = clusterN[i].ver_name1;

			   closestN[y].friend_num[x] = (float)x;
			   closestN[y].friend_name[x] = str;
			   //cout << "str =" << str << endl;
			   closestN[y].intimate[x] = clusterN[i].intimate;
		       // cout << closestN[y].ver_num << "  " << closestN[y].friend_num[x] << "  Intimate is:" << closestN[y].intimate[x] << endl;
		       // cout << closestN[y].ver_name << "  " << closestN[y].friend_name[x] << endl;
	}

	/*
	x = 0; y = 0;
	for (int i = 0; i < Graph_Size*(Graph_Size - 1) / 2; i++)
	{

		x = clusterN[i].num1;
		y = clusterN[i].num2;
		cout << "=============" << endl;
		cout << closestN[x].ver_num << "  " << closestN[x].friend_num[y] << "  Intimate is:" << closestN[x].intimate[y] << endl;
		cout << closestN[x].friend_name[y] << "  " << closestN[x].ver_name << "  " << endl;

		cout << closestN[y].ver_num << "  " << closestN[x].friend_num[y] << "  Intimate is:" << closestN[y].intimate[x] << endl;
		cout << closestN[x].ver_name << "  " << closestN[x].friend_name[y] << endl;

		
	}
	*/


	// To print out the list of relationships among the vertices 
	cout << endl;
	cout << "Finding the relationships...." << endl;
	cout << endl;
	cout << "===================================================================================================" << endl;
	for (int i = 0; i < Graph_Size*(Graph_Size - 1) / 2; i++)
	{
		cout << clusterN[i].ver_num << " ==>  (" << clusterN[i].num1 << ") " << clusterN[i].ver_name1 << "   AND "
			<< " (" << clusterN[i].num2 << ") " << clusterN[i].ver_name2;
		printf("    Convergence is: %.2f", clusterN[i].convergence);
		printf("    Spacing is: %.2f", clusterN[i].spacing);
		if (clusterN[i].intimate == 2)
		    cout << "    Intimate  is: " << clusterN[i].intimate <<"   <== Perfect"<<endl;
		else
			cout << "    Intimate  is: " << clusterN[i].intimate << endl;

	}
	cout << "===================================================================================================" << endl;


	//To print out the closest vertices
	cout << endl;
	cout << "Finding the closest vertices..." << endl;
	cout << endl;
	
	float max = 0.0;
	string name;
	int num;
	for (int x = 0; x < Graph_Size; x++)
	{
		max = closestN[x].intimate[y];
		for (int y = 0; y < Graph_Size; y++)
		{
			//cout << closestN[x].intimate[y] << endl;
			if (closestN[x].intimate[y] > max)
			{
				max = closestN[x].intimate[y];
				name = closestN[y].ver_name;
				num = closestN[y].ver_num;
			}

		}
		cout << "(" << closestN[x].ver_num << ") " << closestN[x].ver_name << " cloased to (" << num << ") " << name << " Intimate = " << max << endl;
		cout << "-------------------------------------------------" << endl;
		max = 0.0;
	}


	// Get the total execution time and finalize
	cout << '\n';
	cout <<"The total Execution Time... "<< endl;
	double end = omp_get_wtime();
	printf("Time = %.16g", end - start);
	cout << endl;
	system("pause");
	return 0;
}

// This function to read the data of the graph from given file
// Save the data in the memoery based on the the main structure NodeData

NodeData *ReadFile()
{
	NodeData *Node = new NodeData[Graph_Size];


	try {
		ifstream infile;
		infile.open("GraphFile.txt");
		cout << "Reading the global graph from the given file" << endl;
		cout << "Read the file ......" << endl;
		cout << endl;
		string ch;
		for (int i = 0; i < Graph_Size; i++)
		{
			getline(infile, ch);

			//cout << ch<< endl;

			std::istringstream iss(ch);
			std::string sub;
			int count = 0;
			int count_like = 0;
			int count_Dislike = 0;
			int count_chct = 0;
			while (iss >> sub)
			{
				if (count == 0)
				{
					Node[i].ver_num = stoi(sub);
					count = count + 1;
					continue;
				}
				if (count == 1)
				{
					Node[i].ver_name = sub;
					count = count + 1;
					continue;
				}
				if (count >= 2 && count <= 6)
				{
					Node[i].Like[count_like] = sub;
					count = count + 1;
					count_like = count_like + 1;
					continue;
				}
				if (count >= 7 && count <= 11)
				{
					Node[i].Dislike[count_Dislike] = sub;
					count = count + 1;
					count_Dislike = count_Dislike + 1;
				}

			}
		}
		infile.close();
	}
	
	catch (std::exception& e)
	{
		cout << "Please check the source file" << endl;
	}
	
	// To print out the data for the checking 
	/*
	int j = 0;
	for (int i = 0; i < 5; i++)
	{
		cout << Node[i].ver_num << "==>" << Node[i].ver_name << endl;
		for (int j = 0; j < 5; j++)
		{
			cout << "Like " << Node[i].Like[j] << endl;
		}
		for (j = 0; j < 5; j++)
		{
			cout << "Dislike " << Node[i].Dislike[j] << endl;
		}
		for ( j = 0; j < 5; j++)
		{
			cout<<"Chct " << Node[i].chct[j] << endl;
			
		}
		cout << "-------------------------------" << endl;
	}
	*/

	return Node;
}



















