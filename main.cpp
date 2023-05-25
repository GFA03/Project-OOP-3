#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <list>
#include <random>
#include <unistd.h>
#include <typeinfo>
#include <algorithm>
#include <filesystem>
#include <termios.h>
#include <unistd.h>

#define RED "\x1b[31m"
#define MAGENTA "\x1b[35m"
#define GOLD "\x1b[33m"
#define BRONZE "\x1b[30m"
#define RESET "\x1b[0m"


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

class Card{
    std::string name;
protected:
    int quickSellValue;
public:
    Card(const std::string& name = "Unknown", int quickSellValue = 0);
    Card(const Card& obj);
    Card& operator=(const Card& obj);
    friend std::istream& operator>>(std::istream& in, Card& obj);
    friend std::ostream& operator<<(std::ostream& out, const Card& obj);
    virtual ~Card() = default;

    std::string getName() const { return name; }
    virtual int getQuickSellValue() const = 0;
};

Card::Card(const std::string& name, int quickSellValue): name(name), quickSellValue(quickSellValue){}

Card::Card(const Card& obj): name(obj.name), quickSellValue(obj.quickSellValue) {}

Card& Card::operator=(const Card& obj)
{
    if(this != &obj)
    {
        this->name = obj.name;
        this->quickSellValue = obj.quickSellValue;
    }
    return *this;
}

std::istream& operator>>(std::istream& in, Card& obj)
{
    std::cout << "Insert name: ";
    std::getline(in, obj.name);
    std::cout << "Quick sell value: ";
    obj.quickSellValue = readInt(in);
    return in;
}

std::ostream& operator<<(std::ostream& out, const Card& obj)
{
    out << "Name: " << RESET << obj.name << '\n';
    out << "Quick sell value: " << RESET << obj.quickSellValue << '\n';
    return out;
}

class PlayerCard: public Card
{
    static int playerCount;
    const int playerId;
    int attackOVR;
    int defenseOVR;

public:
    PlayerCard(std::string name = "Unknown", int attackOVR = 0, int defenseOVR = 0, int quickSellValue = 0);
    PlayerCard(const PlayerCard& obj);
    PlayerCard& operator=(const PlayerCard& obj);
    friend std::istream& operator>>(std::istream& in, PlayerCard& obj);
    friend std::ostream& operator<<(std::ostream& out, const PlayerCard& obj);
    ~PlayerCard() = default;

    int getAttackOVR() const { return attackOVR; }
    int getDefenseOVR() const { return defenseOVR; }
    void addAttackOVR(int boost) { attackOVR += boost; }
    void addDefenseOVR(int boost) { defenseOVR += boost; }
    const int getPlayerId() const;
    int calcOVR() const;
    int getQuickSellValue() const { return quickSellValue; }
    bool operator<(const PlayerCard& obj) const;
};

int PlayerCard::playerCount = 1;

PlayerCard::PlayerCard(std::string name, int attackOVR, int defenseOVR, int quickSellValue) : Card(name, quickSellValue), playerId(playerCount++)
{
    this->attackOVR = attackOVR;
    this->defenseOVR = defenseOVR;
}

PlayerCard::PlayerCard(const PlayerCard& obj) : Card(obj), playerId(playerCount++)
{
    this->attackOVR = obj.attackOVR;
    this->defenseOVR = obj.defenseOVR;
}

PlayerCard& PlayerCard::operator=(const PlayerCard& obj)
{
    if(this != &obj)
    {
        Card::operator=(obj);
        this->attackOVR = obj.attackOVR;
        this->defenseOVR = obj.defenseOVR;
    }
    return *this;
}

std::istream& operator>>(std::istream& in, PlayerCard& obj)
{
    // in >> static_cast<Card&>(obj);
    in >> (Card&)obj;
    std::cout << "Insert attack overall: ";
    obj.attackOVR = readInt(in);

    std::cout << "Insert defense overall: ";
    obj.defenseOVR = readInt(in);

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
    return out;
}

const int PlayerCard::getPlayerId() const{return this->playerId;}

int PlayerCard::calcOVR() const{return std::max(this->attackOVR, this->defenseOVR);}

bool PlayerCard::operator<(const PlayerCard& obj) const
{
    return (this->calcOVR() < obj.calcOVR());
}

class UseCard :virtual public Card
{
    static int useCardCount;
    const int useCardId;
    int duration;
public:
    UseCard(std::string name = "Unknown", int quickSellValue = 0, int duration = 0);
    UseCard(const UseCard& obj);
    UseCard& operator=(const UseCard& obj);
    friend std::istream& operator>>(std::istream& in, UseCard& obj);
    friend std::ostream& operator<<(std::ostream& out, const UseCard& obj);
    virtual ~UseCard() = default;

    int getDuration() const { return duration; }
    const int getUseCardId() const {return useCardId;}
    virtual void printCard() = 0;
};

int UseCard::useCardCount = 1;

UseCard::UseCard(std::string name, int quickSellValue, int duration) : Card(name, quickSellValue), useCardId(useCardCount++)
{
    this->duration = duration;
}

UseCard::UseCard(const UseCard& obj) : Card(obj), useCardId(useCardCount++)
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
    TeamUseCard(std::string name = "Unknown", int quickSellValue = 0, int duration = 0, int statsBoost = 0, std::string type = "None");
    TeamUseCard(const TeamUseCard& obj);
    TeamUseCard& operator=(const TeamUseCard& obj);
    friend std::istream& operator>>(std::istream& in, TeamUseCard& obj);
    friend std::ostream& operator<<(std::ostream& out, const TeamUseCard& obj);
    ~TeamUseCard() = default;

    int getQuickSellValue() const{return quickSellValue;}
    void printCard();
};

TeamUseCard::TeamUseCard(std::string name, int quickSellValue, int duration, int statsBoost, std::string type) : Card(name, quickSellValue), UseCard(name, quickSellValue, duration)
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

void TeamUseCard::printCard()
{
    std::cout << (UseCard&)(*this);
    std::cout << "Stats boost: " << this->statsBoost << '\n';
    std::cout << "Type: " << this->type << '\n';
}

class PlayerUseCard: public UseCard
{
    int boost;
    PlayerCard* boostedPlayer;
public:
    PlayerUseCard(std::string name = "Unknown", int quickSellValue = 0, int duration = 0, int boost = 0, PlayerCard* boostedPlayer = nullptr);
    PlayerUseCard(const PlayerUseCard& obj);
    PlayerUseCard& operator=(const PlayerUseCard& obj);
    friend std::istream& operator>>(std::istream& in, PlayerUseCard& obj);
    friend std::ostream& operator<<(std::ostream& out, const PlayerUseCard& obj);
    ~PlayerUseCard();

    void printCard();
    int getQuickSellValue() const {return quickSellValue;}
};

PlayerUseCard::PlayerUseCard(std::string name, int quickSellValue, int duration, int boost, PlayerCard* boostedPlayer) : Card(name, quickSellValue), UseCard(name, quickSellValue, duration)
{
    this->boost = boost;
    this->boostedPlayer = boostedPlayer;
}

PlayerUseCard::PlayerUseCard(const PlayerUseCard& obj) : UseCard(obj)
{
    this->boost = obj.boost;
    this->boostedPlayer = obj.boostedPlayer;
}

PlayerUseCard& PlayerUseCard::operator=(const PlayerUseCard& obj)
{
    if(this != &obj)
    {
        UseCard::operator=(obj);
        this->boost = obj.boost;
        this->boostedPlayer = obj.boostedPlayer;
    }
    return *this;
}

std::istream& operator>>(std::istream& in, PlayerUseCard& obj)
{
    in >> (UseCard&)obj;
    in >> *obj.boostedPlayer;
    obj.boost = readInt(in);
    return in;
}

std::ostream& operator<<(std::ostream& out, const PlayerUseCard& obj)
{
    out << (UseCard&)obj;
    if(obj.boostedPlayer != nullptr)
        out << *obj.boostedPlayer << std::endl;
    out << "Boost: " << obj.boost << '\n';
    return out;
}

void PlayerUseCard::printCard()
{
    std::cout << (UseCard&)(*this);
    if(this->boostedPlayer != nullptr)
        std::cout << *this->boostedPlayer << std::endl;
    std::cout << "Boost: " << this->boost << '\n';
}

PlayerUseCard::~PlayerUseCard()
{
    if(this->boostedPlayer != nullptr)
    {
        delete this->boostedPlayer;
        this->boostedPlayer = nullptr;
    }
}

class Database
{
    static bool readOnly;
    static std::vector<const PlayerCard*> specialPlayers;
    static std::vector<const PlayerCard*> goldPlayers;
    static std::vector<const PlayerCard*> silverPlayers;
    static std::vector<const PlayerCard*> bronzePlayers;
    static std::vector<const UseCard*> useCards;
    static void addPlayer(const PlayerCard& player);
public:
    static void readPlayers();
    static void readUseCards();
    static const std::vector<const PlayerCard*> getSpecialPlayers();
    static const std::vector<const PlayerCard*> getGoldPlayers();
    static const std::vector<const PlayerCard*> getSilverPlayers();
    static const std::vector<const PlayerCard*> getBronzePlayers();
    static const UseCard* getUseCard(int id);
    static const std::vector<const UseCard*> getUseCards();
    virtual ~Database() = 0;
};

bool Database::readOnly = false;
std::vector<const PlayerCard*> Database::specialPlayers = {};
std::vector<const PlayerCard*> Database::goldPlayers = {};
std::vector<const PlayerCard*> Database::silverPlayers = {};
std::vector<const PlayerCard*> Database::bronzePlayers = {};
std::vector<const UseCard*> Database::useCards = {};

void Database::addPlayer(const PlayerCard& player)
{
    int ovr = player.calcOVR();
    if(ovr >= 85)
        specialPlayers.push_back(&player);
    else if(ovr >= 75)
        goldPlayers.push_back(&player);
    else if(ovr >= 65)
        silverPlayers.push_back(&player);
    else
        bronzePlayers.push_back(&player);
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
    if(readOnly == true)
        return;
    readOnly = true;
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
        addPlayer(*tempPlayer);
    }
    fin.close();
}

void Database::readUseCards()
{
    UseCard* tempUseCard;
    tempUseCard = new TeamUseCard("Special attack", 1200, 5, 100, "attack");
    useCards.push_back(tempUseCard);
    tempUseCard = new TeamUseCard("Special defense", 1200, 5, 100, "defense");
    useCards.push_back(tempUseCard);
    tempUseCard = new TeamUseCard("Gold attack", 800, 3, 50, "attack");
    useCards.push_back(tempUseCard);
    tempUseCard = new TeamUseCard("Gold defense", 800, 3, 50, "defense");
    useCards.push_back(tempUseCard);
    tempUseCard = new TeamUseCard("Silver attack", 400, 1, 25, "attack");
    useCards.push_back(tempUseCard);
    tempUseCard = new TeamUseCard("Silver attack", 400, 1, 25, "defense");
    useCards.push_back(tempUseCard);
    tempUseCard = new PlayerUseCard("Special attack", 1000, 10, 10);
    useCards.push_back(tempUseCard);
    tempUseCard = new PlayerUseCard("Special defense", 1000, 10, 10);
    useCards.push_back(tempUseCard);
}

const std::vector<const PlayerCard*> Database::getSpecialPlayers(){return specialPlayers;}

const std::vector<const PlayerCard*> Database::getGoldPlayers(){return goldPlayers;}

const std::vector<const PlayerCard*> Database::getSilverPlayers(){return silverPlayers;}

const std::vector<const PlayerCard*> Database::getBronzePlayers(){return bronzePlayers;}

const std::vector<const UseCard*> Database::getUseCards(){return useCards;}

const UseCard* Database::getUseCard(int id)
{
    for(int i = 0; i < useCards.size(); i++)
    {
        if(useCards[i]->getUseCardId() == id)
            return useCards[i];
    }
    return nullptr;
}

class Game;

class Club
{
    std::string name;
    std::unordered_map<int, const PlayerCard*> allPlayers;
    std::vector<const PlayerCard*> lineup;
    std::vector<const UseCard*> useCards;
    int budget;
public:
    Club(std::string name = "Unknown", std::unordered_map<int, const PlayerCard*> allPlayers = {}, std::vector<const PlayerCard*> lineup = {}, 
    std::vector<const UseCard*> useCards = {}, int budget = 0);
    Club(const Club& obj);
    Club& operator=(const Club& obj);
    friend std::istream& operator>>(std::istream& in, Club& obj);
    friend std::ostream& operator<<(std::ostream& out, const Club& obj);
    ~Club() = default;

    void showLineup();
    void showAllPlayers();
    std::string getName() const { return name; }
    bool checkPlayerInClub(int playerId);
    const std::unordered_map<int, const PlayerCard*> getAllPlayers() const { return allPlayers; }
    const std::vector<const PlayerCard*> getLineup() const {return lineup;}
    const std::vector<const UseCard*> getUseCards() const {return useCards;}
    int getBudget() const { return budget; }
    void setBudget(int budget) { this->budget = budget;}
    int getNumTeamUseCards() const;
    int getNumPlayerUseCards() const;
    bool addPlayer(const PlayerCard* player);
    bool addToLineup(int playerId);
    bool addUseCard(const UseCard* useCard);
    bool removeFromLineup(int playerId);
    bool removeFromTeam(int playerId);
    bool findPlayerUseCard(int id);
    bool findTeamUseCard(int id);

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    Club operator +(int); // increases top speed
    friend Club operator+(int, Club);
};

Club::Club(std::string name, std::unordered_map<int, const PlayerCard*> allPlayers, std::vector<const PlayerCard*> lineup, 
    std::vector<const UseCard*> useCards, int budget)
{
    this->name = name;
    this->allPlayers = allPlayers;
    this->lineup = lineup;
    this->useCards = useCards;
    this->budget = budget;
}

Club::Club(const Club& obj)
{
    this->name = obj.name;
    this->allPlayers = obj.allPlayers;
    this->lineup = obj.lineup;
    this->useCards = obj.useCards;
    this->budget = obj.budget;
}

Club& Club::operator=(const Club& obj)
{
    if(this != &obj)
    {
        this->name = obj.name;
        this->allPlayers = obj.allPlayers;
        this->lineup = obj.lineup;
        this->useCards = obj.useCards;
        this->budget = obj.budget;
    }
    return *this;
}

std::istream& operator>>(std::istream& in, Club& obj)
{
    std::cout << "Insert club's name (No more than 30 characters): ";
    while(true)
    {
        try{
            std::getline(in, obj.name);
            if(obj.name.length() > 30)
                throw std::invalid_argument("Club's name must be at most 30 characters!\n");
            break;
        }
        catch(const std::invalid_argument& e)
        {
            std::cerr << e.what() << "Try again!\n";
        }
    }
    // std::cout << "Insert number of players: ";
    // int allPlayersNr;
    // allPlayersNr = readInt(in);
    // PlayerCard* tempPlayer;
    // for(int i = 0; i < allPlayersNr; i++)
    // {
    //     tempPlayer = new PlayerCard();
    //     in >> *tempPlayer;
    //     obj.allPlayers[tempPlayer->getPlayerId()] = tempPlayer;
    // }
    // std::cout << "Insert budget: ";
    // obj.budget = readInt(in);
    obj.budget = 5000;
    // obj.budget = 200000;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Club& obj)
{
    out << "Club's name: " << obj.name << '\n';
    out << "Players in club: "; 
    if(!obj.allPlayers.empty())
        std::cout << obj.allPlayers.size() << '\n';
    else
        std::cout << "No players in club!\n";
    out << "Lineup: ";
    if(!obj.lineup.empty())
        std::cout << obj.lineup.size() << '\n';
    else
        std::cout << "No players yet!\n";
    out << "Use cards in club:\n";
    if(!obj.useCards.empty())
        for(int i = 0; i < obj.useCards.size(); i++)
            out << obj.useCards[i] << '\n';
    else
        std::cout << "\tNo use cards yet!\n";
    out << "Budget: " << obj.budget << '\n';
    return out;
}

void Club::showAllPlayers()
{
    for(auto player: this->allPlayers)
        std::cout << *player.second << '\n';
}

bool Club::addPlayer(const PlayerCard* player)
{
    if(this->allPlayers.find(player->getPlayerId()) != this->allPlayers.end())
    {
        std::cout << "Player is already in this club!\n";
        return 0;
    }
    this->allPlayers[player->getPlayerId()] = player;
    std::cout << "Player has been added successfully!\n";
    return 1;
}

bool Club::addToLineup(int playerId)
{
    if(this->lineup.size() == 11)
    {
        std::cout << "Lineup full!\n";
        return 0;
    }
    if(this->allPlayers.find(playerId) == this->allPlayers.end())
    {
        std::cout << "Player is not in the club!\n";
        return 0;
    }
    for(int i = 0; i < this->lineup.size(); ++i)
        if(this->lineup[i]->getPlayerId() == playerId)
        {
            std::cout << "Player is already in the lineup!\n";
            return 0;
        }
    this->lineup.push_back(this->allPlayers[playerId]);
    std::cout << "Player has been added to the lineup!\n";
    return 1;
}

bool Club::removeFromLineup(int playerId)
{
    for(int i = 0; i < this->lineup.size(); ++i)
        if(this->lineup[i]->getPlayerId() == playerId)
        {
            this->lineup.erase(this->lineup.begin() + i);
            std::cout << "Player has been removed from the lineup!\n";
            return 1;
        }
    std::cout << "Player has not been found!\n";
    return 0;
}

bool Club::removeFromTeam(int playerId)
{
    try{
        this->allPlayers.erase(playerId);
        std::cout << "Player has been reemoved from the team!\n";
        return 1;
    }
    catch(...)
    {
        std::cout << "Players has not been removed!\n";
    }
    return 0;
}

bool Club::checkPlayerInClub(int playerId)
{
    if(this->allPlayers.find(playerId) != this->allPlayers.end())
        return true;
    return false;
}

bool Club::addUseCard(const UseCard* useCard)
{
    for(int i = 0; i < this->useCards.size(); ++i)
    {
        if(useCard == this->useCards[i])
            return false;
    }
    this->useCards.push_back(useCard);
    return true;
}

int Club::getNumTeamUseCards() const
{
    int cnt = 0;
    for(int i = 0; i < this->useCards.size(); ++i)
        if(dynamic_cast<const TeamUseCard*>(this->useCards[i]) != nullptr)
            cnt++;
    return cnt;
}

int Club::getNumPlayerUseCards() const
{
    int cnt = 0;
    for(int i = 0; i < this->useCards.size(); ++i)
        if(dynamic_cast<const PlayerUseCard*>(this->useCards[i]) != nullptr)
            cnt++;
    return cnt;
}

bool Club::findPlayerUseCard(int id)
{
    for(int i = 0; i < useCards.size(); ++i)
        if(useCards[i]->getUseCardId() == id && dynamic_cast<const PlayerUseCard*>(useCards[i]) != nullptr)
            return true;
    return false;
}

bool Club::findTeamUseCard(int id)
{
    for(int i = 0; i < useCards.size(); ++i)
        if(useCards[i]->getUseCardId() == id && dynamic_cast<const TeamUseCard*>(useCards[i]) != nullptr)
            return true;
    return false;
}

void Club::saveToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cout << "Error in opening the file." << std::endl;
        return;
    }

    // Save the data members to the file
    size_t nameLength = name.length();
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    file.write(name.c_str(), nameLength * sizeof(char));

    size_t numAllPlayers = allPlayers.size();
    file.write(reinterpret_cast<const char*>(&numAllPlayers), sizeof(numAllPlayers));

    for (const auto& pair : allPlayers) {
        int playerId = pair.first;
        file.write(reinterpret_cast<const char*>(&playerId), sizeof(playerId));

        const PlayerCard* playerCard = pair.second;
        // Write the serialized representation of the PlayerCard object
        file.write(reinterpret_cast<const char*>(playerCard), sizeof(PlayerCard));
    }

    size_t numLineup = lineup.size();
    file.write(reinterpret_cast<const char*>(&numLineup), sizeof(numLineup));
    file.write(reinterpret_cast<const char*>(lineup.data()), numLineup * sizeof(const PlayerCard*));

    size_t numUseCards = useCards.size();
    file.write(reinterpret_cast<const char*>(&numUseCards), sizeof(numUseCards));
    file.write(reinterpret_cast<const char*>(useCards.data()), numUseCards * sizeof(const UseCard*));

    file.write(reinterpret_cast<const char*>(&budget), sizeof(budget));

    file.close();
    }

    // Deserialization function to read the Club object from a file
void Club::loadFromFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            std::cout << "File not found or error in opening the file." << std::endl;
            return;
        }

        // Clear the existing data
        name.clear();
        allPlayers.clear();
        lineup.clear();
        useCards.clear();

        // Read the data members from the file
        size_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

        char* nameBuffer = new char[nameLength + 1]; // +1 for null terminator
        file.read(nameBuffer, nameLength * sizeof(char));
        nameBuffer[nameLength] = '\0';
        name = nameBuffer;
        delete[] nameBuffer;

        size_t numAllPlayers;
        file.read(reinterpret_cast<char*>(&numAllPlayers), sizeof(numAllPlayers));

        for (size_t i = 0; i < numAllPlayers; ++i) {
            int playerId;
            file.read(reinterpret_cast<char*>(&playerId), sizeof(playerId));

            PlayerCard* playerCard = new PlayerCard();
            // Read the serialized representation of the PlayerCard object
            file.read(reinterpret_cast<char*>(playerCard), sizeof(PlayerCard));

            allPlayers[playerId] = playerCard;
        }

        size_t numLineup;
        file.read(reinterpret_cast<char*>(&numLineup), sizeof(numLineup));
        lineup.resize(numLineup);
        file.read(reinterpret_cast<char*>(lineup.data()), numLineup * sizeof(const PlayerCard*));

        size_t numUseCards;
        file.read(reinterpret_cast<char*>(&numUseCards), sizeof(numUseCards));
        useCards.resize(numUseCards);
        file.read(reinterpret_cast<char*>(useCards.data()), numUseCards * sizeof(const UseCard*));

        file.read(reinterpret_cast<char*>(&budget), sizeof(budget));

        file.close();
    }

class Game
{
    std::vector<const PlayerCard*> lineup1;
    std::vector<const PlayerCard*> lineup2;
    int homeScore;
    int awayScore;
    int minute;
    std::unordered_map<int, int> yellowCards;
public:
    Game(std::vector<const PlayerCard*> lineup1 = {}, std::vector<const PlayerCard*> lineup2 = {},
        int homeScore = 0, int awayScore = 0, int minute = 0, std::unordered_map<int, int> yellowCards = {});
    Game(const Game& obj);
    Game& operator=(const Game& obj);
    friend std::istream& operator>>(std::istream& in, Game& obj);
    friend std::ostream& operator<<(std::ostream& out, const Game& obj);
    ~Game() = default;

    static int getTeamAttackOVR(std::vector<const PlayerCard*> lineup);
    static int getTeamDefenseOVR(std::vector<const PlayerCard*> lineup);
    std::string eventDraw(int team, int event);
    static std::string getSpaces(std::string temp);
    void setLineups(std::vector<const PlayerCard*> lineup1, std::vector<const PlayerCard*> lineup2);
    int simGame(); //returns 0 = team1 victory, 1 = draw, 2 = team2 victory
};

Game::Game(std::vector<const PlayerCard*> lineup1, std::vector<const PlayerCard*> lineup2, int homeScore, int awayScore,
            int minute, std::unordered_map<int, int> yellowCards)
{
    this->lineup1 = lineup1;
    this->lineup2 = lineup2;
    this->homeScore = homeScore;
    this->awayScore = awayScore;
    this->minute = minute;
    this->yellowCards = yellowCards;
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


void Club::showLineup()
{
    if(!this->lineup.empty()){
        for(int i = 0; i < this->lineup.size(); i++)
            std::cout << *this->lineup[i] << '\n';
        std::cout << "Total attack OVR: " << Game::getTeamAttackOVR(this->lineup) << '\n';
        std::cout << "Total defense OVR: " << Game::getTeamDefenseOVR(this->lineup) << '\n';
    }
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

std::string Game::eventDraw(int team, int event) // 1 - Team 1, 2 - Team 2 || 1 - GOAL, 2 - YELLOW CARD, 3 - RED CARD
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
            temp += " - GOAL: ";
        }else if(event == 2)
        {
            temp += " - YELLOW CARD: ";
            this->yellowCards[this->lineup1[player]->getPlayerId()]++;
        }
        if(event == 3 || this->yellowCards[this->lineup1[player]->getPlayerId()] == 2){
            temp += " - RED CARD: ";
        }
        temp += this->lineup1[player]->getName();
        if(event == 3 || this->yellowCards[this->lineup1[player]->getPlayerId()] == 2)
            this->lineup1.erase(this->lineup1.begin() + player);
    }
    else if(team == 2)
    {   
        temp += "                                   ";
        temp += std::to_string(minute);
        if(event == 1)
        {
            temp += " - GOAL: ";
        }else if(event == 2)
        {
            temp += " - YELLOW CARD: ";
            this->yellowCards[this->lineup2[player]->getPlayerId()]++;

        }
        else{
            temp += " - RED CARD: ";
        }
        temp += this->lineup2[player]->getName();
        if(event == 3 || this->yellowCards[this->lineup2[player]->getPlayerId()] == 2)
            this->lineup2.erase(this->lineup2.begin() + player);
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
    std::uniform_int_distribution<int> eventType(0, 4000);
    int chanceEvent;
    std::string events = "";
    // calc chances (team1 attack and defense ovr and then for team2)
    int scoringChancesForTeamA;
    int scoringChancesForTeamB;
    while(minute < 93)
    {
        scoringChancesForTeamA = (int)(1.0 / (1.0 + std::exp((Game::getTeamDefenseOVR(this->lineup2) - Game::getTeamAttackOVR(this->lineup1)) / 100.0)) * 100);
        scoringChancesForTeamB = (int)(1.0 / (1.0 + std::exp((Game::getTeamDefenseOVR(this->lineup1) - Game::getTeamAttackOVR(this->lineup2)) / 100.0)) * 100);
        system("clear");
        std::cout << "+-------------------------------------------------------------------+\n";
        std::cout << "|           YOUR TEAM           " << this->homeScore << " - " << this->awayScore << "            OPPONENT TEAM      |" << std::endl;
        std::cout << "|                                " << this->minute << "                                 |\n";
        std::cout << "+-------------------------------------------------------------------+\n";
        chanceEvent = eventType(rng);
        if(chanceEvent < scoringChancesForTeamA){
            this->homeScore ++;
            events += this->eventDraw(1, 1);
        }
        else if(chanceEvent > 200 && chanceEvent < 225)
            events += this->eventDraw(1, 2);
        else if(chanceEvent > 500 && chanceEvent < 505)
            events += this->eventDraw(1, 3);
        else if(chanceEvent < 100 && chanceEvent > 100 - scoringChancesForTeamB){
            this->awayScore ++;
            events += this->eventDraw(2, 1);
        }
        else if(chanceEvent > 300 && chanceEvent < 325)
            events += this->eventDraw(2, 2);
        else if(chanceEvent > 800 && chanceEvent < 805)
            events += this->eventDraw(2, 3);
        std::cout << events;
        if(events.length() > 1)
            std::cout << "+-------------------------------------------------------------------+\n";
        usleep(100000);
        this->minute++;
    }
    system("clear");
        std::cout << "+-------------------------------------------------------------------+\n";
        std::cout << "|           YOUR TEAM           " << this->homeScore << " - " << this->awayScore << "            OPPONENT TEAM      |" << std::endl;
        std::cout << "|                                " << this->minute << "                                 |\n";
        std::cout << "+-------------------------------------------------------------------+\n";
        std::cout << events;
        if(events.length() > 1)
            std::cout << "+-------------------------------------------------------------------+\n";
    if(homeScore > awayScore)
        return 1;
    if(awayScore > homeScore)
        return 2;
    return 0;
}

template <typename T>
class Pack
{
    std::list<T> items;
public:
    Pack(std::string type = "Unknown");
    Pack(const Pack& obj);
    Pack& operator=(const Pack& obj);
    template <typename T1> friend std::ostream& operator<<(std::ostream& out, const Pack<T1>& obj);
    ~Pack() = default;

    const std::list<T> getItems() const{return items;}
    int getValue() const;
    void setItems(std::list<T>);
    void deleteElement(int index);
};

template <typename T>
std::vector<T> extractRandomElements(const std::vector<T>& inputVector, int numElements) {
    std::vector<T> randomElements = inputVector;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(randomElements.begin(), randomElements.end(), gen);

    randomElements.resize(numElements);

    return randomElements;
}

template <typename T>   // type = "silver", "gold", "special", "bronze"
std::list<T> generatePackItems(std::string type){
    std::vector<T> itemsShuffled;
    std::string templateName = typeid(T).name();
    std::transform(templateName.begin(), templateName.end(), templateName.begin(), ::tolower);
    // std::cout << typeid(T).name() << '\n';
    if(templateName.find("playercard") != std::string::npos)
    {
        if(type == "special")
        {
            itemsShuffled = extractRandomElements<T>(Database::getSpecialPlayers(), 8);
            std::list<T> items(itemsShuffled.begin(), itemsShuffled.end());
            return items;
        }
        if(type == "gold")
        {
            itemsShuffled = extractRandomElements<T>(Database::getGoldPlayers(), 8);
            std::list<T> items(itemsShuffled.begin(), itemsShuffled.end());
            return items;
        }
        if(type == "silver")
        {
            itemsShuffled = extractRandomElements<T>(Database::getSilverPlayers(), 8);
            std::list<T> items(itemsShuffled.begin(), itemsShuffled.end());
            return items;
        }
        itemsShuffled = extractRandomElements<T>(Database::getBronzePlayers(), 8);
        std::list<T> items(itemsShuffled.begin(), itemsShuffled.end());
        return items;
    }
    // if(templateName.find("usecard") != std::string::npos)
    // {
    //     itemsShuffled = extractRandomElements<T>(Database::getUseCards(), 2);
    //     std::list<T> items(itemsShuffled.begin(), itemsShuffled.end());
    //     return items;
    // }
    return {};
}

template <typename T>
Pack<T>::Pack(std::string type)
{
    this->items = generatePackItems<T>(type);
}

template <typename T>
Pack<T>::Pack(const Pack<T>& obj)
{
    this->items = obj.items;
}

template <typename T>
Pack<T>& Pack<T>::operator=(const Pack<T>& obj)
{
    if(this != &obj)
        this->items = obj.items;
    return *this;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Pack<T>& obj)
{
    //order the list
    // obj.items.sort();
    for(auto item: obj.items)
    {
        out << *item << '\n';
    }
    out << '\n';
    return out;
}

template <typename T>
int Pack<T>::getValue() const{
    if(this->items.empty()) return 0;
    int sum = 0;
    for(auto item: this->items)
        sum += item->getQuickSellValue();
    return sum;
}

template <typename T>
void Pack<T>::setItems(std::list<T> items)
{
    this->items = items;
}

bool endsWith(const std::string& str, const std::string& suffix) {
    if (str.length() >= suffix.length()) {
        return (str.substr(str.length() - suffix.length()) == suffix);
    }
    return false;
}

class Menu
{
    static Menu* instance;
    static int nrOfInstances;
    std::string filename;
    Club* club;
    Game* game;
    // std::unordered_map<int, int> boostedPlayers; // from PlayerCard id to duration of boost, when it gets to 0 it will be deleted from unordered map and from
    // the club with all players and lineup
    Menu(Club* club = nullptr, std::string filename = "unknown", Game* game = nullptr);
    Menu(const Menu& obj) = delete;
    Menu& operator=(const Menu& obj) = delete;
public:
    static Menu* getInstance();
    ~Menu();

    void run();
    void clubMenu();
    void packMenu();
    void openedPackMenu(Pack<const PlayerCard*>*);
    void openedPackUseCardsMenu(Pack<const UseCard*>*);
    void gameMenu();
};

int Menu::nrOfInstances = 0;
Menu* Menu::instance = nullptr;

Menu::Menu(Club* club, std::string filename, Game* game)
{
    this->club = club;
    this->filename = filename;
    this->game = game;
}

Menu* Menu::getInstance()
{
    nrOfInstances++;
    if(instance == nullptr){
        instance = new Menu();
    }
    return instance;
}

Menu::~Menu(){
    nrOfInstances--;
    this->club->saveToFile(this->filename);
    if(nrOfInstances == 0)
        if(instance){
            delete instance;
            //save data in filename
        }
}

void waitForUserInput()
{
    std::cout << "\nPress Enter to continue...";
    std::string line;
    std::cin >> line;
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Menu::run(){
    system("clear");
    std::cout << "Insert a filename to save the game (it shall contain no spaces and end in .txt): ";
    while(true){
        try{
            std::getline(std::cin, filename);
            if(filename.find(" ") != std::string::npos)
                throw std::runtime_error("Invalid filename (it contains spaces)!");
            if(filename.length() <= 4)
                throw std::runtime_error("Invalid filename (too short)!");
            if(endsWith(filename, ".txt"))
                break;
            throw std::runtime_error("Invalid filename (it doesn't end in .txt)!");
        }
        catch(const std::runtime_error &e)
        {
            std::cerr << e.what() << '\n' << "Try again!\n";
        }
    }
    if(!std::filesystem::exists(filename))
    {
        this->club = new Club();
        std::cin >> *this->club;
        for(int i = 0; i < 5; i++)
        {
            this->club->addPlayer(extractRandomElements(Database::getBronzePlayers(), 1)[0]);
            this->club->addPlayer(extractRandomElements(Database::getSilverPlayers(), 1)[0]);
        }
        this->club->addPlayer(extractRandomElements(Database::getGoldPlayers(), 1)[0]);
    }else{
        this->club->loadFromFile(filename);
    }
    int command;
    bool running = true;
    while(running)
    {
        system("clear");
        std::cout << "1 - View club!\n";
        std::cout << "2 - Open packs!\n"; // te va duce pe o pagina in care selectezi ce pack vrei sa cumperi (fiecare are afisat pretul, + bugetul tau)
        std::cout << "3 - Play game!\n"; // pt asta va avea de selectat dificultatea
        std::cout << "0 - Quit!\n";
        command = readInt(std::cin);
        switch(command)
        {
            case 1:{
                this->clubMenu();
                break;
            }
            case 2:{
                this->packMenu();
                break;
            }
            case 3:{
                this->gameMenu();
                break;
            }
            case 0:{
                running = false;
                break;
            }
            default:{
                std::cout << "Try again!\n";
                break;
            }
        }
    }
}

void Menu::clubMenu()
{
    int command;
    bool running = true;
    while(running)
    {
        system("clear");
        std::cout << "1 - Show club!\n";
        std::cout << "2 - View all players!\n";
        std::cout << "3 - View lineup!\n";
        std::cout << "4 - Add to lineup!\n";
        std::cout << "5 - Remove from lineup!\n";
        std::cout << "6 - Random lineup!\n";
        // std::cout << "7 - Choose player for boost!\n";
        std::cout << "0 - Quit!\n";
        command = readInt(std::cin);
        switch(command)
        {
            case 1:{
                system("clear");
                std::cout << *this->club << '\n';
                waitForUserInput();
                break;
            }
            case 2:{
                system("clear");
                this->club->showAllPlayers();
                waitForUserInput();
                break;
            }
            case 3:{
                system("clear");
                if(this->club->getLineup().empty())
                {
                    std::cout << "No players in lineup!" << '\n';
                }
                else
                    this->club->showLineup();
                waitForUserInput();
                break;
            }
            case 4:{
                system("clear");
                int playerId;
                this->club->showAllPlayers();
                std::cout << "Give player id(or 0 - Back): ";
                while(true){
                    playerId = readInt(std::cin);
                    if(playerId == 0)
                        break;
                    this->club->addToLineup(playerId);
                }
                if(playerId == 0)
                    break;
                waitForUserInput();
                break;
            }
            case 5:{
                system("clear");
                int playerId;
                std::cout << "Give player id(or 0 - Back): ";
                while(true){
                    playerId = readInt(std::cin);
                    if(playerId == 0)
                        break;
                    if(this->club->removeFromLineup(playerId) == true)
                        break;
                }
                if(playerId == 0)
                    break;
                waitForUserInput();
                break;
            }
            case 6:{
                system("clear");
                for(auto player: this->club->getAllPlayers())
                    this->club->addToLineup(player.first);
            }
            // case 7:{
            //     system("clear");
            //     if(this->club->getNumPlayerUseCards() == 0)
            //         break;
            //     int useCardId;
            //     std::cout << "Insert use card id: ";
            //     while(true){
            //         useCardId = readInt(std::cin);
            //         if(useCardId == 0)
            //             break;
            //         if(this->club->findPlayerUseCard(useCardId))
            //             break;
            //         std::cout << "Try again!\n";
            //     }
            //     if(useCardId == 0)
            //         break;
            //     int playerId;
            //     std::cout << "Insert player id: ";
            //     while(true)
            //     {
            //         playerId = readInt(std::cin);
            //         if(playerId == 0)
            //             break;
            //         if(this->club->getAllPlayers().find(playerId) != this->club->getAllPlayers().end())
            //             break;
            //         std::cout << "Try again!\n";
            //     }
            //     if(playerId == 0)
            //         break;
            //     if(Database::getUseCard(useCardId)->getName() == "Special attack")
            //     {
            //         const PlayerCard* boostedPlayer = new PlayerCard(this->club->getAllPlayers().at(playerId)->getName(), this->club->getAllPlayers().at(playerId)->getAttackOVR() + 10,
            //                                          this->club->getAllPlayers().at(playerId)->getDefenseOVR(), this->club->getAllPlayers().at(playerId)->getQuickSellValue());
            //         boostedPlayers[boostedPlayer->getPlayerId()] = Database::getUseCard(useCardId)->getDuration();
                    
            //     } else
            //     {
            //         const PlayerCard* boostedPlayer = new PlayerCard(this->club->getAllPlayers().at(playerId)->getName(), this->club->getAllPlayers().at(playerId)->getAttackOVR(),
            //                                          this->club->getAllPlayers().at(playerId)->getDefenseOVR() + 10, this->club->getAllPlayers().at(playerId)->getQuickSellValue());
            //         boostedPlayers[boostedPlayer->getPlayerId()] = Database::getUseCard(useCardId)->getDuration();
            //     }
            //     break;
            // }
            case 0:{
                running = false;
                break;
            }
            default:{
                std::cout << "Try again!\n";
                break;
            }
        }
    }
}

void Menu::packMenu()
{
    int command;
    bool running = true;
    Pack<const PlayerCard*>* playerPack;
    Pack<const UseCard*>* useCardPack;
    while(running)
    {
        system("clear");
        std::cout << MAGENTA << "Budget: " << this->club->getBudget() << RESET << '\n';
        std::cout << "1 - Special pack - 30.000\n";
        std::cout << "2 - Gold pack - 20.000\n";
        std::cout << "3 - Silver pack - 10.000\n";
        std::cout << "4 - Bronze pack - 5.000\n";
        // std::cout << "5 - Use cards - 100.000\n";
        std::cout << "0 - Quit!\n";
        command = readInt(std::cin);
        switch(command)
        {
            case 1:{
                system("clear");
                if(this->club->getBudget() < 30000)
                    std::cout << "Insufficient funds!";
                else{
                this->club->setBudget(this->club->getBudget() - 30000);
                playerPack = new Pack<const PlayerCard*>("special");
                this->openedPackMenu(playerPack);
                }
                waitForUserInput();
                break;
            }
            case 2:{
                system("clear");
                if(this->club->getBudget() < 20000)
                    std::cout << "Insufficient funds!";
                else{
                this->club->setBudget(this->club->getBudget() - 20000);
                playerPack = new Pack<const PlayerCard*>("gold");
                this->openedPackMenu(playerPack);
                }
                waitForUserInput();
                break;
            }
            case 3:{
                system("clear");
                if(this->club->getBudget() < 10000)
                    std::cout << "Insufficient funds!";
                else{
                this->club->setBudget(this->club->getBudget() - 10000);
                playerPack = new Pack<const PlayerCard*>("silver");
                this->openedPackMenu(playerPack);
                }
                waitForUserInput();
                break;
            }
            case 4:{
                system("clear");
                if(this->club->getBudget() < 5000)
                    std::cout << "Insufficient funds!";
                else{
                this->club->setBudget(this->club->getBudget() - 5000);
                playerPack = new Pack<const PlayerCard*>("bronze");
                this->openedPackMenu(playerPack);
                }
                waitForUserInput();
                break;
            }
            // case 5:{
            //     system("clear");
            //     if(this->club->getBudget() < 100000)
            //         std::cout << "Insufficient funds!";
            //     else{
            //     this->club->setBudget(this->club->getBudget() - 100000);
            //     useCardPack = new Pack<const UseCard*>("");
            //     this->openedPackUseCardsMenu(useCardPack);
            //     }
            //     waitForUserInput();
            //     break;
            // }
            case 0:{
                running = false;
                break;
            }
            default:{
                std::cout << "Try again!\n";
                break;
            }
        }
    }
}

void Menu::openedPackMenu(Pack<const PlayerCard*>* playerPack)
{
    system("clear");
    std::list<const PlayerCard*> tempList;
    int commandPack;
    while(!playerPack->getItems().empty())
    {
        tempList = {};
        std::cout << *playerPack << '\n';
        std::cout << "1 - Store all items in the club!\n"; // cand dai store elimini itemele din pachet atunci cand le bagi in club
        std::cout << "2 - Quick sell all items for " << playerPack->getValue() << "!\n";
        commandPack = readInt(std::cin);
        switch(commandPack){
            case 1:{
                int index = 0;
                for(auto player: playerPack->getItems())
                {
                    if(this->club->getAllPlayers().find(player->getPlayerId()) == this->club->getAllPlayers().end()) // If the player is not already in the club we remove it from the pack and add it to the club
                    {
                        this->club->addPlayer(player);
                    }
                    else {
                        tempList.push_back(player);
                    }
                }
                playerPack->setItems(tempList);
                break;
            }
            case 2:{
                for(auto player: playerPack->getItems())
                {
                    this->club->setBudget(this->club->getBudget() + player->getQuickSellValue());
                }
                playerPack->setItems(tempList);
                break;
            }
            default:{
                std::cout << "Invalid command!\n";
                break;
            }
        }
    }
}

// void Menu::openedPackUseCardsMenu(Pack<const UseCard*>* useCardPack)
// {
//     system("clear");
//     std::list<const UseCard*> tempList;
//     int commandPack;
//     while(!useCardPack->getItems().empty())
//     {
//         tempList = {};
//         std::cout << *useCardPack << '\n';
//         std::cout << "1 - Store all items in the club!\n";
//         std::cout << "2 - Quick sell all items for " << useCardPack->getValue() << "!\n";
//         commandPack = readInt(std::cin);
//         switch(commandPack){
//             case 1:{
//                 int index = 0;
//                 for(auto useCard: useCardPack->getItems())
//                 {
//                     if(this->club->addUseCard(useCard) == false);
//                      tempList.push_back(useCard);
//                 }
//                 useCardPack->setItems(tempList);
//                 break;
//             }
//             case 2:{
//                 for(auto useCard: useCardPack->getItems())
//                 {
//                     this->club->setBudget(this->club->getBudget() + useCard->getQuickSellValue());
//                 }
//                 useCardPack->setItems(tempList);
//                 break;
//             }
//             default:{
//                 std::cout << "Invalid command!\n";
//                 break;
//             }
//         }
//     }
// }

void Menu::gameMenu()
{
    int command;
    bool running = true;
    int result;
    std::vector<const PlayerCard*> lineupComputer;
    if(this->club->getLineup().size() == 11)
        while(running)
        {
            system("clear");
            std::cout << "Choose difficulty:\n";
            std::cout << "You will be rewarded more money for higher difficulty!\n";
            std::cout << "1 - Easy!\n";
            std::cout << "2 - Medium!\n";
            std::cout << "3 - Hard!\n";
            std::cout << "4 - Very hard!\n";
            std::cout << "0 - Back!\n";
            command = readInt(std::cin);
            switch(command)
            {
                case 1:{
                    system("clear");
                    lineupComputer = extractRandomElements(Database::getBronzePlayers(), 11);
                    game = new Game(this->club->getLineup(), lineupComputer);
                    result = game->simGame();
                    if(result == 0)
                        this->club->setBudget(this->club->getBudget() + 200);
                    else if(result == 1)
                        this->club->setBudget(this->club->getBudget() + 500);
                    else if(result == 2)
                        this->club->setBudget(this->club->getBudget() + 100);
                    // for(auto player: this->club->getLineup())
                    // {
                    //     if(boostedPlayers[player->getPlayerId()] > 0)
                    //     {
                    //         boostedPlayers[player->getPlayerId()]--;
                    //         if(boostedPlayers[player->getPlayerId()] == 0)
                    //         {
                    //             this->club->removeFromLineup(player->getPlayerId());
                    //             this->club->removeFromTeam(player->getPlayerId());
                    //         }
                    //     }
                    // }
                    waitForUserInput();
                    running = false;
                    break;
                }
                case 2:{
                    system("clear");
                    lineupComputer = extractRandomElements(Database::getSilverPlayers(), 11);
                    game = new Game(this->club->getLineup(), lineupComputer);
                    result = game->simGame();
                    if(result == 0)
                        this->club->setBudget(this->club->getBudget() + 500);
                    else if(result == 1)
                        this->club->setBudget(this->club->getBudget() + 1500);
                    else if(result == 2)
                        this->club->setBudget(this->club->getBudget() + 250);
                    // for(auto player: this->club->getLineup())
                    // {
                    //     if(boostedPlayers[player->getPlayerId()] > 0)
                    //     {
                    //         boostedPlayers[player->getPlayerId()]--;
                    //         if(boostedPlayers[player->getPlayerId()] == 0)
                    //         {
                    //             this->club->removeFromLineup(player->getPlayerId());
                    //             this->club->removeFromTeam(player->getPlayerId());
                    //         }
                    //     }
                    // }
                    waitForUserInput();
                    running = false;
                    break;
                }
                case 3:{
                    system("clear");
                    lineupComputer = extractRandomElements(Database::getGoldPlayers(), 11);
                    game = new Game(this->club->getLineup(), lineupComputer);
                    result = game->simGame();
                    if(result == 0)
                        this->club->setBudget(this->club->getBudget() + 1000);
                    else if(result == 1)
                        this->club->setBudget(this->club->getBudget() + 3000);
                    else if(result == 2)
                        this->club->setBudget(this->club->getBudget() + 350);
                    // for(auto player: this->club->getLineup())
                    // {
                    //     if(boostedPlayers[player->getPlayerId()] > 0)
                    //     {
                    //         boostedPlayers[player->getPlayerId()]--;
                    //         if(boostedPlayers[player->getPlayerId()] == 0)
                    //         {
                    //             this->club->removeFromLineup(player->getPlayerId());
                    //             this->club->removeFromTeam(player->getPlayerId());
                    //         }
                    //     }
                    // }
                    waitForUserInput();
                    running = false;
                    break;
                }
                case 4:{
                    system("clear");
                    lineupComputer = extractRandomElements(Database::getSpecialPlayers(), 11);
                    game = new Game(this->club->getLineup(), lineupComputer);
                    result = game->simGame();
                    if(result == 0)
                        this->club->setBudget(this->club->getBudget() + 2500);
                    else if(result == 1)
                        this->club->setBudget(this->club->getBudget() + 5000);
                    else if(result == 2)
                        this->club->setBudget(this->club->getBudget() + 450);
                    // for(auto player: this->club->getLineup())
                    // {
                    //     if(boostedPlayers[player->getPlayerId()] > 0)
                    //     {
                    //         boostedPlayers[player->getPlayerId()]--;
                    //         if(boostedPlayers[player->getPlayerId()] == 0)
                    //         {
                    //             this->club->removeFromLineup(player->getPlayerId());
                    //             this->club->removeFromTeam(player->getPlayerId());
                    //         }
                    //     }
                    // }
                    waitForUserInput();
                    //delete game
                    running = false;
                    break;
                }
                case 0:{
                    running = false;
                    break;
                }
                default:{
                    std::cout << "Try again!\n";
                    break;
                }
            }
        }// testeaza dupa meci daca aveai pe cineva in lineup care era boosted ca sa ii dai duration -- (dupa ce dai duration -- verifici daca a ajuns la 0)
        // caz in care ii dai delete din allPlayers
    else{
        std::cout << "Lineup incomplete!\n";
        waitForUserInput();
    }
}

int main() {
    // TeamUseCard d("sadsada", 15, 20, "Attack"), e(d);
    Database::readPlayers();
    Database::readUseCards();
    Menu* menu;
    menu = menu->getInstance();
    try{
        menu->run();
    }
    catch(...)
    {
        std::cout << "Unexpected error occured!\n";
    }
    return 0;
}