#include <fstream>
#include <iostream>
#include "genetic.h"

using namespace AI_homework::TSP_genetic_algorithm;

std::vector<Genetic::City> ReadInputFileOrDie(const char *filename);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file>\n";
        std::exit(EXIT_FAILURE);
    }

    Genetic::CityInfo cities = ReadInputFileOrDie(argv[1]);

    std::ofstream fout(argv[2]);
    if (!fout.is_open())
    {
        std::cerr << "Cannot open output file: (" << argv[2] << ")\n";
        std::exit(EXIT_FAILURE);
    }

    Genetic solver(cities);
    Genetic::Path path = solver.FindPath(10000, 1.0, 0.05);


    for (int city_index : path.first)
    {
        char name = cities[city_index].name;
        std::cout << name << ' ';
        fout << name << ' ';
    }
    std::cout << path.second << std::endl;
    fout << path.second << std::endl;

    return 0;
}

std::vector<Genetic::City> ReadInputFileOrDie(const char *filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open())
    {
        std::cerr << "Cannot open input file: (" << filename << ")\n";
        std::exit(EXIT_FAILURE);
    }

    int city_num;
    fin >> city_num;
    std::vector<Genetic::City> cities(city_num);
    for (Genetic::City &city : cities)
    {
        fin >> city.name >> city.x >> city.y;
    }

  return cities;
}
