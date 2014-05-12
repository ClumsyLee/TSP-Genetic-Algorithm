#include <cmath>
#include <algorithm>
#include <iostream>
#include <random>
#include <utility>
#include "genetic.h"

namespace AI_homework {
namespace TSP_genetic_algorithm {


Genetic::Genetic(const CityInfo &city_info)
    : city_info_(city_info),
      distances_(),
      generator_(),
      population_size_(0),
      crossover_rate_(0.0),
      mutation_rate_(0.0),
      population_(),
      best_individual_(),
      generation_(0)
{
    int city_number = city_info_.size();
    // resize the array to size() rows
    distances_.resize(city_number);

    // calculate the distance between each city
    for (int i = 0; i < city_number; i++)
        for (int j = 0; j < city_number; j++)
        {
            double x_differ = city_info_[i].x - city_info_[j].x;
            double y_differ = city_info_[i].y - city_info_[j].y;
            distances_[i].push_back(std::sqrt(x_differ * x_differ +
                                              y_differ * y_differ));
        }

    // initialize random number generator
    generator_.seed(std::random_device()());
}

Genetic::Path Genetic::FindPath(int population_size, double crossover_rate,
                                double mutation_rate)
{
    // initialize parameters
    population_size_ = population_size > 0 ? population_size : 1;
    crossover_rate_ = (crossover_rate >= 0.0 && crossover_rate <= 1.0) ?
                       crossover_rate :
                       1.0;
    mutation_rate_ = (mutation_rate >= 0.0 && mutation_rate <= 1.0) ?
                     mutation_rate :
                     0.0;

    // initialize status
    GenerateRandomPopulation();
    UpdateBestIndividual();
    generation_ = 0;

    while (!ShouldTerminate())
    {
        std::clog << "Generation " << generation_
                  << " : " << best_individual_.fitness << std::endl;
        Select();
        Crossover();
        Mutate();
        UpdateBestIndividual();
        generation_++;
    }

    Path best_path(best_individual_.path, best_individual_.cost);

    return best_path;
}


// assume the population is randomly arranged at any time
void Genetic::Crossover()
{
    int mating_number = std::round(population_size_ * crossover_rate_);
    for (int i = 1; i < mating_number; i += 2)
    {
        // mating i and i - 1
        population_[i].Crossover(population_[i - 1], generator_);
        Evaluate(population_[i]);
        Evaluate(population_[i - 1]);
    }
}

void Genetic::Evaluate(Individual &individual)
{
    double cost = 0.0;
    int last_city = individual.path.back();
    for (int new_city : individual.path)
    {
        cost += distances_[last_city][new_city];
        last_city = new_city;
    }
    individual.cost = cost;
    individual.fitness = 1.0 / cost;
}

void Genetic::GenerateRandomPopulation()
{
    population_.clear();

    Individual base_individual;
    int city_number = city_info_.size();
    for (int i = 0; i < city_number; i++)
        base_individual.path.push_back(i);

    for (int i = 0; i < population_size_; i++)
    {
        population_.push_back(base_individual);
        std::shuffle(population_.back().path.begin() + 1,
                     population_.back().path.end(),
                     generator_);
        Evaluate(population_.back());
    }
// population_.back().path = {0, 10, 13, 3, 7, 9, 14, 18, 6, 17, 15, 4, 12, 19, 5, 16, 8, 1, 11, 2};
// Evaluate(population_.back());
}

void Genetic::Mutate()
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    for (auto &individual : population_)
    {
        if (distribution(generator_) < mutation_rate_)
        {
            individual.Mutate(generator_);
            Evaluate(individual);
        }
    }
}

void Genetic::Select()
{
    double fitness_sum = 0.0;
    for (const auto &individual : population_)
        fitness_sum += individual.fitness;

    Population new_population;
    new_population.reserve(population_size_);

    // choose new population according to a individual's ratio
    typedef std::pair<int, double> FitnessInfo;
    std::vector<FitnessInfo> fractions;
    for (int i = 0; i < population_size_; i++)
    {
        double ratio = population_[i].fitness / fitness_sum * population_size_;
        int number_chosen = static_cast<int>(ratio);

        new_population.insert(new_population.end(), number_chosen,
                              population_[i]);
        fractions.push_back(FitnessInfo(i, ratio - number_chosen));
    }

    // move the best elements to the front
    int more_need = population_size_ - new_population.size();
    std::nth_element(fractions.begin(), fractions.begin() + more_need - 1,
                     fractions.end(),
                     [] (const FitnessInfo &lhs, const FitnessInfo &rhs)
                     { return lhs.second > rhs.second; });

    // fill new_population
    for (int i = 0; i < more_need; i++)
    {
        new_population.push_back(population_[fractions[i].first]);
    }

    // new population created
    population_ = std::move(new_population);
}

bool Genetic::ShouldTerminate()
{
    return generation_ > 2000;
}

void Genetic::UpdateBestIndividual()
{
    double max_fitness = best_individual_.fitness;
    int index_of_best_individual = -1;

    for (int i = 0; i < population_size_; i++)
    {
        if (population_[i].fitness > max_fitness)
        {
            max_fitness = population_[i].fitness;
            index_of_best_individual = i;
        }
    }

    if (index_of_best_individual >= 0)  // found better
    {
        best_individual_ = population_[index_of_best_individual];
    }
}

}  // namespace TSP_genetic_algorithm
}  // namespace AI_homework
