# phi placement

реализация 2 задачи по компиляторам на c++.

алгоритм сделан по статье `A Linear Time Algorithm for Placing PHI-Nodes`:
- dom(n) итеративно
- idom(n) через sdom(n)
- дерево доминаторов и уровни
- j-edge по правилу `x !sdom y`
- df(x) через `subtree(x)` и j-edge
- idf(defs) итеративно

## сборка

```bash
cmake -S . -B build
cmake --build build
```

## запуск

```bash
./build/phi_placement < input.txt
```

## формат входа

```text
m
u1 v1
u2 v2
...
um vm
k
d1 d2 ... dk
```

где:
- `m` — количество дуг cfg
- `ui vi` — дуга `ui -> vi`
- `k` — количество узлов с определениями переменной
- `d1..dk` — множество defs

## формат выхода

программа печатает множество узлов для вставки phi:

```text
{n1, n2, ...}
```

## пример

вход:

```text
9
start 1
1 2
1 3
2 4
3 4
3 5
4 1
4 5
5 stop
3
2 3 4
```

выход:

```text
{1, 4, 5}
```
