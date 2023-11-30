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
    translation TEXT,
    root TEXT,
    sentence TEXT,
    FOREIGN KEY(data_id) REFERENCES Data(id)
);