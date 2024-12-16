use actix_web::{
    get, post,
    web::{self, Json},
    App, HttpResponse, HttpServer, Responder,
};

use chrono::NaiveDateTime;
use serde::{Deserialize, Serialize};
use sqlx::{error::Error, prelude::FromRow, query_builder, sqlite::SqlitePoolOptions, SqlitePool};

#[derive(Deserialize, Debug)]
struct QueryParams {
    page: Option<u32>,
}

#[derive(Deserialize, Serialize, Debug)]
struct JsonArray {
    pub voltages: Vec<JsonData>,
}

#[derive(Deserialize, Serialize, Debug, FromRow)]
struct JsonData {
    pub sensor_id: u8,
    #[sqlx(rename = "voltage")]
    pub sensor_voltage: f32,
    pub timestamp: i64,
}

#[get("/")]
async fn index() -> impl Responder {
    "Hello, World"
}

async fn get_sensor_data(pool: &SqlitePool, page: u32) -> Result<Vec<JsonData>, Error> {
    sqlx::query_as::<_, JsonData>(
        r#"SELECT * FROM sensor_data ORDER BY timestamp ASC LIMIT 25 OFFSET ?"#,
    )
    .bind(page)
    .fetch_all(pool)
    .await
}

async fn upload_sensor_data(pool: &SqlitePool, data: &[JsonData]) -> Result<(), Error> {
    let mut qb = sqlx::query_builder::QueryBuilder::new(
        "INSERT INTO sensor_data (sensor_id, voltage, timestamp) ",
    );
    qb.push_values(data, |mut b, sensor| {
        b.push_bind(sensor.sensor_id)
            .push_bind(sensor.sensor_voltage)
            .push_bind(sensor.timestamp);
    })
    .build()
    .execute(pool)
    .await
    .map(|_| ())
}

fn render_table(sensor_data: &[JsonData]) -> String {
    let mut table = String::from(
        r#"<table border="1"><tr><th>Sensor ID</th><th>Voltage</th><th>Timestamp</th></tr>"#,
    );
    for sensor in sensor_data {
        let datetime = NaiveDateTime::from_timestamp_micros(sensor.timestamp).unwrap();
        let formatted_time = datetime.format("%Y-%m-%d %H:%M:%S").to_string();
        table.push_str(&format!(
            r#"<tr><td>{}</td><td>{:.2}</td><td>{}</td></tr>"#,
            sensor.sensor_id, sensor.sensor_voltage, formatted_time
        ));
    }
    table.push_str("</table>");

    table
}

#[post("/sensor")]
async fn handle_post(
    json_data: web::Json<JsonArray>,
    pool: web::Data<SqlitePool>,
) -> impl Responder {
    let data = json_data.into_inner().voltages;
    upload_sensor_data(&pool.into_inner(), &data).await.unwrap();
    "Received"
}

#[get("/sensor")]
async fn get_sensors(
    query: web::Query<QueryParams>,
    pool: web::Data<SqlitePool>,
) -> impl Responder {
    let page = query.page.unwrap_or(0);
    let data = get_sensor_data(&pool.into_inner(), page).await.unwrap();
    let table = render_table(&data);
    let value = format!(
        "<html><head><title>Sensor Data</title></head><body><h1>Sensor Data</h1>{}</body></html>",
        table
    );

    HttpResponse::Ok().content_type("text/html").body(value)
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    let pool = SqlitePoolOptions::new()
        .connect("sqlite:database.db")
        .await
        .unwrap();
    HttpServer::new(move || {
        App::new()
            .service(index)
            .service(handle_post)
            .service(get_sensors)
            .app_data(web::Data::new(pool.clone()))
    })
    .bind(("0.0.0.0", 80))?
    .run()
    .await
}
