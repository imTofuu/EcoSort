const expressapp = require("express")();
expressapp.use(require("express").json());
const { Pool } = require("pg");
require("dotenv").config();

const pool = new Pool({
    url: process.env.DATABASE_URL,
});

expressapp.get("/api/ping", (req, res) => {
    res.status(200).json({ message: "pong" });
});