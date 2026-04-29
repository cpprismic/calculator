#!/usr/bin/env bash
# Запускает N раз калькулятор с уникальными операндами (всегда cache-miss).
# Использование: ./scripts/perf_workload.sh [путь_к_бинарнику] [количество_запусков]
# Пример:        ./scripts/perf_workload.sh ./build_perf/calculator 500
set -euo pipefail

CALC="${1:-./build_perf/calculator}"
RUNS="${2:-500}"
OPS=(add subtract multiply divide power)

for i in $(seq 1 "$RUNS"); do
    op="${OPS[$((i % 5))]}"
    a=$((i * 3 + 1))  # уникально: 4, 7, 10, 13, ...
    b=$((i * 2 + 7))  # уникально: 9, 11, 13, 15, ... (никогда 0 => нет division by zero)
    "$CALC" "{\"operation\": \"$op\", \"operands\": [$a, $b]}" > /dev/null 2>&1
done
