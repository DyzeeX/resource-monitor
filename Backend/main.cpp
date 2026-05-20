#include "Core/ResourceMonitor.hpp"
#include "Core/JSONManager.hpp" 

#include "crow.h"

#include <thread>
#include <chrono>
#include <mutex>
#include <set>

int main() {
    ResourceMonitor rm;
    crow::SimpleApp app;

    std::mutex conn_mutex;
    std::set<crow::websocket::connection*> clients;

    CROW_WEBSOCKET_ROUTE(app, "/ws")
        .onopen([&](crow::websocket::connection& conn) {
            std::lock_guard<std::mutex> lock(conn_mutex);
            clients.insert(&conn);
            CROW_LOG_INFO << "Client connected";
        })
        .onclose([&](crow::websocket::connection& conn, const std::string&, uint16_t) {
            std::lock_guard<std::mutex> lock(conn_mutex);
            clients.erase(&conn);
            CROW_LOG_INFO << "Client disconnected";
        })
        .onmessage([](crow::websocket::connection&, const std::string&, bool) {

        });

    std::thread([&]() {
    while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            rm.Update();
            std::string json = JSONManager::ConvertToString(rm);
            std::lock_guard<std::mutex> lock(conn_mutex);

            for (auto* c : clients) {
                c->send_text(json);
            }
        }
    }).detach();

    app.port(2006).multithreaded().run();
    return 0;
}