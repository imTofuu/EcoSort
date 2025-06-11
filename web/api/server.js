const expressapp = require("express")();
expressapp.use(require("express").json());
const { Pool } = require("pg");
require("dotenv").config();

const pool = new Pool({
    url: process.env.DATABASE_URL,
});

expressapp.get("ping", (req, res) => {
    res.status(200).json({ message: "pong" });
});

expressapp.listen(3000, () => {
    console.log("Server is running on port 3000");
})