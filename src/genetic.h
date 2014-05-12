#ifndef GENETIC_H_
#define GENETIC_H_

#include <random>
#include <utility>
#include <vector>
#include "individual.h"

namespace AI_homework {
namespace TSP_genetic_algorithm {

class Genetic
{
 public:
    struct City
    {
        char name;
        double x, y;
    };
    typedef std::vector<City> CityInfo;
    typedef std::pair<std::vector<int>, double> Path;

    explicit Genetic(const CityInfo &city_info);

    Path FindPath(int population_size, double crossover_rate,
                  double mutation_rate);

 private:
    typedef std::vector<std::vector<double>> DistanceArray;
    typedef std::vector<Individual> Population;

    // assume the population is randomly arranged at any time
    // so do NOT sort the population
    void Crossover();
    void Evaluate(Individual &individual);
    void GenerateRandomPopulation();
    void Mutate();
    void Select();
    bool ShouldTerminate();
    void UpdateBestIndividual();

    // problem related vaiables
    const CityInfo city_info_;
    DistanceArray distances_;

    // random number generator
    mutable std::mt19937 generator_;

    // parameters for algorithm
    int population_size_;
    double crossover_rate_;
    double mutation_rate_;

    // status variables
    Population population_;  // do NOT sort
    Individual best_individual_;
    int stable_truns_;
    int generation_;
};

}  // namespace TSP_genetic_algorithm
}  // namespace AI_homework

#endif  // GENETIC_H_
