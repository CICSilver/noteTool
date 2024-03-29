-- 创建Data表
CREATE TABLE Data (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    date TEXT UNIQUE
);

-- 创建Word表
CREATE TABLE Word (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    data_id INT,
    word TEXT UNIQUE,
    FOREIGN KEY(data_id) REFERENCES Data(id) ON DELETE CASCADE
);

-- 创建Translation表
CREATE TABLE Translation (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    word_id INT,
    sub_id INT,
    zh_translation TEXT,
    en_translation TEXT,
    sentence TEXT,
    root TEXT,
    FOREIGN KEY(word_id) REFERENCES Word(id) ON DELETE CASCADE
);