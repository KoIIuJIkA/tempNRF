//===----------------------------------------------------------------------===//

#include "../include/redis_find.h" // im a superman.

using json = nlohmann::json;
using Redis = sw::redis::Redis;
using StringView = sw::redis::StringView;
using OptionalString = sw::redis::OptionalString;

int main() {

    try {
        auto redis = RedisInterface("tcp://127.0.0.1:6379");

        std::ifstream f("../data/data_model.json");

        if (f.is_open()) {
            json data = json::parse(f);

            redis.create("key_data", data.dump(4));

            redis.find("../data/config_file.json");
            std::cout << "Hello, World!";
            // redis.delete("key_data");
        }

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
