/*
====================================================================================
Problem Statement
====================================================================================

	You are given initial conversion rates between different currencies

	example:
	USD/SGD: 1.4
	USD/INR: 87.0
	USD/CAD: 1.39
	PKR/AED: 35.0

	You have to design a class which will take these are input
	and give conversion rate between two currencies

	rate function should give the rate if direct or indirect conversion can be done
	for example INR/CAD is possible in above example as below

	INR/USD -> USD/CAD 

	but SGD to PKR is not possible (in this case throw exception)

	FxConversion object will be initialized once with initial fx rates but 
	rate function will be called many many times for lookup. The solution has to 
	be more optimized for lookup

====================================================================================
*/

#include <vector>
#include <string>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <set>
#include <list>
#include <algorithm>
#include <stdexcept>

typedef std::tuple<std::string, std::string, double>  RatePoint;
typedef std::vector<RatePoint>		RatePointList;

class FxConversion {
public:
	FxConversion(std::vector<std::tuple<std::string, std::string, double>> rates) {
        int num_entries = rates.size();
        for(int i =0; i< num_entries; i++)
        {
            auto [ cur1, cur2, fx_rate] = rates[i];
            graph[cur1][cur2] = fx_rate;
            graph[cur2][cur1] = 1/fx_rate;
        }
	}

    void print_graph()
    {
        for(const auto& c1 : graph)
        {
            for(const auto& c2 : c1.second)
            {
                std::cout<<c1.first<<" - "<<c2.first<<" "<<c2.second<<std::endl;
            }
        }
    }
    
    
    //since our graph is not weighted we may use simple BFS algorithm
    //thus we will find the path with optimal number of edges between two currencies
    //In case there're no path one will get an empty vector
	std::vector<std::string> bfs(const std::string& from, const std::string& to) {
	    
	    //variable for resulting path
	    std::vector<std::string> resulting_path;
	    
	    //variable for current node
	    std::string current = from;
	    
	    //we will store visited nodes in a dedicated set
        std::set<std::string> visited;
        
        //we will use list to store nodes to visit
        std::list<std::string> queue;
        
        //we will use hash map to store previous visited node
        std::unordered_map<std::string, std::string> prev;
        
        visited.insert(current);
        queue.push_back(current);
        
        while(!queue.empty())
        {
            current = queue.front();
            queue.pop_front();
            
            for (const auto& adj : graph[current])
            {
                //Check if we have visited this adjacent node
                if(visited.find(adj.first)==visited.end())
                {
                    prev[adj.first] = current;
                    
                    if(adj.first==to)
                    {
                        std::string temp = to;
                        resulting_path.push_back(temp);
                        
                        while (temp!=from)
                        {
                            temp = prev[temp];
                            resulting_path.push_back(temp);
                        }
                        
                        return resulting_path;
                    }
                    
                    visited.insert(adj.first);
                    queue.push_back(adj.first);
                }
            }
        }
        
		return resulting_path;
	}
	
	
	double rate(const std::string& from, const std::string& to) {
	    
	    //for equivalent currencies exchange rate is always 1.0
	    if(from==to) return 1.0;
	    
	    //check that the path exists in cache
	    bool exists;
	    
	    if(cache_for_paths.count(from))
	    {
	        if(cache_for_paths[from].count(to))
	        {
	            exists = true;
	        }
	        else
	        {
	            exists = false;
	        }
	    }
	    else
	    {
	        exists = false;
	    }
	    
	    //if path does not exist in cache then calculate the path
	    if(!exists)
	    {
	        //calculate the path
	        std::vector<std::string> path = bfs(from, to);
	        
	        //remeber that we get reversed path from bfs
	        //save it to the cache
            cache_for_paths[to][from] = path;
            
            //get a non-reversed path and also save it to the cache
            std::reverse(path.begin(), path.end());
            cache_for_paths[from][to] = path;
	    }
	    

        //get the path from cache:
        std::vector<std::string> path = cache_for_paths[from][to];

        //throw exception if there're no path
        if(path.empty()) throw std::invalid_argument("");

        //otherwise calculate the resulting cross fx rate from the cached path:
        double resulting_fx = 1.0;

	    for (int i=1; i<path.size(); i++)
	    {
	        resulting_fx*=graph[(path[i-1])][(path[i])];
	    }
	    return resulting_fx;
	}
	
private:

    //the graph itself
    std::unordered_map<std::string, std::unordered_map<std::string, double>> graph;
    
    //cache for storing already calculated paths between two currencies
    //We will not make a cache for resulting fx rates because rates could change while path stays the same
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::string>>> cache_for_paths;
    
};

int main() {

	RatePointList rpl{ 
		{"USD", "SGD", 1.401},
		{"USD", "CAD", 1.390},
		{"USD", "IDR", 87000.001}
	};

	FxConversion  fx_conv(rpl);

	std::string first = "CAD", second = "SGD";
	std::cout << "converting from " << first << " to " << second << " " <<  fx_conv.rate(first, second) << std::endl;

	first = "SGD"; second = "IDR";
	std::cout << "converting from " << first << " to " << second << " " << fx_conv.rate(first, second) << std::endl;

	first = "IDR"; second = "SGD";
	std::cout << "converting from " << first << " to " << second << " " << fx_conv.rate(first, second) << std::endl;

	try {
		first = "MYR"; second = "SGD";
		std::cout << "converting from " << first << " to " << second << " " << fx_conv.rate(first, second) << std::endl;
	}
	catch (...) {
		std::cout << "converting from " << first << " to " << second << " " << "Exception received" << std::endl;
	}

	return 0;
}
