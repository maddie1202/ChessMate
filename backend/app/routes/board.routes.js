
module.exports = app => {

    const board = require("../controllers/board.controller.js");

    //Create a new board
    app.post("/board", board.create);

    //Retrieve all boards with gameId
    app.get("/game/:gameId", board.findAll);

    //Retrieve a single Board with boardId
    app.get("/board/:boardId", board.findOne);

    //Update a Board with boardId
    app.put("/board/:boardId", board.update);

    //Delete a Board by boardId
    app.delete("/board/:boardId", board.delete);

    //Delete all Boards with gameId
    app.delete("/game/:gameId", board.deleteAll);

}