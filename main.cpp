#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <list>

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

    const int getPlayerId();
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
    out << color << (Card&)obj;
    out << color << "Attack overall: " << RESET << obj.attackOVR << '\n';
    out << color << "Defense overall: " << RESET << obj.defenseOVR << '\n';
    out << color << "Quick sell value: " << RESET << obj.quickSellValue << '\n';
    return out;
}

const int PlayerCard::getPlayerId(){return this->playerId;}

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

int main() {
    // readPlayers();
    PlayerCard b("Cristiano Ronaldo", 92, 32, 1900);
    UseCard c("salas", 12);
    // TeamUseCard d("sadsada", 15, 20, "Attack"), e(d);
    // std::cout << b;
    Database* d;
    d = d->getInstance();
    for(int i = 0; i < d->getBronzePlayers().size(); i++)
        std::cout << *d->getBronzePlayers()[i] << '\n';
    return 0;
}

// to do list: trebuie clasa GAME si clasa TEAM 
// de altfel nu ai facut PlayerUseCard