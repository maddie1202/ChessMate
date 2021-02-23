const express = require("express");
const bodyParser = require("body-parser");

const app = express();

// parse requests of content-type: application/json
app.use(bodyParser.json());

// parse requests of content-type: application/x-www-form-urlencoded
app.use(bodyParser.urlencoded({ extended: true }));

// simple route
app.get("/", (req, res) => {
  res.json({ message: "Welcome to ChessMate." });
});

// set port, listen for requests
app.listen(3000, () => {
  console.log("Server is running on port 3000.");
});

/*
What we do are:
– import express and body-parser modules. 
- Express is for building the Rest apis, and 
- body-parser helps to parse the request and create the req.body object that we will need to access in our routes.
– create an Express app, then add body-parser middlewares using app.use() method.
– define a GET route which is simple for test.
– listen on port 3000 for incoming requests.
*/