module.exports = app => {

    const board = require("../controllers/board.controller.js");

    //Create a new board
    app.post("/makeboard", board.create);

    //Retrieve all boards with gameId
    app.get("/getgame/:gameID", board.findAll);

    //Retrieve a single Board with boardId
    app.get("/getboard/:boardID", board.findOne);

    //Update a Board with boardId
    app.put("/updateboard/:boardID", board.update);

    //Delete a Board by boardId
    app.delete("/deleteboard/:boardID", board.delete);

    //Delete all Boards with gameId
    app.delete("/deletegame/:gameID", board.deleteAll);

}