const Game = require("../models/Game.model.js");

exports.findOne = (req,res) => {

    const gameID = req.params.gameID;

    console.log("gameID = " + gameID + " in controller");
    
    Game.findById(gameID, (err, data) => {
        if (err) {
              if (err.kind === "not_found") {
                res.status(404).send({
                  message: `Not found Game with gameID ${gameID}.`
                });
              } else {
                res.status(500).send({
                  message: "Error retrieving Game with gameID " + gameID
                });
              }
        } else res.send(data);
    });
};

exports.findAll = (req,res) => {

    const userID = req.params.userID;

    Game.getAll(userID, (err, data) => {
        if (err)
          res.status(500).send({
            message:
              err.message || "Some error occurred while retrieving games."
          });
        else res.send(data);
    });
};