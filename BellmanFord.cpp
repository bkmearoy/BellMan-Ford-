
#include <iostream>
#include <climits>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

class Bellman{
	private:
		struct wEdge{
			int cost; std::string src; std::string dest;
			wEdge(int c, std::string s,std::string d){
				cost=c; dest=d; src=s;
			}
			wEdge(){}
		};
		std::string source;
    
        struct wVertex{
        std::string src;
        int distance;
        wVertex(std::string s){
            src=s;
        }
        wVertex(){}
    };
    
	public:
    // Create Vertex and Edge Vectors
	std::vector<wVertex> V_tracts;
	std::vector<wEdge> E_tracts;
		Bellman();
        void StorePrint();
        void BellmanFord();
	
};
// Read file through constructor 
Bellman::Bellman(){
	std::string line;
	char source;
	char incoming_input;
	int weight_input;
	
    std::cout<<"\nStep 1: Read Input line-by-line " <<std::endl;
    std::cout<<"************************************\n";
	while(std::getline(std::cin,line)){//read input file
		std::cout<<line<<std::endl;
		std::vector<std::string> parsed_lines;
		line.erase(std::remove(line.begin(),line.end(),' '),line.end());//remove all whitespace
		char *cline = new char[line.length() + 1];//convert to cstr in order tokenize the semicolons
        strcpy(cline, line.c_str());

        char * pLine;
        pLine = strtok(cline, ";");

        std::string source=pLine;
        wVertex vertex(source);//creates the vertex based on first input
        std::string fLine;
        while((pLine = strtok(NULL, ";"))!=NULL){//parses through the rest of the edges
        	fLine=pLine;
        	parsed_lines.push_back(fLine);//adds rest to list of edges
        }
        if(parsed_lines.size()==0){//makes sure vertex has edges
			V_tracts.push_back(vertex);
        	continue;
        }
        for(int i=0;i<parsed_lines.size();i++){//now to get rid of parenthesis and commas to look for dest. and cost.
			char *oldLine = new char[parsed_lines[i].length() + 1];
   	        strcpy(oldLine, parsed_lines[i].c_str());
   	        char *parLine;
   	        parLine = strtok(oldLine, ",");
   			std::string destVertex = parLine;
   			int loc = destVertex.find("(");
   			destVertex.erase(loc,1);// gets rid of parenthesis
   			//got the incoming, now for the cost
   			parLine = strtok(NULL, ","); //continue looking for cost
   			std::string costw=parLine;
   			loc = costw.find(")");
   			costw.erase(costw.length() - 1, 1); //erase ) at last position
   			int cost=atoi(costw.c_str());
   			wEdge edge(cost,source,destVertex);//adds new edge to vertex
   			E_tracts.push_back(edge);
        }
		V_tracts.push_back(vertex);
	}
	std::cout<<"**********************************************************************\n";
}

//printfunction to print final Shortest distance from a single source
void Bellman::StorePrint(){
    std::cout << "\nStep 2: store Data as  source/dest/cost "<<std::endl;
    std::cout<<"********************************************"<<std::endl;
    std::cout<<"src  dest  cost" <<std::endl;
    for(unsigned int  i=0; i<E_tracts.size();i++){
        std::cout<<E_tracts[i].src <<"      "<<E_tracts[i].dest<< "     " <<E_tracts[i].cost << std::endl;
        
    }
    
    std::cout << std::endl;
    
}

// Bellman Ford algorithm used to find shortes distance from a
// single source and also ditects negative weight cycle.
void Bellman::BellmanFord(){
    int V =V_tracts.size();
    int E =E_tracts.size();
    std::cout << "\nStep 3: Calculate number of Vertex (V) and Edges (E) " <<std::endl;
    std::cout<<"***********************************************************"<<std::endl;
    std::cout<<"V: "<<V_tracts.size() <<std::endl;
    std::cout<<"E: "<<E_tracts.size() <<std::endl;
    
    std::cout << "\nStep 4: Initialize the distance star vertex to (0) & the rest to (INT_MAX)-Infinity "<<std::endl;
    std::cout<<"**************************************************************************************"<<std::endl;
    // set the first vertex as a single source
    source=V_tracts[0].src;
    for(int i=0;i<V_tracts.size();i++){
        if(V_tracts[i].src==source){
            V_tracts[i].distance=0;//set source distance to 0
        }
        else{
            V_tracts[i].distance=INT_MAX;//set other distances to "infitnity"
        }
        std::cout<<V_tracts[i].src<<": "<<V_tracts[i].distance <<std::endl;
    }
    std::cout<<"\n";
    
    /**
     Relax all edges |V| - 1 times. A simple shortest path from src
     to any other vertex can have at-most |V| - 1 edges
     **/
    
    std::cout << "\nStep 5: Relax all edges/Distance from the source "<<std::endl;
    std::cout<<"******************************************************"<<std::endl;
    for(int i=1;i<V_tracts.size()-1;i++){//go through and find initial shortest distance
        for(int j=0;j<E_tracts.size();j++){
            std::string src = E_tracts[j].src;
            std::string dest = E_tracts[j].dest;
            wVertex vsrc; wVertex vdest;
            int ksrc; int kdest;
            for(int k=0;k<V_tracts.size();k++){//find correct vertices
                if(src==V_tracts[k].src){
                    vsrc=V_tracts[k];
                    ksrc=k;
                }
                if(dest==V_tracts[k].src){
                    vdest=V_tracts[k];
                    kdest=k;
                }
            }
            int weight=E_tracts[j].cost;
            if(vsrc.distance!=INT_MAX&&vsrc.distance+weight<vdest.distance){//finds the shortest distance
                vdest.distance=vsrc.distance+weight;
                V_tracts[ksrc]=vsrc;
                V_tracts[kdest]=vdest;
                //std::cout<<V_tracts[ksrc].src<<"--->"<<V_tracts[kdest].src<<" cost: "<<weight<<std::endl;
                std::cout<<source<<" -----> "<<V_tracts[kdest].src<<" Distance: "<<vdest.distance<<std::endl<<std::endl;
                for(int k=0;k<V_tracts.size();k++){
                    std::cout<<V_tracts[k].src<<": "<<V_tracts[k].distance<<std::endl;
                }
                std::cout<<"**********************"<<std::endl;
            }
        }
    }
    
    /**
     check for negative-weight cycles, if graph doesn't contain negative weight cycle.
     print call to print the shortes path else: print  negative weight cycle detected
     **/
   // bool found=false;
    std::cout << "\nStep 6: Check if negative Weigh Cycle at vertext Exist "<<std::endl;
    std::cout<<"************************************************************"<<std::endl;
    
    bool found=false;
    for(int i=0;i<E_tracts.size();i++){
        std::string src = E_tracts[i].src;
        std::string dest = E_tracts[i].dest;
        wVertex vsrc; wVertex vdest;
        int ksrc; int kdest;
        for(int k=0;k<V_tracts.size();k++){//find correct vertices
            if(src==V_tracts[k].src){
                vsrc=V_tracts[k];
                ksrc=k;
            }
            if(dest==V_tracts[k].src){
                vdest=V_tracts[k];
                kdest=k;
            }
        }
        int weight=E_tracts[i].cost;
        if(vsrc.distance!=INT_MAX&&vsrc.distance+weight<vdest.distance){//finds the shortest distance
            std::cout<<"negative weigh Cycle found!"<<std::endl;
            found=true;
        }
    }
    if(!found){
        std::cout<<"No negative cycles."<<std::endl;
    }
    
    std::cout << "\nStep 7: Final print of shortest distance from a single source "<<std::endl;
    std::cout<<"******************************************************************"<<std::endl;
    std::cout<<"Vertex  Shortest Distance" <<std::endl;
    for(int k=0;k<V_tracts.size();k++){
        std::cout<<V_tracts[k].src<<"        "<<V_tracts[k].distance <<std::endl;
      
    }

  return;
}// end of Bellman_Ford funtion 


// main funtion
int main(void){
    Bellman D;
    D.StorePrint();
    D.BellmanFord();
    
    return 0;
}
