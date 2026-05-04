#include "app/calculator.hpp"
#include "app/task.hpp"
#include "logger/logger.hpp"
#include "storage/storage.hpp"

int main() {
    logger::Logger::getInstance().setLevel(logger::LogLevel::Warning);

    storage::Storage storage;
    calculator::Calculator calc;

    // Phase 1 — cache-miss path: find (miss) -> calculate -> save (cache + DB)
    constexpr int kMissRounds = 1'000;
    for (int i = 0; i < kMissRounds; ++i) {
        calculator::Task t;
        t.operation     = "add";
        t.first_number  = i;
        t.second_number = i + 1;
        if (!storage.find(t)) {
            calc.calculate(t);
            storage.save(t);
        }
    }

    // Phase 2 — cache-hit path: find (hit) -> done, no DB access
    constexpr int kHitRounds = 10'000'000;
    for (int i = 0; i < kHitRounds; ++i) {
        calculator::Task t;
        t.operation     = "add";
        t.first_number  = i % kMissRounds;
        t.second_number = (i % kMissRounds) + 1;
        storage.find(t);
    }
}
