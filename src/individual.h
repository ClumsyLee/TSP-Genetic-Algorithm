#include <random>
#include <vector>

namespace AI_homework {
namespace TSP_genetic_algorithm {

class Individual
{
 public:
    Individual();

    void Crossover(Individual &mate, std::mt19937 &generator);
    void Mutate(std::mt19937 &generator);

    std::vector<int> path;
    double cost;
    double fitness;
};

}  // namespace TSP_genetic_algorithm
}  // namespace AI_homework
