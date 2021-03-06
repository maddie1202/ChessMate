//

const Board = require("../models/board.model.js");

//create a new board in table
exports.create = (req,res) => {
    //validate request
    if (!req.body.placements) {
        res.status(400).send({
          message: "Content can not be empty!"
        });

      }

      // Create a Board
      const newBoard = new Board({
        placements: req.body.placements,
        gameID: req.body.gameID,
        sequenceNum : req.body.sequenceNum,
        wrookO_moved : req.body.wrookO_moved,
        wrookI_moved : req.body.wrookI_moved,
        brookO_moved : req.body.brookO_moved,
        brookI_moved : req.body.brookI_moved,
        wking_moved : req.body.wking_moved,
        bking_moved : req.body.bking_moved
      });

      // Save Board in the database
      Board.create(newBoard, (err, data) => {
        if (err)
          res.status(500).send({
            message:
              err.message || "Some error occurred while creating the Board."
          });
        else res.send(data);
      });
};

//find board by boardID
exports.findOne = (req,res) => {

    const boardID = req.params.boardID;
    console.log("boardID = " + boardID + "in controller");
    Board.findById(boardID, (err, data) => {
        if (err) {
              if (err.kind === "not_found") {
                res.status(404).send({
                  message: `Not found Board with boardID ${boardID}.`
                });
              } else {
                res.status(500).send({
                  message: "Error retrieving Board with boardID " + boardID
                });
              }
        } else res.send(data);
    });
};


//Retrive latest board with gameID
//app.get("/getlatestboard/:gameID", board.findLatest);
exports.findLatest = (req,res) => {

    const gameID = req.params.gameID;
    Board.findLatest(gameID, (err, data) => {
        if (err) {
              if (err.kind === "not_found") {
                res.status(404).send({
                  message: `Not found latest Board with gameID ${gameID}.`
                });
              } else {
                res.status(500).send({
                  message: "Error retrieving latest Board with gameID " + gameID
                });
              }
        } else res.send(data);
    });
};


//find all boards with gameID
exports.findAll = (req,res) => {

    const gameID = req.params.gameID;

    Board.getAll(gameID, (err, data) => {
        if (err)
          res.status(500).send({
            message:
              err.message || "Some error occurred while retrieving boards."
          });
        else res.send(data);
    });
};

//update board with boardID and board details
exports.update = (req,res) => {
      // Validate Request
      if (!req.body) {
        res.status(400).send({
          message: "Content can not be empty!"
        });
      }

      const boardID = req.body.boardID;
      // Create a Board
      const board = new Board({
          boardID: req.body.boardID,
          placements: req.body.placements,
          gameID: req.body.gameID,
          sequenceNum: req.body.sequenceNum
      });

      Board.updateById(boardID, board, (err, data) => {
        if (err) {
            if (err.kind === "not_found") {
              res.status(404).send({
                message: `Not found Board with boardID ${boardID}.`
              });
            } else {
              res.status(500).send({
                message: "Error updating Board with boardID " + boardID
              });
            }
          } else res.send(data);
      });
};

//remove a board using boardID
exports.delete = (req,res) => {
    const boardID = req.params.boardID;

    Board.remove(boardID, (err, data) => {
        if (err) {
              if (err.kind === "not_found") {
                res.status(404).send({
                  message: `Not found Board with boardID ${boardID}.`
                });
              } else {
                res.status(500).send({
                  message: "Could not delete Board with boardID " + boardID
                });
              }
        } else res.send({ message: `Board was deleted successfully!` });
    });
};

//remove all boards with gameID
exports.deleteAll = (req,res) => {
    const gameID = req.params.gameID;
    Board.removeAll(gameID, (err, data) => {
        if (err)
              res.status(500).send({
                message:
                  err.message || "Some error occurred while removing all boards for the game."
              });
            else res.send({ message: `All Boards with gameID ${gameID} were deleted successfully!` });
    });
};





