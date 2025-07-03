# Athlete Awards Tracker - Учет наград спортсменов

## 1. Требования

### 1.1 Системные требования
- **Qt 6.8.2+** (MinGW 64-bit или любая Qt ≥ 6.8.2)
- **CMake ≥ 3.16**
- **Компилятор с поддержкой C++17**:
  - MinGW (GCC 11+) - поставляется с Qt
  - MSVC v143 (Visual Studio 2022) на Windows
  - GCC ≥ 11 или Clang ≥ 15 на Linux/macOS
- **Рекомендуемый генератор сборки**: Ninja

### 1.2 Дополнительные зависимости
- SQLite3 (включена в Qt)
- Qt Charts (для визуализации статистики)
- Qt PDF (для генерации отчетов)

## 2. Инструкция по сборке

### 2.1 Сборка через Qt Creator
1. **Открытие проекта**:
   - Запустите Qt Creator
   - Выберите `Файл → Открыть файл или проект...`
   - Укажите корневой `CMakeLists.txt`

2. **Настройка Kit**:
   - В списке `Текущий проект` выберите Kit (например `Desktop Qt 6.8.2 MinGW 64-bit`)
   - В настройках сборки (`Projects → Build`):
     - Установите `Build type`: Debug/Release
     - Выберите генератор CMake: Ninja
     - Убедитесь, что `CMAKE_PREFIX_PATH` указывает на папку Qt (например `C:/Qt/6.8.2/mingw_64/lib/cmake`)

3. **Сборка**:
   - Нажмите `Запустить CMake` (кнопка "Запустить CMake")
   - Выполните `Сборка → Собрать проект` (Ctrl+B)

4. **Запуск**:
   - Нажмите `Запустить` (зеленая стрелка или F5)
   - Исполняемый файл будет находиться в:
     ```
     <папка_проекта>/build/Desktop_Qt_6.8.2_MinGW_64_bit-<Debug|Release>/
     ```

### 2.2 Консольная сборка (Linux/macOS/WSL)
```bash
mkdir build && cd build
cmake -G Ninja -DCMAKE_PREFIX_PATH=/path/to/qt -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
./AthleteAwardsTracker
```

### 2.3 Консольная сборка (Windows)
```mkdir build
cd build
cmake -G "Ninja" -DCMAKE_PREFIX_PATH="C:\Qt\6.8.2\mingw_64\lib\cmake" ..
cmake --build .
.\AthleteAwardsTracker.exe
```
