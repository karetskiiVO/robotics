#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    SetupVisuals();
    SetupConnections();
    ReadPackets();
    AdjustVisuals();
    DrawPackets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Setups the plot additional graphics (i.e. axis labels)
void MainWindow::SetupVisuals()
{
    ui->plot->xAxis->setLabel("Time");
    ui->plot->yAxis->setLabel("Frequency");

    ui->plot->axisRect()->setAutoMargins(QCP::msNone); // Disable automatic margins
    ui->plot->axisRect()->setMargins(QMargins(50, 50, 50, 50)); // Set margins manually
}

void MainWindow::SetupConnections()
{
    connect(ui->loadFileButton, &QPushButton::clicked, this, &MainWindow::ReadPackets);
    connect(ui->plot, &QCustomPlot::mouseMove, this, [this](QMouseEvent *event) {
        double x = ui->plot->xAxis->pixelToCoord(event->pos().x());
        double y = ui->plot->yAxis->pixelToCoord(event->pos().y());
        CheckHover(QPointF(x, y));
    });
}

// Reads the data needed to be displayed
// TODO: Make not dummy version, that reads real data given by other modules of application
// TODO: Decide whether it should be real-time evaluated or fixed in time
void MainWindow::ReadPackets()
{
    // Выбор файла с помощью диалогового окна
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Открыть файл"), 
        "", // Начальная директория (пустая - текущая)
        tr("Текстовые файлы (*.txt);;Все файлы (*)"));
    
    if (fileName.isEmpty())
        return; // Пользователь отменил выбор
    
    QFile file(fileName);
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }
    
    // Чтение содержимого файла
    QTextStream in(&file);
    QString fileContent = in.readAll();
    file.close();
    
    ParsePackets(fileContent.toStdString());
}

void MainWindow::ParsePackets(const std::string& input)
{
    std::istringstream stream(input);
    std::string line;
    Packet current_packet;
    bool in_packet = false;

    while (std::getline(stream, line)) {
        // Удаляем лишние пробелы в начале и конце строки
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty()) {
            continue;  // Пропускаем пустые строки
        }

        if (line == "Packet") {
            // Если мы уже внутри пакета, сохраняем предыдущий
            if (in_packet) {
                packets_.push_back(current_packet);
            }
            in_packet = true;
            current_packet = Packet();  // Сбрасываем текущий пакет
            continue;
        }

        if (!in_packet) {
            continue;  // Пропускаем строки вне блока Packet
        }

        // Разбираем строки вида "ключ: значение"
        size_t colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string key = line.substr(0, colon_pos);
            std::string value_str = line.substr(colon_pos + 1);
            
            // Удаляем пробелы вокруг значения
            value_str.erase(0, value_str.find_first_not_of(" \t"));
            value_str.erase(value_str.find_last_not_of(" \t") + 1);

            try {
                if (key == "id") {
                    current_packet.header.id = std::stoi(value_str);
                } else if (key == "timepos") {
                    current_packet.header.timepos = MeasureUnits::Time(std::stoi(value_str));
                } else if (key == "carrier_frequency") {
                    current_packet.header.carrier_frequency = MeasureUnits::Frequency(std::stoi(value_str));
                } else if (key == "clock_frequency") {
                    current_packet.header.clock_frequency = MeasureUnits::Frequency(std::stoi(value_str));
                } else if (key == "power") {
                    current_packet.header.power = std::stod(value_str);
                } else if (key == "size") {
                    current_packet.header.size = std::stoi(value_str);
                }
            } catch (const std::exception& e) {
                // Обработка ошибок преобразования типов
                throw std::runtime_error("Failed to parse inputed file: invlalid syntax\n");
            }
        }
    }

    // Добавляем последний пакет, если он есть
    if (in_packet) {
        packets_.push_back(current_packet);
    }
}

void MainWindow::CheckHover(const QPointF &pos)
{
    bool hovered = false;
    
    // Проверяем все прямоугольники
    for (size_t i = 0; i < rectangles_.size(); ++i) {
        auto *rect = rectangles_[i];
        QRectF rectArea(rect->topLeft->coords().x(),
                       rect->topLeft->coords().y(),
                       rect->bottomRight->coords().x() - rect->topLeft->coords().x(),
                       rect->bottomRight->coords().y() - rect->topLeft->coords().y());
        
        if (rectArea.contains(pos)) {
            // Формируем текст подсказки
            QString tooltip = QString::fromStdString(
                "id: " + std::to_string(static_cast<uint64_t>(packets_[i].header.id)) + 
                "\ntimepos: " + std::to_string(packets_[i].header.timepos.asNanoseconds()) +
                "\ncarrier_frequency: " + std::to_string(packets_[i].header.carrier_frequency.asHertz()) +
                "\nclock_frequency: " + std::to_string(packets_[i].header.clock_frequency.asHertz()) +
                "\npower: " + std::to_string(packets_[i].header.power.asMilliwatt()) + 
                "\nsize: " + std::to_string(packets_[i].header.size) +
                "\nduration: " + std::to_string(packets_[i].PacketDuration().asNanoseconds()) +
                '\n'
            );
            
            // Показываем подсказку
            QToolTip::showText(QCursor::pos(), tooltip, ui->plot);
            
            hovered = true;
        }
    }
    
    // Если курсор не над прямоугольниками - скрываем подсказку
    if (!hovered) {
        QToolTip::hideText();
    }
    
    ui->plot->setCursor(hovered ? Qt::PointingHandCursor : Qt::ArrowCursor);
    ui->plot->replot();
}

// Adjust visuals to correctly display all packets
void MainWindow::AdjustVisuals() {
    // Adjust label sizes to draw packets
    MeasureUnits::Duration max_time(0);
    MeasureUnits::Frequency max_freq(0);

    for (size_t i = 0; i < packets_.size(); ++i)
    {
        max_time = std::max(max_time, packets_[i].PacketDuration());
        max_freq = std::max(max_freq, packets_[i].header.carrier_frequency + packets_[i].header.clock_frequency);
    }

    ui->plot->xAxis->setRange(0, max_time.asNanoseconds() + 1);
    ui->plot->yAxis->setRange(0, max_freq.asHertz() + 1);
}

// Draws packets on plot
// TODO: Decide a way to distinguish packets so that they could be differed from one another
// (requires overall paket view model)
void MainWindow::DrawPackets()
{
    rectangles_.clear();

    for (size_t i = 0; i < packets_.size(); ++i)
    {
        // Create a rectangle item
        QCPItemRect* rect = new QCPItemRect(ui->plot);
        rectangles_.push_back(rect);

        // Set the rectangle's position and size
        // Top-left corner
        rect->topLeft->setCoords(0, (packets_[i].header.carrier_frequency + packets_[i].header.clock_frequency).asHertz());
        // Bottom-right corner
        rect->bottomRight->setCoords(packets_[i].PacketDuration().asNanoseconds(), 
                                    (packets_[i].header.carrier_frequency - packets_[i].header.clock_frequency).asHertz()); 

        // Customize the rectangle's appearance
        rect->setPen(QPen(Qt::blue)); // Border color
        rect->setBrush(QBrush(QColor(0, 0, 255, 50))); // Fill color with transparency
    }

    ui->plot->replot();
}