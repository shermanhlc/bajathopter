-- Add migration script here
CREATE TABLE IF NOT EXISTS sensor_data (
    sensor_id INTEGER check(sensor_id >= 0 AND sensor_id <= 255) NOT NULL,
    voltage REAL NOT NULL,
    timestamp INTEGER NOT NULL,
    PRIMARY KEY (sensor_id, timestamp)
);
