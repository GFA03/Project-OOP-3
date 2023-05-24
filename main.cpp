#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <list>
#include <random>
#include <unistd.h>

#define RED "\x1b[31m"
#define MAGENTA "\x1b[35m"
#define GOLD "\x1b[33m"
#define BRONZE "\x1b[30m"
#define RESET "\x1b[0m"

//make it abstract!!!
class Card{
    std::string name;

public:
    Card(const std::string& name = "Unknown");
    Card(const Card& obj);
    Card& operator=(const Card& obj);
    friend std::istream& operator>>(std::istream& in, Card& obj);
    friend std::ostream& operator<<(std::ostream& out, const Card& obj);
    virtual ~Card() = default;

    std::string getName() const { return name; }
};

Card::Card(const std::string& name): name(name){}

Card::Card(const Card& obj): name(obj.name) {}

Card& Card::operator=(const Card& obj)
{
    if(this != &obj)
    {
        this->name = obj.name;
    }
    return *this;
}

std::istream& operator>>(std::istream& in, Card& obj)
{
    std::cout << "Insert name: ";
    std::getline(in, obj.name);
    return in;
}

std::ostream& operator<<(std::ostream& out, const Card& obj)
{
    out << "Name: " << RESET << obj.name << '\n';
    return out;
}

class PlayerCard: public Card
{
    static int playerCount;
    const int playerId;
    int attackOVR;
    int defenseOVR;
    int quickSellValue;

public:
    PlayerCard(std::string name = "Unknown", int attackOVR = 0, int defenseOVR = 0, int quickSellValue = 0);
    PlayerCard(const PlayerCard& obj);
    PlayerCard& operator=(const PlayerCard& obj);
    friend std::istream& operator>>(std::istream& in, PlayerCard& obj);
    friend std::ostream& operator<<(std::ostream& out, const PlayerCard& obj);
    ~PlayerCard() = default;

    int getAttackOVR() const { return attackOVR; }
    int getDefenseOVR() const { return defenseOVR; }
    const int getPlayerId() const;
    int calcOVR() const;
};

int PlayerCard::playerCount = 1;

PlayerCard::PlayerCard(std::string name, int attackOVR, int defenseOVR, int quickSellValue) : Card(name), playerId(playerCount++)
{
    this->attackOVR = attackOVR;
    this->defenseOVR = defenseOVR;
    this->quickSellValue = quickSellValue;
}

PlayerCard::PlayerCard(const PlayerCard& obj) : Card(obj), playerId(playerCount++)
{
    this->attackOVR = obj.attackOVR;
    this->defenseOVR = obj.defenseOVR;
    this->quickSellValue = quickSellValue;
}

PlayerCard& PlayerCard::operator=(const PlayerCard& obj)
{
    if(this != &obj)
    {
        Card::operator=(obj);
        this->attackOVR = obj.attackOVR;
        this->defenseOVR = obj.defenseOVR;
        this->quickSellValue = quickSellValue;
    }
    return *this;
}

bool isValidInteger(const std::string& str) {
    std::istringstream iss(str);
    int number;
    return (iss >> number) && (iss.eof());
}

int readInt(std::istream& in)
{
    std::string input;
    while(true)
    {
        in >> input;
        if(isValidInteger(input))
        {
            return std::stoi(input);
        }
        std::cout << "Please give an integer!\n";   
    }
}

std::istream& operator>>(std::istream& in, PlayerCard& obj)
{
    // in >> static_cast<Card&>(obj);
    in >> (Card&)obj;
    std::cout << "Insert attack overall: ";
    obj.attackOVR = readInt(in);

    std::cout << "Insert defense overall: ";
    obj.defenseOVR = readInt(in);

    std::cout << "Insert quick sell value: ";
    obj.quickSellValue = readInt(in);

    return in;
}

std::ostream& operator<<(std::ostream& out, const PlayerCard& obj)
{
    std::string color = "";
    int ovr = std::max(obj.attackOVR, obj.defenseOVR);
    if(ovr >= 85)
        color = MAGENTA;
    else if(ovr >= 75)
        color = GOLD;
    else if(ovr <= 65)
        color = BRONZE;
    out << color << "Player id: " << RESET << obj.playerId << '\n';
    out << color << (Card&)obj;
    out << color << "Attack overall: " << RESET << obj.attackOVR << '\n';
    out << color << "Defense overall: " << RESET << obj.defenseOVR << '\n';
    out << color << "Quick sell value: " << RESET << obj.quickSellValue << '\n';
    return out;
}

const int PlayerCard::getPlayerId() const{return this->playerId;}

int PlayerCard::calcOVR() const{return std::max(this->attackOVR, this->defenseOVR);}

class UseCard : public Card
{
    int duration;

public:
    UseCard(std::string name = "Unknown", int duration = 0);
    UseCard(const UseCard& obj);
    UseCard& operator=(const UseCard& obj);
    friend std::istream& operator>>(std::istream& in, UseCard& obj);
    friend std::ostream& operator<<(std::ostream& out, const UseCard& obj);
    virtual ~UseCard() = default;
};

UseCard::UseCard(std::string name, int duration) : Card(name)
{
    this->duration = duration;
}

UseCard::UseCard(const UseCard& obj) : Card(obj)
{
    this->duration = obj.duration;
}

UseCard& UseCard::operator=(const UseCard& obj)
{
    if(this != &obj)
    {
        Card::operator=(obj);
        this->duration = obj.duration;
    }
    return *this;
}

std::istream& operator>>(std::istream& in, UseCard& obj)
{
    in >> (Card&)obj;
    std::cout << "Insert duration: ";
    obj.duration = readInt(in);
    return in;
}

std::ostream& operator<<(std::ostream& out, const UseCard& obj)
{
    out << (Card&)obj;
    out << "Duration: " << obj.duration << '\n';
    return out;
}

class TeamUseCard : public UseCard
{
    int statsBoost;
    std::string type;

public:
    TeamUseCard(std::string name = "Unknown", int duration = 0, int statsBoost = 0, std::string type = "None");
    TeamUseCard(const TeamUseCard& obj);
    TeamUseCard& operator=(const TeamUseCard& obj);
    friend std::istream& operator>>(std::istream& in, TeamUseCard& obj);
    friend std::ostream& operator<<(std::ostream& out, const TeamUseCard& obj);
    ~TeamUseCard() = default;
};

TeamUseCard::TeamUseCard(std::string name, int duration, int statsBoost, std::string type) : UseCard(name, duration)
{
    this->statsBoost = statsBoost;
    this->type = type;
}

TeamUseCard::TeamUseCard(const TeamUseCard& obj) : UseCard(obj)
{
    this->statsBoost = obj.statsBoost;
    this->type = obj.type;
}

TeamUseCard& TeamUseCard::operator=(const TeamUseCard& obj)
{
    if(this != &obj)
    {
        UseCard::operator=(obj);
        this->statsBoost = obj.statsBoost;
        this->type = obj.type;
    }
    return *this;
}

std::istream& operator>>(std::istream& in, TeamUseCard& obj)
{
    in >> (UseCard&)obj;
    std::cout << "Insert stats boost: ";
    obj.statsBoost = readInt(in);
    std::cout << "Insert type: ";
    std::getline(in, obj.type);
    return in;
}

std::ostream& operator<<(std::ostream& out, const TeamUseCard& obj)
{
    out << (UseCard&)obj;
    out << "Stats boost: " << obj.statsBoost << '\n';
    out << "Type: " << obj.type << '\n';
    return out;
}

class Database
{
    static Database* instance;
    // std::vector<const PlayerCard*> specialPlayers;
    std::vector<const PlayerCard*> goldPlayers;
    std::vector<const PlayerCard*> silverPlayers;
    std::vector<const PlayerCard*> bronzePlayers;
    Database(std::vector<const PlayerCard*> specialPlayers = {}, std::vector<const PlayerCard*> goldPlayers = {}, 
            std::vector<const PlayerCard*> silverPlayers = {}, std::vector<const PlayerCard*> bronzePlayers = {});
    Database(const Database& obj) = delete;
    Database& operator=(const Database& obj) = delete;
    void addPlayer(const PlayerCard& player);
    void readPlayers();
public:
    std::vector<const PlayerCard*> specialPlayers;
    static Database* getInstance();
    const std::vector<const PlayerCard*> getSpecialPlayers() const;
    const std::vector<const PlayerCard*> getGoldPlayers() const;
    const std::vector<const PlayerCard*> getSilverPlayers() const;
    const std::vector<const PlayerCard*> getBronzePlayers() const;
};

Database* Database::instance = nullptr;

Database::Database(std::vector<const PlayerCard*> specialPlayers, std::vector<const PlayerCard*> goldPlayers, 
            std::vector<const PlayerCard*> silverPlayers, std::vector<const PlayerCard*> bronzePlayers)
{
    this->specialPlayers = specialPlayers;
    this->goldPlayers = goldPlayers;
    this->silverPlayers = silverPlayers;
    this->bronzePlayers = bronzePlayers;
}

Database* Database::getInstance()
{
    if(instance == nullptr){
        instance = new Database();
        instance->readPlayers();
    }
    return instance;
}

void Database::addPlayer(const PlayerCard& player)
{
    int ovr = player.calcOVR();
    if(ovr >= 85)
        this->specialPlayers.push_back(&player);
    else if(ovr >= 75)
        this->goldPlayers.push_back(&player);
    else if(ovr >= 65)
        this->silverPlayers.push_back(&player);
    else
        this->bronzePlayers.push_back(&player);
}



std::vector<std::string> lineRead(std::string line)
{
    std::vector<std::string> temp;
    std::string tempString = "";
    int index;
    while(line.find(',') != -1)
    {
        index = line.find(',');
        temp.push_back(line.substr(0, index));
        line.erase(0, index + 1);
    }
    temp.push_back(line);
    return temp;
}
// 6-15 ATTACK OVR
// 16-22 DEFENSE OVR
std::pair<int, int> findBestOVR(std::vector<std::string>& playerStats)
{
    int maxAttackOVR = 0, maxDefenseOVR = 0;
    for(int i = 6; i <= 22; ++i)
    {
        if(i <= 15)
        {
            //attack
            if(stoi(playerStats[i]) > maxAttackOVR)
                maxAttackOVR = stoi(playerStats[i]);
        }
        else
        {
            // defense
            if(stoi(playerStats[i]) > maxDefenseOVR)
                maxDefenseOVR = stoi(playerStats[i]);
        }
    }
    std::pair<int, int> maxOvrs = {maxAttackOVR, maxDefenseOVR};
    return maxOvrs;
}


//reading from .csv file the players stats
void Database::readPlayers()
{
    std::ifstream fin("players_fifa23_cleaned.csv");
    std::string x;
    std::vector<std::string> playerStats;
    int attackOVR, defenseOVR;
    std::getline(fin, x);
    int i = 0;
    PlayerCard* tempPlayer;
    while(std::getline(fin, x))
    {
        playerStats = lineRead(x);
        // std::cout << findBestOVR(playerStats).first << " " << findBestOVR(playerStats).second <<  " " << playerStats[23] << '\n';
        tempPlayer = new PlayerCard(playerStats[0], findBestOVR(playerStats).first, findBestOVR(playerStats).second, stoi(playerStats[23]));
        this->addPlayer(*tempPlayer);
    }
    fin.close();
}

const std::vector<const PlayerCard*> Database::getSpecialPlayers() const{return this->specialPlayers;}

const std::vector<const PlayerCard*> Database::getGoldPlayers() const{return this->goldPlayers;}

const std::vector<const PlayerCard*> Database::getSilverPlayers() const{return this->silverPlayers;}

const std::vector<const PlayerCard*> Database::getBronzePlayers() const{return this->bronzePlayers;}

class Club
{
    std::unordered_map<int, const PlayerCard*> allPlayers;
    std::vector<const PlayerCard*> lineup;
    std::vector<const UseCard*> useCards;
    int budget;
public:
    Club(std::unordered_map<int, const PlayerCard*> allPlayers = {}, std::vector<const PlayerCard*> lineup = {}, 
    std::vector<const UseCard*> useCards = {}, int budget = 0);
    Club(const Club& obj);
    Club& operator=(const Club& obj);
    friend std::istream& operator>>(std::istream& in, Club& obj);
    friend std::ostream& operator<<(std::ostream& out, const Club& obj);
    ~Club() = default;

    int getBudget() const { return budget; }
    void addPlayer(const PlayerCard* player);
    void addToLineup(int playerId);
    void removeFromLineup(int playerId);
};

Club::Club(std::unordered_map<int, const PlayerCard*> allPlayers, std::vector<const PlayerCard*> lineup, 
    std::vector<const UseCard*> useCards, int budget)
{
    this->allPlayers = allPlayers;
    this->lineup = lineup;
    this->useCards = useCards;
    this->budget = budget;
}

Club::Club(const Club& obj)
{
    this->allPlayers = obj.allPlayers;
    this->lineup = obj.lineup;
    this->useCards = obj.useCards;
    this->budget = obj.budget;
}

Club& Club::operator=(const Club& obj)
{
    if(this != &obj)
    {
        this->allPlayers = obj.allPlayers;
        this->lineup = obj.lineup;
        this->useCards = obj.useCards;
        this->budget = obj.budget;
    }
    return *this;
}

std::istream& operator>>(std::istream& in, Club& obj)
{
    std::cout << "Insert number of players: ";
    int allPlayersNr;
    allPlayersNr = readInt(in);
    PlayerCard* tempPlayer;
    for(int i = 0; i < allPlayersNr; i++)
    {
        tempPlayer = new PlayerCard();
        in >> *tempPlayer;
        obj.allPlayers[tempPlayer->getPlayerId()] = tempPlayer;
    }
    std::cout << "Insert budget: ";
    obj.budget = readInt(in);
    return in;
}

std::ostream& operator<<(std::ostream& out, const Club& obj)
{
    out << "Players in club:\n";
    for(auto player: obj.allPlayers)
        out << player.second << '\n';
    out << "Lineup:\n";
    for(int i = 0; i < obj.lineup.size(); i++)
        out << obj.lineup[i] << '\n';
    out << "Use cards in club:\n";
    for(int i = 0; i < obj.useCards.size(); i++)
        out << obj.useCards[i] << '\n';
    out << "Budget: " << obj.budget << '\n';
    return out;
}

void Club::addPlayer(const PlayerCard* player)
{
    if(this->allPlayers.find(player->getPlayerId()) == this->allPlayers.end())
    {
        std::cout << "Player is already in this club!\n";
        return;
    }
    this->allPlayers[player->getPlayerId()] = player;
    std::cout << "Player has been added successfully!\n";
}

void Club::addToLineup(int playerId)
{
    if(this->lineup.size() == 11)
    {
        std::cout << "Lineup full!\n";
        return;
    }
    if(this->allPlayers.find(playerId) == this->allPlayers.end())
    {
        std::cout << "Player is not in the club!\n";
        return;
    }
    for(int i = 0; i < this->lineup.size(); ++i)
        if(this->lineup[i]->getPlayerId() == playerId)
        {
            std::cout << "Player is already in the lineup!\n";
            return;
        }
    this->lineup.push_back(this->allPlayers[playerId]);
    std::cout << "Player has been added to the lineup!\n";
}

void Club::removeFromLineup(int playerId)
{
    for(int i = 0; i < this->lineup.size(); ++i)
        if(this->lineup[i]->getPlayerId() == playerId)
        {
            this->lineup.erase(this->lineup.begin() + i);
            std::cout << "Player has been removed from the lineup!\n";
            return;
        }
    std::cout << "Player has not been found!\n";
}

class Game
{
    std::vector<const PlayerCard*> lineup1;
    std::vector<const PlayerCard*> lineup2;
    int homeScore;
    int awayScore;
    int minute;
public:
    Game(std::vector<const PlayerCard*> lineup1 = {}, std::vector<const PlayerCard*> lineup2 = {},
        int homeScore = 0, int awayScore = 0, int minute = 0);
    Game(const Game& obj);
    Game& operator=(const Game& obj);
    friend std::istream& operator>>(std::istream& in, Game& obj);
    friend std::ostream& operator<<(std::ostream& out, const Game& obj);
    ~Game() = default;

    int getTeamAttackOVR(std::vector<const PlayerCard*> lineup);
    int getTeamDefenseOVR(std::vector<const PlayerCard*> lineup);
    std::string eventDraw(int team, int event, int minute);
    static std::string getSpaces(std::string temp);
    void setLineups(std::vector<const PlayerCard*> lineup1, std::vector<const PlayerCard*> lineup2);
    int simGame(); //returns 0 = team1 victory, 1 = draw, 2 = team2 victory
};

Game::Game(std::vector<const PlayerCard*> lineup1, std::vector<const PlayerCard*> lineup2, int homeScore, int awayScore,
            int minute)
{
    this->lineup1 = lineup1;
    this->lineup2 = lineup2;
    this->homeScore = homeScore;
    this->awayScore = awayScore;
    this->minute = minute;
}

Game::Game(const Game& obj)
{
    this->lineup1 = obj.lineup1;
    this->lineup2 = obj.lineup2;
    this->homeScore = obj.homeScore;
    this->awayScore = obj.awayScore;
    this->minute = obj.minute;
}

Game& Game::operator=(const Game& obj)
{
    if(this != &obj)
    {
        this->lineup1 = obj.lineup1;
        this->lineup2 = obj.lineup2;
        this->homeScore = obj.homeScore;
        this->awayScore = obj.awayScore;
        this->minute = obj.minute;
    }
    return *this;
}

int Game::getTeamAttackOVR(std::vector<const PlayerCard*> lineup)
{
    int total = 0;
    for(int i = 0; i < lineup.size(); i++)
        total += lineup[i]->getAttackOVR();
    return total;
}

int Game::getTeamDefenseOVR(std::vector<const PlayerCard*> lineup)
{
    int total = 0;
    for(int i = 0; i < lineup.size(); i++)
        total += lineup[i]->getDefenseOVR();
    return total;
}

std::string Game::getSpaces(std::string temp){
    std::string result = "";
    for(int i = 0; i < 50 - temp.length(); i++)
        result += " ";
    return result;
}

std::ostream& operator<<(std::ostream& out, const Game& obj)
{
    out << "Your lineup:" << Game::getSpaces("Your lineup:") << "Opponent lineup:\n";
    for(int i = 0; i < obj.lineup1.size(); i++)
        out << obj.lineup1[i]->getName() << Game::getSpaces(obj.lineup1[i]->getName()) << obj.lineup2[i]->getName() <<'\n';
    out << "\n";
    return out;
}

void Game::setLineups(std::vector<const PlayerCard*> lineup1, std::vector<const PlayerCard*> lineup2)
{
    this->lineup1 = lineup1;
    this->lineup2 = lineup2;
}

void gameDraw(int scoreHome, int scoreAway)
{
    std::cout << "\tYour Team\t" << scoreHome << " - " << scoreAway << "\t\tOpponent\n";

}

std::string Game::eventDraw(int team, int event, int minute) // 1 - Team 1, 2 - Team 2 || 1 - GOAL, 2 - YELLOW CARD, 3 - RED CARD
{
    std::string temp = "|";
    std::random_device rd;
    std::mt19937 rng(rd());
    int teamSize;
    if(team == 1)
        teamSize = this->lineup1.size() - 1;
    else
        teamSize = this->lineup2.size() - 1;
    std::uniform_int_distribution<int> playerNr(0, teamSize);
    int player = playerNr(rng);
    if(team < 1 || team > 2) return "";
    if(event > 3 || event < 1) return "";
    if(team == 1)
    {
        temp += std::to_string(minute);
        if(event == 1)
        {
            this->homeScore++;
            temp += " - GOAL: ";
        }else if(event == 2)
        {
            temp += " - YELLOW CARD: ";
        }
        else{
            temp += " - RED CARD: ";
        }
        temp += this->lineup1[player]->getName();
        if(event == 3)
            this->lineup1.erase(this->lineup1.begin() + player);
    }
    else if(team == 2)
    {   
        temp += "                                   ";
        temp += std::to_string(minute);
        if(event == 1)
        {
            this->awayScore++;
            temp += " - GOAL: ";
        }else if(event == 2)
        {
            temp += " - YELLOW CARD: ";
        }
        else{
            temp += " - RED CARD: ";
        }
        temp += this->lineup2[player]->getName();
        if(event == 3)
            this->lineup1.erase(this->lineup1.begin() + player);
    }
    for(int i = temp.length(); i < 68; i++)
        temp += " ";
    temp += "|\n";
    return temp;
}
// you need to keep in mind the 2 yellow cards add to a red card
int Game::simGame()
{
    if(this->lineup1.size() != 11 && this->lineup2.size() != 11)
        return -1;
    std::random_device rd;
    std::mt19937 rng(rd());
    // std::uniform_int_distribution<double> eventType(0.0, 1.0);
    std::string events = "";
    // calc chances (team1 attack and defense ovr and then for team2)
    double scoringChancesForTeamA;
    double scoringChancesForTeamB;
    while(minute < 93)
    {
        scoringChancesForTeamA = 1.0 / (1.0 + std::exp((this->getTeamDefenseOVR(lineup2) - this->getTeamAttackOVR(lineup1)) / 10.0));
        scoringChancesForTeamB = 1.0 / (1.0 + std::exp((this->getTeamDefenseOVR(lineup1) - this->getTeamAttackOVR(lineup2)) / 10.0));
        system("clear");
        std::cout << "+-------------------------------------------------------------------+\n";
        std::cout << "|           YOUR TEAM           " << this->homeScore << " - " << this->awayScore << "            OPPONENT TEAM      |" << std::endl;
        std::cout << "|                                " << this->minute << "                                 |\n";
        std::cout << "+-------------------------------------------------------------------+\n";
        if(minute == 23)
            events += this->eventDraw(2, 1, this->minute);
        std::cout << events;
        if(events.length() > 1)
            std::cout << "+-------------------------------------------------------------------+\n";
        // usleep(100000);
        this->minute++;
    }
    return 0;
}

int main() {
    // TeamUseCard d("sadsada", 15, 20, "Attack"), e(d);
    Database* d;
    d = d->getInstance();
    // std::unordered_map<int, const PlayerCard*> team1;
    // std::unordered_map<int, const PlayerCard*> team2;
    // std::vector<const PlayerCard*> lineup1;
    // std::vector<const PlayerCard*> lineup2;
    // for(int i = 0; i < 22; i++)
    // {
    //     if(i < 11){
    //         team1[i] = d->specialPlayers[i];
    //         lineup1.push_back(team1[i]);
    //     }
    //     else{
    //         team2[i-11] = d->specialPlayers[i];
    //         lineup2.push_back(team2[i-11]);
    //     }
    // }
    // Club club1(team1, lineup1), club2(team2, lineup2);
    // Game game1(lineup1, lineup2);
    // std::cout << "Lineup 1: " << game1.getTeamAttackOVR(lineup1) << " " << game1.getTeamDefenseOVR(lineup1) << '\n';
    // std::cout << "Lineup 2: " << game1.getTeamAttackOVR(lineup2) << " " << game1.getTeamDefenseOVR(lineup2) << '\n';
    // std::cout << chancesForTeamA << '\n';
    // std::cout << chancesForTeamB << '\n';
    // std::cout << game1;
    // game1.simGame();
    return 0;
}
// NU UITA SA SCOTI DIN CLASA DATABASE VECTORUL DE LA PUBLIC
// testeaza daca red card-ul iti va scoate playerul din lineup si din Club
