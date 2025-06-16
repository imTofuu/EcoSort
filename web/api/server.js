// Handles REST
const expressapp = require("express")();
expressapp.use(require("express").json()); // Parses JSON bodies

// Handles database queries
const {PrismaClient} = require("@prisma/client");

// Gets environment variables; set in docker or coolify
require("dotenv").config();

const pc = new PrismaClient();

// Used to test if the server is reachable.
expressapp.get("/ping", (req, res) => {
    res.status(200).send({message: "pong"});
});

expressapp.post("/addscore", async (req, res) => {

    if (!req.body.name) {
        return res.status(400).send({
            status: 400,
            error: "Name is required"
        });
    }

    // Get the user with the name passed if it exists.
    let user = await pc.user.findUnique({
        where: {
            name: req.body.name
        }
    }).catch((err) => {
        return res.status(500).send({
            status: 500,
            error: err
        });
    });

    // Create a new user if it does not exist.
    if (!user) {
        user = await pc.user.create({
            data: {
                name: req.body.name
            }
        }).catch((err) => {
            return res.status(500).send({
                status: 500,
                error: err
            });
        });
    }

    // Create a new score record for the user with the score passed in the request body, and send an error on fail.
    const scoreRecord = await pc.score.create({
        data: {
            userName: user["name"],
            score: req.body.score
        }
    }).catch((err) => {
        return res.status(500).send({
            status: 500,
            error: err
        });
    });

    // Return the record created on success.
    res.status(200).send({
        status: 200,
        score: scoreRecord
    });
});

expressapp.get("/getleaderboard", async (req, res) => {
    // Synchronously get all the scores from the database where the username contains the string in the search
    // parameter, passed from the url (e.g. /getleaderboard?search=example). Send code 500 (internal error) if the query
    // fails for some reason.
    const scores = await pc.score.findMany({
        where: {
            userName: {
                contains: req.query.search || "",
                mode: "insensitive"
            }
        }
    }).catch((err) => {
        return res.status(500).send({
            status: 500,
            error: err
        })
    });

    res.status(200).send({
        status: 200,
        scores: scores
    });
});

// Start the server on port 3000, which is mapped as port 3000 on the host in the docker-compose and coolify.
expressapp.listen(3000, () => {
    console.log("Server is running on port 3000");
});