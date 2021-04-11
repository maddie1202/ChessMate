module.exports = app => {

    const board = require("../controllers/board.controller.js");

    //Retrieve all boards with gameId
    app.get("/getgame/:gameID", board.findAll);

    //Retrieve a single Board with boardId
    app.get("/getboard/:boardID", board.findOne);

    //Retrive latest board with gameID
    app.get("/getlatestboard/:gameID", board.findLatest);

    //insert a new board
    app.post("/makeboard", board.create);

    //Update a Board with boardId
    app.put("/updateboard", board.update);

    //Delete a Board by boardId
    app.delete("/deleteboard/:boardID", board.delete);

    //Delete all Boards with gameId
    app.delete("/deletegame/:gameID", board.deleteAll);

}