-- 创建Data表
CREATE TABLE Data (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    date TEXT
);

-- 创建Word表
CREATE TABLE Word (
    id INT PRIMARY KEY,
    data_id INT,
    word TEXT,
    translation TEXT,
    root TEXT,
    sentence TEXT,
    FOREIGN KEY(data_id) REFERENCES Data(id)
);