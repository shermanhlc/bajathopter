use actix_web::{
    get, post,
    web::{self, Json},
    App, HttpServer, Responder,
};

use serde::{Deserialize, Serialize};

#[derive(Deserialize, Serialize, Debug)]
struct JsonArray {
    pub voltages: Vec<JsonData>,
}

#[derive(Deserialize, Serialize, Debug)]
struct JsonData {
    pub sensor_id: u8,
    pub sensor_voltage: f32,
    pub timestamp: i64,
}

#[get("/")]
async fn index() -> impl Responder {
    "Hello, World"
}

#[post("/sensor")]
async fn handle_post(json_data: web::Json<JsonArray>) -> impl Responder {
    println!("Recieved data");
    println!("{:#?}", json_data);
    dbg!(json_data.into_inner());
    "Received"
}

#[actix_web::main]
async fn main() -> std::io::Result<()> {
    HttpServer::new(|| App::new().service(index).service(handle_post))
        .bind(("0.0.0.0", 80))?
        .run()
        .await
}
