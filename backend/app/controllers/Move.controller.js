
const Move = require("../models/Move.model.js");

// Retrive all possible moves
//app.get("/getallmoves", move.findAll);
exports.findAll = (req,res) => {

    Move.getAll((err, data) => {
        if (err)
          res.status(500).send({
            message:
              err.message || "Some error occurred while retrieving moves."
          });
        else res.send(data);
    });
};

//Add a possible move
//app.post("/createmoves", move.createAll);
exports.createAll = (req,res) => {
    //validate request
    if (req.body.placements == null) {
        res.status(400).send({
          message: "Content can not be empty!"
        });
    }

    const moves = req.body.placements;
    const move = moves.split("~");
    // Save Move in the database
    Move.create(move, (err, data) => {
    if (err)
      res.status(500).send({
        message:
          err.message || "Some error occurred while creating the Moves."
      });
    else res.send(data);
    });
};


//delete all possible moves
//app.delete("/deleteallmoves", move.deleteAll);
exports.deleteAll = (req,res) => {

    Move.removeAll((err, data) => {
        if (err)
              res.status(500).send({
                message:
                  err.message || "Some error occurred while removing all possible moves."
              });
            else res.send({ message: `All Moves were deleted successfully!` });
    });
};
