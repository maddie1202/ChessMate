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

//create a new game in table
exports.create = (req,res) => {
    //validate request
    if (!req.body.difficulty) {
        res.status(400).send({
          message: "Content can not be empty!"
        });

      }

      // Create a Game
      const newGame = new Game({
        difficulty: req.body.difficulty,
        timeleft: req.body.timeleft
      });

      // Save Game in the database
      Game.create(newGame, (err, data) => {
        if (err)
          res.status(500).send({
            message:
              err.message || "Some error occurred while creating the Game."
          });
        else res.send(data);
      });
};

//update game with gameID and game details
exports.update = (req,res) => {
      // Validate Request
      if (!req.body.gameID) {
        res.status(400).send({
          message: "Content can not be empty!"
        });
      }

      const gameID = req.body.gameID;

      // Create a Game
      const game = new Game({
          gameID: req.body.gameID,
          difficulty: req.body.difficulty,
          timeleft: req.body.timeleft
      });

      Game.updateById(gameID, game, (err, data) => {
        if (err) {
            if (err.kind === "not_found") {
              res.status(404).send({
                message: `Not found Game with gameID ${gameID}.`
              });
            } else {
              res.status(500).send({
                message: "Error updating Game with gameID " + gameID
              });
            }
          } else res.send(data);
      });
};

//remove a game using gameID
exports.delete = (req,res) => {
    const gameID = req.params.gameID;

    Game.remove(gameID, (err, data) => {
        if (err) {
              if (err.kind === "not_found") {
                res.status(404).send({
                  message: `Not found Game with gameID ${gameID}.`
                });
              } else {
                res.status(500).send({
                  message: "Could not delete Game with gameID " + gameID
                });
              }
        } else res.send({ message: `Game was deleted successfully!` });
    });
};

//remove all games with userID
exports.deleteAll = (req,res) => {
    const userID = req.params.userID;
    Game.removeAll(userID, (err, data) => {
        if (err)
              res.status(500).send({
                message:
                  err.message || "Some error occurred while removing all games associated with user."
              });
            else res.send({ message: `All Games with userID ${userID} were deleted successfully!` });
    });
};

exports.createResult = (req,res) => {
      // Validate Request
      if (!req.body.gameID) {
        res.status(400).send({
          message: "Content can not be empty!"
        });
      }

      const userID = req.body.userID;
      const gameID = req.body.gameID;
      const result = req.body.result;

      Game.createResult(userID, gameID, result, (err, data) => {
            if (err)
                res.status(500).send({
                  message:
                    err.message || "Some error occurred while creating the Result."
                });
            else res.send(data);
      });
};

exports.updateResult = (req,res) => {
    // Validate Request
          if (!req.body.gameID) {
            res.status(400).send({
              message: "Content can not be empty!"
            });
          }


          const userID = req.body.userID;
          const gameID = req.body.gameID;
          const result = req.body.result;

          Game.updateResult(userID, gameID, result, (err, data) => {
            if (err) {
                if (err.kind === "not_found") {
                  res.status(404).send({
                    message: `Not found Result with gameID ${gameID} and userID ${userID}.`
                  });
                } else {
                  res.status(500).send({
                    message: "Error updating Result with userID " + userID + " and gameID " + gameID
                  });
                }
              } else res.send(data);
          });
};
