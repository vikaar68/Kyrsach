#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Task.h"

class ScrumBoard {
private:
    //размеры окна
    const float WINDOW_WIDTH = 1920.0f;
    const float WINDOW_HEIGHT = 1080.0f;
    
    //создание секций и блоков
    std::vector<std::string> sectionNames;
    std::vector<sf::RectangleShape> sections;
    std::vector<sf::Text> sectionTexts;
    std::vector<std::vector<Task>> tasks;
    sf::Font font;
    sf::Text titleText;
    
    //перетаскивание и прокрутка
    std::vector<float> scrollOffsets;
    std::vector<bool> isDraggingScroll;
    std::vector<sf::Vector2f> dragStartPositions;
    int draggingTaskSection;
    int draggingTaskIndex;
    sf::Vector2f dragStartMousePos;
    
    //интерфейс верхней панели
    sf::RectangleShape topPanel;
    sf::RectangleShape companyButton;
    sf::RectangleShape saveButton;
    sf::Text companyButtonText;
    sf::Text saveButtonText;
    bool showCompanyWindow;
    sf::RectangleShape companyWindow;
    std::vector<sf::RectangleShape> companyRects;
    std::vector<sf::Text> companyTexts;
    std::vector<std::string> companies;
    
    //состояние кнопок
    bool companyButtonPressed;
    bool saveButtonPressed;

public:
    ScrumBoard();
    bool initialize(); //загрузка ресурсов
    void handleEvent(const sf::Event& event, sf::RenderWindow& window); //ввод пользователя
    void update(float deltaTime); //обновление состояния приложения
    void draw(sf::RenderWindow& window); //рисование интерфейса
    void addTask(const std::string& taskName, int section); //создание новой задачи
    void updateTaskPositions(); //обновление позиции задачи
    
private:
    void createSections(); //секции - блоки
    void createSampleTasks(); //примеры в блоках
    void createTitle(); //текст названия
    void createTopPanel(); //верхняя панель
    void createCompanyWindow(); //окно компаний
    void centerText(sf::Text& text, const sf::RectangleShape& shape, float yOffset = 0.0f); //центрирование текста
};