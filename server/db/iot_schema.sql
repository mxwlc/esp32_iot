CREATE TABLE
    IF NOT EXISTS Devices (
        device_id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
        device_address VARCHAR(100) UNIQUE,
        device_type VARCHAR(50)
    );

CREATE TABLE
    IF NOT EXISTS Sensors (
        sensor_id INTEGER PRIMARY KEY AUTOINCREMENT,
        sensor_address VARCHAR(100) UNIQUE,
        device_address VARCHAR(100),
        sensor_name VARCHAR(100),
        sensor_description VARCHAR(255),
        unit VARCHAR(50),
        FOREIGN KEY (device_address) REFERENCES Devices (device_address)
    );

CREATE TABLE
    IF NOT EXISTS Readings (
        reading_id INTEGER PRIMARY KEY AUTOINCREMENT,
        sensor_address INTEGER,
        data_value FLOAT,
        recorded_at TIMESTAMP DEFAULT current_timestamp,
        FOREIGN KEY (sensor_address) REFERENCES Sensors (sensor_address)
    );