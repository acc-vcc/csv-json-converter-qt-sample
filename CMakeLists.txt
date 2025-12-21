cmake_minimum_required(VERSION 3.5)

project(ConvCsvJson LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt の自動処理（moc / uic / rcc）
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Qt6 → Qt5 の順に探す
find_package(Qt6 COMPONENTS Widgets QUIET)
if(Qt6_FOUND)
    message(STATUS "Using Qt6")
    set(QT_PACKAGE Qt6)
else()
    find_package(Qt5 COMPONENTS Widgets QUIET)
    if(Qt5_FOUND)
        message(STATUS "Using Qt5")
        set(QT_PACKAGE Qt5)
    else()
        message(FATAL_ERROR "Qt5 も Qt6 も見つかりませんでした。Qt をインストールするか CMAKE_PREFIX_PATH/Qt*_DIR を指定してください。")
    endif()
endif()

# ソースファイル
set(SOURCES
    main.cpp
    mainwindow.cpp
)

set(HEADERS
    mainwindow.h
)

set(UI
    mainwindow.ui
)

# 実行ファイル
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI}
)

# Qt5 or Qt6 のリンク
target_link_libraries(${PROJECT_NAME}
    ${QT_PACKAGE}::Widgets
)

