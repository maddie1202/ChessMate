module.exports = app => {

    const move = require("../controllers/Move.controller.js");

    //Retrieve all possible moves
    app.get("/getallmoves", move.findAll);

    //Add a possible move
    app.post("/createonemove", move.createOne);

    //delete all possible moves
    app.delete("/deleteallmoves", move.deleteAll);

}
