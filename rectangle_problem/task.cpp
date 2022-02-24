// for the binary matrix MxN find the ONES-submatrix with the highest area
// input - binary matrix (zero-ones matrix)
// output - upper-left and lower-right coordinates of the requested submatrix

#include <vector>
#include<iostream>
#include <stack>
#include <tuple>

typedef std::tuple<int,int,int,int> Crd;

void get_max_rectangle(const std::vector<int> & hist, int & max_area, Crd & coords, int y)
{
	int hist_len = hist.size();
	int i = 0;
	std::stack <int> left_indices;
	
	
	while (i < hist_len)
	{
		if((left_indices.empty())||(hist[left_indices.top()]<=hist[i]))
		{
			left_indices.push(i);
			i++;
		}
		else
		{
			int next_left = left_indices.top();
			left_indices.pop();

			int current_area;
			Crd current_coords;

			if(left_indices.empty())
			{
				current_area = hist[next_left]*i;
				current_coords = std::make_tuple(0, (y - hist[next_left] + 1), i-1, y);

			}
			else
			{
				current_area = hist[next_left]*(i-left_indices.top()-1);
				current_coords = std::make_tuple((left_indices.top()+1), (y - hist[next_left] + 1), i-1, y);
			}

			if(max_area<current_area) 
			{
				max_area = current_area;
				coords = current_coords;
			}
			
		}
	}


	while(!left_indices.empty())
	{
		int next_left = left_indices.top();
		left_indices.pop();

		int current_area;
		Crd current_coords;

		if(left_indices.empty())
		{
			current_area = hist[next_left]*i;
			current_coords = std::make_tuple(0, (y - hist[next_left] + 1), i-1, y);
		}
		else
		{
			current_area = hist[next_left]*(i-left_indices.top()-1);
			current_coords = std::make_tuple((left_indices.top()+1), (y - hist[next_left] + 1), i-1, y);
		}

		if(max_area<current_area)
		{
			max_area = current_area;
			coords = current_coords;
		}
		
	}

}

int main()
{
	int max_rectangle=0;
	Crd coordinates;

	int N, M;
	std::cin>>N>>M;
	std::vector<std::vector<int>> matrix(N, std::vector<int>(M));
	for(int i=0; i<N; i++)
	{
		for(int j=0; j<M; j++)
		{
			std::cin>>matrix[i][j];
		}

	}
	
	get_max_rectangle(matrix[0], max_rectangle, coordinates, 0);
	
	for(int i=1; i<N; i++)
	{
		for(int j=0; j<M; j++)
		{
			if(matrix[i][j])
			{
				matrix[i][j] += matrix[i-1][j];
			}
		}
				

		get_max_rectangle(matrix[i], max_rectangle, coordinates, i);


	}
	


	std::cout<<max_rectangle<<std::endl;
	std::cout<<"("<<(std::get<0>(coordinates))<<", "<<(std::get<1>(coordinates))<<")"<<std::endl;
	std::cout<<"("<<(std::get<2>(coordinates))<<", "<<(std::get<3>(coordinates))<<")"<<std::endl;	

	return 0;
}
