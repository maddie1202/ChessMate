
module.exports = app => {

    const boards = require("../controllers/board.controller.js");

    //Create a new board
    app.post("/boards", boards.create);

    //Retrieve all boards with gameId
    app.get("/games/:gameId", boards.findAll);

    //Retrieve a single Board with boardId
    app.get("/boards/:boardId", boards.findOne);

    //Update a Board with boardId
    app.put("/boards/:boardId", boards.update);

    //Delete a Board by boardId
    app.delete("/boards/:boardId", boards.delete);

    //Delete all Boards with gameId
    app.delete("/games/:gameId", boards.deleteAll);

}