/**
 * @file random_walk.hh
 * @author Corey Maxwell (corey.maxwell18@proton.me)
 * @brief Functions for simulating Sensor Outputs
 * @version 0.1
 * @date 2024-08-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef RANDOM_WALK_HH
#define RANDOM_WALK_HH
#pragma once

#include <cmath>
#include <random>

/**
 * @brief Functions and classes for simulating Sensor Outputs
 * 
 */
namespace random_walk
{
/**
 * @brief A class containing the logic and data for a Random Walk
 */
class RandomWalk
{
  public:
    /**
     * @brief Constructor For the random Walk
     */
    RandomWalk();
    /**
     * @brief Constructor for a random Walk object with a starting point
     * @param _price Starting Price
     */
    RandomWalk(double _price);
    /**
     * @brief Destructor
     */
    ~RandomWalk();
    /**
     * @brief Gets the internal price
     * @return double
     */
    double get_price() const;
    /**
     * @brief Iterates forward one step
     */
    void iteration();

  private:
    double price;
    double u_bound;
    double l_bound;
    std::random_device rd;
    std::mt19937 mt;
    std::normal_distribution<double> dist;
};
}; // namespace random_walk
#endif // RANDOM_WALK_HPP
