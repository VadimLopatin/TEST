from typing import Tuple

import math
import pytest

def vwap_price(required_size: int, sizes: Tuple[float, ...], prices: Tuple[float, ...]) -> float:
    """
    Get the VWAP price for a size from the given profile, or NaN if a price
    cannot be calculated. Consider the incoming data as an order book with the function's intention to
    calculate average price for the given required size;
    or top of the book if the latter is not specified.
    """
    size_left = required_size
    vwap = 0
    i = 0
    num_entries = len(sizes)
    
    if(required_size==0):
        if(num_entries==0):
            return 0.0
        else:
            return prices[0]
    
    while(size_left>0)&(i<(num_entries)):
        if(size_left >= sizes[i]):
            size_left -= sizes[i]
            vwap += sizes[i]*prices[i]
            i += 1
        else:
            vwap += size_left*prices[i]
            size_left = 0
    
    if (size_left >0):
        return float('NaN')
    else:
        return vwap/required_size


@pytest.mark.parametrize(
    ["required_size", "prices",       "sizes",                "expected_vwap_price"],
    [
     [0,              [],                   [],               0.0],
     [1,              [],                   [],               float('NaN')],
     [0,              [1.0],                [1],              1.0],
     [1,              [1.0],                [1],              1.0],
     [1,              [0.0],                [1],              0.0],
     [2,              [0.0, 1.0, 3.0],      [1, 1, 5],        0.5],
     [5,              [0.0, 1.0, 3.0],      [1, 2, 5],        1.6],
     [8,              [0.0, 1.0, 3.0],      [1, 2, 5],        2.125],
     [9,              [0.0, 1.0, 3.0],      [1, 2, 5],        float('NaN')],
    ])
def test_vwap_price(required_size, prices, sizes, expected_vwap_price):
    if math.isnan(expected_vwap_price):
        assert math.isnan(vwap_price(required_size, sizes, prices))
    else:
        assert vwap_price(required_size, sizes, prices) == expected_vwap_price

