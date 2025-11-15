// ScrumBoard.cpp
#include "ScrumBoard.h"
#include <iostream>
#include <algorithm>

//конструктор
ScrumBoard::ScrumBoard() {
    sectionNames = {"Назначено", "В процессе", "Блокировано", "Готово"};
    tasks.resize(4);
    scrollOffsets.resize(4, 0.0f);
    isDraggingScroll.resize(4, false);
    dragStartPositions.resize(4, sf::Vector2f(0, 0));
    draggingTaskSection = -1;
    draggingTaskIndex = -1;
    showCompanyWindow = false;
    companyButtonPressed = false;
    saveButtonPressed = false;
    
    companies = {"Компания A", "Компания Б", "Компания С", "Компания Д"};
}

//инициализация
bool ScrumBoard::initialize() {
    if (!font.loadFromFile("ofont.ru_Pastry Chef.ttf")) {
        std::cout << "Не удалось загрузить шрифт!" << std::endl;
        return false;
    }
    
    createTitle();
    createTopPanel();
    createSections();
    createSampleTasks();
    createCompanyWindow();
    
    return true;
}

//создание заголовка
void ScrumBoard::createTitle() {
    titleText.setString("Scrum Board - Управление задачами");
    titleText.setFont(font);
    titleText.setCharacterSize(36);
    titleText.setFillColor(sf::Color(50, 50, 100));
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(650, 110);
}

//создание верхней панели
void ScrumBoard::createTopPanel() {
    topPanel.setSize(sf::Vector2f(WINDOW_WIDTH, 60));
    topPanel.setFillColor(sf::Color(120, 165, 205));
    topPanel.setOutlineThickness(0);
    topPanel.setPosition(0, 20);

    //кнопка компаний - слева
    companyButton.setSize(sf::Vector2f(200, 40));
    companyButton.setFillColor(sf::Color(180, 210, 235));
    companyButton.setOutlineThickness(0);
    companyButton.setPosition(150, 30);

    //кнопка сохранить - справа
    saveButton.setSize(sf::Vector2f(200, 40));
    saveButton.setFillColor(sf::Color(180, 210, 235));
    saveButton.setOutlineThickness(0);
    saveButton.setPosition(WINDOW_WIDTH - 350, 30); // 1920 - 350 = 1570

    //текст кнопки компаний
    companyButtonText.setString("Компании");
    companyButtonText.setFont(font);
    companyButtonText.setCharacterSize(24);
    companyButtonText.setFillColor(sf::Color(50, 50, 80));
    companyButtonText.setStyle(sf::Text::Bold);
    
    //текст кнопки сохранить
    saveButtonText.setString("Сохранить");
    saveButtonText.setFont(font);
    saveButtonText.setCharacterSize(24);
    saveButtonText.setFillColor(sf::Color(50, 50, 80));
    saveButtonText.setStyle(sf::Text::Bold);

    //центрирование текста
    centerText(companyButtonText, companyButton);
    centerText(saveButtonText, saveButton);
}

//центрирование текста в прямоугольнике
void ScrumBoard::centerText(sf::Text& text, const sf::RectangleShape& shape, float yOffset) {
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::Vector2f shapePos = shape.getPosition();
    sf::Vector2f shapeSize = shape.getSize();
    
    text.setPosition(
        shapePos.x + (shapeSize.x - textBounds.width) / 2,
        shapePos.y + (shapeSize.y - textBounds.height) / 2 + yOffset
    );
}

//создание окна компаний
void ScrumBoard::createCompanyWindow() {
    float companyHeight = 55.0f;
    float padding = 15.0f;
    
    float windowWidth = 380.0f;
    float windowHeight = companyHeight * companies.size() + padding * 2;
    
    float startX = (WINDOW_WIDTH - windowWidth) / 2;
    float startY = (WINDOW_HEIGHT - windowHeight) / 2;
    
    companyWindow.setSize(sf::Vector2f(windowWidth, windowHeight));
    companyWindow.setFillColor(sf::Color(180, 210, 235));
    companyWindow.setOutlineColor(sf::Color(160, 190, 220));
    companyWindow.setOutlineThickness(3);
    companyWindow.setPosition(startX, startY);
    
    companyRects.clear();
    companyTexts.clear();
    
    float companyWidth = windowWidth - padding * 2;
    
    //создание кнопок компаний
    for (size_t i = 0; i < companies.size(); i++) {
        sf::RectangleShape companyRect;
        companyRect.setSize(sf::Vector2f(companyWidth, companyHeight));
        companyRect.setFillColor(sf::Color(180, 210, 235));
        companyRect.setOutlineColor(sf::Color(100, 130, 160));
        companyRect.setOutlineThickness(3);
        companyRect.setPosition(startX + padding, startY + padding + i * companyHeight);
        companyRects.push_back(companyRect);
        
        sf::Text companyText;
        companyText.setString(companies[i]);
        companyText.setFont(font);
        companyText.setCharacterSize(24);
        companyText.setFillColor(sf::Color(50, 50, 80));
        companyText.setStyle(sf::Text::Bold);
        
        centerText(companyText, companyRect, -3.0f);
        companyTexts.push_back(companyText);
    }
}

//создание секций
void ScrumBoard::createSections() {
    sf::Color sectionColors[] = {
        sf::Color(180, 210, 235),
        sf::Color(160, 195, 225),
        sf::Color(140, 180, 215),
        sf::Color(120, 165, 205)
    };
    
    float totalWidth = 1820.0f;
    float sectionWidth = (totalWidth - 120.0f) / 4.0f;
    float spacing = 40.0f;
    float startX = 50.0f;
    
    for (int i = 0; i < 4; i++) {
        sf::RectangleShape section;
        section.setSize(sf::Vector2f(sectionWidth, 700));
        section.setFillColor(sectionColors[i]);
        section.setOutlineThickness(0);
        section.setPosition(startX + i * (sectionWidth + spacing), 240);
        sections.push_back(section);
        
        sf::Text text;
        text.setString(sectionNames[i]);
        text.setFont(font);
        text.setCharacterSize(32);
        text.setFillColor(sf::Color(50, 50, 80));
        text.setStyle(sf::Text::Bold);
        
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            startX + i * (sectionWidth + spacing) + (sectionWidth - textBounds.width) / 2, 
            200
        );
        sectionTexts.push_back(text);
    }
}

//создание примеров задач
void ScrumBoard::createSampleTasks() {
    for (int i = 0; i < 4; i++) {
        tasks[i].clear();
    }
    
    addTask("Получение презентации", 0);
    addTask("Просмотр презентации", 0);
    addTask("Выбор темы", 0);
    addTask("Особые описания", 0);
    addTask("Оптимизирование", 0);
    
    addTask("Прослушивание лекции", 1);
    addTask("Начало написания кода", 1);
    addTask("Блок 1 текста готов", 1);
    
    addTask("Контроль кода", 2);
    addTask("Обработка", 2);
    
    addTask("Проект", 3);
    addTask("Инструменты сборки", 3);
    addTask("Перетестировать по порядку устройства", 3);
    
    updateTaskPositions();
}

//добавление задачи
void ScrumBoard::addTask(const std::string& taskName, int section) {
    if (section >= 0 && section < 4) {
        float totalWidth = 1820.0f;
        float sectionWidth = (totalWidth - 120.0f) / 4.0f;
        float spacing = 40.0f;
        float startX = 50.0f;
        float taskWidth = sectionWidth - 30.0f;
        
        float x = startX + section * (sectionWidth + spacing) + (sectionWidth - taskWidth) / 2;
        float y = 270 + tasks[section].size() * 90 - scrollOffsets[section];
        
        Task newTask(taskName, font, x, y);
        newTask.currentSection = section;
        newTask.shape.setSize(sf::Vector2f(taskWidth, 85));
        tasks[section].push_back(newTask);
    }
}

//обновление позиций задач
void ScrumBoard::updateTaskPositions() {
    float totalWidth = 1820.0f;
    float sectionWidth = (totalWidth - 120.0f) / 4.0f;
    float spacing = 40.0f;
    float startX = 50.0f;
    float taskWidth = sectionWidth - 30.0f;
    
    for (int section = 0; section < 4; section++) {
        float startY = 270.0f;
        for (size_t i = 0; i < tasks[section].size(); i++) {
            float x = startX + section * (sectionWidth + spacing) + (sectionWidth - taskWidth) / 2;
            float y = startY + i * 95 - scrollOffsets[section];
            tasks[section][i].setPosition(x, y);
            tasks[section][i].shape.setSize(sf::Vector2f(taskWidth, 85));
        }
    }
}

//обработка событий
void ScrumBoard::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    //проверка нажатия левой кнопки мышки
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            
            //проверка кнопки компаний
            if (companyButton.getGlobalBounds().contains(mousePos)) {
                showCompanyWindow = !showCompanyWindow;
                companyButtonPressed = true;
                return;
            }
            
            //проверка кнопки сохранить
            if (saveButton.getGlobalBounds().contains(mousePos)) {
                saveButtonPressed = true;
                std::cout << "Кнопка 'Сохранить' нажата!" << std::endl;
                return;
            }
            
            //выбор нужной компании
            if (showCompanyWindow) {
                for (size_t i = 0; i < companyRects.size(); i++) {
                    if (companyRects[i].getGlobalBounds().contains(mousePos)) {
                        std::cout << "Выбрана компания: " << companies[i] << std::endl;
                        showCompanyWindow = false;
                        return;
                    }
                }
                //закрытие окна, если нажал вне него
                if (!companyWindow.getGlobalBounds().contains(mousePos)) {
                    showCompanyWindow = false;
                }
            }
            
            //перетаскивание задач
            for (int i = 0; i < 4; i++) {
                for (size_t j = 0; j < tasks[i].size(); j++) {
                    if (tasks[i][j].shape.getGlobalBounds().contains(mousePos)) {
                        tasks[i][j].isMoving = true;
                        draggingTaskSection = i;
                        draggingTaskIndex = j;
                        return;
                    }
                }
            }
            
            //проверка нажатия на секцию для прокрутки
            for (int i = 0; i < 4; i++) {
                if (sections[i].getGlobalBounds().contains(mousePos)) {
                    isDraggingScroll[i] = true;
                    dragStartPositions[i] = mousePos;
                    break;
                }
            }
        }
    }
    
    //отпускание мышки
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            saveButtonPressed = false;
            companyButtonPressed = false;
            
            if (draggingTaskSection != -1 && draggingTaskIndex != -1) {
                Task& draggedTask = tasks[draggingTaskSection][draggingTaskIndex];
                draggedTask.isMoving = false;
                
                //центр перетаскиваемой задачи
                sf::FloatRect taskBounds = draggedTask.shape.getGlobalBounds();
                sf::Vector2f taskCenter(
                    taskBounds.left + taskBounds.width / 2,
                    taskBounds.top + taskBounds.height / 2
                );
                
                //определение новой секции
                for (int newSection = 0; newSection < 4; newSection++) {
                    if (sections[newSection].getGlobalBounds().contains(taskCenter)) {
                        if (newSection != draggingTaskSection) {
                            Task movedTask = draggedTask;
                            movedTask.currentSection = newSection;
                            movedTask.isMoving = false;
                            tasks[newSection].push_back(movedTask);
                            
                            tasks[draggingTaskSection].erase(tasks[draggingTaskSection].begin() + draggingTaskIndex);
                            
                            scrollOffsets[draggingTaskSection] = 0.0f;
                            scrollOffsets[newSection] = 0.0f;
                            
                            updateTaskPositions();
                            break;
                        } else {
                            updateTaskPositions();
                        }
                    }
                }
                
                draggingTaskSection = -1;
                draggingTaskIndex = -1;
            }
            
            for (int i = 0; i < 4; i++) {
                isDraggingScroll[i] = false;
            }
        }
    }
    
    //движение мыши
    if (event.type == sf::Event::MouseMoved) {
        //перетаскивание задач
        if (draggingTaskSection != -1 && draggingTaskIndex != -1) {
            Task& draggedTask = tasks[draggingTaskSection][draggingTaskIndex];
            if (draggedTask.isMoving) {
                draggedTask.setPosition(event.mouseMove.x - 190, event.mouseMove.y - 40);
            }
        }
        
        //прокрутка секций
        for (int i = 0; i < 4; i++) {
            if (isDraggingScroll[i]) {
                sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
                float deltaY = dragStartPositions[i].y - mousePos.y;
                float maxScroll = std::max(0.0f, (tasks[i].size() * 90.0f) - 650.0f);
                
                scrollOffsets[i] += deltaY;
                scrollOffsets[i] = std::max(0.0f, std::min(scrollOffsets[i], maxScroll));
                dragStartPositions[i] = mousePos;
                updateTaskPositions();
            }
        }
    }
    
    //прокрутка колесиком мыши
    if (event.type == sf::Event::MouseWheelScrolled) {
        float mouseX = event.mouseWheelScroll.x;
        
        //определение в какой секции находится мышь
        for (int i = 0; i < 4; i++) {
            if (sections[i].getGlobalBounds().contains(mouseX, event.mouseWheelScroll.y)) {
                float scrollDelta = -event.mouseWheelScroll.delta * 30.0f;
                float maxScroll = std::max(0.0f, (tasks[i].size() * 90.0f) - 650.0f);
                
                scrollOffsets[i] += scrollDelta;
                scrollOffsets[i] = std::max(0.0f, std::min(scrollOffsets[i], maxScroll));
                updateTaskPositions();
                break;
            }
        }
    }
}

//обновление
void ScrumBoard::update(float deltaTime) {
    //визуальная обратная связь для кнопки сохранить
    if (saveButtonPressed) {
        saveButton.setFillColor(sf::Color(150, 190, 220));
    } else {
        saveButton.setFillColor(sf::Color(180, 210, 235));
    }
    
    //визуальная обратная связь для кнопки компаний
    if (companyButtonPressed) {
        companyButton.setFillColor(sf::Color(150, 190, 220));
    } else {
        companyButton.setFillColor(sf::Color(180, 210, 235));
    }
}

//рисование
void ScrumBoard::draw(sf::RenderWindow& window) {
    window.draw(titleText);
    
    window.draw(topPanel);
    window.draw(companyButton);
    window.draw(companyButtonText);
    window.draw(saveButton);
    window.draw(saveButtonText);
    
    //рисуем секции
    for (const auto& section : sections) {
        window.draw(section);
    }
    
    //рисуем заголовки секций
    for (const auto& text : sectionTexts) {
        window.draw(text);
    }
    
    //рисуем задачи
    for (int i = 0; i < 4; i++) {
        sf::FloatRect sectionBounds = sections[i].getGlobalBounds();
        
        for (const auto& task : tasks[i]) {
            sf::FloatRect taskBounds = task.shape.getGlobalBounds();
            
            //проверка видимости задачи в секции
            if (taskBounds.top + taskBounds.height >= sectionBounds.top && 
                taskBounds.top <= sectionBounds.top + sectionBounds.height) {
                window.draw(task.shape);
                window.draw(task.text);
            }
        }
    }
    
    //рисуем окно компаний
    if (showCompanyWindow) {
        //затемнение фона под окном
        sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
        
        window.draw(companyWindow);
        
        //рисуем прямоугольники компаний
        for (const auto& rect : companyRects) {
            window.draw(rect);
        }
        
        //рисуем текст компаний
        for (const auto& text : companyTexts) {
            window.draw(text);
        }
    }
}