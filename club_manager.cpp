#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <queue>
#include <iomanip>

struct Event {
    std::string time;
    int id;
    std::string details;
};

struct Table {
    bool occupied = false;
    std::string client;
    int total_minutes = 0;
    int last_used_time = 0;
};

int parseTime(const std::string& time) {
    int hours = std::stoi(time.substr(0, 2));
    int minutes = std::stoi(time.substr(3, 2));
    return hours * 60 + minutes;
}

std::string formatTime(int total_minutes) {
    int hours = total_minutes / 60;
    int minutes = total_minutes % 60;
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes;
    return oss.str();
}

void processEvents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return;
    }

    int num_tables;
    file >> num_tables;

    std::string work_start, work_end;
    file >> work_start >> work_end;

    int price_per_hour;
    file >> price_per_hour;

    std::vector<Event> events;
    std::string line;
    std::getline(file, line); // Move to the next line after the header.
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string time, id_str;
        std::getline(iss, time, ' ');
        std::getline(iss, id_str, ' ');
        int id = std::stoi(id_str);
        std::string details;
        std::getline(iss, details);
        events.push_back({ time, id, details });
    }

    int start_time = parseTime(work_start);
    int end_time = parseTime(work_end);

    std::map<std::string, int> clients;
    std::queue<std::string> waiting_queue;
    std::vector<Table> tables(num_tables);

    std::cout << work_start << std::endl;

    for (const auto& event : events) {
        int event_time = parseTime(event.time);

        if (event_time < start_time) {
            std::cout << event.time << " " << event.id << " " << event.details << std::endl;
            std::cout << event.time << " 13 NotOpenYet" << std::endl;
            continue;
        }

        if (event.id == 1) { // Client arrives
            if (clients.count(event.details)) {
                std::cout << event.time << " " << event.id << " " << event.details << std::endl;
                std::cout << event.time << " 13 YouShallNotPass" << std::endl;
            }
            else {
                clients[event.details] = -1; // Client is in the club but not at a table
                std::cout << event.time << " " << event.id << " " << event.details << std::endl;
            }
        }
        else if (event.id == 2) { // Client sits at a table
            std::istringstream iss(event.details);
            std::string client;
            int table_id;
            iss >> client >> table_id;

            if (!clients.count(client) || clients[client] == -1) {
                if (table_id <= 0 || table_id > num_tables || tables[table_id - 1].occupied) {
                    std::cout << event.time << " " << event.id << " " << client << " " << table_id << std::endl;
                    std::cout << event.time << " 13 PlaceIsBusy" << std::endl;
                }
                else {
                    clients[client] = table_id - 1;
                    tables[table_id - 1].occupied = true;
                    tables[table_id - 1].client = client;
                    tables[table_id - 1].last_used_time = event_time;
                    std::cout << event.time << " " << event.id << " " << client << " " << table_id << std::endl;
                }
            }
            else {
                std::cout << event.time << " 13 ClientUnknown" << std::endl;
            }
        }
        else if (event.id == 4) { // Client leaves
            std::string client = event.details;
            if (!clients.count(client) || clients[client] == -1) {
                std::cout << event.time << " 13 ClientUnknown" << std::endl;
            }
            else {
                int table_id = clients[client];
                if (table_id != -1) {
                    int usage_time = event_time - tables[table_id].last_used_time;
                    tables[table_id].total_minutes += usage_time;
                    tables[table_id].occupied = false;
                    tables[table_id].client.clear();
                }
                clients.erase(client);
                std::cout << event.time << " " << event.id << " " << client << std::endl;

                // Assign table to waiting client if possible
                if (!waiting_queue.empty()) {
                    std::string next_client = waiting_queue.front();
                    waiting_queue.pop();
                    clients[next_client] = table_id;
                    tables[table_id].occupied = true;
                    tables[table_id].client = next_client;
                    tables[table_id].last_used_time = event_time;
                    std::cout << event.time << " 12 " << next_client << " " << table_id + 1 << std::endl;
                }
            }
        }
        else if (event.id == 3) { // Client waits
            std::string client = event.details;
            std::cout << event.time << " " << event.id << " " << client << std::endl;
            bool free_table = false;
            for (const auto& table : tables) {
                if (!table.occupied) {
                    free_table = true;
                    break;
                }
            }

            if (free_table) {
                std::cout << event.time << " 13 ICanWaitNoLonger!" << std::endl;
            }
            else {
                waiting_queue.push(client);
            }
        }
    }

    for (const auto& [client, table_id] : clients) {
        if (table_id != -1) {
            int usage_time = end_time - tables[table_id].last_used_time;
            tables[table_id].total_minutes += usage_time;
            tables[table_id].occupied = false;
            std::cout << work_end << " 11 " << client << std::endl;
        }
    }

    std::cout << work_end << std::endl;
    for (int i = 0; i < num_tables; ++i) {
        int total_hours = (tables[i].total_minutes + 59) / 60; // Round up to full hours
        int total_revenue = total_hours * price_per_hour; // Correct revenue calculation
        std::cout << i + 1 << " " << total_revenue << " " << formatTime(tables[i].total_minutes) << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    processEvents(argv[1]);
    return 0;
}