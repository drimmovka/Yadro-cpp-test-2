# Computer Club Monitoring System


## Сборка и запуск
```bash
cmake -B build && cmake --build build  
./build/computer-club-monitoring-system <input-file>
```


## Тестирование
```bash
cd test
cmake -B build && cmake --build build
./build/test
```


## Проверка памяти
```bash
valgrind --leak-check=full ./build/computer-club-monitoring-system test/tests/test_11.txt 

==53416== Memcheck, a memory error detector
==53416== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==53416== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==53416== Command: ./build/computer-club-monitoring-system test/tests/test_11.txt
==53416==
[ ...output omitted... ]
==53416== 
==53416== HEAP SUMMARY:
==53416==     in use at exit: 0 bytes in 0 blocks
==53416==   total heap usage: 231,564 allocs, 231,564 frees, 1,034,015 bytes allocated
==53416== 
==53416== All heap blocks were freed -- no leaks are possible
==53416== 
==53416== For lists of detected and suppressed errors, rerun with: -s
==53416== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## Структура проекта
```
.
├── CMakeLists.txt          # Главный конфиг сборки
├── include/                # Заголовки (.hpp)
├── src/                    # Исходники (.cpp)
├── test/
│   ├── CMakeLists.txt      # Конфиг для тестов
│   ├── answers/            # Эталонные ответы (.txt)
│   └── tests/              # Входные тесты (.txt)
└── third_party/
    └── googletest/         # Исходники GoogleTest (submodule)

CMake-файлы:
- Корневой CMakeLists.txt 
- test/CMakeLists.txt 

```
