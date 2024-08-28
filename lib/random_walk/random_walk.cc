/**
 * @file random_walk.cc
 * @author Corey Maxwell (corey.maxwell18@proton.me)
 * @brief Function Declarations for the Random Walk Object
 * @version 0.1
 * @date 2024-08-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "random_walk.hh"
namespace random_walk{
RandomWalk::~RandomWalk()
{
}

RandomWalk::RandomWalk()
{
	price = 0;
	u_bound = 4096;
	l_bound = 0;
	mt = std::mt19937(rd());
	dist = std::normal_distribution<double>(0.0, 0.005);
}
RandomWalk::RandomWalk(double _price)
{
	price = _price;
	mt = std::mt19937(rd());
	dist = std::normal_distribution<double>(0.0, 0.005);
	u_bound = price + 10;
	l_bound = price - 10;
}

double RandomWalk::get_price() const
{
	return price;
}

void RandomWalk::iteration()
{
	if (price < l_bound)
	{
		price = l_bound + 1;
		return;
	}
	if (price > u_bound - 1)
	{
		price = u_bound - 1;
		return;
	}
	double random = dist(mt);
	price = price + random;
}
};