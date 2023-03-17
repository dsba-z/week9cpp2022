#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>


class FootballTeam
{
public:
    FootballTeam(std::string Name, std::string City, std::string Stadium, double Level):
    name(Name), city(City), stadium(Stadium), level(Level), points(0), wins(0), losses(0), draws(0) {}

    double getLevel() const
    {
        return level;
    }

private:
    std::string name;
    std::string city;
    std::string stadium;
    int points;
    int wins;
    int losses;
    int draws;
    double level;
};

class Tournament
{
public: 
    Tournament(std::vector<FootballTeam> Teams, int Round = 0):
    teams(Teams), round(Round) {}

    void simulateRound();

private:
    std::vector<FootballTeam> teams;
    int round;
};

class Match
{
public:
    Match(FootballTeam& LTeam, FootballTeam& VTeam): 
    localTeam(LTeam), visitorTeam(VTeam), localGoals(0), visitorGoals(0) {}

    void play();

private:
    int localGoals;
    int visitorGoals;
    FootballTeam& localTeam;
    FootballTeam& visitorTeam;
};

void Match::play()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(0, ceil( abs(localTeam.getLevel() - visitorTeam.getLevel()) * 10) );   

    int k = distr(gen);

    for (int i = 0; i < k; i++)
    {
        std::vector<double> v{- localTeam.getLevel(), 0.0, visitorTeam.getLevel()};
        std::piecewise_linear_distribution distr2(v.begin(), v.end(), v.begin());
        if (distr2(gen) < 0)
            localGoals++;
        else
            visitorGoals++;
    }
}  
    

void parseLineTeam(const std::string& line, std::string& name, std::string& city, std::string& stadium, double& level)
{
    size_t posName = line.find(',');
    name = line.substr(0, posName);
    
    size_t posCity = line.find(',', posName + 1);
    city = line.substr(posName + 1, posCity - posName - 1);
    
    size_t posStadium = line.find(',', posCity + 1);
    stadium = line.substr(posCity + 1, posStadium - posCity - 1);
    
    size_t posLevel = line.size() - 1;
    std::string levelString = line.substr(posStadium + 1, posLevel - posStadium);
    std::stringstream ss(levelString);
    ss >> level;
    
}

void loadTeams(std::istream& in, std::vector<FootballTeam>& teams)
{
    std::string buffer;
    std::getline(in, buffer);
    while(std::getline(in, buffer))
    {
        std::string name;
        std::string city;
        std::string stadium;
        double level;
        parseLineTeam(buffer, name, city, stadium, level);
        std::cout << "name = " << name << "\n";
        std::cout << "city = " << city << "\n";
        std::cout << "stadium = " << stadium << "\n";
        std::cout << "level = " << level << "\n\n";
        teams.push_back(FootballTeam (name, city, stadium, level));
    }
}


int main()
{
    const std::string FILENAME = "../../../data/problem2_matches/football.csv";
    std::ifstream inputFile;
    inputFile.open(FILENAME);
    
    // FootballTeam name("asda", " safa", "asfaas", 1);
    std::vector<FootballTeam> teams;
    loadTeams(inputFile, teams);
    
    return 0;
}
