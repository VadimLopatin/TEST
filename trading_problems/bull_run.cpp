/*
	Given the time series data of daily closing price of a oil
	Find the starting date of longest bull run for oil.
        Assume the input time is monotonically increasing. 
	longest bull run : The longest period which instrument saw continous increase in price
*/

#include <iostream>
#include <map>
#include <vector>
#include <optional>

struct PriceOnDate {
	std::string time;
	double value;
};

typedef std::vector<PriceOnDate> TimeSeries;

optional<std::string> bull_run_start(const TimeSeries& ts_obj) {
	bool found = false;

	int serie_size = ts_obj.size();

	int start = 0, current_start = 0;
	int length = 1, current_length = 1;
	
	for(int i=1; i< serie_size; i++)
	{
		if(ts_obj[i].value > ts_obj[i-1].value)
		{
			current_length++;
		}		
		else
		{
			if(current_length > length)
			{
				length = current_length;
				start = current_start;
			}
			current_start = i;
			current_length = 1;
			
		}
	}

	if(current_length > length)
	{
		length = current_length;
		start = current_start;
	}
	
	found = (length!=1);
	

	if (found) {
		return {(ts_obj[start].time)};
	} else {
		return {};
	}
}

int main() {
	TimeSeries time_series = {
		{"20190102", 33.90009},	
		{"20190103", 32.90009}, 
		{"20190104", 33.80009},
		{"20190105", 33.90001}, 
		{"20190106", 33.90009}, 
		{"20190107", 34.90009},
		{"20190108", 35.90009}, 
		{"20190109", 35.90509}, 
		{"20190110", 39.90009},
		{"20190111", 31.90009}, 
		{"20190112", 33.90009}, 
		{"20190113", 34.90009},
		{"20190114", 36.90009}, 
		{"20190115", 35.90009}, 
		{"20190116", 36.90709},
		{"20190117", 33.90009},		
		{"20190118", 30.90009}
	};

	auto output = bull_run_start(time_series);

	
	if (output.has_value()) {
		cout << output.value().c_str() << endl;
	} else {
		cout << "Bull Run not found" << endl;
	}
	
	return 0;
}
