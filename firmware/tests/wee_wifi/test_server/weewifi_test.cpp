#include <iostream>
#include "crow_all.h"

int main(int argc, char *argv[])
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/jsontest").methods("POST"_method) 
    (
        [](const crow::request& req) 
        { 
            auto x = crow::json::load(req.body); 
            if (!x) 
            {
                return crow::response(400); 
            }
            std::cout << x << std::endl;
            return crow::response(200); 
        }
    );

    app.port(5001).multithreaded().run();
    return 0;
}
