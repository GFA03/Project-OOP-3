#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

// Reading from the .csv file players stats
void readPlayers()
{
    std::ifstream fin("players_fifa23_cleaned.csv");
    std::string x;
    std::vector<std::string> playerStats;
    int attackOVR, defenseOVR;
    std::getline(fin, x);
    int i = 0;
    while(std::getline(fin, x))
    {
        if(i == 6)
            break;
        i++;
        playerStats = lineRead(x);
        // std::cout << findBestOVR(playerStats).first << " " << findBestOVR(playerStats).second << '\n';
    }
}

int main() {
    // std::cout << x.substr(0, index) << '\n';
    // x.erase(0, index+1);
    // std::cout << x << '\n';
    // std::getline(fin, x);
    // std::cout << x << '\n';
    // lineRead(x);
    readPlayers();
    return 0;
}
