-- Инициализация базы данных product_sales

CREATE TABLE Роли (
    id_роли SERIAL PRIMARY KEY,
    Наименование_роли VARCHAR(100) NOT NULL UNIQUE
);

CREATE TABLE Пользователи (
    id_пользователя SERIAL PRIMARY KEY,
    Логин VARCHAR(100) NOT NULL UNIQUE,
    Хеш_пароля VARCHAR(255) NOT NULL,
    id_роли INT NOT NULL REFERENCES Роли(id_роли)
);

CREATE TABLE Покупатели (
    id_покупателя SERIAL PRIMARY KEY,
    Наименование VARCHAR(255) NOT NULL,
    Адрес VARCHAR(255),
    Номер_телефона VARCHAR(50),
    Контактное_лицо VARCHAR(255),
    id_пользователя INT UNIQUE REFERENCES Пользователи(id_пользователя) ON DELETE SET NULL
);

CREATE TABLE Товары (
    id_товара SERIAL PRIMARY KEY,
    Наименование VARCHAR(255) NOT NULL,
    Оптовая_цена NUMERIC(10,2) NOT NULL CHECK(Оптовая_цена >= 0),
    Розничная_цена NUMERIC(10,2) NOT NULL CHECK(Розничная_цена >= 0),
    Справочная_информация TEXT
);

CREATE TABLE Скидки (
    id_скидки SERIAL PRIMARY KEY,
    Размер_скидки NUMERIC(5,2) NOT NULL CHECK(Размер_скидки >= 0 AND Размер_скидки <= 100),
    Описание VARCHAR(255)
);

CREATE TABLE Документы (
    id_документа SERIAL PRIMARY KEY,
    Дата_покупки DATE NOT NULL DEFAULT CURRENT_DATE,
    id_покупателя INT NOT NULL REFERENCES Покупатели(id_покупателя),
    id_скидки INT REFERENCES Скидки(id_скидки)
);

CREATE TABLE Розничные_продажи (
    id_документа INT REFERENCES Документы(id_документа) ON DELETE CASCADE,
    id_товара INT REFERENCES Товары(id_товара),
    Количество INT NOT NULL CHECK(Количество > 0),
    PRIMARY KEY(id_документа, id_товара)
);

CREATE TABLE Оптовые_продажи (
    id_документа INT REFERENCES Документы(id_документа) ON DELETE CASCADE,
    id_товара INT REFERENCES Товары(id_товара),
    Количество INT NOT NULL CHECK(Количество > 0),
    PRIMARY KEY(id_документа, id_товара)
);

-- Начальные данные

INSERT INTO Роли VALUES (1, 'Администратор'), (2, 'Пользователь');

INSERT INTO Пользователи (Логин, Хеш_пароля, id_роли)
VALUES ('admin', '3eb3fe66b31e3b4d10fa70b5cad49c7112294af6ae4e476a1c405155d45aa121', 1);

INSERT INTO Товары (Наименование, Оптовая_цена, Розничная_цена, Справочная_информация) VALUES
    ('Ноутбук', 45000.00, 59990.00, 'Ноутбук 15.6", 8GB RAM, 256GB SSD'),
    ('Монитор', 12000.00, 17990.00, 'Монитор 24", IPS, Full HD'),
    ('Клавиатура', 800.00, 1490.00, 'Клавиатура проводная, USB'),
    ('Мышь', 400.00, 790.00, 'Мышь оптическая, USB');

INSERT INTO Покупатели (Наименование, Адрес, Номер_телефона, Контактное_лицо) VALUES
    ('ООО "ТехноСервис"', 'г. Москва, ул. Ленина, д. 10', '+7(495)123-45-67', 'Иванов И.И.'),
    ('АО "КомпьютерПлюс"', 'г. Санкт-Петербург, пр. Невский, д. 25', '+7(812)987-65-43', 'Петров П.П.');

INSERT INTO Скидки (Размер_скидки, Описание) VALUES
    (5.00, 'Скидка для мелких заказов'),
    (10.00, 'Скидка для крупных заказов');
