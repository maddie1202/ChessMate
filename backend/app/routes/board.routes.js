
module.exports = app => {

    const board = require("../controllers/board.controller.js");

    //Create a new board
    app.post("/makeboard", board.create);

    //Retrieve all boards with gameId
    app.get("/getgame/:gameId", board.findAll);

    //Retrieve a single Board with boardId
    app.get("/getboard/:boardID", board.findOne);

    //Update a Board with boardId
    app.put("/updateboard/:boardId", board.update);

    //Delete a Board by boardId
    app.delete("/deleteboard/:boardId", board.delete);

    //Delete all Boards with gameId
    app.delete("/deletegame/:gameId", board.deleteAll);

}