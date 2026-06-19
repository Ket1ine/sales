#include <QApplication>
#include <QMessageBox>
#include "model/database.h"
#include "view/loginwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Реализация готовой продукции");

    if (!Database::instance().connect("localhost", 5432, "product_sales", "postgres", "")) {
        QMessageBox::critical(nullptr, "Ошибка БД",
            "Не удалось подключиться к базе данных PostgreSQL.\n"
            "Проверьте, что сервер запущен и база product_sales создана.");
        return 1;
    }

    LoginWindow loginWindow;
    loginWindow.show();

    return app.exec();
}
