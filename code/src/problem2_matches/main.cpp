#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>


class FootballTeam
{
public:
    FootballTeam(){}
    FootballTeam(std::string n, std::string c, std::string s, double l):
    name(n), city(c), stadium(s), level(l), points(0), wins(0), losses(0), draws(0){}
    std::string getName() const
    {
        return name;
    }
    double getLevel() const
    {
        return level;
    }
    void addWin()
    {
        wins++;
    }
    void addDraw()
    {
        draws++;
    }
    void addLoss()
    {
        losses++;
    }
    void setPoints(int p)
    {
        points += p;
    }

private:
    std::string name, city, stadium;
    double level;
    int points, wins, draws, losses;
};

std::ostream& operator<<(std::ostream& out, const FootballTeam& ft)
{
    out << ft.getName();
    return out;
}

class Tournament
{
public:
    Tournament(std::vector<FootballTeam> t = {}):teams(t), round(0){}
    void printTeams()
    {
        for (const FootballTeam& i: teams)
            std::cout << i << '\n';
    }
private:
    int round;
    std::vector<FootballTeam> teams;
};

class Match
{
public:
    Match(FootballTeam& l, FootballTeam& v):localTeam(l), visitorTeam(v), localGoals(0), visitorGoals(0){}
    void play();
private:
    int localGoals, visitorGoals;
    FootballTeam& localTeam, visitorTeam;

};

void Match::play()
{
    // first step
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(0, ceil(abs(localTeam.getLevel() - visitorTeam.getLevel()) * 10));
    int k = distr(gen);
    // second step
    std::vector<double> vD{-localTeam.getLevel(), 0, visitorTeam.getLevel()};
    std::vector<int> vI{0, 1, 0};
    std::piecewise_linear_distribution<double> distr2(vD.begin(), vD.end(), vI.begin());
    for (int i = 0; i < k; i++)
    {
        double t = distr2(gen);
        if (t <= 0)
            localGoals++;
        else
            visitorGoals++;
    }
    if (localGoals > visitorGoals)
    {
        localTeam.addWin();
        localTeam.setPoints(3);
        visitorTeam.addLoss();
    }
    else if (visitorGoals > localGoals)
    {
        visitorTeam.addWin();
        visitorTeam.setPoints(3);
        localTeam.addLoss();
    }
    else 
    {
        visitorTeam.addDraw();
        localTeam.addDraw();
        visitorTeam.setPoints(1);
        localTeam.setPoints(1);
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
        teams.push_back({name, city, stadium, level});
    }
}


int main()
{
    const std::string FILENAME = "../../../data/problem2_matches/football.csv";
    std::ifstream inputFile;
    inputFile.open(FILENAME);
    FootballTeam Ft;
    FootballTeam Ft2("n", "c", "s", 25);
    std::vector<FootballTeam> teams;
    loadTeams(inputFile, teams);
    Tournament tour(teams);
    tour.printTeams();
    Match mt(teams[0], teams[1]);
    mt.play();
    return 0;
}
